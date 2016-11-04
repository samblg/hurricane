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

#ifdef WIN32
#include <Windows.h>


#define DISPLAY_SOCKET_ERROR(message) \
    printf("%s: %d\n", message, WSAGetLastError());
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <cstdio>
#include <cerrno>

#define SOCKET int
#define closesocket close
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define LPSOCKADDR sockaddr*

#define DISPLAY_SOCKET_ERROR(message) perror(message)
#endif

#include <string>
#include <functional>
#include <exception>

namespace hurricane {
	namespace util {
        class SocketException : public std::exception {
        public:
            SocketException(const std::string& message) : _message(message) {
            }

        // exception interface
        public:
            const char* what() const noexcept {
                return _message.c_str();
            }

        private:
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
			typedef std::function<void(int sentSize)> SendCallback;
			typedef std::function<void(char* buf, int size)> ReceiveCallback;

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

			int Send(const char* buf, size_t size) {
				int iSend = send(_socket, buf, size, 0);
				if ( iSend == SOCKET_ERROR )
                {
                    throw SocketException("send() Failed");
				}

				return iSend;
			}

			void SendAsync(const char* buf, size_t size, SendCallback callback) {
				int sentSize = Send(buf, size);
				callback(sentSize);
			}

			int Receive(char* buf, size_t size) {
				int iLen = recv(_socket, buf, size, 0);

				if ( iLen == SOCKET_ERROR )
				{
                    throw SocketException("recv() Failed");
				}

				return iLen;
			}

			bool ReceiveAsync(char* buf, size_t size) {
				int receivedSize = Receive(buf, size);

				if ( !receivedSize ) {
					return false;
				}

				_receiveCallback(buf, receivedSize);
				return true;
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
					exit(-1);
				}
			}

			void Listen(const std::string& address, int port) {
				sockaddr_in serverAddress;

                serverAddress.sin_family = AF_INET;
                serverAddress.sin_port = htons(port);
                serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

                if ( bind(_socket, (const LPSOCKADDR)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR )
                {
                    DISPLAY_SOCKET_ERROR("bind() Failed");
                    exit(-1);
				}

				if ( listen(_socket, 5) == SOCKET_ERROR )
                {
                    DISPLAY_SOCKET_ERROR("lisiten() Failed");
					exit(-1);
				}
			}

			TcpConnection* Accept() {
				sockaddr_in clientAddress;
                socklen_t length = sizeof(clientAddress);
				SOCKET clientSocket = accept(_socket, (struct sockaddr *)&clientAddress, &length);

				if ( clientSocket == INVALID_SOCKET )
				{
                    DISPLAY_SOCKET_ERROR("accept() Failed");
					exit(-1);
				}

				printf("Accepted client IP:[%s],port:[%d]\n",
					inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

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
					exit(-1);
				}
			}

			void Connect(const std::string& address, int port) {
				sockaddr_in serverAddress;

				serverAddress.sin_family = AF_INET;
				serverAddress.sin_port = htons(port);
				serverAddress.sin_addr.s_addr = inet_addr(address.c_str());
				int ret = connect(_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

				if ( ret == INVALID_SOCKET )
				{
                    DISPLAY_SOCKET_ERROR("connect() Failed");
                    throw SocketException("connect() Failed");
				}
			}
		};

	}
}
