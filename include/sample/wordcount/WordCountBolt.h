#pragma once

#include "hurricane/bolt/IBolt.h"

#include <map>
#include <string>
#include <cstdint>

class WordCountBolt : public hurricane::bolt::IBolt {
public:
    virtual hurricane::bolt::IBolt* Clone() override {
        return new WordCountBolt(*this);
    }
	virtual void Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) override;
	virtual void Cleanup() override;
	virtual std::vector<std::string> DeclareFields() override;
	virtual void Execute(const hurricane::base::Tuple& tuple) override;

private:
	std::shared_ptr<hurricane::collector::OutputCollector> _outputCollector;
	std::map<std::string, int32_t> _wordCounts;
};