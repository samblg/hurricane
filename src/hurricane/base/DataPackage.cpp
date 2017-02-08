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

int32_t Int32Writable::Read(ByteArrayReader& reader, Variant& variant) {
    int32_t intValue = reader.Read<int32_t>();
    variant.SetInt32Value(intValue);

    return sizeof(int32_t);
}

int32_t Int32Writable::Write(ByteArrayWriter& writer, const Variant& variant) {
    int32_t value = variant.GetInt32Value();
    writer.Write(value);

    return sizeof(int32_t);
}

int32_t Int64Writable::Read(ByteArrayReader& reader, Variant& variant) {
    int64_t intValue = reader.Read<int64_t>();
    variant.SetInt64Value(intValue);

    return sizeof(int64_t);
}

int32_t Int64Writable::Write(ByteArrayWriter& writer, const Variant& variant) {
    int64_t value = variant.GetInt64Value();
    writer.Write(value);

    return sizeof(int64_t);
}

int32_t UInt32Writable::Read(ByteArrayReader& reader, Variant& variant) {
    uint32_t intValue = reader.Read<uint32_t>();
    variant.SetUInt32Value(intValue);

    return sizeof(uint32_t);
}

int32_t UInt32Writable::Write(ByteArrayWriter& writer, const Variant& variant) {
    uint32_t value = variant.GetUInt32Value();
    writer.Write(value);

    return sizeof(uint32_t);
}

int32_t UInt64Writable::Read(ByteArrayReader& reader, Variant& variant) {
    uint64_t intValue = reader.Read<uint64_t>();
    variant.SetUInt64Value(intValue);

    return sizeof(uint64_t);
}

int32_t UInt64Writable::Write(ByteArrayWriter& writer, const Variant& variant) {
    uint64_t value = variant.GetUInt64Value();
    writer.Write(value);

    return sizeof(uint64_t);
}

int32_t BooleanWritable::Read(ByteArrayReader& reader, Variant& variant) {
    bool boolValue = reader.Read<bool>();
    variant.SetBooleanValue(boolValue);

    return sizeof(bool);
}

int32_t BooleanWritable::Write(ByteArrayWriter& writer, const Variant& variant) {
    bool value = variant.GetBooleanValue();
    writer.Write(value);

    return sizeof(bool);
}

int32_t FloatWritable::Read(ByteArrayReader& reader, Variant& variant) {
    float floatValue = reader.Read<float>();
    variant.SetFloatValue(floatValue);

    return sizeof(float);
}

int32_t FloatWritable::Write(ByteArrayWriter& writer, const Variant& variant) {
    float value = variant.GetFloatValue();
    writer.Write(value);

    return sizeof(float);
}

int32_t StringWritable::Read(ByteArrayReader& reader, Variant& variant) {
    int32_t size = reader.Read<int32_t>();

    ByteArray bytes = reader.ReadData(size);

    variant.SetStringValue(bytes.ToStdString());

    return static_cast<int32_t>(sizeof(int32_t) + bytes.size());
}

int32_t StringWritable::Write(ByteArrayWriter& writer, const Variant& variant) {
    std::string value = variant.GetStringValue();

    writer.Write(int32_t(value.size()));
    writer.Write(value.c_str(), static_cast<int32_t>(value.size()));
    return static_cast<int32_t>(sizeof(int32_t) + value.size());
}

DataPackage::DataPackage() : _version(0) {}

void DataPackage::AddVariant(const Variant& variant) {
    _variants.push_back(variant);
}

ByteArray DataPackage::Serialize() {
    ByteArray body = SerializeBody();
    ByteArray head = SerializeHead(static_cast<int32_t>(body.size()));

    return head + body;
}

void DataPackage::Deserialize(const ByteArray& data) {
    ByteArrayReader reader(data);

    DeserializeHead(reader);
    DeserializeBody(reader);
}

ByteArray DataPackage::SerializeBody() {
    ByteArrayWriter bodyWriter;
    for ( const Variant& variant : _variants ) {
        SerializeVariant(bodyWriter, variant);
    }
    return bodyWriter.ToByteArray();
}

ByteArray DataPackage::SerializeHead(int32_t bodySize) {
    ByteArrayWriter headWriter;
    _length = sizeof(int32_t) + sizeof(_version) + bodySize;
    headWriter.Write(_length);
    headWriter.Write(_version);
    ByteArray head = headWriter.ToByteArray();

    return head;
}

void DataPackage::DeserializeHead(ByteArrayReader& reader) {
    _length = reader.Read<int32_t>();
    _version = reader.Read<int8_t>();
}

void DataPackage::DeserializeBody(ByteArrayReader& reader) {
    while ( reader.Tell() < _length ) {
        Variant variant = DeserializeVariant(reader);
        _variants.push_back(variant);
    }
}

Variant DataPackage::DeserializeVariant(ByteArrayReader& reader) {
    Variant variant;

    if ( reader.Tell() >= _length ) {
        return variant;
    }

    int8_t typeCode = reader.Read<int8_t>();
    std::map<int8_t, std::shared_ptr<Writable>>& writables = GetWritables();
    std::shared_ptr<Writable> writable = writables[typeCode];
    writable->Read(reader, variant);

    return variant;
}

void DataPackage::SerializeVariant(ByteArrayWriter& writer, const Variant& variant) {
    Variant::Type type = variant.GetType();
    int8_t typeCode = Variant::TypeCodes[type];
    std::map<int8_t, std::shared_ptr<Writable>>& writables = GetWritables();
    std::shared_ptr<Writable> writable = writables[typeCode];

    writer.Write<int8_t>(typeCode);
    writable->Write(writer, variant);
}

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
