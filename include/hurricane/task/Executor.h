#pragma once

#include "hurricane/message/Message.h"

namespace hurricane {
namespace task {

class Executor {
public:
    struct MessageType {
        enum {
            OnTuple
        };
    };

    virtual ~Executor() {}

    virtual void Start() = 0;
};

}
}
