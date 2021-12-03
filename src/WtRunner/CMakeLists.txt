#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtRunner LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

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

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/WtRunner)

#7. 添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

IF(WIN32)
	SET(LIBS
		WtCore
		WTSTools
		WTSUtils

		dl
		pthread
		boost_filesystem
		boost_thread
		ws2_32
	)
ELSE(UNIX)
	SET(LIBS
		WtCore
		WTSTools
		WTSUtils

		dl
		pthread
		boost_filesystem
		boost_thread
	)
ENDIF()

ADD_EXECUTABLE(WtRunner ${SRCS} ${HDRS})
TARGET_LINK_LIBRARIES(WtRunner ${LIBS})

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/parsers/)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/traders/)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/executer/)

IF(WIN32)
	add_custom_command(TARGET WtRunner POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserUDP.dll ${EXECUTABLE_OUTPUT_PATH}/parsers/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTP.dll ${EXECUTABLE_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderXTP.dll ${EXECUTABLE_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderMocker.dll ${EXECUTABLE_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPMini.dll ${EXECUTABLE_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPOpt.dll ${EXECUTABLE_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderFemas.dll ${EXECUTABLE_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderiTap.dll ${EXECUTABLE_OUTPUT_PATH}/traders/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtExeFact.dll ${EXECUTABLE_OUTPUT_PATH}/executer/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataReader.dll ${EXECUTABLE_OUTPUT_PATH}/
		
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtRiskMonFact.dll ${EXECUTABLE_OUTPUT_PATH}/
		
		COMMENT "depends modules copied")
ELSE(UNIX)
	add_custom_command(TARGET WtRunner POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libParserUDP.so ${EXECUTABLE_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTP.so ${EXECUTABLE_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderXTP.so ${EXECUTABLE_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderMocker.so ${EXECUTABLE_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPMini.so ${EXECUTABLE_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderCTPOpt.so ${EXECUTABLE_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderFemas.so ${EXECUTABLE_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libTraderiTap.so ${EXECUTABLE_OUTPUT_PATH}/traders/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtExeFact.so ${EXECUTABLE_OUTPUT_PATH}/executer/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtDataReader.so ${EXECUTABLE_OUTPUT_PATH}/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/libWtRiskMonFact.so ${EXECUTABLE_OUTPUT_PATH}/

		COMMENT "depends modules copied")
ENDIF()


