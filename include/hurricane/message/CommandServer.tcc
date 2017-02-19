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

namespace hurricane {
namespace message {

template <class CommandServerContext>
CommandServer<CommandServerContext>::CommandServer(hurricane::util::NetListener* listener) :
    _listener(listener) {
}

template <class CommandServerContext>
CommandServer<CommandServerContext>::~CommandServer() {
    if ( _listener ) {
        delete _listener;
        _listener = nullptr;
    }
}

template <class CommandServerContext>
void CommandServer<CommandServerContext>::StartListen() {
    std::map<int32_t, CommandHandler>& commandHandlers = _commandHandlers;
    ConnectHandler connectHandler = _connectHandler;

    _listener->OnConnection([this, connectHandler, commandHandlers](std::shared_ptr<hurricane::util::TcpConnection> connection) {
        CommandServerContext* context = new CommandServerContext;
        hurricane::util::TcpConnection* rawConnection = connection.get();

        _connectHandler(context);

        connection->OnData([this, commandHandlers, context, rawConnection]
                           (const char* buffer, int32_t size, const util::SocketError& error) {
            if ( error.GetType() != util::SocketError::Type::NoError ) {
                LOG(LOG_ERROR) << error.what();
                return;
            }

            hurricane::base::ByteArray commandBytes(buffer, size);
            Command command;
            command.Deserialize(commandBytes);

            int32_t commandType = command.GetType();
            try {
                CommandHandler handler = commandHandlers.at(commandType);
                Responsor Responsor = std::bind(&CommandServer::Response, this, rawConnection, std::placeholders::_1);

                handler(context, command, Responsor);
            }
            catch ( const std::exception& e ) {
                LOG(LOG_ERROR) << "Some errors in command handler";
                LOG(LOG_ERROR) << e.what();

                Responsor Responsor = std::bind(&CommandServer::Response, this, rawConnection, std::placeholders::_1);

                hurricane::message::Response response(hurricane::message::Response::Status::Failed);
                Responsor(response);
            }
        });
    });

    _listener->StartListen();
}

template <class CommandServerContext>
void CommandServer<CommandServerContext>::Response(hurricane::util::TcpConnection* connection, const hurricane::message::Response& response) {
    hurricane::base::ByteArray responseBytes = response.Serialize();
    connection->Send(responseBytes.data(), responseBytes.size());
}

template <class CommandServerContext>
void CommandServer<CommandServerContext>::OnCommand(int32_t commandType, CommandHandler handler) {
    _commandHandlers.insert({ commandType, handler });
}

template <class CommandServerContext>
void CommandServer<CommandServerContext>::OnConnection(ConnectHandler handler) {
    _connectHandler = handler;
}

}
}
