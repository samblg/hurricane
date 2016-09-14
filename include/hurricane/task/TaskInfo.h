#pragma once

#include "hurricane/base/NetAddress.h"
#include "hurricane/base/Variant.h"
#include <string>
#include <cstdint>

namespace hurricane {
	namespace task {
		class TaskInfo {
		public:
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

			const hurricane::base::NetAddress& GetSourceSupervisor() const {
				return _sourceSupervisor;
			}

			void SetSourceSupervisor(const hurricane::base::NetAddress& sourceSupervisor) {
				_sourceSupervisor = sourceSupervisor;
			}

			int GetSourceExecutorIndex() const {
				return _sourceExecutorIndex;
			}

			void SetSourceExecutorIndex(int sourceExecutorIndex) {
				_sourceExecutorIndex = sourceExecutorIndex;
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

            std::vector<hurricane::base::Variant> ToVariants();
            void ParseVariant(const std::vector<hurricane::base::Variant>& variants);
            static TaskInfo FromVariants(const std::vector<hurricane::base::Variant>& variants);

			std::string _topologyName;
			std::string _taskName;

			hurricane::base::NetAddress _sourceSupervisor;
			int32_t _sourceExecutorIndex;

			hurricane::base::NetAddress _destinationSupervisor;
			int32_t _destinationExecutorIndex;
		};
	}
}