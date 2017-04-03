SRC = src
INCLUDE = include
TARGET = target
BUILD = $(TARGET)/build
CC = gcc
CXX = g++

CXXFLAGS = -std=c++11 -I$(INCLUDE) -DOS_LINUX -g -fPIC
LDFALGS = -lpthread

OBJECTS = $(BUILD)/PackageDataSink.o \
          $(BUILD)/EPollConnection.o \
          $(BUILD)/EPollStream.o \
          $(BUILD)/EPollClient.o \
          $(BUILD)/EPollServer.o \
          $(BUILD)/EPollLoop.o \
          $(BUILD)/eventqueueloop.o \
          $(BUILD)/net.o \
          $(BUILD)/logger.o \
          $(BUILD)/time.o \
          $(BUILD)/thread_pool.o \
          $(BUILD)/common_utils.o \
          $(BUILD)/String.o \
          $(BUILD)/HttpContext.o \
          $(BUILD)/HttpRequest.o \
          $(BUILD)/HttpResponse.o \
          $(BUILD)/HttpServer.o \
          $(BUILD)/HttpConnection.o

OBJECTS_SAMPLE = $(BUILD)/sample.o \
          $(BUILD)/PackageDataSink.o \
          $(BUILD)/EPollConnection.o \
          $(BUILD)/EPollStream.o \
          $(BUILD)/EPollClient.o \
          $(BUILD)/EPollServer.o \
          $(BUILD)/EPollLoop.o \
          $(BUILD)/eventqueueloop.o \
          $(BUILD)/net.o \
          $(BUILD)/logger.o \
          $(BUILD)/time.o \
          $(BUILD)/thread_pool.o \
          $(BUILD)/common_utils.o \
          $(BUILD)/String.o \
          $(BUILD)/HttpContext.o \
          $(BUILD)/HttpRequest.o \
          $(BUILD)/HttpResponse.o \
          $(BUILD)/HttpServer.o \
          $(BUILD)/HttpConnection.o

OBJECTS_CLIENT = $(BUILD)/client_sample.o \
          $(BUILD)/PackageDataSink.o \
          $(BUILD)/EPollConnection.o \
          $(BUILD)/EPollStream.o \
          $(BUILD)/EPollClient.o \
          $(BUILD)/EPollServer.o \
          $(BUILD)/EPollLoop.o \
          $(BUILD)/eventqueueloop.o \
          $(BUILD)/net.o \
          $(BUILD)/logger.o \
          $(BUILD)/time.o \
          $(BUILD)/thread_pool.o \
          $(BUILD)/common_utils.o \
          $(BUILD)/String.o \
          $(BUILD)/HttpContext.o \
		  $(BUILD)/HttpRequest.o \
		  $(BUILD)/HttpResponse.o \
          $(BUILD)/HttpConnection.o

all: $(TARGET)/sample $(TARGET)/client_sample $(TARGET)/libmeshy.so

clean:
	rm -rf $(TARGET)/*
	mkdir $(BUILD)

$(TARGET)/libmeshy.so: $(OBJECTS)
	mkdir -pv $(TARGET)
	$(CXX) -o $@ $(OBJECTS) $(LDFALGS) -shared

$(TARGET)/sample: $(OBJECTS_SAMPLE)
	mkdir -pv $(TARGET)
	$(CXX) -o $@ $(OBJECTS_SAMPLE) $(LDFALGS)

$(TARGET)/client_sample: $(OBJECTS_CLIENT)
	mkdir -pv $(TARGET)
	$(CXX) -o $@ $(OBJECTS_CLIENT) $(LDFALGS)

$(BUILD)/sample.o: $(SRC)/sample.cpp $(INCLUDE)/net.h $(INCLUDE)/eventqueue.h $(INCLUDE)/eventqueueloop.h \
		$(INCLUDE)/PackageDataSink.h $(INCLUDE)/DataSink.h $(INCLUDE)/bytearray.h \
		$(INCLUDE)/rest/HttpServer.h $(INCLUDE)/rest/HttpConnection.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/sample.cpp

$(BUILD)/client_sample.o: $(SRC)/client_sample.cpp $(INCLUDE)/net.h $(INCLUDE)/eventqueue.h $(INCLUDE)/eventqueueloop.h \
		$(INCLUDE)/PackageDataSink.h $(INCLUDE)/DataSink.h $(INCLUDE)/bytearray.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/client_sample.cpp

$(BUILD)/PackageDataSink.o: $(SRC)/PackageDataSink.cpp $(INCLUDE)/PackageDataSink.h \
		$(INCLUDE)/eventqueue.h $(INCLUDE)/DataSink.h $(INCLUDE)/bytearray.h \
		$(INCLUDE)/utils/thread_pool.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/PackageDataSink.cpp

$(BUILD)/EPollServer.o: $(SRC)/epoll/EPollServer.cpp $(INCLUDE)/epoll/EPollServer.h \
		$(INCLUDE)/linux/net_linux.h \
		$(INCLUDE)/net.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/epoll/EPollServer.cpp

$(BUILD)/EPollClient.o: $(SRC)/epoll/EPollClient.cpp $(INCLUDE)/epoll/EPollClient.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/epoll/EPollClient.cpp

$(BUILD)/EPollConnection.o: $(SRC)/epoll/EPollConnection.cpp $(INCLUDE)/epoll/EPollConnection.h \
		$(INCLUDE)/linux/net_linux.h \
		$(INCLUDE)/net.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/epoll/EPollConnection.cpp

$(BUILD)/EPollStream.o: $(SRC)/epoll/EPollStream.cpp $(INCLUDE)/epoll/EPollStream.h \
		$(INCLUDE)/linux/net_linux.h \
		$(INCLUDE)/net.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/epoll/EPollStream.cpp

$(BUILD)/EPollLoop.o: $(SRC)/epoll/EPollLoop.cpp $(INCLUDE)/epoll/EPollLoop.h \
		$(INCLUDE)/loop.h $(INCLUDE)/DataSink.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/epoll/EPollLoop.cpp

$(BUILD)/eventqueueloop.o: $(SRC)/eventqueueloop.cpp $(INCLUDE)/eventqueueloop.h \
		$(INCLUDE)/eventqueue.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/eventqueueloop.cpp

$(BUILD)/net.o: $(SRC)/net.cpp $(INCLUDE)/net.h $(INCLUDE)/linux/net_linux.h $(INCLUDE)/bytearray.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/net.cpp

$(BUILD)/logger.o: $(SRC)/utils/logger.cpp $(INCLUDE)/utils/logger.h \
		$(INCLUDE)/utils/concurrent_queue.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/utils/logger.cpp

$(BUILD)/time.o: $(SRC)/utils/time.cpp $(INCLUDE)/utils/time.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/utils/time.cpp

$(BUILD)/thread_pool.o: $(SRC)/utils/thread_pool.cpp $(INCLUDE)/utils/thread_pool.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/utils/thread_pool.cpp

$(BUILD)/common_utils.o: $(SRC)/utils/common_utils.cpp $(INCLUDE)/utils/common_utils.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/utils/common_utils.cpp

$(BUILD)/String.o: $(SRC)/utils/String.cpp $(INCLUDE)/utils/String.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/utils/String.cpp

$(BUILD)/HttpContext.o: $(SRC)/http/HttpContext.cpp $(INCLUDE)/rest/HttpContext.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/http/HttpContext.cpp

$(BUILD)/HttpRequest.o: $(SRC)/http/HttpRequest.cpp $(INCLUDE)/rest/HttpRequest.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/http/HttpRequest.cpp

$(BUILD)/HttpResponse.o: $(SRC)/http/HttpResponse.cpp $(INCLUDE)/rest/HttpResponse.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/http/HttpResponse.cpp

$(BUILD)/HttpServer.o: $(SRC)/http/HttpServer.cpp $(INCLUDE)/rest/HttpServer.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/http/HttpServer.cpp

$(BUILD)/HttpConnection.o: $(SRC)/http/HttpConnection.cpp $(INCLUDE)/rest/HttpConnection.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRC)/http/HttpConnection.cpp
