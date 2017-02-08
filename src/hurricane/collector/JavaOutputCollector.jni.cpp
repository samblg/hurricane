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

#include "hurricane/collector/JavaOutputCollector.jni.h"
#include "hurricane/collector/JavaOutputCollector.h"
#include "hurricane/multilang/java/NativeObjectWrapper.h"
#include "hurricane/multilang/java/String.h"
#include "hurricane/multilang/java/Signature.h"
#include "hurricane/multilang/java/SignatureImpl.h"
#include "hurricane/base/Values.h"

using hurricane::java::NativeObjectWrapper;
using hurricane::java::Signature;
using hurricane::java::String;
using hurricane::collector::JavaOutputCollector;
using hurricane::base::Tuple;
using hurricane::java::ToBoolean;
using hurricane::java::FromBoolean;
using hurricane::java::ToCharacter;
using hurricane::java::FromCharacter;
using hurricane::java::ToByte;
using hurricane::java::FromByte;
using hurricane::java::ToShort;
using hurricane::java::FromShort;
using hurricane::java::ToInteger;
using hurricane::java::FromInteger;
using hurricane::java::ToLong;
using hurricane::java::FromLong;
using hurricane::java::ToFloat;
using hurricane::java::FromFloat;
using hurricane::java::ToDouble;
using hurricane::java::FromDouble;
using hurricane::java::JAVA_BOOLEAN_CLASS;
using hurricane::java::JAVA_BYTE_CLASS;
using hurricane::java::JAVA_CHAR_CLASS;
using hurricane::java::JAVA_SHORT_CLASS;
using hurricane::java::JAVA_INT_CLASS;
using hurricane::java::JAVA_LONG_CLASS;
using hurricane::java::JAVA_FLOAT_CLASS;
using hurricane::java::JAVA_DOUBLE_CLASS;
using hurricane::java::JAVA_STRING_CLASS;

JNIEXPORT void JNICALL Java_hurricane_jni_OutputCollector_emit
  (JNIEnv* env, jobject object, jobjectArray args) {
    NativeObjectWrapper hjni(env, object);
    Tuple tuple;

    jsize argSize = env->GetArrayLength(args);
    for ( int argIndex = 0; argIndex != argSize; ++ argIndex ) {
        jobject arg = env->GetObjectArrayElement(args, argIndex);
        jclass cls = env->GetObjectClass(arg);
        std::string className = NativeObjectWrapper::GetClassName(env, cls);

        if ( className == JAVA_BOOLEAN_CLASS ) {
            tuple.Add({ FromBoolean(env, object) });
        }
        else if ( className == JAVA_CHAR_CLASS ) {
            tuple.Add({ FromCharacter(env, object) });
        }
        else if ( className == JAVA_BYTE_CLASS ) {
            tuple.Add({ FromByte(env, object) });
        }
        else if ( className == JAVA_SHORT_CLASS ) {
            tuple.Add({ FromShort(env, object) });
        }
        else if ( className == JAVA_INT_CLASS ) {
            tuple.Add({ FromInteger(env, object) });
        }
        else if ( className == JAVA_LONG_CLASS ) {
            tuple.Add({ FromLong(env, object) });
        }
        else if ( className == JAVA_FLOAT_CLASS ) {
            tuple.Add({ FromFloat(env, object) });
        }
        else if ( className == JAVA_STRING_CLASS ) {
            String data(object);
            tuple.Add({ data.ToStdString() });
        }
    }

    hjni.CallVoidMethod<JavaOutputCollector>(&JavaOutputCollector::Emit, tuple);
}
