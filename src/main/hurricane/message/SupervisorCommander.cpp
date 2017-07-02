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

#include "hurricane/Hurricane.h"

#include "hurricane/message/SupervisorCommander.h"
#include "hurricane/base/ByteArray.h"
#include "hurricane/base/DataPackage.h"
#include "hurricane/message/Command.h"

using hurricane::base::NetAddress;
using hurricane::base::ByteArray;
using hurricane::base::DataPackage;

const int DATA_BUFFER_SIZE = 65535;

namespace hurricane {
	namespace message {

		void SupervisorCommander::Join() {
			Connect();

			Command command(Command::Type::Join, {
				_supervisorName
			});
			DataPackage messagePackage = command.ToDataPackage();
			ByteArray message = messagePackage.Serialize();

			char resultBuffer[DATA_BUFFER_SIZE];
			int32_t resultSize =
				_connector->SendAndReceive(message.data(), message.size(), resultBuffer, DATA_BUFFER_SIZE);

			ByteArray result(resultBuffer, resultSize);
			DataPackage resultPackage;
			resultPackage.Deserialize(result);
			command = Command(resultPackage);

			std::cout << command.GetType() << std::endl;
			std::cout << command.GetArg(0).GetStringValue() << std::endl;
		}

		void SupervisorCommander::Alive() {
			Connect();

			Command command(Command::Type::Alive, {
				_supervisorName
			});
			DataPackage messagePackage = command.ToDataPackage();
			ByteArray message = messagePackage.Serialize();

			char resultBuffer[DATA_BUFFER_SIZE];
			int32_t resultSize =
				_connector->SendAndReceive(message.data(), message.size(), resultBuffer, DATA_BUFFER_SIZE);

			ByteArray result(resultBuffer, resultSize);
			DataPackage resultPackage;
			resultPackage.Deserialize(result);
			command = Command(resultPackage);
		}

		void SupervisorCommander::SendTuple(int taskIndex,
			const base::Values& values) {
			Connect();

			base::Variants args = { _supervisorName, _taskIndex };
			for ( const base::Value& value : values ) {
				args.push_back(value.ToVariant());
			}

			Command command(Command::Type::Data, args);

			DataPackage messagePackage = command.ToDataPackage();
			ByteArray message = messagePackage.Serialize();

			char resultBuffer[DATA_BUFFER_SIZE];
			int32_t resultSize =
				_connector->SendAndReceive(message.data(), message.size(), resultBuffer, DATA_BUFFER_SIZE);

			ByteArray result(resultBuffer, resultSize);
			DataPackage resultPackage;
			resultPackage.Deserialize(result);
			command = Command(resultPackage);

			std::cout << command.GetType() << std::endl;
			std::cout << command.GetArg(0).GetStringValue() << std::endl;
		}
		void SupervisorCommander::RandomDestination(const std::string srcType, int32_t srcIndex, 
			std::string * host, int * port, int* destIndex)
		{
			Connect();

			Command command(Command::Type::RandomDestination, {
				_supervisorName, srcType, srcIndex
			});
			DataPackage messagePackage = command.ToDataPackage();
			ByteArray message = messagePackage.Serialize();

			char resultBuffer[DATA_BUFFER_SIZE];
			int32_t resultSize =
				_connector->SendAndReceive(message.data(), message.size(), resultBuffer, DATA_BUFFER_SIZE);

			ByteArray result(resultBuffer, resultSize);
			DataPackage resultPackage;
			resultPackage.Deserialize(result);
			command = Command(resultPackage);

			*host = command.GetArg(1).GetStringValue();
			*port = command.GetArg(2).GetIntValue();
			*destIndex = command.GetArg(3).GetIntValue();
		}

		void SupervisorCommander::GroupDestination(const std::string srcType, int srcIndex, 
				std::string * host, int * port, int * destIndex,
				int fieldIndex)
		{
			Connect();

			Command command(Command::Type::RandomDestination, {
				_supervisorName, srcType, srcIndex, fieldIndex
			});
			DataPackage messagePackage = command.ToDataPackage();
			ByteArray message = messagePackage.Serialize();

			char resultBuffer[DATA_BUFFER_SIZE];
			int32_t resultSize =
				_connector->SendAndReceive(message.data(), message.size(), resultBuffer, DATA_BUFFER_SIZE);

			ByteArray result(resultBuffer, resultSize);
			DataPackage resultPackage;
			resultPackage.Deserialize(result);
			command = Command(resultPackage);

			*host = command.GetArg(1).GetStringValue();
			*port = command.GetArg(2).GetIntValue();
			*destIndex = command.GetArg(3).GetIntValue();
		}
	}
}