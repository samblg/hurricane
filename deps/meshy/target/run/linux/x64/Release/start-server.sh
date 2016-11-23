export LD_LIBRARY_PATH=./

cp -uv ../../../../bin/linux/x64/Release/meshy-sample .
cp -uv ../../../../bin/linux/x64/Release/meshy-client-sample .
cp -uv ../../../../lib/linux/x64/Release/libmeshy.so .

./meshy-sample
