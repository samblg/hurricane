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
#include <sstream>
#include <string>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include "hurricane/base/ByteArray.h"
#include "hurricane/base/Variant.h"

namespace hurricane {
namespace base {

class Writable {
public:
    virtual std::string GetType() const = 0;
    virtual int32_t Read(ByteArrayReader& reader, Variant& variant) = 0;
    virtual int32_t Write(ByteArrayWriter& writer, const Variant& variant) = 0;
};

class Int32Writable : public Writable {
public:
    std::string GetType() const {
        return "int32";
    }

    int32_t Read(ByteArrayReader& reader, Variant& variant) override;
    int32_t Write(ByteArrayWriter& writer, const Variant& variant) override;
};

class Int64Writable : public Writable {
public:
    std::string GetType() const {
        return "int64";
    }

    int32_t Read(ByteArrayReader& reader, Variant& variant) override;
    int32_t Write(ByteArrayWriter& writer, const Variant& variant) override;
};

class UInt32Writable : public Writable {
public:
    std::string GetType() const {
        return "uint32";
    }

    int32_t Read(ByteArrayReader& reader, Variant& variant) override;
    int32_t Write(ByteArrayWriter& writer, const Variant& variant) override;
};

class UInt64Writable : public Writable {
public:
    std::string GetType() const {
        return "uint64";
    }

    int32_t Read(ByteArrayReader& reader, Variant& variant) override;
    int32_t Write(ByteArrayWriter& writer, const Variant& variant) override;
};

class BooleanWritable : public Writable {
public:
    std::string GetType() const {
        return "boolean";
    }

    int32_t Read(ByteArrayReader& reader, Variant& variant) override;
    int32_t Write(ByteArrayWriter& writer, const Variant& variant) override;
};

class FloatWritable : public Writable {
public:
    std::string GetType() const {
        return "float";
    }

    int32_t Read(ByteArrayReader& reader, Variant& variant) override;
    int32_t Write(ByteArrayWriter& writer, const Variant& variant) override;
};

class StringWritable : public Writable {
public:
    std::string GetType() const {
        return "string";
    }

    int32_t Read(ByteArrayReader& reader, Variant& variant) override;
    int32_t Write(ByteArrayWriter& writer, const Variant& variant) override ;
};

std::map<int8_t, std::shared_ptr<Writable>>& GetWritables();

class DataPackage {
public:
    DataPackage();
    void AddVariant(const Variant& variant);
    ByteArray Serialize();
    void Deserialize(const ByteArray& data);

    const Variants& GetVariants() const {
        return _variants;
    }

private:
    ByteArray SerializeBody() ;
    ByteArray SerializeHead(int32_t bodySize);
    void DeserializeHead(ByteArrayReader& reader);
    void DeserializeBody(ByteArrayReader& reader);
    Variant DeserializeVariant(ByteArrayReader& reader);
    void SerializeVariant(ByteArrayWriter& writer, const Variant& variant);

    int8_t _version;
    int32_t _length;
    std::vector<Variant> _variants;
};

}
}
