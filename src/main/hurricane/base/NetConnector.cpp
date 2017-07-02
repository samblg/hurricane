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
#include "hurricane/base/NetConnector.h"
#include "Meshy.h"

#include <thread>
#include <chrono>

void NetConnector::Connect()
{
    _client = meshy::TcpClient::Connect(_host.GetHost(), _host.GetPort(), nullptr);
}

int32_t NetConnector::SendAndReceive(const char * buffer, int32_t size, char* resultBuffer, int32_t resultSize)
{
    _client->Send(meshy::ByteArray(buffer, size));

    bool receivedData = false;
    _client->OnDataIndication([&receivedData, &resultSize, &resultBuffer](const char* buf, int64_t size) {
        if ( resultSize > size) {
            resultSize = size;
        }

        memcpy(resultBuffer, buf, resultSize);
        receivedData = true;
    });

    while ( !receivedData ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return resultSize;
}