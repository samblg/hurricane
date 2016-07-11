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

#include "net.h"

namespace meshy {
	class IOCPStream;
	typedef std::shared_ptr<IOCPStream> IOCPStreamPtr;

	// TODO: move utility to other places
	class IOCP {
	public:
		enum {
			DataBuffSize = BUFSIZ
		};

		class OperationType {
		public:
			enum {
				Read,
				Write
			};
		};

		struct OperationData
		{
			OVERLAPPED overlapped;
			WSABUF databuff;
			char buffer[DataBuffSize];
			int32_t operationType;
			IOCPStream* stream;
		};
		typedef std::shared_ptr<OperationData> OperationDataPtr;

		static HANDLE GetCompletionPort();
		static OperationDataPtr CreateOperationData(IOCPStreamPtr stream, HANDLE completionPort);
		static void ResetOperationData(OperationData* perIOData);
	};
    class IOCPStream : public BasicStream {
    public:
        IOCPStream(NativeSocket clientSocket, NativeSocketAddress clientAddress) :
            BasicStream(clientSocket), _clientAddress(clientAddress) {
        }

        IOCPStream(const IOCPStream& iocpStream) = delete;

        virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) override;
        virtual int32_t Send(const ByteArray& byteArray) override;

        void SetOperationData(IOCP::OperationDataPtr operationData) {
            _operationData = operationData;
        }

        IOCP::OperationDataPtr GetOperationData() {
            return _operationData;
        }

        IOCP::OperationDataPtr GetOperationData() const {
            return _operationData;
        }

    private:
        IOCP::OperationDataPtr _operationData;
        NativeSocketAddress _clientAddress;
    };

}