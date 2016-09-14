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

#include "hurricane/spout/ISpout.h"

namespace hurricane {
    namespace trident {
        class TridentSpout : public ISpout {
        public:
            TridentSpout() {}

            virtual void Open(base::OutputCollector& outputCollector) = 0;
            virtual void Close() = 0;
            virtual void Execute() = 0;

            virtual ISpout* Clone() const = 0;
        };
    }
}