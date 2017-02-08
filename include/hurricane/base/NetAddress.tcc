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

namespace hurricane {
namespace base {

NetAddress::NetAddress() : _port(0) {
}

NetAddress::NetAddress(const std::string& host, int32_t port) : _host(host), _port(port) {
}

void NetAddress::Serialize(Variants& variants) const {
    Variant::Serialize(variants, _host);
    Variant::Serialize(variants, _port);
}

void NetAddress::Deserialize(Variants::const_iterator& it) {
    Variant::Deserialize(it, _host);
    Variant::Deserialize(it, _port);
}

}
}
