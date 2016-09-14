#include "hurricane/service/Supervisor.h"
#include "hurricane/message/CommandClient.h"
#include "hurricane/util/NetConnector.h"
#include "hurricane/util/Configuration.h"

namespace hurricane {
	namespace service {
		Supervisor::Supervisor(const hurricane::util::Configuration& configuration) :
            CommandServer(new hurricane::util::NetListener(hurricane::base::NetAddress(
                    configuration.GetProperty("supervisor.host"),
                    configuration.GetIntegerProperty("supervisor.port")))) {
            _supervisorConfiguration.reset(new hurricane::util::Configuration(configuration));

            hurricane::base::NetAddress nimbusAddress(configuration.GetProperty("nimbus.host"),
                configuration.GetIntegerProperty("nimbus.port"));
            _nimbusConnector = new hurricane::util::NetConnector(nimbusAddress);
            _nimbusClient = new hurricane::message::CommandClient(_nimbusConnector);
            
            _name = configuration.GetProperty("supervisor.name");
            InitSelfContext();
            OnConnection(std::bind(&Supervisor::OnConnect, this, std::placeholders::_1));
		}

		void Supervisor::OnConnect(SupervisorContext* context) {
		}

		void Supervisor::JoinNimbus(JoinNimbusCallback callback) {
			hurricane::message::CommandClient* commandClient = _nimbusClient;

			_nimbusConnector->Connect([commandClient, callback, this]() {
				hurricane::message::Command command(hurricane::message::Command::Type::Join);
				command.AddArgument({ "supervisor" });
                std::vector<hurricane::base::Variant> context = _selfContext->ToVariants();
                command.AddArguments(context);

				commandClient->SendCommand(command, [callback](const hurricane::message::Response& response) {
					callback(response);
				});
			});
        }

        void Supervisor::InitSelfContext() {
            this->_selfContext.reset(new SupervisorContext);
            _selfContext->SetId(_name);
            _selfContext->SetSpoutCount(_supervisorConfiguration->GetIntegerProperty("supervisor.spout.num"));
            _selfContext->SetBoltCount(_supervisorConfiguration->GetIntegerProperty("supervisor.bolt.num"));
            _selfContext->SetTaskInfos(std::vector<hurricane::task::TaskInfo>(_selfContext->GetSpoutCount() + _selfContext->GetBoltCount()));

            std::set<int> freeSpouts;
            for ( int spoutIndex = 0; spoutIndex != _selfContext->GetSpoutCount(); ++ spoutIndex ) {
                freeSpouts.insert(spoutIndex);
            }
            _selfContext->SetFreeSpouts(freeSpouts);

            std::set<int> freeBolts;
            for ( int boltIndex = 0; boltIndex != _selfContext->GetBoltCount(); ++ boltIndex ) {
                freeBolts.insert(boltIndex);
            }
            _selfContext->SetFreeBolts(freeBolts);
        }
	}
}