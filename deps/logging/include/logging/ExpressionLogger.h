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

#include "logging/Logger.h"

namespace hurricane {
namespace logging {

class ExpressionLogger : public Logger {
public:
    ExpressionLogger(Severity severity,
           const std::string& sourceFile, int32_t sourceLine, const std::string& expression);
    virtual ~ExpressionLogger();

    std::string GetExpression() const;
    void SetExpression(const std::string& expression);

private:
    std::string _expression;
};

}
}
