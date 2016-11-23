#pragma once

#include "hurricane/task/Executor.h"
#include <thread>
#include <memory>

namespace hurricane {

namespace spout {
    class ISpout;
}
namespace task {

class SpoutExecutor : public Executor {
public:
    SpoutExecutor();
    ~SpoutExecutor() {}

    void Start();
    void SetSpout(spout::ISpout* spout);

    std::shared_ptr<spout::ISpout> GetSpout() {
        return _spout;
    }

<<<<<<< HEAD
=======
    int GetFlowParam() const;
    void SetFlowParam(int GetFlowParam);

>>>>>>> master
private:
    void MainLoop();

    std::thread _thread;
    std::shared_ptr<spout::ISpout> _spout;
<<<<<<< HEAD
=======
    int _flowParam;
>>>>>>> master
};

}
}
