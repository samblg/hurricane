#pragma once

#include "hurricane/task/TaskDeclarer.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace hurricane {
	namespace bolt {
		class IBolt;

		class BoltDeclarer : public hurricane::task::TaskDeclarer {
		public:
			BoltDeclarer() = default;
			BoltDeclarer(const std::string& boltName, IBolt* bolt);

			BoltDeclarer& ParallismHint(int parallismHint) {
				SetParallismHint(parallismHint);

				return *this;
			}
			
			BoltDeclarer& Global(const std::string& sourceTaskName) {
				SetSourceTaskName(sourceTaskName);
				SetGroupMethod(hurricane::task::TaskDeclarer::GroupMethod::Global);

				return *this;
            }

            BoltDeclarer& Field(const std::string& sourceTaskName, const std::string& groupField) {
				SetSourceTaskName(sourceTaskName);
				SetGroupMethod(hurricane::task::TaskDeclarer::GroupMethod::Field);
				SetGroupField(groupField);

				return *this;
			}

            BoltDeclarer& Random(const std::string& sourceTaskName) {
                SetSourceTaskName(sourceTaskName);
                SetGroupMethod(hurricane::task::TaskDeclarer::GroupMethod::Random);

                return *this;
            }

			const std::string& GetGroupField() const {
				return _groupField;
			}

			void SetGroupField(const std::string& groupField) {
				_groupField = groupField;
			}

            std::shared_ptr<IBolt> GetBolt() const {
                return _bolt;
            }

            const std::vector<std::string>& GetFields() const {
                return _fields;
            }

            const std::map<std::string, int>& GetFieldsMap() const {
                return _fieldsMap;
            }

		private:
			std::shared_ptr<IBolt> _bolt;
			std::string _groupField;
            std::vector<std::string> _fields;
            std::map<std::string, int> _fieldsMap;
		};
	}
}
