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

#include "hurricane/util/Socket.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")

#include "hurricane/util/Socket.h"

#include <Winsock2.h>  
#include <cstdio>

namespace hurricane {
    namespace util {
        WSAInitializer::WSAInitializer() {
            if ( WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0 )
            {
                LOG(LOG_FATAL) << "Failed to load Winsock.";
                exit(-1);
            }
        }

        WSAInitializer& WSAInitializer::InitInstance()
        {
            static WSAInitializer instance;

            return instance;
        }

        WSAInitializer::~WSAInitializer()
        {
            WSACleanup();
        }

    }
}
#endif

namespace hurricane {
namespace util {

Socket::Socket() {
#ifdef WIN32
    WSAInitializer::InitInstance();
#endif
}

Socket::~Socket() {
    closesocket(_socket);
}

int32_t Socket::Send(const char* buf, size_t size) {
    struct sockaddr addr;
    int32_t iSend = static_cast<int32_t>(sendto(_socket, buf, static_cast<int32_t>(size), 0, &addr, sizeof(addr)));
    if ( iSend == SOCKET_ERROR )
    {
        throw SocketError(SocketError::Type::SendError, "send() Failed");
    }

    return iSend;
}

void Socket::SendAsync(const char* buf, size_t size, SendCallback callback) {
    try {
        int32_t sentSize = Send(buf, size);
        callback(sentSize, SocketError());
    }
    catch ( const SocketError& e ) {
        LOG(LOG_ERROR) << e.what();
        callback(0, e);
    }
}

int32_t Socket::Receive(char* buf, size_t size) {
    int32_t iLen = static_cast<int32_t>(recv(_socket, buf, static_cast<int32_t>(size), 0));

    if ( iLen == SOCKET_ERROR )
    {
        throw SocketError(SocketError::Type::RecvError, "recv() Failed");
    }

    return iLen;
}

bool Socket::ReceiveAsync(char* buf, size_t size) {
    try {
        int32_t receivedSize = Receive(buf, size);

        if ( !receivedSize ) {
            return false;
        }

        _receiveCallback(buf, receivedSize, SocketError());
        return true;
    }
    catch ( const SocketError& e ) {
        _receiveCallback(nullptr, 0, e);
    }

    return false;
}

void Socket::OnData(ReceiveCallback callback) {
    _receiveCallback = callback;
}

TcpConnection::TcpConnection(SOCKET socket, sockaddr_in clientAddress) :
    _clientAddress(clientAddress) {
    _socket = socket;
}

TcpServer::TcpServer() {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( _socket == INVALID_SOCKET )
    {
        DISPLAY_SOCKET_ERROR("socket() Failed.");
    }
}

void TcpServer::Listen(const std::string& address, int32_t port) {
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( bind(_socket, (const LPSOCKADDR)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR )
    {
        DISPLAY_SOCKET_ERROR("bind() Failed");
    }

    if ( listen(_socket, 5) == SOCKET_ERROR )
    {
        DISPLAY_SOCKET_ERROR("lisiten() Failed");
    }
}

TcpConnection* TcpServer::Accept() {
    sockaddr_in clientAddress;
#ifdef WIN32
    int32_t length = sizeof(clientAddress);
#else
    socklen_t length = sizeof(clientAddress);
#endif
    SOCKET clientSocket = accept(_socket, (struct sockaddr*)&clientAddress, &length);

    if ( clientSocket == INVALID_SOCKET )
    {
        DISPLAY_SOCKET_ERROR("accept() Failed");
    }

#ifdef WIN32
    char ipDotSec[20];
    inet_ntop(AF_INET, &(clientAddress.sin_addr), ipDotSec, 16);
    LOG(LOG_DEBUG) << "Accepted client IP: " << ipDotSec << ",port:[" << ntohs(clientAddress.sin_port) << "]";
#else
    LOG(LOG_DEBUG) << "Accepted client IP: " << inet_ntoa(clientAddress.sin_addr) << ",port:[" << ntohs(clientAddress.sin_port) << "]";
#endif

    return new TcpConnection(clientSocket, clientAddress);
}

TcpClient::TcpClient() {
    _socket = socket(AF_INET, SOCK_STREAM, 0);

    if ( _socket == INVALID_SOCKET )
    {
        DISPLAY_SOCKET_ERROR("socket() Failed");
    }
}

void TcpClient::Connect(const std::string& address, int32_t port) {
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

#ifdef WIN32
    inet_pton(AF_INET, address.c_str(), (void *)&serverAddress.sin_addr.s_addr);
#else
    serverAddress.sin_addr.s_addr = inet_addr(address.c_str());
#endif

    int32_t ret = connect(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    if ( ret == INVALID_SOCKET )
    {
        DISPLAY_SOCKET_ERROR("connect() Failed");
        throw SocketError(SocketError::Type::ConnectError, "connect() Failed");
    }
}

}
}
