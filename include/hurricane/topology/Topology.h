#pragma once

#include "hurricane/spout/SpoutDeclarer.h"
#include "hurricane/bolt/BoltDeclarer.h"

#include <memory>
#include <map>
#include <string>

namespace hurricane {
	namespace spout {
		class ISpout;
	}

	namespace bolt {
		class IBolt;
	}

	namespace topology {
		class Topology {
		public:
			Topology(const std::string& name);

			hurricane::spout::SpoutDeclarer& SetSpout(const std::string& spoutName, hurricane::spout::ISpout* spout);
			hurricane::bolt::BoltDeclarer& SetBolt(const std::string& boltName, hurricane::bolt::IBolt* bolt);

			const std::string& GetName() const {
				return _name;
			}

            const std::map<std::string, hurricane::spout::SpoutDeclarer>& GetSpoutDeclarers() const {
                return _spoutDeclarers;
            }

            const std::map<std::string, hurricane::bolt::BoltDeclarer>& GetBoltDeclarers() const {
                return _boltDeclarers;
            }

		private:
			std::string _name;
			std::map<std::string, hurricane::spout::SpoutDeclarer> _spoutDeclarers;
			std::map<std::string, hurricane::bolt::BoltDeclarer> _boltDeclarers;
		};
	}
}