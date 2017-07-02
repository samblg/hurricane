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

#include "hurricane/base/ITask.h"
#include "hurricane/message/MessageLoop.h"

#include <thread>
#include <memory>
#include <vector>
#include <iostream>
#include <mutex>
#include <functional>

namespace hurricane {
namespace base {

template <class TaskType>
class Executor {
public:
    enum class Status {
        Stopping,
        Running
    };

    Executor() : _status(Status::Stopping) {
    }

    virtual ~Executor() {}

    void StartTask(const std::string& taskName, TaskType* task) {
		_messageLoop.MessageMap(BoltMessage::MessageType::Data,
			this, &BoltMessageLoop::OnData);
        _taskName = taskName;
        _task = std::shared_ptr<TaskType>(task);

		_thread = std::thread(std::bind(&Executor::StartThread, this));
    }

    virtual void StopTask() {
		_messageLoop.Stop();
    }

    Status GetStatus() const {
        return _status;
    }

    const std::string& GetTaskName() const {
        return _taskName;
    }

protected:
	virtual void OnCreate() = 0;
	virtual void OnStop() = 0;
    std::shared_ptr<TaskType> _task;
	hurricane::message::MessageLoop _messageLoop;

private:
    void StartThread() {
		_status = Status::Running;

		OnCreate();
		_messageLoop.Run();
		OnStop();

        _status = Status::Stopping;
    }

    std::thread _thread;
	Status _status;
    std::string _taskName;
};

}
}
