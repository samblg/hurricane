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

#include "logging/Logging.h"

#ifdef WIN32
#include <Ws2tcpip.h>


#define DISPLAY_SOCKET_ERROR(message) \
    LOG(LOG_ERROR) << message << WSAGetLastError()
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <cstdio>
#include <cerrno>

#define SOCKET int32_t
#define closesocket close
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define LPSOCKADDR sockaddr*

#define DISPLAY_SOCKET_ERROR(message) perror(message)
#endif

#include "hurricane/util/SocketError.h"

#include <string>
#include <functional>
#include <exception>
#include <cstdint>

namespace hurricane {
namespace util {

#ifdef WIN32
class WSAInitializer {
public:
    static WSAInitializer& InitInstance();
    ~WSAInitializer();

private:
    WSAInitializer();
    WSADATA _wsaData;
};
#endif

class Socket {
public:
    typedef std::function<void(int32_t sentSize, const SocketError& error)> SendCallback;
    typedef std::function<void(char* buf, int32_t size, const SocketError& error)> ReceiveCallback;

    Socket();
    ~Socket();

    SOCKET GetSocket() const {
        return _socket;
    }

    int32_t Send(const char* buf, size_t size);
    void SendAsync(const char* buf, size_t size, SendCallback callback);
    int32_t Receive(char* buf, size_t size);
    bool ReceiveAsync(char* buf, size_t size);
    void OnData(ReceiveCallback callback);

protected:
    SOCKET _socket;
    ReceiveCallback _receiveCallback;
};

class TcpConnection : public Socket {
public:
    TcpConnection(SOCKET socket, sockaddr_in clientAddress);

private:
    sockaddr_in _clientAddress;
};

class TcpServer : public Socket {
public:
    TcpServer();
    void Listen(const std::string& address, int32_t port);
    TcpConnection* Accept();
};

class TcpClient : public Socket {
public:
    TcpClient();
    void Connect(const std::string& address, int32_t port);
};

}
}
