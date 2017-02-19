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

#include <map>
#include <string>
#include <cstdint>
#include <sstream>

namespace hurricane {
namespace util {
class Configuration {
public:
    const std::string& GetProperty(const std::string& propertyName) const {
        return _properties.at(propertyName);
    }

    const int32_t GetIntegerProperty(const std::string& propertyName) const {
        return std::stoi(_properties.at(propertyName));
    }

    const bool GetBooleanProperty(const std::string& propertyName) const {
        std::string propertyValue = _properties.at(propertyName);
        if ( propertyValue == "true" ) {
            return true;
        }

        return false;
    }

    void SetProperty(const std::string& propertyName, const std::string& propertyValue) {
        _properties[propertyName] = propertyValue;
    }

    void SetProperty(const std::string& propertyName, int32_t propertyValue) {
        std::ostringstream os;
        os << propertyValue;
        _properties[propertyName] = os.str();
    }

    void Parse(const std::string& fileName);

private:
    std::map<std::string, std::string> _properties;
};
}
}
