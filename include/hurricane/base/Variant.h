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
#include <set>
#include <algorithm>
#include <list>


#define VARIANT_GETTER(TypeName, CType, valueName) \
    CType Get##TypeName##Value() const { \
        if ( _type == Type::Invalid ) { \
            std::cerr << "Invalid"; \
        } \
    \
        if ( _type == Type::##TypeName ) { \
            return valueName; \
        } \
    \
        throw "Type mismatched"; \
    }

namespace hurricane {
    namespace base {
        class Variant;
        typedef std::vector<Variant> Variants;

        class Serializable {
        public:
            virtual void Serialize(Variants& variants) const = 0;
            virtual void Deserialize(Variants::const_iterator& it) = 0;
        };

        class Variant {
        public:
            enum class Type {
                Invalid,
                Int32,
                Int64,
                UInt32,
                UInt64,
                Boolean,
                Float,
                String,
            };

            static std::map<Type, int8_t> TypeCodes;
            static std::map<Type, std::string> TypeNames;

            Variant() : _type(Type::Invalid) {}
            Variant(int32_t intValue) : _type(Type::Int32), _int32Value(intValue) {}
            Variant(int64_t longValue) : _type(Type::Int64), _int64Value(longValue) {}
            Variant(uint32_t intValue) : _type(Type::UInt32), _uint32Value(intValue) {}
            Variant(uint64_t longValue) : _type(Type::UInt64), _uint64Value(longValue) {}
            Variant(bool boolValue) : _type(Type::Boolean), _boolValue(boolValue) {}
            Variant(float floatValue) : _type(Type::Float), _floatValue(floatValue) {}
            Variant(const std::string& stringValue) : _type(Type::String), _stringValue(stringValue) {
            }
            Variant(const char* stringValue) : _type(Type::String), _stringValue(stringValue) {
            }

            ~Variant() {}
            Variant(const Variant& variant) : _type(variant._type) {
                if ( _type == Type::Int32 ) {
                    _int32Value = variant._int32Value;
                }
                else if ( _type == Type::Int64 ) {
                    _int64Value = variant._int64Value;
                }
                else if ( _type == Type::UInt32 ) {
                    _uint32Value = variant._uint32Value;
                }
                else if ( _type == Type::UInt64 ) {
                    _uint64Value = variant._uint64Value;
                }
                else if ( _type == Type::Boolean ) {
                    _boolValue = variant._boolValue;
                }
                else if ( _type == Type::Float ) {
                    _floatValue = variant._floatValue;
                }
                else if ( _type == Type::String ) {
                    _stringValue = variant._stringValue;
                }
            }

            const Variant& operator=(const Variant& variant) {
                _type = variant._type;
                if ( _type == Type::Int32 ) {
                    _int32Value = variant._int32Value;
                }
                else if ( _type == Type::Int64 ) {
                    _int64Value = variant._int64Value;
                }
                else if ( _type == Type::UInt32 ) {
                    _uint32Value = variant._uint32Value;
                }
                else if ( _type == Type::UInt64 ) {
                    _uint64Value = variant._uint64Value;
                }
                else if ( _type == Type::Boolean ) {
                    _boolValue = variant._boolValue;
                }
                else if ( _type == Type::Float ) {
                    _floatValue = variant._floatValue;
                }
                else if ( _type == Type::String ) {
                    _stringValue = variant._stringValue;
                }

                return *this;
            }

            Type GetType() const {
                return _type;
            }

            template <class Value>
            Value GetValue() const {
                throw "Not Implemented";
            }

            int32_t GetInt32Value() const {
                if ( _type == Type::Invalid ) {
                    std::cerr << "Invalid";
                }

                if ( _type == Type::Int32 ) {
                    return _int32Value;
                }

                std::cout << "Type mismatched. " <<
                             "Expected: " << TypeNames[Type::Int32] <<
                             ". Actually: " << TypeNames[_type] << std::endl;
                throw "Type mismatched";
            }

            void SetInt32Value(int32_t value) {
                _type = Type::Int32;
                _int32Value = value;
            }

            int64_t GetInt64Value() const {
                if ( _type == Type::Invalid ) {
                    std::cerr << "Invalid";
                }

                if ( _type == Type::Int64 ) {
                    return _int64Value;
                }

                std::cout << "Type mismatched. " <<
                             "Expected: " << TypeNames[Type::Int64] <<
                             ". Actually: " << TypeNames[_type] << std::endl;
                throw "Type mismatched";
            }

            void SetInt64Value(int64_t value) {
                _type = Type::Int64;
                _int64Value = value;
            }

            uint32_t GetUInt32Value() const {
                if ( _type == Type::Invalid ) {
                    std::cerr << "Invalid";
                }

                if ( _type == Type::UInt32 ) {
                    return _uint32Value;
                }

                std::cout << "Type mismatched. " <<
                             "Expected: " << TypeNames[Type::UInt32] <<
                             ". Actually: " << TypeNames[_type] << std::endl;
                throw "Type mismatched";
            }

            void SetUInt32Value(uint32_t value) {
                _type = Type::UInt32;
                _uint32Value = value;
            }

            uint64_t GetUInt64Value() const {
                if ( _type == Type::Invalid ) {
                    std::cerr << "Invalid";
                }

                if ( _type == Type::UInt64 ) {
                    return _uint64Value;
                }

                std::cout << "Type mismatched. " <<
                             "Expected: " << TypeNames[Type::UInt64] <<
                             ". Actually: " << TypeNames[_type] << std::endl;
                throw "Type mismatched";
            }

            void SetUInt64Value(uint64_t value) {
                _type = Type::UInt64;
                _uint64Value = value;
            }

            bool GetBooleanValue() const {
                if ( _type == Type::Invalid ) {
                    std::cerr << "Invalid";
                }

                if ( _type == Type::Boolean ) {
                    return _boolValue;
                }

                std::cout << "Type mismatched. " <<
                             "Expected: " << TypeNames[Type::Boolean] <<
                             ". Actually: " << TypeNames[_type] << std::endl;
                throw "Type mismatched";
            }

            void SetBooleanValue(bool value) {
                _type = Type::Boolean;
                _boolValue = value;
            }

            float GetFloatValue() const {
                if ( _type == Type::Invalid ) {
                    std::cerr << "Invalid";
                }

                if ( _type == Type::Float ) {
                    return _floatValue;
                }

                std::cout << "Type mismatched. " <<
                             "Expected: " << TypeNames[Type::Float] <<
                             ". Actually: " << TypeNames[_type] << std::endl;
                throw "Type mismatched";
            }

            void SetFloatValue(float value) {
                _type = Type::Float;
                _floatValue = value;
            }

            std::string GetStringValue() const {
                if ( _type == Type::Invalid ) {
                    std::cerr << "Invalid";
                }

                if ( _type == Type::String ) {
                    return _stringValue;
                }

                std::cout << "Type mismatched. " <<
                             "Expected: " << TypeNames[Type::String] <<
                             ". Actually: " << TypeNames[_type] << std::endl;
                throw "Type mismatched";
            }

            void SetStringValue(const std::string& value) {
                _type = Type::String;
                _stringValue = value;
            }

<<<<<<< HEAD
=======
            static void Deserialize(Variants::const_iterator& it, Variant& value) {
                value = *it;
                it ++;
            }

>>>>>>> master
            static void Deserialize(Variants::const_iterator& it, int32_t& value) {
                value = it->GetInt32Value();
                it ++;
            }

            static void Deserialize(Variants::const_iterator& it, int64_t& value) {
                value = it->GetInt64Value();
                it ++;
            }

            static void Deserialize(Variants::const_iterator& it, uint32_t& value) {
                value = it->GetUInt32Value();
                it ++;
            }

            static void Deserialize(Variants::const_iterator& it, uint64_t& value) {
                value = it->GetUInt64Value();
                it ++;
            }

            static void Deserialize(Variants::const_iterator& it, std::string& value) {
                value = it->GetStringValue();
                it ++;
            }

            static void Deserialize(Variants::const_iterator& it, Serializable& value) {
                value.Deserialize(it);
            }

            template <class Element>
            static void Deserialize(Variants::const_iterator& it, std::vector<Element>& values) {
                size_t size = 0;
                Deserialize(it, size);

                values.resize(size);
                for ( Element& value : values ) {
                    Deserialize(it, value);
                }
            }
<<<<<<< HEAD

            template <class Element>
            static void Deserialize(Variants::const_iterator& it, std::list<Element>& values) {
                size_t size = 0;
                Deserialize(it, size);

=======

            template <class Element>
            static void Deserialize(Variants::const_iterator& it, std::list<Element>& values) {
                size_t size = 0;
                Deserialize(it, size);

>>>>>>> master
                values.resize(size);
                for ( Element& value : values ) {
                    Deserialize(it, value);
                }
            }

            template <class Element>
            static void Deserialize(Variants::const_iterator& it, std::set<Element>& values) {
                size_t size = 0;
                Deserialize(it, size);

                values.clear();
                for ( int i = 0; i != size; ++ i ) {
                    Element value;
                    Deserialize(it, value);

                    values.insert(value);
                }
            }

            template <class Key, class Element>
            static void Deserialize(Variants::const_iterator& it, std::map<Key, Element>& values) {
                size_t size = 0;
                Deserialize(it, size);

                for ( int i = 0; i != size; ++ i ) {
                    Key key;
                    Deserialize(it, key);

                    Element value;
                    Deserialize(it, value);

                    values.insert({ key, value });
                }
            }
<<<<<<< HEAD

            static void Serialize(Variants& variants, int32_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, int64_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, uint32_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, uint64_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, bool value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, float value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, const std::string& value) {
                variants.push_back(Variant(value));
            }

=======

            static void Serialize(Variants& variants, const Variant& value) {
                variants.push_back(value);
            }

            static void Serialize(Variants& variants, int32_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, int64_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, uint32_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, uint64_t value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, bool value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, float value) {
                variants.push_back(Variant(value));
            }

            static void Serialize(Variants& variants, const std::string& value) {
                variants.push_back(Variant(value));
            }

>>>>>>> master
            template <class Element>
            static void Serialize(Variants& variants, std::vector<Element> values) {
                variants.push_back(Variant(values.size()));
                for ( const Element& value : values ) {
                    Serialize(variants, value);
                }
            }

            template <class Element>
            static void Serialize(Variants& variants, std::list<Element> values) {
                variants.push_back(Variant(values.size()));
                for ( const Element& value : values ) {
                    Serialize(variants, value);
                }
            }

            template <class Element>
            static void Serialize(Variants& variants, std::set<Element> values) {
                variants.push_back(Variant(values.size()));
                for ( const Element& value : values ) {
                    Serialize(variants, value);
                }
            }

            template <class Key, class Element>
            static void Serialize(Variants& variants, std::map<Key, Element> values) {
                variants.push_back(Variant(values.size()));
                for ( const std::pair<Key, Element>& value : values ) {
                    Serialize(variants, value.first);
                    Serialize(variants, value.second);
                }
            }

            static void Serialize(Variants& variants, const Serializable& object) {
                object.Serialize(variants);
            }

        private:
            Type _type;
            union {
                int32_t _int32Value;
                int64_t _int64Value;
                uint32_t _uint32Value;
                uint64_t _uint64Value;
                bool _boolValue;
                float _floatValue;
            };
            std::string _stringValue;
        };

        template<> inline int32_t Variant::GetValue<int32_t>() const {
            return GetInt32Value();
        }

        template<> inline int64_t Variant::GetValue<int64_t>() const {
            return GetInt64Value();
        }

        template<> inline uint32_t Variant::GetValue<uint32_t>() const {
            return GetUInt32Value();
        }

        template<> inline uint64_t Variant::GetValue<uint64_t>() const {
            return GetUInt64Value();
        }

        template<> inline bool Variant::GetValue<bool>() const {
            return GetBooleanValue();
        }

        template<> inline float Variant::GetValue<float>() const {
            return GetFloatValue();
        }

        template<> inline std::string Variant::GetValue<std::string>() const {
            return GetStringValue();
        }
    }
}
