#pragma once

#include "hurricane/topology/Topology.h"
#include <map>
#include <string>
#include <memory>

typedef void* LibraryHandle;

namespace hurricane {
	namespace topology {
		class Topology;

		class TopologyLoader {
		public:
            typedef Topology* (*TopologyGetter)();

			static TopologyLoader& GetInstance();
			std::shared_ptr<Topology> GetTopology(const std::string& name);

		private:
			TopologyLoader();
			TopologyLoader(const TopologyLoader& loader) = delete;
			const TopologyLoader& operator = (const TopologyLoader& loader) = delete;

			std::map<std::string, std::shared_ptr<Topology>> _topologies;
            std::map<std::string, LibraryHandle> _libraryHandles;
		};
	}
}
