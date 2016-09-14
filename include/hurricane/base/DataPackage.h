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

        class IntWritable : public Writable {
        public:
			std::string GetType() const {
				return "int";
			}

            int32_t Read(ByteArrayReader& reader, Variant& variant) override {
                int32_t intValue = reader.read<int32_t>();
                variant.SetIntValue(intValue);

                return sizeof(int32_t);
            }

            int32_t Write(ByteArrayWriter& writer, const Variant& variant) override {
                int value = variant.GetIntValue();
                writer.write(value);

                return sizeof(int32_t);
            }
        };

        class StringWritable : public Writable {
		public:
			std::string GetType() const {
				return "string";
			}

            int32_t Read(ByteArrayReader& reader, Variant& variant) override {
                int32_t size = reader.read<int32_t>();

                ByteArray bytes = reader.readData(size);

                variant.SetStringValue(bytes.toStdString());

                return sizeof(int32_t) + bytes.size();
            }

            int32_t Write(ByteArrayWriter& writer, const Variant& variant) override {
                std::string value = variant.GetStringValue();

                writer.write(int32_t(value.size()));
                writer.write(value.c_str(), value.size());
                return sizeof(int32_t) + value.size();
            }
        };

        //extern std::map<int8_t, std::shared_ptr<Writable>> Writables;
		std::map<int8_t, std::shared_ptr<Writable>>& GetWritables();

        class DataPackage {
        public:
            DataPackage() : _version(0) {}

            void AddVariant(const Variant& variant) {
                _variants.push_back(variant);
            }

            const Variants& GetVariants() const {
                return _variants;
            }

            ByteArray Serialize() {
                ByteArray body = SerializeBody();
                ByteArray head = SerializeHead(body.size());

                return head + body;
            }

            void Deserialize(const ByteArray& data) {
                ByteArrayReader reader(data);

                DeserializeHead(reader);
                DeserializeBody(reader);
            }

        private:
            ByteArray SerializeBody() {
                ByteArrayWriter bodyWriter;
                for ( const Variant& variant : _variants ) {
                    SerializeVariant(bodyWriter, variant);
                }
                return bodyWriter.ToByteArray();
            }

            ByteArray SerializeHead(int32_t bodySize) {
                ByteArrayWriter headWriter;
                _length = sizeof(int32_t) + sizeof(_version) + bodySize;
                headWriter.write(_length);
                headWriter.write(_version);
                ByteArray head = headWriter.ToByteArray();

                return head;
            }

            void DeserializeHead(ByteArrayReader& reader) {
                _length = reader.read<int32_t>();
                _version = reader.read<int8_t>();
            }

            void DeserializeBody(ByteArrayReader& reader) {
                while ( reader.tell() < _length ) {
                    Variant variant = DeserializeVariant(reader);
                    _variants.push_back(variant);
                }
            }

            Variant DeserializeVariant(ByteArrayReader& reader) {
                Variant variant;

                if ( reader.tell() >= _length ) {
                    return variant;
                }

				int8_t typeCode = reader.read<int8_t>();
				std::map<int8_t, std::shared_ptr<Writable>>& writables = GetWritables();
				std::shared_ptr<Writable> writable = writables[typeCode];
                writable->Read(reader, variant);

                return variant;
            }

            void SerializeVariant(ByteArrayWriter& writer, const Variant& variant) {
                Variant::Type type = variant.GetType();
                int8_t typeCode = Variant::TypeCodes[type];
				std::map<int8_t, std::shared_ptr<Writable>>& writables = GetWritables();
				std::shared_ptr<Writable> writable = writables[typeCode];

                writer.write<int8_t>(typeCode);
                writable->Write(writer, variant);
            }

            int8_t _version;
            int32_t _length;
            std::vector<Variant> _variants;
        };

    }
}
