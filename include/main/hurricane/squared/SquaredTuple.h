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
#include "hurricane/base/Values.h"

#include <cstdint>
#include <map>

namespace hurricane {
    namespace trident {
        class TridentTuple {
        public:
            TridentTuple(const base::Fields& fields, const base::Values& values) :
                    _fields(fields), _values(values){
                int fieldIndex = 0;
                for ( const std::string& field : fields ) {
                    _fieldMaps[field] = fieldIndex;

                    ++ fieldIndex;
                }
            }

            int32_t GetInteger(int index) {
                return _values[index].ToInt32();
            }

            int32_t GetInteger(const std::string& fieldName) {
                int index = _fieldMaps[fieldName];

                return GetInteger(index);
            }

            std::string GetString(int index) {
                return _values[index].ToString();
            }

            std::string GetString(const std::string& fieldName) {
                int index = _fieldMaps[fieldName];

                return GetString(index);
            }

            const base::Values& GetValues() {
                return _values;
            }

            void SetBatchId(int batchId) {
                _batchId = batchId;
            }

            int GetBatchId() const {
                return _batchId;
            }

        private:
            base::Fields _fields;
            std::map<std::string, int> _fieldMaps;
            base::Values _values;
            int _batchId;
        };
    }
}
