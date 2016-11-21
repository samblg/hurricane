#include "hurricane/service/SupervisorContext.h"

namespace hurricane {
namespace service {

using hurricane::base::Variant;
using hurricane::base::Variants;
using hurricane::base::Serializable;

SupervisorContext::SupervisorContext() : _spoutCount(0), _boltCount(0) {
}

void SupervisorContext::Serialize(base::Variants& variants) const
{
    Variant::Serialize(variants, _id);
    Variant::Serialize(variants, _spoutCount);
    Variant::Serialize(variants, _boltCount);
    Variant::Serialize(variants, _freeSpouts);
    Variant::Serialize(variants, _freeBolts);
    Variant::Serialize(variants, _busySpouts);
    Variant::Serialize(variants, _busyBolts);
    Variant::Serialize(variants, _taskInfos);

}

void SupervisorContext::Deserialize(Variants::const_iterator& it)
{
    Variant::Deserialize(it, _id);
    Variant::Deserialize(it, _spoutCount);
    Variant::Deserialize(it, _boltCount);
    Variant::Deserialize(it, _freeSpouts);
    Variant::Deserialize(it, _freeBolts);
    Variant::Deserialize(it, _busySpouts);
    Variant::Deserialize(it, _busyBolts);
    Variant::Deserialize(it, _taskInfos);
}

}
}
