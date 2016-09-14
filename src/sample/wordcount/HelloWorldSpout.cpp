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