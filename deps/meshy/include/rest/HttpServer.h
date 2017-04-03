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

#ifndef NET_FRAMEWORK_HTTPSERVER_H
#define NET_FRAMEWORK_HTTPSERVER_H

#include "Meshy.h"
#include "rest/HttpConnection.h"
#include <vector>
#include <functional>

namespace  meshy {
    class IoLoop;

    class HttpServer {
    public:
        typedef std::function<void(HttpConnection* connection)> ConnectionHandler;

        HttpServer();
        virtual ~HttpServer();

        void Listen(const std::string& host, int port, int backlog = 20);
        void OnConnection(ConnectionHandler handler) {
            _connectionHandler = handler;
        }

    private:
        TcpServer _server;
        ConnectionHandler _connectionHandler;
    };
}
#endif //NET_FRAMEWORK_HTTPSERVER_H
