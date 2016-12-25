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

#include "hurricane/base/Values.h"

namespace hurricane {
namespace order {

class OrderTuple : public hurricane::base::Tuple {
public:
    OrderTuple() = default;
    OrderTuple(const base::Tuple& tuple) : base::Tuple(tuple) {
    }

    base::Value& operator[](size_t index) {
        return base::Tuple::operator [](index);
    }

    const base::Value& operator[](size_t index) const {
        return base::Tuple::operator [](index);
    }

    base::Value& operator[](const std::string& fieldName) {
        return base::Tuple::operator [](_fieldsMap->at(fieldName));
    }

    const base::Value& operator[](const std::string& fieldName) const {
        return base::Tuple::operator [](_fieldsMap->at(fieldName));
    }

    void Serialize(base::Variants& variants) const override;
    void Deserialize(base::Variants::const_iterator& it) override;

    int32_t GetOrderId() const;
    void SetOrderId(int32_t orderId);

private:
    int64_t _orderId;
};

}
}
