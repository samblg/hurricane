#pragma once

#include <string>
#include <vector>
#include "hurricane/base/ByteArray.h"
#include "hurricane/base/Variant.h"

namespace hurricane {
	namespace message {
		class Command {
		public:
			struct Type {
				enum {
					Invalid = 0,
                    Join,
                    Heartbeat,
                    SyncMetadata,
<<<<<<< HEAD
                    SendTuple
=======
                    SendTuple,
                    AskField
>>>>>>> master
				};
			};

			Command(int32_t type = Type::Invalid) : _type(type) {
			}

			Command(int32_t type, std::vector<hurricane::base::Variant>& arguments) :
					_type(type), _arguments(arguments) {
			}

			int32_t GetType() const {
				return _type;
			}

			void SetType(int32_t type) {
				_type = type;
			}

			hurricane::base::Variant GetArgument(int index) const {
				return _arguments[index];
			}

			int32_t GetArgumentCount() const {
				return _arguments.size();
			}

			const std::vector<hurricane::base::Variant>& GetArguments() const {
				return _arguments;
			}

			void AddArgument(const hurricane::base::Variant& argument) {
				_arguments.push_back(argument);
			}

            void AddArguments(const std::vector<hurricane::base::Variant>& arguments) {
                for ( const hurricane::base::Variant & argument : arguments ) {
                    _arguments.push_back(argument);
                }
            }

			void Deserialize(const hurricane::base::ByteArray& data);
			hurricane::base::ByteArray Serialize() const;

		private:
			int32_t _type;
			std::vector<hurricane::base::Variant> _arguments;
		};

		class Response {
		public:
			struct Status {
				enum {
					Failed = 0,
					Successful = 1
				};
			};

			Response(int32_t status = Status::Failed) : _status(status) {
			}

			Response(int32_t status, std::vector<hurricane::base::Variant>& arguments) :
				_status(status), _arguments(arguments) {
			}

			int32_t GetStatus() const {
				return _status;
			}

			void SetStatus(int32_t status) {
				_status = status;
			}

			hurricane::base::Variant GetArgument(int index) const {
				return _arguments[index];
			}

			int32_t GetArgumentCount() const {
				return _arguments.size();
			}

			const std::vector<hurricane::base::Variant>& GetArguments() const {
				return _arguments;
			}

            void AddArguments(const std::vector<hurricane::base::Variant>& arguments) {
                for ( const hurricane::base::Variant & argument : arguments ) {
                    _arguments.push_back(argument);
                }
            }

			void AddArgument(const hurricane::base::Variant& argument) {
				_arguments.push_back(argument);
			}

			void Deserialize(const hurricane::base::ByteArray& data);
			hurricane::base::ByteArray Serialize() const;

		private:
			int32_t _status;
			std::vector<hurricane::base::Variant> _arguments;
		};
	}
}
