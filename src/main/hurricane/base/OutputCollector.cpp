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

#include "hurricane/base/OutputCollector.h"
#include "hurricane/topology/ITopology.h"

#include <iostream>

namespace hurricane {
namespace base {

void OutputCollector::Emit(const Values& values) {
	if ( _strategy == Strategy::Global ) {
		if ( _commander ) {
			_commander->SendTuple(this->_taskIndex, values);
		}
	}
	else if ( _strategy == Strategy::Random ) {
		this->RandomDestination();
        _commander->SendTuple(this->_taskIndex, values);
	}
	else if ( _strategy == Strategy::Group ) {
		this->GroupDestination();
        _commander->SendTuple(this->_taskIndex, values);
	}
}

}
}