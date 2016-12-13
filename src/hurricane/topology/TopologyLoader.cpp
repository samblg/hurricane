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

#include "hurricane/topology/TopologyLoader.h"
#include "hurricane/base/Library.h"
#include "logging/Logging.h"

namespace hurricane {
    namespace topology {
        const std::string& GET_TOPOLOGY_INTERFACE = "GetTopology";

        TopologyLoader& TopologyLoader::GetInstance() {
            static TopologyLoader instance;

            return instance;
        }

        std::shared_ptr<Topology> TopologyLoader::GetTopology(const std::string& topologyName) {
            if ( _libraryHandles.find(topologyName) == _libraryHandles.end() ) {
                LibraryHandle libraryHandle = HurricaneLibraryLoad(topologyName);
                _libraryHandles[topologyName] = libraryHandle;
                TopologyGetter topologyGetter =
                    HurricaneLibraryGetSymbol<TopologyGetter>(libraryHandle, GET_TOPOLOGY_INTERFACE);

                _topologies[topologyName].reset(topologyGetter());
            }

            return _topologies[topologyName];
        }
    }
}
