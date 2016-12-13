-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-src-logging-CheckLogger_cpp.o \
__-__-__-__-__-__-src-logging-ExpressionLogger_cpp.o \
__-__-__-__-__-__-src-logging-Logger_cpp.o \
__-__-__-__-__-__-src-logging-IOStreamManager_cpp.o \
__-__-__-__-__-__-src-logging-DirectLoggerStream_cpp.o \
__-__-__-__-__-__-src-logging-Logging_cpp.o

all: liblogging.so

liblogging.so: $(OBJS)
	@echo -e "\e[32m[LINK] \033[0mliblogging.so"
	@$(LD) $(OBJS) -shared -o liblogging.so   

install: ../../lib/liblogging.so

../../lib/liblogging.so: liblogging.so
	@echo -e "\e[32m[INSTALL] \033[0mliblogging.so -> ../../lib"
	@cp liblogging.so ../../lib

clean:
	@echo -e "\e[32m[CLEAN] \033[0mliblogging.so"
	@rm -f liblogging.so
	@rm -f *.o

__-__-__-__-__-__-src-logging-CheckLogger_cpp.o: ../../../../../../src/logging/CheckLogger.cpp \
 ../../../../../../include/logging/CheckLogger.h \
 ../../../../../../include/logging/ExpressionLogger.h \
 ../../../../../../include/logging/Logger.h \
 ../../../../../../include/logging/Severity.h \
 ../../../../../../include/logging/LoggerStream.h \
 ../../../../../../include/logging/System.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/logging/CheckLogger.cpp -> $@"
	@$(CXX) ../../../../../../src/logging/CheckLogger.cpp -c -o $@ -I../../../../../../include -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-logging-ExpressionLogger_cpp.o: ../../../../../../src/logging/ExpressionLogger.cpp \
 ../../../../../../include/logging/ExpressionLogger.h \
 ../../../../../../include/logging/Logger.h \
 ../../../../../../include/logging/Severity.h \
 ../../../../../../include/logging/LoggerStream.h \
 ../../../../../../include/logging/System.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/logging/ExpressionLogger.cpp -> $@"
	@$(CXX) ../../../../../../src/logging/ExpressionLogger.cpp -c -o $@ -I../../../../../../include -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-logging-Logger_cpp.o: ../../../../../../src/logging/Logger.cpp \
 ../../../../../../include/logging/Logger.h \
 ../../../../../../include/logging/Severity.h \
 ../../../../../../include/logging/LoggerStream.h \
 ../../../../../../include/logging/System.h \
 ../../../../../../include/logging/DirectLoggerStream.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/logging/Logger.cpp -> $@"
	@$(CXX) ../../../../../../src/logging/Logger.cpp -c -o $@ -I../../../../../../include -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-logging-IOStreamManager_cpp.o: ../../../../../../src/logging/IOStreamManager.cpp \
 ../../../../../../include/logging/IOStreamManager.h \
 ../../../../../../include/logging/Severity.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/logging/IOStreamManager.cpp -> $@"
	@$(CXX) ../../../../../../src/logging/IOStreamManager.cpp -c -o $@ -I../../../../../../include -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-logging-DirectLoggerStream_cpp.o: \
 ../../../../../../src/logging/DirectLoggerStream.cpp \
 ../../../../../../include/logging/DirectLoggerStream.h \
 ../../../../../../include/logging/LoggerStream.h \
 ../../../../../../include/logging/Severity.h \
 ../../../../../../include/logging/IOStreamManager.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/logging/DirectLoggerStream.cpp -> $@"
	@$(CXX) ../../../../../../src/logging/DirectLoggerStream.cpp -c -o $@ -I../../../../../../include -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

__-__-__-__-__-__-src-logging-Logging_cpp.o: ../../../../../../src/logging/Logging.cpp \
 ../../../../../../include/logging/Logging.h \
 ../../../../../../include/logging/Interface.h \
 ../../../../../../include/logging/Logger.h \
 ../../../../../../include/logging/Severity.h \
 ../../../../../../include/logging/LoggerStream.h \
 ../../../../../../include/logging/System.h \
 ../../../../../../include/logging/ExpressionLogger.h \
 ../../../../../../include/logging/CheckLogger.h \
 ../../../../../../include/logging/IOStreamManager.h \
 ../../../../../../include/logging/Check.h
	@echo -e "\e[32m[CXX] \033[0m../../../../../../src/logging/Logging.cpp -> $@"
	@$(CXX) ../../../../../../src/logging/Logging.cpp -c -o $@ -I../../../../../../include -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

