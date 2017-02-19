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

#include "hurricane/multilang/java/NativeObjectWrapper.h"
#include "hurricane/multilang/java/String.h"
#include "hurricane/multilang/java/Signature.h"
#include "hurricane/multilang/java/SignatureImpl.h"

namespace hurricane {
namespace java {

NativeObjectWrapper::NativeObjectWrapper(JNIEnv* env, jobject object) : _env(env), _object(object) {
}

}
}

#define IMPLEMENT_JNI_TYPE(JavaType, JavaTypePath, CppType, valueType) \
    CppType From##JavaType(JNIEnv* env, jobject obj) { \
        jclass clazz = env->FindClass(JavaTypePath); \
        static const std::string methodSig = Signature::ForMethod<CppType>(); \
        static const std::string methodName = #valueType"Value"; \
        jmethodID methodID = env->GetMethodID(clazz, methodName.c_str(), methodSig.c_str()); \
    \
        return CppType(env->Call##JavaType##Method(obj, methodID)); \
    } \
    \
    jobject To##JavaType(JNIEnv* env, CppType value) { \
        jclass clazz = env->FindClass(JavaTypePath); \
        static const std::string methodSig = Signature::ForMethod<void, CppType>((CppType*)(0)); \
        static const std::string methodName = "<init>"; \
        jmethodID methodID = env->GetMethodID(clazz, methodName.c_str(), methodSig.c_str()); \
    \
        jobject obj = env->NewObject(clazz, methodID, j##valueType(value)); \
    \
        return obj; \
    }

#define IMPLEMENT_JNI_TYPE_ALIAS(JavaType, JavaTypePath, CppType, valueType, JniAlias) \
    CppType From##JavaType(JNIEnv* env, jobject obj) { \
        jclass clazz = env->FindClass(JavaTypePath); \
        static const std::string methodSig = Signature::ForMethod<CppType>(); \
        static const std::string methodName = #valueType"Value"; \
        jmethodID methodID = env->GetMethodID(clazz, methodName.c_str(), methodSig.c_str()); \
    \
        return CppType(env->Call##JniAlias##Method(obj, methodID)); \
    } \
    \
    jobject To##JavaType(JNIEnv* env, CppType value) { \
        jclass clazz = env->FindClass(JavaTypePath); \
        static const std::string methodSig = Signature::ForMethod<void, CppType>((CppType*)(0)); \
        static const std::string methodName = "<init>"; \
        jmethodID methodID = env->GetMethodID(clazz, methodName.c_str(), methodSig.c_str()); \
    \
        jobject obj = env->NewObject(clazz, methodID, value); \
    \
        return obj; \
    }

namespace hurricane {
namespace java {

IMPLEMENT_JNI_TYPE(Boolean, "java/lang/Boolean", bool, boolean)
IMPLEMENT_JNI_TYPE_ALIAS(Character, "java/lang/Character", uint16_t, character, Char)
IMPLEMENT_JNI_TYPE(Byte, "java/lang/Byte", int8_t, byte)
IMPLEMENT_JNI_TYPE(Short, "java/lang/Short", int16_t, short)
IMPLEMENT_JNI_TYPE_ALIAS(Integer, "java/lang/Integer", int32_t, int, Int)
IMPLEMENT_JNI_TYPE(Long, "java/lang/Long", int64_t, long)
IMPLEMENT_JNI_TYPE(Float, "java/lang/Float", float, float)
IMPLEMENT_JNI_TYPE(Double, "java/lang/Double", double, double)

}
}
