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

#ifndef NET_FRAME_THREAD_POOL_H
#define NET_FRAME_THREAD_POOL_H

#include "utils/concurrent_queue.h"

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#define MIN_THREADS 10

namespace meshy {
    template<class Type>
    class ThreadPool {
        ThreadPool &operator=(const ThreadPool &) = delete;

        ThreadPool(const ThreadPool &other) = delete;

    public:
        ThreadPool(int32_t threads, std::function<void(Type &record)> handler);

        virtual ~ThreadPool();

        void Submit(Type record);

    private:

    private:
        bool _shutdown;
        int32_t _threads;
        std::function<void(Type &record)> _handler;
        std::vector <std::thread> _workers;
        ConcurrentQueue <Type> _tasks;
    };

}
#include "template/utils/thread_pool.tcc"

#endif //NET_FRAME_THREAD_POOL_H
