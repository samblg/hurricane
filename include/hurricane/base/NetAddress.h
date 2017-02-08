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

#include <string>
#include "hurricane/base/Variant.h"

namespace hurricane {
namespace base {

class NetAddress : public Serializable {
public:
    inline NetAddress();
    inline NetAddress(const std::string& host, int32_t port);
    inline void Serialize(Variants& variants) const override;
    inline void Deserialize(Variants::const_iterator& it) override;

    const std::string& GetHost() const {
        return _host;
    }

    void SetHost(const std::string& host) {
        _host = host;
    }

    int32_t GetPort() const {
        return _port;
    }

    void SetPort(int32_t port) {
        _port = port;
    }

private:
    std::string _host;
    int32_t _port;
};

}
}

#include "hurricane/base/NetAddress.tcc"
