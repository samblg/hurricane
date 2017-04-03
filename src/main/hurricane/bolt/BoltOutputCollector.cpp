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

#include "BoltOutputCollector.h"
#include "hurricane/bolt/BoltExecutor.h"

void Hurricane::bolt::BoltOutputCollector::RandomDestination()
{
	_executor->RandomDestination(this);
}

void Hurricane::bolt::BoltOutputCollector::GroupDestination()
{
	_executor->GroupDestination(this, GetFieldIndex());
}

void Hurricane::bolt::BoltOutputCollector::Ack(const Values & values)
{
}

void Hurricane::bolt::BoltOutputCollector::Fail(const Values & values)
{
}

#endif
