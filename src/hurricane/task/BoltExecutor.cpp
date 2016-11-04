#include "hurricane/task/BoltExecutor.h"
#include "hurricane/bolt/IBolt.h"
#include "hurricane/collector/TaskQueue.h"

namespace hurricane {
namespace task {

BoltExecutor::BoltExecutor()
{
    _loop.MessageMap(Executor::MessageType::OnTuple, this, &BoltExecutor::OnTuple);
}

void BoltExecutor::Start()
{
    _thread = std::thread(&BoltExecutor::StartLoop, this);
}

void BoltExecutor::OnTuple(message::Message& message)
{
}

void BoltExecutor::StartLoop()
{
    collector::TaskItem* taskItem;

    while ( _taskQueue->Pop(taskItem) ) {
        _bolt->Execute(taskItem->GetTuple());

        delete taskItem;
        taskItem = nullptr;
    }
}


}
}
