#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtDtPorter LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

IF(GCC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

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

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtDtPorter)

#7. 添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

IF(MSVC)
	list (APPEND SRCS ../Common/mdump.cpp)
ENDIF()

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})
ADD_LIBRARY(WtDtPorter SHARED ${SRCS} ${HDRS})

SET(LIBS
	WtDtCore
	WTSTools
	WTSUtils
	)
IF(MSVC)
ELSE(GCC)
	LIST(APPEND LIBS
		dl
		pthread
		boost_filesystem
		boost_thread
		)
	IF(WIN32)
		LIST(APPEND LIBS
			ws2_32
		)
	ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(WtDtPorter ${LIBS})
IF (MSVC)
ELSE (GCC)
	SET_TARGET_PROPERTIES(WtDtPorter PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1)
ENDIF ()

file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH}/parsers/)

IF(WIN32)
	add_custom_command(TARGET WtDtPorter POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataWriter.dll ${LIBRARY_OUTPUT_PATH}/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTP.dll ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserXTP.dll ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTPMini.dll ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTPOpt.dll ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserFemas.dll ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParseriTap.dll ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMENT "depends modules copied")
ELSE(UNIX)
	add_custom_command(TARGET WtDtPorter POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataWriter.so ${LIBRARY_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTP.so ${LIBRARY_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserXTP.so ${LIBRARY_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTPMini.so ${LIBRARY_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserCTPOpt.so ${LIBRARY_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserFemas.so ${LIBRARY_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParseriTap.so ${LIBRARY_OUTPUT_PATH}/parsers/
		
		COMMENT "depends modules copied")
ENDIF()
		