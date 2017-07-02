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

#include "hurricane/Hurricane.h"

#include "hurricane/topology/SimpleTopology.h"
#include "hurricane/spout/ISpout.h"
#include "hurricane/bolt/IBolt.h"

#include <iostream>
#include <algorithm>

namespace hurricane {
namespace topology {

SimpleTopology::SimpleTopology() {
}

void SimpleTopology::SetSpouts(std::map<std::string, std::shared_ptr<spout::ISpout>> spouts) {
    _spouts = spouts;
}

void SimpleTopology::SetBolts(std::map<std::string, std::shared_ptr<bolt::IBolt>> bolts) {
    _bolts = bolts;
}

void SimpleTopology::SetNetwork(std::map<std::string, std::vector<std::string>> network) {
    _network = network;
}

void SimpleTopology::EmitFrom(const std::string& src, const base::Values& values) {
}

void SimpleTopology::Start() {
}

const std::map<std::string, std::shared_ptr<spout::ISpout>>& SimpleTopology::GetSpouts() const
{
	return _spouts;
}

const std::map<std::string, std::shared_ptr<bolt::IBolt>>& SimpleTopology::GetBolts() const
{
	return _bolts;
}

const std::map<std::string, std::vector<std::string>>& SimpleTopology::GetNetwork() const
{
	return _network;
}

std::map<std::string, std::shared_ptr<spout::ISpout>>& SimpleTopology::GetSpouts()
{
    return _spouts;
}

std::map<std::string, std::shared_ptr<bolt::IBolt>>& SimpleTopology::GetBolts()
{
    return _bolts;
}

std::map<std::string, std::vector<std::string>>& SimpleTopology::GetNetwork()
{
    return _network;
}

template <class TaskType, class ExecutorType>
void SimpleTopology::allocateTask(std::map<std::string, std::shared_ptr<TaskType>>& tasks,
    std::vector<std::shared_ptr<ExecutorType>>& freeExecutors,
    std::map<std::string, std::vector<std::shared_ptr<ExecutorType>>>& allocatedExecutor) {
}

}
}
