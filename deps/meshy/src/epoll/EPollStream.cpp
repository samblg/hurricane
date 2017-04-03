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

//
// Created by kinuxroot on 16-3-9.
//

#include "epoll/EPollStream.h"

#include "epoll/EPollLoop.h"
#include "utils/logger.h"
#include <unistd.h>
#include "bytearray.h"

namespace meshy {
    int32_t EPollStream::Receive(char *buffer, int32_t bufferSize, int32_t &readSize) {
        readSize = 0;
        int32_t nread = 0;
        NativeSocketEvent ev;

        while ((nread = read(GetNativeSocket(), buffer + readSize, bufferSize - 1)) > 0) {
            readSize += nread;
        }

        return nread;
    }

    int32_t EPollStream::Send(const meshy::ByteArray& byteArray) {
        TRACE_DEBUG("EPollConnection::Send");

        struct epoll_event ev;
        NativeSocket clientSocket = GetNativeSocket();

        if ( EPollLoop::Get()->ModifyEpollEvents(_events | EPOLLOUT, clientSocket) ) {
            // TODO: MARK ERASE
            TRACE_ERROR("FATAL epoll_ctl: mod failed!");
        }

        const char *buf = byteArray.data();
        int32_t size = byteArray.size();
        int32_t n = size;

        while (n > 0) {
            int32_t nwrite;
            nwrite = write(clientSocket, buf + size - n, n);
            if (nwrite < n) {
                if (nwrite == -1 && errno != EAGAIN) {
                    TRACE_ERROR("FATAL write data to peer failed!");
                }
                break;
            }
            n -= nwrite;
        }

        return 0;
    }
}
