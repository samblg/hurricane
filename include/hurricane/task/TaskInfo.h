#pragma once

#include "hurricane/base/NetAddress.h"
#include "hurricane/base/Variant.h"
#include <string>
#include <cstdint>
#include <list>

namespace hurricane {
    namespace service {
        class SupervisorContext;
    }

	namespace task {
        class PathInfo {
        public:
            struct GroupMethod {
                enum {
                    Invalid = 0,
                    Global,
                    Field,
                    Random
                };
            };

            PathInfo() : _groupMethod(GroupMethod::Invalid), _destinationExecutorIndex(-1) {}

            int GetGroupMethod() const {
                return _groupMethod;
            }

            void SetGroupMethod(int groupMethod) {
                _groupMethod = groupMethod;
            }

            const hurricane::base::NetAddress& GetDestinationSupervisor() const {
                return _destinationSupervisor;
            }

            void SetDestinationSupervisor(const hurricane::base::NetAddress& destinationSupervisor) {
                _destinationSupervisor = destinationSupervisor;
            }

            int GetDestinationExecutorIndex() const {
                return _destinationExecutorIndex;
            }

            void SetDestinationExecutorIndex(int destinationExecutorIndex) {
                _destinationExecutorIndex = destinationExecutorIndex;
            }

            const std::string& GetFieldName() const {
                return _fieldName;
            }

            void SetFieldName(const std::string& fieldName) {
                _fieldName = fieldName;
            }

            std::vector<hurricane::base::Variant> ToVariants() const;
            void ParseVariant(const std::vector<hurricane::base::Variant>& variants);
            std::vector<hurricane::base::Variant>::const_iterator
                ParseVariant(std::vector<hurricane::base::Variant>::const_iterator begin);
            static PathInfo FromVariants(const std::vector<hurricane::base::Variant>& variants);
            static PathInfo FromVariants(std::vector<hurricane::base::Variant>::const_iterator begin);

        private:
            int _groupMethod;
            hurricane::base::NetAddress _destinationSupervisor;
            int32_t _destinationExecutorIndex;
            std::string _fieldName;
        };

		class TaskInfo {
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

            std::vector<hurricane::base::Variant> ToVariants() const;
            void ParseVariant(const std::vector<hurricane::base::Variant>& variants);
            std::vector<hurricane::base::Variant>::const_iterator
            ParseVariant(std::vector<hurricane::base::Variant>::const_iterator begin);
            static TaskInfo FromVariants(const std::vector<hurricane::base::Variant>& variants);
            static TaskInfo FromVariants(std::vector<hurricane::base::Variant>::const_iterator begin);

			std::string _topologyName;
            std::string _taskName;
            std::list<PathInfo> _paths;

            hurricane::service::SupervisorContext* _supervisorContext;
            int _executorIndex;
		};
	}
}
