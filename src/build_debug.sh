if [ ! -d "./build_debug" ];then
mkdir build_debug
fi
cd build_debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make