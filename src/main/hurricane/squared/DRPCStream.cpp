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

#include "DRPCStream.h"
#include "EachBolt.h"
#include "GroupByBolt.h"
#include "AggregaterBolt.h"
#include "PersistentAggregaterBolt.h"
#include "TridentTopology.h"
#include "String.h"
#include "MapGet.h"

namespace hurricane {
    namespace trident {
        DRPCStream::DRPCStream()
        {
        }

        DRPCStream * DRPCStream::Each(const base::Fields & inputFields, Operation * operation, const base::Fields & outputFields)
        {
            std::shared_ptr<bolt::IBolt> bolt =
                std::make_shared<EachBolt>(inputFields, operation, outputFields);
            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);

            return this;
        }

        DRPCStream * DRPCStream::GroupBy(const base::Fields & fields)
        {
            std::shared_ptr<bolt::IBolt> bolt =
                std::make_shared<GroupByBolt>(fields);
            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);

            return this;
        }

        DRPCStream* DRPCStream::StateQuery(TridentState* state, base::Fields& inputFields,
            MapGet* mapGetter, base::Fields& outputFields) {
            mapGetter->SetState(state);
            std::shared_ptr<bolt::IBolt> bolt =
                std::shared_ptr<MapGet>();

            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);

            return this;
        }

        DRPCStream* DRPCStream::Aggregate(base::Fields& inputFields, BaseAggregater* aggregater,
            base::Fields* outputFields) {
            std::shared_ptr<AggregaterBolt> bolt =
                std::make_shared<AggregaterBolt>(aggregater, fields);
            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);

            return this;
        }

        std::string DRPCStream::WaitFormResult(const std::string & args)
        {
            return std::string();
        }

        std::string DRPCStream::WaitFormResult()
        {
            return std::string();
        }

        TridentState * TridentStream::PersistentAggregate(const TridentStateFactory * factory, BaseAggregater * operation, const base::Fields & fields)
        {
            std::shared_ptr<PersistentAggregateBolt> bolt =
                std::make_shared<PersistentAggregateBolt>(factory, operation, fields);
            _boltNames.push_back(RandomString("abcdedfgihjklmnopqrstuvwxyz"));
            _bolts.push_back(bolt);

            return bolt->GetState();
        }

        void DRPCStream::Deploy(TridentToplogy * topology)
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
