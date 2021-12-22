
#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtTester LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

#3. 添加包含路径
IF (MSVC)
	IF("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		SET(INCS $ENV{MyDepends141}/include)
		SET(LNKS $ENV{MyDepends141}/lib/x64)
	ELSE()
		SET(INCS $ENV{MyDepends141}/include)
		SET(LNKS $ENV{MyDepends141}/lib/x86)
	ENDIF()
ELSE (GNU)
	IF(WIN32)
		SET (MyDeps $ENV{MyDependsGcc})
	ELSE()
		SET (MyDeps "/home/mydeps")
	ENDIF()
	SET(INCS ${MyDeps}/include)
	SET(LNKS ${MyDeps}/lib)
ENDIF ()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtBtRunner)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

SET(LIBS
	WtBtCore
	WTSTools
	WTSUtils
)

IF(MSVC)
ELSE(GNU)
	LIST(APPEND LIBS
		boost_filesystem
		pthread
		dl)
ENDIF()

#添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

IF(MSVC)
	list (APPEND SRCS ../Common/mdump.cpp)
ENDIF()

#9. 生成可执行文件
ADD_EXECUTABLE(WtBtRunner ${SRCS} ${HDRS})
TARGET_LINK_LIBRARIES(WtBtRunner ${LIBS})

IF (MSVC)
ELSE (GNU)
	SET_TARGET_PROPERTIES(WtBtRunner PROPERTIES
        LINK_FLAGS_RELEASE -s)
ENDIF ()

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtBtPorter)
#ADD_EXECUTABLE(TestBtPorter ../TestBtPorter/TestBtPorter.cpp)
#TARGET_LINK_LIBRARIES(TestBtPorter ${LIBS})

