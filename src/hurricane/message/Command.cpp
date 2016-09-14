#include "hurricane/message/Command.h"
#include "hurricane/base/DataPackage.h"
#include <iostream>

namespace hurricane {
	namespace message {
		void Command::Deserialize(const hurricane::base::ByteArray& data) {
			hurricane::base::DataPackage dataPackage;
			dataPackage.Deserialize(data);

			if ( !dataPackage.GetVariants().size() ) {
				std::cerr << "Data package error";
			}

			_arguments = dataPackage.GetVariants();
			_type = _arguments[0].GetIntValue();
			_arguments.erase(_arguments.begin());
		}

		hurricane::base::ByteArray Command::Serialize() const {
			hurricane::base::DataPackage dataPackage;

			dataPackage.AddVariant(hurricane::base::Variant(_type));
			for ( const hurricane::base::Variant& argument : _arguments ) {
				dataPackage.AddVariant(argument);
			}

			return dataPackage.Serialize();
		}

		void Response::Deserialize(const hurricane::base::ByteArray& data) {
			hurricane::base::DataPackage dataPackage;
			dataPackage.Deserialize(data);

			if ( !dataPackage.GetVariants().size() ) {
				std::cerr << "Data package error";
			}

			_arguments = dataPackage.GetVariants();
			_status = _arguments[0].GetIntValue();
			_arguments.erase(_arguments.begin());
		}

		hurricane::base::ByteArray Response::Serialize() const {
			hurricane::base::DataPackage dataPackage;

			dataPackage.AddVariant(hurricane::base::Variant(_status));
			for ( const hurricane::base::Variant& argument : _arguments ) {
				dataPackage.AddVariant(argument);
			}

			return dataPackage.Serialize();
		}
	}
}