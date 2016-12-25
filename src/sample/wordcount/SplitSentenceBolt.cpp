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

#include "sample/wordcount/SplitSentenceBolt.h"
#include "hurricane/util/StringUtil.h"

void SplitSentenceBolt::Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
}

void SplitSentenceBolt::Cleanup() {
}

std::vector<std::string> SplitSentenceBolt::DeclareFields() {
    return{ "word" };
}

void SplitSentenceBolt::Execute(const hurricane::base::Tuple& tuple) {
    std::string sentence = tuple[0].GetStringValue();
    int64_t sourceMicroseconds = tuple[1].GetInt64Value();
    int32_t id = tuple[2].GetInt32Value();

    std::vector<std::string> words = SplitString(sentence, ' ');

    for ( const std::string& word : words ) {
        _outputCollector->Emit({ word, sourceMicroseconds, id });
    }
}
