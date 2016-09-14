#include "hurricane/service/Nimbus.h"
#include "hurricane/util/Configuration.h"

#include <iostream>
#include <string>

using namespace std;

void StartNimbus(const std::string& configFileName);

int main(int argc, char *argv[])
{
	if ( argc < 2 ) {
		return EXIT_FAILURE;
	}

	StartNimbus(argv[1]);

    return EXIT_SUCCESS;
}

void StartNimbus(const std::string& configFileName) {
	hurricane::util::Configuration supervisorConfiguration;
	supervisorConfiguration.Parse(configFileName);

	hurricane::service::Nimbus nimbus(hurricane::base::NetAddress("127.0.0.1", 6009));
	nimbus.StartListen();
}