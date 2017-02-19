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

#include "hurricane/order/OrderBolt.h"
#include "hurricane/order/OrderOutputCollector.h"
#include <algorithm>

namespace hurricane {
namespace order {

OrderBolt::OrderBolt(bool isFinal) : _isFinal(isFinal), _nextOrderId(1) {
}

void OrderBolt::Prepare(std::shared_ptr<collector::OutputCollector> outputCollector)
{
    std::shared_ptr<hurricane::order::OrderOutputCollector> orderCollector =
            std::make_shared<hurricane::order::OrderOutputCollector>(outputCollector);

    Prepare(orderCollector);
    _nextOrderId = 1;
    _tupleQueue.clear();
}

void OrderBolt::Execute(const base::Tuple& tuple)
{
    OrderTuple orderTuple = tuple;
    orderTuple.ParseBaseTuple();

    if ( orderTuple.GetOrderId() != _nextOrderId ) {
        InsertTuple(orderTuple);

        return;
    }

    while ( true ) {
        base::Tuple baseTuple = orderTuple.ToBaseTuple();
        Execute(baseTuple);
        _nextOrderId ++;

        orderTuple = _tupleQueue.front();
        if ( orderTuple.GetOrderId() != _nextOrderId ) {
            break;
        }

        _tupleQueue.pop_front();
    }
}

void OrderBolt::InsertTuple(const OrderTuple& tuple)
{
    auto insertPos = std::find_if(_tupleQueue.begin(), _tupleQueue.end(),
                               [tuple](const OrderTuple& another) -> bool {
        return another.GetOrderId() > tuple.GetOrderId();
    });

    if ( insertPos == _tupleQueue.end() ) {
        _tupleQueue.push_back(tuple);

        return;
    }

    _tupleQueue.insert(insertPos, tuple);
}

}

}
