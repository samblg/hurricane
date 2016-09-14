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
#if ( HURRICANE_MODE == HURRICANE_RELEASE ) 

#include "SpoutOutputCollector.h"
#include "hurricane/spout/SpoutExecutor.h"

void hurricane::spout::SpoutOutputCollector::RandomDestination()
{
	_executor->RandomDestination(this);
}

void hurricane::spout::SpoutOutputCollector::GroupDestination()
{
	_executor->GroupDestination(this, GetFieldIndex());
}

void hurricane::spout::SpoutOutputCollector::Emit(const base::Values & values, int msgId)
{
}

#endif