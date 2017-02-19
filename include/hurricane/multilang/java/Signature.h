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

#include <jni.h>

#include <string>
#include <exception>
#include <typeinfo>
#include <cstdint>
#include <iostream>

namespace hurricane {
namespace java {

#define HJARG(Type) (const Type*)(nullptr)

class Signature {
public:
    static const char ArgumentLeftBrace = '(';
    static const char ArgumentRightBrace = ')';

    template <class Type>
    static std::string ForType(const Type* t) {
        return signature_impl::ForType(t);
    }

    static std::string ForMethodArgs() {
        return "";
    }

    template <class Arg, class ...Args>
    static std::string ForMethodArgs(const Arg*, Args ...args) {
        return ForType<Arg>((const Arg*)(nullptr)) + ForMethodArgs(args...);
    }

    template <class ReturnValue, class ...Args>
    static std::string ForMethod(const Args* ...args) {
        return "(" + ForMethodArgs(args...) + ")" + ForType<ReturnValue>((const ReturnValue*)(nullptr));
    }
};

}
}
