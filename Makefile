CONFIGURATION := Release

ifdef ENABLE_MULTILANG_JVM
HURRICANE_CONFIGURATION := Release-with-JVM
else
HURRICANE_CONFIGURATION := Release
endif

all:
	cd deps/logging/target/linux/x64/$(CONFIGURATION)/build;make install
	cd deps/meshy/target/linux/x64/$(CONFIGURATION)/build;make install
	#cd target/linux/x64/$(HURRICANE_CONFIGURATION)/build;make all; make install
	cd src/sample/President;make all
	cd src/sample/Manager;make all
clean:
	cd target/linux/x64/$(HURRICANE_CONFIGURATION)/build;make clean
	cd deps/meshy/target/linux/x64/$(CONFIGURATION)/build;make clean
	cd src/sample/President;make all
	cd src/sample/Manager;make all


