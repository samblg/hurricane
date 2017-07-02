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

#include "hurricane/base/Values.h"
#include "hurricane/message/SupervisorCommander.h"

namespace hurricane {

    namespace topology {
    class ITopology;
    }

    namespace base {

        class OutputCollector {
        public:
            struct Strategy {
                enum Values {
                    Global = 0,
                    Random = 1,
                    Group = 2
                };
            };    

            OutputCollector(const std::string& src, int strategy) :
                _src(src), _strategy(strategy), _commander(nullptr) {}

            virtual void Emit(const Values& values);
            void SetCommander(hurricane::message::SupervisorCommander* commander) {
                if ( _commander ) {
                    delete _commander;
                }

                _commander = commander;
            }

            void SetTaskIndex(int taskIndex) {
                _taskIndex = taskIndex;
            }

            void SetGroupField(int groupField) {
                _groupField = groupField;
            }

            int GetGroupField() const {
                return _groupField;
            }

            int GetStrategy() const {
                return _strategy;
            }

            virtual void RandomDestination() {};
            virtual void GroupDestination() {};

        private:
            std::string _src;
            int _strategy;
            int _taskIndex;
            hurricane::message::SupervisorCommander* _commander;
            int _groupField;
        };

    }
}
