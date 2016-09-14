#pragma once

#include <string>
#include <cstdint>

namespace hurricane {
	namespace task {
		class TaskDeclarer {
		public:
			struct Type {
				enum {
					Invalid = 0,
					Spout = 1,
					Bolt = 2
				};
			};

			struct GroupMethod {
				enum {
					Global = 0,
					Random = 1,
					Field = 2
				};
			};

			TaskDeclarer() : 
				_type(Type::Invalid), _groupMethod(GroupMethod::Global), _parallismHint(1) {
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

			int32_t GetType() const {
				return _type;
			}

			void SetType(int32_t type) {
				_type = type;
			}

			int GetGroupMethod() const {
				return _groupMethod;
			}

			void SetGroupMethod(int32_t groupMethod) {
				_groupMethod = groupMethod;
			}

			const std::string& GetSourceTaskName() const {
				return _sourceTaskName;
			}

			void SetSourceTaskName(const std::string& sourceTaskName) {
				_sourceTaskName = sourceTaskName;
			}

			int32_t GetParallismHint() const {
				return _parallismHint;
			}

			void SetParallismHint(int parallismHint) {
				_parallismHint = parallismHint;
			}

			std::string _topologyName;
			std::string _taskName;

			int32_t _type;
			int32_t _groupMethod;

			std::string _sourceTaskName;
			int32_t _parallismHint;
		};
	}
}