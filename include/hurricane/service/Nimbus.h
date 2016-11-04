#pragma once

#include "hurricane/message/CommandServer.h"
#include "hurricane/util/NetListener.h"
#include "hurricane/base/NetAddress.h"
#include "hurricane/service/SupervisorContext.h"

namespace hurricane {
	namespace util {
		class Configuration;
	}

    namespace topology {
        class Topology;
    }

    namespace message {
        class CommandClient;
    }

    namespace spout {
        class SpoutDeclarer;
    }

    namespace bolt {
        class BoltDeclarer;
    }

	namespace service {
		class Nimbus : public hurricane::message::CommandServer<SupervisorContext> {
		public:
			Nimbus(const hurricane::base::NetAddress& host);
			Nimbus(const hurricane::util::Configuration& configuration);

			void OnConnect(SupervisorContext* context);
			void OnJoin(SupervisorContext* context, const hurricane::message::Command& command,
				hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser);
            void SubmitTopology(hurricane::topology::Topology* topology);
            
        private:
            void SendHeartbeat(const std::string supervisorId);
            std::list<hurricane::task::TaskInfo> GetAllSpoutTasks(const std::map<std::string, hurricane::spout::SpoutDeclarer>& spoutDeclarers, hurricane::topology::Topology* topology);
            void AllocateSpoutTasks(std::map<std::string, hurricane::task::TaskInfo*> nameToSpoutTasks, std::list<hurricane::task::TaskInfo> originSpoutTasks);
            std::map<std::string, std::vector<task::TaskInfo*> > AllocateSpoutTasks(std::list<task::TaskInfo>& originSpoutTasks);
            std::list<hurricane::task::TaskInfo> GetAllBoltTasks(hurricane::topology::Topology* topology, const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers);
            std::map<std::string, std::vector<task::TaskInfo*> > AllocateBoltTasks(std::list<task::TaskInfo>& originBoltTasks);
            std::vector<task::TaskInfo*> FindTask(
                    const std::map<std::string, std::vector<task::TaskInfo*> >& nameToBoltTasks,
                    const std::map<std::string, std::vector<task::TaskInfo*> >& nameToSpoutTasks,
                    const std::string& sourceTaskName);
            std::vector<task::TaskInfo*> FindTask(
                    const std::map<std::string, std::vector<task::TaskInfo*> >& nameToBoltTasks,
                    const std::string& sourceTaskName);
            void CalculateTaskPaths(
                    const std::map<std::string, std::vector<task::TaskInfo*> >& nameToBoltTasks,
                    const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers,
                    const std::map<std::string, std::vector<task::TaskInfo*> >& nameToSpoutTasks);
            void ShowSupervisorMetadata();
            void ShowSupervisorTaskInfos();
            void ShowTaskInfos(const std::vector<hurricane::task::TaskInfo>& taskInfos);
            void SyncWithSupervisors();

		private:
			hurricane::base::NetAddress _nimbusHost;
            std::vector<SupervisorContext> _supervisors;
            int _supervisorCount;
            std::map<std::string, std::shared_ptr<hurricane::message::CommandClient>> _supervisorClients;
        };
	}
}
