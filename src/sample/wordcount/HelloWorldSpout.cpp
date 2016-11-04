#include "sample/wordcount/HelloWorldSpout.h"
#include <sys/time.h>
#include <thread>
#include <chrono>

void HelloWorldSpout::Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) {
	_outputCollector = outputCollector;
}

void HelloWorldSpout::Cleanup() {
}

std::vector<std::string> HelloWorldSpout::DeclareFields() {
	return { "sentence" };
}

void HelloWorldSpout::NextTuple() {
    timeval currentTime;
    gettimeofday(&currentTime, nullptr);

    int64_t currentMicroseconds = currentTime.tv_sec;
    currentMicroseconds *= 1000000;
    currentMicroseconds += currentTime.tv_usec;

	_outputCollector->Emit({
        "Hello World", currentMicroseconds
	});

    std::this_thread::sleep_for(std::chrono::microseconds(100));
}
