#pragma once

#include "hurricane/message/CommandServer.h"
#include "hurricane/util/NetListener.h"
#include "hurricane/base/NetAddress.h"
#include "hurricane/service/SupervisorContext.h"

#include <functional>
#include <memory>

namespace hurricane {
	namespace util {
		class NetConnector;
		class Configuration;
	}

	namespace message {
		class CommandClient;
	}

	namespace service {
		class Supervisor : public hurricane::message::CommandServer<SupervisorContext> {
		public:
			typedef std::function<void(const hurricane::message::Response& response)> JoinNimbusCallback;
            
			Supervisor(const hurricane::util::Configuration& configuration);

			void OnConnect(SupervisorContext* context);

			void JoinNimbus(JoinNimbusCallback callback);

        private:
            void InitSelfContext();

		private:
            std::string _name;
            std::shared_ptr<hurricane::util::Configuration> _supervisorConfiguration;
			hurricane::util::NetConnector* _nimbusConnector;
			hurricane::message::CommandClient* _nimbusClient;
            std::shared_ptr<hurricane::service::SupervisorContext> _selfContext;
		};
	}
}