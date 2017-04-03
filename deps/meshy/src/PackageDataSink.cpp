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

#include "PackageDataSink.h"
#include "eventqueue.h"
#include "utils/logger.h"
#include "net.h"
#include "rest/HttpContext.h"
#include "rest/HttpRequest.h"
#include "rest/HttpResponse.h"

namespace meshy {
    PackageDataSink::PackageDataSink(EventQueue *eventQueue) : _eventQueue(eventQueue), _totalSize(0) {
        _threadPool = new ThreadPool<BaseEvent>(10, [&](BaseEvent &event) {
            TRACE_DEBUG("Thread onEvent sink!");

            std::string requestText = event.GetData().ToStdString();

            HttpRequest request;
            request.ParseStdString(requestText);

            HttpResponse response;
            response.SetVersion("HTTP/1.1");
            response.SetStatusCode(200);
            response.SetStatusMessage("OK");
            response.SetContent("Hello! Sink in Thread!");

            event.GetStream()->Send(ByteArray(response.ToStdString())); // Send to peer

            TRACE_DEBUG("Thread onEvent sink end.");
        });
    }

    PackageDataSink::~PackageDataSink() {
        if (_threadPool) {
            delete _threadPool;
            _threadPool = nullptr;
        }
    }

    int32_t PackageDataSink::OnDataIndication(IStream *stream, const char *buf, int64_t bytes) {
        _data.Concat(ByteArray(buf, bytes));
        // The package is Complete
        if (_data.size() >= _totalSize) {
            //_eventQueue->PostEvent(new BaseEvent("data", _data, connection));
            _threadPool->Submit(BaseEvent("data", _data, stream));

            _data.clear();
            _totalSize = 0;
        }


        return bytes;
    }
}
