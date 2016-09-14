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