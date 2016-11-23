#pragma once

namespace hurricane {
	namespace topology {
		class Topology;
	}
}

#include "hurricane/base/externc.h"

BEGIN_EXTERN_C
hurricane::topology::Topology* GetTopology();
END_EXTERN_C
