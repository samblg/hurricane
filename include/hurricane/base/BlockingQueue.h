#pragma once

#include <condition_variable>
#include <string>
#include <queue>
#include <chrono>

namespace hurricane {
    namespace base {
        template <class T>
        class BlockingQueue {
        public:
            BlockingQueue() {
            }

            virtual ~BlockingQueue() {
            }

            void Push(const T& element) {
                std::unique_lock<std::mutex> locker(_mutex);

                _queue.push(element);
                _emptyCv.notify_one();
            }

            bool Pop(T& element, int milliseconds = 0) {
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

            bool Peek(T& element) const {
                std::unique_lock<std::mutex> locker(_mutex);

                if ( !_queue.size() ) {
                    return false;
                }

                element = _queue.front();
                return true;
            }

            bool Empty() const {
                std::unique_lock<std::mutex> locker(_mutex);

                return _queue.size() == 0;
            }

            int GetSize() const {
                std::unique_lock<std::mutex> locker(_mutex);

                return _queue.size();
            }

        private:
            mutable std::mutex _mutex;
            std::condition_variable _emptyCv;
            std::queue<T> _queue;
        };
    }
}