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

#include "hurricane/multilang/java/Class.h"
#include "hurricane/multilang/java/VirtualMachine.h"

#include <iostream>

namespace hurricane {
namespace java {

Class Class::ForName(const std::string& className)
{
    return VirtualMachine::GetDefault()->FindClass(className);
}

Class::Class(VirtualMachine* vm, const std::string& className, jclass clazz) :
    _vm(vm), _env(vm->GetEnv()), _className(className), _clazz(clazz)
{
}

}
}
