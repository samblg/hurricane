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

#include "iocp/IOCPServer.h"
#include "iocp/IOCPLoop.h"
#include "utils/common_utils.h"
#include "utils/logger.h"
#include <cstdint>
#include <cassert>
#include <memory>
#include <WS2tcpip.h>

#ifndef DISABLE_ASSERT
#ifdef assert
#undef assert
#endif

#define assert(x)
#endif

namespace meshy {
	IOCPServer::IOCPServer() : _completionPort(nullptr), _socket(0), _dataSink(nullptr), _ioOperationDataGroup()
	{
		WindowsSocketInitializer::Initialize();
	}

	IOCPServer::~IOCPServer()
	{}

	int32_t IOCPServer::Bind(const std::string host, int32_t port) {
		_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
		if (nullptr == _completionPort) {
			TRACE_ERROR("CreateIoCompletionPort failed. Error: " + GetLastError());
			assert(false);
			return -1;
		}

		SOCKET listenfd = socket(AF_INET, SOCK_STREAM, 0);
		SetNativeSocket(listenfd);

		int32_t option = 1;
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)option, sizeof(option));

		SOCKADDR_IN srvAddr;
		inet_pton(AF_INET, host.c_str(), &(srvAddr.sin_addr));
		srvAddr.sin_family = AF_INET;
		srvAddr.sin_port = htons(port);
		int32_t errorCode = ::bind(listenfd, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
		if (SOCKET_ERROR == errorCode) {
			TRACE_ERROR("Bind failed. Error: " + GetLastError());
			assert(false);
			return errorCode;
		}

		return 0;
	}

    int32_t IOCPServer::Listen(const std::string& host, int32_t port, int32_t backlog) {
        int32_t errorCode = Bind(host, port);
        if ( errorCode != 0 ) {
            return errorCode;
        }

        return Listen(nullptr, backlog);
    }

	int32_t IOCPServer::Listen(DataSink* dataSink, int32_t backlog) {
		int32_t listenfd = GetNativeSocket();
		int32_t errorCode = listen(listenfd, backlog);
		if (-1 == errorCode) {
			TRACE_ERROR("Listen socket failed!");
			assert(0);
			return errorCode;
		}

		this->SetDataSink(dataSink);
		IOCPLoop::Get()->AddServer(this);

        return 0;
	}

	WSAConnectionPtr IOCPServer::Accept(int32_t listenfd)
	{
		SOCKADDR_IN saRemote;
		int RemoteLen;
		SOCKET acceptSocket;

		RemoteLen = sizeof(saRemote);
		acceptSocket = accept(GetNativeSocket(), (SOCKADDR*)&saRemote, &RemoteLen); // blocked
		if (SOCKET_ERROR == acceptSocket)
		{
			std::cerr << "Accept Socket Error: " << GetLastError() << std::endl;
			throw std::exception("Accept Socket Error: ");
		}
		
		WSAConnectionPtr connection = std::make_shared<WSAConnection>(acceptSocket, saRemote);
        connection->SetDataSink(GetDataSink());

        IOCP::OperationDataPtr perIOData = IOCP::CreateOperationData(connection, this->GetCompletionPort());
		_ioOperationDataGroup.push_back(perIOData);
		connection->SetOperationData(perIOData);

        DWORD flags = 0;
        DWORD RecvBytes = 0;
        WSARecv(connection->GetNativeSocket(),
            &(connection->GetOperationData()->databuff), 1, &RecvBytes, &flags,
            &(connection->GetOperationData()->overlapped), NULL);

		return connection;
	}


	void IOCPServer::SetDataSink(DataSink *dataSink)
	{
		_dataSink = dataSink;
	}

	DataSink* IOCPServer::GetDataSink() const
	{
		return _dataSink;
	}

	void IOCPServer::SetCompletionPort(HANDLE completionPort)
	{
		_completionPort = completionPort;
	}

	HANDLE IOCPServer::GetCompletionPort() const
	{
		return _completionPort;
	}


    void IOCPServer::OnConnectIndication(ConnectIndicationHandler handler) {
        this->_connectHandler = handler;

    }
    void IOCPServer::OnDisconnectIndication(DisconnectIndicationHandler handler) {
        this->_disconnectHandler = handler;
    }
}
