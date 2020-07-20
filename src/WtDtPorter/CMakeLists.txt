
#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtDtPorter LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

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

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/bin/WtDtPorter)

#7. 添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})
ADD_LIBRARY(WtDtPorter SHARED ${SRCS} ${HDRS})

SET(LIBS
	${PROJECT_SOURCE_DIR}/../build/libs/libWtDtCore.a
	${PROJECT_SOURCE_DIR}/../build/libs/libWTSTools.a
	${PROJECT_SOURCE_DIR}/../build/libs/libWTSUtils.a
	
	dl
	pthread
	boost_filesystem	
)

TARGET_LINK_LIBRARIES(WtDtPorter ${LIBS})
IF (WIN32)
ELSE (UNIX)
	SET_TARGET_PROPERTIES(WtDtPorter PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1)
ENDIF ()

add_custom_command(TARGET WtDtPorter POST_BUILD		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libWtDataWriter.so ${LIBRARY_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserCTP.so ${LIBRARY_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserXTP.so ${LIBRARY_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserCTPMini.so ${LIBRARY_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserFemas.so ${LIBRARY_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParseriTap.so ${LIBRARY_OUTPUT_PATH}/
		
		COMMENT "depends modules copied")
		