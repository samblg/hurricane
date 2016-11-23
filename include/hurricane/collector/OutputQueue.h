#pragma once

#include "hurricane/base/BlockingQueue.h"
#include "hurricane/base/Values.h"

namespace hurricane {
namespace collector {

class OutputItem {
public:
<<<<<<< HEAD
    OutputItem(int taskIndex, const base::Tuple& tuple) :
            _taskIndex(taskIndex), _tuple(tuple) {
=======
    OutputItem(int taskIndex, const base::Tuple& tuple, const std::string& taskName) :
            _taskIndex(taskIndex), _tuple(tuple) {
        _tuple.SetSourceTask(taskName);
>>>>>>> master
    }

    int GetTaskIndex() const {
        return _taskIndex;
    }

<<<<<<< HEAD
=======
    base::Tuple& GetTuple() {
        return _tuple;
    }

>>>>>>> master
    const base::Tuple& GetTuple() const {
        return _tuple;
    }

private:
    int _taskIndex;
    base::Tuple _tuple;
};

class OutputQueue : public base::BlockingQueue<OutputItem*> {
};

}
}
