#pragma once

#include "hurricane/message/CommandServer.h"
#include "hurricane/util/NetListener.h"
#include "hurricane/base/NetAddress.h"
#include "hurricane/service/ManagerContext.h"

#include <thread>

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
typedef std::pair<std::string, std::string> TaskPathName;

class President : public hurricane::message::CommandServer<ManagerContext> {
public:
    President(const hurricane::base::NetAddress& host);
    President(const hurricane::util::Configuration& configuration);

    void OnConnect(ManagerContext* context);
    void OnJoin(ManagerContext* context, const hurricane::message::Command& command,
                hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responsor Responsor);
    void OnAskField(ManagerContext* context, const hurricane::message::Command& command,
                    hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responsor Responsor);
    void OnOrderId(ManagerContext* context, const hurricane::message::Command& command,
                   hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responsor Responsor);
    void SubmitTopology(hurricane::topology::Topology* topology);

private:
    void SendHeartbeat(const std::string managerId, int32_t sendTimes);
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
    void ShowManagerMetadata();
    void ShowManagerTaskInfos();
    void ShowTaskInfos(const std::vector<hurricane::task::TaskInfo>& taskInfos);
    void SyncWithManagers();
    void StartConnectionChecker();
    void CheckConnections();

private:
    hurricane::base::NetAddress _presidentHost;
    std::vector<ManagerContext> _managers;
    int32_t _managerCount;
    std::shared_ptr<hurricane::util::Configuration> _configuration;
    std::map<std::string, std::shared_ptr<hurricane::message::CommandClient>> _managerClients;
    std::map<TaskPathName, std::vector<task::ExecutorPosition>> _fieldsCandidates;
    std::map<TaskPathName, std::map<std::string, task::ExecutorPosition>> _fieldsDestinations;
    std::map<std::string, int64_t> _orderIds;
    bool _submitted;

    std::thread _connectionChecker;
};
}
}
