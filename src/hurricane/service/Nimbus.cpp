#include "hurricane/service/Nimbus.h"
#include "hurricane/util/Configuration.h"
#include "hurricane/topology/Topology.h"
#include "sample/wordcount/WordCountTopology.h"

#include <iostream>

namespace hurricane {
	namespace service {
		Nimbus::Nimbus(const hurricane::base::NetAddress& host) : 
				CommandServer(new hurricane::util::NetListener(host)),
				_nimbusHost(host) {
			OnConnection(std::bind(&Nimbus::OnConnect, this, std::placeholders::_1));
			OnCommand(hurricane::message::Command::Type::Join, this, &Nimbus::OnJoin);
		}

		Nimbus::Nimbus(const hurricane::util::Configuration& configuration) : 
				Nimbus(hurricane::base::NetAddress(
					configuration.GetProperty("nimbus.host"),
					configuration.GetIntegerProperty("nimbus.port"))) {
		}

		void Nimbus::OnConnect(SupervisorContext* context) {
		}

		void Nimbus::OnJoin(SupervisorContext* context, const hurricane::message::Command& command,
				hurricane::message::CommandServer<hurricane::message::BaseCommandServerContext>::Responser responser) {
			std::string joinerType = command.GetArgument(0).GetStringValue();

            std::cout << "Join node: " << joinerType << std::endl;
            
            SupervisorContext supervisorContext = SupervisorContext::FromVariants(command.GetArguments().cbegin() + 1);

            std::cout << "Supervisor name: " << supervisorContext.GetId() << std::endl;
            std::cout << "Spout count: " << supervisorContext.GetSpoutCount() << std::endl;
            std::cout << "Bolt count: " << supervisorContext.GetBoltCount() << std::endl;
            std::cout << "Task info count: " << supervisorContext.GetTaskInfos().size() << std::endl;
            std::cout << "Free spout count: " << supervisorContext.GetFreeSpouts().size() << std::endl;
            std::cout << "Free bolt count: " << supervisorContext.GetFreeBolts().size() << std::endl;
            std::cout << "Busy spout count: " << supervisorContext.GetBusySpouts().size() << std::endl;
            std::cout << "Busy bolt count: " << supervisorContext.GetBusyBolts().size() << std::endl;

            _supervisors.push_back(supervisorContext);

            if ( _supervisors.size() == 2 ) {
                topology::Topology* topology = GetTopology();
                SubmitTopology(topology);
            }

			hurricane::message::Response response(hurricane::message::Response::Status::Successful);
			response.AddArgument({ "nimbus" });

			responser(response);
		}

        void Nimbus::SubmitTopology(hurricane::topology::Topology* topology) {
            std::cout << "Submit topology: " << topology->GetName() << std::endl;

            const std::map<std::string, hurricane::spout::SpoutDeclarer>& spoutDeclarers = topology->GetSpoutDeclarers();
            const std::map<std::string, hurricane::bolt::BoltDeclarer>& boltDeclarers = topology->GetBoltDeclarers();

            for ( const auto& spoutPair : spoutDeclarers ) {
                hurricane::spout::SpoutDeclarer spoutDeclarer = spoutPair.second;
                std::cout << "Spout " << spoutDeclarer.GetTaskName() << std::endl;
                std::cout << "ParallismHint: " << spoutDeclarer.GetParallismHint() << std::endl;
                

                // Allocate spout tasks
                for ( const SupervisorContext& supervisorContext : _supervisors ) {
                    if ( supervisorContext.GetFreeSpouts().size() ) {

                    }
                }
            }

            for ( const auto& boltPair : boltDeclarers ) {
                hurricane::bolt::BoltDeclarer boltDeclarer = boltPair.second;
                std::cout << "Bolt " << boltDeclarer.GetTaskName() << std::endl;
                std::cout << "Source: " << boltDeclarer.GetSourceTaskName() << std::endl;
                std::cout << "ParallismHint: " << boltDeclarer.GetParallismHint() << std::endl;

                // Allocate bolt tasks
                for ( const SupervisorContext& supervisorContext : _supervisors ) {
                    if ( supervisorContext.GetFreeBolts().size() ) {

                    }
                }
            }
        }
	}
}