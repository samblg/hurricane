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

void OutputDispatcher::SetNimbusClient(message::CommandClient* nimbusClient)
{
    _nimbusClient.reset(nimbusClient);
}

void OutputDispatcher::Start()
{
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
    std::string sourceTaskName = taskInfo.GetTaskName();
    std::string destTaskName = path.GetTaskName();

    outputItem->GetTuple().SetSourceTask(sourceTaskName);
    outputItem->GetTuple().SetDestTask(destTaskName);

    if ( path.GetGroupMethod() == task::PathInfo::GroupMethod::Global ) {
        const task::ExecutorPosition& executorPosition = path.GetDestinationExecutors()[0];

        SendTupleTo(outputItem, executorPosition);
    }
    else if ( path.GetGroupMethod() == task::PathInfo::GroupMethod::Random ) {
        int destCount = path.GetDestinationExecutors().size();
        int destIndex = rand() % destCount;

        const task::ExecutorPosition& executorPosition = path.GetDestinationExecutors()[destIndex];

        SendTupleTo(outputItem, executorPosition);
    }
    else if ( path.GetGroupMethod() == task::PathInfo::GroupMethod::Field ) {
        TaskPathName taskPathName = { sourceTaskName, destTaskName };

        auto taskPairIter = _fieldsDestinations.find(taskPathName);
        if ( taskPairIter == _fieldsDestinations.end() ) {
            _fieldsDestinations.insert({ taskPathName, std::map<std::string, task::ExecutorPosition>() });
            taskPairIter = _fieldsDestinations.find(taskPathName);
        }

        std::map<std::string, task::ExecutorPosition>& destinations = taskPairIter->second;
        int fieldIndex = this->_taskFieldsMap[sourceTaskName]->at(path.GetFieldName());
        std::string fieldValue = outputItem->GetTuple()[fieldIndex].GetStringValue();
        auto fieldDestIter = destinations.find(fieldValue);

        if ( fieldDestIter == destinations.end() ) {
            AskField(taskPathName, fieldValue,
                     [taskPathName, outputItem, fieldValue, this](task::ExecutorPosition executorPosition) -> void {
                _fieldsDestinations[taskPathName].insert({fieldValue, executorPosition});
                SendTupleTo(outputItem, executorPosition);
            });
        }
        else {
            const task::ExecutorPosition& executorPosition = fieldDestIter->second;
            SendTupleTo(outputItem, executorPosition);
        }
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

        commandClient->GetConnector()->Connect([
                outputItem, commandClient, destIdentifier, executorPosition, this] {
            hurricane::message::Command command(hurricane::message::Command::Type::SendTuple);

            base::Variants commandVariants;
            _selfAddress.Serialize(commandVariants);
            executorPosition.Serialize(commandVariants);
            outputItem->GetTuple().Serialize(commandVariants);

            command.AddArguments(commandVariants);

            try {
                commandClient->SendCommand(command,
                    [destIdentifier, this](const hurricane::message::Response& response) -> void {
                    if ( response.GetStatus() == hurricane::message::Response::Status::Successful ) {
                    }
                    else {
                        std::cout << "Send to " << destIdentifier << " failed." << std::endl;
                    }
                });
            }
            catch ( util::SocketException& e ) {
//                std::cout << e.what() << std::endl;
            }
        });
    }
}

void OutputDispatcher::AskField(TaskPathName taskPathName,
        const std::string& fieldValue, OutputDispatcher::AskFieldCallback callback)
{
    _nimbusClient->Connect([taskPathName, fieldValue, callback, this]() {
        hurricane::message::Command command(hurricane::message::Command::Type::AskField);
        command.AddArgument(taskPathName.first);
        command.AddArgument(taskPathName.second);
        command.AddArgument(fieldValue);

        _nimbusClient->SendCommand(command, [callback](const hurricane::message::Response& response) {
            task::ExecutorPosition destination;
            const base::Variants respArguments = response.GetArguments();

            base::Variants::const_iterator argIter = respArguments.cbegin();
            destination.Deserialize(argIter);

            callback(destination);
        });
    });
}

}
}
