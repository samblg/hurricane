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

typedef int32_t(*CBoltClone)();
typedef int32_t(*CBoltPrepare)(int boltIndex);
typedef int32_t(*CBoltCleanup)(int boltIndex);
typedef int32_t(*CBoltExecute)(int boltIndex,
    void* wrapper, void* emitter, CValues* cValues);

typedef struct {
    CBoltClone onClone;
    CBoltPrepare onPrepare;
    CBoltCleanup onCleanup;
    CBoltExecute onExecute;
} CBolt;

void TestCBolt(CBolt* cBolt);

END_C_DECLARE

#ifdef __cplusplus

#include "hurricane/base/Values.h"
#include "hurricane/base/Fields.h"
#include "hurricane/bolt/IBolt.h"

namespace hurricane {
    namespace base {
        class OutputCollector;
    }

    namespace bolt {
        class CBoltWrapper : public IBolt {
        public:
            CBoltWrapper(const CBolt* cBolt);
            virtual base::Fields DeclareFields() const;
            virtual void Prepare(base::OutputCollector & outputCollector);
            virtual void Cleanup() override;
            virtual void Execute(const base::Values& values);
            virtual bolt::IBolt * Clone() const override;

            static void Emit(CBoltWrapper* bolt, CValues* cValues);

        private:
            CBolt _cBolt;
            int _boltIndex;
            base::OutputCollector* _collector;
        };
    }
}

#endif
