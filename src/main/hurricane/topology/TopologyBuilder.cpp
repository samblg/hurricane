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
#include "hurricane/Hurricane.h"

#include "hurricane/topology/TopologyBuilder.h"
#include "hurricane/spout/ISpout.h"
#include "hurricane/bolt/IBolt.h"

#include <memory>

namespace hurricane {
namespace topology {

void TopologyBuilder::SetSpout(const std::string& name, spout::ISpout* spout) {
    _spouts[name] = std::shared_ptr<spout::ISpout>(spout);
}

void TopologyBuilder::SetBolt(const std::string& name, bolt::IBolt* bolt, const std::string& prev) {
    _bolts[name] = std::shared_ptr<bolt::IBolt>(bolt);

    auto desinations = _network.find(prev);
    if ( desinations == _network.end() ) {
        desinations = _network.insert(make_pair(prev, std::vector<std::string>())).first;
    }

    desinations->second.push_back(name);
}

SimpleTopology* TopologyBuilder::Build() {
    SimpleTopology* topology = new SimpleTopology;
    topology->SetSpouts(_spouts);
    topology->SetBolts(_bolts);
    topology->SetNetwork(_network);

    return topology;
}

}
}