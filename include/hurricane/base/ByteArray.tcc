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

namespace hurricane {
namespace base {

template <class T>
int32_t ByteArrayReader::Read(T* buffer, int32_t count) {
    if ( _pos >= static_cast<int32_t>(_buffer.size()) ) {
        return 0;
    }

    size_t sizeToRead = sizeof(T) * count;
    if ( _pos + sizeToRead > static_cast<int32_t>(_buffer.size()) ) {
        sizeToRead = _buffer.size() - _pos;
    }

    memcpy(buffer, _pos + _buffer.data(), sizeToRead);
    _pos += static_cast<int32_t>(sizeToRead);

    return static_cast<int32_t>(sizeToRead);
}

template <class T>
T ByteArrayReader::Read() {
    T t;
    Read(&t, 1);

    return t;
}

int32_t ByteArrayReader::Tell() const {
    return _pos;
}

template <class T>
int32_t ByteArrayWriter::Write(const T* buffer, int32_t count) {
    int32_t sizeToWrite = sizeof(T) * count;
    ByteArray buffer2((const char*)(buffer), sizeToWrite);
    _buffer.Concat(buffer2);

    return sizeToWrite;
}

template <class T>
int32_t ByteArrayWriter::Write(const T& value) {
    return Write(&value, 1);
}

const ByteArray& ByteArrayWriter::ToByteArray() const {
    return _buffer;
}

int32_t ByteArrayWriter::Tell() const {
    return static_cast<int32_t>(_buffer.size());
}

}
}
