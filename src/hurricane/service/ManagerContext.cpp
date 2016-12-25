/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#include "hurricane/service/ManagerContext.h"

namespace hurricane {
namespace service {

using hurricane::base::Variant;
using hurricane::base::Variants;
using hurricane::base::Serializable;

ManagerContext::ManagerContext() : _spoutCount(0), _boltCount(0) {
}

void ManagerContext::Serialize(base::Variants& variants) const
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

void ManagerContext::Deserialize(Variants::const_iterator& it)
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
