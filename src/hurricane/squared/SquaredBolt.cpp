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

#include "TridentBolt.h"
#include "TridentTuple.h"

namespace hurricane {
    namespace trident {
        TridentBolt::TridentBolt(const base::Fields & inputFields,
            const base::Fields & outputFields) :
                _inputFields(inputFields),
                _outputFields(outputFields)
        {
        }

        void TridentBolt::Prepare(base::OutputCollector & outputCollector)
        {
            this->_collector = &outputCollector;
        }

        void TridentBolt::Cleanup()
        {
        }

        void TridentBolt::Execute(const base::Values & values)
        {
            Execute(TridentTuple(_inputFields, values), _collector)
        }

        IBolt* TridentBolt::Clone() const
        {
            return new TridentBolt(this);
        }
        
        Fields TridentBolt::DeclareFields() const
        {
            return _outputFields;
        }
    }
}