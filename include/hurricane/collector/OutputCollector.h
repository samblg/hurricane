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

#include <memory>
#include <string>

namespace hurricane {
namespace base {
class Tuple;
}

namespace collector {

class OutputQueue;

class OutputCollector {
public:
    OutputCollector();
    OutputCollector(int32_t taskIndex, const std::string& taskName, std::shared_ptr<OutputQueue> queue);

    void SetQueue(std::shared_ptr<OutputQueue> queue) {
        _queue = queue;
    }

    std::shared_ptr<OutputQueue> GetQueue() const {
        return _queue;
    }

    void Emit(const hurricane::base::Tuple& tuple);

private:
    int32_t _taskIndex;
    std::string _taskName;
    std::shared_ptr<OutputQueue> _queue;
};

}
}
