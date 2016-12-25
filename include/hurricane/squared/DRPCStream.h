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

#include "hurricane/base/Fields.h"

namespace hurricane {
    namespace squared {
        class SquaredTopology;
        class SquaredSpout;
        class Operation;
        class BaseFunction;
        class BaseFilter;
        class BaseAggregator;
        class SquaredState;
        class SquaredStateFactory;
        class MapGet;

        class DRPCStream {
        public:
            DRPCStream();
            DRPCStream(const std::string& spoutName.
                SquaredSpout* spout);

            DRPCStream* Each(const base::Fields& inputFields,
                Operation* operation, const base::Fields& outputFields);
            DRPCStream* GroupBy(const Fields& fields);
            DRPCStream* StateQuery(SquaredState* state, base::Fields& inputFields,
                MapGet* mapGetter, base::Fields& outputFields);
            DRPCStream* Aggregate(base::Fields& inputFields, BaseAggregator* aggregator,
                base::Fields* outputFields);
            std::string WaitFormResult(const std::string& args);

            void Deploy(SquaredToplogy* topology);
        };
    }
}