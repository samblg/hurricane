/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#pragma once

#include "hurricane/topology/ITopology.h"
#include "hurricane/spout/SpoutExecutor.h"
#include "hurricane/bolt/BoltExecutor.h"

#include <memory>
#include <map>
#include <vector>
#include <string>

namespace hurricane {
namespace topology {

class SimpleTopology : public ITopology {
public:
    SimpleTopology();

    void SetSpouts(std::map<std::string, std::shared_ptr<spout::ISpout>> spouts) override;
    void SetBolts(std::map<std::string, std::shared_ptr<bolt::IBolt>> bolts) override;
    void SetNetwork(std::map<std::string, std::vector<std::string>> network) override;
    void EmitFrom(const std::string& src, const base::Values& values) override;
    void Start() override;

	// Inherited via ITopology
	virtual const std::map<std::string, std::shared_ptr<spout::ISpout>>& GetSpouts() const override;
	virtual const std::map<std::string, std::shared_ptr<bolt::IBolt>>& GetBolts() const override;
	virtual const std::map<std::string, std::vector<std::string>>& GetNetwork() const override;

    std::map<std::string, std::shared_ptr<spout::ISpout>>& GetSpouts();
    std::map<std::string, std::shared_ptr<bolt::IBolt>>& GetBolts();
    std::map<std::string, std::vector<std::string>>& GetNetwork();

private:
    template <class TaskType, class ExecutorType>
    void allocateTask(std::map<std::string, std::shared_ptr<TaskType>>& tasks,
        std::vector<std::shared_ptr<ExecutorType>>& freeExecutors,
        std::map<std::string, std::vector<std::shared_ptr<ExecutorType>>>& allocatedExecutor);
private:
    std::map<std::string, std::shared_ptr<spout::ISpout>> _spouts;
    std::map<std::string, std::shared_ptr<bolt::IBolt>> _bolts;
    std::map<std::string, std::vector<std::string>> _network;
};

}
}
