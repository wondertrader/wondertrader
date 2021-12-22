#1. 确定CMake的最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#2. 确定工程名
PROJECT(WtHftStraFact LANGUAGES CXX)
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

#7. 添加源码
file(GLOB SRCS "*.cpp")
file(GLOB HDRS "*.h")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})
ADD_LIBRARY(WtHftStraFact SHARED ${SRCS} ${HDRS})

SET(LIBS
	boost_filesystem
	dl
)

IF (WIN32)
ELSE (UNIX)
	TARGET_LINK_LIBRARIES(WtHftStraFact ${LIBS})
	SET_TARGET_PROPERTIES(WtHftStraFact PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
ENDIF ()

