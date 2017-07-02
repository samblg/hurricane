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
#include "hurricane/Hurricane.h"

#include "hurricane/spout/SpoutExecutor.h"
#include "hurricane/base/OutputCollector.h"
#include "hurricane/message/SupervisorCommander.h"
#include "hurricane/spout/SpoutOutputCollector.h"

#include <iostream>
#include <string>

namespace hurricane {
    namespace spout {
        void SpoutExecutor::StopTask()
        {
            _needToStop = true;
            Executor::StopTask();
        }

        void SpoutExecutor::OnCreate() {
            std::cout << "Start Spout Task" << std::endl;
            _needToStop = false;

            if ( _task->GetStrategy() == base::ITask::Strategy::Global ) {
                SpoutOutputCollector outputCollector(GetTaskName(), base::ITask::Strategy::Global, this);
                RandomDestination(&outputCollector);

                _task->Open(outputCollector);
            }

            while ( !_needToStop ) {
                _task->Execute();
            }

            _task->Close();
        }

        void SpoutExecutor::OnStop() {
            std::cout << "Stop Spout Task" << std::endl;
        }

        void SpoutExecutor::SetCommander(message::SupervisorCommander * commander)
        {
            _commander = commander;
        }

        void SpoutExecutor::RandomDestination(SpoutOutputCollector * outputCollector)
        {
            std::string host;
            int32_t port;
            int32_t destIndex;

            _commander->RandomDestination("spout", _executorIndex, &host, &port, &destIndex);
            outputCollector->SetCommander(new message::SupervisorCommander(
                base::NetAddress(host, port), _commander->GetSupervisorName()));
            outputCollector->SetTaskIndex(destIndex);
        }

        void SpoutExecutor::GroupDestination(SpoutOutputCollector * outputCollector, int fieldIndex)
        {
            std::string host;
            int32_t port;
            int32_t destIndex;

            _commander->GroupDestination("spout", _executorIndex, 
                &host, &port, &destIndex, fieldIndex);
            outputCollector->SetCommander(new message::SupervisorCommander(
                base::NetAddress(host, port), _commander->GetSupervisorName()));
            outputCollector->SetTaskIndex(destIndex);
        }
    }
}