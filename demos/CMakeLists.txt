CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(WTSDemo)

#3. 环境判断
IF (WIN32)
	MESSAGE(STATUS "Operation System is windows")
ELSEIF (UNIX)
	MESSAGE(STATUS "Operation System is UNIX-like OS's")
ENDIF ()

MESSAGE(STATUS "Generator is ${CMAKE_GENERATOR}")

IF (MSVC)
	ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
	IF(CMAKE_CL_64)
		SET(INCS $ENV{MyDepends141}/include ${PROJECT_SOURCE_DIR}/../src)
		SET(LNKS $ENV{MyDepends141}/lib/x64)

		MESSAGE(STATUS "MyDepends is at $ENV{MyDepends141}")
		MESSAGE(STATUS "Platform is x64")
		SET(PLATFORM x64)
	ELSE()
		SET(INCS $ENV{MyDepends141}/include ${PROJECT_SOURCE_DIR}/../src)
		SET(LNKS $ENV{MyDepends141}/lib/x86)

		MESSAGE(STATUS "MyDepends is at $ENV{MyDepends141}")
		MESSAGE(STATUS "Platform is x86")
		SET(PLATFORM x86)
	ENDIF()
	SET(PREFIX )
	SET(SUFFIX .dll)
ELSE (GNUCC)
	IF(WIN32)
		SET (MyDeps $ENV{MyDependsGcc})
		SET(SUFFIX .dll)

		MESSAGE(STATUS "MyDepends is at $ENV{MyDependsGcc}")
	ELSE(UNIX)
		SET (MyDeps "/home/mydeps")
		SET(SUFFIX .so)

		MESSAGE(STATUS "MyDepends is at /home/mydeps")
	ENDIF()

	SET(INCS ${MyDeps}/include ${PROJECT_SOURCE_DIR}/../src)
	SET(LNKS ${MyDeps}/lib)

	MESSAGE(STATUS "Platform is x64")
	SET(PLATFORM x64)
	SET(PREFIX lib)
ENDIF()

ADD_SUBDIRECTORY(WtExeFact)
ADD_SUBDIRECTORY(WtCtaStraFact)
ADD_SUBDIRECTORY(WtHftStraFact)
ADD_SUBDIRECTORY(WtSelStraFact)

