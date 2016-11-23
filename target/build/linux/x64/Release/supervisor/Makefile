-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-hurricane-tool-StartSupervisor_cpp.o \
__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Library_cpp.o \
__-__-__-__-__-__-src-hurricane-base-Values_cpp.o \
__-__-__-__-__-__-src-hurricane-bolt-BoltDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-bolt-BoltExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-bolt-BoltOutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-collector-OutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-collector-OutputDispatcher_cpp.o \
__-__-__-__-__-__-src-hurricane-message-Command_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandClient_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandDispatcher_cpp.o \
__-__-__-__-__-__-src-hurricane-message-CommandServer_cpp.o \
__-__-__-__-__-__-src-hurricane-message-MessageLoop_cpp.o \
__-__-__-__-__-__-src-hurricane-message-NimbusCommander_cpp.o \
__-__-__-__-__-__-src-hurricane-message-SupervisorCommander_cpp.o \
__-__-__-__-__-__-src-hurricane-service-Nimbus_cpp.o \
__-__-__-__-__-__-src-hurricane-service-Supervisor_cpp.o \
__-__-__-__-__-__-src-hurricane-service-SupervisorContext_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-SpoutDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-SpoutExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-SpoutOutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-task-BoltExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-Executor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-PathInfo_cpp.o \
__-__-__-__-__-__-src-hurricane-task-SpoutExecutor_cpp.o \
__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o \
__-__-__-__-__-__-src-hurricane-task-TaskInfo_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-SimpleTopology_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-TopologyBuilder_cpp.o \
__-__-__-__-__-__-src-hurricane-topology-TopologyLoader_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o \
__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o \
__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o \
__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o

all: supervisor

supervisor: $(OBJS)
	$(LD) $(OBJS) -o supervisor  -lpthread -ldl

install: ../../../../../bin/linux/x64/Release/supervisor

../../../../../bin/linux/x64/Release/supervisor: supervisor
	cp supervisor ../../../../../bin/linux/x64/Release

clean:
	rm -f supervisor
	rm -f *.o

__-__-__-__-__-__-src-hurricane-tool-StartSupervisor_cpp.o: \
 ../../../../../../src/hurricane/tool/StartSupervisor.cpp \
 ../../../../../../include/hurricane/service/Supervisor.h \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/service/SupervisorContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/collector/OutputDispatcher.h \
 ../../../../../../include/hurricane/util/Configuration.h
	$(CXX) ../../../../../../src/hurricane/tool/StartSupervisor.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-base-DataPackage_cpp.o: ../../../../../../src/hurricane/base/DataPackage.cpp \
 ../../../../../../include/hurricane/base/DataPackage.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/base/DataPackage.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-base-Library_cpp.o: ../../../../../../src/hurricane/base/Library.cpp \
 ../../../../../../include/hurricane/base/Library.h
	$(CXX) ../../../../../../src/hurricane/base/Library.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-base-Values_cpp.o: ../../../../../../src/hurricane/base/Values.cpp \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/base/Values.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-bolt-BoltDeclarer_cpp.o: ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/IBolt.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/bolt/BoltDeclarer.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-bolt-BoltExecutor_cpp.o: ../../../../../../src/hurricane/bolt/BoltExecutor.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/bolt/BoltExecutor.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-bolt-BoltOutputCollector_cpp.o: \
 ../../../../../../src/hurricane/bolt/BoltOutputCollector.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/bolt/BoltOutputCollector.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-collector-OutputCollector_cpp.o: \
 ../../../../../../src/hurricane/collector/OutputCollector.cpp \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/collector/OutputQueue.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/collector/OutputCollector.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

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
	$(CXX) ../../../../../../src/hurricane/collector/OutputDispatcher.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-message-Command_cpp.o: ../../../../../../src/hurricane/message/Command.cpp \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/DataPackage.h
	$(CXX) ../../../../../../src/hurricane/message/Command.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-message-CommandClient_cpp.o: \
 ../../../../../../src/hurricane/message/CommandClient.cpp \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h
	$(CXX) ../../../../../../src/hurricane/message/CommandClient.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-message-CommandDispatcher_cpp.o: \
 ../../../../../../src/hurricane/message/CommandDispatcher.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/message/CommandDispatcher.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-message-CommandServer_cpp.o: \
 ../../../../../../src/hurricane/message/CommandServer.cpp \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h
	$(CXX) ../../../../../../src/hurricane/message/CommandServer.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-message-MessageLoop_cpp.o: ../../../../../../src/hurricane/message/MessageLoop.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/message/MessageLoop.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-message-NimbusCommander_cpp.o: \
 ../../../../../../src/hurricane/message/NimbusCommander.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/message/NimbusCommander.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-message-SupervisorCommander_cpp.o: \
 ../../../../../../src/hurricane/message/SupervisorCommander.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/message/SupervisorCommander.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-service-Nimbus_cpp.o: ../../../../../../src/hurricane/service/Nimbus.cpp \
 ../../../../../../include/hurricane/service/Nimbus.h \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/service/SupervisorContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/message/CommandClient.h \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/topology/TopologyLoader.h \
 ../../../../../../include/sample/wordcount/WordCountTopology.h \
 ../../../../../../include/hurricane/base/externc.h
	$(CXX) ../../../../../../src/hurricane/service/Nimbus.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-service-Supervisor_cpp.o: ../../../../../../src/hurricane/service/Supervisor.cpp \
 ../../../../../../include/hurricane/service/Supervisor.h \
 ../../../../../../include/hurricane/message/CommandServer.h \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/message/Command.h \
 ../../../../../../include/hurricane/base/ByteArray.h \
 ../../../../../../include/hurricane/service/SupervisorContext.h \
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
 ../../../../../../include/hurricane/collector/TaskQueue.h
	$(CXX) ../../../../../../src/hurricane/service/Supervisor.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-service-SupervisorContext_cpp.o: \
 ../../../../../../src/hurricane/service/SupervisorContext.cpp \
 ../../../../../../include/hurricane/service/SupervisorContext.h \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/service/SupervisorContext.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-spout-SpoutDeclarer_cpp.o: ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/spout/ISpout.h \
 ../../../../../../include/hurricane/task/ITask.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/spout/SpoutDeclarer.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-spout-SpoutExecutor_cpp.o: ../../../../../../src/hurricane/spout/SpoutExecutor.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/spout/SpoutExecutor.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-spout-SpoutOutputCollector_cpp.o: \
 ../../../../../../src/hurricane/spout/SpoutOutputCollector.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/spout/SpoutOutputCollector.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

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
	$(CXX) ../../../../../../src/hurricane/task/BoltExecutor.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-task-Executor_cpp.o: ../../../../../../src/hurricane/task/Executor.cpp \
 ../../../../../../include/hurricane/task/Executor.h \
 ../../../../../../include/hurricane/message/Message.h \
 ../../../../../../include/hurricane/base/BlockingQueue.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/task/Executor.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-task-PathInfo_cpp.o: ../../../../../../src/hurricane/task/PathInfo.cpp \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/task/PathInfo.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

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
	$(CXX) ../../../../../../src/hurricane/task/SpoutExecutor.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-task-TaskDeclarer_cpp.o: ../../../../../../src/hurricane/task/TaskDeclarer.cpp \
 ../../../../../../include/hurricane/task/TaskDeclarer.h
	$(CXX) ../../../../../../src/hurricane/task/TaskDeclarer.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-task-TaskInfo_cpp.o: ../../../../../../src/hurricane/task/TaskInfo.cpp \
 ../../../../../../include/hurricane/task/TaskInfo.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/task/TaskInfo.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-topology-SimpleTopology_cpp.o: \
 ../../../../../../src/hurricane/topology/SimpleTopology.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/topology/SimpleTopology.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-topology-Topology_cpp.o: ../../../../../../src/hurricane/topology/Topology.cpp \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h
	$(CXX) ../../../../../../src/hurricane/topology/Topology.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-topology-TopologyBuilder_cpp.o: \
 ../../../../../../src/hurricane/topology/TopologyBuilder.cpp \
 ../../../../../../include/hurricane/Hurricane.h
	$(CXX) ../../../../../../src/hurricane/topology/TopologyBuilder.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-topology-TopologyLoader_cpp.o: \
 ../../../../../../src/hurricane/topology/TopologyLoader.cpp \
 ../../../../../../include/hurricane/topology/TopologyLoader.h \
 ../../../../../../include/hurricane/topology/Topology.h \
 ../../../../../../include/hurricane/spout/SpoutDeclarer.h \
 ../../../../../../include/hurricane/task/TaskDeclarer.h \
 ../../../../../../include/hurricane/bolt/BoltDeclarer.h \
 ../../../../../../include/hurricane/base/Library.h \
 ../../../../../../include/sample/wordcount/WordCountTopology.h \
 ../../../../../../include/hurricane/base/externc.h
	$(CXX) ../../../../../../src/hurricane/topology/TopologyLoader.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-util-Configuration_cpp.o: ../../../../../../src/hurricane/util/Configuration.cpp \
 ../../../../../../include/hurricane/util/Configuration.h \
 ../../../../../../include/hurricane/util/StringUtil.h
	$(CXX) ../../../../../../src/hurricane/util/Configuration.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-util-NetConnector_cpp.o: ../../../../../../src/hurricane/util/NetConnector.cpp \
 ../../../../../../include/hurricane/util/NetConnector.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../deps/meshy/include/Meshy.h \
 ../../../../../../deps/meshy/include/IoLoop.h \
 ../../../../../../deps/meshy/include/net.h \
 ../../../../../../deps/meshy/include/bytearray.h \
 ../../../../../../deps/meshy/include/utils/exendian.h
	$(CXX) ../../../../../../src/hurricane/util/NetConnector.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-util-NetListener_cpp.o: ../../../../../../src/hurricane/util/NetListener.cpp \
 ../../../../../../include/hurricane/util/NetListener.h \
 ../../../../../../include/hurricane/util/Socket.h \
 ../../../../../../include/hurricane/base/NetAddress.h \
 ../../../../../../include/hurricane/base/Variant.h
	$(CXX) ../../../../../../src/hurricane/util/NetListener.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-util-Socket_cpp.o: ../../../../../../src/hurricane/util/Socket.cpp
	$(CXX) ../../../../../../src/hurricane/util/Socket.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

__-__-__-__-__-__-src-hurricane-util-StringUtil_cpp.o: ../../../../../../src/hurricane/util/StringUtil.cpp \
 ../../../../../../include/hurricane/util/StringUtil.h
	$(CXX) ../../../../../../src/hurricane/util/StringUtil.cpp -c -o $@ -I../../../../../../include -I../../../../../../deps/meshy/include -O2 -std=c++11

