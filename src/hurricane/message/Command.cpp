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

#include "hurricane/message/Command.h"
#include "hurricane/base/DataPackage.h"
#include "logging/Logging.h"

#include <iostream>

namespace hurricane {
namespace message {

Command::Command(int32_t type) : _type(type) {
}

Command::Command(int32_t type, std::vector<hurricane::base::Variant>& arguments) :
    _type(type), _arguments(arguments) {
}

void Command::AddArgument(const hurricane::base::Variant& argument) {
    _arguments.push_back(argument);
}

void Command::AddArguments(const std::vector<hurricane::base::Variant>& arguments) {
    for ( const hurricane::base::Variant& argument : arguments ) {
        _arguments.push_back(argument);
    }
}

void Command::Deserialize(const hurricane::base::ByteArray& data) {
    hurricane::base::DataPackage dataPackage;
    dataPackage.Deserialize(data);

    if ( !dataPackage.GetVariants().size() ) {
        LOG(LOG_ERROR) << "Data package error";
    }

    _arguments = dataPackage.GetVariants();
    _type = _arguments[0].GetInt32Value();
    _arguments.erase(_arguments.begin());
}

hurricane::base::ByteArray Command::Serialize() const {
    hurricane::base::DataPackage dataPackage;

    dataPackage.AddVariant(hurricane::base::Variant(_type));
    for ( const hurricane::base::Variant& argument : _arguments ) {
        dataPackage.AddVariant(argument);
    }

    return dataPackage.Serialize();
}

Response::Response(int32_t status) : _status(status) {
}

Response::Response(int32_t status, std::vector<hurricane::base::Variant>& arguments) :
    _status(status), _arguments(arguments) {
}

void Response::AddArguments(const std::vector<hurricane::base::Variant>& arguments) {
    for ( const hurricane::base::Variant& argument : arguments ) {
        _arguments.push_back(argument);
    }
}

void Response::AddArgument(const hurricane::base::Variant& argument) {
    _arguments.push_back(argument);
}

void Response::Deserialize(const hurricane::base::ByteArray& data) {
    hurricane::base::DataPackage dataPackage;
    dataPackage.Deserialize(data);

    if ( !dataPackage.GetVariants().size() ) {
        LOG(LOG_ERROR) << "Data package error";
    }

    _arguments = dataPackage.GetVariants();
    _status = _arguments[0].GetInt32Value();
    _arguments.erase(_arguments.begin());
}

hurricane::base::ByteArray Response::Serialize() const {
    hurricane::base::DataPackage dataPackage;

    dataPackage.AddVariant(hurricane::base::Variant(_status));
    for ( const hurricane::base::Variant& argument : _arguments ) {
        dataPackage.AddVariant(argument);
    }

    return dataPackage.Serialize();
}

}
}
