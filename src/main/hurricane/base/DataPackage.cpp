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

namespace hurricane {
	namespace base {
		std::map<int8_t, std::shared_ptr<Writable>> Writables =
		{
			{ 0, std::shared_ptr<Writable>(new IntWritable) },
			{ 3, std::shared_ptr<Writable>(new StringWritable) }
		};

		std::map<Variant::Type, int8_t> Variant::TypeCodes = {
			{ Variant::Type::Integer, 0 },
			{ Variant::Type::String, 3 }
		};

		std::map < Variant::Type, std::string > Variant::TypeNames = {
			{ Variant::Type::Invalid, "Invalid" },
			{ Variant::Type::Integer, "Integer" },
			{ Variant::Type::Boolean, "Boolean" },
			{ Variant::Type::Float, "Float" },
			{ Variant::Type::String, "String" }
		};
	}
}