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

#include <Windows.h>
#include <iostream>
#include <thread>
#include <functional>
#include <map>
#include <cstdint>

class Message {
public:
	Message(int32_t type) : _type(type) {
	}

	int32_t GetType() const {
		return _type;
	}

	void SetType(int32_t type) {
		_type = type;
	}

private:
	int32_t _type;
};

class MessageLoop {
public:
	typedef std::function<void(Message*)> MessageHandler;

	MessageLoop() {
		_threadId = GetCurrentThreadId();
	}

	MessageLoop(const MessageLoop&) = delete;
	const MessageLoop& operator=(const MessageLoop&) = delete;

	template <class ObjectType, class MethodType>
	void MessageMap(int messageType, ObjectType* self, MethodType method) {
		MessageMap(messageType, std::bind(method, self, std::placeholders::_1));
	}

	void MessageMap(int messageType, MessageHandler handler) {
		_messageHandlers.insert({ messageType, handler });
	}

	void Run() {
		MSG msg;

		while ( GetMessage(&msg, 0, 0, 0) ) {
			std::cerr << "Recived Message" << std::endl;
			auto handler = _messageHandlers.find(msg.message);

			if ( handler != _messageHandlers.end() ) {
				handler->second((Message*)(msg.wParam));
			}

			DispatchMessage(&msg);
		}
	}

	void PostMessage(Message* message) {
		PostThreadMessage(_threadId, message->GetType(), WPARAM(message), 0);
	}

private:
	std::map<int, MessageHandler> _messageHandlers;
	uint64_t _threadId;
};

class MessageLoopManager {
public:
	static MessageLoopManager& GetInstance() {
		static MessageLoopManager manager;

		return manager;
	}

	MessageLoopManager(const MessageLoopManager&) = delete;
	const MessageLoopManager& operator=(const MessageLoopManager&) = delete;

	void Register(const std::string& name, MessageLoop* loop) {
		_messageLoops.insert({ name, std::shared_ptr<MessageLoop>(loop) });
	}

	void PostMessage(const std::string& name, Message* message) {
		auto messageLoopPair = _messageLoops.find(name);
		if ( messageLoopPair != _messageLoops.end() ) {
			messageLoopPair->second->PostMessage(message);
		}
	}

private:
	MessageLoopManager() {}

	std::map<std::string, std::shared_ptr<MessageLoop>> _messageLoops;
};