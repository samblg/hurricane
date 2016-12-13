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
    Variant::Serialize(variants, _manager);
    Variant::Serialize(variants, _executorIndex);
}

void ExecutorPosition::Deserialize(Variants::const_iterator& it)
{
    Variant::Deserialize(it, _manager);
    Variant::Deserialize(it, _executorIndex);
}

}
}
