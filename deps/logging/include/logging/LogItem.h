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

#include "logging/Logger.h"

#include <string>

namespace hurricane {
namespace logging {

class LogItem {
public:
    LogItem() : _empty(true) {}

    LogItem(Severity severity, const std::string& content)
            : _empty(false), _severity(severity), _content(content) {
    }

    bool Empty() const {
        return _empty;
    }

    void SetEmpty(bool empty) {
        _empty = empty;
    }

    Severity GetSeverity() const {
        return _severity;
    }

    void SetSeverity(Severity severity) {
        _severity = severity;
    }

    const std::string& GetContent() const {
        return _content;
    }

    void SetContent(const std::string& content) {
        _content = content;
    }

private:
    bool _empty;
    Severity _severity;
    std::string _content;
};

}
}
