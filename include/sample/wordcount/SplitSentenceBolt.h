#pragma once

#include "hurricane/bolt/IBolt.h"
#include <fstream>

class SplitSentenceBolt : public hurricane::bolt::IBolt {
public:
    virtual hurricane::bolt::IBolt* Clone() override {
        return new SplitSentenceBolt;
    }
	virtual void Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) override;
	virtual void Cleanup() override;
	virtual std::vector<std::string> DeclareFields() override;
	virtual void Execute(const hurricane::base::Tuple& tuple) override;

private:
<<<<<<< HEAD
	std::shared_ptr<hurricane::collector::OutputCollector> _outputCollector;
    std::ofstream _logFile;
=======
    std::shared_ptr<hurricane::collector::OutputCollector> _outputCollector;
>>>>>>> master
};
