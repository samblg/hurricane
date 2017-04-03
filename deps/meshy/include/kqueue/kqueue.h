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


#ifndef CPPSTORM_KQUEUE_H
#define CPPSTORM_KQUEUE_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "bsd/net_bsd.h"
#include "net.h"

class kqueue {
};

class KQueueConnection : public TcpConnection {
public:
	KQueueConnection(NativeSocket clientSocket) :
		TcpConnection(clientSocket, NativeSocketAddress()) {
	}

	KQueueConnection(const TcpConnection& connection) = delete;

	virtual ByteArray Receive() override;
	virtual void Send(const ByteArray & byteArray) override;
};
typedef std::shared_ptr<KQueueConnection> KQueueConnectionPtr;

#endif //CPPSTORM_KQUEUE_H
