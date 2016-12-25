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

#include "hurricane/task/Executor.h"
#include <thread>
#include <memory>

namespace hurricane {

namespace spout {
    class ISpout;
}
namespace task {

class SpoutExecutor : public Executor {
public:
    SpoutExecutor();
    ~SpoutExecutor() {}

    void Start();
    void SetSpout(spout::ISpout* spout);

    std::shared_ptr<spout::ISpout> GetSpout() {
        return _spout;
    }

    int32_t GetFlowParam() const;
    void SetFlowParam(int32_t GetFlowParam);

private:
    void MainLoop();

    std::thread _thread;
    std::shared_ptr<spout::ISpout> _spout;
    int32_t _flowParam;
};

}
}
