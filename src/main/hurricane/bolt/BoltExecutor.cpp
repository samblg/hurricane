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

#include "hurricane/bolt/BoltExecutor.h"
#include "hurricane/bolt/BoltMessage.h"
#include "hurricane/message/MessageLoop.h"
#include "hurricane/bolt/BoltOutputCollector.h"
#include "hurricane/message/SupervisorCommander.h"

#include <iostream>
#include <thread>
#include <chrono>

#ifdef WIN32
#ifdef PostMessage
#undef PostMessage
#endif // PostMessage
#endif // WIN32

namespace hurricane {

    namespace bolt {
        BoltExecutor::BoltExecutor() : base::Executor<bolt::IBolt>() {
            _messageLoop.MessageMap(BoltMessage::MessageType::Data,
                this, &BoltExecutor::OnData);
        }

        void BoltExecutor::SendData(const base::Values& values)
        {
            _messageLoop.PostMessage(new BoltMessage(values));
        }

        void BoltExecutor::OnData(hurricane::message::Message* message) {
            BoltMessage* boltMessage = dynamic_cast<BoltMessage*>(message);
            _task->Execute(boltMessage->GetValues());

            delete message;
        }

        void BoltExecutor::OnCreate()
        {
            std::cout << "Start Bolt Task" << std::endl;

            if ( _task->GetStrategy() == base::ITask::Strategy::Global ) {
                BoltOutputCollector outputCollector(GetTaskName(), base::OutputCollector::Strategy::Global, this);
                RandomDestination(&outputCollector);

                _task->Prepare(outputCollector);
            }
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