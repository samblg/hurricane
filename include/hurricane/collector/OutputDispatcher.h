#pragma once

#include "hurricane/task/TaskInfo.h"
#include "hurricane/base/NetAddress.h"
#include <memory>
#include <thread>
#include <map>

namespace hurricane {

namespace message {
class CommandClient;
}

namespace collector {

class OutputQueue;
class OutputItem;
class TaskQueue;

class DispatchTaskInfo {
private:
    int _taskType;
    std::vector<task::PathInfo> _globalPaths;
    std::vector<std::vector<task::PathInfo>> _randomPaths;
    std::vector<std::map<std::string, std::vector<task::PathInfo>>> _fieldPaths;
};

class OutputDispatcher {
public:
    OutputDispatcher() : _selfSpoutCount(0) {
    }

    OutputDispatcher(std::shared_ptr<OutputQueue> queue) : _queue(queue), _selfSpoutCount(0) {
    }

    void SetQueue(std::shared_ptr<OutputQueue> queue) {
        _queue = queue;
    }

    std::shared_ptr<OutputQueue> GetQueue() const {
        return _queue;
    }

    void SetSelfAddress(const base::NetAddress& selfAddress) {
        _selfAddress = selfAddress;
    }

    void SetSelfSpoutCount(int selfSpoutCount) {
        _selfSpoutCount = selfSpoutCount;
    }

    void SetSelfTasks(std::vector<std::shared_ptr<hurricane::collector::TaskQueue>> selfTasks) {
        _selfTasks = selfTasks;
    }

    void SetTaskInfos(const std::vector<hurricane::task::TaskInfo>& taskInfos);

    void Start();

private:
    void MainThread();
    bool ProcessPath(const task::TaskInfo& taskInfo, const task::PathInfo& path,
            OutputItem* outputItem);

private:
    std::shared_ptr<OutputQueue> _queue;
    std::thread _thread;
    base::NetAddress _selfAddress;
    int _selfSpoutCount;

    std::vector<hurricane::task::TaskInfo> _taskInfos;
    std::map<std::string, std::vector<hurricane::task::PathInfo>> _globalPaths;
    std::map<std::string, std::vector<hurricane::task::PathInfo>> _

    std::vector<std::shared_ptr<hurricane::collector::TaskQueue>> _selfTasks;
    std::map<std::string, message::CommandClient*> _commandClients;
};

}
}
