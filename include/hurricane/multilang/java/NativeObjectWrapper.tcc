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

template <class CppClass, class... Args>
void NativeObjectWrapper::CreateObject(Args... args) {
    CppClass* calleeObject = new CppClass(args...);
    jlong calleeHandle = reinterpret_cast<int64_t>(calleeObject);

    jclass cls = _env->GetObjectClass(_object);
    jfieldID nativeHandleFieldId = _env->GetFieldID(cls, "nativeHandle", "J");
    if ( !nativeHandleFieldId ) {
        LOG(LOG_FATAL) << "Can't find field nativeHandle in class " << GetClassName() << std::endl;
    }

    _env->SetLongField(_object, nativeHandleFieldId, calleeHandle);
}

template <class CppClass>
void NativeObjectWrapper::DestroyObject() {
    jclass cls = _env->GetObjectClass(_object);
    jfieldID nativeHandleFieldId = _env->GetFieldID(cls, "nativeHandle", "J");
    if ( !nativeHandleFieldId ) {
        LOG(LOG_FATAL) << "Can't find field nativeHandle in class " << GetClassName() << std::endl;
    }

    jlong calleeHandle = _env->GetLongField(_object, nativeHandleFieldId);
    CppClass* calleeObject = reinterpret_cast<CppClass*>(calleeHandle);

    if ( calleeObject ) {
        delete calleeObject;
        calleeHandle = 0;
        _env->SetLongField(_object, nativeHandleFieldId, calleeHandle);
    }
}

template <class CppClass, class ResultType, class Method, class... Args>
ResultType NativeObjectWrapper::CallMethod(Method methodPointer, Args... args) {
    jclass cls = _env->GetObjectClass(_object);
    jfieldID nativeHandleFieldId = _env->GetFieldID(cls, "nativeHandle", "J");
    if ( !nativeHandleFieldId ) {
        LOG(LOG_FATAL) << "Can't find field nativeHandle in class " << GetClassName() << std::endl;
    }

    jlong calleeHandle = _env->GetLongField(_object, nativeHandleFieldId);
    CppClass* calleeObject = reinterpret_cast<CppClass*>(calleeHandle);

    return FromNative((calleeObject->*methodPointer)(args...));
}

template <class CppClass, class Method, class... Args>
void NativeObjectWrapper::CallVoidMethod(Method methodPointer, Args... args) {
    jclass cls = _env->GetObjectClass(_object);
    jfieldID nativeHandleFieldId = _env->GetFieldID(cls, "nativeHandle", "J");
    if ( !nativeHandleFieldId ) {
        LOG(LOG_FATAL) << "Can't find field nativeHandle in class " << GetClassName() << std::endl;
    }

    jlong calleeHandle = _env->GetLongField(_object, nativeHandleFieldId);
    CppClass* calleeObject = reinterpret_cast<CppClass*>(calleeHandle);

    (calleeObject->*methodPointer)(args...);
}

}
}
