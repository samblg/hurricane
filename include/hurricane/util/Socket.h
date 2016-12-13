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

#include <string>
#include <functional>
#include <exception>
#include <cstdint>

namespace hurricane {
    namespace util {
        class SocketError {
        public:
            enum class Type {
                NoError,
                ConnectError,
                SendError,
                RecvError
            };

            SocketError() : _type(Type::NoError) {}

            SocketError(Type type, const std::string& message) :
                _type(type), _message(message) {
            }

            Type GetType() const {
                return _type;
            }

        // exception interface
        public:
            const char* what() const {
                return _message.c_str();
            }

            const std::string GetMessage() const {
                return _message;
            }

        private:
            Type _type;
            std::string _message;
        };

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

            Socket() {
#ifdef WIN32
                WSAInitializer::InitInstance();
#endif
            }

            SOCKET GetSocket() const {
                return _socket;
            }

            ~Socket() {
                closesocket(_socket);
            }

            int32_t Send(const char* buf, size_t size) {
                int32_t iSend = static_cast<int32_t>(send(_socket, buf, static_cast<int32_t>(size), 0));
                if ( iSend == SOCKET_ERROR )
                {
                    throw SocketError(SocketError::Type::SendError, "send() Failed");
                }

                return iSend;
            }

            void SendAsync(const char* buf, size_t size, SendCallback callback) {
                try {
                    int32_t sentSize = Send(buf, size);
                    callback(sentSize, SocketError());
                }
                catch ( const SocketError& e ) {
                    callback(0, e);
                }
            }

            int32_t Receive(char* buf, size_t size) {
                int32_t iLen = static_cast<int32_t>(recv(_socket, buf, static_cast<int32_t>(size), 0));

                if ( iLen == SOCKET_ERROR )
                {
                    throw SocketError(SocketError::Type::RecvError, "recv() Failed");
                }

                return iLen;
            }

            bool ReceiveAsync(char* buf, size_t size) {
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

            void OnData(ReceiveCallback callback) {
                _receiveCallback = callback;
            }

        protected:
            SOCKET _socket;
            ReceiveCallback _receiveCallback;
        };

        class TcpConnection : public Socket {
        public:
            TcpConnection(SOCKET socket, sockaddr_in clientAddress) :
                _clientAddress(clientAddress) {
                _socket = socket;
            }

        private:
            sockaddr_in _clientAddress;
        };

        class TcpServer : public Socket {
        public:
            TcpServer() {
                _socket = socket(AF_INET, SOCK_STREAM, 0);
                if ( _socket == INVALID_SOCKET )
                {
                    DISPLAY_SOCKET_ERROR("socket() Failed.");
                }
            }

            void Listen(const std::string& address, int32_t port) {
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

            TcpConnection* Accept() {
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
        };

        class TcpClient : public Socket {
        public:
            TcpClient() {
                _socket = socket(AF_INET, SOCK_STREAM, 0);

                if ( _socket == INVALID_SOCKET )
                {
                    DISPLAY_SOCKET_ERROR("socket() Failed");
                }
            }

            void Connect(const std::string& address, int32_t port) {
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
        };

    }
}
