
#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(TestPorter LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

#3. 环境判断
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

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtPorter)

#7. 添加源码
file(GLOB SRCS *.cpp)

SET(LIBS
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtPorter/libWtPorter.so
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtPorter/libWtExecMon.so
	dl
)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

ADD_EXECUTABLE(TestPorter ${SRCS})
TARGET_LINK_LIBRARIES(TestPorter ${LIBS})