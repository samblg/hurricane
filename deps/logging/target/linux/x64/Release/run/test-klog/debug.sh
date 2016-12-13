source ../environment.sh

cp -u ../../bin/test-klog .

gdb ./test-klog core
