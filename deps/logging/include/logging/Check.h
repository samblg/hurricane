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

#include <cstdlib>

#define CHECK(expr) LOG_CHECK(expr)

#define CHECK_OP(expr1, op, expr2) CHECK((expr1) op (expr2))

#define CHECK_EQ(expr1, expr2) CHECK_OP(expr1, ==, expr2)
#define CHECK_NE(expr1, expr2) CHECK_OP(expr1, !=, expr2)
#define CHECK_LE(expr1, expr2) CHECK_OP(expr1, <=, expr2)
#define CHECK_LT(expr1, expr2) CHECK_OP(expr1, <, expr2)
#define CHECK_GE(expr1, expr2) CHECK_OP(expr1, >=, expr2)
#define CHECK_GT(expr1, expr2) CHECK_OP(expr1, >, expr2)

#define CHECK_NOTNULL(expr) CHECK(expr); expr

#define DCHECK(expr) DLOG_CHECK(expr)

#define DCHECK_OP(expr1, op, expr2) DCHECK(expr1 op expr2)

#define DCHECK_EQ(expr1, expr2) DCHECK_OP(expr1, ==, expr2)
#define DCHECK_NE(expr1, expr2) DCHECK_OP(expr1, !=, expr2)
#define DCHECK_LE(expr1, expr2) DCHECK_OP(expr1, <=, expr2)
#define DCHECK_LT(expr1, expr2) DCHECK_OP(expr1, <, expr2)
#define DCHECK_GE(expr1, expr2) DCHECK_OP(expr1, >=, expr2)
#define DCHECK_GT(expr1, expr2) DCHECK_OP(expr1, >, expr2)

