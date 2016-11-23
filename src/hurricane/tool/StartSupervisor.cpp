#include "hurricane/service/Supervisor.h"
#include "hurricane/util/Configuration.h"

#include <iostream>
#include <string>

using namespace std;

void StartSupervisor(const std::string& configFileName);

int main(int argc, char *argv[])
{
	if ( argc < 2 ) {
		return EXIT_FAILURE;
	}

	StartSupervisor(argv[1]);

    return EXIT_SUCCESS;
}

void StartSupervisor(const std::string& configFileName) {
	hurricane::util::Configuration supervisorConfiguration;
	supervisorConfiguration.Parse(configFileName);

    std::cout << supervisorConfiguration.GetProperty("nimbus.host") << std::endl;
    std::cout << supervisorConfiguration.GetIntegerProperty("nimbus.port") << std::endl;
    std::cout << supervisorConfiguration.GetProperty("supervisor.host") << std::endl;
    std::cout << supervisorConfiguration.GetIntegerProperty("supervisor.port") << std::endl;

    hurricane::service::Supervisor supervisor(supervisorConfiguration);
	supervisor.JoinNimbus([&supervisor](const hurricane::message::Response& response) {
		if ( response.GetStatus() != hurricane::message::Response::Status::Successful ) {
			std::cerr << "Can't join nimbus." << std::endl;
			std::cerr << "Exit with failure." << std::endl;

			exit(EXIT_FAILURE);
		}
        else {
            std::cout << "Join successfully" << std::endl;
        }

        supervisor.StartListen();
	});
}
