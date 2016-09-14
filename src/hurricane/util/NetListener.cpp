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

#include "hurricane/util/NetListener.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace hurricane {
	namespace util {
		const int DATA_BUFFER_SIZE = 65535;

		void NetListener::StartListen()
		{
			_server = std::make_shared<TcpServer>();

			_server->Listen(_host.GetHost(), _host.GetPort());
			std::cout << "Listen on " << _host.GetHost() << ":" << _host.GetPort() << std::endl;

			while ( 1 )
			{
				std::shared_ptr<TcpConnection> connection = std::shared_ptr<TcpConnection>(_server->Accept());

				std::cerr << "A client is connected" << std::endl;

				std::thread dataThread(std::bind(&NetListener::DataThreadMain, this, std::placeholders::_1),
					connection);
				dataThread.detach();
			}
		}

		void NetListener::DataThreadMain(std::shared_ptr<TcpConnection> connection)
		{
			int32_t _lostTime = 0;

			_connectionCallback(connection);

			try {
				char buffer[DATA_BUFFER_SIZE];
				while ( 1 ) {
					bool successful = connection->ReceiveAsync(buffer, DATA_BUFFER_SIZE);

					if ( !successful ) {
						break;
					}
					//if ( !length ) {
					//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					//	_lostTime ++;

					//	if ( _lostTime < 10 ) {
					//		continue;
					//	}
					//	else {
					//		break;
					//	}
					//}
					//else {
					//	_lostTime = 0;
					//}

					//_receiver(connection, buffer, length);
				}
			}
			catch ( const std::exception& e ) {
				std::cerr << e.what() << std::endl;
			}
		}
	}
}
