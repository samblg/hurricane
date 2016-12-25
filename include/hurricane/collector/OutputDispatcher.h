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

#pragma once

#include "hurricane/task/TaskInfo.h"
#include "hurricane/base/NetAddress.h"
#include <memory>
#include <thread>
#include <map>
#include <functional>

namespace hurricane {

namespace message {
class CommandClient;
}

namespace collector {

class OutputQueue;
class OutputItem;
class TaskQueue;

typedef std::pair<std::string, std::string> TaskPathName;

class DispatchTaskInfo {
private:
    int32_t _taskType;
    std::vector<task::PathInfo> _globalPaths;
    std::vector<std::vector<task::PathInfo>> _randomPaths;
    std::vector<std::map<std::string, std::vector<task::PathInfo>>> _fieldPaths;
};

class OutputDispatcher {
public:
    typedef std::function<void(task::ExecutorPosition)> AskFieldCallback;

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

    void SetSelfSpoutCount(int32_t selfSpoutCount) {
        _selfSpoutCount = selfSpoutCount;
    }

    void SetSelfTasks(std::vector<std::shared_ptr<hurricane::collector::TaskQueue>> selfTasks) {
        _selfTasks = selfTasks;
    }

    void SetTaskInfos(const std::vector<hurricane::task::TaskInfo>& taskInfos);
    void SetPresidentClient(message::CommandClient* presidentClient);

    void Start();

    void SetTaskFields(const std::map<std::string, const std::vector<std::string>*>& taskFields) {
        _taskFields = taskFields;
    }

    void SetTaskFieldsMap(const std::map<std::string, const std::map<std::string, int32_t>*>& taskFieldsMap) {
        _taskFieldsMap = taskFieldsMap;
    }

private:
    void MainThread();
    void ProcessPath(const task::TaskInfo& taskInfo, const task::PathInfo& path,
            OutputItem* outputItem);
    void SendTupleTo(OutputItem* outputItem, const task::ExecutorPosition& executorPosition);
    void AskField(TaskPathName taskPathName, const std::string& fieldValue, AskFieldCallback callback);

private:
    std::shared_ptr<OutputQueue> _queue;
    std::thread _thread;
    base::NetAddress _selfAddress;
    int32_t _selfSpoutCount;

    std::vector<hurricane::task::TaskInfo> _taskInfos;

    std::vector<std::shared_ptr<hurricane::collector::TaskQueue>> _selfTasks;
    std::shared_ptr<message::CommandClient> _presidentClient;
    std::map<std::string, message::CommandClient*> _commandClients;
    std::map<TaskPathName, std::map<std::string, task::ExecutorPosition>> _fieldsDestinations;
    std::map<std::string, const std::vector<std::string>*> _taskFields;
    std::map<std::string, const std::map<std::string, int32_t>*> _taskFieldsMap;
};

}
}
