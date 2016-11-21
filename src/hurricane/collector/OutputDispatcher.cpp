#include "hurricane/collector/OutputDispatcher.h"
#include "hurricane/collector/OutputQueue.h"
#include "hurricane/collector/TaskQueue.h"
#include "hurricane/util/StringUtil.h"
#include "hurricane/message/CommandClient.h"
#include "hurricane/message/Command.h"
#include "hurricane/util/NetConnector.h"

namespace hurricane {
namespace collector {

void OutputDispatcher::SetTaskInfos(const std::vector<task::TaskInfo>& taskInfos)
{
    _taskInfos = taskInfos;
}

void OutputDispatcher::Start()
{
//    _randomTaskInfos.clear();
    _thread = std::thread(&OutputDispatcher::MainThread, this);
}

void OutputDispatcher::MainThread()
{
    while ( true ) {
        OutputItem* outputItem = nullptr;
        if ( _queue->Pop(outputItem) ) {
            int taskIndex = outputItem->GetTaskIndex();
            task::TaskInfo taskInfo = _taskInfos[taskIndex];
            for ( const task::PathInfo& pathInfo : taskInfo.GetPaths() ) {
                ProcessPath(taskInfo, pathInfo, outputItem);
            }

            delete outputItem;
            outputItem = nullptr;
        }
    }
}

bool OutputDispatcher::ProcessPath(const task::TaskInfo& taskInfo, const task::PathInfo& path,
        OutputItem* outputItem)
{

    if ( path.GetGroupMethod() == task::PathInfo::GroupMethod::Global ) {
//        hurricane::base::NetAddress destAddress = path.GetDestinationSupervisor();
//        std::string destIdentifier = destAddress.GetHost() + Int2String(destAddress.GetPort());

//        if ( destIdentifier == selfIdentifier ) {
//            int executorIndex = path.GetDestinationExecutorIndex();
//            int boltIndex = executorIndex - _selfSpoutCount;

//            std::shared_ptr<hurricane::collector::TaskQueue> taskQueue = _selfTasks[boltIndex];
//            TaskItem* taskItem = new TaskItem(outputItem->GetTaskIndex(), outputItem->GetTuple());
//            taskQueue->Push(taskItem);

//            return true;
//        }

//        std::map<std::string, message::CommandClient*>::iterator commandClientPair =
//                _commandClients.find(destIdentifier);
//        if ( commandClientPair == _commandClients.end() ) {
//            util::NetConnector* connector = new util::NetConnector(destAddress);
//            message::CommandClient* commandClient = new message::CommandClient(connector);
//            _commandClients.insert({destIdentifier, commandClient});

//            commandClientPair = _commandClients.find(destIdentifier);
//        }

//        // TODO: Add remote connection
    }
    else if ( path.GetGroupMethod() == task::PathInfo::GroupMethod::Random ) {
        int destCount = path.GetDestinationExecutors().size();
        int destIndex = rand() % destCount;

//        std::cout << "Random: ";
        const task::ExecutorPosition& executorPosition = path.GetDestinationExecutors()[destIndex];
//        std::cout << executorPosition.GetSupervisor().GetHost() << ":"
//                << executorPosition.GetSupervisor().GetPort()
//                << "(" << executorPosition.GetExecutorIndex() << ")" << std::endl;

        SendTupleTo(outputItem, executorPosition);
    }
}

void OutputDispatcher::SendTupleTo(OutputItem* outputItem, const task::ExecutorPosition& executorPosition)
{
    hurricane::base::NetAddress destAddress = executorPosition.GetSupervisor();
    std::string destIdentifier = destAddress.GetHost() + ":" + Int2String(destAddress.GetPort());
    std::string selfIdentifier = _selfAddress.GetHost() + ":" + Int2String(_selfAddress.GetPort());

    if ( destIdentifier == selfIdentifier ) {
        int executorIndex = executorPosition.GetExecutorIndex();
        int boltIndex = executorIndex - _selfSpoutCount;

        std::shared_ptr<hurricane::collector::TaskQueue> taskQueue = _selfTasks[boltIndex];
        TaskItem* taskItem = new TaskItem(outputItem->GetTaskIndex(), outputItem->GetTuple());
        taskQueue->Push(taskItem);
    }
    else {
        std::map<std::string, message::CommandClient*>::iterator commandClientPair =
                _commandClients.find(destIdentifier);
        if ( commandClientPair == _commandClients.end() ) {
            util::NetConnector* connector = new util::NetConnector(destAddress);
            message::CommandClient* commandClient = new message::CommandClient(connector);
            _commandClients.insert({destIdentifier, commandClient});

            commandClientPair = _commandClients.find(destIdentifier);
        }

        message::CommandClient* commandClient = commandClientPair->second;
        std::cout << commandClient << std::endl;

        commandClient->GetConnector()->Connect([outputItem, commandClient, destIdentifier, this] {
            hurricane::message::Command command(hurricane::message::Command::Type::SendTuple);

            base::Variants selfAddressVariants;
            _selfAddress.Serialize(selfAddressVariants);

            command.AddArguments(selfAddressVariants);
            // FIXME
            command.AddArguments(outputItem->GetTuple().GetValues());

            commandClient->SendCommand(command,
                [destIdentifier, this](const hurricane::message::Response& response) -> void {
                if ( response.GetStatus() == hurricane::message::Response::Status::Successful ) {
                    std::cout << "Send to " << destIdentifier << " successfully." << std::endl;
                }
                else {
                    std::cout << "Send to " << destIdentifier << " failed." << std::endl;
                }
            });
        });
    }
}

}
}
