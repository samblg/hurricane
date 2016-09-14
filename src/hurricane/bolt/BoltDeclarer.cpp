#include "hurricane/bolt/BoltDeclarer.h"
#include "hurricane/bolt/IBolt.h"

namespace hurricane {
	namespace bolt {
		BoltDeclarer::BoltDeclarer(const std::string& boltName, IBolt* bolt) :
				_bolt(bolt){
			SetType(hurricane::task::TaskDeclarer::Type::Bolt);
			SetTaskName(boltName);
		}
	}
}