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