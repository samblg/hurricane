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

#ifdef OS_WIN32
#include "win32/net_win32.h"
#elif defined(OS_BSD)
#include "bsd/net_bsd.h"
#elif defined(OS_LINUX)
#include "linux/net_linux.h"
#endif

#include "bytearray.h"

#include <memory>
#include <string>
#include <iostream>
#include <functional>

#if defined(OS_BSD) || defined(OS_LINUX)
#include <unistd.h>
#include <netinet/in.h>
#endif


namespace meshy {
	class Socket {
	public:
		Socket() : _nativeSocket(0) { }

		Socket(NativeSocket nativeSocket) : _nativeSocket(nativeSocket) { }

		virtual ~Socket() {
#ifdef OS_WIN32
			closesocket(_nativeSocket);
#else
			close(_nativeSocket);
#endif
		}

		NativeSocket GetNativeSocket() const {
			return _nativeSocket;
		}

		void SetNativeSocket(NativeSocket nativeSocket) {
			_nativeSocket = nativeSocket;
		}

	private:
		NativeSocket _nativeSocket;
	};

	class DataSink;

	class IStream {
	public:
        typedef std::function<void(const char* buf, int64_t size)> DataIndicationHandler;

		virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) = 0;
		virtual int32_t Send(const ByteArray& byteArray) = 0;

        virtual void OnDataIndication(DataIndicationHandler handler) = 0;
        virtual DataIndicationHandler GetDataIndication() = 0;
	};

    class IConnectable {
    public:
        virtual void Connect(const std::string& host, int32_t port) = 0;
    };

	template<class ConnectionType>
	class BasicServer : public Socket {
	public:
        typedef std::function<void(IStream* stream)> ConnectIndicationHandler;
        typedef std::function<void(IStream* stream)> DisconnectIndicationHandler;

		BasicServer() { }

		virtual int32_t Listen(const std::string& host, int32_t port, int backlog) = 0;
        virtual void OnConnectIndication(ConnectIndicationHandler handler) = 0;
        virtual void OnDisconnectIndication(DisconnectIndicationHandler handler) = 0;

        virtual ConnectionType Accept(int32_t listenfd) = 0;
	};

    class BasicStream : public IStream, public Socket {
    public:
        BasicStream() = default;
        BasicStream(NativeSocket nativeSocket) : Socket(nativeSocket) {}

        BasicStream(const BasicStream& stream) = delete;

        virtual void SetDataSink(DataSink* dataSink) {
            _dataSink = dataSink;
        }

        virtual DataSink* GetDataSink() {
            return _dataSink;
        }

        virtual const DataSink* GetDataSink() const {
            return _dataSink;
        }

    private:
        DataSink* _dataSink;
    };
}
