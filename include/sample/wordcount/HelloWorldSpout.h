#pragma once

#include "hurricane/spout/ISpout.h"
#include <vector>

class HelloWorldSpout : public hurricane::spout::ISpout {
public:
    virtual hurricane::spout::ISpout* Clone() override {
        return new HelloWorldSpout(*this);
    }
	virtual void Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) override;
	virtual void Cleanup() override;
	virtual std::vector<std::string> DeclareFields() override;
	virtual void NextTuple() override;

private:
	std::shared_ptr<hurricane::collector::OutputCollector> _outputCollector;
    std::vector<std::string> _words;
};
