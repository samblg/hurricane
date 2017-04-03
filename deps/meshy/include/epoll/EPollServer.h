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

#ifndef NET_FRAMEWORK_EPOLLSERVER_H
#define NET_FRAMEWORK_EPOLLSERVER_H

#include "net.h"
#include "PackageDataSink.h"
#include "epoll/EPollConnection.h"


namespace meshy {

    class EPollServer : public BasicServer<EPollConnectionPtr> {
    public:
        EPollServer() { }
        virtual ~EPollServer() { }

        int32_t Listen(const std::string& host, int32_t port, int32_t backlog = 20) override;

        void OnConnectIndication(ConnectIndicationHandler handler) {
            _connectHandler = handler;
        }
        void OnDisconnectIndication(DisconnectIndicationHandler handler) {
            _disconnectIndication = handler;
        }

        EPollConnectionPtr Accept(int32_t sockfd);

    private:
        int32_t _Bind(const std::string& host, int32_t port);

        DataSink* _dataSink;
        ConnectIndicationHandler _connectHandler;
        DisconnectIndicationHandler _disconnectIndication;
    };

}
#endif //NET_FRAMEWORK_EPOLLSERVER_H
