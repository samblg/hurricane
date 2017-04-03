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

#include "rest/HttpContext.h"
#include "utils/String.h"
#include <iostream>

StdStringList HttpContext::GetHeaderNames() const {
    StdStringList headerNames;

    for ( const auto& pair : _headers ) {
        headerNames.push_back(pair.first);
    }

    return headerNames;
}

HttpContext HttpContext::FromStdStringList(const StdStringList& stringList) {
    HttpContext context;
    context.ParseStdStringList(stringList);

    return context;
}

void HttpContext::ParseStdStringList(const StdStringList &stringList) {
    for ( const std::string& line : stringList ) {
        StdStringList words = SplitString(line, ':');

        if ( !words.size() ) {
            return;
        }

        std::string headerName = words[0];
        std::string headerValue = words[1];
        if ( words.size() > 2 ) {
            for ( int wordIndex = 2; wordIndex < words.size(); ++ wordIndex ) {
                headerValue += ':';
                headerValue += words[wordIndex];
            }
        }

        headerValue.erase(headerValue.begin());

        SetHeader(headerName, headerValue);
    }
}

std::string HttpContext::ToStdString() const {
    std::string headersString;

    for ( const auto& headerPair : _headers ) {
        std::string headerString = headerPair.first + ':' + headerPair.second + "\r\n";
        headersString += headerString;
    }

    if ( _content.length() > 0 ) {
        headersString += "\r\n";
        headersString += _content;
    }

    return headersString;
}

void HttpContext::SetContent(const std::string &content) {
    _content = content;

    int contentLength = _content.size();
    if ( contentLength > 0 ) {
        SetHeader("Content-Length", itos(contentLength));
    }
}

