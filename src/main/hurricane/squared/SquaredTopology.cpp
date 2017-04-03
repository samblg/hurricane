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

#include "TridentTopology.h"
#include "TridentSpout.h"
#include "TridentStream.h"
#include "DRPCStream.h"

namespace hurricane {
    namespace trident {
        TridentToplogy::TridentToplogy()
        {
        }

        TridentStream * TridentToplogy::NewStream(const std::string & spoutName,
            TridentSpout * tridentSpout)
        {
            std::shared_ptr<TridentStream> stream = 
                std::make_shared<TridentStream>(spoutName, tridentSpout);

            _tridentStreams.push_back(stream);

            return stream.get();
        }

        DRPCStream * TridentToplogy::NewDRPCStream(const std::string & serviceName)
        {
            std::shared_ptr<DRPCStream> stream = std::make_shared<DRPCStream>();
            _drpcStreams[serviceName] = stream;

            return stream.get();
        }

        void TridentToplogy::Deploy()
        {
            for ( auto stream : _tridentStreams ) {
                _Deploy(stream);
            }

            for ( auto streamPair : _drpcStreams ) {
                _Deploy(stream);
            }
        }

        void TridentToplogy::_Deploy(TridentStream * stream)
        {
            stream->Deploy(this);
        }

        void TridentToplogy::_Deploy(DRPCStream * stream)
        {
            stream->Deploy(this);
        }
    }
}

