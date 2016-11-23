#include "hurricane/task/TaskInfo.h"
#include "hurricane/base/Variant.h"

namespace hurricane {
namespace task {

using hurricane::base::Variant;
using hurricane::base::Variants;
using hurricane::base::Serializable;

void PathInfo::Serialize(base::Variants& variants) const
{
    Variant::Serialize(variants, _groupMethod);
    Variant::Serialize(variants, _destinationTask);
    Variant::Serialize(variants, _fieldName);
    Variant::Serialize(variants, _destinationExecutors);
}

void PathInfo::Deserialize(Variants::const_iterator& it)
{
    Variant::Deserialize(it, _groupMethod);
    Variant::Deserialize(it, _destinationTask);
    Variant::Deserialize(it, _fieldName);
    Variant::Deserialize(it, _destinationExecutors);
}

void ExecutorPosition::Serialize(base::Variants& variants) const
{
    Variant::Serialize(variants, _supervisor);
    Variant::Serialize(variants, _executorIndex);
}

void ExecutorPosition::Deserialize(Variants::const_iterator& it)
{
    Variant::Deserialize(it, _supervisor);
    Variant::Deserialize(it, _executorIndex);
}

}
}
