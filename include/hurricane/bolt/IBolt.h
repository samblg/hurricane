#pragma once

#include "hurricane/task/ITask.h"
#include "hurricane/base/Values.h"

namespace hurricane {
	namespace bolt {
		class IBolt : public hurricane::task::ITask {
		public:
            virtual IBolt* Clone() = 0;
			virtual void Execute(const hurricane::base::Tuple& tuple) = 0;
		};
	}
}