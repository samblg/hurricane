cmake_minimum_required(VERSION 3.3)
project(net_framework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/bsd/net_bsd.h
    include/epoll/EPollClient.h
    include/epoll/EPollConnection.h
    include/epoll/epollloop.h
    include/epoll/EPollServer.h
    include/epoll/EPollStream.h
    include/iocp/IOCPClient.h
    include/iocp/IOCPConnection.h
    include/iocp/IOCPLoop.h
    include/iocp/IOCPServer.h
    include/iocp/IOCPStream.h
    include/kqueue/kqueue.h
    include/kqueue/kqueueloop.h
    include/linux/common.h
    include/linux/net_linux.h
    include/template/utils/thread_pool.tcc
    include/utils/common_utils.h
    include/utils/concurrent_queue.h
    include/utils/logger.h
    include/utils/thread_pool.h
    include/utils/time.h
    include/utils/String.h
    include/win32/net_win32.h
    include/bytearray.h
    include/DataSink.h
    include/eventqueue.h
    include/eventqueueloop.h
    include/IoLoop.h
    include/loop.h
    include/net.h
    include/PackageDataSink.h
    include/Meshy.h
    src/epoll/EPollClient.cpp
    src/epoll/EPollConnection.cpp
    src/epoll/epollloop.cpp
    src/epoll/EPollServer.cpp
    src/epoll/EPollStream.cpp
    src/iocp/IOCPClient.cpp
    src/iocp/IOCPConnection.cpp
    src/iocp/iocploop.cpp
    src/iocp/IOCPServer.cpp
    src/iocp/IOCPStream.cpp
    src/kqueue/kqueue.cpp
    src/kqueue/kqueueloop.cpp
    src/utils/common_utils.cpp
    src/utils/logger.cpp
    src/utils/thread_pool.cpp
    src/utils/time.cpp
    src/utils/String.cpp
    src/win32/net_win32.cpp
    src/client_sample.cpp
    src/eventqueueloop.cpp
    src/net.cpp
    src/PackageDataSink.cpp
    src/sample.cpp
    CMakeLists.txt
    Makefile.bsd
    Makefile.linux include/utils/exendian.h include/rest/HttpServer.h include/rest/HttpRequest.h include/rest/HttpResponse.h include/rest/HttpClient.h include/rest/HttpDataSink.h include/rest/HttpContext.h src/http/HttpResponse.cpp src/http/HttpRequest.cpp include/rest/HttpConnection.h src/http/HttpServer.cpp src/http/HttpConnection.cpp)

include_directories(include)
include_directories(include/bsd)
include_directories(include/epoll)
include_directories(include/iocp)
include_directories(include/kqueue)
include_directories(include/linux)
include_directories(include/template)
include_directories(include/utils)
include_directories(include/win32)

add_executable(net_framework ${SOURCE_FILES})
