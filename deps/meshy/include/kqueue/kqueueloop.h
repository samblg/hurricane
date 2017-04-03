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


#ifndef CPPSTORM_KQUEUELOOP_H
#define CPPSTORM_KQUEUELOOP_H

#include "loop.h"
#include "kqueue/kqueue.h"
#include "net.h"
#include "DataSink.h"
#include <map>
#include <memory>
#include <thread>
#include <string>

class EventQueue;

class KQueueLoop : public Loop {
public:
    KQueueLoop(const std::string& host, int port);
    void SetDataSink(DataSink* dataSink);

protected:
    virtual void Run() override;

private:
    void ServerListenThread();
    int32_t _RegisterKQueue(int32_t kq, int32_t fd);
    void _HandleEvent(int32_t kq, int32_t fd, struct kevent* events, int32_t nevents);
    void _Accept(int32_t kq, int32_t fd, int32_t connSize);
    void _Read(int32_t sock, uint32_t availBytes);
    void _Enqueue(KQueueConnectionPtr connection, const char* buf, int64_t bytes);

private:
    std::string _host;
    int _port;
    DataSink* _dataSink;
	std::map<NativeSocket, KQueueConnectionPtr> _connections;
};

#endif //CPPSTORM_KQUEUELOOP_H
