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

#ifndef WIN32
#define KLOG_PREDICT_BRANCH_NOT_TAKEN(x) (__builtin_expect(x, 0))
#else
#define KLOG_PREDICT_BRANCH_NOT_TAKEN(x) x
#endif

#ifdef USE_KLOG

#define LOG(severity) (hurricane::logging::Logger( \
        (severity), \
        __FILE__, \
        __LINE__ \
    ).Reference())

#define LOG_IF(severity, expr) !(KLOG_PREDICT_BRANCH_NOT_TAKEN(!(!(expr)))) ? \
    (void) 0 : \
    hurricane::logging::LogMessageVoidify() & (hurricane::logging::ExpressionLogger( \
            (severity), \
            __FILE__, \
            __LINE__, \
            #expr \
    ).Reference())


#define LOG_CHECK(expr) !(KLOG_PREDICT_BRANCH_NOT_TAKEN(!(expr))) ? \
    (void) 0 : \
    hurricane::logging::LogMessageVoidify() & hurricane::logging::CheckLogger( \
        LOG_FATAL, \
        __FILE__, \
        __LINE__, \
        #expr \
    ).Reference()

#ifndef NDEBUG
#define DLOG(severity) LOG(severity)
#define DLOG_IF(severity, expr) LOG_IF(severity, expr)
#define DLOG_CHECK(expr) LOG_CHECK(expr)
#else // NDEBUG
#define DLOG(severity) !(KLOG_PREDICT_BRANCH_NOT_TAKEN(0)) ? \
    (void) 0 : hurricane::logging::LogMessageVoidify() & LOG(severity)
#define DLOG_IF(severity, expr) DLOG(severity)
#define DLOG_CHECK(expr) DLOG(LOG_FATAL)
#endif // NDEBUG

#else // USE_KLOG

#define LOG(severity) !(KLOG_PREDICT_BRANCH_NOT_TAKEN(0)) ? \
    (void) 0 : \
    hurricane::logging::LogMessageVoidify() & (hurricane::logging::Logger( \
        (severity), \
        __FILE__, \
        __LINE__ \
    ).Reference())
#define LOG_IF(severity, expr) LOG(severity)
#define LOG_CHECK(expr) LOG(KLOG_FATAL)

#define DLOG(severity) LOG(severity)
#define DLOG_IF(severity, expr) LOG(severity)
#define DLOG_CHECK(expr) LOG(KLOG_FATAL)

#endif /// USE_KLOG
