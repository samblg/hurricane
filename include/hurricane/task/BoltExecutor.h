#pragma once

#include "hurricane/task/Executor.h"
#include <thread>
#include <memory>

namespace hurricane {

namespace bolt {
class IBolt;
}

namespace collector {
class TaskQueue;
}

namespace task {

class BoltExecutor : public Executor {
public:
    BoltExecutor();
    ~BoltExecutor() {}

    void Start();

    std::shared_ptr<bolt::IBolt> GetBolt() {
        return _bolt;
    }

    void SetBolt(bolt::IBolt* bolt) {
        _bolt.reset(bolt);
    }

    void SetTaskQueue(std::shared_ptr<collector::TaskQueue> taskQueue) {
        _taskQueue = taskQueue;
    }

protected:
    hurricane::message::MessageLoop _loop;

private:
    void OnTuple(hurricane::message::Message& message);
    void StartLoop();

    std::thread _thread;
    std::shared_ptr<bolt::IBolt> _bolt;
    std::shared_ptr<collector::TaskQueue> _taskQueue;
};

}
}
