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

#include "sample/wordcount/WordCountTopology.h"
#include "sample/wordcount/HelloWorldSpout.h"
#include "sample/wordcount/SplitSentenceBolt.h"
#include "sample/wordcount/WordCountBolt.h"

#include "hurricane/topology/Topology.h"

hurricane::topology::Topology* GetTopology() {
    hurricane::topology::Topology* topology = new hurricane::topology::Topology("word-count-topology");

    topology->SetSpout("hello-world-spout", new HelloWorldSpout)
        .ParallismHint(1);

    topology->SetBolt("split-sentence-bolt", new SplitSentenceBolt)
        .Random("hello-world-spout")
        .ParallismHint(3);

    topology->SetBolt("word-count-bolt", new WordCountBolt)
        .Field("split-sentence-bolt", "word")
        .ParallismHint(2);

    return topology;
}
