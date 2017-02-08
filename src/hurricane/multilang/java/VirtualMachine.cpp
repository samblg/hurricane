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

#include "hurricane/multilang/java/VirtualMachine.h"
#include "hurricane/multilang/java/Class.h"
#include "hurricane/util/StringUtil.h"
#include "logging/Logging.h"

#include <cstring>
#include <iostream>
#include <thread>

namespace hurricane {
namespace java {

VirtualMachine* VirtualMachine::_DefaultVM = nullptr;

VirtualMachine::~VirtualMachine()
{
    if ( _vm ) {
        _vm->DestroyJavaVM();
        _vm = nullptr;
    }

    if ( _env ) {
        _env = nullptr;
    }
}

void VirtualMachine::AddClassPath(const std::string& classPath) {
    _classPaths.push_back(classPath);
}

void VirtualMachine::Start()
{
    std::string classPathArg = "-Djava.class.path=" + JoinString(_classPaths, ':');
    JavaVMOption options[1];
    char* optionString = new char[classPathArg.size() + 1];
    strncpy(optionString, classPathArg.c_str(), classPathArg.size());
    optionString[classPathArg.size()] = '\0';
    options[0].optionString = optionString;

    JavaVMInitArgs vmArgs;
    memset(&vmArgs, 0, sizeof(vmArgs));
    vmArgs.version = _version;
    vmArgs.nOptions = 1;
    vmArgs.options = options;

    jint status = JNI_CreateJavaVM(&_vm, (void**)&_env, &vmArgs);
    LOG(LOG_DEBUG) << "VM: " << _vm;
    LOG(LOG_DEBUG) << "This: " << this;

    delete [] optionString;

    if ( status == JNI_ERR ) {
        LOG(LOG_FATAL) << "JVM Created failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Class VirtualMachine::FindClass(const std::string& className)
{
    LOG(LOG_DEBUG) << "Find class: " << className;
    LOG(LOG_DEBUG) << "Current thread id: " <<std::this_thread::get_id();
    JNIEnv* threadEnv = GetEnv();

    jclass clazz = threadEnv->FindClass(className.c_str());
    if ( clazz == 0 ) {
        LOG(LOG_FATAL) << "Can't find class " << className << std::endl;
        exit(EXIT_FAILURE);
    }

    return Class(this, className, clazz);
}


JNIEnv* VirtualMachine::GetEnv() {
    JNIEnv* threadEnv = nullptr;
    _vm->AttachCurrentThread((void**)&threadEnv, nullptr);

    return threadEnv;
}

}
}
