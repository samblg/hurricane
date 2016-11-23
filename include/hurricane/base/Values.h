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
#include <exception>
#include <cstdint>
#include <vector>
#include <initializer_list>
#include <iostream>
#include "hurricane/base/Variant.h"

namespace hurricane {
    namespace base {
        typedef Variant Value;

        class Values : public std::vector<Value> {
        public:
            Values() = default;
            Values(std::initializer_list<Value> values) : std::vector<Value>(values) {
            }

            Value& operator[](size_t index) {
                return std::vector<Value>::operator[](index);
            }

            const Value& operator[](size_t index) const {
                return std::vector<Value>::operator[](index);
            }
        };

        class Tuple : public base::Serializable {
		public:
			Tuple() = default;
			Tuple(std::initializer_list<Value> values) : _values(values) {
			}

			Value& operator[](size_t index) {
				return _values[index];
			}

			const Value& operator[](size_t index) const {
				return _values[index];
			}

            Value& operator[](const std::string& fieldName) {
                return _values[_fieldsMap->at(fieldName)];
            }

            const Value& operator[](const std::string& fieldName) const {
                return _values[_fieldsMap->at(fieldName)];
            }

			int GetSize() const {
				return _values.size();
			}

			void Add(const Value& value) {
				_values.push_back(value);
			}

            void SetSourceTask(const std::string& sourceTask) {
                _sourceTask = sourceTask;
            }

            const std::string& GetSourceTask() const {
                return _sourceTask;
            }

            void SetDestTask(const std::string& destTask) {
                _destTask = destTask;
            }

            const std::string& GetDestTask() const {
                return _destTask;
            }

            const Values& GetValues() const {
                return _values;
            }

            void SetFields(const std::vector<std::string>* fields) {
                _fields = fields;
            }

            const std::vector<std::string>* GetFields() const {
                return _fields;
            }

            void SetFieldsMap(const std::map<std::string, int>* fieldsMap) {
                _fieldsMap = fieldsMap;
            }

            void Serialize(Variants& variants) const override;
            void Deserialize(Variants::const_iterator& it) override;

		private:
            std::string _sourceTask;
            std::string _destTask;
            Values _values;
            const std::vector<std::string>* _fields;
            const std::map<std::string, int>* _fieldsMap;
        };
    }
}
