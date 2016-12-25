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

#include "hurricane/task/SpoutExecutor.h"
#include "hurricane/spout/ISpout.h"
#include <chrono>

namespace hurricane {
namespace task {

const int32_t BASIC_FLOW_PARAM = 1000 * 1000;

SpoutExecutor::SpoutExecutor()
{

}

void SpoutExecutor::Start()
{
    // It is managed by main thread, so we don't need to detach it
    _thread = std::thread(&SpoutExecutor::MainLoop, this);
}

void SpoutExecutor::SetSpout(spout::ISpout* spout)
{
    _spout.reset(spout);
}

void SpoutExecutor::MainLoop()
{
    int32_t flowTime = BASIC_FLOW_PARAM / _flowParam;
    while ( true ) {
        _spout->NextTuple();
        std::this_thread::sleep_for(std::chrono::microseconds(flowTime));
    }
}

int32_t SpoutExecutor::GetFlowParam() const
{
    return _flowParam;
}

void SpoutExecutor::SetFlowParam(int32_t flowParam)
{
    _flowParam = flowParam;
}

}
}
