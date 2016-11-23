#pragma once

#include "hurricane/task/TaskDeclarer.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

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

<<<<<<< HEAD
=======
            const std::vector<std::string>& GetFields() const {
                return _fields;
            }

            const std::map<std::string, int>& GetFieldsMap() const {
                return _fieldsMap;
            }

>>>>>>> master
		private:
			std::shared_ptr<ISpout> _spout;
            std::vector<std::string> _fields;
            std::map<std::string, int> _fieldsMap;
		};
	}
}
