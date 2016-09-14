#include "hurricane/spout/SpoutDeclarer.h"
#include "hurricane/spout/ISpout.h"

namespace hurricane {
	namespace spout {
		SpoutDeclarer::SpoutDeclarer(const std::string& spoutName, ISpout* spout) :
			_spout(spout){
			SetType(hurricane::task::TaskDeclarer::Type::Spout);
			SetTaskName(spoutName);
		}
	}
}