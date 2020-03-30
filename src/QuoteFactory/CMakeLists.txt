
#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(QuoteFactory LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

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

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/bin/QuoteFactory)

#7. 添加源码
FILE( GLOB SRCS *.cpp)
FILE( GLOB HDRS *.h *.hpp)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

SET(LIBS
	${PROJECT_SOURCE_DIR}/../build/libs/libWTSTools.a
	${PROJECT_SOURCE_DIR}/../build/libs/libWTSUtils.a
	
	boost_thread
	boost_filesystem
	
	pthread
	dl
)

#9. 生成可执行文件
ADD_EXECUTABLE(QuoteFactory ${SRCS} ${HDRS})
TARGET_LINK_LIBRARIES(QuoteFactory ${LIBS})


