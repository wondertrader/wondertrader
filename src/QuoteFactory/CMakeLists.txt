
#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(QuoteFactory LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

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

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/bin/QuoteFactory)

#7. 添加源码
FILE( GLOB SRCS *.cpp)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

SET(LIBS
	${PROJECT_SOURCE_DIR}/../build/libs/libWTSTools.a
	${PROJECT_SOURCE_DIR}/../build/libs/libWTSUtils.a
	${PROJECT_SOURCE_DIR}/../build/libs/libWtDtCore.a
	
	boost_thread
	boost_filesystem
	
	pthread
	dl
)

#9. 生成可执行文件
ADD_EXECUTABLE(QuoteFactory ${SRCS})
TARGET_LINK_LIBRARIES(QuoteFactory ${LIBS})

file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH}/parsers/)

add_custom_command(TARGET QuoteFactory POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserCTP.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserXTP.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParseriTap.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserFemas.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libParserCTPMini.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build/bin/libWtDataWriter.so ${EXECUTABLE_OUTPUT_PATH}/
		
		COMMENT "depends modules copied")


