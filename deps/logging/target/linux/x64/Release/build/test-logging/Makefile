-include Makefile.config

-include ../Makefile.deps

OBJS := __-__-__-__-__-__-test-test--logging-main_cpp.o

all: test-logging

test-logging: $(OBJS)
	@echo -e "\e[32m[LINK] \033[0mtest-logging"
	@$(LD) $(OBJS) -o test-logging -L"../logging" -llogging 

install: ../../bin/test-logging

../../bin/test-logging: test-logging
	@echo -e "\e[32m[INSTALL] \033[0mtest-logging -> ../../bin"
	@cp test-logging ../../bin

clean:
	@echo -e "\e[32m[CLEAN] \033[0mtest-logging"
	@rm -f test-logging
	@rm -f *.o

__-__-__-__-__-__-test-test--logging-main_cpp.o: ../../../../../../test/test-logging/main.cpp \
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
	@echo -e "\e[32m[CXX] \033[0m../../../../../../test/test-logging/main.cpp -> $@"
	@$(CXX) ../../../../../../test/test-logging/main.cpp -c -o $@ -I../../../../../../include -I../../../../../../include -std=c++11 -O2 -msse3 -mavx -fPIC -DNDEBUG -DUSE_KLOG

