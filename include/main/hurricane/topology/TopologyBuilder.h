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

#include "hurricane/topology/SimpleTopology.h"

#include <memory>
#include <map>
#include <vector>
#include <string>

namespace hurricane {
namespace topology {

class TopologyBuilder {
public:
    void SetSpout(const std::string& name, spout::ISpout* spout);
    void SetBolt(const std::string& name, bolt::IBolt* bolt, const std::string& prev);

	SimpleTopology* Build();

private:
    std::map<std::string, std::shared_ptr<spout::ISpout>> _spouts;
    std::map<std::string, std::shared_ptr<bolt::IBolt>> _bolts;
    std::map<std::string, std::vector<std::string>> _network;
};

}
}
