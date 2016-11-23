#include "hurricane/bolt/BoltDeclarer.h"
#include "hurricane/bolt/IBolt.h"

namespace hurricane {
	namespace bolt {
		BoltDeclarer::BoltDeclarer(const std::string& boltName, IBolt* bolt) :
				_bolt(bolt){
			SetType(hurricane::task::TaskDeclarer::Type::Bolt);
			SetTaskName(boltName);

            _fields = _bolt->DeclareFields();
            int fieldIndex = 0;
            for ( const std::string& field : _fields ) {
                _fieldsMap.insert({field, fieldIndex});
            }
		}
	}
}
