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

#ifndef NET_FRAME_LOGGER_H
#define NET_FRAME_LOGGER_H

#include "utils/concurrent_queue.h"
#include <memory>
#include <thread>
#include <queue>
#include <string>
#include <fstream>

namespace meshy {

    enum Priority {
        DEBUG,
        STATE,
        INFO,
        WARNING,
        FAULT
    };

    class Logger {
        Logger &operator=(const Logger &) = delete;

        Logger(const Logger &other) = delete;

    public:
        static Logger *Get();

        void SetPriority(Priority priority);

        Priority GetPriority();

        void WriteLog(Priority priority, const std::string &log);

    private:
        Logger(Priority priority);

        virtual ~Logger();

        void _InitializeFileStream();

        void _WriteThread();

    private:
        ConcurrentQueue <std::string> _queue;
        std::ofstream *_fileStream;
        Priority _priority;
        bool _shutdown;
    };

#define TRACE_DEBUG(LOG_CONTENT) Logger::Get()->WriteLog(DEBUG, LOG_CONTENT);
#define TRACE_STATE(LOG_CONTENT) Logger::Get()->WriteLog(STATE, LOG_CONTENT);
#define TRACE_INFO(LOG_CONTENT) Logger::Get()->WriteLog(INFO, LOG_CONTENT);
#define TRACE_WARNING(LOG_CONTENT) Logger::Get()->WriteLog(WARNING, LOG_CONTENT);
#define TRACE_ERROR(LOG_CONTENT) Logger::Get()->WriteLog(FAULT, LOG_CONTENT);

}

#endif //NET_FRAME_LOGGER_H
