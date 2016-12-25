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

#include "hurricane/task/BoltExecutor.h"
#include "hurricane/bolt/IBolt.h"
#include "hurricane/collector/TaskQueue.h"

namespace hurricane {
namespace task {

BoltExecutor::BoltExecutor()
{
    _loop.MessageMap(Executor::MessageType::OnTuple, this, &BoltExecutor::OnTuple);
}

void BoltExecutor::Start()
{
    // It is managed by main thread, so we don't need to detach it
    _thread = std::thread(&BoltExecutor::StartLoop, this);
}

void BoltExecutor::OnTuple(message::Message& message)
{
}

void BoltExecutor::StartLoop()
{
    collector::TaskItem* taskItem;

    while ( _taskQueue->Pop(taskItem) ) {
        _bolt->Execute(taskItem->GetTuple());

        delete taskItem;
        taskItem = nullptr;
    }
}


}
}
