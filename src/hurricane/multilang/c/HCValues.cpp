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

// PyHurricane.cpp : Defines the exported functions for the DLL application.
//

#include <iostream>
#include <string>

#include "hurricane/multilang/c/HCValues.h"
#include "hurricane/base/Values.h"

using hurricane::base::Value;
using hurricane::base::Values;

void PrintCValue(const CValue& value) {
    if ( value.type == HC_TYPE_INT16 ) {
        std::cout << "int16" << std::endl;
        std::cout << value.int16Value << std::endl;
    }
    else if ( value.type == HC_TYPE_INT32 ) {
        std::cout << "int32" << std::endl;
        std::cout << value.int32Value << std::endl;
    }
    else if ( value.type == HC_TYPE_STRING ) {
        std::cout << "string" << std::endl;

        std::string content(value.stringValue, value.length);
        std::cout << content << std::endl;
    }
}

void PrintCValues(CValues* values) {
    std::cout << values->length << std::endl;
    std::cout << values->values << std::endl;

    for ( int i = 0; i < values->length; i ++ ) {
        PrintCValue(values->values[i]);
    }
}

void Values2CValues(const Values& values, CValues* cValues) {
    int valueIndex = 0;

    cValues->length = values.size();
    cValues->values = new CValue[cValues->length];

    for ( const Value& value : values ) {
        if ( value.getType() == Value::Type::Int32 ) {
            cValues->values[valueIndex].type = HC_TYPE_INT32;
            cValues->values[valueIndex].int32Value = value.ToInt32();
        }
        else if ( value.getType() == Value::Type::String ) {
            const std::string& content = value.ToString();

            cValues->values[valueIndex].type = HC_TYPE_STRING;
            cValues->values[valueIndex].stringValue = 
                const_cast<char*>(content.c_str());
            cValues->values[valueIndex].length = content.length();
        }

        valueIndex ++;
    }
}

void CValues2Values(const CValues& cValues, Values* values) {
    for ( int valueIndex = 0; valueIndex != cValues.length; ++ valueIndex ) {
        CValue& cValue = cValues.values[valueIndex];
        if ( cValue.type == HC_TYPE_INT32 ) {
            values->push_back(Value(cValue.int32Value));
        }
        if ( cValue.type == HC_TYPE_STRING ) {
            values->push_back(Value(
                std::string(cValue.stringValue, cValue.length)));
        }
    }
}
