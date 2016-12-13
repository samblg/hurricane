export LD_LIBRARY_PATH=./

cp -uv ../bin/meshy-sample .
cp -uv ../lib/libmeshy.so .
cp -uv ../../../../../../klog/target/linux/x64/Release/lib/libklog.so .

./meshy-sample
