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

#include "hurricane/base/NetAddress.h"
#include "hurricane/base/Variant.h"
#include <string>
#include <cstdint>
#include <list>
#include <map>

namespace hurricane {
namespace service {
class ManagerContext;
}

namespace task {
class ExecutorPosition : public hurricane::base::Serializable {
public:
    ExecutorPosition();
    ExecutorPosition(const hurricane::base::NetAddress& manager, int32_t executorIndex);

    const hurricane::base::NetAddress& GetManager() const {
        return _manager;
    }

    void SetManager(const hurricane::base::NetAddress& manager) {
        _manager = manager;
    }

    int32_t GetExecutorIndex() const {
        return _executorIndex;
    }

    void SetExecutorIndex(int32_t executorIndex) {
        _executorIndex = executorIndex;
    }

    virtual void Serialize(hurricane::base::Variants& variants) const override;
    virtual void Deserialize(hurricane::base::Variants::const_iterator& it) override;

private:
    hurricane::base::NetAddress _manager;
    int32_t _executorIndex;
};

class PathInfo : public hurricane::base::Serializable {
public:
    struct GroupMethod {
        enum {
            Invalid = 0,
            Global,
            Field,
            Random
        };
    };

    PathInfo();

    int32_t GetGroupMethod() const {
        return _groupMethod;
    }

    void SetGroupMethod(int32_t groupMethod) {
        _groupMethod = groupMethod;
    }

    void SetDestinationTask(const std::string& taskName) {
        _destinationTask = taskName;
    }

    const std::string& GetTaskName() const {
        return _destinationTask;
    }

    const std::string& GetFieldName() const {
        return _fieldName;
    }

    void SetFieldName(const std::string& fieldName) {
        _fieldName = fieldName;
    }

    const std::vector<ExecutorPosition>& GetDestinationExecutors() const {
        return _destinationExecutors;
    }

    void SetDestinationExecutors(const std::vector<ExecutorPosition>& executors) {
        _destinationExecutors = executors;
    }

    virtual void Serialize(hurricane::base::Variants& variants) const override;
    virtual void Deserialize(hurricane::base::Variants::const_iterator& it) override;

private:
    int32_t _groupMethod;
    std::string _destinationTask;
    std::string _fieldName;
    std::vector<ExecutorPosition> _destinationExecutors;
};

class TaskInfo : public hurricane::base::Serializable {
public:
    TaskInfo();

    const std::string& GetTopologyName() const {
        return _topologyName;
    }

    void SetTopologyName(const std::string& topologyName) {
        _topologyName = topologyName;
    }

    const std::string& GetTaskName() const {
        return _taskName;
    }

    void SetTaskName(const std::string& taskName) {
        _taskName = taskName;
    }

    const std::list<PathInfo>& GetPaths() const {
        return _paths;
    }

    void SetPaths(const std::list<PathInfo>& paths) {
        _paths = paths;
    }

    void AddPath(const PathInfo& path) {
        _paths.push_back(path);
    }

    const hurricane::service::ManagerContext* GetManagerContext() const {
        return _managerContext;
    }

    hurricane::service::ManagerContext* GetManagerContext() {
        return _managerContext;
    }

    void SetManagerContext(hurricane::service::ManagerContext* context) {
        _managerContext = context;
    }

    int32_t GetExecutorIndex() const {
        return _executorIndex;
    }

    void SetExecutorIndex(int32_t executorIndex) {
        _executorIndex = executorIndex;
    }

    virtual void Serialize(hurricane::base::Variants& variants) const override;
    virtual void Deserialize(hurricane::base::Variants::const_iterator& it) override;

    std::string _topologyName;
    std::string _taskName;
    std::list<PathInfo> _paths;

    hurricane::service::ManagerContext* _managerContext;
    int32_t _executorIndex;
};
}
}
