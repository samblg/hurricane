-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-PackageDataSink_cpp.o \
__-__-__-__-__-__-src-eventqueueloop_cpp.o \
__-__-__-__-__-__-src-net_cpp.o \
__-__-__-__-__-__-src-epoll-EPollClient_cpp.o \
__-__-__-__-__-__-src-epoll-EPollConnection_cpp.o \
__-__-__-__-__-__-src-epoll-EPollServer_cpp.o \
__-__-__-__-__-__-src-epoll-EPollStream_cpp.o \
__-__-__-__-__-__-src-epoll-EpollLoop_cpp.o \
__-__-__-__-__-__-src-utils-String_cpp.o \
__-__-__-__-__-__-src-utils-common__utils_cpp.o \
__-__-__-__-__-__-src-utils-logger_cpp.o \
__-__-__-__-__-__-src-utils-thread__pool_cpp.o \
__-__-__-__-__-__-src-utils-time_cpp.o \
__-__-__-__-__-__-src-http-HttpConnection_cpp.o \
__-__-__-__-__-__-src-http-HttpContext_cpp.o \
__-__-__-__-__-__-src-http-HttpRequest_cpp.o \
__-__-__-__-__-__-src-http-HttpResponse_cpp.o \
__-__-__-__-__-__-src-http-HttpServer_cpp.o

all: libmeshy.so

libmeshy.so: $(OBJS)
	$(LD) $(OBJS) -shared -o libmeshy.so   -lpthread

install: ../../../../../lib/linux/x64/Release/libmeshy.so

../../../../../lib/linux/x64/Release/libmeshy.so: libmeshy.so
	cp libmeshy.so ../../../../../lib/linux/x64/Release

clean:
	rm -f libmeshy.so
	rm -f *.o

__-__-__-__-__-__-src-PackageDataSink_cpp.o: ../../../../../../src/PackageDataSink.cpp \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/eventqueue.h \
 ../../../../../../include/utils/logger.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpResponse.h
	$(CXX) ../../../../../../src/PackageDataSink.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-eventqueueloop_cpp.o: ../../../../../../src/eventqueueloop.cpp \
 ../../../../../../include/eventqueueloop.h \
 ../../../../../../include/loop.h ../../../../../../include/eventqueue.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h
	$(CXX) ../../../../../../src/eventqueueloop.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-net_cpp.o: ../../../../../../src/net.cpp ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h
	$(CXX) ../../../../../../src/net.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-epoll-EPollClient_cpp.o: ../../../../../../src/epoll/EPollClient.cpp \
 ../../../../../../include/utils/logger.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/common.h \
 ../../../../../../include/utils/common_utils.h
	$(CXX) ../../../../../../src/epoll/EPollClient.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-epoll-EPollConnection_cpp.o: ../../../../../../src/epoll/EPollConnection.cpp \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/common.h \
 ../../../../../../include/utils/logger.h
	$(CXX) ../../../../../../src/epoll/EPollConnection.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-epoll-EPollServer_cpp.o: ../../../../../../src/epoll/EPollServer.cpp \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/loop.h \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/common.h \
 ../../../../../../include/utils/common_utils.h \
 ../../../../../../include/utils/logger.h
	$(CXX) ../../../../../../src/epoll/EPollServer.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-epoll-EPollStream_cpp.o: ../../../../../../src/epoll/EPollStream.cpp \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/common.h \
 ../../../../../../include/utils/logger.h
	$(CXX) ../../../../../../src/epoll/EPollStream.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-epoll-EpollLoop_cpp.o: ../../../../../../src/epoll/EpollLoop.cpp \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/common.h \
 ../../../../../../include/utils/logger.h
	$(CXX) ../../../../../../src/epoll/EpollLoop.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-utils-String_cpp.o: ../../../../../../src/utils/String.cpp \
 ../../../../../../include/utils/String.h
	$(CXX) ../../../../../../src/utils/String.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-utils-common__utils_cpp.o: ../../../../../../src/utils/common_utils.cpp \
 ../../../../../../include/utils/common_utils.h
	$(CXX) ../../../../../../src/utils/common_utils.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-utils-logger_cpp.o: ../../../../../../src/utils/logger.cpp \
 ../../../../../../include/utils/logger.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/utils/time.h
	$(CXX) ../../../../../../src/utils/logger.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-utils-thread__pool_cpp.o: ../../../../../../src/utils/thread_pool.cpp \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/utils/logger.h
	$(CXX) ../../../../../../src/utils/thread_pool.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-utils-time_cpp.o: ../../../../../../src/utils/time.cpp \
 ../../../../../../include/utils/time.h
	$(CXX) ../../../../../../src/utils/time.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-http-HttpConnection_cpp.o: ../../../../../../src/http/HttpConnection.cpp \
 ../../../../../../include/Meshy.h ../../../../../../include/IoLoop.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/common.h \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/rest/HttpConnection.h \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/rest/HttpResponse.h
	$(CXX) ../../../../../../src/http/HttpConnection.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-http-HttpContext_cpp.o: ../../../../../../src/http/HttpContext.cpp \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/utils/String.h
	$(CXX) ../../../../../../src/http/HttpContext.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-http-HttpRequest_cpp.o: ../../../../../../src/http/HttpRequest.cpp \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/utils/String.h
	$(CXX) ../../../../../../src/http/HttpRequest.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-http-HttpResponse_cpp.o: ../../../../../../src/http/HttpResponse.cpp \
 ../../../../../../include/rest/HttpResponse.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/utils/String.h
	$(CXX) ../../../../../../src/http/HttpResponse.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

__-__-__-__-__-__-src-http-HttpServer_cpp.o: ../../../../../../src/http/HttpServer.cpp \
 ../../../../../../include/rest/HttpServer.h \
 ../../../../../../include/Meshy.h ../../../../../../include/IoLoop.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/thread_pool.h \
 ../../../../../../include/utils/concurrent_queue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/common.h \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/rest/HttpConnection.h \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/rest/HttpResponse.h
	$(CXX) ../../../../../../src/http/HttpServer.cpp -c -o $@ -I../../../../../../include -O2 -std=c++11 -fPIC -fPIC -DOS_LINUX

