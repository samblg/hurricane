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
