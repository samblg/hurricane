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

namespace hurricane {
namespace base {

template <class T>
BlockingQueue<T>::BlockingQueue() {
}

template <class T>
BlockingQueue<T>::~BlockingQueue() {
}

template <class T>
void BlockingQueue<T>::Push(const T& element) {
    std::unique_lock<std::mutex> locker(_mutex);

    _queue.push(element);
    _emptyCv.notify_one();
}

template <class T>
bool BlockingQueue<T>::Pop(T& element, int32_t milliseconds) {
    std::unique_lock<std::mutex> locker(_mutex);

    if ( !_queue.size() ) {
        if ( milliseconds == 0 ) {
            _emptyCv.wait(locker);
        }
        else {
            _emptyCv.wait_for(locker, std::chrono::milliseconds(milliseconds));
            if ( !_queue.size() ) {
                return false;
            }
        }
    }

    element = _queue.front();
    _queue.pop();

    return true;
}

template <class T>
bool BlockingQueue<T>::Peek(T& element) const {
    std::unique_lock<std::mutex> locker(_mutex);

    if ( !_queue.size() ) {
        return false;
    }

    element = _queue.front();
    return true;
}

template <class T>
bool BlockingQueue<T>::Empty() const {
    std::unique_lock<std::mutex> locker(_mutex);

    return _queue.size() == 0;
}

}
}
