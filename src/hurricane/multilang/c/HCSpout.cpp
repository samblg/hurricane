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

#include "hurricane/multilang/c/HCSpout.h"
#include "hurricane/base/Values.h"
#include "hurricane/base/Fields.h"
#include "hurricane/base/OutputCollector.h"
#include "hurricane/spout/ISpout.h"
#include "logging/Logging.h"

#include <iostream>

using hurricane::base::Fields;
using hurricane::base::Values;
using hurricane::base::OutputCollector;
using hurricane::spout::ISpout;

namespace hurricane {
    namespace spout {
        CSpoutWrapper::CSpoutWrapper(const CSpout* cSpout) : _cSpout(*cSpout),
            _collector(nullptr) {
        }

        Fields CSpoutWrapper::DeclareFields() const
        {
            return Fields();
        }

        void CSpoutWrapper::Open(OutputCollector & outputCollector)
        {
            _collector = &outputCollector;
            _cSpout.onOpen(_spoutIndex);
        }

        void CSpoutWrapper::Close()
        {
            _cSpout.onClose(_spoutIndex);
        }

        void CSpoutWrapper::Execute()
        {
            _cSpout.onExecute(_spoutIndex, this, Emit);
        }

        ISpout * CSpoutWrapper::Clone() const
        {
            int32_t spoutIndex = _cSpout.onClone();
            CSpoutWrapper* spout = new CSpoutWrapper(&_cSpout);
            spout->_spoutIndex = spoutIndex;

            return spout;
        }

        voistd::coutrapper::Emit(CSpoutWrapper* spout, CValues* cValues) {
            LOG(LOG_DEBUG) << "Emit";
            Values values;
            CValues2Values(*cValues, &values);
            spout->_collector->Emit(values);
        }
    }
}
    
using hurricane::spout::CSpoutWrapper;

voidstd::coutut(CSpout* cSpout) {
    int32_t spoutIndex = cSpout->onClone();
    LOG(LOG_DEBUG) << "Spout index: " << spoutIndex;
    
    CSpoutWrapper* spoutWrapper = new CSpoutWrapper(cSpout);
    ISpout* spout = spoutWrapper->Clone();

    OutputCollector outputCollector("helo");
    spout->Open(outputCollector);
    spout->Execute();
    spout->Close();

    delete spout;
    delete spoutWrapper;
}
