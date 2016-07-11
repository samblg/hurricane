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

#ifndef NET_FRAME_EPOLLLOOP_H
#define NET_FRAME_EPOLLLOOP_H

#include "loop.h"
#include "epoll/EPollConnection.h"
#include "epoll/EPollStream.h"
#include "epoll/EPollServer.h"
#include "epoll/EPollClient.h"
#include "net.h"
#include "DataSink.h"
#include <map>
#include <memory>
#include <thread>
#include <string>

#include "linux/net_linux.h"
#include "linux/common.h"
#include <sys/epoll.h>


namespace meshy {

    class EventQueue;

    class EPollServer;

    class EPollLoop : public Loop {
    public:
        static EPollLoop* Get();

        virtual ~EPollLoop() override;

        void AddServer(NativeSocket socket, EPollServer* server);
        void AddStream(EPollStreamPtr stream);

        int32_t AddEpollEvents(int32_t events, int32_t fd);
        int32_t ModifyEpollEvents(int32_t events, int32_t fd);

    protected:
        EPollLoop();

        virtual void _Run() override;

    private:
        void _Initialize();

        void _EPollThread();

        void _HandleEvent(int32_t eventfd, struct epoll_event* events, int32_t nfds);

        int32_t _Accept(int32_t eventfd, int32_t listenfd);

        void _Read(int32_t eventfd, int32_t fd, uint32_t events);

        void _Enqueue(EPollStreamPtr connection, const char* buf, int64_t nread);

    private:
        int32_t _eventfd;
        bool _shutdown;

        std::map<NativeSocket, EPollServer*> _servers;
        std::map <NativeSocket, EPollStreamPtr> _streams;
    };
}

#endif //HURRICANE_EPOLLLOOP_H
