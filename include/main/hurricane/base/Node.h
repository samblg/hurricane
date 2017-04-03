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

#include <string>
#include <ctime>
#include "hurricane/base/NetAddress.h"

namespace hurricane {
	namespace base {
		class Node {
		public:
			enum class Status {
				Dead,
				Alived
			};

			Node() : _address("", 0), _status(Status::Dead) {
			};

			Node(std::string name, const NetAddress& address) :
				_name(name), _address(address), _status(Status::Dead) {
			}

			const std::string& GetName() const {
				return _name;
			}
			
			void SetName(const std::string& name) {
				_name = name;
			}

			const NetAddress& GetAddress() const {
				return _address;
			}

			void SetAddress(const NetAddress& address) {
				_address = address;
			}

			Status GetStatus() const {
				return _status;
			}

			void SetStatus(Status status) {
				_status = status;
			}

            void Alive() {
				_lastLiveTime = time_t(0);
			}

            Status UpdateStatus() {
				time_t currentTime = 0;
				if ( _lastLiveTime - currentTime > 9 ) {
					_status = Status::Dead;
				}

				return _status;
			}

		private:
			std::string _name;
			NetAddress _address;
			Status _status;
			time_t _lastLiveTime;
		};
	}
}
