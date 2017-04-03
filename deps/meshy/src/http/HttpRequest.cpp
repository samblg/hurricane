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

#include "rest/HttpRequest.h"
#include "utils/String.h"

namespace  meshy {
    void HttpRequest::ParseStdString(const std::string &text) {
        StdStringList stringList = SplitString(text, '\n');
        for ( std::string& line: stringList ) {
            line.pop_back();
        }

        ParseStdStringList(stringList);
    }

    void HttpRequest::ParseStdStringList(const StdStringList &stringList) {
        std::string requestLine = stringList.front();
        ParseRequestLine(requestLine);

        StdStringList contextLines = stringList;
        contextLines.erase(contextLines.begin());
        HttpContext::ParseStdStringList(contextLines);
    }

    void HttpRequest::ParseRequestLine(const std::string &requestLine) {
        StdStringList words = SplitString(requestLine, ' ');

        SetMethod(words[0]);
        SetPath(words[1]);
        SetVersion(words[2]);
    }

    HttpRequest HttpRequest::FromStdString(const std::string &text) {
        HttpRequest request;
        request.ParseStdString(text);

        return request;
    }

    HttpRequest HttpRequest::FromStdStringList(const StdStringList &stringList) {
        HttpRequest request;
        request.ParseStdStringList(stringList);

        return request;
    }

}
