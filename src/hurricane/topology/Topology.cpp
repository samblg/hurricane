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

#include "hurricane/topology/Topology.h"

namespace hurricane {
    namespace topology {
        Topology::Topology(const std::string& name) : _name(name) {
        }

        hurricane::spout::SpoutDeclarer& Topology::SetSpout(const std::string& spoutName, hurricane::spout::ISpout* spout) {
            _spoutDeclarers.insert({ spoutName, hurricane::spout::SpoutDeclarer(spoutName, spout) });

            return _spoutDeclarers[spoutName];
        }

        hurricane::bolt::BoltDeclarer& Topology::SetBolt(const std::string& boltName, hurricane::bolt::IBolt* bolt) {
            _boltDeclarers.insert({ boltName, hurricane::bolt::BoltDeclarer(boltName, bolt) });

            return _boltDeclarers[boltName];
        }
    }
}
