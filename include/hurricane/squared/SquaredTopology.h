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

#include "hurricane/topology/SimpleTopology.h"
#include <memory>
#include <map>

namespace hurricane {
    namespace squared {
        class SquaredSpout;
        class SquaredStream;
        class DRPCStream;

        class SquaredToplogy : public SimpleTopology {
        public:
            SquaredToplogy SquaredToplogy();
            SquaredStream* NewStream(const std::string& spoutName,
                SquaredSpout* squaredSpout);

            DRPCStream* NewDRPCStream(const std::string& serviceName);

            void Deploy();
            
        private:
            void _Deploy(SquaredStream* stream);
            void _Deploy(DRPCStream* stream);

        private:
            std::vector<std::shared_ptr<SquaredStream>> _squaredStreams;
            std::map<std::string, std::shared_ptr<DRPCStream>> _drpcStreams;
        };
    }
}