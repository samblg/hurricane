#include "hurricane/service/Supervisor.h"
#include "hurricane/message/CommandClient.h"
#include "hurricane/util/NetConnector.h"
#include "hurricane/util/Configuration.h"
#include "hurricane/topology/Topology.h"
#include "hurricane/task/SpoutExecutor.h"
#include "hurricane/task/BoltExecutor.h"
#include "hurricane/spout/ISpout.h"
#include "hurricane/bolt/IBolt.h"
#include "hurricane/collector/OutputCollector.h"
#include "hurricane/collector/OutputQueue.h"
#include "hurricane/collector/TaskQueue.h"
#include "sample/wordcount/WordCountTopology.h"

namespace hurricane {
    namespace service {

    Supervisor::Supervisor(const hurricane::util::Configuration& configuration) :
            CommandServer(new hurricane::util::NetListener(hurricane::base::NetAddress(
                    configuration.GetProperty("supervisor.host"),
                    configuration.GetIntegerProperty("supervisor.port")))),
            _host(configuration.GetProperty("supervisor.host")),
            _port(configuration.GetIntegerProperty("supervisor.port")) {
        _supervisorConfiguration.reset(new hurricane::util::Configuration(configuration));
        _name = configuration.GetProperty("supervisor.name");

        initNimbusConnector();
        InitSelfContext();
        reserveExecutors();
        initEvents();
    }

    void Supervisor::initNimbusConnector()
    {
        hurricane::base::NetAddress nimbusAddress(_supervisorConfiguration->GetProperty("nimbus.host"),
            _supervisorConfiguration->GetIntegerProperty("nimbus.port"));
        _nimbusConnector = new hurricane::util::NetConnector(nimbusAddress);
        _nimbusClient = new hurricane::message::CommandClient(_nimbusConnector);
    }

    void Supervisor::reserveExecutors()
    {
        _spoutExecutors.resize(_supervisorConfiguration->GetIntegerProperty("supervisor.spout.num"));
        _boltExecutors.resize(_supervisorConfiguration->GetIntegerProperty("supervisor.bolt.num"));
        _spoutCollectors.resize(_supervisorConfiguration->GetIntegerProperty("supervisor.spout.num"));
        _boltCollectors.resize(_supervisorConfiguration->GetIntegerProperty("supervisor.bolt.num"));
        _boltTaskQueues.resize(_supervisorConfiguration->GetIntegerProperty("supervisor.bolt.num"));

        for ( auto& boltTask : _boltTaskQueues ) {
            boltTask.reset(new collector::TaskQueue);
        }

        _outputDispatcher.SetQueue(std::shared_ptr<collector::OutputQueue>(
            new collector::OutputQueue()));
        _outputDispatcher.SetSelfAddress(hurricane::base::NetAddress(_host, _port));
        _outputDispatcher.SetSelfTasks(_boltTaskQueues);
        _outputDispatcher.SetSelfSpoutCount(_spoutExecutors.size());
        _outputDispatcher.Start();
    }

    void Supervisor::initEvents()
    {
        OnConnection(std::bind(&Supervisor::OnConnect, this, std::placeholders::_1));
        OnCommand(hurricane::message::Command::Type::Heartbeat, this, &Supervisor::OnHeartbeat);
        OnCommand(hurricane::message::Command::Type::SyncMetadata, this, &Supervisor::OnSyncMetadata);
    }

    void Supervisor::OnConnect(SupervisorContext* context) {
    }

    void Supervisor::JoinNimbus(JoinNimbusCallback callback) {
        hurricane::message::CommandClient* commandClient = _nimbusClient;

        _nimbusConnector->Connect([commandClient, callback, this]() {
            hurricane::message::Command command(hurricane::message::Command::Type::Join);
            command.AddArgument({ "supervisor" });
            command.AddArgument({ this->_host });
            command.AddArgument({ this->_port });
            std::vector<hurricane::base::Variant> context = _selfContext->ToVariants();
            command.AddArguments(context);

            commandClient->SendCommand(command, [callback](const hurricane::message::Response& response) {
                callback(response);
            });
        });
    }

    void Supervisor::OnHeartbeat(SupervisorContext* context, const message::Command& command,
        hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser)
    {
        hurricane::message::Response response(hurricane::message::Response::Status::Successful);
        response.AddArgument({ _name });

        responser(response);
    }

    void Supervisor::OnSyncMetadata(SupervisorContext* context, const message::Command& command,
        message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser)
    {
        const std::vector<hurricane::base::Variant>& arguments = command.GetArguments();

        int syncMethod = arguments[0].GetIntValue();
        if ( syncMethod != 1 ) {
            hurricane::message::Response response(hurricane::message::Response::Status::Failed);
            responser(response);

            return;
        }

        hurricane::message::Response response(hurricane::message::Response::Status::Successful);
        _selfContext->ParseVariants(arguments.cbegin() + 1);
        OwnSupervisorTasks();
        _outputDispatcher.SetTaskInfos(_selfContext->GetTaskInfos());

        ShowSupervisorMetadata();
        ShowTaskInfos();

        _topology.reset(GetTopology());
        InitExecutors();

        responser(response);
    }

    void Supervisor::InitSelfContext() {
        this->_selfContext.reset(new SupervisorContext);
        _selfContext->SetId(_name);
        _selfContext->SetSpoutCount(_supervisorConfiguration->GetIntegerProperty("supervisor.spout.num"));
        _selfContext->SetBoltCount(_supervisorConfiguration->GetIntegerProperty("supervisor.bolt.num"));
        _selfContext->SetTaskInfos(std::vector<hurricane::task::TaskInfo>(_selfContext->GetSpoutCount() + _selfContext->GetBoltCount()));

        std::set<int> freeSpouts;
        for ( int spoutIndex = 0; spoutIndex != _selfContext->GetSpoutCount(); ++ spoutIndex ) {
            freeSpouts.insert(spoutIndex);
        }
        _selfContext->SetFreeSpouts(freeSpouts);

        std::set<int> freeBolts;
        for ( int boltIndex = 0; boltIndex != _selfContext->GetBoltCount(); ++ boltIndex ) {
            freeBolts.insert(boltIndex);
        }
        _selfContext->SetFreeBolts(freeBolts);
    }

    void Supervisor::InitSpoutExecutors()
    {
        std::cout << "Init spout executors" << std::endl;
        const std::map<std::string, hurricane::spout::SpoutDeclarer>& spoutDeclarers =
                _topology->GetSpoutDeclarers();
        std::set<int> busySpouts = _selfContext->GetBusySpouts();
        for ( int spoutIndex : busySpouts ) {
            hurricane::task::TaskInfo& spoutTask = _selfContext->GetSpoutTaskInfo(spoutIndex);
            std::string taskName = spoutTask.GetTaskName();
            const hurricane::spout::SpoutDeclarer& spoutDeclarer = spoutDeclarers.at(taskName);

            std::shared_ptr<collector::OutputQueue> outputQueue = _outputDispatcher.GetQueue();
            collector::OutputCollector* collector = new collector::OutputCollector(spoutIndex,
                    outputQueue);
            _spoutCollectors[spoutIndex].reset(collector);

            spout::ISpout* spout = spoutDeclarer.GetSpout()->Clone();
            spout->Prepare(_spoutCollectors[spoutIndex]);

            std::shared_ptr<task::SpoutExecutor> spoutExecutor(new task::SpoutExecutor);
            spoutExecutor->SetSpout(spout);
            _spoutExecutors[spoutIndex] = spoutExecutor;
        }
    }

    void Supervisor::InitBoltExecutors()
    {
        std::cout << "Init bolt executors" << std::endl;
        const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers =
                _topology->GetBoltDeclarers();
        std::set<int> busyBolts = _selfContext->GetBusyBolts();
        int spoutCount = _selfContext->GetSpoutCount();
        for ( int boltIndex : busyBolts ) {
            std::cout << boltIndex << std::endl;

            hurricane::task::TaskInfo& boltTask = _selfContext->GetBoltTaskInfo(boltIndex);
            std::string taskName = boltTask.GetTaskName();
            const hurricane::bolt::BoltDeclarer& boltDeclarer = boltDeclarers.at(taskName);

            std::shared_ptr<collector::OutputQueue> outputQueue = _outputDispatcher.GetQueue();
            collector::OutputCollector* collector = new collector::OutputCollector(
                        spoutCount + boltIndex, outputQueue);
            _boltCollectors[boltIndex].reset(collector);

            bolt::IBolt* bolt = boltDeclarer.GetBolt()->Clone();
            bolt->Prepare(_boltCollectors[boltIndex]);

            std::shared_ptr<task::BoltExecutor> boltExecutor(new task::BoltExecutor);
            _boltExecutors[boltIndex] = boltExecutor;
            boltExecutor->SetTaskQueue(_boltTaskQueues[boltIndex]);
            boltExecutor->SetBolt(bolt);
        }
    }

    void Supervisor::InitExecutors()
    {
        InitSpoutExecutors();
        InitBoltExecutors();

        std::set<int> busyBolts = _selfContext->GetBusyBolts();
        std::set<int> busySpouts = _selfContext->GetBusySpouts();

        for ( int boltIndex : busyBolts ) {
            _boltExecutors[boltIndex]->Start();
        }

        for ( int spoutIndex : busySpouts ) {
            _spoutExecutors[spoutIndex]->Start();
        }
    }

    void Supervisor::OwnSupervisorTasks()
    {
        std::vector<hurricane::task::TaskInfo>& taskInfos = _selfContext->GetTaskInfos();
        for ( hurricane::task::TaskInfo& taskInfo : taskInfos ) {
            taskInfo.SetSupervisorContext(_selfContext.get());
        }
    }

    void Supervisor::ShowSupervisorMetadata()
    {
        std::cout << "Supervisor name: " << _selfContext->GetId() << std::endl;
        std::cout << "  Spout count: " << _selfContext->GetSpoutCount() << std::endl;
        std::cout << "  Bolt count: " << _selfContext->GetBoltCount() << std::endl;
        std::cout << "  Task info count: " << _selfContext->GetTaskInfos().size() << std::endl;
        std::cout << "  Free spout count: " << _selfContext->GetFreeSpouts().size() << std::endl;
        std::cout << "  Free bolt count: " << _selfContext->GetFreeBolts().size() << std::endl;
        std::cout << "  Busy spout count: " << _selfContext->GetBusySpouts().size() << std::endl;
        std::cout << "  Busy bolt count: " << _selfContext->GetBusyBolts().size() << std::endl;
    }

    void Supervisor::ShowTaskInfos()
    {
        const std::vector<hurricane::task::TaskInfo>& taskInfos = _selfContext->GetTaskInfos();
        for ( const hurricane::task::TaskInfo& taskInfo : taskInfos ) {
            if ( !taskInfo.GetSupervisorContext() ) {
                continue;
            }

            std::cout << "    Supervisor: " << taskInfo.GetSupervisorContext()->GetId() << std::endl;
            std::cout << "    Exectuor index: " << taskInfo.GetExecutorIndex() << std::endl;
            std::cout << "    Task name: " << taskInfo.GetTaskName() << std::endl;
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
}
}
