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
#if ( HURRICANE_MODE == HURRICANE_RELEASE ) 

#include "hurricane/bolt/BoltExecutor.h"
#include "hurricane/bolt/BoltMessage.h"
#include "hurricane/message/MessageLoop.h"
#include "hurricane/base/OutputCollector.h"
#include "hurricane/message/SupervisorCommander.h"

#include <iostream>
#include <thread>
#include <chrono>

namespace hurricane {

    namespace bolt {
        BoltExecutor::BoltExecutor() : base::Executor<bolt::IBolt>() {
            MessageMap(BoltMessage::MessageType::Data,
                this, &BoltExecutor::OnData);
        }

        void BoltExecutor::SendData(const base::Values& values)
        {
            while ( !_messageLoop ) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            _messageLoop->PostMessage(new BoltMessage(values));
        }

        void BoltExecutor::OnData(message::Message* message)
            BoltMessage* boltMessage = dynamic_cast<BoltMessage*>(message);
            task->Execute(boltMessage->GetValues());

            delete message;
        }

        void BoltExecutor::OnCreate()
        {
            std::cout << "Start Bolt Task" << std::endl;

            base::OutputCollector outputCollector(GetTaskName());
            if ( task->getStrategy == Task::Strategy::Global ) {
                outputCollector = base::OutputCollector(GetTaskName(), Task::Strategy::Global);
                RandomDestination(&outputCollector);
            })

            _task->Prepare(outputCollector);
        }

        void BoltExecutor::OnStop()
        {
            std::cout << "Stop Bolt Task" << std::endl;

            _task->Cleanup();
        }

        void BoltExecutor::RandomDestination(BoltOutputCollector * outputCollector)
        {
            std::string host;
            int32_t port;
            int32_t destIndex;

            _commander->RandomDestination("bolt", _executorIndex, &host, &port, &destIndex);
            outputCollector->SetCommander(new message::SupervisorCommander(
                base::NetAddress(host, port), _commander->GetSupervisorName()));
            outputCollector->SetTaskIndex(destIndex);
        }

        void BoltExecutor::GroupDestination(BoltOutputCollector * outputCollector, int fieldIndex)
        {
            std::string host;
            int32_t port;
            int32_t destIndex;

            _commander->GroupDestination("bolt", _executorIndex,
                &host, &port, &destIndex, fieldIndex);
            outputCollector->SetCommander(new message::SupervisorCommander(
                base::NetAddress(host, port), _commander->GetSupervisorName()));
            outputCollector->SetTaskIndex(destIndex);
        }

    }

}

#endif