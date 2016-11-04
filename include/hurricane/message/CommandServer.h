#pragma once

#include "hurricane/util/NetListener.h"
#include "hurricane/message/Command.h"
#include "hurricane/base/ByteArray.h"
#include <functional>
#include <cstdint>
#include <map>

namespace hurricane {
	namespace util {
		class TcpConnection;
	}

	namespace message {
		class BaseCommandServerContext {
		public:
			const std::string& GetId() const {
				return _id;
			}

			void SetId(const std::string& id) {
				_id = id;
			}

		private:
			std::string _id;
		};

		template <class CommandServerContext>
		class CommandServer {
		public:
			typedef std::function<void(const Response& response)> Responser;
			typedef std::function<void(CommandServerContext* context)> ConnectHandler;
			typedef std::function<void(CommandServerContext* context, const Command& command, Responser)> CommandHandler;

			CommandServer(hurricane::util::NetListener* listener = nullptr) : _listener(listener) {
			}

			virtual ~CommandServer();
			void StartListen();

			void OnConnection(ConnectHandler handler) {
				_connectHandler = handler;
			}

			template <class ObjectType, class HandlerType>
			void OnCommand(int32_t commandType, ObjectType* self, HandlerType handler) {
				OnCommand(commandType, std::bind(handler, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			}

			void OnCommand(int32_t commandType, CommandHandler handler) {
				_commandHandlers.insert({ commandType, handler });
			}

			void Response(hurricane::util::TcpConnection* connection, const Response& response);
			
			void SetListener(hurricane::util::NetListener* listener) {
				_listener = listener;
			}

		private:
			hurricane::util::NetListener* _listener;
			std::map<int32_t, CommandHandler> _commandHandlers;
			ConnectHandler _connectHandler;
		};

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

				connection->OnData([this, commandHandlers, context, rawConnection](const char* buffer, int32_t size) {
					hurricane::base::ByteArray commandBytes(buffer, size);
					Command command;
					command.Deserialize(commandBytes);

					int32_t commandType = command.GetType();
                    try {
                        CommandHandler handler = commandHandlers.at(commandType);
                        Responser responser = std::bind(&CommandServer::Response, this, rawConnection, std::placeholders::_1);

                        handler(context, command, responser);
                    }
                    catch ( const std::exception& e ) {
                        std::cout << "Some errors in command handler" << std::endl;
                        std::cerr << e.what() << std::endl;

                        Responser responser = std::bind(&CommandServer::Response, this, rawConnection, std::placeholders::_1);

                        hurricane::message::Response response(hurricane::message::Response::Status::Failed);
                        responser(response);
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
	}
}
