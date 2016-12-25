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

#include "logging/Logger.h"

#ifndef KLOG_ENABLE_THREAD
#include "logging/DirectLoggerStream.h"

namespace hurricane {
namespace logging {

typedef DirectLoggerStream ActiveLoggerStream;

}

}
#else
#include "logging/ThreadLoggerStream.h"

namespace logging {
namespace logging {

typedef ThreadLoggerStream ActiveLoggerStream;

}
}

#include "logging/LogItemQueue.h"
#include <thread>
#endif // !KLOG_ENABLE_THREAD

#include <chrono>
#include <iomanip>
#include <sstream>
#include <map>

namespace hurricane {
namespace logging {

template <class DestDuration, class SrcDuration>
int64_t DurationCount(SrcDuration srcDu);

template <class TimePoint>
std::string FormatBasicTime(const TimePoint& timePoint);

template <class TimePoint>
std::string FormatExtendTime(const TimePoint& timePoint);

static std::map<Severity, std::string>& GetSeverityMessages();

Logger::Logger(Severity severity,
               const std::string& sourceFile, int32_t sourceLine)
        : _severity(severity),
          _loggerStream(new ActiveLoggerStream(severity)),
          _sourceFile(sourceFile), _sourceLine(sourceLine), _inOutput(false)
{
}

Logger::~Logger()
{
    endl();

    _loggerStream->Output(_bufferStream.str());
    _loggerStream->Submit();

    delete _loggerStream;
    _loggerStream = nullptr;
}

void Logger::endl()
{
    _bufferStream << std::endl;
}

static std::string GetRelativeFileName(const std::string& filePath) {
#ifndef WIN32
    const char PATH_SPLASH = '/';
#else
    const char PATH_SPLASH = '\\';
#endif

    std::string::size_type pos = filePath.rfind(PATH_SPLASH);

    if ( pos == std::string::npos ) {
        return filePath;
    }

    char prefix = filePath[pos];
    if ( prefix == PATH_SPLASH ) {
        return filePath.substr(pos + 1);
    }

    return filePath.substr(pos);
}

void Logger::OutputPrefix()
{
    std::string prefix = FormatPrefix();

    _bufferStream << prefix;
}

std::string Logger::FormatPrefix()
{
    auto now = std::chrono::system_clock::now();
    std::string basicTime = FormatBasicTime(now);
    std::string extendTime = FormatExtendTime(now);

    if ( _sourceFileName.empty() ) {
        _sourceFileName = GetRelativeFileName(_sourceFile);
    }

    std::ostringstream formatter;
    formatter << "[" << GetSeverityMessages()[_severity] << "]" <<
                     "[" << basicTime << extendTime << "]" <<
                     "[" << _sourceFileName << ":" << _sourceLine << "] ";


    return formatter.str();
}

template <class DestDuration, class SrcDuration>
int64_t DurationCount(SrcDuration srcDu) {
    return srcDu.count() * DestDuration::period::den * SrcDuration::period::num /
        DestDuration::period::num / SrcDuration::period::den;
}

template <class TimePoint>
std::string FormatBasicTime(const TimePoint& timePoint) {
    time_t rawtime = TimePoint::clock::to_time_t(timePoint);
    char timeBuffer[80];
#ifdef WIN32
    tm timeInfoObj;
    tm* timeInfo = &timeInfoObj;
    localtime_s(timeInfo, &rawtime);
#else
    tm* timeInfo = localtime(&rawtime);
#endif
    strftime(timeBuffer, 80, "%Y-%m-%d %H:%M:%S.", timeInfo);

    return std::string(timeBuffer);
}

template <class TimePoint>
std::string FormatExtendTime(const TimePoint& timePoint) {
    typename TimePoint::duration du = timePoint.time_since_epoch();
    int64_t ns = DurationCount<std::chrono::nanoseconds>(du) % (1000 * 1000 * 1000);
    int64_t microseconds = ns / 1000;

    int64_t milliseconds = microseconds / 1000;
    microseconds %= 1000;

    std::ostringstream formatter;

    formatter << std::setw(3) << std::setfill('0') << milliseconds <<
                 std::setw(3) << std::setfill('0') << microseconds;

    return formatter.str();
}

static std::map<Severity, std::string>& GetSeverityMessages() {
    static std::map<Severity, std::string> severityMessages = {
        { LOG_DEBUG, "DEBUG" },
        { LOG_INFO, "INFO" },
        { LOG_WARNING, "WARNING" },
        { LOG_ERROR, "ERROR" },
        { LOG_FATAL, "FATAL" }
    };

    return severityMessages;
}

void WaitLoggerThread() {
#ifdef KLOG_ENABLE_THREAD
    while ( !LogItemQueue::GetInstance().Empty() ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
#endif // !KLOG_ENABLE_THREAD
}

}
}

hurricane::logging::Logger& std::endl(hurricane::logging::Logger& logger) {
    logger.endl();

    return logger;
}
