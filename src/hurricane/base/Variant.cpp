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

#include "hurricane/base/Variant.h"

namespace hurricane {
namespace base {

Variant::Variant() : _type(Type::Invalid) {}
Variant::Variant(const char* stringValue) : _type(Type::String), _stringValue(stringValue) {
}

Variant::~Variant() {}

Variant::Variant(const Variant& variant) : _type(variant._type) {
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

const Variant& Variant::operator=(const Variant& variant) {
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

}
}
