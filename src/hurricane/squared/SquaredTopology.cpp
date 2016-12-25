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

#include "SquaredTopology.h"
#include "SquaredSpout.h"
#include "SquaredStream.h"
#include "DRPCStream.h"

namespace hurricane {
    namespace squared {
        SquaredToplogy::SquaredToplogy()
        {
        }

        SquaredStream * SquaredToplogy::NewStream(const std::string & spoutName,
            SquaredSpout * squaredSpout)
        {
            std::shared_ptr<SquaredStream> stream = 
                std::make_shared<SquaredStream>(spoutName, squaredSpout);

            _squaredStreams.push_back(stream);

            return stream.get();
        }

        DRPCStream * SquaredToplogy::NewDRPCStream(const std::string & serviceName)
        {
            std::shared_ptr<DRPCStream> stream = std::make_shared<DRPCStream>();
            _drpcStreams[serviceName] = stream;

            return stream.get();
        }

        void SquaredToplogy::Deploy()
        {
            for ( auto stream : _squaredStreams ) {
                _Deploy(stream);
            }

            for ( auto streamPair : _drpcStreams ) {
                _Deploy(stream);
            }
        }

        void SquaredToplogy::_Deploy(SquaredStream * stream)
        {
            stream->Deploy(this);
        }

        void SquaredToplogy::_Deploy(DRPCStream * stream)
        {
            stream->Deploy(this);
        }
    }
}

