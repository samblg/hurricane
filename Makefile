all:
	cd deps/logging/target/linux/x64/Release/build;make install
	cd deps/meshy/target/linux/x64/Release/build;make install
	cd target/linux/x64/Release/build;make install

clean:
	cd target/linux/x64/Release/build;make clean
	cd deps/meshy/target/linux/x64/Release/build;make clean
	cd deps/logging/target/linux/x64/Release/build;make clean

