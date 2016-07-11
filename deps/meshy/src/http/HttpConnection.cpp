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

#include "Meshy.h"
#include "rest/HttpConnection.h"
#include "bytearray.h"
#include <iostream>

namespace meshy {
    HttpConnection::HttpConnection(TcpConnection* connection) :
            _connection(connection) {
        std::cout << _connection << std::endl;
        auto tcpDataHandler = std::bind(&HttpConnection::HandleData, this, std::placeholders::_1, std::placeholders::_2);
        _connection->OnDataIndication(tcpDataHandler);
    }

    void HttpConnection::HandleData(const char *buffer, int64_t size) {
        std::cout << buffer << std::endl;
        std::cout << size << std::endl;
        std::string requestText(buffer, size);

        _request.ParseStdString(requestText);

        if ( _requstHandler ) {
            _requstHandler(_request);
        }

        if ( _dataHandler && _request.GetContent().size() > 0 ) {
            _dataHandler(_request.GetContent());
        }
    }

    void HttpConnection::SendResponse(const HttpResponse &response) {
        _response = response;

        _connection->Send(ByteArray(_response.ToStdString()));
    }
}
