/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#pragma once

#include "hurricane/base/DataPackage.h"
#include <memory>

namespace meshy {
    class TcpConnection;
}

namespace hurricane {
	namespace message {
		class Command {
		public:
			class Type {
			public:
				enum Values {
					Invalid = 0,
					Join = 1,
					Alive = 2,
					Leave = 3,
					StartTopology = 4,
					ShutdownTopology = 5,
					StartSpout = 6,
					StopSpout = 7,
					StartBolt = 8,
					StopBolt = 9,
					RandomDestination = 10,
					GroupDestination = 11,
					Response = 254,
					Data = 255
				};
			};

			Command() : _type(Command::Type::Invalid) {}

			Command(Command::Type::Values type, const hurricane::base::Variants& args) : 
					_type(type), _args(args) {
			}

			Command(const hurricane::base::DataPackage& dataPackage) {
				const hurricane::base::Variants variants = dataPackage.GetVariants();
				_type = Command::Type::Values(variants[0].GetIntValue());
				_args = variants;
				_args.erase(_args.begin());
			}

			hurricane::base::DataPackage ToDataPackage() const {
				hurricane::base::DataPackage dataPackage;
				dataPackage.AddVariant(int(_type));
				for ( auto arg : _args ) {
					dataPackage.AddVariant(arg);
				}

				return dataPackage;
			}

			Command::Type::Values GetType() const {
				return _type;
			}

			void SetType(Command::Type::Values type) {
				_type = type;
			}

			const hurricane::base::Variant& GetArg(int index) const {
				return _args[index];
			}

			hurricane::base::Variant& GetArg(int index) {
				return _args[index];
			}

			const hurricane::base::Variants& GetArgs() const {
				return _args;
			}

			void SetArgs(const hurricane::base::Variants& args) {
				_args = args;
			}

			void PushArg(const hurricane::base::Variant& arg) {
				_args.push_back(arg);
			}

			const hurricane::base::Variant PopArg() {
				hurricane::base::Variant arg = _args.back();
				_args.pop_back();

				return arg;
			}

			std::shared_ptr<meshy::TcpConnection> GetSrc() const {
				return _src;
			}

            void SetSrc(std::shared_ptr<meshy::TcpConnection> src) {
				_src = src;
			}

		private:
			Command::Type::Values _type;
			hurricane::base::Variants _args;
            std::shared_ptr<meshy::TcpConnection> _src;
		};
	}
}