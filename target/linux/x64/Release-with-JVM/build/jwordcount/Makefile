-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-hurricane-collector-OutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o \
__-__-__-__-__-__-src-hurricane-base-ByteArray_cpp.o \
__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Library_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Values_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Variant_cpp.o \
__-__-__-__-__-__-src-hurricane-bolt-BoltDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-bolt-JavaBolt_cpp.o \
__-__-__-__-__-__-src-hurricane-message-Command_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandClient_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandServer_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-JavaSpout_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-SpoutDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o \
__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o \
__-__-__-__-__-__-src-sample-jwordcount-JWordCountBolt_cpp.o \
__-__-__-__-__-__-src-sample-jwordcount-JWordCountTopology_cpp.o

all: libjwordcount.so

libjwordcount.so: $(OBJS)
	@echo -e "[\e[32mLINK\033[0m] libjwordcount.so"
	@$(LD) $(OBJS) -shared -o libjwordcount.so -L"../hurricane-java" $(LOGGING_LIB) $(JNI_LIB) -lhurricane-java -llogging -ldl -ljvm 

install: ../../lib/libjwordcount.so

../../lib/libjwordcount.so: libjwordcount.so
	@echo -e "[\e[32mINSTALL\033[0m] jwordcount/libjwordcount.so -> ../../lib"
	@cp libjwordcount.so ../../lib

clean:
	@echo -e "[\e[32mCLEAN\033[0m] jwordcount/libjwordcount.so"
	@rm -f libjwordcount.so
	@echo -e "[\e[32mCLEAN\033[0m] jwordcount/*.o"
	@rm -f *.o

__-__-__-__-__-__-src-hurricane-collector-OutputCollector_cpp.o: \
 ../../../../../../src/hurricane/collector/OutputCollector.cpp \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/collector/OutputQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.tcc \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/collector/OutputItem.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/collector/OutputCollector.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/collector/OutputCollector.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o: ../../../../../../src/hurricane/task/TaskDeclarer.cpp \
 ../../../../../../include/hurricane/task/TaskDeclarer.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/task/TaskDeclarer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/TaskDeclarer.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o: ../../../../../../src/hurricane/topology/Topology.cpp \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/topology/Topology.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/topology/Topology.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-ByteArray_cpp.o: ../../../../../../src/hurricane/base/ByteArray.cpp \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/ByteArray.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/ByteArray.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o: ../../../../../../src/hurricane/base/DataPackage.cpp \
 ../../../../../../include/hurricane/base/DataPackage.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/DataPackage.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/DataPackage.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Library_cpp.o: ../../../../../../src/hurricane/base/Library.cpp \
 ../../../../../../include/hurricane/base/Library.h 
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/Library.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Library.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Values_cpp.o: ../../../../../../src/hurricane/base/Values.cpp \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/Values.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Values.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Variant_cpp.o: ../../../../../../src/hurricane/base/Variant.cpp \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/Variant.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Variant.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-bolt-BoltDeclarer_cpp.o: ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-bolt-JavaBolt_cpp.o: ../../../../../../src/hurricane/bolt/JavaBolt.cpp \
 ../../../../../../include/hurricane/bolt/JavaBolt.h \
 ../../../../../../include/hurricane/multilang/java/String.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h \
 ../../../../../../include/hurricane/multilang/java/Array.h \
 ../../../../../../include/hurricane/multilang/java/Array.tcc \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/collector/JavaOutputCollector.h \
 ../../../../../../include/hurricane/multilang/java/Signature.h \
 ../../../../../../include/hurricane/multilang/java/NativeObjectWrapper.h \
 ../../../../../../include/hurricane/multilang/java/NativeObjectWrapper.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/bolt/JavaBolt.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/bolt/JavaBolt.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-message-Command_cpp.o: ../../../../../../src/hurricane/message/Command.cpp \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/DataPackage.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/message/Command.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/message/Command.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-message-CommandClient_cpp.o: \
 ../../../../../../src/hurricane/message/CommandClient.cpp \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/message/CommandClient.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/message/CommandClient.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-message-CommandServer_cpp.o: \
 ../../../../../../src/hurricane/message/CommandServer.cpp \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/message/CommandServer.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/message/CommandServer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/message/CommandServer.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-spout-JavaSpout_cpp.o: ../../../../../../src/hurricane/spout/JavaSpout.cpp \
 ../../../../../../include/hurricane/spout/JavaSpout.h \
 ../../../../../../include/hurricane/multilang/java/String.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h \
 ../../../../../../include/hurricane/multilang/java/Array.h \
 ../../../../../../include/hurricane/multilang/java/Array.tcc \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/collector/JavaOutputCollector.h \
 ../../../../../../include/hurricane/multilang/java/Signature.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/spout/JavaSpout.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/spout/JavaSpout.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-spout-SpoutDeclarer_cpp.o: ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o: ../../../../../../src/hurricane/util/Configuration.cpp \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/Configuration.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/Configuration.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o: ../../../../../../src/hurricane/util/NetConnector.cpp \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/NetConnector.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/NetConnector.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o: ../../../../../../src/hurricane/util/NetListener.cpp \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/NetListener.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/NetListener.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o: ../../../../../../src/hurricane/util/Socket.cpp \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/Socket.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/Socket.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o: ../../../../../../src/hurricane/util/StringUtil.cpp \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/StringUtil.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/StringUtil.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-sample-jwordcount-JWordCountBolt_cpp.o: \
 ../../../../../../src/sample/jwordcount/JWordCountBolt.cpp \
 ../../../../../../include/sample/jwordcount/JWordCountBolt.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/sample/jwordcount/JWordCountBolt.cpp -> $@"
	@$(CXX) ../../../../../../src/sample/jwordcount/JWordCountBolt.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-sample-jwordcount-JWordCountTopology_cpp.o: \
 ../../../../../../src/sample/jwordcount/JWordCountTopology.cpp \
 ../../../../../../include/sample/jwordcount/JWordCountTopology.h \
 ../../../../../../include/hurricane/base/externc.h \
 ../../../../../../include/sample/jwordcount/JWordCountBolt.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/spout/JavaSpout.h \
 ../../../../../../include/hurricane/multilang/java/String.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h \
 ../../../../../../include/hurricane/multilang/java/Array.h \
 ../../../../../../include/hurricane/multilang/java/Array.tcc \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/bolt/JavaBolt.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/sample/jwordcount/JWordCountTopology.cpp -> $@"
	@$(CXX) ../../../../../../src/sample/jwordcount/JWordCountTopology.cpp -c -o $@ -I../../../../../../include -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

