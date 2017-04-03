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

#ifndef CPPSTORM_EPOLL_H
#define CPPSTORM_EPOLL_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "linux/net_linux.h"
#include "net.h"

#include "epoll/EPollStream.h"


namespace meshy {
    class EPollLoop;

    class EPollConnection : public EPollStream {
    public:
        EPollConnection(NativeSocket nativeSocket) :
                EPollStream(nativeSocket) { }
        virtual ~EPollConnection() { }

        EPollConnection(const EPollConnection& connection) = delete;
    };

    typedef std::shared_ptr <EPollConnection> EPollConnectionPtr;

}

#endif //CPPSTORM_EPOLL_H
