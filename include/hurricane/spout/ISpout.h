#pragma once

#include "hurricane/task/ITask.h"
#include "hurricane/base/Values.h"

namespace hurricane {
	namespace spout {
		class ISpout : public hurricane::task::ITask {
        public:
            virtual ISpout* Clone() = 0;
			virtual void NextTuple() = 0;
		};
	}
}