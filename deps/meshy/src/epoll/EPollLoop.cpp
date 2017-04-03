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

#include "epoll/EPollLoop.h"
#include "utils/logger.h"
#include <signal.h>
#include <cassert>

namespace meshy {
    using namespace std::placeholders;

    EPollLoop* EPollLoop::Get()
	{
        static EPollLoop epollLoop;
        return &epollLoop;
    }

    EPollLoop::EPollLoop()
	{
        TRACE_DEBUG("EPollLoop::EPollLoop");

        // TODO: temproray approach to avoid crash
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGPIPE);
        sigprocmask(SIG_BLOCK, &set, NULL);

        _Initialize();
    }

    EPollLoop::~EPollLoop()
	{
        _shutdown = true;
    }
	
	void EPollLoop::AddServer(NativeSocket socket, EPollServer* server)
	{
        _servers.insert({socket, server});
    }

    void EPollLoop::AddStream(EPollStreamPtr stream)
    {
        _streams[stream->GetNativeSocket()] = stream;
    }

    int32_t EPollLoop::AddEpollEvents(int32_t events, int32_t fd)
	{
        NativeSocketEvent ev;
        ev.events = events;
        ev.data.fd = fd;

        return epoll_ctl(_eventfd, EPOLL_CTL_ADD, fd, &ev);
    }

    int32_t EPollLoop::ModifyEpollEvents(int32_t events, int32_t fd)
    {
        NativeSocketEvent ev;
        ev.events = events;
        ev.data.fd = fd;

        return epoll_ctl(_eventfd, EPOLL_CTL_MOD, fd, &ev);
    }

    void EPollLoop::_Initialize()
	{
        _eventfd = epoll_create(MAX_EVENT_COUNT);
        if (_eventfd == -1) {
            TRACE_ERROR("FATAL epoll_create failed!");
            assert(0);
        }
    }

    void EPollLoop::_Run()
	{
        auto func = std::bind(&EPollLoop::_EPollThread, this);
        std::thread listenThread(func);
        listenThread.detach();
    }

    void EPollLoop::_EPollThread()
	{
        TRACE_DEBUG("_EPollThread");
        NativeSocketEvent events[MAX_EVENT_COUNT];

        while (!_shutdown) {
            int32_t nfds;
            nfds = epoll_wait(_eventfd, events, MAX_EVENT_COUNT, -1);
            if (-1 == nfds) {
                TRACE_ERROR("FATAL epoll_wait failed!");
                exit(EXIT_FAILURE);
            }

            _HandleEvent(_eventfd, events, nfds);
        }
    }

    void EPollLoop::_HandleEvent(int32_t eventfd, NativeSocketEvent* events, int32_t nfds)
	{
        for (int i = 0; i < nfds; ++i) {
            int32_t fd;
            fd = events[i].data.fd;

            if (_servers.find(fd) != _servers.end()) {
                _Accept(eventfd, fd);
                continue;
            }

            int32_t n = 0;
            if (events[i].events & EPOLLIN) {
                _Read(eventfd, fd, events[i].events);
            }

            if (events[i].events & EPOLLOUT) {
            }
        }
    }

    int32_t EPollLoop::_Accept(int32_t eventfd, int32_t listenfd)
	{
        TRACE_DEBUG("_Accept");
        EPollServer* server = _servers.find(listenfd)->second;
        EPollConnectionPtr connection = server->Accept(eventfd);

        if (connection != nullptr) {
            _streams[connection->GetNativeSocket()] = connection;
        }
    }

    void EPollLoop::_Read(int32_t eventfd, int32_t fd, uint32_t events)
	{
        TRACE_DEBUG("_Read");

        EPollStreamPtr stream = _streams[fd];

        char buffer[BUFSIZ];
        int32_t readSize;
        int32_t nread = stream->Receive(buffer, BUFSIZ, readSize);

        stream->SetEvents(events);

        if ((nread == -1 && errno != EAGAIN) || readSize == 0) {
            _streams.erase(fd);

            // Print error message
            char message[50];
            sprintf(message, "errno: %d: %s, nread: %d, n: %d", errno, strerror(errno), nread, readSize);
            TRACE_WARNING(message);
            return;
        }

        // Write buf to the receive queue.
        _Enqueue(stream, buffer, readSize);
    }

    void EPollLoop::_Enqueue(EPollStreamPtr stream, const char* buf, int64_t nread)
	{
        TRACE_DEBUG("_Enqueue");

        if ( stream->GetDataIndication() ) {
            stream->GetDataIndication()(buf, nread);
        }
    }

}
