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
#include "hurricane/base/Values.h"

namespace hurricane {
namespace collector {

class OutputCollector;

class JavaOutputCollector : public java::Object {
public:
    const static std::string ClassName;
    static java::Class& GetDefaultClass();

    JavaOutputCollector(java::Class clazz, OutputCollector* outputCollector = nullptr);
    JavaOutputCollector(OutputCollector* outputCollector = nullptr);

    void Emit(const base::Tuple& tuple);

    jobject GetObject() {
        return _object;
    }

private:
    java::Class _clazz;
    jobject _object;
    OutputCollector* _outputCollector;

    java::Method _constructMethod;
};

}
}

HURRICANE_JAVA_REGISTER_CLASS(hurricane::collector::JavaOutputCollector, "hurricane/jni/OutputCollector")
