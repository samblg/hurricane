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

#pragma once

#include <string>
#include <vector>
#include "hurricane/base/ByteArray.h"
#include "hurricane/base/Variant.h"

namespace hurricane {
namespace message {
class Command {
public:
    struct Type {
        enum {
            Invalid = 0,
            Join,
            Heartbeat,
            SyncMetadata,
            SendTuple,
            AskField,
            OrderId
        };
    };

    Command(int32_t type = Type::Invalid);
    Command(int32_t type, std::vector<hurricane::base::Variant>& arguments);

    void AddArgument(const hurricane::base::Variant& argument);
    void AddArguments(const std::vector<hurricane::base::Variant>& arguments);

    void Deserialize(const hurricane::base::ByteArray& data);
    hurricane::base::ByteArray Serialize() const;

    int32_t GetType() const {
        return _type;
    }

    void SetType(int32_t type) {
        _type = type;
    }

    hurricane::base::Variant GetArgument(int32_t index) const {
        return _arguments[index];
    }

    int32_t GetArgumentCount() const {
        return static_cast<int32_t>(_arguments.size());
    }

    const std::vector<hurricane::base::Variant>& GetArguments() const {
        return _arguments;
    }

private:
    int32_t _type;
    std::vector<hurricane::base::Variant> _arguments;
};

class Response {
public:
    struct Status {
        enum {
            Failed = 0,
            Successful = 1
        };
    };

    Response(int32_t status = Status::Failed);
    Response(int32_t status, std::vector<hurricane::base::Variant>& arguments);

    void AddArguments(const std::vector<hurricane::base::Variant>& arguments);
    void AddArgument(const hurricane::base::Variant& argument);

    void Deserialize(const hurricane::base::ByteArray& data);
    hurricane::base::ByteArray Serialize() const;

    int32_t GetStatus() const {
        return _status;
    }

    void SetStatus(int32_t status) {
        _status = status;
    }

    hurricane::base::Variant GetArgument(int32_t index) const {
        return _arguments[index];
    }

    int32_t GetArgumentCount() const {
        return static_cast<int32_t>(_arguments.size());
    }

    const std::vector<hurricane::base::Variant>& GetArguments() const {
        return _arguments;
    }

private:
    int32_t _status;
    std::vector<hurricane::base::Variant> _arguments;
};
}
}
