#pragma once

namespace hurricane {
	namespace base {
		class Tuple;
	}

	namespace collector {
		class OutputCollector {
		public:
			void Emit(const hurricane::base::Tuple& tuple);
		};
	}
}