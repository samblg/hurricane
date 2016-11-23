## What's Hurricane

Hurricane is Hurricane is a C++ based open-source distributed real-time processing system. 
Different from the batch processing system like Apache Hadoop, 
Hurricane uses stream model to process data. It also supports multi-language interfaces, 
such as Python, JavaScript, Java and Swift.

We imitate the interface of Apache Storm and simplify it, 
so the developer familiar with Storm can learn the Hurricane easily.

## Get Started

Before gaining an insight into Hurricane, we use an example program to illustrate the 
usage of Hurricane.

The example program is used to count the words of all text in data stream.

At first, we need to define a `spout` to generate the data stream. 
For the sake of briefness, the spout we defined only generate the text "Hello World".
So the remaining part of program will calculate the count of word "Hello" and "World".

Before showng the code, we use a diagram to show the structure of program.

![Word Count Diagram](wordcount.png)

###HelloWorldSpout.h

```cpp
#pragma once

#include "hurricane/spout/ISpout.h"

class HelloWorldSpout : public hurricane::spout::ISpout {
public:
    virtual void Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) override;
	virtual void Cleanup() override;
	virtual std::vector<std::string> DeclareFields() override;
	virtual void NextTuple() override;

private:
	std::shared_ptr<hurricane::collector::OutputCollector> _outputCollector;
};

```

The `HelloWorldSpout` is inherited from the ISpout class and override some virtual functions. 
Now we explain these member functions:

* Prepare: Hurricane will call this member function to initialize the spout task.
* Cleanup: Hurricane will call this member function to destroy the spout task.
* DeclareFields: Hurricane will call this member function to get the fields of spout.
* NextTuple: Hurricane will repeated call this member function to get the next tuple.

Now let we have a look at the implementation of HelloWorldSpout.

### HelloWorldSpout.cpp

```cpp
#include "sample/wordcount/HelloWorldSpout.h"

void HelloWorldSpout::Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
}

void HelloWorldSpout::Cleanup() {
}

std::vector<std::string> HelloWorldSpout::DeclareFields() {
	return { "sentence" };
}

void HelloWorldSpout::NextTuple() {
	_outputCollector->Emit({
		"Hello World"
	});
}
```

In Prepare member function, we save the outputCollector passed by Hurricane.
We will use the output collector to emit tuples.

In DeclareFields member function, we return a vector of string. Every element in the vector is a field name.

In NextTuple member function, we use output collector to emit a `tuple`. 
The `tuple` is an array of `Value`, which can store all the basic type variable in C++(including std::string).
The fields declared by DeclareFields is used to name the elements in tuple.

So we know that the HelloWorldSpout will repeated Emit tuples contains string "Hello Wolrd". 
It is a very simple `Spout`.

### SplitSentenceBolt.h

Now we have a data generator called `HelloWorldSpout`. But a system must process the data, or it is useless.

In Hurricane, we use the `Bolt` task to process input data and generate output data. 
Now we define a Bolt called `SplitSentenceBolt` to split the sentence in data stream into words.

```cpp
#pragma once

#include "hurricane/bolt/IBolt.h"

class SplitSentenceBolt : public hurricane::bolt::IBolt {
public:
    virtual void Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) override;
	virtual void Cleanup() override;
	virtual std::vector<std::string> DeclareFields() override;
	virtual void Execute(const hurricane::base::Tuple& tuple) override;

private:
	std::shared_ptr<hurricane::collector::OutputCollector> _outputCollector;
};
```

The SplitSentenceBolt is inherited from class `IBolt`, which is the base class of all Bolts.
And the interface of Bolt is similiar to the interface of Spout. It has Prepare, Cleanup and DeclareFields.

But the last member function of Bolt is `Execute`. 
Hurrican will call this function when accept a tuple from other spouts or bolts.
We will put all data processing code int this member function.

### SplitSentenctBolt.cpp

Now we will show the implementation of SplitSentenctBolt.

```cpp
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
	std::string sentence = tuple[0].ToString();
	std::vector<std::string> words = SplitString(sentence, ' ');

	for ( const std::string& word : words ) {
		_outputCollector->Emit({ word });
	}
}
```

In DeclareFields function, we return one field called word.

In Execute function, we get the first elment of tuple and convert it to sting.
Then we call SplitSentenct to split the sentence into words.
At last, we ergodic words and emit every word as an individual tuple.

So this Bolt will split the sentence from other task, and emit many tuples. 
Every tuple contains a word.

### WordCountBolt.h

After get all words in the sentences, we need to count all the words. 
The `WordCountBolt` class is defined to finish the counting task.

The definition of WordCountBolt:

```cpp
#pragma once

#include "hurricane/bolt/IBolt.h"

#include <map>
#include <string>
#include <cstdint>

class WordCountBolt : public hurricane::bolt::IBolt {
public:
    virtual void Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) override;
	virtual void Cleanup() override;
	virtual std::vector<std::string> DeclareFields() override;
	virtual void Execute(const hurricane::base::Tuple& tuple) override;

private:
	std::shared_ptr<hurricane::collector::OutputCollector> _outputCollector;
	std::map<std::string, int32_t> _wordCounts;
};
```

The WordCountBolt use a map to store count of words.

### WordCountBolt.cpp

```cpp
#include "sample/wordcount/WordCountBolt.h"
#include "hurricane/util/StringUtil.h"

void WordCountBolt::Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
}

void WordCountBolt::Cleanup() {
}

std::vector<std::string> WordCountBolt::DeclareFields() {
	return{ "word", "count" };
}

void WordCountBolt::Execute(const hurricane::base::Tuple& tuple) {
	std::string word = tuple[0].ToString();

	auto wordCountIterator = _wordCounts.find(word);
	if ( wordCountIterator == _wordCounts.end() ) {
		_wordCounts.insert({ word, 0 });
		wordCountIterator = _wordCounts.find(word);
	}

	wordCountIterator->second ++;

	_outputCollector->Emit({ word, wordCountIterator->second });
}
```

In function `DeclareFields`, we define two fields. 
The first field is word and the second field is the count of word.

In function `Execute`, we get the first element of tuple and convert it into string.

Then we find a record of the word. If record does not exist, we initialize the record with count 0.

Then we increment the count of word and emit the word and its count.

### WordCountTopology.cpp

We have compelete all task components of `Word Count`.
Then we need to create a `Topology` to combine these components.

```cpp
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
		.GlobalGroup("hello-world-spout")
		.ParallismHint(1);

	topology->SetBolt("word-count-bolt", new WordCountBolt)
		.FieldGroup("split-sentence-bolt", "word")
		.ParallismHint(2);

	return topology;
}
```

The function name must be `GetTopology`.

In this function, we use the constructor of Topology to create a new Topology. 
The parameter of constructor is the name of topology.

Then we use SetSpout to append a spout to topology. 
The first argument is the name of spout task, the second argument is an spout instance.
We use ParallismHint to set the paramllism of taks.

Here we create a HelloWorldSpout instance and append it to topology. 
The task name is `"hello-world-spout"`.

Then we use SetBolt to append a bolt to topology.
The first argument is the name of bolt task, the second argument is an bolt instance.

Here we create a SplitSentenceBolt instance and WordCountBolt instance, then append them into topology.
The task names are `"split-sentence-bolt"` and `"word-count-bolt"`.

We use Global to stream the data from spout to one SplitSentenceBolt 
and use Group to stream the data from SplitSentenceBolt to WordCountBolt.

At last, we return the topology and build it as an dynamic library. 
Then we can use Hurricane to load and execute the library.

## The concepts of Hurricane

After the example, you can have a rough idea of Hurricane. Now we illustrate some concepts of Hurricane.
If you have learned Apache, you must be familiar with these concepts.

We discuss following concepts:

* Topology
* Stream
* Tuple
* Spout
* Bolt
* Stream grouping

### Topology
The logic for a realtime application is packaged into a Hurricane topology. 
A Hurricane topology is analogous to a MapReduce job. 
One key difference is that a MapReduce job eventually finishes, 
whereas a topology runs forever. Of course, you can kill the topology manually.
A topology is a graph of spouts and bolts that are connected with stream groupings. 
We have seen the structure diagram of example program. 
You can treat the topology in example as the graph in this diagram.
These concepts are described below.

### Stream
The stream is an important abstraction in Hurricane. 
A stream is an unbounded sequence of tuples that is processed and created in parallel in a distributed fashion.
Streams are defined with a schema that names the fields in the stream's tuples.
Every stream is given an id when declared. Spout and bolt will create and specifying an id for stream automatically.

### Tuple
Tuple is the data unit transferred in stream. 
The spout and bolt need to use tuple to organize the data.
Tuples can contain integers, longs, shorts, characters, floats, doubles and strings.

### Spout
A spout is a source of streams in a topology.
Generally spouts will read tuples from an external source and emit them into the topology.
Spouts can either be reliable or unreliable.
A reliable spout is capable of replaying a tuple if it failed to be processed by Hurricane, whereas an unreliable spout forgets about the tuple as soon as it is emitted.

The main member function on spouts is NextTuple.
NextTuple either emits a new tuple into the topology or simply returns if there are no new tuples to emit.

The other main methods on spouts are Ack and Fail.
These are called when Hurricane detects that a tuple emitted from the spout either successfully completed through the topology or failed to be completed.
Ack and Fail are only called for reliable spouts.

### Bolt
All processing in topologies is done in bolts. 
Bolts can do anything from filtering, functions, aggregations, joins, talking to databases, and more.

Bolts can do simple stream transformations.
Doing complex stream transformations often requires multiple steps and thus multiple bolts.
For example, transforming a stream of tweets into a stream of trending images requires at least two steps:
a bolt to do a rolling count of retweets for each image,
and one or more bolts to stream out the top X images 
(you can do this particular stream transformation in a more scalable way with three bolts than with two).

The main member function in bolts is the Execute function which takes in as input a new tuple.
Bolts emit new tuples using the OutputCollector object.
Bolts must call the Ack method on the OutputCollector for every tuple they process so that Storm knows when tuples are completed 
(and can eventually determine that its safe to ack the original spout tuples). 
For the common case of processing an input tuple, emitting 0 or more tuples based on that tuple, and then acking the input tuple, 
Hurricane provides an IBasicBolt interface which does the acking automatically.

Its perfectly fine to launch new threads in bolts that do processing asynchronously. OutputCollector is thread-safe and can be called at any time.

### Stream grouping
Part of defining a topology is specifying for each bolt which streams it should receive as input.
A stream grouping defines how that stream should be partitioned among the bolt's tasks.

There are four built-in stream groupings in Hurricane:

* Random grouping: 
Tuples are randomly distributed across the bolt's tasks in a way 
such that each bolt is guaranteed to get an equal number of tuples.
* Fields grouping: 
The stream is partitioned by the fields specified in the grouping.
For example, if the stream is grouped by the "user-id" field, tuples with the same "user-id" will always go to the same task, but tuples with different "user-id"'s may go to different tasks.
* Global grouping:
The entire stream goes to a single one of the bolt's tasks.
Specifically, it goes to the task with the lowest id.
* Local or random grouping:
If the target bolt has one or more tasks in the same worker process,
tuples will be shuffled to just those in-process tasks.
Otherwise, this acts like a normal shuffle grouping.

## The architecture of Hurricane
Now we illustrate the architecture of Hurricane simply.
You can gaining an insight into Hurricane throught this chapter.

The Hurricane is a master-slave based distributed system. 
The master node will control all the slave node.
If the master node is down, all system will stop, which is called `single point of failure`.
Hurricane will solve this problem in future.

### President
President is the master node of Hurricane. A Hurricane cluster can only have one president.
It will start a `CommandServer` and wait for other node to connect to it.

A CommandServer is a message loop based on TCP/IP protocol, 
in which you can register a command and handler.
When a CommandClient send a command to the server, 
it will call the registered handler to process the command.
All Command and arguments are serialized and deserialized by a simple binary protocol.

After a slave node join the president, president will add it to the cluster metadata.

All topology management command are sent to president. 
When you startup a topology, president will find a free executor in cluster metadata 
and send command to the slave node owned the executor.
President will try to distribute the computing loads on all slave node to control the load-balance.
If there is no availiable resources, president will report error. 

But now president can't rebalance the computing executors after detecting a manager disconnected.
We will solve this problem in future.

### Manager
Manager is the slave node of Hurricane. A Hurricane cluster will have a lot of managers.

Manager will be started and use CommandClient to send Join command to president.
After manager receive the response of president, it will startup a CommandServer 
to receive command from other managers.

Manager will send Heartbeat command to Manager in a fixed period. 
If predisent find a manager have detected the manager disconnected for some periods, 
it will destroy the connection from this manager.

When receive the StartTask command, manager will create a new task instance and
deploy it into the executor specified by president.

After receive tuple from other task, manager will send the tuple to the correct executor
and wait for next command.

Now we do not consider the problem that executor maybe shutdown, 
but it will be solved in next version.

### Executor
Executor is a thread to execute task.

An executor is used to execute a spout or bolt. 
It has a message loop and wait the message from manager.

When SpoutExecutor receives the StartSpout message, it will initialize the spout task
and execute NextTuple function repeated until it receive the StopSpout message.

When BoltExecutor receives the StartBolt message, it will initialize the bolt task
and wait for ExecuteBolt message. The manager will send the ExecuteBolt message to executor
, then executor will execute the Execute function of its bolt task.
When BoltExecutor receives the StopBolt message, it will destroy the bolt task.

### Task
Task is a spout or bolt which is executed by executor.

Different from Apache Storm, a Hurricane executor only execute one task, 
because it is simple and easy to schedule.

### Summary
At last, let we summarize this chapter by a architecture diagram of Hurricane.

![Hurricane Architecture](architecture.png)

Now we advice you to read the code of Hurricane to know more of Hurricane.