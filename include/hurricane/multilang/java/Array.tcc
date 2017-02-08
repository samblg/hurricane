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

namespace hurricane {
namespace java {

template <class Element>
Array<Element>::Array(jobjectArray array) : _vm(VirtualMachine::GetDefault()), _array(array) {
    JNIEnv* env = _vm->GetEnv();

    jsize arraySize = env->GetArrayLength(_array);
    _data.resize(arraySize);

    for ( int i = 0; i != arraySize; ++ i ) {
        _data[i] = Element(env->GetObjectArrayElement(array, i));
    }
}

template <class Element>
Array<Element>::Array(jobject array) : Array(jobjectArray(array)) {}

template <class Element>
Array<Element>::Array(size_t size) : _vm(VirtualMachine::GetDefault()), _data(size) {}

namespace signature_impl {

template <class Element>
std::string ForType(const Array<Element>*) {
    return "[" + ForType((const Element*)(nullptr));
}

}

}
}
