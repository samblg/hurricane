#pragma once

#include "hurricane/message/CommandServer.h"
#include "hurricane/util/NetListener.h"
#include "hurricane/base/NetAddress.h"
#include "hurricane/service/SupervisorContext.h"
#include "hurricane/collector/OutputDispatcher.h"

#include <functional>
#include <memory>

namespace hurricane {
	namespace util {
		class NetConnector;
		class Configuration;
	}

	namespace message {
		class CommandClient;
    }

    namespace task {
        class SpoutExecutor;
        class BoltExecutor;
    }

    namespace topology {
        class Topology;
    }

    namespace collector {
        class OutputCollector;
        class TaskQueue;
    }

	namespace service {
		class Supervisor : public hurricane::message::CommandServer<SupervisorContext> {
		public:
			typedef std::function<void(const hurricane::message::Response& response)> JoinNimbusCallback;
            
			Supervisor(const hurricane::util::Configuration& configuration);

			void OnConnect(SupervisorContext* context);

			void JoinNimbus(JoinNimbusCallback callback);

            void OnHeartbeat(SupervisorContext* context, const hurricane::message::Command& command,
                            hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser);
            void OnSyncMetadata(SupervisorContext* context, const hurricane::message::Command& command,
                            hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser);


        private:
            void InitSelfContext();
            void InitExecutors();
            void OwnSupervisorTasks();
            void ShowSupervisorMetadata();
            void ShowTaskInfos();
            void InitSpoutExecutors();
            void InitBoltExecutors();
            void initNimbusConnector();
            void reserveExecutors();
            void initEvents();

		private:
            std::string _name;
            std::string _host;
            int _port;
            std::shared_ptr<hurricane::util::Configuration> _supervisorConfiguration;
			hurricane::util::NetConnector* _nimbusConnector;
			hurricane::message::CommandClient* _nimbusClient;
            std::shared_ptr<hurricane::service::SupervisorContext> _selfContext;
            std::vector<std::shared_ptr<hurricane::task::SpoutExecutor>> _spoutExecutors;
            std::vector<std::shared_ptr<hurricane::task::BoltExecutor>> _boltExecutors;
            std::vector<std::shared_ptr<hurricane::collector::OutputCollector>> _spoutCollectors;
            std::vector<std::shared_ptr<hurricane::collector::OutputCollector>> _boltCollectors;
            std::vector<std::shared_ptr<hurricane::collector::TaskQueue>> _boltTaskQueues;
            std::shared_ptr<topology::Topology> _topology;
            hurricane::collector::OutputDispatcher _outputDispatcher;
        };
	}
}
