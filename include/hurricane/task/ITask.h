#pragma once

#include "hurricane/collector/OutputCollector.h"

#include <vector>
#include <memory>

namespace hurricane {
	namespace task {
		class ITask {
		public:
			virtual void Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) = 0;
			virtual void Cleanup() = 0;
			virtual std::vector<std::string> DeclareFields() = 0;
		};
	}
}