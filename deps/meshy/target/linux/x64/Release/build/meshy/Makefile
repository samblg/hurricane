-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-PackageDataSink_cpp.o \
__-__-__-__-__-__-src-EventQueueLoop_cpp.o \
__-__-__-__-__-__-src-Net_cpp.o \
__-__-__-__-__-__-src-epoll-EPollClient_cpp.o \
__-__-__-__-__-__-src-epoll-EPollConnection_cpp.o \
__-__-__-__-__-__-src-epoll-EPollServer_cpp.o \
__-__-__-__-__-__-src-epoll-EPollStream_cpp.o \
__-__-__-__-__-__-src-epoll-EpollLoop_cpp.o \
__-__-__-__-__-__-src-utils-CommonUtils_cpp.o \
__-__-__-__-__-__-src-utils-String_cpp.o \
__-__-__-__-__-__-src-utils-ThreadPool_cpp.o \
__-__-__-__-__-__-src-utils-Time_cpp.o \
__-__-__-__-__-__-src-http-HttpConnection_cpp.o \
__-__-__-__-__-__-src-http-HttpContext_cpp.o \
__-__-__-__-__-__-src-http-HttpRequest_cpp.o \
__-__-__-__-__-__-src-http-HttpResponse_cpp.o \
__-__-__-__-__-__-src-http-HttpServer_cpp.o

all: libmeshy.so

libmeshy.so: $(OBJS)
	@echo -e "\e[32m[LINK] \033[0mlibmeshy.so"
	@$(LD) $(OBJS) -shared -o libmeshy.so -L"$(LOGGING_LIB)" -llogging -lpthread

install: ../../lib/libmeshy.so

../../lib/libmeshy.so: libmeshy.so
	@echo -e "\e[32m[INSTALL] \033[0mlibmeshy.so -> ../../lib"
	@cp libmeshy.so ../../lib

clean:
	@echo -e "\e[32m[CLEAN] \033[0mlibmeshy.so"
	@rm -f libmeshy.so
	@rm -f *.o

__-__-__-__-__-__-src-PackageDataSink_cpp.o: ../../../../../../src/PackageDataSink.cpp \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/EventQueue.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpResponse.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/PackageDataSink.cpp -> $@"
	@$(CXX) ../../../../../../src/PackageDataSink.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-EventQueueLoop_cpp.o: ../../../../../../src/EventQueueLoop.cpp \
 ../../../../../../include/EventQueueLoop.h \
 ../../../../../../include/Loop.h ../../../../../../include/EventQueue.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/EventQueueLoop.cpp -> $@"
	@$(CXX) ../../../../../../src/EventQueueLoop.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-Net_cpp.o: ../../../../../../src/Net.cpp ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/Net.cpp -> $@"
	@$(CXX) ../../../../../../src/Net.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-epoll-EPollClient_cpp.o: ../../../../../../src/epoll/EPollClient.cpp \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/Loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/Common.h \
 ../../../../../../include/utils/CommonUtils.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/epoll/EPollClient.cpp -> $@"
	@$(CXX) ../../../../../../src/epoll/EPollClient.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-epoll-EPollConnection_cpp.o: ../../../../../../src/epoll/EPollConnection.cpp \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/Loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/Common.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/epoll/EPollConnection.cpp -> $@"
	@$(CXX) ../../../../../../src/epoll/EPollConnection.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-epoll-EPollServer_cpp.o: ../../../../../../src/epoll/EPollServer.cpp \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/Loop.h \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/Common.h \
 ../../../../../../include/utils/CommonUtils.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/epoll/EPollServer.cpp -> $@"
	@$(CXX) ../../../../../../src/epoll/EPollServer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-epoll-EPollStream_cpp.o: ../../../../../../src/epoll/EPollStream.cpp \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/Loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/Common.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/epoll/EPollStream.cpp -> $@"
	@$(CXX) ../../../../../../src/epoll/EPollStream.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-epoll-EpollLoop_cpp.o: ../../../../../../src/epoll/EpollLoop.cpp \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/Loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/Common.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/epoll/EpollLoop.cpp -> $@"
	@$(CXX) ../../../../../../src/epoll/EpollLoop.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-utils-CommonUtils_cpp.o: ../../../../../../src/utils/CommonUtils.cpp \
 ../../../../../../include/utils/CommonUtils.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/utils/CommonUtils.cpp -> $@"
	@$(CXX) ../../../../../../src/utils/CommonUtils.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-utils-String_cpp.o: ../../../../../../src/utils/String.cpp \
 ../../../../../../include/utils/String.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/utils/String.cpp -> $@"
	@$(CXX) ../../../../../../src/utils/String.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-utils-ThreadPool_cpp.o: ../../../../../../src/utils/ThreadPool.cpp \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/utils/ThreadPool.cpp -> $@"
	@$(CXX) ../../../../../../src/utils/ThreadPool.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-utils-Time_cpp.o: ../../../../../../src/utils/Time.cpp \
 ../../../../../../include/utils/Time.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/utils/Time.cpp -> $@"
	@$(CXX) ../../../../../../src/utils/Time.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-http-HttpConnection_cpp.o: ../../../../../../src/http/HttpConnection.cpp \
 ../../../../../../include/Meshy.h ../../../../../../include/IoLoop.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/Loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/Common.h \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/rest/HttpConnection.h \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/rest/HttpResponse.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/http/HttpConnection.cpp -> $@"
	@$(CXX) ../../../../../../src/http/HttpConnection.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-http-HttpContext_cpp.o: ../../../../../../src/http/HttpContext.cpp \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/utils/String.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/http/HttpContext.cpp -> $@"
	@$(CXX) ../../../../../../src/http/HttpContext.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-http-HttpRequest_cpp.o: ../../../../../../src/http/HttpRequest.cpp \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/utils/String.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/http/HttpRequest.cpp -> $@"
	@$(CXX) ../../../../../../src/http/HttpRequest.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-http-HttpResponse_cpp.o: ../../../../../../src/http/HttpResponse.cpp \
 ../../../../../../include/rest/HttpResponse.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/utils/String.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/http/HttpResponse.cpp -> $@"
	@$(CXX) ../../../../../../src/http/HttpResponse.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

__-__-__-__-__-__-src-http-HttpServer_cpp.o: ../../../../../../src/http/HttpServer.cpp \
 ../../../../../../include/rest/HttpServer.h \
 ../../../../../../include/Meshy.h ../../../../../../include/IoLoop.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/Loop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/PackageDataSink.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/utils/ThreadPool.h \
 ../../../../../../include/utils/ConcurrentQueue.h \
 ../../../../../../include/template/utils/thread_pool.tcc \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/DataSink.h \
 ../../../../../../include/linux/Common.h \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/rest/HttpConnection.h \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/rest/HttpResponse.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/http/HttpServer.cpp -> $@"
	@$(CXX) ../../../../../../src/http/HttpServer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

