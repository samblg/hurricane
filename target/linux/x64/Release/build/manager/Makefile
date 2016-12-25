-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-hurricane-tool-StartManager_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Constants_cpp.o \
__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Library_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Values_cpp.o \
__-__-__-__-__-__-src-hurricane-bolt-BoltDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-collector-OutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-collector-OutputDispatcher_cpp.o \
__-__-__-__-__-__-src-hurricane-message-Command_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandClient_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandServer_cpp.o \
__-__-__-__-__-__-src-hurricane-service-Manager_cpp.o \
__-__-__-__-__-__-src-hurricane-service-ManagerContext_cpp.o \
__-__-__-__-__-__-src-hurricane-service-President_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-SpoutDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-task-BoltExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-Executor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-PathInfo_cpp.o \
__-__-__-__-__-__-src-hurricane-task-SpoutExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-task-TaskInfo_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-TopologyLoader_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o \
__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o

all: manager

manager: $(OBJS)
	@echo -e "\e[32m[LINK] \033[0mmanager"
	@$(LD) $(OBJS) -o manager -L"$(LOGGING_LIB)" -llogging -lpthread -ldl 

install: ../../bin/manager

../../bin/manager: manager
	@echo -e "\e[32m[INSTALL] \033[0mmanager -> ../../bin"
	@cp manager ../../bin

clean:
	@echo -e "\e[32m[CLEAN] \033[0mmanager"
	@rm -f manager
	@rm -f *.o

__-__-__-__-__-__-src-hurricane-tool-StartManager_cpp.o: ../../../../../../src/hurricane/tool/StartManager.cpp \
 ../../../../../../include/hurricane/service/Manager.h \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/service/ManagerContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/collector/OutputDispatcher.h \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/base/Constants.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/tool/StartManager.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/tool/StartManager.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Constants_cpp.o: ../../../../../../src/hurricane/base/Constants.cpp \
 ../../../../../../include/hurricane/base/Constants.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/base/Constants.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Constants.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o: ../../../../../../src/hurricane/base/DataPackage.cpp \
 ../../../../../../include/hurricane/base/DataPackage.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/base/DataPackage.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/DataPackage.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Library_cpp.o: ../../../../../../src/hurricane/base/Library.cpp \
 ../../../../../../include/hurricane/base/Library.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/base/Library.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Library.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-base-Values_cpp.o: ../../../../../../src/hurricane/base/Values.cpp \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/base/Values.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/base/Values.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-bolt-BoltDeclarer_cpp.o: ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/bolt/BoltDeclarer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-collector-OutputCollector_cpp.o: \
 ../../../../../../src/hurricane/collector/OutputCollector.cpp \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/collector/OutputQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/collector/OutputCollector.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/collector/OutputCollector.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-collector-OutputDispatcher_cpp.o: \
 ../../../../../../src/hurricane/collector/OutputDispatcher.cpp \
 ../../../../../../include/hurricane/collector/OutputDispatcher.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/collector/OutputQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/collector/TaskQueue.h \
 ../../../../../../include/hurricane/util/StringUtil.h \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/collector/OutputDispatcher.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/collector/OutputDispatcher.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-message-Command_cpp.o: ../../../../../../src/hurricane/message/Command.cpp \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/DataPackage.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/message/Command.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/message/Command.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-message-CommandClient_cpp.o: \
 ../../../../../../src/hurricane/message/CommandClient.cpp \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/message/CommandClient.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/message/CommandClient.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-message-CommandServer_cpp.o: \
 ../../../../../../src/hurricane/message/CommandServer.cpp \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/message/CommandServer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/message/CommandServer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-service-Manager_cpp.o: ../../../../../../src/hurricane/service/Manager.cpp \
 ../../../../../../include/hurricane/service/Manager.h \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
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
 ../../../../../../include/hurricane/message/Message.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/task/BoltExecutor.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/collector/OutputQueue.h \
 ../../../../../../include/hurricane/collector/TaskQueue.h \
 ../../../../../../include/hurricane/base/Constants.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/service/Manager.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/service/Manager.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-service-ManagerContext_cpp.o: \
 ../../../../../../src/hurricane/service/ManagerContext.cpp \
 ../../../../../../include/hurricane/service/ManagerContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/service/ManagerContext.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/service/ManagerContext.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-service-President_cpp.o: ../../../../../../src/hurricane/service/President.cpp \
 ../../../../../../include/hurricane/service/President.h \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
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
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/service/President.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/service/President.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-spout-SpoutDeclarer_cpp.o: ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/spout/SpoutDeclarer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-BoltExecutor_cpp.o: ../../../../../../src/hurricane/task/BoltExecutor.cpp \
 ../../../../../../include/hurricane/task/BoltExecutor.h \
 ../../../../../../include/hurricane/task/Executor.h \
 ../../../../../../include/hurricane/message/Message.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/collector/TaskQueue.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/task/BoltExecutor.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/BoltExecutor.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-Executor_cpp.o: ../../../../../../src/hurricane/task/Executor.cpp \
 ../../../../../../include/hurricane/task/Executor.h \
 ../../../../../../include/hurricane/message/Message.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/task/Executor.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/Executor.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-PathInfo_cpp.o: ../../../../../../src/hurricane/task/PathInfo.cpp \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/task/PathInfo.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/PathInfo.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-SpoutExecutor_cpp.o: ../../../../../../src/hurricane/task/SpoutExecutor.cpp \
 ../../../../../../include/hurricane/task/SpoutExecutor.h \
 ../../../../../../include/hurricane/task/Executor.h \
 ../../../../../../include/hurricane/message/Message.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/task/SpoutExecutor.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/SpoutExecutor.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o: ../../../../../../src/hurricane/task/TaskDeclarer.cpp \
 ../../../../../../include/hurricane/task/TaskDeclarer.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/task/TaskDeclarer.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/TaskDeclarer.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-task-TaskInfo_cpp.o: ../../../../../../src/hurricane/task/TaskInfo.cpp \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/task/TaskInfo.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/task/TaskInfo.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o: ../../../../../../src/hurricane/topology/Topology.cpp \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/topology/Topology.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/topology/Topology.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-topology-TopologyLoader_cpp.o: \
 ../../../../../../src/hurricane/topology/TopologyLoader.cpp \
 ../../../../../../include/hurricane/topology/TopologyLoader.h \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/base/Library.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/topology/TopologyLoader.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/topology/TopologyLoader.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o: ../../../../../../src/hurricane/util/Configuration.cpp \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/util/Configuration.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/Configuration.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o: ../../../../../../src/hurricane/util/NetConnector.cpp \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/util/NetConnector.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/NetConnector.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o: ../../../../../../src/hurricane/util/NetListener.cpp \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h 
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/util/NetListener.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/NetListener.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o: ../../../../../../src/hurricane/util/Socket.cpp
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/util/Socket.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/Socket.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o: ../../../../../../src/hurricane/util/StringUtil.cpp \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/hurricane/util/StringUtil.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/util/StringUtil.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

