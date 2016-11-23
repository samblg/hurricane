#include "sample/wordcount/HelloWorldSpout.h"
#include "hurricane/util/StringUtil.h"
#include <sys/time.h>
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
    timeval currentTime;
    gettimeofday(&currentTime, nullptr);

    int64_t currentMicroseconds = currentTime.tv_sec;
    currentMicroseconds *= 1000000;
    currentMicroseconds += currentTime.tv_usec;

    ++ id;

    std::vector<std::string> words(5);
    for ( int i = 0; i < 5; i ++ ) {
        words[i] = _words[rand() % _words.size()];
    }

    std::string sentence = JoinStrings(words);
	_outputCollector->Emit({
        sentence, currentMicroseconds, id
    });
}
