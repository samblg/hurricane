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

#include "hurricane/multilang/java/String.h"
#include "hurricane/multilang/java/Array.h"
#include "hurricane/multilang/java/Class.h"

#include <jni.h>
#include <string>

namespace hurricane {
namespace bolt {

class JavaBolt {
public:
    const static std::string ClassName;
    static java::Class& GetDefaultClass();

    JavaBolt(java::Class clazz, const std::string& className);
    JavaBolt(const std::string& className);

    JavaBolt* Clone();
    void Prepare();
    void Cleanup();
    void NextTuple();
    std::vector<std::string> DeclareFields();

private:
    java::Class _clazz;
    std::string _className;
    jobject _object;

    java::Method _constructMethod;
    java::Method _prepareMethod;
    java::Method _cleanupMethod;
    java::Method _nextTupleMethod;
    java::Method _declareFieldsMethod;
};

}
}
