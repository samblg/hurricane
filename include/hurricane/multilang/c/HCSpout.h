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

#include "hurricane/multilang/common/Common.h"
#include "hurricane/multilang/c/HCValues.h"

BEGIN_C_DECALRE

#include <stdint.h>

typedef int32_t (*CSpoutClone)();
typedef int32_t(*CSpoutOpen)(int spoutIndex);
typedef int32_t(*CSpoutClose)(int spoutIndex);
typedef int32_t(*CSpoutExecute)(int spoutIndex, void* wrapper, void* emitter);

typedef struct {
    CSpoutClone onClone;
    CSpoutOpen onOpen;
    CSpoutClose onClose;
    CSpoutExecute onExecute;
} CSpout;

void TestCSpout(CSpout* cSpout);

END_C_DECLARE

#ifdef __cplusplus

#include "hurricane/base/Values.h"
#include "hurricane/base/Fields.h"
#include "hurricane/spout/ISpout.h"

namespace hurricane {
    namespace base {
        class OutputCollector;
    }

    namespace spout {
        class CSpoutWrapper : public ISpout {
        public:
            CSpoutWrapper(const CSpout* cSpout);
            virtual base::Fields DeclareFields() const override;
            virtual void Open(base::OutputCollector & outputCollector) override;
            virtual void Close() override;
            virtual void Execute() override;
            virtual ISpout * Clone() const override;
            static void Emit(CSpoutWrapper* spout, CValues* cValues);

        private:
            CSpout _cSpout;
            int _spoutIndex;
            base::OutputCollector* _collector;
        };
    }
}

#endif
