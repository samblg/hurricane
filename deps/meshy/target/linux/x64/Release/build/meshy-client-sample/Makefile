-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-ClientSample_cpp.o

all: meshy-client-sample

meshy-client-sample: $(OBJS)
	@echo -e "\e[32m[LINK] \033[0mmeshy-client-sample"
	@$(LD) $(OBJS) -o meshy-client-sample -L"../meshy" -L"$(LOGGING_LIB)" -lmeshy -llogging 

install: ../../bin/meshy-client-sample

../../bin/meshy-client-sample: meshy-client-sample
	@echo -e "\e[32m[INSTALL] \033[0mmeshy-client-sample -> ../../bin"
	@cp meshy-client-sample ../../bin

clean:
	@echo -e "\e[32m[CLEAN] \033[0mmeshy-client-sample"
	@rm -f meshy-client-sample
	@rm -f *.o

__-__-__-__-__-__-src-ClientSample_cpp.o: ../../../../../../src/ClientSample.cpp \
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
 ../../../../../../include/linux/Common.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/ClientSample.cpp -> $@"
	@$(CXX) ../../../../../../src/ClientSample.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DOS_LINUX -DUSE_KLOG

