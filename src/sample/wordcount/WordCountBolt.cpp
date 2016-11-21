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
    std::string word = tuple[0].GetStringValue();

	auto wordCountIterator = _wordCounts.find(word);
	if ( wordCountIterator == _wordCounts.end() ) {
		_wordCounts.insert({ word, 0 });
		wordCountIterator = _wordCounts.find(word);
	}

	wordCountIterator->second ++;

	_outputCollector->Emit({ word, wordCountIterator->second });
}
