#include "hurricane/task/SpoutExecutor.h"
#include "hurricane/spout/ISpout.h"
#include <chrono>

namespace hurricane {
namespace task {

SpoutExecutor::SpoutExecutor()
{

}

void SpoutExecutor::Start()
{
    _thread = std::thread(&SpoutExecutor::MainLoop, this);
}

void SpoutExecutor::SetSpout(spout::ISpout* spout)
{
    _spout.reset(spout);
}

void SpoutExecutor::MainLoop()
{
<<<<<<< HEAD
    while ( true ) {
        _spout->NextTuple();
    }
}

=======
    int flowTime = 1000 * 1000 / _flowParam;
    while ( true ) {
        _spout->NextTuple();
        std::this_thread::sleep_for(std::chrono::microseconds(flowTime));
    }
}

int SpoutExecutor::GetFlowParam() const
{
    return _flowParam;
}

void SpoutExecutor::SetFlowParam(int flowParam)
{
    _flowParam = flowParam;
}
>>>>>>> master

}
}
