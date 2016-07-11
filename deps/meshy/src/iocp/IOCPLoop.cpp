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

#include "iocp/IOCPLoop.h"
#include "iocp/IOCPConnection.h"
#include "IoLoop.h"
#include "utils/logger.h"

#include "eventqueue.h"

#include <thread>
#include <vector>
#include <iostream>


namespace meshy {

	IOCPLoop* IOCPLoop::Get()
	{
		static IOCPLoop IOCPLoop;
		return &IOCPLoop;
	}

	IOCPLoop::IOCPLoop() : _dataSink(nullptr), _shutdown(false), _serverQueue(), _servers(), _streams()
	{
		GetSystemInfo(&_systemInfo);
	}

	IOCPLoop::~IOCPLoop()
	{
	}

	void IOCPLoop::AddServer(IOCPServer* server)
	{
		_serverQueue.Push(server);
	}

	void IOCPLoop::_Run() {
		auto iocpFunc = std::bind(&IOCPLoop::_IOCPThread, this);
		std::thread iocpThread(iocpFunc);
		iocpThread.detach();
	}

	void IOCPLoop::_Initialize()
	{

	}

	void IOCPLoop::_IOCPThread()
	{
		TRACE_DEBUG("Server ready, wait for new connection ...");
		HANDLE completionPort = IOCP::GetCompletionPort();

		while (!_shutdown) {
			IOCPServer* server = nullptr;
			_serverQueue.Pop(server, true); // blocked process
			NativeSocket listenfd = server->GetNativeSocket();
            server->SetCompletionPort(completionPort);

			if (nullptr != server)
			{
				// Create the threads for the server according to the number of processors
				for (DWORD i = 0; i < (_systemInfo.dwNumberOfProcessors * 2); ++i) {
					// Create server worker thread and pass the completion port to the thread.
					auto workerFunc = std::bind(&IOCPLoop::_WorkThread, this, listenfd, completionPort);
					std::thread workerThread(workerFunc);
					workerThread.detach();
				}

				// Handle accept
				auto connectionFunc = std::bind(&IOCPLoop::_IOCPConnectionThread, this, server);
				std::thread acceptThread(connectionFunc);
				acceptThread.detach();
			}
		}
	}

	void IOCPLoop::_IOCPConnectionThread(IOCPServer* server)
	{
		while (!_shutdown)
		{
            std::cout << "to accept" << std::endl;
			WSAConnectionPtr connection = server->Accept(server->GetNativeSocket());
			if (connection != nullptr) {
				_streams[connection->GetNativeSocket()] = connection;
			}
		}
	}

	void IOCPLoop::_WorkThread(int32_t listenfd, HANDLE completionPort)
	{
		DWORD bytesReceived;
		LPOVERLAPPED lpOverlapped;
		PULONG key = 0;
		IOCP::OperationData* perIOData = nullptr;
		DWORD flags = 0;
		BOOL result = false;

		while (!_shutdown) {
			// Check the errors of socket
			result = GetQueuedCompletionStatus(completionPort, &bytesReceived, (PULONG_PTR)&key, (LPOVERLAPPED*)&lpOverlapped, INFINITE);
			if (0 == result) {
				TRACE_ERROR("GetQueuedCompletionStatus Error: " + GetLastError());
				if (nullptr != lpOverlapped) {
					// TODO: 显示删除
				}
				continue;
			}
			
			perIOData = (IOCP::OperationData*)CONTAINING_RECORD(lpOverlapped, IOCP::OperationData, overlapped);

			if ( 0 == bytesReceived) {
				perIOData->stream = nullptr;
				delete perIOData;
				continue;
			}

			// Begin to process data received from client.
			//ByteArray data(perIOData->databuff.buf, bytesReceived);
			//if ( _dataSink ) {
			//	_dataSink->OnDataIndication(perIOData->stream, perIOData->databuff.buf, bytesReceived);
			//}

			IOCPStreamPtr stream = _streams[perIOData->stream->GetNativeSocket()];
			this->_Enqueue(stream, perIOData->databuff.buf, bytesReceived);
			IOCP::ResetOperationData(perIOData);

			DWORD RecvBytes;
			// Receive normal data.
			DWORD Flags = 0;

			WSARecv(stream->GetNativeSocket(),
				&(stream->GetOperationData()->databuff), 1, &RecvBytes, &Flags,
				&(stream->GetOperationData()->overlapped), NULL);
		}
	}

	void IOCPLoop::_Enqueue(IOCPStreamPtr stream, const char* buf, int64_t nread)
	{
		TRACE_DEBUG("_Enqueue");
		if (stream->GetDataSink()) {
			TRACE_DEBUG("_Enqueue, datasink registered.");
			stream->GetDataSink()->OnDataIndication(stream.get(), buf, nread);
		}
	}

	void IOCPLoop::SetDataSink(DataSink* dataSink) {
		_dataSink = dataSink;
	}
}
