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

#include "iocp/IOCPStream.h"
#include "utils/logger.h"

namespace meshy {
    int32_t IOCPStream::Receive(char* buffer, int32_t bufferSize, int32_t& readSize)
    {
		// IOCP already handled data reading out, leave this function blank.
		return 0;
    }

    int32_t IOCPStream::Send(const ByteArray &byteArray)
    {
        return send(GetNativeSocket(), byteArray.data(), byteArray.size(), 0);
    }

	HANDLE IOCP::GetCompletionPort() {
		HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
		if (nullptr == completionPort) {
			TRACE_ERROR("CreateIoCompletionPort failed. Error:" + GetLastError());
			throw std::exception("CreateIoCompletionPort failed.");
		}

		return completionPort;
	}

	IOCP::OperationDataPtr IOCP::CreateOperationData(IOCPStreamPtr stream, HANDLE completionPort) {
		// Begin to process I/O using overlapped I/O
		// Post one or many WSARecv or WSASend requests to the new socket
		// Worker thread will serve the I/O request after the I/O request finished.
		IOCP::OperationData* perIOData = new IOCP::OperationData();
		perIOData->stream = stream.get();

		// Relate the socket to CompletionPort
		CreateIoCompletionPort((HANDLE)(perIOData->stream->GetNativeSocket()),
            completionPort, (ULONG_PTR)perIOData->stream->GetNativeSocket(), 0);

		ZeroMemory(&(perIOData->overlapped), sizeof(OVERLAPPED));
		perIOData->databuff.len = BUFSIZ;
		perIOData->databuff.buf = perIOData->buffer;
		perIOData->operationType = IOCP::OperationType::Read;

		return IOCP::OperationDataPtr(perIOData);
	}

	void IOCP::ResetOperationData(OperationData* perIOData)
	{
		// Create single I/O operation data for next overlapped invoking.
		ZeroMemory(&(perIOData->overlapped), sizeof(OVERLAPPED));
		perIOData->databuff.len = BUFSIZ;
		perIOData->databuff.buf = perIOData->buffer;
		perIOData->operationType = 0;
	}
}
