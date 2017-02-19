-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-hurricane-bolt-JavaBolt_cpp.o \
__-__-__-__-__-__-src-hurricane-spout-JavaSpout_cpp.o \
__-__-__-__-__-__-src-hurricane-collector-JavaOutputCollector_cpp.o \
__-__-__-__-__-__-src-hurricane-multilang-java-Array_cpp.o \
__-__-__-__-__-__-src-hurricane-multilang-java-Class_cpp.o \
__-__-__-__-__-__-src-hurricane-multilang-java-NativeObjectWrapper_cpp.o \
__-__-__-__-__-__-src-hurricane-multilang-java-Object_cpp.o \
__-__-__-__-__-__-src-hurricane-multilang-java-String_cpp.o \
__-__-__-__-__-__-src-hurricane-multilang-java-VirtualMachine_cpp.o

all: libhurricane-java.so

libhurricane-java.so: $(OBJS)
	@echo -e "[\e[32mLINK\033[0m] libhurricane-java.so"
	@$(LD) $(OBJS) -shared -o libhurricane-java.so $(LOGGING_LIB) $(JNI_LIB) -llogging -ljvm 

install: ../../lib/libhurricane-java.so

../../lib/libhurricane-java.so: libhurricane-java.so
	@echo -e "[\e[32mINSTALL\033[0m] hurricane-java/libhurricane-java.so -> ../../lib"
	@cp libhurricane-java.so ../../lib

clean:
	@echo -e "[\e[32mCLEAN\033[0m] hurricane-java/libhurricane-java.so"
	@rm -f libhurricane-java.so
	@echo -e "[\e[32mCLEAN\033[0m] hurricane-java/*.o"
	@rm -f *.o

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
	@$(CXX) ../../../../../../src/hurricane/bolt/JavaBolt.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

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
	@$(CXX) ../../../../../../src/hurricane/spout/JavaSpout.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-collector-JavaOutputCollector_cpp.o: \
 ../../../../../../src/hurricane/collector/JavaOutputCollector.cpp \
 ../../../../../../include/hurricane/collector/JavaOutputCollector.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h \
 ../../../../../../include/hurricane/base/Values.h \
 ../../../../../../include/hurricane/base/Variant.h \
 ../../../../../../include/hurricane/base/Variant.tcc \
 ../../../../../../include/hurricane/base/Values.tcc \
 ../../../../../../include/hurricane/multilang/java/Signature.h \
 ../../../../../../include/hurricane/collector/OutputCollector.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/collector/JavaOutputCollector.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/collector/JavaOutputCollector.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-multilang-java-Array_cpp.o: ../../../../../../src/hurricane/multilang/java/Array.cpp \
 ../../../../../../include/hurricane/multilang/java/Array.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h \
 ../../../../../../include/hurricane/multilang/java/Array.tcc
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/multilang/java/Array.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/multilang/java/Array.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-multilang-java-Class_cpp.o: ../../../../../../src/hurricane/multilang/java/Class.cpp \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h 
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/multilang/java/Class.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/multilang/java/Class.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-multilang-java-NativeObjectWrapper_cpp.o: \
 ../../../../../../src/hurricane/multilang/java/NativeObjectWrapper.cpp \
 ../../../../../../include/hurricane/multilang/java/NativeObjectWrapper.h \
 ../../../../../../include/hurricane/multilang/java/NativeObjectWrapper.tcc \
 ../../../../../../include/hurricane/multilang/java/String.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h \
 ../../../../../../include/hurricane/multilang/java/Signature.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/multilang/java/NativeObjectWrapper.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/multilang/java/NativeObjectWrapper.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-multilang-java-Object_cpp.o: ../../../../../../src/hurricane/multilang/java/Object.cpp \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h 
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/multilang/java/Object.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/multilang/java/Object.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-multilang-java-String_cpp.o: ../../../../../../src/hurricane/multilang/java/String.cpp \
 ../../../../../../include/hurricane/multilang/java/String.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Object.h \
 ../../../../../../include/hurricane/multilang/java/SignatureImpl.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/multilang/java/String.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/multilang/java/String.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-hurricane-multilang-java-VirtualMachine_cpp.o: \
 ../../../../../../src/hurricane/multilang/java/VirtualMachine.cpp \
 ../../../../../../include/hurricane/multilang/java/VirtualMachine.h \
 ../../../../../../include/hurricane/multilang/java/Class.h \
 ../../../../../../include/hurricane/util/StringUtil.h
	@echo -e "[\e[32mCXX\033[0m] ../../../../../../src/hurricane/multilang/java/VirtualMachine.cpp -> $@"
	@$(CXX) ../../../../../../src/hurricane/multilang/java/VirtualMachine.cpp -c -o $@ -I../../../../../../include $(LOGGING_INCLUDE) $(JNI_INCLUDE) -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

