
#1. 确定CMake的最低版本需求
cmake_minimum_required(VERSION 3.0.0)

#2. 确定工程名
project(WtDtCore LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

#4. 添加包含文件目录
IF (WIN32)
	IF("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		SET(INCS $ENV{MyDepends141}/include)
		SET(LNKS $ENV{MyDepends141}/lib/x64)
	ELSE()
		SET(INCS $ENV{MyDepends141}/include)
		SET(LNKS $ENV{MyDepends141}/lib/x86)
	ENDIF()
ELSE (UNIX)
	SET (MyDeps "/home/mydeps")
	SET(INCS ${MyDeps}/include)
	SET(LNKS ${MyDeps}/lib)
ENDIF ()

SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/../build/libs)

#7. 添加源码
FILE(GLOB SRCS *.cpp)
FILE( GLOB HDRS *.h *.hpp)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

ADD_LIBRARY(WtDtCore STATIC ${SRCS} ${HDRS})