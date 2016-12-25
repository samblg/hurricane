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

#include <string>
#include <cstdint>

namespace hurricane {
    namespace task {
        class TaskDeclarer {
        public:
            struct Type {
                enum {
                    Invalid = 0,
                    Spout = 1,
                    Bolt = 2
                };
            };

            struct GroupMethod {
                enum {
                    Global = 0,
                    Field = 1,
                    Random = 2
                };
            };

            TaskDeclarer() : 
                _type(Type::Invalid), _groupMethod(GroupMethod::Global), _parallismHint(1) {
            }
            const std::string& GetTopologyName() const {
                return _topologyName;
            }

            void SetTopologyName(const std::string& topologyName) {
                _topologyName = topologyName;
            }

            const std::string& GetTaskName() const {
                return _taskName;
            }

            void SetTaskName(const std::string& taskName) {
                _taskName = taskName;
            }

            int32_t GetType() const {
                return _type;
            }

            void SetType(int32_t type) {
                _type = type;
            }

            int32_t GetGroupMethod() const {
                return _groupMethod;
            }

            void SetGroupMethod(int32_t groupMethod) {
                _groupMethod = groupMethod;
            }

            const std::string& GetSourceTaskName() const {
                return _sourceTaskName;
            }

            void SetSourceTaskName(const std::string& sourceTaskName) {
                _sourceTaskName = sourceTaskName;
            }

            int32_t GetParallismHint() const {
                return _parallismHint;
            }

            void SetParallismHint(int32_t parallismHint) {
                _parallismHint = parallismHint;
            }

            std::string _topologyName;
            std::string _taskName;

            int32_t _type;
            int32_t _groupMethod;

            std::string _sourceTaskName;
            int32_t _parallismHint;
        };
    }
}
