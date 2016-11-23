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

#include "utils/logger.h"
#include "utils/time.h"
#include <iostream>
#include <sstream>

namespace meshy {
    const std::string PRIORITY_STRING[] =
            {
                    "DEBUG",
                    "CONFIG",
                    "INFO",
                    "WARNING",
                    "ERROR"
            };

    Logger *Logger::Get() {
        static Logger logger(DEBUG);
        return &logger;
    }

    Logger::Logger(Priority priority) : _queue(), _fileStream(nullptr), _shutdown(false) {
        _priority = priority;
        _InitializeFileStream();
        auto func = std::bind(&Logger::_WriteThread, this);
        std::thread writeThread(func);
        writeThread.detach();
    }

    Logger::~Logger() {
        _shutdown = true;

        if (nullptr != _fileStream) {
            _fileStream->close();
            delete _fileStream;
            _fileStream = nullptr;
        }
    }

    void Logger::SetPriority(Priority priority) {
        _priority = priority;
    }

    Priority Logger::GetPriority() {
        return _priority;
    }

    void Logger::_InitializeFileStream() {
        // Prepare fileName
        std::string fileName = "logs/Hurricane_log.log";

        // Initialize file stream
        _fileStream = new std::ofstream();
        std::ios_base::openmode mode = std::ios_base::out;
        mode |= std::ios_base::trunc;
        _fileStream->open(fileName, mode);

        // Error handling
        if (!_fileStream->is_open()) {
            // Print error information
            std::ostringstream ss_error;
            ss_error << "FATAL ERROR:  could not Open log file: [" << fileName << "]";
            ss_error << "\n\t\t std::ios_base state = " << _fileStream->rdstate();
            std::cerr << ss_error.str().c_str() << std::endl << std::flush;

            // Cleanup
            _fileStream->close();
            delete _fileStream;
            _fileStream = nullptr;
        }
    }

    void Logger::WriteLog(Priority priority, const std::string &log) {
        if (priority < _priority)
            return;

        std::stringstream stream;
        stream << HurricaneUtils::GetCurrentTimeStamp()
        << " [" << PRIORITY_STRING[priority] << "] "
        << log;

        _queue.Push(stream.str());
    }

    void Logger::_WriteThread() {
        while (!_shutdown) {
            std::string log;
            _queue.Pop(log, true);

            std::cout << log << std::endl;

            if (_fileStream)
                *_fileStream << log << std::endl;
        }
    }
}
