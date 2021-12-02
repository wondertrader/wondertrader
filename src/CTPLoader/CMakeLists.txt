
#1. 确定CMake的最低版本需求
cmake_minimum_required(VERSION 3.0.0)

#2. 确定工程名
project(CTPLoader LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)


#4. 添加包含文件目录
IF (MSVC)
	message("MSVC ON WIN32")
	IF("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		SET(INCS $ENV{MyDepends141}/include)
		SET(LNKS $ENV{MyDepends141}/lib/x64)
	ELSE()
		SET(INCS $ENV{MyDepends141}/include)
		SET(LNKS $ENV{MyDepends141}/lib/x86)
	ENDIF()
ELSE (GCC)
	IF(WIN32)
		message("GCC ON WIN32")
		SET (MyDeps $ENV{MyDependsGcc})
		message("${MyDeps}")
	ELSE()
		SET (MyDeps "/home/mydeps")
	ENDIF()
	SET(INCS ${MyDeps}/include)
	SET(LNKS ${MyDeps}/lib)
ENDIF ()

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build_${CMAKE_BUILD_TYPE}/bin/CTPLoader)

#7. 添加源码
SET(SRC  
	${PROJECT_SOURCE_DIR}/CTPLoader.cpp
	${PROJECT_SOURCE_DIR}/TraderSpi.cpp
	${PROJECT_SOURCE_DIR}/TraderSpi.h
)

INCLUDE_DIRECTORIES(${INCS})
LINK_DIRECTORIES(${LNKS})

SET(LIBS	
	boost_filesystem
	dl
)

#9. 生成可执行文件
ADD_LIBRARY(CTPLoader SHARED ${SRC})
IF (MSVC)
ELSE (GCC)
	TARGET_LINK_LIBRARIES(CTPLoader ${LIBS})
ENDIF ()


