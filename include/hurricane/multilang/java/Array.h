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

#include "hurricane/multilang/java/Class.h"
#include "hurricane/multilang/java/Object.h"
#include "hurricane/multilang/java/VirtualMachine.h"
#include "hurricane/multilang/java/SignatureImpl.h"

#include <jni.h>
#include <vector>

namespace hurricane {
namespace java {

template <class Element>
class Array : public Object {
public:
    Array(jobjectArray array);
    Array(jobject array);
    Array(size_t size);

    Element& operator[](int index) {
        return _data[index];
    }

    const Element& operator[](int index) const {
        return _data[index];
    }

    size_t GetSize() const {
        return _data.size();
    }

    const std::vector<Element>& GetData() const {
        return _data;
    }

private:
    VirtualMachine* _vm;
    jobjectArray _array;
    std::vector<Element> _data;
};

namespace signature_impl {

template <class Element>
std::string ForType(const Array<Element>*);

}
}
}

#include "hurricane/multilang/java/Array.tcc"
