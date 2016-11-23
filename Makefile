all:
	cd deps/meshy/target/build/linux/x64/Release;make
	cd target/build/linux/x64/Release;make

clean:
	cd target/build/linux/x64/Release;make clean
	cd deps/meshy/target/build/linux/x64/Release;make clean

install:
	cd deps/meshy/target/build/linux/x64/Release;make install
	cd target/build/linux/x64/Release;make install
