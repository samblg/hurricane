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

#include "sample/jwordcount/JWordCountTopology.h"
#include "sample/jwordcount/JWordCountBolt.h"

#include "hurricane/topology/Topology.h"

#include "hurricane/spout/JavaSpout.h"
#include "hurricane/bolt/JavaBolt.h"

using hurricane::spout::JavaSpout;
using hurricane::bolt::JavaBolt;

hurricane::topology::Topology* GetTopology() {
    hurricane::topology::Topology* topology = new hurricane::topology::Topology("jword-count-topology");

    topology->SetSpout("sentence-spout", new JavaSpout("sample/jwordcount/SentenceSpout"))
        .ParallismHint(1);

    topology->SetBolt("split-sentence-bolt", new JavaBolt("sample/jwordcount/SplitSentenceBolt"))
        .Random("hello-world-spout")
        .ParallismHint(3);

    topology->SetBolt("word-count-bolt", new JWordCountBolt)
        .Field("split-sentence-bolt", "word")
        .ParallismHint(2);

    return topology;
}
