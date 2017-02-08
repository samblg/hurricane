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
#include <vector>
#include <cstring>
#include <cstdint>

namespace hurricane {
namespace base {
class ByteArray : public std::vector<char> {
public:
    ByteArray() = default;
    ByteArray(size_t size);
    ByteArray(const char* buffer, int32_t size);

    std::string ToStdString() const;
    ByteArray& Concat(const ByteArray& buffer2);
    ByteArray operator+(const ByteArray& buffer2) const;
};

class IODevice {
public:
    enum class SeekMode {
        Set,
        Forward,
        Backward
    };

    ~IODevice() {}
};

class ByteArrayReader : public IODevice {
public:
    ByteArrayReader(const ByteArray& buffer);

    template <class T>
    int32_t Read(T* buffer, int32_t count);
    template <class T>
    T Read();

    ByteArray ReadData(int32_t size);
    inline int32_t Tell() const;
    void Seek(SeekMode mode, int32_t size);

private:
    ByteArray _buffer;
    int32_t _pos;
};

class ByteArrayWriter {
public:
    ByteArrayWriter() = default;

    template <class T>
    int32_t Write(const T* buffer, int32_t count);
    template <class T>
    int32_t Write(const T& value);
    int32_t Write(const ByteArray& buffer);

    inline const ByteArray& ToByteArray() const;
    inline int32_t Tell() const;

private:
    ByteArray _buffer;
};

}
}

#include "hurricane/base/ByteArray.tcc"
