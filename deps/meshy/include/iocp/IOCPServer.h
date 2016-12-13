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

#ifndef NET_FRAMEWORK_IOCPSERVER_H
#define NET_FRAMEWORK_IOCPSERVER_H

#include "Net.h"
#include "PackageDataSink.h"
#include "iocp/IOCPConnection.h"
#include "iocp/IOCPStream.h"
#include <vector>

namespace meshy {

    class IOCPServer : public BasicServer<WSAConnectionPtr> {
    public:
        IOCPServer();
        virtual ~IOCPServer();

        int32_t Bind(const std::string host, int32_t port);
        int32_t Listen(const std::string& host, int32_t port, int32_t backlog);

        WSAConnectionPtr Accept(int32_t listenfd) override;

        void SetDataSink(DataSink* dataSink);

        DataSink* GetDataSink() const;

        void SetCompletionPort(HANDLE completionPort);

        HANDLE GetCompletionPort() const;

        virtual void OnConnect(ConnectHandler handler) {
            _connectHandler = handler;
        }

        virtual void OnDisconnec(DisconnectHandler handler) {
            _disconnectHandler = handler;
        }

    private:
        HANDLE _completionPort;
        NativeSocket _socket;
        DataSink* _dataSink;
        std::vector<IOCP::OperationDataPtr> _ioOperationDataGroup;
        ConnectHandler _connectHandler;
        DisconnectHandler _disconnectHandler;
    };
}
#endif //NET_FRAMEWORK_IOCPSERVER_H
