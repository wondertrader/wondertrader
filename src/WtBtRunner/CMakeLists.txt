
#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtTester LANGUAGES CXX)
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

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtTester)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

SET(LIBS
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/libs/libWtBtCore.a
	
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/libs/libWTSTools.a
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/libs/libWTSUtils.a
	
	${MyDeps}/lib/libmysqlclient.a
	boost_filesystem
	pthread
	dl
)

#9. 生成可执行文件
ADD_EXECUTABLE(WtBtRunner WtBtRunner.cpp)
TARGET_LINK_LIBRARIES(WtBtRunner ${LIBS})


SET(LIBS
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtBtPorter/libWtBtPorter.so
	
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/libs/libWTSTools.a
	${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/libs/libWTSUtils.a
	
	${MyDeps}/lib/libmysqlclient.a
	boost_filesystem
	pthread
	dl
)
ADD_EXECUTABLE(TestBtPorter ../TestBtPorter/TestBtPorter.cpp)
TARGET_LINK_LIBRARIES(TestBtPorter ${LIBS})

