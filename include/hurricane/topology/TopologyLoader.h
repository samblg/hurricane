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

#include "hurricane/topology/Topology.h"
#include "hurricane/base/Library.h"
#include <map>
#include <string>
#include <memory>

namespace hurricane {
namespace topology {
class Topology;

class TopologyLoader {
public:
    typedef Topology* (*TopologyGetter)();

    static TopologyLoader& GetInstance();
    std::shared_ptr<Topology> GetTopology(const std::string& name);

private:
    TopologyLoader() {}
    TopologyLoader(const TopologyLoader& loader) = delete;
    const TopologyLoader& operator = (const TopologyLoader& loader) = delete;

    std::map<std::string, std::shared_ptr<Topology>> _topologies;
    std::map<std::string, LibraryHandle> _libraryHandles;
};
}
}
