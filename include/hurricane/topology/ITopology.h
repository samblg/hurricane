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

#include "hurricane/base/Values.h"

#include <string>
#include <memory>
#include <map>

namespace hurricane {

namespace spout {
    class ISpout;
}

namespace bolt {
    class IBolt;
}

namespace topology {

class ITopology {
public:
    ~ITopology () {}

    virtual void SetSpouts(std::map<std::string, std::shared_ptr<spout::ISpout>> spouts) = 0;
    virtual void SetBolts(std::map<std::string, std::shared_ptr<bolt::IBolt>> bolts) = 0;
    virtual void SetNetwork(std::map<std::string, std::vector<std::string>> network) = 0;
    virtual void EmitFrom(const std::string& src, const base::Values& values) = 0;

	virtual const std::map<std::string, std::shared_ptr<spout::ISpout>>& GetSpouts() const = 0;
	virtual const std::map<std::string, std::shared_ptr<bolt::IBolt>>& GetBolts() const = 0;
	virtual const std::map<std::string, std::vector<std::string>>& GetNetwork() const = 0;

    virtual void Start() = 0;
};

}

}
