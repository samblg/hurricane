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

#include "hurricane/base/NetAddress.h"
#include "hurricane/base/NetConnector.h"
#include "hurricane/base/Values.h"
#include <string>

namespace hurricane {
	namespace message {
		class SupervisorCommander {
		public:
			SupervisorCommander(const hurricane::base::NetAddress& nimbusAddress,
				const std::string& supervisorName) :
				_nimbusAddress(nimbusAddress), _supervisorName(supervisorName) {
			}

			void Connect() {
				if ( !_connector.get() ) {
					_connector = std::make_shared<NetConnector>(_nimbusAddress);
					_connector->Connect();
				}
			}

			void Join();
			void Alive();
			void SendTuple(int taskIndex, const base::Values& values);
			void RandomDestination(const std::string srcType, int32_t srcIndex,
				std::string * host, int * port, int* destIndex);
			void GroupDestination(const std::string srcType, int srcIndex,
				std::string * host, int * port, int* destIndex,
				int fieldIndex);
			const std::string GetSupervisorName() const {
				return _supervisorName;
			}

		private:
			hurricane::base::NetAddress _nimbusAddress;
			std::string _supervisorName;
			std::shared_ptr<NetConnector> _connector;
            int32_t _taskIndex;
		};
	}
}
