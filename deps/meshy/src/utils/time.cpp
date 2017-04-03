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

#include "utils/time.h"
#include <sstream>
#include <assert.h>

namespace HurricaneUtils
{
    // TODO: Add special implementation for put_time in Linux
    std::string GetCurrentTimeStamp()
    {
        // get current time
        auto currentTime = std::chrono::system_clock::now();
        // get milliseconds
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;

        auto currentTimePoint = std::chrono::system_clock::to_time_t(currentTime);

        // output the time stamp
        std::ostringstream stream;

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
        stream << std::put_time(std::localtime(&currentTimePoint), "%T");
#else
        char buffer[80];
        auto success = std::strftime(buffer, 80, "%T", std::localtime(&currentTimePoint));
        assert(0 != success);
        stream << buffer;
#endif

        stream << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

        return stream.str();
    }
}
