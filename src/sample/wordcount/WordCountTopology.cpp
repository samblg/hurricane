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

//    topology->SetBolt("word-count-bolt", new WordCountBolt)
//        .Field("split-sentence-bolt", "word")
//        .Random("split-sentence-bolt")
//        .ParallismHint(2);

	return topology;
}
