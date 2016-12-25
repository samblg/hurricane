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

#include "sample/wordcount/WordCountBolt.h"
#include "hurricane/util/StringUtil.h"
#include "logging/Logging.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#include <sstream>

std::string itos(int32_t number) {
    std::ostringstream ss;
    ss << number;

    return ss.str();
}

void WordCountBolt::Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
    _logFile = new std::ofstream("timestamp" + itos(rand()) + ".txt");
}

void WordCountBolt::Cleanup() {
    delete _logFile;
}

std::vector<std::string> WordCountBolt::DeclareFields() {
    return{ "word", "count" };
}

void WordCountBolt::Execute(const hurricane::base::Tuple& tuple) {
    std::string word = tuple[0].GetStringValue();
    int64_t sourceMicroseconds = tuple[1].GetInt64Value();
    int32_t id = tuple[2].GetInt32Value();

    auto wordCountIterator = _wordCounts.find(word);
    if ( wordCountIterator == _wordCounts.end() ) {
        _wordCounts.insert({ word, 0 });
        wordCountIterator = _wordCounts.find(word);
    }

    wordCountIterator->second ++;

    LOG(LOG_INFO) << word << ' ' << wordCountIterator->second;
    _outputCollector->Emit({ word, wordCountIterator->second });

#ifndef WIN32
    timeval currentTime;
    gettimeofday(&currentTime, nullptr);

    int64_t currentMicroseconds = currentTime.tv_sec;
    currentMicroseconds *= 1000000;
    currentMicroseconds += currentTime.tv_usec;
#else
    int64_t currentMicroseconds = 0;
#endif

    *_logFile << sourceMicroseconds << ' ' << currentMicroseconds << std::endl;
}
