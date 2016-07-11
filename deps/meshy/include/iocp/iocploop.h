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

#pragma once

#include "loop.h"
#include "DataSink.h"
#include "IOCPConnection.h"
#include "IOCPServer.h"

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <memory>
#include <thread>
#include <map>
#include <mutex>
#include <condition_variable>

namespace meshy {

	class EventQueue;

	class IOCPLoop : public Loop {
	public:
		static IOCPLoop* Get();
		void AddServer(IOCPServer* server);

	private:
		IOCPLoop();
		virtual ~IOCPLoop();
		void SetDataSink(DataSink* dataSink);

	protected:
		virtual void _Run() override;

	private:
		void _Initialize();

		void _IOCPThread();

		void _IOCPConnectionThread(IOCPServer* server);

		void _WorkThread(int32_t listenfd, HANDLE completionPort);

		void _Enqueue(IOCPStreamPtr stream, const char* buf, int64_t nread);

	private:
		std::string                _host;
		int                        _port;
		DataSink*                  _dataSink;
		bool                       _shutdown;
		SYSTEM_INFO                _systemInfo;

		ConcurrentQueue<IOCPServer*>              _serverQueue;  // For adding servers
		std::map <NativeSocket, IOCPServer*>      _servers;      // For destructing servers
		std::map <NativeSocket, WSAConnectionPtr> _streams;      // Storing connections
	};
}
