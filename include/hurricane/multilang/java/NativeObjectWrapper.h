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

#include "logging/Logging.h"

#include <jni.h>
#include <string>
#include <iostream>
#include <cstdint>

namespace hurricane {
namespace java {

class NativeObjectWrapper {
public:
    NativeObjectWrapper(JNIEnv* env, jobject object);

    template <class CppClass, class... Args>
    void CreateObject(Args... args);

    template <class CppClass>
    void DestroyObject();

    template <class CppClass, class ResultType, class Method, class... Args>
    ResultType CallMethod(Method methodPointer, Args... args);

    template <class CppClass, class Method, class... Args>
    void CallVoidMethod(Method methodPointer, Args... args);

    std::string ToNative(jstring javaValue) {
        return _env->GetStringUTFChars(javaValue, 0);
    }

    jstring FromNative(const std::string nativeValue) {
        return _env->NewStringUTF(nativeValue.c_str());
    }

    std::string GetClassName() {
        jclass cls = _env->GetObjectClass(_object);
        jobject clsObj = (jobject)cls;
        jclass clsCls = _env->GetObjectClass(clsObj);
        jmethodID classNameMethodId = _env->GetMethodID(clsCls, "getName", "()Ljava/lang/String;");
        std::string className = ToNative((jstring)(_env->CallObjectMethod(clsObj, classNameMethodId)));

        return className;
    }

    static std::string GetClassName(JNIEnv* env, jclass cls) {
        jobject clsObj = (jobject)cls;
        jclass clsCls = env->GetObjectClass(clsObj);
        jmethodID classNameMethodId = env->GetMethodID(clsCls, "getName", "()Ljava/lang/String;");
        std::string className =
                env->GetStringUTFChars(((jstring)(env->CallObjectMethod(clsObj, classNameMethodId))), 0);

        return className;
    }

private:
    JNIEnv* _env;
    jobject _object;
};

const static std::string JAVA_BOOLEAN_CLASS = "java.lang.Boolean";
const static std::string JAVA_BYTE_CLASS = "java.lang.Byte";
const static std::string JAVA_CHAR_CLASS = "java.lang.Character";
const static std::string JAVA_SHORT_CLASS = "java.lang.Short";
const static std::string JAVA_INT_CLASS = "java.lang.Integer";
const static std::string JAVA_LONG_CLASS = "java.lang.Long";
const static std::string JAVA_FLOAT_CLASS = "java.lang.Float";
const static std::string JAVA_DOUBLE_CLASS = "java.lang.Double";
const static std::string JAVA_STRING_CLASS = "java.lang.String";

#define DECLARE_JNI_TYPE(JavaClass, CppType) \
    CppType From##JavaClass(JNIEnv* env, jobject obj); \
    jobject To##JavaClass(JNIEnv* env, CppType value);

DECLARE_JNI_TYPE(Boolean, bool)
DECLARE_JNI_TYPE(Character, uint16_t)
DECLARE_JNI_TYPE(Byte, int8_t)
DECLARE_JNI_TYPE(Short, int16_t)
DECLARE_JNI_TYPE(Integer, int32_t)
DECLARE_JNI_TYPE(Long, int64_t)
DECLARE_JNI_TYPE(Float, float)
DECLARE_JNI_TYPE(Double, double)

}
}

#include "hurricane/multilang/java/NativeObjectWrapper.tcc"
