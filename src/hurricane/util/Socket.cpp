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
#include <cstring>

#define PACKAGE_HEADER_SIZE 4

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
        // 接收缓冲区
    int nRecvBuf=32*1024;//设置为32K
    setsockopt(_socket,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
    //发送缓冲区
    int nSendBuf=32*1024;//设置为32K
    setsockopt(_socket,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
}

Socket::~Socket() {
    closesocket(_socket);
}

int32_t Socket::Send(const char* buf, size_t size) {
    struct sockaddr addr;
    char *send_buf = new char[size + PACKAGE_HEADER_SIZE];

    memcpy(send_buf, &size, PACKAGE_HEADER_SIZE);
    memcpy(send_buf + PACKAGE_HEADER_SIZE, buf, size);

    int32_t iSend = static_cast<int32_t>(sendto(_socket, send_buf, static_cast<int32_t>(size + PACKAGE_HEADER_SIZE), 0, &addr, sizeof(addr)));
    delete [] send_buf;
    send_buf = nullptr;
    
    if ( iSend == SOCKET_ERROR )
    {
        throw SocketError(SocketError::Type::SendError, "send() Failed");
    }
    if ( iSend != size + PACKAGE_HEADER_SIZE )
    {
        throw SocketError(SocketError::Type::SendError, "send() data size not fit!");
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
    int32_t iLen = 0;
    char len_buf[PACKAGE_HEADER_SIZE] = {0};
    do {
        iLen += static_cast<int32_t>(recv(_socket, len_buf, static_cast<int32_t>(PACKAGE_HEADER_SIZE - iLen), 0));
    } while (iLen < PACKAGE_HEADER_SIZE);
    int32_t buf_size = 0;
    memcpy(&buf_size, len_buf, PACKAGE_HEADER_SIZE);

    iLen = 0;
    do {
        iLen += static_cast<int32_t>(recv(_socket, buf+iLen, static_cast<int32_t>(buf_size - iLen), 0));
    } while (iLen < buf_size);

    if ( iLen == SOCKET_ERROR )
    {
        throw SocketError(SocketError::Type::RecvError, "recv() Failed");
    }
    if (iLen != buf_size) {
        throw SocketError(SocketError::Type::RecvError, "recv() Data not fit!");
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
