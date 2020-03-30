
#1. 确定CMake的最低版本需求
cmake_minimum_required(VERSION 3.0.0)

#2. 确定工程名
project(WTSUtils LANGUAGES C CXX)
SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")


#4. 添加包含文件目录
IF (WIN32)
	IF("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		SET(INCS $ENV{MyX64Deps12}/include)
		SET(LNKS $ENV{MyX64Deps12}/lib)
	ELSE()
		SET(INCS $ENV{MyDepends12}/include)
		SET(LNKS $ENV{MyDepends12}/lib)
	ENDIF()
ELSE (UNIX)
	SET (MyDeps "/home/mydeps")
	SET(INCS ${MyDeps}/include)
	SET(LNKS ${MyDeps}/lib)
ENDIF ()

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/libs)

#7. 添加源码
file(GLOB SRCS "*.cpp" "./zstdlib/*.c" "./pugixml/*.cpp")
file(GLOB HDRS "*.h" "./zstdlib/*.h" "./pugixml/*.hpp")

#8. 编译成动态库，并生成库文件
add_library(WTSUtils STATIC ${SRCS} ${HDRS})