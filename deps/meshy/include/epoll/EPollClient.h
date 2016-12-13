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

#ifndef NET_FRAMEWORK_EPOLLCLIENT_H
#define NET_FRAMEWORK_EPOLLCLIENT_H

#include "epoll/EPollStream.h"
#include "Net.h"
#include "DataSink.h"

#include <memory>

namespace meshy {

    class EPollClient;

    typedef std::shared_ptr<EPollClient> EPollClientPtr;

class EPollClient : public EPollStream, public IConnectable {
public:
    EPollClient(const EPollClient& client) = delete;
    virtual ~EPollClient() { }

    virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) override;
    virtual int32_t Send(const ByteArray& byteArray) override;

    uint32_t GetEvents() const {
        return _events;
    }

    void SetEvents(uint32_t events) {
        _events = events;
    }

    void Connect(const std::string& host, int32_t port) override;
    static EPollClientPtr Connect(const std::string& ip, int32_t port, DataSink* dataSink);

private:
    EPollClient(NativeSocket clientSocket) :
            EPollStream(clientSocket){
        this->SetNativeSocket(clientSocket);
    }

private:
    uint32_t _events;
};


}

#endif //NET_FRAMEWORK_EPOLLCLIENT_H
