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
#include "hurricane/Hurricane.h"

#include "hurricane/message/MessageLoop.h"
#include "hurricane/message/Message.h"

#include <Windows.h>

// Because Windows defined the PostMessage Macro
// So we must undefine it to disable compiler convert PostMessage to PostMessageA/PostMessageW
#ifdef PostMessage
#undef PostMessage
#endif

namespace hurricane {
namespace message {
	MessageLoop::MessageLoop() {
		_threadId = GetCurrentThreadId();
	}

	void MessageLoop::Run() {
		MSG msg;

		while ( GetMessage(&msg, 0, 0, 0) ) {
			auto handler = _messageHandlers.find(msg.message);

			if ( handler != _messageHandlers.end() ) {
				handler->second((Message*)(msg.wParam));
			}

			DispatchMessage(&msg);

			if ( msg.message == Message::Type::Stop ) {
				break;
			}
		}
	}

	void MessageLoop::PostMessage(Message* message) {
		PostThreadMessage(_threadId, message->GetType(), WPARAM(message), 0);
	}

	void MessageLoop::Stop() {
		PostMessage(new Message(Message::Type::Stop));
	}
}
}