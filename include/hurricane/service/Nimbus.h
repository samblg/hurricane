#pragma once

#include "hurricane/message/CommandServer.h"
#include "hurricane/util/NetListener.h"
#include "hurricane/base/NetAddress.h"
#include "hurricane/service/SupervisorContext.h"

namespace hurricane {
	namespace util {
		class Configuration;
	}

    namespace topology {
        class Topology;
    }

	namespace service {
		class Nimbus : public hurricane::message::CommandServer<SupervisorContext> {
		public:
			Nimbus(const hurricane::base::NetAddress& host);
			Nimbus(const hurricane::util::Configuration& configuration);

			void OnConnect(SupervisorContext* context);
			void OnJoin(SupervisorContext* context, const hurricane::message::Command& command,
				hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser);
            void SubmitTopology(hurricane::topology::Topology* topology);

		private:
			hurricane::base::NetAddress _nimbusHost;
            std::vector<SupervisorContext> _supervisors;
		};
	}
}