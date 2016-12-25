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
        NetAddress() : _port(0) {
        }

        NetAddress(const std::string& host, int32_t port) : _host(host), _port(port) {
        }

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

        virtual void Serialize(Variants& variants) const override {
            Variant::Serialize(variants, _host);
            Variant::Serialize(variants, _port);
        }

        virtual void Deserialize(Variants::const_iterator& it) override {
            Variant::Deserialize(it, _host);
            Variant::Deserialize(it, _port);
        }

    private:
        std::string _host;
        int32_t _port;
    };

}
}
