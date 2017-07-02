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

#include "hurricane/base/OutputCollector.h"
#include "hurricane/base/Values.h"
namespace hurricane {
    namespace spout {
        class SpoutExecutor;

        class SpoutOutputCollector : public base::OutputCollector {
        public:
            SpoutOutputCollector(const std::string& src, int strategy, SpoutExecutor* executor) :
                base::OutputCollector(src, strategy), _executor(executor) {
            }

            virtual void RandomDestination() override;
            virtual void GroupDestination() override;
            void Emit(const base::Values& values, int msgId);

        private:
            SpoutExecutor* _executor;
        };
    }
}