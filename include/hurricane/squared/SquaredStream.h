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
#include <list>
#include <memory>

namespace hurricane {
    namespace bolt {
        class IBolt;
    }

    namespace squared {
        class SquaredToplogy;
        class SquaredSpout;
        class Operation;
        class BaseFunction;
        class BaseFilter;
        class BaseAggregator;
        class SquaredState;
        class SquaredStateFactory;

        class SquaredStream {
        public:
            SquaredStream();
            SquaredStream(const std::string& spoutName,
                SquaredSpout* spout);

            SquaredStream* Each(const base::Fields& inputFields,
                Operation* operation, const base::Fields& outputFields);
            SquaredStream* GroupBy(const base::Fields& fields);
            SquaredState* PersistentAggregate(const SquaredStateFactory* factory,
                BaseAggregator* operation, const base::Fields& fields);

            void Deploy(SquaredToplogy* topology);

        private:
            std::string _spoutName;
            std::shared_ptr<SquaredSpout> _spout;
            std::list<std::shared_ptr<bolt::IBolt>> _bolts;
            std::list<std::string> _boltNames;
        };
    }
}