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
#include <jni.h>

namespace hurricane {
namespace java {

class String : public Object {
public:
    const static std::string ClassName;
    static Class& GetDefaultClass();

    String(const Class& clazz);
    String(const Class& clazz, const std::string& data);
    String();
    String(const std::string& data);
    String(jstring data);
    String(jobject data);

    const std::string& ToStdString() const {
        return _data;
    }

    void SetData(const std::string& data) {
        _data = data;
    }

    jstring ToJavaString() const;

private:
    Class _clazz;
    jstring _object;
    std::string _data;
};

}
}

HURRICANE_JAVA_REGISTER_CLASS(hurricane::java::String, "java/lang/String")
