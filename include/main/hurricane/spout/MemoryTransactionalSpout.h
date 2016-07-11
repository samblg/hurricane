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

#include "TransactionalSpout.h"

namespace hurricane {
	namespace spout {
		class MemoryCoordinator : public Coordinator {
		public:
			MemoryCoordinator() {}

			virtual void Open(base::OutputCollector& outputCollector) {
				Coordinator::Open(outputCollector);
			}

			virtual void Close() {

			}

			virtual void Execute() {

			}

			virtual ISpout* Clone() const override {
				return new MemoryCoordinator(*this);
			}

			virtual base::Fields DeclareFields() const override {
				return {};
			}
		};

		class MemoryEmitter : public bolt::Emitter {
		public:
			MemoryEmitter() {}

			virtual void Prepare(base::OutputCollector& outputCollector) {
				bolt::Emitter::Prepare(outputCollector);

			}

			virtual void Cleanup() {

			}

			virtual void Execute(const base::Values& values) {

			}

			virtual IBolt* Clone() const {
				return new MemoryEmitter(*this);
			}

			virtual base::Fields DeclareFields() const {
				return {};
			}
		};

		class MemoryTransactionalSpout : public TransactionalSpout {
		public:
			MemoryTransactionalSpout() :
				_coordinator(new MemoryCoordinator), _emitter(new MemoryEmitter) {

			}

			virtual hurricane::spout::Coordinator* GetCoordinator() {
				return _coordinator;
			}

			virtual hurricane::bolt::Emitter* GetEmitter() {
				return _emitter;
			}

		private:
			MemoryCoordinator* _coordinator;
			MemoryEmitter* _emitter;
		};
	}
}