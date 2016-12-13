/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#include "hurricane/service/Manager.h"
#include "hurricane/message/CommandClient.h"
#include "hurricane/util/NetConnector.h"
#include "hurricane/util/Configuration.h"
#include "hurricane/topology/Topology.h"
#include "hurricane/topology/TopologyLoader.h"
#include "hurricane/task/SpoutExecutor.h"
#include "hurricane/task/BoltExecutor.h"
#include "hurricane/spout/ISpout.h"
#include "hurricane/bolt/IBolt.h"
#include "hurricane/collector/OutputCollector.h"
#include "hurricane/collector/OutputQueue.h"
#include "hurricane/collector/TaskQueue.h"
#include "hurricane/base/Constants.h"
#include "logging/Logging.h"

namespace hurricane {
    namespace service {

    Manager::Manager(const hurricane::util::Configuration& configuration) :
            CommandServer(new hurricane::util::NetListener(hurricane::base::NetAddress(
                    configuration.GetProperty(ConfigurationKey::ManagerHost),
                    configuration.GetIntegerProperty(ConfigurationKey::ManagerPort)))),
            _host(configuration.GetProperty(ConfigurationKey::ManagerHost)),
            _port(configuration.GetIntegerProperty(ConfigurationKey::ManagerPort)) {
        _managerConfiguration.reset(new hurricane::util::Configuration(configuration));
        _name = configuration.GetProperty(ConfigurationKey::ManagerName);

        InitPresidentConnector();
        InitSelfContext();
        ReserveExecutors();
        InitEvents();
    }

    void Manager::InitPresidentConnector()
    {
        hurricane::base::NetAddress presidentAddress(_managerConfiguration->GetProperty(ConfigurationKey::PresidentPort),
            _managerConfiguration->GetIntegerProperty(ConfigurationKey::PresidentPort));
        _presidentConnector = new hurricane::util::NetConnector(presidentAddress);
        _presidentClient = new hurricane::message::CommandClient(_presidentConnector);
    }

    void Manager::ReserveExecutors()
    {
        _spoutExecutors.resize(_managerConfiguration->GetIntegerProperty(ConfigurationKey::SpoutCount));
        _boltExecutors.resize(_managerConfiguration->GetIntegerProperty(ConfigurationKey::BoltCount));
        _spoutCollectors.resize(_managerConfiguration->GetIntegerProperty(ConfigurationKey::SpoutCount));
        _boltCollectors.resize(_managerConfiguration->GetIntegerProperty(ConfigurationKey::BoltCount));
        _boltTaskQueues.resize(_managerConfiguration->GetIntegerProperty(ConfigurationKey::BoltCount));

        for ( auto& boltTask : _boltTaskQueues ) {
            boltTask.reset(new collector::TaskQueue);
        }

        _outputDispatcher.SetQueue(std::shared_ptr<collector::OutputQueue>(
            new collector::OutputQueue()));
        _outputDispatcher.SetSelfAddress(hurricane::base::NetAddress(_host, _port));
        _outputDispatcher.SetSelfTasks(_boltTaskQueues);
        _outputDispatcher.SetSelfSpoutCount(static_cast<int32_t>(_spoutExecutors.size()));

        hurricane::base::NetAddress presidentAddress(_managerConfiguration->GetProperty(ConfigurationKey::PresidentHost),
            _managerConfiguration->GetIntegerProperty(ConfigurationKey::PresidentPort));
        _presidentConnector = new hurricane::util::NetConnector(presidentAddress);
        _presidentClient = new hurricane::message::CommandClient(_presidentConnector);
        _outputDispatcher.SetPresidentClient(_presidentClient);

        _outputDispatcher.Start();
    }

    void Manager::InitEvents()
    {
        OnConnection(std::bind(&Manager::OnConnect, this, std::placeholders::_1));
        OnCommand(hurricane::message::Command::Type::Heartbeat, this, &Manager::OnHeartbeat);
        OnCommand(hurricane::message::Command::Type::SyncMetadata, this, &Manager::OnSyncMetadata);
        OnCommand(hurricane::message::Command::Type::SendTuple, this, &Manager::OnSendTuple);
    }

    void Manager::InitTaskFieldsMap()
    {
        const std::map<std::string, hurricane::spout::SpoutDeclarer>& spoutDeclarers =
                _topology->GetSpoutDeclarers();
        for ( const auto& spoutDeclarerPair : spoutDeclarers ) {
            const spout::SpoutDeclarer& spoutDeclarer = spoutDeclarerPair.second;

            _taskFields[spoutDeclarer.GetTaskName()] = &spoutDeclarer.GetFields();
            _taskFieldsMap[spoutDeclarer.GetTaskName()] = &spoutDeclarer.GetFieldsMap();
        }

        const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers =
                _topology->GetBoltDeclarers();
        for ( const auto& boltDeclarerPair : boltDeclarers ) {
            const bolt::BoltDeclarer& boltDeclarer = boltDeclarerPair.second;

            _taskFields[boltDeclarer.GetTaskName()] = &boltDeclarer.GetFields();
            _taskFieldsMap[boltDeclarer.GetTaskName()] = &boltDeclarer.GetFieldsMap();
        }

        _outputDispatcher.SetTaskFields(_taskFields);
        _outputDispatcher.SetTaskFieldsMap(_taskFieldsMap);
    }

    void Manager::OnConnect(ManagerContext* context) {
    }

    void Manager::JoinPresident(JoinPresidentCallback callback) {
        hurricane::message::CommandClient* commandClient = _presidentClient;

        _presidentConnector->Connect([commandClient, callback, this](const util::SocketError&) {
            hurricane::message::Command command(hurricane::message::Command::Type::Join);
            command.AddArgument({ NodeType::Manager });
            command.AddArgument({ this->_host });
            command.AddArgument({ this->_port });
            std::vector<hurricane::base::Variant> context;
            _selfContext->Serialize(context);
            command.AddArguments(context);

            commandClient->SendCommand(command, [callback](const hurricane::message::Response& response,
                    const message::CommandError& error) {
                if ( error.GetType() != message::CommandError::Type::NoError ) {
                    LOG(LOG_ERROR) << error.what();
                    return;
                }

                callback(response);
            });
        });
    }

    void Manager::OnHeartbeat(ManagerContext* context, const message::Command& command,
        hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responsor Responsor)
    {
        hurricane::message::Response response(hurricane::message::Response::Status::Successful);
        response.AddArgument({ _name });

        Responsor(response);
    }

    void Manager::OnSyncMetadata(ManagerContext* context, const message::Command& command,
        message::CommandServer<hurricane::message::BaseCommandServerContext>::Responsor Responsor)
    {
        const std::vector<hurricane::base::Variant>& arguments = command.GetArguments();

        int32_t syncMethod = arguments[0].GetInt32Value();
        if ( syncMethod != 1 ) {
            hurricane::message::Response response(hurricane::message::Response::Status::Failed);
            Responsor(response);

            return;
        }

        hurricane::message::Response response(hurricane::message::Response::Status::Successful);
        base::Variants::const_iterator currentIterator = arguments.cbegin() + 1;
        _selfContext->Deserialize(currentIterator);

        OwnManagerTasks();
        _outputDispatcher.SetTaskInfos(_selfContext->GetTaskInfos());

        ShowManagerMetadata();
        ShowTaskInfos();

        std::string topologyName = _managerConfiguration->GetProperty(ConfigurationKey::TopologyName);
        _topology = hurricane::topology::TopologyLoader::GetInstance().GetTopology(topologyName);

        InitTaskFieldsMap();
        InitExecutors();

        Responsor(response);
    }

    void Manager::OnSendTuple(ManagerContext* context, const message::Command& command,
                                 message::CommandServer<hurricane::message::BaseCommandServerContext>::Responsor Responsor)
    {
        const base::Variants& arguments = command.GetArguments();
        base::Variants::const_iterator it = arguments.cbegin();

        base::NetAddress sourceAddress;
        sourceAddress.Deserialize(it);

        task::ExecutorPosition destination;
        destination.Deserialize(it);

        base::Tuple tuple;
        tuple.Deserialize(it);
        tuple.SetFields(_taskFields[tuple.GetSourceTask()]);
        tuple.SetFieldsMap(_taskFieldsMap[tuple.GetSourceTask()]);

        int32_t executorIndex = destination.GetExecutorIndex();
        int32_t boltIndex = executorIndex - _selfContext->GetSpoutCount();

        std::shared_ptr<hurricane::collector::TaskQueue> taskQueue = _boltTaskQueues[boltIndex];
        collector::TaskItem* taskItem =
                new collector::TaskItem(executorIndex, tuple);
        taskQueue->Push(taskItem);

        hurricane::message::Response response(hurricane::message::Response::Status::Successful);
        Responsor(response);
    }

    void Manager::InitSelfContext() {
        this->_selfContext.reset(new ManagerContext);
        _selfContext->SetId(_name);
        _selfContext->SetSpoutCount(_managerConfiguration->GetIntegerProperty(ConfigurationKey::SpoutCount));
        _selfContext->SetBoltCount(_managerConfiguration->GetIntegerProperty(ConfigurationKey::BoltCount));
        _selfContext->SetTaskInfos(std::vector<hurricane::task::TaskInfo>(_selfContext->GetSpoutCount() + _selfContext->GetBoltCount()));

        std::set<int32_t> freeSpouts;
        int spoutCount = _selfContext->GetSpoutCount();
        for ( int32_t spoutIndex = 0; spoutIndex != spoutCount; ++ spoutIndex ) {
            freeSpouts.insert(spoutIndex);
        }
        _selfContext->SetFreeSpouts(freeSpouts);

        std::set<int32_t> freeBolts;
        int boltCount = _selfContext->GetBoltCount();
        for ( int32_t boltIndex = 0; boltIndex != boltCount; ++ boltIndex ) {
            freeBolts.insert(boltIndex);
        }
        _selfContext->SetFreeBolts(freeBolts);
    }

    void Manager::InitSpoutExecutors()
    {
        LOG(LOG_DEBUG) << "Init spout executors";
        const std::map<std::string, hurricane::spout::SpoutDeclarer>& spoutDeclarers =
                _topology->GetSpoutDeclarers();
        std::set<int32_t> busySpouts = _selfContext->GetBusySpouts();
        for ( int32_t spoutIndex : busySpouts ) {
            hurricane::task::TaskInfo& spoutTask = _selfContext->GetSpoutTaskInfo(spoutIndex);
            std::string taskName = spoutTask.GetTaskName();
            const hurricane::spout::SpoutDeclarer& spoutDeclarer = spoutDeclarers.at(taskName);

            std::shared_ptr<collector::OutputQueue> outputQueue = _outputDispatcher.GetQueue();
            collector::OutputCollector* collector = new collector::OutputCollector(spoutIndex,
                    taskName, outputQueue);
            _spoutCollectors[spoutIndex].reset(collector);

            spout::ISpout* spout = spoutDeclarer.GetSpout()->Clone();
            spout->Prepare(_spoutCollectors[spoutIndex]);

            std::shared_ptr<task::SpoutExecutor> spoutExecutor(new task::SpoutExecutor);
            spoutExecutor->SetSpout(spout);
            int32_t flowParam = _managerConfiguration->GetIntegerProperty(ConfigurationKey::SpoutFlowParam);
            spoutExecutor->SetFlowParam(flowParam);
            _spoutExecutors[spoutIndex] = spoutExecutor;
        }
    }

    void Manager::InitBoltExecutors()
    {
        LOG(LOG_DEBUG) << "Init bolt executors";
        const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers =
                _topology->GetBoltDeclarers();
        std::set<int32_t> busyBolts = _selfContext->GetBusyBolts();
        int32_t spoutCount = _selfContext->GetSpoutCount();
        for ( int32_t boltIndex : busyBolts ) {
            LOG(LOG_DEBUG) << boltIndex;

            hurricane::task::TaskInfo& boltTask = _selfContext->GetBoltTaskInfo(boltIndex);
            std::string taskName = boltTask.GetTaskName();
            const hurricane::bolt::BoltDeclarer& boltDeclarer = boltDeclarers.at(taskName);

            std::shared_ptr<collector::OutputQueue> outputQueue = _outputDispatcher.GetQueue();
            collector::OutputCollector* collector = new collector::OutputCollector(
                        spoutCount + boltIndex, taskName, outputQueue);
            _boltCollectors[boltIndex].reset(collector);

            bolt::IBolt* bolt = boltDeclarer.GetBolt()->Clone();
            bolt->Prepare(_boltCollectors[boltIndex]);

            std::shared_ptr<task::BoltExecutor> boltExecutor(new task::BoltExecutor);
            _boltExecutors[boltIndex] = boltExecutor;
            boltExecutor->SetTaskQueue(_boltTaskQueues[boltIndex]);
            boltExecutor->SetBolt(bolt);
        }
    }

    void Manager::InitExecutors()
    {
        InitSpoutExecutors();
        InitBoltExecutors();

        std::set<int32_t> busyBolts = _selfContext->GetBusyBolts();
        std::set<int32_t> busySpouts = _selfContext->GetBusySpouts();

        for ( int32_t boltIndex : busyBolts ) {
            _boltExecutors[boltIndex]->Start();
        }

        for ( int32_t spoutIndex : busySpouts ) {
            _spoutExecutors[spoutIndex]->Start();
        }
    }

    void Manager::OwnManagerTasks()
    {
        std::vector<hurricane::task::TaskInfo>& taskInfos = _selfContext->GetTaskInfos();
        for ( hurricane::task::TaskInfo& taskInfo : taskInfos ) {
            taskInfo.SetManagerContext(_selfContext.get());
        }
    }

    void Manager::ShowManagerMetadata()
    {
        LOG(LOG_DEBUG) << "Manager name: " << _selfContext->GetId();
        LOG(LOG_DEBUG) << "  Spout count: " << _selfContext->GetSpoutCount();
        LOG(LOG_DEBUG) << "  Bolt count: " << _selfContext->GetBoltCount();
        LOG(LOG_DEBUG) << "  Task info count: " << _selfContext->GetTaskInfos().size();
        LOG(LOG_DEBUG) << "  Free spout count: " << _selfContext->GetFreeSpouts().size();
        LOG(LOG_DEBUG) << "  Free bolt count: " << _selfContext->GetFreeBolts().size();
        LOG(LOG_DEBUG) << "  Busy spout count: " << _selfContext->GetBusySpouts().size();
        LOG(LOG_DEBUG) << "  Busy bolt count: " << _selfContext->GetBusyBolts().size();
    }

    void Manager::ShowTaskInfos()
    {
        const std::vector<hurricane::task::TaskInfo>& taskInfos = _selfContext->GetTaskInfos();
        for ( const hurricane::task::TaskInfo& taskInfo : taskInfos ) {
            if ( !taskInfo.GetManagerContext() ) {
                continue;
            }

            LOG(LOG_DEBUG) << "    Manager: " << taskInfo.GetManagerContext()->GetId();
            LOG(LOG_DEBUG) << "    Exectuor index: " << taskInfo.GetExecutorIndex();
            LOG(LOG_DEBUG) << "    Task name: " << taskInfo.GetTaskName();
            LOG(LOG_DEBUG) << "    Paths: ";
            const std::list<hurricane::task::PathInfo>& paths = taskInfo.GetPaths();

            for ( const hurricane::task::PathInfo& path : paths ) {
                LOG(LOG_DEBUG) << "      Path: ";
                int32_t groupMethod = path.GetGroupMethod();
                LOG(LOG_DEBUG) << "        Group method: " << groupMethod;

                if ( path.GetGroupMethod() == hurricane::task::PathInfo::GroupMethod::Global) {
                    LOG(LOG_DEBUG) << "        Destination host: " <<
                                 path.GetDestinationExecutors()[0].GetManager().GetHost();
                    LOG(LOG_DEBUG) << "        Destination port: " <<
                                 path.GetDestinationExecutors()[0].GetManager().GetPort();
                    LOG(LOG_DEBUG) << "        Destination executor index: " <<
                                 path.GetDestinationExecutors()[0].GetExecutorIndex();
                }
            }
        }
    }
}
}
