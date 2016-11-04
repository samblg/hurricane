#pragma once

#include "hurricane/task/TaskDeclarer.h"
#include <memory>
#include <string>

namespace hurricane {
	namespace spout {
		class ISpout;

		class SpoutDeclarer : public hurricane::task::TaskDeclarer {
		public:
			SpoutDeclarer() = default;
			SpoutDeclarer(const std::string& spoutName, ISpout* spout);

			SpoutDeclarer& ParallismHint(int parallismHint) {
				SetParallismHint(parallismHint);

				return *this;
			}

            std::shared_ptr<ISpout> GetSpout() const {
                return _spout;
            }

		private:
			std::shared_ptr<ISpout> _spout;
		};
	}
}
