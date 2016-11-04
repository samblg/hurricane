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
    while ( true ) {
        _spout->NextTuple();
    }
}


}
}
