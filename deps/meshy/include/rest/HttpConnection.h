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

#ifndef NET_FRAMEWORK_HTTPCONNECTION_H
#define NET_FRAMEWORK_HTTPCONNECTION_H

#include "Meshy.h"
#include "rest/HttpRequest.h"
#include "rest/HttpResponse.h"
#include <functional>

namespace meshy {
    class HttpConnection {
    public:
        typedef std::function<void(const HttpRequest& request)> RequestHandler;
        typedef std::function<void(const std::string& data)> DataHandler;

        HttpConnection(TcpConnection* connection);

        void HandleData(const char* buffer, int64_t size);

        void OnData(DataHandler dataHandler) {
            _dataHandler = dataHandler;
        }

        void OnRequest(RequestHandler requestHandler) {
            _requstHandler = requestHandler;
        }

        void SendResponse(const HttpResponse& response);

    private:
        TcpConnection* _connection;
        HttpRequest _request;
        HttpResponse _response;
        DataHandler _dataHandler;
        RequestHandler _requstHandler;
    };
}
#endif //NET_FRAMEWORK_HTTPCONNECTION_H
