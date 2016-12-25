//#undef NDEBUG
//#undef USE_KLOG

#include "logging/Logging.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <thread>

using hurricane::logging::Logging;

int32_t main()
{
    using namespace std;

    Logging::SetLogFile("all.log");
    Logging::SetLogFile({ LOG_INFO, LOG_DEBUG }, "output.log");
    Logging::SetLogFile({ LOG_WARNING, LOG_ERROR, LOG_FATAL }, "error.log");

//    std::shared_ptr<int32_t> ddd;
//    CHECK(ddd);

    LOG(LOG_INFO) << "hello " << 123 << std::endl << "lkdasjflajl";
    LOG(LOG_INFO) << "hello " << 345 << std::endl << "lkdasjflajl";
    LOG(LOG_INFO) << "hello " << 456 << std::endl << "lkdasjflajl";
    LOG(LOG_INFO) << "hello " << 678 << std::endl << "lkdasjflajl";
    LOG(LOG_INFO) << "hello " << 890 << std::endl << "lkdasjflajl";
    LOG(LOG_INFO) << "hello " << 012 << std::endl << "lkdasjflajl";
    LOG_IF(LOG_INFO, 1 == 1) << "Test log if true";
    LOG_IF(LOG_INFO, 1 == 2) << "Test log if false";
    CHECK(1 == 1) << "It will pass check";
    CHECK(1 == 2) << "It will not pass check";

    DLOG(LOG_INFO) << "This is debug log";
    DLOG_IF(LOG_INFO, 1 == 1) << "Test dlog if true";
    DLOG_IF(LOG_INFO, 1 == 2) << "Test dlog if false";
    DCHECK(2 == 3) << "It will failed in debug mode";

    hurricane::logging::WaitLoggerThread();

    return 0;
}
