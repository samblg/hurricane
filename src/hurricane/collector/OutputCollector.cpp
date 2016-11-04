#include "hurricane/collector/OutputCollector.h"
#include "hurricane/collector/OutputQueue.h"
#include "hurricane/base/Values.h"

#include <iostream>

namespace hurricane {
	namespace collector {
        void OutputCollector::Emit(const hurricane::base::Tuple& tuple) {
            if ( _taskIndex != -1 ) {
                _queue->Push(new OutputItem(_taskIndex, tuple));
            }
		}
	}
}
