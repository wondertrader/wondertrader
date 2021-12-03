#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtMsgQue LANGUAGES CXX)
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

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin)

#7. 添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})
ADD_LIBRARY(WtMsgQue SHARED ${SRCS} ${HDRS})

IF(MSVC)
	SET(LIBS
		nanomsg
		ws2_32
		mswsock
		)
ELSE(GCC)
	IF(WIN32)
		SET(LIBS
			dl
			pthread
			nanomsg
			ws2_32
			mswsock
		)
	ELSE(UNIX)
		SET(LIBS
			dl
			pthread
			nanomsg
		)
	ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(WtMsgQue ${LIBS})
IF (MSVC)
ELSE (GCC)
	SET_TARGET_PROPERTIES(WtMsgQue PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1)
ENDIF ()