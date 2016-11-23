-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-client__sample_cpp.o

all: meshy-client-sample

meshy-client-sample: $(OBJS)
	$(LD) $(OBJS) -o meshy-client-sample -L"../meshy" -lmeshy 

install: ../../../../../bin/linux/x64/Release/meshy-client-sample

../../../../../bin/linux/x64/Release/meshy-client-sample: meshy-client-sample
	cp meshy-client-sample ../../../../../bin/linux/x64/Release

clean:
	rm -f meshy-client-sample
	rm -f *.o

__-__-__-__-__-__-src-client__sample_cpp.o: ../../../../../../src/client_sample.cpp \
 ../../../../../../include/net.h \
 ../../../../../../include/linux/net_linux.h \
 ../../../../../../include/bytearray.h \
 ../../../../../../include/utils/exendian.h \
 ../../../../../../include/eventqueue.h \
 ../../../../../../include/eventqueueloop.h \
 ../../../../../../include/loop.h ../../../../../../include/IoLoop.h \
 ../../../../../../include/epoll/EPollLoop.h \
 ../../../../../../include/epoll/EPollConnection.h \
 ../../../../../../include/linux/net_linux.h \
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
	$(CXX) ../../../../../../src/client_sample.cpp -c -o $@ -I../../../../../../include -I../../../../../../include -O2 -std=c++11 -fPIC -DOS_LINUX

