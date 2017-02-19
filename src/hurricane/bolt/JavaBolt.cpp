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

#include "hurricane/bolt/JavaBolt.h"
#include "hurricane/collector/JavaOutputCollector.h"
#include "hurricane/multilang/java/VirtualMachine.h"
#include "hurricane/multilang/java/Signature.h"
#include "hurricane/multilang/java/NativeObjectWrapper.h"

namespace hurricane {
namespace bolt {

using java::Object;
using java::Array;
using java::String;
using java::Class;
using java::Method;
using java::VirtualMachine;
using java::Signature;
using collector::JavaOutputCollector;

const std::string JavaBolt::ClassName = "hurricane/jni/IBolt";

static const std::string ConstructorMethodName = "<init>";
static const std::string PrepareMethodName = "prepare";
static const std::string CleanupMethodName = "cleanup";
static const std::string ExecuteMethodName = "execute";
static const std::string DeclareFieldsMethodName = "declareFields";

Class& JavaBolt::GetDefaultClass()
{
    static Class boltClass = VirtualMachine::GetDefault()->FindClass(JavaBolt::ClassName);

    return boltClass;
}

JavaBolt::JavaBolt(java::Class clazz, const std::string& className) :
    _clazz(clazz), _className(className)
{
    std::string constructSig = Signature::ForMethod<void>();
    std::string prepareSig = Signature::ForMethod<void, JavaOutputCollector>(
                HJARG(JavaOutputCollector));
    std::string cleanupSig = Signature::ForMethod<void>();
    std::string executeSig = Signature::ForMethod<void, Array<Object>>(HJARG(Array<Object>));
    std::string declareFieldsSig = Signature::ForMethod<Array<String>>();

    _constructMethod = _clazz.GetMethod(ConstructorMethodName, constructSig);
    _prepareMethod = _clazz.GetMethod(PrepareMethodName, prepareSig);
    _cleanupMethod = _clazz.GetMethod(CleanupMethodName, cleanupSig);
    _executeMethod = _clazz.GetMethod(ExecuteMethodName, executeSig);
    _declareFieldsMethod = _clazz.GetMethod(DeclareFieldsMethodName, declareFieldsSig);

    _object = _clazz.NewObject(_constructMethod);
}

JavaBolt::JavaBolt(const std::string& className) :
        JavaBolt(VirtualMachine::GetDefault()->FindClass(className), className)
{
}

JavaBolt* JavaBolt::Clone()
{
    return new JavaBolt(_className);
}

void JavaBolt::Prepare(std::shared_ptr<collector::OutputCollector> outputCollector)
{
    _innerCollector = outputCollector;
    _collector = new JavaOutputCollector(outputCollector.get());
    _clazz.CallVoidMethod(_object, _prepareMethod, _collector->GetObject());
}

void JavaBolt::Cleanup()
{
    _clazz.CallVoidMethod(_object, _cleanupMethod);
}

void JavaBolt::Execute(const base::Tuple& tuple)
{
    std::cout << "Execute" << std::endl;
    JNIEnv* env = VirtualMachine::GetDefault()->GetEnv();
    jclass clazz = env->FindClass("java/lang/Object");

    size_t arraySize = tuple.GetSize();
    jobjectArray array = env->NewObjectArray(arraySize, clazz, nullptr);
    const base::Values& values = tuple.GetValues();

    for ( int i = 0; i != arraySize; ++ i ) {
        const base::Value& value = values[i];
        jobject obj = nullptr;

        switch ( value.GetType() ) {
        case base::Value::Type::Boolean:
            obj = java::ToBoolean(env, value.GetBooleanValue());
            break;
        case base::Value::Type::Int32:
            obj = java::ToBoolean(env, value.GetBooleanValue());
            break;
        case base::Value::Type::Int64:
            obj = java::ToBoolean(env, value.GetBooleanValue());
            break;
        case base::Value::Type::Float:
            obj = java::ToBoolean(env, value.GetBooleanValue());
            break;
        case base::Value::Type::String:
            String data(value.GetStringValue());
            obj = data.ToJavaString();
            break;
        }

        env->SetObjectArrayElement(array, i, obj);
    }

    _clazz.CallVoidMethod(_object, _executeMethod, array);
}

std::vector<std::string> JavaBolt::DeclareFields()
{
    Array<String> javaFields = _clazz.CallArrayMethod<Array<String>>(
                _object, _declareFieldsMethod);

    size_t size = javaFields.GetSize();
    std::vector<std::string> fields(size);
    for ( int i = 0; i != javaFields.GetSize(); ++ i ) {
        fields[i] = javaFields[i].ToStdString();
    }

    return fields;
}

}

}
