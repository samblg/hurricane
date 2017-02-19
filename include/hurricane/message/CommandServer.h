#pragma once

#include "hurricane/util/NetListener.h"
#include "hurricane/message/Command.h"
#include "hurricane/base/ByteArray.h"
#include "logging/Logging.h"

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
    typedef std::function<void(const Response& response)> Responsor;
    typedef std::function<void(CommandServerContext* context)> ConnectHandler;
    typedef std::function<void(CommandServerContext* context, const Command& command, Responsor)> CommandHandler;

    CommandServer(hurricane::util::NetListener* listener = nullptr);
    virtual ~CommandServer();

    void StartListen();
    void OnConnection(ConnectHandler handler);

    template <class ObjectType, class HandlerType>
    void OnCommand(int32_t commandType, ObjectType* self, HandlerType handler) {
        OnCommand(commandType, std::bind(handler, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    void OnCommand(int32_t commandType, CommandHandler handler);
    void Response(hurricane::util::TcpConnection* connection, const Response& response);

    void SetListener(hurricane::util::NetListener* listener) {
        _listener = listener;
    }

private:
    hurricane::util::NetListener* _listener;
    std::map<int32_t, CommandHandler> _commandHandlers;
    ConnectHandler _connectHandler;
};

}
}

#include "hurricane/message/CommandServer.tcc"
