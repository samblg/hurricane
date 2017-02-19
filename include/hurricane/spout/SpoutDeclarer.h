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

#include "hurricane/task/TaskDeclarer.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace hurricane {
namespace spout {
class ISpout;

class SpoutDeclarer : public hurricane::task::TaskDeclarer {
public:
    SpoutDeclarer() = default;
    SpoutDeclarer(const std::string& spoutName, ISpout* spout);

    SpoutDeclarer& ParallismHint(int32_t parallismHint) {
        SetParallismHint(parallismHint);

        return *this;
    }

    std::shared_ptr<ISpout> GetSpout() const {
        return _spout;
    }

    const std::vector<std::string>& GetFields() const {
        return _fields;
    }

    const std::map<std::string, int32_t>& GetFieldsMap() const {
        return _fieldsMap;
    }

private:
    std::shared_ptr<ISpout> _spout;
    std::vector<std::string> _fields;
    std::map<std::string, int32_t> _fieldsMap;
};

}
}
