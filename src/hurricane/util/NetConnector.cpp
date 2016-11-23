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

#include "hurricane/util/NetConnector.h"
#include <iostream>
#ifdef USE_MESHY
#include "Meshy.h"
#endif

namespace hurricane {
	namespace util {
		const int RECEIVE_BUFFER_SIZE = 65535;

		void NetConnector::Connect()
		{
            if ( !_client.get() ) {
                _client = std::make_shared<TcpClient>();

                try {
                    _client->Connect(_host.GetHost(), _host.GetPort());
                }
                catch ( const std::exception& e ) {
                    std::cout << "Release client" << std::endl;
                    _client.reset();
                    throw e;
                }
            }
		}

		void NetConnector::Connect(ConnectCallback callback) {
			Connect();
			callback();
		}

		int32_t NetConnector::SendAndReceive(const char * buffer, int32_t size, char* resultBuffer, int32_t resultSize)
		{
			_client->Send(buffer, size);
			return _client->Receive(resultBuffer, resultSize);
		}

		void NetConnector::SendAndReceive(const char* buffer, int32_t size, DataReceiver receiver)
        {
            _client->Send(buffer, size);
			char resultBuffer[RECEIVE_BUFFER_SIZE];
			int readSize = _client->Receive(resultBuffer, RECEIVE_BUFFER_SIZE);

			receiver(resultBuffer, readSize);
		}
	}
}
