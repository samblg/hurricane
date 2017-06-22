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


#ifndef NET_FRAME_DATASINK_H
#define NET_FRAME_DATASINK_H

#include "Net.h"
#include "Export.h"

#include <cstdint>
#include <functional>

namespace meshy {
    class MESHY_API DataSink {
    public:
        virtual int32_t Write(IStream* stream, const char* buf, int64_t bytes) = 0;
        IStream::DataHandler StreamDataHandler(IStream* stream) {
            return std::bind(&DataSink::Write, this, stream, std::placeholders::_1, std::placeholders::_2);
        }
    };
}

#endif //NET_FRAME_DATASINK_H
