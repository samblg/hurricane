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

namespace meshy {

    class IOCPClient;

    typedef std::shared_ptr<IOCPClient> IOCPClientPtr;

    class IOCPClient : public IOCPStream, public IConnectable {
    public:
        IOCPClient(const IOCPClient& client) = delete;
        virtual ~IOCPClient() { }

        virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) override;
        virtual int32_t Send(const ByteArray& byteArray) override;

        uint32_t GetEvents() const {
            return _events;
        }

        void SetEvents(uint32_t events) {
            _events = events;
        }

        void Connect(const std::string& host, int32_t port) override {}
        static IOCPClientPtr Connect(const std::string& ip, int32_t port, DataSink* dataSink) {
            return IOCPClientPtr();
        }

    private:
        IOCPClient(NativeSocket clientSocket, NativeSocketAddress clientAddress) :
            IOCPStream(clientSocket, clientAddress) {
            this->SetNativeSocket(clientSocket);
        }

    private:
        uint32_t _events;
    };


}