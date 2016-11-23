-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-sample_cpp.o

all: meshy-sample

meshy-sample: $(OBJS)
	$(LD) $(OBJS) -o meshy-sample -L"../meshy" -lmeshy 

install: ../../../../../bin/linux/x64/Release/meshy-sample

../../../../../bin/linux/x64/Release/meshy-sample: meshy-sample
	cp meshy-sample ../../../../../bin/linux/x64/Release

clean:
	rm -f meshy-sample
	rm -f *.o

__-__-__-__-__-__-src-sample_cpp.o: ../../../../../../src/sample.cpp \
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
 ../../../../../../include/utils/logger.h \
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
	$(CXX) ../../../../../../src/sample.cpp -c -o $@ -I../../../../../../include -I../../../../../../include -O2 -std=c++11 -fPIC -DOS_LINUX

