if [ ! -d "./build_all" ];then
mkdir build_all
fi
cd build_all
cmake -DCMAKE_BUILD_TYPE=Release ..
make