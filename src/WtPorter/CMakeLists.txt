#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtPorter LANGUAGES CXX)
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

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtPorter)

#7. 添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

IF(MSVC)
	list (APPEND SRCS ../Common/mdump.cpp)
ENDIF()

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})
ADD_LIBRARY(WtPorter SHARED ${SRCS} ${HDRS})

SET(LIBS WtCore WTSTools WTSUtils)

IF (MSVC)
	LIST(APPEND LIBS ws2_32)
ELSE(GNU)
	LIST(APPEND LIBS
		dl
		pthread
		boost_filesystem
		boost_thread)
	IF(WIN32)
		LIST(APPEND LIBS
			ws2_32)
	ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(WtPorter ${LIBS})
IF (MSVC)
ELSE (GNU)
	SET_TARGET_PROPERTIES(WtPorter PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1)
ENDIF ()

file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH}/parsers/)
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH}/traders/)
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH}/executer/)

IF(WIN32)
	add_custom_command(TARGET WtPorter POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserUDP.dll ${LIBRARY_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTP.dll ${LIBRARY_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderXTP.dll ${LIBRARY_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderMocker.dll ${LIBRARY_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPMini.dll ${LIBRARY_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPOpt.dll ${LIBRARY_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderFemas.dll ${LIBRARY_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderiTap.dll ${LIBRARY_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtExeFact.dll ${LIBRARY_OUTPUT_PATH}/executer/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataReader.dll ${LIBRARY_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtRiskMonFact.dll ${LIBRARY_OUTPUT_PATH}/
        
        COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtMsgQue.dll ${LIBRARY_OUTPUT_PATH}/
		
		COMMENT "depends modules copied")
ELSE(UNIX)
	add_custom_command(TARGET WtPorter POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserUDP.so ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTP.so ${LIBRARY_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderXTP.so ${LIBRARY_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderMocker.so ${LIBRARY_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPMini.so ${LIBRARY_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPOpt.so ${LIBRARY_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderFemas.so ${LIBRARY_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderiTap.so ${LIBRARY_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtExeFact.so ${LIBRARY_OUTPUT_PATH}/executer/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataReader.so ${LIBRARY_OUTPUT_PATH}/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtRiskMonFact.so ${LIBRARY_OUTPUT_PATH}/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtMsgQue.so ${LIBRARY_OUTPUT_PATH}/

		COMMENT "depends modules copied")
ENDIF()


