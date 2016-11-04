#include "hurricane/service/Nimbus.h"
#include "hurricane/util/NetConnector.h"
#include "hurricane/message/CommandClient.h"
#include "hurricane/util/Configuration.h"
#include "hurricane/topology/Topology.h"
#include "sample/wordcount/WordCountTopology.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <list>
#include <cassert>

namespace hurricane {
	namespace service {
		Nimbus::Nimbus(const hurricane::base::NetAddress& host) : 
				CommandServer(new hurricane::util::NetListener(host)),
                _nimbusHost(host),
                _supervisorCount(0) {
			OnConnection(std::bind(&Nimbus::OnConnect, this, std::placeholders::_1));
			OnCommand(hurricane::message::Command::Type::Join, this, &Nimbus::OnJoin);
		}

		Nimbus::Nimbus(const hurricane::util::Configuration& configuration) : 
				Nimbus(hurricane::base::NetAddress(
					configuration.GetProperty("nimbus.host"),
					configuration.GetIntegerProperty("nimbus.port"))) {
            _supervisorCount = configuration.GetIntegerProperty("nimbus.supervisor.count");

            std::cout << "Need supervisors: " << _supervisorCount << std::endl;
		}

		void Nimbus::OnConnect(SupervisorContext* context) {
		}

		void Nimbus::OnJoin(SupervisorContext* context, const hurricane::message::Command& command,
				hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser) {
			std::string joinerType = command.GetArgument(0).GetStringValue();

            std::cout << "Join node: " << joinerType << std::endl;
            
            SupervisorContext supervisorContext = SupervisorContext::FromVariants(command.GetArguments().cbegin() + 3);
            std::string supervisorHost = command.GetArgument(1).GetStringValue();
            int supervisorPort = command.GetArgument(2).GetIntValue();

            std::cout << "Supervisor name: " << supervisorContext.GetId() << std::endl;
            std::cout << "Host: " << supervisorHost << std::endl;
            std::cout << "Port: " << supervisorPort << std::endl;
            std::cout << "Spout count: " << supervisorContext.GetSpoutCount() << std::endl;
            std::cout << "Bolt count: " << supervisorContext.GetBoltCount() << std::endl;
            std::cout << "Task info count: " << supervisorContext.GetTaskInfos().size() << std::endl;
            std::cout << "Free spout count: " << supervisorContext.GetFreeSpouts().size() << std::endl;
            std::cout << "Free bolt count: " << supervisorContext.GetFreeBolts().size() << std::endl;
            std::cout << "Busy spout count: " << supervisorContext.GetBusySpouts().size() << std::endl;
            std::cout << "Busy bolt count: " << supervisorContext.GetBusyBolts().size() << std::endl;

            supervisorContext.SetNetAddress(hurricane::base::NetAddress(
                    supervisorHost, supervisorPort));
            supervisorContext.PrepareTaskInfos();
            _supervisors.push_back(supervisorContext);

            // Response
			hurricane::message::Response response(hurricane::message::Response::Status::Successful);
			response.AddArgument({ "nimbus" });

			responser(response);

            // Initialize command clients
            hurricane::base::NetAddress supervisorAddress(supervisorHost,
                supervisorPort);
            hurricane::util::NetConnector* supervisorConnector =
                    new hurricane::util::NetConnector(supervisorAddress);
            hurricane::message::CommandClient* supervisorCommandClient =
                    new hurricane::message::CommandClient(supervisorConnector);

            _supervisorClients.insert({supervisorContext.GetId(),
                    std::shared_ptr<hurricane::message::CommandClient>(supervisorCommandClient)});

            SendHeartbeat(supervisorContext.GetId());

            // Initialize topology
            if ( _supervisors.size() == _supervisorCount ) {
                topology::Topology* topology = GetTopology();
                SubmitTopology(topology);
            }
		}

        std::list<task::TaskInfo> Nimbus::GetAllSpoutTasks(
                const std::map<std::string, hurricane::spout::SpoutDeclarer>& spoutDeclarers,
                hurricane::topology::Topology* topology)
        {
            std::list<hurricane::task::TaskInfo> originSpoutTasks;
            for ( const auto& spoutPair : spoutDeclarers ) {
                hurricane::spout::SpoutDeclarer spoutDeclarer = spoutPair.second;
                std::cout << "Spout " << spoutDeclarer.GetTaskName() << std::endl;
                std::cout << "ParallismHint: " << spoutDeclarer.GetParallismHint() << std::endl;

                int parallismHint = spoutDeclarer.GetParallismHint();
                for ( int taskIndex = 0; taskIndex != parallismHint; ++ taskIndex ) {
                    hurricane::task::TaskInfo taskInfo;
                    taskInfo.SetTopologyName(topology->GetName());
                    taskInfo.SetTaskName(spoutDeclarer.GetTaskName());

                    originSpoutTasks.push_back(taskInfo);
                }
            }

            return originSpoutTasks;
        }

        std::map<std::string, std::vector<hurricane::task::TaskInfo*>>
                Nimbus::AllocateSpoutTasks(std::list<hurricane::task::TaskInfo>& originSpoutTasks)
        {
            std::map<std::string, std::vector<hurricane::task::TaskInfo*>> nameToSpoutTasks;
            // Allocate task for every supervisor
            for ( SupervisorContext& supervisorContext : _supervisors ) {
                if ( !originSpoutTasks.size() ) {
                    break;
                }

                while ( true ) {
                    if ( !originSpoutTasks.size() ) {
                        break;
                    }

                    // If useNextSpout return -1, the spout slots is used up
                    int spoutIndex = supervisorContext.useNextSpout();
                    if ( spoutIndex == -1 ) {
                        break;
                    }

                    // Put the spout task into spout slot
                    hurricane::task::TaskInfo taskInfo = originSpoutTasks.front();
                    taskInfo.SetSupervisorContext(&supervisorContext);
                    taskInfo.SetExecutorIndex(supervisorContext.GetExecutorIndex(
                            SupervisorContext::ExecutorType::Spout, spoutIndex));
                    originSpoutTasks.pop_front();
                    supervisorContext.SetSpoutTaskInfo(spoutIndex, taskInfo);

                    // Insert the spout task pointer into mapper
                    std::string taskName = taskInfo.GetTaskName();
                    auto spoutTasksPair = nameToSpoutTasks.find(taskName);
                    if ( spoutTasksPair == nameToSpoutTasks.end() ) {
                        nameToSpoutTasks.insert({taskName, std::vector<hurricane::task::TaskInfo*>()});
                        spoutTasksPair = nameToSpoutTasks.find(taskName);
                    }

                    spoutTasksPair->second.push_back(&(supervisorContext.GetSpoutTaskInfo(spoutIndex)));
                }
            }

            return nameToSpoutTasks;
        }

        std::list<task::TaskInfo> Nimbus::GetAllBoltTasks(hurricane::topology::Topology* topology,
                const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers)
        {
            std::list<hurricane::task::TaskInfo> originBoltTasks;
            for ( const auto& boltPair : boltDeclarers ) {
                hurricane::bolt::BoltDeclarer boltDeclarer = boltPair.second;
                std::cout << "Bolt " << boltDeclarer.GetTaskName() << std::endl;
                std::cout << "Source: " << boltDeclarer.GetSourceTaskName() << std::endl;
                std::cout << "ParallismHint: " << boltDeclarer.GetParallismHint() << std::endl;

                int parallismHint = boltDeclarer.GetParallismHint();
                for ( int taskIndex = 0; taskIndex != parallismHint; ++ taskIndex ) {
                    hurricane::task::TaskInfo taskInfo;
                    taskInfo.SetTopologyName(topology->GetName());
                    taskInfo.SetTaskName(boltDeclarer.GetTaskName());

                    originBoltTasks.push_back(taskInfo);
                }
            }

            return originBoltTasks;
        }

        std::map<std::string, std::vector<hurricane::task::TaskInfo*>>
            Nimbus::AllocateBoltTasks(std::list<hurricane::task::TaskInfo>& originBoltTasks)
        {
            std::map<std::string, std::vector<hurricane::task::TaskInfo*>> nameToBoltTasks;
            // Allocate bolt tasks
            for ( SupervisorContext& supervisorContext : _supervisors ) {
                if ( !originBoltTasks.size() ) {
                    break;
                }

                while ( true ) {
                    if ( !originBoltTasks.size() ) {
                        break;
                    }

                    // If useNextBolt return -1, the bolt slots is used up
                    int boltIndex = supervisorContext.useNextBolt();
                    if ( boltIndex == -1 ) {
                        break;
                    }

                    // Put the bolt task into bolt slot
                    hurricane::task::TaskInfo taskInfo = originBoltTasks.front();
                    taskInfo.SetSupervisorContext(&supervisorContext);
                    taskInfo.SetExecutorIndex(supervisorContext.GetExecutorIndex(
                            SupervisorContext::ExecutorType::Bolt, boltIndex));
                    originBoltTasks.pop_front();
                    supervisorContext.SetBoltTaskInfo(boltIndex, taskInfo);

                    // Insert the bolt task pointer into mapper
                    std::string taskName = taskInfo.GetTaskName();
                    auto boltTasksPair = nameToBoltTasks.find(taskName);
                    if ( boltTasksPair == nameToBoltTasks.end() ) {
                        nameToBoltTasks.insert({taskName, std::vector<hurricane::task::TaskInfo*>()});
                        boltTasksPair = nameToBoltTasks.find(taskName);
                    }

                    boltTasksPair->second.push_back(&(supervisorContext.GetBoltTaskInfo(boltIndex)));
                }
            }

            return nameToBoltTasks;
        }

        std::vector<task::TaskInfo*> Nimbus::FindTask(
                const std::map<std::string, std::vector<task::TaskInfo*>>& nameToBoltTasks,
                const std::map<std::string, std::vector<task::TaskInfo*>>& nameToSpoutTasks,
                const std::string& sourceTaskName)
        {
            auto spoutTaskPair = nameToSpoutTasks.find(sourceTaskName);
            if ( spoutTaskPair != nameToSpoutTasks.end() ) {
                return spoutTaskPair->second;
            }

            auto boltTaskPair = nameToBoltTasks.find(sourceTaskName);
            if ( boltTaskPair != nameToBoltTasks.end() ) {
                return boltTaskPair->second;
            }

            return std::vector<task::TaskInfo*>();
        }

        std::vector<task::TaskInfo*> Nimbus::FindTask(
                const std::map<std::string, std::vector<task::TaskInfo*>>& nameToBoltTasks,
                const std::string& sourceTaskName)
        {
            auto boltTaskPair = nameToBoltTasks.find(sourceTaskName);
            if ( boltTaskPair != nameToBoltTasks.end() ) {
                return boltTaskPair->second;
            }

            return std::vector<task::TaskInfo*>();
        }

        void Nimbus::ShowTaskInfos(const std::vector<hurricane::task::TaskInfo>& taskInfos)
        {
            for ( const hurricane::task::TaskInfo& taskInfo : taskInfos ) {
                if ( !taskInfo.GetSupervisorContext() ) {
                    continue;
                }

                std::cout << "    Supervisor: " << taskInfo.GetSupervisorContext()->GetId() << std::endl;
                std::cout << "    Exectuor index: " << taskInfo.GetExecutorIndex() << std::endl;
                std::cout << "    Paths: " << std::endl;
                const std::list<hurricane::task::PathInfo>& paths = taskInfo.GetPaths();

                for ( const hurricane::task::PathInfo& path : paths ) {
                    std::cout << "      Path: " << std::endl;
                    int groupMethod = path.GetGroupMethod();
                    std::cout << "        Group method: " << groupMethod << std::endl;
                    if ( path.GetGroupMethod() == hurricane::task::PathInfo::GroupMethod::Global) {
                        std::cout << "        Destination host: " <<
                                     path.GetDestinationSupervisor().GetHost() << std::endl;
                        std::cout << "        Destination port: " <<
                                     path.GetDestinationSupervisor().GetPort() << std::endl;
                    }
                    std::cout << "        Destination executor index: " << path.GetDestinationExecutorIndex() << std::endl;
                }
            }
        }

        void Nimbus::SyncWithSupervisors()
        {
            for ( SupervisorContext& supervisorContext : _supervisors ) {
                std::string supervisorId = supervisorContext.GetId();
                std::cout << "Sync meta data with supervisr: " << supervisorId;
                std::shared_ptr<hurricane::message::CommandClient> supervisorClient =
                        _supervisorClients[supervisorId];

                supervisorClient->GetConnector()->Connect([supervisorId, supervisorClient, &supervisorContext, this] {
                    hurricane::message::Command command(hurricane::message::Command::Type::SyncMetadata);

                    // 1 means Nimbus to Supervisor
                    // 2 means Supervisor to Nimbus
                    command.AddArgument({ 1 });
                    command.AddArguments(supervisorContext.ToVariants());
                    supervisorClient->SendCommand(command,
                        [supervisorId, this](const hurricane::message::Response& response) -> void {
                        if ( response.GetStatus() == hurricane::message::Response::Status::Successful ) {
                            std::cout << "Sync with " << supervisorId << " successfully." << std::endl;
                        }
                        else {
                            std::cout << "Sync with " << supervisorId << " failed." << std::endl;
                        }
                    });
                });
            }
        }

        void Nimbus::ShowSupervisorTaskInfos()
        {
            std::cout << std::endl << "================ Allocate result ================" << std::endl;
            for ( SupervisorContext& supervisorContext : _supervisors ) {
                std::cout << supervisorContext.GetId() << std::endl;
                std::cout << "  Host: " << supervisorContext.GetNetAddress().GetHost() << std::endl;
                std::cout << "  Port: " << supervisorContext.GetNetAddress().GetPort() << std::endl;

                std::cout << "  Tasks: " << std::endl;
                const std::vector<hurricane::task::TaskInfo>& taskInfos =
                        supervisorContext.GetTaskInfos();
                ShowTaskInfos(taskInfos);
            }
        }

        void Nimbus::CalculateTaskPaths(
                const std::map<std::string, std::vector<hurricane::task::TaskInfo*>>& nameToBoltTasks,
                const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers,
                const std::map<std::string, std::vector<hurricane::task::TaskInfo*>>& nameToSpoutTasks)
        {
            for ( const auto& boltPair : boltDeclarers ) {
                hurricane::bolt::BoltDeclarer boltDeclarer = boltPair.second;
                // No setted source task
                if ( boltDeclarer.GetSourceTaskName().empty() ) {
                    continue;
                }

                std::string sourceTaskName = boltDeclarer.GetSourceTaskName();
                std::vector<hurricane::task::TaskInfo*> sourceTasks =
                        FindTask(nameToBoltTasks, nameToSpoutTasks, sourceTaskName);

                std::string destTaskName = boltDeclarer.GetTaskName();
                std::vector<hurricane::task::TaskInfo*> destTasks =
                        FindTask(nameToBoltTasks, destTaskName);

                std::set<int> globalSelectedSources;
                for ( hurricane::task::TaskInfo* destTask : destTasks ) {
                    if ( boltDeclarer.GetGroupMethod() == task::TaskDeclarer::GroupMethod::Global ) {
                        // Global random group
                        if ( globalSelectedSources.size() == sourceTasks.size() ) {
                            break;
                        }

                        int sourceTaskIndex = rand() % sourceTasks.size();
                        while ( globalSelectedSources.find(sourceTaskIndex) !=
                                globalSelectedSources.end()) {
                            sourceTaskIndex = rand() % sourceTasks.size();
                        }

                        hurricane::task::TaskInfo* sourceTask = sourceTasks[sourceTaskIndex];

                        hurricane::task::PathInfo pathInfo;
                        pathInfo.SetGroupMethod(hurricane::task::PathInfo::GroupMethod::Global);
                        pathInfo.SetDestinationExecutorIndex(destTask->GetExecutorIndex());
                        pathInfo.SetDestinationSupervisor(destTask->GetSupervisorContext()->GetNetAddress());

                        sourceTask->AddPath(pathInfo);
                        globalSelectedSources.insert(sourceTaskIndex);
                    }
                    else if ( boltDeclarer.GetGroupMethod() == task::TaskDeclarer::GroupMethod::Field ) {
                        // Resolve the destination by field when run task.
                        for ( hurricane::task::TaskInfo* sourceTask : sourceTasks ) {
                            hurricane::task::PathInfo pathInfo;
                            pathInfo.SetGroupMethod(hurricane::task::PathInfo::GroupMethod::Field);
                            pathInfo.SetFieldName(boltDeclarer.GetGroupField());

                            sourceTask->AddPath(pathInfo);
                        }
                    }
                    else if ( boltDeclarer.GetGroupMethod() == task::TaskDeclarer::GroupMethod::Random ) {
                        // Resolve the destination by field when run task.
                        for ( hurricane::task::TaskInfo* sourceTask : sourceTasks ) {
                            hurricane::task::PathInfo pathInfo;
                            pathInfo.SetGroupMethod(hurricane::task::PathInfo::GroupMethod::Random);
                            pathInfo.SetDestinationExecutorIndex(destTask->GetExecutorIndex());
                            pathInfo.SetDestinationSupervisor(destTask->GetSupervisorContext()->GetNetAddress());

                            sourceTask->AddPath(pathInfo);
                        }
                    }
                    else {
                        std::cerr << "Unsupported group method occured" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }

        void Nimbus::ShowSupervisorMetadata()
        {
            std::cout << std::endl << "================ Supervisor metadata ================" << std::endl;
            for ( SupervisorContext& supervisorContext : _supervisors ) {
                std::cout << "Supervisor name: " << supervisorContext.GetId() << std::endl;
                std::cout << "  Spout count: " << supervisorContext.GetSpoutCount() << std::endl;
                std::cout << "  Bolt count: " << supervisorContext.GetBoltCount() << std::endl;
                std::cout << "  Task info count: " << supervisorContext.GetTaskInfos().size() << std::endl;
                std::cout << "  Free spout count: " << supervisorContext.GetFreeSpouts().size() << std::endl;
                std::cout << "  Free bolt count: " << supervisorContext.GetFreeBolts().size() << std::endl;
                std::cout << "  Busy spout count: " << supervisorContext.GetBusySpouts().size() << std::endl;
                std::cout << "  Busy bolt count: " << supervisorContext.GetBusyBolts().size() << std::endl;
            }
        }

        void Nimbus::SubmitTopology(hurricane::topology::Topology* topology) {
            std::cout << "Submit topology: " << topology->GetName() << std::endl;

            const std::map<std::string, hurricane::spout::SpoutDeclarer>& spoutDeclarers =
                    topology->GetSpoutDeclarers();
            const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers =
                    topology->GetBoltDeclarers();

            // Allocate task and send to supervisor
            std::list<hurricane::task::TaskInfo> originSpoutTasks =
                    GetAllSpoutTasks(spoutDeclarers, topology);
            std::map<std::string, std::vector<hurricane::task::TaskInfo*>> nameToSpoutTasks =
                    AllocateSpoutTasks(originSpoutTasks);

            std::list<hurricane::task::TaskInfo> originBoltTasks =
                    GetAllBoltTasks(topology, boltDeclarers);
            std::map<std::string, std::vector<hurricane::task::TaskInfo*>> nameToBoltTasks =
                    AllocateBoltTasks(originBoltTasks);

            CalculateTaskPaths(nameToBoltTasks, boltDeclarers, nameToSpoutTasks);
            ShowSupervisorTaskInfos();
            ShowSupervisorMetadata();
            SyncWithSupervisors();
        }

        const int MAX_HEARTBEAT_FAILED_TIMES = 5;
        void Nimbus::SendHeartbeat(const std::string supervisorId)
        {
            std::cout << "Sending heartbeat to " << supervisorId << std::endl;

            int sendTimes = 0;
            while ( true ) {
                try {
                    std::shared_ptr<hurricane::message::CommandClient> commandClient =
                            _supervisorClients.at(supervisorId);

                    commandClient->GetConnector()->Connect([commandClient, supervisorId, this] {
                        std::cout << "Connected to " << supervisorId << std::endl;
                        hurricane::message::Command command(hurricane::message::Command::Type::Heartbeat);

                        commandClient->SendCommand(command,
                            [supervisorId, this](const hurricane::message::Response& response) -> void {
                            if ( response.GetStatus() == hurricane::message::Response::Status::Successful ) {
                                std::cout << supervisorId << " alived." << std::endl;
                            }
                            else {
                                std::cout << supervisorId << " dead." << std::endl;
                            }
                        });
                    });

                    break;
                }
                catch ( const std::exception& e ) {
                    std::cerr << "Error in sending heartbeat to " << supervisorId << std::endl;
                    std::cerr << e.what() << std::endl;

                    sendTimes ++;
                    std::cout << "Sendtimes: " << sendTimes << std::endl;
                    if ( sendTimes >= MAX_HEARTBEAT_FAILED_TIMES ) {
                        break;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }
        }
	}
}
