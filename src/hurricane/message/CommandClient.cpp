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

#include "hurricane/message/CommandClient.h"
#include "hurricane/message/Command.h"
#include "hurricane/util/NetConnector.h"
#include "hurricane/base/ByteArray.h"
#include "logging/Logging.h"

#include <iostream>

namespace hurricane {
    namespace message {
        CommandClient::~CommandClient() {
            if ( _connector ) {
                delete _connector;
                _connector = nullptr;
            }
        }

        void CommandClient::Connect(CommandClient::ConnectCallback callback)
        {
            this->_connector->Connect([callback](const util::SocketError& error) {
                if ( error.GetType() != util::SocketError::Type::NoError ) {
                    callback(CommandError(CommandError::Type::ConnectError,
                            error.what()));
                    return;
                }

                callback(CommandError());
            });
        }

        void CommandClient::SendCommand(const Command& command, SendCommandCallback callback) {
            try {
                hurricane::base::ByteArray commandBytes = command.Serialize();

                _connector->SendAndReceive(commandBytes.data(), static_cast<int32_t>(commandBytes.size()), [callback]
                        (char* resultBuffer, int32_t readSize, const util::SocketError& error) {
                    hurricane::message::Response response;

                    if ( error.GetType() != util::SocketError::Type::NoError ) {
                        callback(response, CommandError(CommandError::Type::SendError,
                                error.what()));
                        return;
                    }

                    hurricane::base::ByteArray commandBytes(resultBuffer, readSize);
                    response.Deserialize(commandBytes);
                    callback(response, CommandError());
                });
            }
            catch (const char* message) {
                LOG(LOG_ERROR) << message;
            }
        }
    }
}
