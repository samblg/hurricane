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

#pragma once

#include "logging/Severity.h"

#include <vector>
#include <map>
#include <set>

namespace hurricane {
namespace logging {

class IOStreamManager {
public:
    static IOStreamManager& GetInstance();

    ~IOStreamManager();

    std::vector<std::ostream*> GetDefaultOutputStreams(Severity severity);
    void SetOutputStream(std::ostream* outputStream, bool toManage = true);
    void SetOutputStream(hurricane::logging::Severity severity,
                         std::ostream* outputStream, bool toManage = true);
    void SetOutputStream(std::vector<hurricane::logging::Severity> severities,
                         std::ostream* outputStream, bool toManage = true);

    void SetOutputFile(const std::string& fileName);
    void SetOutputFile(hurricane::logging::Severity severity, const std::string& fileName);
    void SetOutputFile(std::vector<hurricane::logging::Severity> severities, const std::string& fileName);

private:
    IOStreamManager();
    std::ostream* CreateOutputFile(const std::string& fileName);

private:
    std::map<Severity, std::vector<std::ostream*>> _defaultOutputStreams;
    std::set<std::ostream*> _managedStreams;
};

}
}
