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

#include <iostream>
#include <string>
#include <cstdint>
#include <map>
#include <vector>

namespace hurricane {
	namespace base {
		class Variant {
		public:
			enum class Type {
				Invalid,
				Integer,
				Boolean,
				Float,
				String,
			};

			static std::map<Type, int8_t> TypeCodes;
			static std::map<Type, std::string> TypeNames;

			Variant() : _type(Type::Invalid) {}
			Variant(int32_t intValue) : _type(Type::Integer), _intValue(intValue) {}
			Variant(const std::string& stringValue) : _type(Type::String), _stringValue(stringValue) {
			}

			~Variant() {}
			Variant(const Variant& variant) : _type(variant._type) {
				if ( _type == Type::Integer ) {
					_intValue = variant._intValue;
				}
				else if ( _type == Type::String ) {
					_stringValue = variant._stringValue;
				}
			}

			const Variant& operator=(const Variant& variant) {
				_type = variant._type;
				if ( _type == Type::Integer ) {
					_intValue = variant._intValue;
				}
				else if ( _type == Type::String ) {
					_stringValue = variant._stringValue;
				}

				return *this;
			}

			Type GetType() const {
				return _type;
			}

			int32_t GetIntValue() const {
				if ( _type == Type::Invalid ) {
					std::cerr << "Invalid";
				}

				if ( _type == Type::Integer ) {
					return _intValue;
				}

				throw "Type mismatched";
			}

			void SetIntValue(int32_t value) {
				_type = Type::Integer;
				_intValue = value;
			}

			std::string GetStringValue() const {
				if ( _type == Type::Invalid ) {
					std::cerr << "Invalid";
				}

				if ( _type == Type::String ) {
					return _stringValue;
				}

				throw "Type mismatched";
			}

			void SetStringValue(const std::string& value) {
				_type = Type::String;
				_stringValue = value;
			}

		private:
			Type _type;
			union {
				int32_t _intValue;
				bool _boolValue;
				float _floatValue;
			};
			std::string _stringValue;
		};

		typedef std::vector<Variant> Variants;
	}
}