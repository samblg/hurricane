#include "hurricane/topology/TopologyLoader.h"
#include "hurricane/base/Library.h"
#include "sample/wordcount/WordCountTopology.h"

namespace hurricane {
	namespace topology {
		TopologyLoader& TopologyLoader::GetInstance() {
			static TopologyLoader instance;

			return instance;
		}

        TopologyLoader::TopologyLoader() {
		}

		std::shared_ptr<Topology> TopologyLoader::GetTopology(const std::string& topologyName) {
            if ( _libraryHandles.find(topologyName) == _libraryHandles.end() ) {
                LibraryHandle libraryHandle = HurricaneLibraryLoad(topologyName);
                _libraryHandles[topologyName] = libraryHandle;
                TopologyGetter topologyGetter =
                        HurricaneLibraryGetSymbol<TopologyGetter>(libraryHandle, "GetTopology");

                std::cout << "Getter: " << topologyGetter << std::endl;
                _topologies[topologyName].reset(topologyGetter());
            }

			return _topologies[topologyName];
		}
	}
}
