export LD_LIBRARY_PATH=./

cp -uv ../bin/meshy-client-sample .
cp -uv ../lib/libmeshy.so .
cp -uv ../../../../../../klog/target/linux/x64/Release/lib/libklog.so .

./meshy-client-sample
