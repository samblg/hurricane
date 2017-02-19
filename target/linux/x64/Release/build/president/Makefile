-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-hurricane-bolt-BoltDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-collector-OutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-collector-OutputDispatcher_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-SpoutDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-tool-StartPresident_cpp.o \
__-__-__-__-__-__-src-hurricane-base-ByteArray_cpp.o \
__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Library_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Values_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Variant_cpp.o \
__-__-__-__-__-__-src-hurricane-message-Command_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandClient_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandServer_cpp.o \
__-__-__-__-__-__-src-hurricane-service-Manager_cpp.o \
__-__-__-__-__-__-src-hurricane-service-ManagerContext_cpp.o \
__-__-__-__-__-__-src-hurricane-service-President_cpp.o \
__-__-__-__-__-__-src-hurricane-task-BoltExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-Executor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-PathInfo_cpp.o \
__-__-__-__-__-__-src-hurricane-task-SpoutExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-task-TaskInfo_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-TopologyLoader_cpp.o \
__-__-__-__-__-__-src-hurricane-order-OrderBolt_cpp.o \
__-__-__-__-__-__-src-hurricane-order-OrderOutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-order-OrderSpout_cpp.o \
__-__-__-__-__-__-src-hurricane-order-OrderTuple_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o \
__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o

all: president

president: $(OBJS)
	@echo -e "[\e[32mLINK\033[0m] president"
	@$(LD) $(OBJS) -o president $(LOGGING_LIB) $(MESHY_LIB) -llogging -lmeshy -lpthread -ldl 

install: ../../bin/president

../../bin/president: president
	@echo -e "[\e[32mINSTALL\033[0m] president/president -> ../../bin"
	@cp president ../../bin

clean:
	@echo -e "[\e[32mCLEAN\033[0m] president/president"
	@rm -f president
	@echo -e "[\e[32mCLEAN\033[0m] president/*.o"
	@rm -f *.o

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
	@$(CXX) ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

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
	@$(CXX) ../../../../../../src/hurricane/collector/OutputCollector.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-collector-OutputDispatcher_cpp.o: \
 ../../../../../../src/hurricane/collector/OutputDispatcher.cpp \
 ../../../../../../include/hurricane/collector/OutputDispatcher.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc \
 ../../../../../../include/hurricane/collector/OutputQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.tcc \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/collector/OutputItem.tcc \
 ../../../../../../include/hurricane/collector/TaskQueue.h \
 ../../../../../../include/hurricane/collector/TaskItem.tcc \
 ../../../../../../include/hurricane/util/StringUtil.h \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/collector/OutputDispatcher.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/collector/OutputDispatcher.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

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
	@$(CXX) ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-tool-StartPresident_cpp.o: ../../../../../../src/hurricane/tool/StartPresident.cpp \
 ../../../../../../include/hurricane/service/President.h \
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
 ../../../../../../include/hurricane/message/CommandServer.tcc \
 ../../../../../../include/hurricane/service/ManagerContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/topology/TopologyLoader.h \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/base/Library.h \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/base/Constants.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/tool/StartPresident.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/tool/StartPresident.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-ByteArray_cpp.o: ../../../../../../src/hurricane/base/ByteArray.cpp \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/ByteArray.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/ByteArray.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o: ../../../../../../src/hurricane/base/DataPackage.cpp \
 ../../../../../../include/hurricane/base/DataPackage.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/DataPackage.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/DataPackage.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Library_cpp.o: ../../../../../../src/hurricane/base/Library.cpp \
 ../../../../../../include/hurricane/base/Library.h 
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/Library.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Library.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Values_cpp.o: ../../../../../../src/hurricane/base/Values.cpp \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/Values.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Values.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Variant_cpp.o: ../../../../../../src/hurricane/base/Variant.cpp \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/base/Variant.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Variant.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-message-Command_cpp.o: ../../../../../../src/hurricane/message/Command.cpp \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/DataPackage.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/message/Command.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/message/Command.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

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
	@$(CXX) ../../../../../../src/hurricane/message/CommandClient.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

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
	@$(CXX) ../../../../../../src/hurricane/message/CommandServer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-service-Manager_cpp.o: ../../../../../../src/hurricane/service/Manager.cpp \
 ../../../../../../include/hurricane/service/Manager.h \
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
 ../../../../../../include/hurricane/message/CommandServer.tcc \
 ../../../../../../include/hurricane/service/ManagerContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/collector/OutputDispatcher.h \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/topology/TopologyLoader.h \
 ../../../../../../include/hurricane/base/Library.h \
 ../../../../../../include/hurricane/task/SpoutExecutor.h \
 ../../../../../../include/hurricane/task/Executor.h \
 ../../../../../../include/hurricane/task/BoltExecutor.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/collector/OutputQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.tcc \
 ../../../../../../include/hurricane/collector/OutputItem.tcc \
 ../../../../../../include/hurricane/collector/TaskQueue.h \
 ../../../../../../include/hurricane/collector/TaskItem.tcc \
 ../../../../../../include/hurricane/base/Constants.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/service/Manager.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/service/Manager.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-service-ManagerContext_cpp.o: \
 ../../../../../../src/hurricane/service/ManagerContext.cpp \
 ../../../../../../include/hurricane/service/ManagerContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/service/ManagerContext.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/service/ManagerContext.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-service-President_cpp.o: ../../../../../../src/hurricane/service/President.cpp \
 ../../../../../../include/hurricane/service/President.h \
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
 ../../../../../../include/hurricane/message/CommandServer.tcc \
 ../../../../../../include/hurricane/service/ManagerContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/topology/TopologyLoader.h \
 ../../../../../../include/hurricane/base/Library.h \
 ../../../../../../include/hurricane/base/Constants.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/service/President.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/service/President.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-BoltExecutor_cpp.o: ../../../../../../src/hurricane/task/BoltExecutor.cpp \
 ../../../../../../include/hurricane/task/BoltExecutor.h \
 ../../../../../../include/hurricane/task/Executor.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/collector/TaskQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.tcc \
 ../../../../../../include/hurricane/collector/TaskItem.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/task/BoltExecutor.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/BoltExecutor.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-Executor_cpp.o: ../../../../../../src/hurricane/task/Executor.cpp \
 ../../../../../../include/hurricane/task/Executor.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/task/Executor.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/Executor.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-PathInfo_cpp.o: ../../../../../../src/hurricane/task/PathInfo.cpp \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/task/PathInfo.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/PathInfo.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-SpoutExecutor_cpp.o: ../../../../../../src/hurricane/task/SpoutExecutor.cpp \
 ../../../../../../include/hurricane/task/SpoutExecutor.h \
 ../../../../../../include/hurricane/task/Executor.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/task/SpoutExecutor.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/SpoutExecutor.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o: ../../../../../../src/hurricane/task/TaskDeclarer.cpp \
 ../../../../../../include/hurricane/task/TaskDeclarer.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/task/TaskDeclarer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/TaskDeclarer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-TaskInfo_cpp.o: ../../../../../../src/hurricane/task/TaskInfo.cpp \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/task/TaskInfo.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/TaskInfo.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o: ../../../../../../src/hurricane/topology/Topology.cpp \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/topology/Topology.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/topology/Topology.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-topology-TopologyLoader_cpp.o: \
 ../../../../../../src/hurricane/topology/TopologyLoader.cpp \
 ../../../../../../include/hurricane/topology/TopologyLoader.h \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/base/Library.h 
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/topology/TopologyLoader.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/topology/TopologyLoader.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-order-OrderBolt_cpp.o: ../../../../../../src/hurricane/order/OrderBolt.cpp \
 ../../../../../../include/hurricane/order/OrderBolt.h \
 ../../../../../../include/hurricane/order/OrderTuple.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/order/OrderOutputCollector.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/order/OrderBolt.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/order/OrderBolt.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-order-OrderOutputCollector_cpp.o: \
 ../../../../../../src/hurricane/order/OrderOutputCollector.cpp \
 ../../../../../../include/hurricane/order/OrderOutputCollector.h \
 ../../../../../../include/hurricane/order/OrderTuple.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/service/Manager.h \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/NetAddress.tcc \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/ByteArray.tcc \
 ../../../../../../include/hurricane/message/CommandServer.tcc \
 ../../../../../../include/hurricane/service/ManagerContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/collector/OutputDispatcher.h \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/util/NetConnector.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/order/OrderOutputCollector.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/order/OrderOutputCollector.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-order-OrderSpout_cpp.o: ../../../../../../src/hurricane/order/OrderSpout.cpp \
 ../../../../../../include/hurricane/order/OrderSpout.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/order/OrderOutputCollector.h \
 ../../../../../../include/hurricane/order/OrderTuple.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/order/OrderSpout.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/order/OrderSpout.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-order-OrderTuple_cpp.o: ../../../../../../src/hurricane/order/OrderTuple.cpp \
 ../../../../../../include/hurricane/order/OrderTuple.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/order/OrderTuple.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/order/OrderTuple.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o: ../../../../../../src/hurricane/util/Configuration.cpp \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/Configuration.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/Configuration.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o: ../../../../../../src/hurricane/util/NetConnector.cpp \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/NetConnector.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/NetConnector.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o: ../../../../../../src/hurricane/util/NetListener.cpp \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/NetAddress.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/NetListener.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/NetListener.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o: ../../../../../../src/hurricane/util/Socket.cpp \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/util/SocketError.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/Socket.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/Socket.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o: ../../../../../../src/hurricane/util/StringUtil.cpp \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/util/StringUtil.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/StringUtil.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(MESHY_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

