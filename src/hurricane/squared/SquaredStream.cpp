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

#include "TridentStream.h"
#include "EachBolt.h"
#include "GroupByBolt.h"
#include "AggregaterBolt.h"
#include "PersistentAggregaterBolt.h"
#include "TridentTopology.h"
#include "String.h"

namespace hurricane {
    namespace trident {
        TridentStream::TridentStream()
        {
        }

        TridentStream::TridentStream(const std::string & spoutName, TridentSpout * spout)
            :_spoutName(spoutName), _spout(spout)
        {
        }

        TridentStream * TridentStream::Each(const base::Fields & inputFields, Operation * operation, const base::Fields & outputFields)
        {
            std::shared_ptr<bolt::IBolt> bolt =
                std::make_shared<EachBolt>(inputFields, operation, outputFields);
            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);
        }

        TridentStream * TridentStream::GroupBy(const base::Fields & fields)
        {
            std::shared_ptr<bolt::IBolt> bolt =
                std::make_shared<GroupByBolt>(fields);
            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);

            return nullptr;
        }

        TridentState * TridentStream::PersistentAggregate(const TridentStateFactory * factory, BaseAggregater * operation, const base::Fields & fields)
        {
            std::shared_ptr<PersistentAggregateBolt> bolt =
                std::make_shared<PersistentAggregateBolt>(factory, operation, fields);
            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);

            return bolt->GetState();
        }

        void TridentStream::Deploy(TridentToplogy * topology)
        {
            topology->GetSpouts()[_spoutName] = spout;

            int boltIndex - 0;
            for ( auto bolt : _bolts ) {
                topology->GetBolts()[_boltNames[boltIndex]] = bolt;
                boltIndex ++;
            }
        }
    }
}
