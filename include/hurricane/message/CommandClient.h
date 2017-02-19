#pragma once

#include <functional>

namespace hurricane {
namespace util {
class NetConnector;
}

namespace message {

class Command;
class Response;

class CommandError {
public:
    enum class Type {
        NoError,
        SendError,
        ConnectError
    };

    CommandError() : _type(Type::NoError) {}

    CommandError(Type type, const std::string& message) :
        _type(type), _message(message) {
    }

    Type GetType() const {
        return _type;
    }

    // exception interface
public:
    const char* what() const {
        return _message.c_str();
    }

    const std::string& GetMessage() const {
        return _message;
    }

private:
    Type _type;
    std::string _message;
};

class CommandClient {
public:
    typedef std::function<void(const CommandError&)> ConnectCallback;
    typedef std::function<void(const Response& response, const CommandError&)> SendCommandCallback;

    CommandClient(hurricane::util::NetConnector* connector);
    ~CommandClient();

    void Connect(ConnectCallback callback);
    void SendCommand(const Command& command, SendCommandCallback callback);

    hurricane::util::NetConnector* GetConnector() {
        return _connector;
    }

    const hurricane::util::NetConnector* GetConnector() const {
        return _connector;
    }

private:
    hurricane::util::NetConnector* _connector;
};

}
}
