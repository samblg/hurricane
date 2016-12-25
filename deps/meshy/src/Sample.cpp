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

#include "Net.h"
#include "EventQueue.h"
#include "EventQueueLoop.h"
#include "IoLoop.h"
#include "PackageDataSink.h"
#include "PackageDataSink.h"
#include "Net.h"
#include "Meshy.h"
#include "rest/HttpServer.h"
#include "logging/Logging.h"

#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <memory>

using namespace std;

const int32_t DefaultPort = 9000;
class SampleEventQueueLoop : public meshy::EventQueueLoop {
public:
    SampleEventQueueLoop(meshy::EventQueue* eventQueue) :
        EventQueueLoop(eventQueue) {}

protected:
    virtual void OnEvent(std::shared_ptr<meshy::BaseEvent> event) override {
    }
};

using hurricane::logging::Logging;

int32_t main() {
    Logging::SetLogFile("all.log");
    Logging::SetLogFile({ LOG_INFO, LOG_DEBUG }, "output.log");
    Logging::SetLogFile({ LOG_WARNING, LOG_ERROR, LOG_FATAL }, "error.log");

    meshy::EventQueue mainEventQueue(5);
    meshy::IoLoop::Get()->Start();

    meshy::PackageDataSink dataSink(&mainEventQueue);

//    meshy::HttpServer server;
//    server.Listen("127.0.0.1", DefaultPort);
//    server.OnConnection([=](meshy::HttpConnection* connection) {
//        connection->OnRequest([connection](const meshy::HttpRequest& request) {
//            LOG(KLOG_DEBUG) << "Request arrived" ;
//            LOG(KLOG_DEBUG) << request.GetMethod() ;
//            LOG(KLOG_DEBUG) << request.GetPath() ;
//            LOG(KLOG_DEBUG) << request.GetVersion() ;

//            meshy::HttpResponse response;
//            response.SetVersion("HTTP/1.1");
//            response.SetStatusCode(200);
//            response.SetStatusMessage("OK");
//            response.SetContent("Hello! Sink in Thread!");

//            connection->SendResponse(response);
//        });

//        connection->OnData([connection](const std::string& data) {
//            LOG(KLOG_DEBUG) << "Data arrived" ;
//            LOG(KLOG_DEBUG) << data ;
//        });
//    });

    meshy::TcpServer server;

    meshy::PackageDataSink* packageDataSink = &dataSink;
    server.Listen("127.0.0.1", DefaultPort);
    server.OnConnect([=](meshy::IStream* stream) {
        stream->OnData(packageDataSink->StreamDataHandler(stream));
    });

    SampleEventQueueLoop sampleQueue(&mainEventQueue);
    sampleQueue.Start();

    return 0;
}
