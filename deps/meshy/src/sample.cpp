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

#include "net.h"
#include "eventqueue.h"
#include "eventqueueloop.h"
#include "IoLoop.h"
#include "PackageDataSink.h"
#include "utils/logger.h"

#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include "PackageDataSink.h"
#include "net.h"
#include "Meshy.h"
#include "rest/HttpServer.h"

using namespace std;

const int DefaultPort = 9000;
class SampleEventQueueLoop : public meshy::EventQueueLoop {
public:
	SampleEventQueueLoop(meshy::EventQueue* eventQueue) :
		EventQueueLoop(eventQueue) {}

protected:
	virtual void OnEvent(std::shared_ptr<meshy::BaseEvent> event) override {
	}
};

int main() {
	meshy::EventQueue mainEventQueue(5);
    meshy::IoLoop::Get()->Start();

	meshy::PackageDataSink dataSink(&mainEventQueue);

    meshy::HttpServer server;
    server.Listen("127.0.0.1", DefaultPort);
    server.OnConnection([=](meshy::HttpConnection* connection) {
        connection->OnRequest([connection](const meshy::HttpRequest& request) {
            std::cout << "Request arrived" << std::endl;
            std::cout << request.GetMethod() << std::endl;
            std::cout << request.GetPath() << std::endl;
            std::cout << request.GetVersion() << std::endl;

            meshy::HttpResponse response;
            response.SetVersion("HTTP/1.1");
            response.SetStatusCode(200);
            response.SetStatusMessage("OK");
            response.SetContent("Hello! Sink in Thread!");

            connection->SendResponse(response);
        });

        connection->OnData([connection](const std::string& data) {
            std::cout << "Data arrived" << std::endl;
            std::cout << data << std::endl;
        });
    });

    //meshy::TcpServer server;

    //meshy::PackageDataSink* packageDataSink = &dataSink;
	//server.Listen("127.0.0.1", DefaultPort);
	//server.OnConnectIndication([=](meshy::IStream* stream) {
	//	std::cout << stream << std::endl;
    //    stream->OnDataIndication([packageDataSink, stream](const char* buf, int64_t size) mutable {
    //        packageDataSink->OnDataIndication(stream, buf, size);
    //    });
	//});

	SampleEventQueueLoop sampleQueue(&mainEventQueue);
	sampleQueue.Start();

	return 0;
}
