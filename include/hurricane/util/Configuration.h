#pragma once

#include <map>
#include <string>
#include <cstdint>
#include <sstream>

namespace hurricane {
	namespace util {
		class Configuration {
		public:
			const std::string& GetProperty(const std::string& propertyName) const {
				return _properties.at(propertyName);
			}

			const int32_t GetIntegerProperty(const std::string& propertyName) const {
				return std::stoi(_properties.at(propertyName));
			}

			void SetProperty(const std::string& propertyName, const std::string& propertyValue) {
				_properties[propertyName] = propertyValue;
			}

			void SetProperty(const std::string& propertyName, int propertyValue) {
				std::ostringstream os;
				os << propertyValue;
				_properties[propertyName] = os.str();
			}

			void Parse(const std::string& fileName);

		private:
			std::map<std::string, std::string> _properties;
		};
	}
}