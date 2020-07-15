CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(WTSDemo)

#3. 环境判断
IF (WIN32)
	MESSAGE(STATUS "Operation System is windows")
	MESSAGE(STATUS "Generator is ${CMAKE_GENERATOR}")
	IF("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		MESSAGE(STATUS "MyDepends is at $ENV{MyX64Deps12}")
		MESSAGE(STATUS "Platform is x64")
	ELSE()
		MESSAGE(STATUS "MyDepends is at $ENV{MyDepends12}")
		MESSAGE(STATUS "Platform is x86")
	ENDIF()
ELSEIF (UNIX)
	MESSAGE(STATUS "Operation System is UNIX-like OS's")
ENDIF ()

ADD_SUBDIRECTORY(WtExeFact)
ADD_SUBDIRECTORY(WtCtaStraFact)
ADD_SUBDIRECTORY(WtHftStraFact)
ADD_SUBDIRECTORY(WtSelStraFact)

