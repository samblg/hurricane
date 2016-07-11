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

#ifndef NET_FRAMEWORK_HTTPREPONSE_H
#define NET_FRAMEWORK_HTTPREPONSE_H

#include "rest/HttpContext.h"

namespace meshy {
    class HttpResponse : public HttpContext {
    public:
        HttpResponse() {}
        virtual std::string ToStdString() const override;

        int GetStatusCode() const {
            return _statusCode;
        }

        void SetStatusCode(int statusCode) {
            _statusCode = statusCode;
        }

        const std::string& GetStatusMessage() const {
            return _statusMessage;
        }

        void SetStatusMessage(const std::string& message) {
            _statusMessage = message;
        }

    private:
        std::string GetResponseLine() const;

        int _statusCode;
        std::string _statusMessage;
    };
}
#endif //NET_FRAMEWORK_HTTPREPONSE_H
