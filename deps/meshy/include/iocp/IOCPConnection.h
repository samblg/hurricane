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

#include "net.h"
#include "IOCPStream.h"
#include <Windows.h>
#include <thread>

namespace meshy {

	class WSAConnection : public IOCPStream {
	public:
		WSAConnection(NativeSocket clientSocket, NativeSocketAddress clientAddress) :
            IOCPStream(clientSocket, clientAddress) {
		}

		WSAConnection(const IOCPStream& stream) = delete;

        virtual void OnDataIndication(DataIndicationHandler handler) {
            _dataHandler = handler;
        }

        virtual DataIndicationHandler GetDataIndication() {
            return _dataHandler;
        }

    private:
        DataIndicationHandler _dataHandler;
	};

	typedef std::shared_ptr<WSAConnection> WSAConnectionPtr;

}
