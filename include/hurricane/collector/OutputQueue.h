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

#include "hurricane/base/BlockingQueue.h"
#include "hurricane/base/Values.h"

namespace hurricane {
namespace collector {

class OutputItem {
public:
    OutputItem(int32_t taskIndex, const base::Tuple& tuple, const std::string& taskName) :
            _taskIndex(taskIndex), _tuple(tuple) {
        _tuple.SetSourceTask(taskName);
    }

    int32_t GetTaskIndex() const {
        return _taskIndex;
    }

    base::Tuple& GetTuple() {
        return _tuple;
    }

    const base::Tuple& GetTuple() const {
        return _tuple;
    }

private:
    int32_t _taskIndex;
    base::Tuple _tuple;
};

class OutputQueue : public base::BlockingQueue<OutputItem*> {
};

}
}
