-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-Sample_cpp.o

all: meshy-sample

meshy-sample: $(OBJS)
	@echo -e "\e[32m[LINK] \033[0mmeshy-sample"
	@$(LD) $(OBJS) -o meshy-sample -L"../meshy" -L"$(LOGGING_LIB)" -lmeshy -llogging 

install: ../../bin/meshy-sample

../../bin/meshy-sample: meshy-sample
	@echo -e "\e[32m[INSTALL] \033[0mmeshy-sample -> ../../bin"
	@cp meshy-sample ../../bin

clean:
	@echo -e "\e[32m[CLEAN] \033[0mmeshy-sample"
	@rm -f meshy-sample
	@rm -f *.o

__-__-__-__-__-__-src-Sample_cpp.o: ../../../../../../src/Sample.cpp \
 ../../../../../../include/Net.h \
 ../../../../../../include/linux/NetLinux.h \
 ../../../../../../include/ByteArray.h \
 ../../../../../../include/utils/Exendian.h \
 ../../../../../../include/EventQueue.h \
 ../../../../../../include/EventQueueLoop.h \
 ../../../../../../include/Loop.h ../../../../../../include/IoLoop.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/NetLinux.h \
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
 ../../../../../../include/Meshy.h \
 ../../../../../../include/epoll/EPollClient.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/epoll/EPollServer.h \
 ../../../../../../include/epoll/EPollStream.h \
 ../../../../../../include/rest/HttpServer.h \
 ../../../../../../include/rest/HttpConnection.h \
 ../../../../../../include/rest/HttpRequest.h \
 ../../../../../../include/rest/HttpContext.h \
 ../../../../../../include/rest/HttpResponse.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/Sample.cpp -> $@"
	@$(CXX) ../../../../../../src/Sample.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

