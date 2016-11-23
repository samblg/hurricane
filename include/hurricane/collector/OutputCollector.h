#pragma once

#include <memory>

namespace hurricane {
namespace base {
    class Tuple;
}

namespace collector {
    class OutputQueue;

    class OutputCollector {
    public:
        OutputCollector() : _taskIndex(-1) {}
<<<<<<< HEAD
        OutputCollector(int taskIndex, std::shared_ptr<OutputQueue> queue) :
            _taskIndex(taskIndex), _queue(queue) {
=======
        OutputCollector(int taskIndex, const std::string& taskName, std::shared_ptr<OutputQueue> queue) :
            _taskIndex(taskIndex), _taskName(taskName), _queue(queue) {
>>>>>>> master
        }

        void SetQueue(std::shared_ptr<OutputQueue> queue) {
            _queue = queue;
        }

        std::shared_ptr<OutputQueue> GetQueue() const {
            return _queue;
        }

        void Emit(const hurricane::base::Tuple& tuple);

    private:
        int _taskIndex;
<<<<<<< HEAD
=======
        std::string _taskName;
>>>>>>> master
        std::shared_ptr<OutputQueue> _queue;
    };

}
}
