#include "hurricane/collector/OutputCollector.h"
#include "hurricane/base/Values.h"

#include <iostream>

namespace hurricane {
	namespace collector {
		void OutputCollector::Emit(const hurricane::base::Tuple& tuple) {
			std::cout << "Output Collector" << std::endl;
			std::cout << tuple.GetSize() << std::endl;
		}
	}
}