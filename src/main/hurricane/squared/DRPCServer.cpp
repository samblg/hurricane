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

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "hurricane/base/NetAddress.h"
#include "hurricane/base/ByteArray.h"
#include "hurricane/base/DataPackage.h"
#include "temp/NetListener.h"
#include "hurricane/message/CommandDispatcher.h"
#include "hurricane/message/NimbusCommander.h"
#include "hurricane/base/Node.h"
#include "temp/WordCountTopology.h"
#include "DRPCStream.h"

using hurricane::base::NetAddress;
using hurricane::base::ByteArray;
using hurricane::base::DataPackage;
using hurricane::message::Command;
using hurricane::message::CommandDispatcher;
using hurricane::message::NimbusCommander;
using hurricane::base::Node;
using hurricane::topology::ITopology;
using hurricane::spout::ISpout;
using hurricane::bolt::IBolt;
using hurricane::trident::DRPCStream;

const NetAddress SERVER_ADDRESS{ "127.0.0.1", 3772 };

int main() {
    NetListener netListener(SERVER_ADDRESS);
    CommandDispatcher dispatcher;
    std::map<std::string, DRPCStream*> streams;
    
    dispatcher
        .OnCommand(Command::Type::Join,
            [&](hurricane::base::Variants args, std::shared_ptr<TcpConnection> src) -> void {
        std::string serviceName = args[0].GetStringValue();
        args.pop_front();
        std::string serviceArgs = args;

        DRPCStream* stream = streams[serviceName];
        std::string result = stream->WaitFormResult(args);

        Command command(Command::Type::Response, {
            result
        });

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(commandBytes.data(), commandBytes.size());
    });

    netListener.OnData([&](std::shared_ptr<TcpConnection> connection,
        const char* buffer, int32_t size) -> void {
        ByteArray receivedData(buffer, size);
        DataPackage receivedPackage;
        receivedPackage.Deserialize(receivedData);

        Command command(receivedPackage);
        command.SetSrc(connection);

        dispatcher.Dispatch(command);
    });

    netListener.StartListen();
}