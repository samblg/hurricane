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

#ifndef NET_FRAMEWORK_EPOLLSTREAM_H_H
#define NET_FRAMEWORK_EPOLLSTREAM_H_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "linux/net_linux.h"
#include "net.h"


namespace meshy {
    class EPollLoop;

    class EPollStream : public BasicStream {
    public:
        EPollStream(NativeSocket nativeSocket) :
                BasicStream(nativeSocket) {}

        virtual ~EPollStream() { }

        EPollStream(const EPollStream &stream) = delete;

        virtual int32_t Receive(char *buffer, int32_t bufferSize, int32_t &readSize) override;
        virtual int32_t Send(const ByteArray &byteArray) override;

        uint32_t GetEvents() const {
            return _events;
        }

        void SetEvents(uint32_t events) {
            _events = events;
        }

        void OnDataIndication(DataIndicationHandler handler) override {
            _dataHandler = handler;
        }
        DataIndicationHandler GetDataIndication() override {
            return _dataHandler;
        }

    private:
        uint32_t _events;
        DataIndicationHandler _dataHandler;
    };

    typedef std::shared_ptr <EPollStream> EPollStreamPtr;
}

#endif //NET_FRAMEWORK_EPOLLSTREAM_H_H
