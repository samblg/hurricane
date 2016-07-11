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


namespace meshy {

    template<class Type>
    ThreadPool<Type>::ThreadPool(int32_t threads, std::function<void(Type &record)> handler)
            : _shutdown(false),
              _threads(threads),
              _handler(handler),
              _workers(),
              _tasks() {
        if (_threads < MIN_THREADS)
            _threads = MIN_THREADS;

        for (int32_t i = 0; i < _threads; ++i)
            _workers.emplace_back(
                    [this] {
                        while (!_shutdown) {
                            Type record;
                            _tasks.Pop(record, true);
                            _handler(record);
                        }
                    }
            );
    }

    template<class Type>
    ThreadPool<Type>::~ThreadPool() {
        for (std::thread &worker: _workers)
            worker.join();
    }

    template<class Type>
    void ThreadPool<Type>::Submit(Type record) {
        _tasks.Push(record);
    }

}