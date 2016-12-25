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

#include "hurricane/util/Configuration.h"
#include "hurricane/util/StringUtil.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

const char ITEM_SPLITTER = '=';

namespace hurricane {
    namespace util {
        void Configuration::Parse(const std::string& fileName) {
            std::ifstream inputFile(fileName.c_str());

            while ( !inputFile.eof() ) {
                std::string line;
                std::getline(inputFile, line);

                if ( inputFile.eof() ) {
                    break;
                }

                if ( line.empty() ) {
                    continue;
                }

                std::vector<std::string> words = SplitString(line, ITEM_SPLITTER);
                std::string propertyName = TrimString(words[0]);
                std::string propertyValue = TrimString(words[1]);

                SetProperty(propertyName, propertyValue);
            }
        }
    }
}
