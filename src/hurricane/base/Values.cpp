#include "hurricane/base/Values.h"

namespace hurricane {
namespace base {

void Tuple::Serialize(Variants& variants) const
{
    base::Variant::Serialize(variants, _sourceTask);
    base::Variant::Serialize(variants, _destTask);
    base::Variant::Serialize(variants, _values);
}

void Tuple::Deserialize(Variants::const_iterator& it)
{
    base::Variant::Deserialize(it, _sourceTask);
    base::Variant::Deserialize(it, _destTask);
    base::Variant::Deserialize(it, _values);
}

}
}
