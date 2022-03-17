
#1. 确定CMake的最低版本需求
cmake_minimum_required(VERSION 3.0.0)

#2. 确定工程名
project(WTSUtils LANGUAGES C CXX)
SET(CMAKE_CXX_STANDARD 11)

IF(MSVC)
ELSE(GNUCC)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/libs)

LIST(APPEND INCS "./yamlcpp")

#7. 添加源码
file(GLOB SRCS "*.cpp" "./zstdlib/*.c" "./yamlcpp/*.cpp" "./lmdb/*.c")
file(GLOB HDRS "*.h" "./zstdlib/*.h")

IF(MSVC)
LIST(APPEND SRCS "./StackTracer/StackTracer.cpp" "./StackTracer/StackWalker.cpp")
ELSE (GNUCC)
LIST(APPEND SRCS "./StackTracer/StackTracer.cpp")
ENDIF()

INCLUDE_DIRECTORIES(${INCS})

#8. 编译成动态库，并生成库文件
add_library(WTSUtils STATIC ${SRCS} ${HDRS})