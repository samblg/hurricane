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

#include "hurricane/multilang/java/VirtualMachine.h"
#include "logging/Logging.h"

#include <jni.h>
#include <string>
#include <map>

namespace hurricane {
namespace java {

class VirtualMachine;

typedef jmethodID Method;

class Class {
public:
    static Class ForName(const std::string& className);

public:
    Class(VirtualMachine* vm, const std::string& className, jclass clazz);

    Method GetStaticMethod(const std::string& methodName, const std::string& signature) {
        return _env->GetStaticMethodID(_clazz, methodName.c_str(), signature.c_str());
    }

    Method GetMethod(const std::string& methodName, const std::string& signature) {
        Method method = _env->GetMethodID(_clazz, methodName.c_str(), signature.c_str());
        if ( method == 0 ) {
            LOG(LOG_FATAL) << "Can't find class method " <<
                    methodName << " in " << _className << std::endl;
            exit(EXIT_FAILURE);
        }

        return method;
    }

    template <class ...Args>
    jobject NewObject(Method method, Args... args) {
        return _vm->GetEnv()->NewObject(_clazz, method, args...);
    }

    template <class ...Args>
    void CallVoidMethod(jobject object, Method method, Args... args) {
        _vm->GetEnv()->CallVoidMethod(object, method, args...);
    }

    template <class ...Args>
    jstring CallStringMethod(jobject object, Method method, Args... args) {
        return (jstring)(_vm->GetEnv()->CallObjectMethod(object, method, args...));
    }

    template <class ArrayType, class ...Args>
    ArrayType CallArrayMethod(jobject object, Method method, Args... args) {
        return (ArrayType)(_vm->GetEnv()->CallObjectMethod(object, method, args...));
    }

    const VirtualMachine* GetVM() const {
        return _vm;
    }

    VirtualMachine* GetVM() {
        return _vm;
    }

    void SetVM(VirtualMachine* vm) {
        _env = _vm->GetEnv();
    }

    const std::string& GetClassName() const {
        return _className;
    }

    void SetClassName(const std::string& className) {
        _className = className;
    }

    const jclass GetClass() const {
        return _clazz;
    }

    jclass GetClass() {
        return _clazz;
    }

    void SetClass(jclass clazz) {
        _clazz = clazz;
    }

private:
    VirtualMachine* _vm;
    JNIEnv* _env;
    std::string _className;
    jclass _clazz;
};

}
}
