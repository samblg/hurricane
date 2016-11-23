#include "hurricane/spout/SpoutDeclarer.h"
#include "hurricane/spout/ISpout.h"

namespace hurricane {
	namespace spout {
		SpoutDeclarer::SpoutDeclarer(const std::string& spoutName, ISpout* spout) :
			_spout(spout){
			SetType(hurricane::task::TaskDeclarer::Type::Spout);
			SetTaskName(spoutName);

            _fields = _spout->DeclareFields();
            int fieldIndex = 0;
            for ( const std::string& field : _fields ) {
                _fieldsMap.insert({field, fieldIndex});
            }
		}
	}
}
