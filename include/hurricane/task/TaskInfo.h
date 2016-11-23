#pragma once

#include "hurricane/base/NetAddress.h"
#include "hurricane/base/Variant.h"
#include <string>
#include <cstdint>
#include <list>
#include <map>

namespace hurricane {
    namespace service {
        class SupervisorContext;
    }

    namespace task {
        class ExecutorPosition : public hurricane::base::Serializable {
        public:
            ExecutorPosition() : _executorIndex(-1) {
            }

            ExecutorPosition(const hurricane::base::NetAddress& supervisor, int executorIndex) :
                        _supervisor(supervisor), _executorIndex(executorIndex) {
            }

            const hurricane::base::NetAddress& GetSupervisor() const {
                return _supervisor;
            }

            void SetSupervisor(const hurricane::base::NetAddress& supervisor) {
                _supervisor = supervisor;
            }

            int GetExecutorIndex() const {
                return _executorIndex;
            }

            void SetExecutorIndex(int executorIndex) {
                _executorIndex = executorIndex;
            }

            virtual void Serialize(hurricane::base::Variants& variants) const override;
            virtual void Deserialize(hurricane::base::Variants::const_iterator& it) override;

        private:
            hurricane::base::NetAddress _supervisor;
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

            PathInfo() : _groupMethod(GroupMethod::Invalid) {}

            int GetGroupMethod() const {
                return _groupMethod;
            }

            void SetGroupMethod(int groupMethod) {
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
            int _groupMethod;
            std::string _destinationTask;
            std::string _fieldName;
            std::vector<ExecutorPosition> _destinationExecutors;
        };

        class TaskInfo : public hurricane::base::Serializable {
        public:
            TaskInfo() : _supervisorContext(nullptr), _executorIndex(-1) {
            }

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

            const hurricane::service::SupervisorContext* GetSupervisorContext() const {
                return _supervisorContext;
            }

            hurricane::service::SupervisorContext* GetSupervisorContext() {
                return _supervisorContext;
            }

            void SetSupervisorContext(hurricane::service::SupervisorContext* context) {
                _supervisorContext = context;
            }

            int GetExecutorIndex() const {
                return _executorIndex;
            }

            void SetExecutorIndex(int executorIndex) {
                _executorIndex = executorIndex;
            }

            virtual void Serialize(hurricane::base::Variants& variants) const override;
            virtual void Deserialize(hurricane::base::Variants::const_iterator& it) override;

			std::string _topologyName;
            std::string _taskName;
            std::list<PathInfo> _paths;

            hurricane::service::SupervisorContext* _supervisorContext;
            int _executorIndex;
		};
	}
}
