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
#include "hurricane/spout/ISpout.h"

#include <iostream>

namespace hurricane {
    
    namespace topology {
        class ITopology;
    }

    namespace message {
        class SupervisorCommander;
    }

    namespace spout {

        class SpoutOutputCollector;

        class SpoutExecutor : public base::Executor<spout::ISpout> {
        public:
            SpoutExecutor() : 
                base::Executor<spout::ISpout>(), _needToStop(false) {
            }

            void StopTask() override;
            void OnCreate() override;
            void OnStop() override;

            void SetExecutorIndex(int executorIndex) {
                _executorIndex = executorIndex;
            }

            void SetCommander(message::SupervisorCommander* commander);
            void RandomDestination(SpoutOutputCollector* outputCollector);
            void GroupDestination(SpoutOutputCollector* outputCollector, int fieldIndex);

        private:
            topology::ITopology* _topology;
            bool _needToStop;
            message::SupervisorCommander* _commander;
            int _executorIndex;
        };

    }
}

