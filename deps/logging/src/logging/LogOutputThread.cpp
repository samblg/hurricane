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

#include "logging/LogOutputThread.h"
#include "logging/LogItemQueue.h"
#include "logging/IOStreamManager.h"

namespace hurricane {
namespace logging {

static LogItemThread LogItemThreadInstance;

LogItemThread::LogItemThread() : _needToStop(false)
{
    _workThread = std::thread(&LogItemThread::ThreadEntry, this);
    _workThread.detach();
}

LogItemThread::~LogItemThread()
{
    _needToStop = true;
}

void LogItemThread::ThreadEntry()
{
    LogItemQueue& logItemQueue = LogItemQueue::GetInstance();

    while ( !_needToStop ) {
        LogItem logItem;
        logItemQueue.Pop(logItem);

        std::vector<std::ostream*> outputStreams =
                hurricane::logging::IOStreamManager::GetInstance().GetDefaultOutputStreams(
                    logItem.GetSeverity());
        for ( std::ostream* outputStream : outputStreams ) {
            *outputStream << logItem.GetContent();
        }
    }
}

}
}
