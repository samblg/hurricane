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

#ifndef NET_FRAMEWORK_HTTPREQUEST_H
#define NET_FRAMEWORK_HTTPREQUEST_H

#include "rest/HttpContext.h"

namespace meshy {
    class HttpRequest : public HttpContext {
    public:
        HttpRequest() {}
        void ParseStdString(const std::string& text);
        virtual void ParseStdStringList(const StdStringList& stringList) override;

        static HttpRequest FromStdString(const std::string& text);
        static HttpRequest FromStdStringList(const StdStringList& stringList);

        const std::string& GetMethod() const {
            return _method;
        }

        const std::string& GetPath() const {
            return _path;
        }

        void SetMethod(const std::string& method) {
            _method = method;
        }

        void SetPath(const std::string& path) {
            _path = path;
        }

    private:
        void ParseRequestLine(const std::string& requestLine);

    private:
        std::string _method;
        std::string _path;
    };
}

#endif //NET_FRAMEWORK_HTTPREQUEST_H
