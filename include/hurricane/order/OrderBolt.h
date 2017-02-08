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
#include "hurricane/bolt/IBolt.h"
#include <list>

namespace hurricane {
namespace order {

class OrderOutputCollector;

class OrderBolt : public bolt::IBolt {
public:
    OrderBolt(bool isFinal);

    void Prepare(std::shared_ptr<collector::OutputCollector> outputCollector) override;
    void Execute(const base::Tuple& tuple) override;

    virtual void Prepare(std::shared_ptr<OrderOutputCollector> outputCollector) = 0;
    virtual void Execute(const OrderTuple& tuple) = 0;

    bool IsFinal() const {
        return _isFinal;
    }

    void SetFinal(bool isFinal) {
        _isFinal = isFinal;
    }

private:
    void InsertTuple(const OrderTuple& tuple);

private:
    bool _isFinal;
    int64_t _nextOrderId;
    std::list<OrderTuple> _tupleQueue;
};

}
}
