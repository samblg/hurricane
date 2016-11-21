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

#include "hurricane/base/DataPackage.h"
#include <iostream>

namespace hurricane {
    namespace base {
		class AllWritables {
		public:
			AllWritables() {
				_writables.insert({ 0, std::shared_ptr<Writable>(new Int32Writable) });
                _writables.insert({ 1, std::shared_ptr<Writable>(new Int64Writable) });
                _writables.insert({ 2, std::shared_ptr<Writable>(new UInt32Writable) });
                _writables.insert({ 3, std::shared_ptr<Writable>(new UInt64Writable) });
                _writables.insert({ 4, std::shared_ptr<Writable>(new BooleanWritable) });
                _writables.insert({ 5, std::shared_ptr<Writable>(new FloatWritable) });
                _writables.insert({ 6, std::shared_ptr<Writable>(new StringWritable) });
			}

			std::map<int8_t, std::shared_ptr<Writable>> _writables;
		};


		std::map<int8_t, std::shared_ptr<Writable>>& GetWritables() {
			static AllWritables allWritables;

			return allWritables._writables;
		}

        std::map<Variant::Type, int8_t> Variant::TypeCodes = {
            { Variant::Type::Int32, 0 },
            { Variant::Type::Int64, 1 },
            { Variant::Type::UInt32, 2 },
            { Variant::Type::UInt64, 3 },
            { Variant::Type::Boolean, 4 },
            { Variant::Type::Float, 5 },
            { Variant::Type::String, 6 }
        };

        std::map < Variant::Type, std::string > Variant::TypeNames = {
            { Variant::Type::Invalid, "Invalid" },
            { Variant::Type::Int32, "Int32" },
            { Variant::Type::Int64, "Int64" },
            { Variant::Type::UInt32, "UInt32" },
            { Variant::Type::UInt64, "UInt64" },
            { Variant::Type::Boolean, "Boolean" },
            { Variant::Type::Float, "Float" },
            { Variant::Type::String, "String" }
        };
    }
}
