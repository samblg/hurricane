SRC = src/main
INCLUDE = include/main
TARGET = target
BUILD = $(TARGET)/build
CC = gcc
CXX = g++

CXXFLAGS = -std=c++11 -I$(INCLUDE) -DOS_LINUX -g
LDFLAGS = -lpthread -Ldeps/meshy/target -lmeshy

COMMON_OBJECTS = \
				$(BUILD)/DataPackage.o \
				$(BUILD)/OutputCollector.o \
				$(BUILD)/BoltExecutor.o \
				$(BUILD)/BoltOutputCollector.o \
				$(BUILD)/CommandDispatcher.o \
				$(BUILD)/MessageLoop.o \
				$(BUILD)/NimbusCommander.o \
				$(BUILD)/SupervisorCommander.o \
				$(BUILD)/SpoutExecutor.o \
				$(BUILD)/SpoutOutputCollector.o \
				$(BUILD)/SimpleTopology.o \
				$(BUILD)/TopologyBuilder.o \

NIMBUS_OBJECTS = $(BUILD)/NimbusLauncher.o

SUPERVISOR_OBJECTS = $(BUILD)/SupervisorLauncher.o

all: $(TARGET)/nimbus $(TARGET)/supervisor

clean:
	rm -rf $(TARGET)/*
	mkdir $(BUILD)

$(TARGET)/nimbus: $(COMMON_OBJECTS) $(NIMBUS_OBJECTS)
	mkdir -pv $(TARGET)
	$(CXX) -o $@ $(COMMON_OBJECTS) $(NIMBUS_OBJECTS) $(LDFLAGS)

$(TARGET)/supervisor: $(COMMON_OBJECTS) $(SUPERVISOR_OBJECTS)
	mkdir -pv $(TARGET)
	$(CXX) -o $@ $(COMMON_OBJECTS) $(SUPERVISOR_OBJECTS) $(LDFLAGS)

$(BUILD)/DataPackage.o: $(SRC)/hurricane/base/DataPackage.cpp \
	$(INCLUDE)/hurricane/base/DataPackage.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/OutputCollector.o: $(SRC)/hurricane/base/OutputCollector.cpp \
	$(INCLUDE)/hurricane/base/OutputCollector.h \
	$(INCLUDE)/hurricane/topology/ITopology.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/BoltExecutor.o: $(SRC)/hurricane/bolt/BoltExecutor.cpp \
	$(INCLUDE)/hurricane/bolt/BoltExecutor.h \
	$(INCLUDE)/hurricane/bolt/BoltMessage.h \
	$(INCLUDE)/hurricane/message/MessageLoop.h \
	$(INCLUDE)/hurricane/base/OutputCollector.h \
	$(INCLUDE)/hurricane/message/SupervisorCommander.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/BoltOutputCollector.o: $(SRC)/hurricane/bolt/BoltOutputCollector.cpp \
	$(INCLUDE)/hurricane/bolt/BoltOutputCollector.h \
	$(INCLUDE)/hurricane/bolt/BoltExecutor.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/CommandDispatcher.o: $(SRC)/hurricane/message/CommandDispatcher.cpp \
	$(INCLUDE)/hurricane/message/CommandDispatcher.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/MessageLoop.o: $(SRC)/hurricane/message/MessageLoop.cpp \
	$(INCLUDE)/hurricane/message/MessageLoop.h \
	$(INCLUDE)/hurricane/message/Message.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/NimbusCommander.o: $(SRC)/hurricane/message/NimbusCommander.cpp \
	$(INCLUDE)/hurricane/message/SupervisorCommander.h \
	$(INCLUDE)/hurricane/base/ByteArray.h \
	$(INCLUDE)/hurricane/base/DataPackage.h \
	$(INCLUDE)/hurricane/message/Command.h \
	$(INCLUDE)/hurricane/message/NimbusCommander.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/SupervisorCommander.o: $(SRC)/hurricane/message/SupervisorCommander.cpp \
	$(INCLUDE)/hurricane/message/SupervisorCommander.h \
	$(INCLUDE)/hurricane/base/ByteArray.h \
	$(INCLUDE)/hurricane/base/DataPackage.h \
	$(INCLUDE)/hurricane/message/Command.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/SpoutExecutor.o: $(SRC)/hurricane/spout/SpoutExecutor.cpp \
	$(INCLUDE)/hurricane/spout/SpoutExecutor.h \
	$(INCLUDE)/hurricane/base/OutputCollector.h \
	$(INCLUDE)/hurricane/message/SupervisorCommander.h \
	$(INCLUDE)/hurricane/spout/SpoutOutputCollector.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/SpoutOutputCollector.o: $(SRC)/hurricane/spout/SpoutOutputCollector.cpp \
	$(INCLUDE)/SpoutOutputCollector.h \
	$(INCLUDE)/hurricane/spout/SpoutExecutor.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/SimpleTopology.o: $(SRC)/hurricane/topology/SimpleTopology.cpp \
	$(INCLUDE)/hurricane/topology/SimpleTopology.h \
	$(INCLUDE)/hurricane/spout/ISpout.h \
	$(INCLUDE)/hurricane/bolt/IBolt.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/TopologyBuilder.o: $(SRC)/hurricane/topology/TopologyBuilder.cpp \
	$(INCLUDE)/hurricane/topology/TopologyBuilder.h \
	$(INCLUDE)/hurricane/spout/ISpout.h \
	$(INCLUDE)/hurricane/bolt/IBolt.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/NimbusLauncher.o: $(SRC)/hurricane/NimbusLauncher.cpp \
	$(INCLUDE)/hurricane/base/NetAddress.h \
	$(INCLUDE)/hurricane/base/ByteArray.h \
	$(INCLUDE)/hurricane/base/DataPackage.h \
	$(INCLUDE)/hurricane/message/CommandDispatcher.h \
	$(INCLUDE)/hurricane/message/NimbusCommander.h \
	$(INCLUDE)/hurricane/base/Node.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/SupervisorLauncher.o: $(SRC)/hurricane/SupervisorLauncher.cpp \
	$(INCLUDE)/hurricane/base/NetAddress.h \
	$(INCLUDE)/hurricane/base/ByteArray.h \
	$(INCLUDE)/hurricane/base/DataPackage.h \
	$(INCLUDE)/hurricane/base/Value.h \
	$(INCLUDE)/hurricane/base/Variant.h \
	$(INCLUDE)/hurricane/message/SupervisorCommander.h \
	$(INCLUDE)/hurricane/message/CommandDispatcher.h
	mkdir -pv $(BUILD)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
