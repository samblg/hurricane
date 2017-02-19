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

#include <vector>
#include <string>
#include <cstdint>

namespace hurricane {
namespace java {

class Class;

class VirtualMachine {
public:
    static VirtualMachine* GetDefault() {
        return _DefaultVM;
    }

    static void SetDefault(VirtualMachine* vm) {
        _DefaultVM = vm;
    }

public:
    VirtualMachine(int32_t version = JNI_VERSION_1_6) :
        _vm(nullptr), _env(nullptr), _version(version) {
    }

    ~VirtualMachine();

    JavaVM* GetVM() {
        return _vm;
    }

    const JavaVM* GetVM() const {
        return _vm;
    }

    JNIEnv* GetEnv();
    const JNIEnv* GetEnv() const {
        return _env;
    }

    void SetVersion(int32_t version) {
        _version = version;
    }

    int32_t GetVersion() {
        return _version;
    }

    void SetClassPaths(const std::vector<std::string>& classPaths) {
        _classPaths = classPaths;
    }

    const std::vector<std::string>& GetClassPaths() const {
        return _classPaths;
    }

    void AddClassPath(const std::string& classPath);
    void Start();
    Class FindClass(const std::string& className);

private:
    static VirtualMachine* _DefaultVM;

private:
    JavaVM* _vm;
    JNIEnv* _env;
    int32_t _version;
    std::vector<std::string> _classPaths;
};

}
}
