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

#include "hurricane/bolt/BoltDeclarer.h"
#include "hurricane/bolt/IBolt.h"

namespace hurricane {
namespace bolt {

BoltDeclarer::BoltDeclarer(const std::string& boltName, IBolt* bolt) :
    _bolt(bolt){
    SetType(hurricane::task::TaskDeclarer::Type::Bolt);
    SetTaskName(boltName);

    _fields = _bolt->DeclareFields();
    int32_t fieldIndex = 0;
    for ( const std::string& field : _fields ) {
        _fieldsMap.insert({field, fieldIndex});
    }
}

BoltDeclarer& BoltDeclarer::ParallismHint(int32_t parallismHint) {
    SetParallismHint(parallismHint);

    return *this;
}

BoltDeclarer& BoltDeclarer::Global(const std::string& sourceTaskName) {
    SetSourceTaskName(sourceTaskName);
    SetGroupMethod(hurricane::task::TaskDeclarer::GroupMethod::Global);

    return *this;
}

BoltDeclarer& BoltDeclarer::Field(const std::string& sourceTaskName, const std::string& groupField) {
    SetSourceTaskName(sourceTaskName);
    SetGroupMethod(hurricane::task::TaskDeclarer::GroupMethod::Field);
    SetGroupField(groupField);

    return *this;
}

BoltDeclarer& BoltDeclarer::Random(const std::string& sourceTaskName) {
    SetSourceTaskName(sourceTaskName);
    SetGroupMethod(hurricane::task::TaskDeclarer::GroupMethod::Random);

    return *this;
}

}
}
