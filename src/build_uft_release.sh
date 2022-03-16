if [ ! -d "./build_uft" ];then
mkdir build_uft
fi
cd build_uft
cmake -DCMAKE_BUILD_TYPE=Release -P CMakeLists_UFT.txt ..
make