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

#include "hurricane/service/President.h"
#include "hurricane/multilang/java/VirtualMachine.h"
#include "hurricane/multilang/java/String.h"
#include "hurricane/topology/TopologyLoader.h"
#include "hurricane/util/Configuration.h"
#include "hurricane/base/Constants.h"

#include <iostream>
#include <string>
#include <csignal>
#include <thread>

using namespace std;

void StartPresident(const std::string& configFileName);
static void StartJVM(const hurricane::util::Configuration& configuration);
static void ProcessSignal(int signalNumber);

int main(int argc, char* argv[])
{
    if ( argc < 2 ) {
        return EXIT_FAILURE;
    }

    StartPresident(argv[1]);

    return EXIT_SUCCESS;
}

void StartPresident(const std::string& configFileName) {
    hurricane::util::Configuration presidentConfigratuion;
    presidentConfigratuion.Parse(configFileName);

    if ( presidentConfigratuion.GetBooleanProperty(hurricane::CONF_KEY_ENABLE_JAVA) ) {
        StartJVM(presidentConfigratuion);
    }

//    signal(SIGINT, ProcessSignal);

    hurricane::service::President president(presidentConfigratuion);
    president.StartListen();
}

static void StartJVM(const hurricane::util::Configuration& configuration)
{
    using hurricane::java::VirtualMachine;
    using hurricane::java::String;

    VirtualMachine* vm = new VirtualMachine;
    VirtualMachine::SetDefault(vm);
    vm->SetVersion(JNI_VERSION_1_8);
    vm->AddClassPath(configuration.GetProperty(hurricane::CONF_KEY_JAVA_CLASSPATH));
    LOG(LOG_DEBUG) << "Java classpath: " <<
                      configuration.GetProperty(hurricane::CONF_KEY_JAVA_CLASSPATH);

    vm->Start();

    LOG(LOG_DEBUG) << "Current thread id: " <<std::this_thread::get_id();

    signal(SIGINT, ProcessSignal);
}

static void ProcessSignal(int signalNumber) {
    using hurricane::java::VirtualMachine;

    LOG(LOG_ERROR) << "Receive signal number: " << signalNumber;
    LOG(LOG_ERROR) << "Exit";
    exit(0);
}
