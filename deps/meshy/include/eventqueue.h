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

#include "bytearray.h"
#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>

namespace meshy
{

    class IStream;

    class BaseEvent
    {
    public:
        BaseEvent() { }

        BaseEvent(const std::string &type, const ByteArray &data,
                  IStream *stream) :
                _type(type), _data(data), _stream(stream)
        {
        }

        void SetData(const ByteArray &data)
        {
            _data = data;
        }

        const ByteArray &GetData() const
        {
            return _data;
        }

        void SetType(const std::string &type)
        {
            _type = type;
        }

        const std::string &GetType() const
        {
            return _type;
        }

        void SetStream(IStream *stream)
        {
            _stream = stream;
        }

        IStream *GetStream() const
        {
            return _stream;
        }

    private:
        std::string _type;
        ByteArray _data;
        IStream* _stream;
    };

    class EventQueue {
    public:
        EventQueue(int timeout = 0) : _timeout(timeout) { }

        void PostEvent(BaseEvent *event)
        {
            std::unique_lock <std::mutex> locker(_mutex);

            _events.push_back(std::shared_ptr<BaseEvent>(event));
        }

        std::shared_ptr <BaseEvent> GetEvent()
        {
            std::unique_lock <std::mutex> locker(_mutex);

            if (_events.empty())
            {
                if (_timeout == 0)
                {
                    return nullptr;
                }

                _waitCondition.wait_for(locker, std::chrono::milliseconds(_timeout));
            }

            if (!_events.empty())
            {
                std::shared_ptr <BaseEvent> event = _events.front();
                _events.erase(_events.begin());

                return event;
            }

            return nullptr;
        }

    private:
        std::vector <std::shared_ptr<BaseEvent>> _events;
        std::mutex _mutex;
        std::condition_variable _waitCondition;
        // ms
        int _timeout;
    };

}
