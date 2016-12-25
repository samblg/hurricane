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

#pragma once

#include "hurricane/order/OrderTuple.h"

namespace hurricane {
namespace order {

void OrderTuple::Serialize(base::Variants& variants) const
{
    base::Tuple tuple = *this;
    tuple.Add({ _orderId });

    tuple.Serialize(variants);
}

void OrderTuple::Deserialize(base::Variants::const_iterator& it)
{
    Tuple::Deserialize(it);

    _orderId = GetValues()[GetSize() - 1].GetInt64Value();
    Pop();
}

int32_t OrderTuple::GetOrderId() const
{
    return _orderId;
}

void OrderTuple::SetOrderId(int32_t orderId)
{
    _orderId = orderId;
}

}
}
