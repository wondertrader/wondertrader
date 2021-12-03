#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(QuoteFactory LANGUAGES CXX)
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
ELSE (GCC)
	IF(WIN32)
		SET (MyDeps $ENV{MyDependsGcc})
	ELSE()
		SET (MyDeps "/home/mydeps")
	ENDIF()
	SET(INCS ${MyDeps}/include)
	SET(LNKS ${MyDeps}/lib)
ENDIF ()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/QuoteFactory)

#7. 添加源码
FILE( GLOB SRCS *.cpp)

IF(MSVC)
	list (APPEND SRCS ../Common/mdump.cpp)
ENDIF()

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

SET(LIBS
	WTSTools
	WTSUtils
	WtDtCore)

IF(MSVC)
	LIST(APPEND LIBS ws2_32)
ELSE(GCC)
	LIST(APPEND LIBS
		dl
		boost_filesystem
		)
	if(WIN32)
		LIST(APPEND LIBS
			ws2_32)
	ENDIF()
ENDIF()

#9. 生成可执行文件
ADD_EXECUTABLE(QuoteFactory ${SRCS})
TARGET_LINK_LIBRARIES(QuoteFactory ${LIBS})

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/parsers/)

IF(WIN32)
	add_custom_command(TARGET QuoteFactory POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTP.dll ${EXECUTABLE_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserXTP.dll ${EXECUTABLE_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParseriTap.dll ${EXECUTABLE_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserFemas.dll ${EXECUTABLE_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTPMini.dll ${EXECUTABLE_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataWriter.dll ${EXECUTABLE_OUTPUT_PATH}/

		COMMENT "depends modules copied")
ELSE(UNIX)
	add_custom_command(TARGET QuoteFactory POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTP.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserXTP.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParseriTap.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserFemas.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTPMini.so ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataWriter.so ${EXECUTABLE_OUTPUT_PATH}/
		
		COMMENT "depends modules copied")
ENDIF()


