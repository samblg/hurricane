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

#pragma

#include "hurricane/message/Command.h"
#include <map>
#include <functional>

namespace meshy {
    class TcpConnection;
}

namespace hurricane {
	namespace message {
		class CommandDispatcher {
		public:
			typedef std::function<
				void(hurricane::base::Variants args, std::shared_ptr<meshy::TcpConnection> src)
			> Handler;

            CommandDispatcher& OnCommand(Command::Type::Values type, Handler handler) {
                _handlers[type] = handler;

                return *this;
            }

			void Dispatch(const Command& command);

		private:
			std::map<Command::Type::Values, Handler> _handlers;
		};
	}
}