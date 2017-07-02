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

#include "hurricane/Hurricane.h"

#include "hurricane/base/NetListener.h"
#include "eventqueue.h"
#include "eventqueueloop.h"
#include "IoLoop.h"
#include "utils/logger.h"

#include <iostream>
#include <thread>
#include <chrono>

const int DATA_BUFFER_SIZE = 65535;

void NetListener::StartListen()
{
    meshy::IoLoop::Get()->Start();

    _server.Listen(_host.GetHost(), _host.GetPort());
    _server.OnConnectIndication([this](meshy::IStream* stream) {
        stream->OnDataIndication([stream, this](const char* buf, int64_t size) mutable {
            this->_receiver(dynamic_cast<meshy::TcpStream*>(stream), buf, size);
        });
    });
}