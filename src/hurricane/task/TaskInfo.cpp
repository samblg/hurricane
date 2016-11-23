#include "hurricane/task/TaskInfo.h"

namespace hurricane {
namespace task {

using hurricane::base::Variant;
using hurricane::base::Variants;
using hurricane::base::Serializable;

void TaskInfo::Serialize(base::Variants& variants) const
{
    Variant::Serialize(variants, _topologyName);
    Variant::Serialize(variants, _taskName);
    Variant::Serialize(variants, _paths);
    Variant::Serialize(variants, _executorIndex);
}

void TaskInfo::Deserialize(base::Variants::const_iterator& it)
{
    Variant::Deserialize(it, _topologyName);
    Variant::Deserialize(it, _taskName);
    Variant::Deserialize(it, _paths);
    Variant::Deserialize(it, _executorIndex);
}

}
}
