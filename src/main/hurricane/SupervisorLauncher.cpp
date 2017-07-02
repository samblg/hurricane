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

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>

#include "hurricane/base/NetAddress.h"
#include "hurricane/base/ByteArray.h"
#include "hurricane/base/DataPackage.h"
#include "hurricane/base/Value.h"
#include "hurricane/base/Variant.h"
#include "hurricane/message/SupervisorCommander.h"
#include "hurricane/message/CommandDispatcher.h"
#include "hurricane/topology/ITopology.h"
#include "hurricane/base/NetListener.h"

using hurricane::base::NetAddress;
using hurricane::base::ByteArray;
using hurricane::base::DataPackage;
using hurricane::base::Variant;
using hurricane::base::Variants;
using hurricane::base::Value;
using hurricane::base::Values;
using hurricane::message::Command;
using hurricane::message::CommandDispatcher;
using hurricane::message::SupervisorCommander;
using hurricane::topology::ITopology;

hurricane::topology::ITopology* GetTopology();

const NetAddress NIMBUS_ADDRESS{ "127.0.0.1", 6000 };
const std::map<std::string, NetAddress> SUPERVISOR_ADDRESSES{
    { "s1",{ "127.0.0.1", 7001 } }
};

void AliveThreadMain(const std::string& name) {
    SupervisorCommander commander(NIMBUS_ADDRESS, name);
    commander.Join();

    while ( 1 ) {
        commander.Alive();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(int argc, char** argv) {
    if ( argc < 2 ) {
        std::cerr << "Wrong arguments" << std::endl;
        exit(-1);
    }

    std::string supervisorName(argv[1]);
    std::cerr << "Supervisor " << supervisorName << " started" << std::endl;

    ITopology* topology = GetTopology();

    std::thread aliveThread(AliveThreadMain, supervisorName);
    aliveThread.detach();

    NetListener netListener(SUPERVISOR_ADDRESSES.at(supervisorName));
    CommandDispatcher dispatcher;
    dispatcher
        .OnCommand(Command::Type::StartBolt,
            [&](hurricane::base::Variants args, std::shared_ptr<meshy::TcpConnection> src) -> void {
        Command command(Command::Type::Response, {
            std::string(supervisorName)
        });

        std::string taskName = args[0].GetStringValue();
        int executorIndex = args[1].GetIntValue();

        std::cout << "Start Bolt" << std::endl;
        std::cout << "Bolt name: " << taskName << std::endl;
        std::cout << "Executor index: " << executorIndex << std::endl;

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(*(reinterpret_cast<meshy::ByteArray*>(&commandBytes)));
    })
        .OnCommand(Command::Type::StartSpout,
            [&](hurricane::base::Variants args, std::shared_ptr<meshy::TcpConnection> src) -> void {
        Command command(Command::Type::Response, {
            std::string(supervisorName)
        });

        std::string taskName = args[0].GetStringValue();
        int executorIndex = args[1].GetIntValue();

        std::cout << "Start Spout" << std::endl;
        std::cout << "Spout name: " << taskName  << std::endl;
        std::cout << "Executor index: " << executorIndex  << std::endl;

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(*(reinterpret_cast<meshy::ByteArray*>(&commandBytes)));
    })
        .OnCommand(Command::Type::Data,
            [&](Variants args, std::shared_ptr<meshy::TcpConnection> src) -> void {
        //TODO: Dispatch tuple
        std::string srcSupervisorName = args[0].GetStringValue();
        args.erase(args.begin());
        int32_t taskIndex = args[0].GetIntValue();
        args.erase(args.begin());

        Values values;
        for ( auto& arg : args ) {
            values.push_back(Value::FromVariant(arg));
        }

        Command command(Command::Type::Response, {
            std::string(supervisorName)
        });

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(*(reinterpret_cast<meshy::ByteArray*>(&commandBytes)));
    });


    netListener.OnData([&](meshy::TcpStream* connection,
        const char* buffer, int32_t size) -> void {
        ByteArray receivedData(buffer, size);
        DataPackage receivedPackage;
        receivedPackage.Deserialize(receivedData);

        Command command(receivedPackage);

        dispatcher.Dispatch(command);
    });

    netListener.StartListen();

    return 0;
}