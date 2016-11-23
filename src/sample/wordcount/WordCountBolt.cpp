#include "sample/wordcount/WordCountBolt.h"
#include "hurricane/util/StringUtil.h"
#include <sys/time.h>
#include <sstream>

std::string itos(int number) {
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
<<<<<<< HEAD
=======
    int64_t sourceMicroseconds = tuple[1].GetInt64Value();
    int32_t id = tuple[2].GetInt32Value();
>>>>>>> master

	auto wordCountIterator = _wordCounts.find(word);
	if ( wordCountIterator == _wordCounts.end() ) {
		_wordCounts.insert({ word, 0 });
		wordCountIterator = _wordCounts.find(word);
	}

	wordCountIterator->second ++;

    std::cout << word << ' ' << wordCountIterator->second << std::endl;
	_outputCollector->Emit({ word, wordCountIterator->second });
<<<<<<< HEAD
=======

    timeval currentTime;
    gettimeofday(&currentTime, nullptr);

    int64_t currentMicroseconds = currentTime.tv_sec;
    currentMicroseconds *= 1000000;
    currentMicroseconds += currentTime.tv_usec;

    *_logFile << sourceMicroseconds << ' ' << currentMicroseconds << std::endl;
>>>>>>> master
}
