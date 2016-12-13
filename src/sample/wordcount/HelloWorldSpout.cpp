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

#include "sample/wordcount/HelloWorldSpout.h"
#include "hurricane/util/StringUtil.h"
#ifndef WIN32
#include <sys/time.h>
#endif
#include <thread>
#include <chrono>

void HelloWorldSpout::Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
    _words = SplitString("Hello world there are some words we generate new sentence randomly", ' ');
}

void HelloWorldSpout::Cleanup() {
}

std::vector<std::string> HelloWorldSpout::DeclareFields() {
    return { "sentence" };
}

void HelloWorldSpout::NextTuple() {
    static int32_t id = 0;
#ifndef WIN32
    timeval currentTime;
    gettimeofday(&currentTime, nullptr);

    int64_t currentMicroseconds = currentTime.tv_sec;
    currentMicroseconds *= 1000000;
    currentMicroseconds += currentTime.tv_usec;
#else
    int64_t currentMicroseconds = 0;
#endif

    ++ id;

    std::vector<std::string> words(5);
    for ( int32_t i = 0; i < 5; i ++ ) {
        words[i] = _words[rand() % _words.size()];
    }

    std::string sentence = JoinString(words);
    _outputCollector->Emit({
        sentence, currentMicroseconds, id
    });
}
