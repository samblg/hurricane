#pragma once

#include <functional>

namespace hurricane {
	namespace util {
		class NetConnector;
	}

	namespace message {
		class Command;
		class Response;

		class CommandClient {
		public:
			typedef std::function<void(const Response& response)> SendCommandCallback;

			CommandClient(hurricane::util::NetConnector* connector) : _connector(connector) {
			}

			~CommandClient();

			void SendCommand(const Command& command, SendCommandCallback callback);
            hurricane::util::NetConnector* GetConnector() {
                return _connector;
            }

            const hurricane::util::NetConnector* GetConnector() const {
                return _connector;
            }

		private:
			hurricane::util::NetConnector* _connector;
		};
	}
}
