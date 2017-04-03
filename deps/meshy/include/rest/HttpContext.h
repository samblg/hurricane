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

#ifndef NET_FRAMEWORK_HTTPCONTEXT_H_H
#define NET_FRAMEWORK_HTTPCONTEXT_H_H

#include "bytearray.h"
#include <map>
#include <string>
#include <vector>

typedef std::vector<std::string> StdStringList;

class HttpContext {
public:
    virtual ~HttpContext() {
    }

    const std::string& GetHeader(const std::string& name) const {
        return _headers.at(name);
    }

    void SetHeader(const std::string& name, const std::string& value) {
        _headers[name] = value;
    }

    bool HasHeader(const std::string& name) const {
        return _headers.find(name) != _headers.end();
    }

    StdStringList GetHeaderNames() const;

    const std::string& GetContent() const {
        return _content;
    }

    void SetContent(const std::string& content);

    const std::string& GetVersion() const {
        return _version;
    }

    void SetVersion(const std::string& version) {
        _version = version;
    }

    virtual void ParseStdStringList(const StdStringList& stringList);
    virtual std::string ToStdString() const;

    static HttpContext FromStdStringList(const StdStringList& stringList);

private:
    std::string _version;
    std::map<std::string, std::string> _headers;
    std::string _content;
};

#endif //NET_FRAMEWORK_HTTPCONTEXT_H_H
