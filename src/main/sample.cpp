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

#include "hurricane/Hurricane.h"
#include "hurricane/topology/LocalTopology.h"
#include "hurricane/topology/TopologyBuilder.h"
#include "hurricane/topology/ITopology.h"
#include "hurricane/base/Executor.h"
#include "hurricane/bolt/BoltExecutor.h"
#include "hurricane/spout/SpoutExecutor.h"
#include "hurricane/base/OutputCollector.h"
#include "hurricane/spout/ISpout.h"
#include "hurricane/bolt/IBolt.h"
#include "hurricane/base/Values.h"
#include "hurricane/base/Fields.h"
#include "util/String.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <thread>
#include <chrono>

using hurricane::topology::TopologyBuilder;
using hurricane::topology::LocalTopology;
using hurricane::topology::ITopology;
using hurricane::spout::ISpout;
using hurricane::bolt::IBolt;
using hurricane::base::OutputCollector;
using hurricane::base::Values;
using hurricane::base::Fields;

class TextGenerateSpout : public ISpout {
public:
    TextGenerateSpout* Clone() const override {
        return new TextGenerateSpout(*this);
    }

    void Open(OutputCollector& outputCollector) override {
        _outputCollector = &outputCollector;
    }

    void Close() override {
    }

    void Execute() override {
        _outputCollector->Emit({ "The cBioPortal for Cancer Genomics provides visualization, analysis, and download of large-scale cancer genomics data sets. The cBioPortal is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License, version 3, as published by the Free Software Foundation" });
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    Fields DeclareFields() const override {
        return { "text" };
    }

private:
    OutputCollector* _outputCollector;
};

// 单词分割
class WordSplitBolt : public IBolt {
public:
    WordSplitBolt* Clone() const override {
        return new WordSplitBolt(*this);
    }

    void Prepare(OutputCollector& outputCollector) override {
        _outputCollector = &outputCollector;
    }

    void Cleanup() override {
    }

    void Execute(const Values& values) override {
        try {
            std::string text = values[0].ToString();
            std::list<std::string> words = SplitString(text, ' ');

            for ( const std::string& word : words ) {
                _outputCollector->Emit({ word, 1 });
            }
        }
        catch ( std::exception e ) {
            std::cerr << e.what() << std::endl;
        }
    }

    Fields DeclareFields() const override {
        return { "word", "count" };
    }

private:
    OutputCollector* _outputCollector;
};

class WordCountBolt : public IBolt {
public:
    WordCountBolt* Clone() const override {
        return new WordCountBolt(*this);
    }

    void Prepare(OutputCollector& outputCollector) override {
        _outputCollector = &outputCollector;
    }

    void Cleanup() override {
    }

    void Execute(const Values& values) override {
        std::string word = values[0].ToString();
        int count = values[1].ToInt32();

        // 单词计数
        auto wordCountIter = _wordCounts.find(word);
        if ( wordCountIter == _wordCounts.end() ) {
            _wordCounts.insert({ word, 0 });
            wordCountIter = _wordCounts.find(word);
        }
        wordCountIter->second += count;

        std::ostringstream formatter;
        formatter << "Word: " << word << "; Current Count: " << count << "; Total Count: " << wordCountIter->second;
        std::cerr << formatter.str() << std::endl;
    }

    Fields DeclareFields() const override {
        return {};
    }

private:
    OutputCollector* _outputCollector;
    std::map<std::string, int> _wordCounts;
};

int main() {
    LocalTopology localTopology;
    TopologyBuilder topologyBuilder;

    topologyBuilder.SetSpout("textGenerateSpout", new TextGenerateSpout);
    topologyBuilder.SetBolt("wordSplitBolt", new WordSplitBolt, "textGenerateSpout");
    topologyBuilder.SetBolt("wordCountBolt", new WordCountBolt, "wordSplitBolt");

    ITopology* topology = topologyBuilder.Build();
    localTopology.Submit("hello", std::shared_ptr<ITopology>(topology));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10));

    return EXIT_SUCCESS;
}
