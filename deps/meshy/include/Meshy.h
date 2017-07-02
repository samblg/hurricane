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

#include "IoLoop.h"
#include "net.h"

#ifdef OS_LINUX
#include "epoll/EPollClient.h"
#include "epoll/EPollConnection.h"
#include "epoll/EPollServer.h"
#include "epoll/EPollStream.h"
#elif defined(OS_WIN32)
#include "iocp/IOCPClient.h"
#include "iocp/IOCPConnection.h"
#include "iocp/IOCPServer.h"
#include "iocp/IOCPStream.h"
#endif

namespace meshy {
#ifdef OS_LINUX
    typedef EPollServer TcpServer;
    typedef EPollConnection TcpConnection;
    typedef EPollClient TcpClient;
    typedef EPollStream TcpStream;
#elif defined(OS_WIN32)
    typedef IOCPServer TcpServer;
    //typedef WSAConnection TcpConnection;
#define TcpConnection WSAConnection
    typedef IOCPClient TcpClient;
    typedef IOCPStream TcpStream;
#endif

}