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

#include "epoll/EPollServer.h"
#include "epoll/EPollLoop.h"
#include "utils/common_utils.h"
#include "utils/logger.h"
#include <cstdint>
#include <cassert>
#include <epoll/EPollLoop.h>

#ifndef DISABLE_ASSERT
#ifdef assert
#undef assert
#endif

#define assert(x)
#endif

namespace meshy {
    int32_t EPollServer::_Bind(const std::string& host, int32_t port) {
        int32_t listenfd;
        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            TRACE_ERROR("Create socket failed!");
            exit(1);
        }

        SetNativeSocket(listenfd);
        int32_t option = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

        // make socket non-blocking
        meshy::SetNonBlocking(listenfd);

        NativeSocketAddress addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(host.c_str());

        int32_t errorCode = bind(listenfd, (struct sockaddr *) &addr, sizeof(addr));
        if (errorCode < 0) {
            TRACE_ERROR("Bind socket failed!");
            assert(0);
            return errorCode;
        }
		
		return 0;
    }

    int32_t EPollServer::Listen(const std::string& host, int32_t port, int32_t backlog) {
        _Bind(host, port);

        int32_t listenfd = GetNativeSocket();

        int32_t errorCode = listen(listenfd, backlog);
        if (-1 == errorCode) {
            TRACE_ERROR("Listen socket failed!");
            assert(0);
            return errorCode;
        }

        errorCode = EPollLoop::Get()->AddEpollEvents(EPOLLIN, listenfd);

        if (errorCode == -1) {
            TRACE_ERROR("FATAL epoll_ctl: listen_sock!");
            assert(0);
            return errorCode;
        }

        EPollLoop::Get()->AddServer(listenfd, this);
    }

    EPollConnectionPtr EPollServer::Accept(int32_t sockfd) {
        int32_t conn_sock = 0;
        int32_t addrlen = 0;
        int32_t remote = 0;

        int32_t listenfd = GetNativeSocket();
        while ((conn_sock = accept(listenfd, (struct sockaddr *) &remote,
                                   (socklen_t * ) & addrlen)) > 0) {
            meshy::SetNonBlocking(conn_sock);

            NativeSocketEvent ev;
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = conn_sock;

            if (epoll_ctl(sockfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                perror("epoll_ctl: add");
                exit(EXIT_FAILURE);
            }

            EPollConnectionPtr connection = std::make_shared<EPollConnection>(conn_sock);
            if ( _connectHandler ) {
                _connectHandler(connection.get());
            }

            return connection;
        } // while

        if (conn_sock == -1) {
            if (errno != EAGAIN && errno != ECONNABORTED
                && errno != EPROTO && errno != EINTR)
                perror("accept");
        }

        return EPollConnectionPtr(nullptr);
    }
}
