#include "hurricane/topology/TopologyLoader.h"
#include "sample/wordcount/WordCountTopology.h"

namespace hurricane {
	namespace topology {
		TopologyLoader& TopologyLoader::GetInstance() {
			static TopologyLoader instance;

			return instance;
		}

		TopologyLoader::TopologyLoader() {
			Topology* topology = ::GetTopology();

			_topologies.insert({ topology->GetName(), std::shared_ptr<Topology>(topology) });
		}

		std::shared_ptr<Topology> TopologyLoader::GetTopology(const std::string& topologyName) {
			return _topologies[topologyName];
		}
	}
}