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

#include "hurricane/base/Executor.h"
#include "hurricane/bolt/IBolt.h"
#include "hurricane/base/Values.h"

namespace hurricane {

    namespace topology {
        class ITopology;
    }

    namespace message {
        class Message;
        class SupervisorCommander;
    }

    namespace bolt {

        class BoltMessageLoop;
        class BoltOutputCollector;

        class BoltExecutor : public base::Executor<bolt::IBolt> {
        public:
            BoltExecutor();

            void SetExecutorIndex(int executorIndex) {
                _executorIndex = executorIndex;
            }

            void SendData(const base::Values& values);
            void OnData(hurricane::message::Message* message);

            void OnCreate() override;
            void OnStop() override;

            void SetCommander(message::SupervisorCommander* commander) {
                _commander = commander;
            }
            void RandomDestination(BoltOutputCollector* outputCollector);
            void GroupDestination(BoltOutputCollector* outputCollector, int fieldIndex);

        private:
            topology::ITopology* _topology;
            message::SupervisorCommander* _commander;
            int _executorIndex;
        };

    }
}
