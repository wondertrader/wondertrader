CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(WTSOtp)

#3. 环境判断
IF (WIN32)
	MESSAGE(STATUS "Operation System is windows")
	MESSAGE(STATUS "Generator is ${CMAKE_GENERATOR}")
	IF("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		MESSAGE(STATUS "MyDepends is at $ENV{MyDepends141}")
		MESSAGE(STATUS "Platform is x64")
	ELSE()
		MESSAGE(STATUS "MyDepends is at $ENV{MyDepends141}")
		MESSAGE(STATUS "Platform is x86")
	ENDIF()
ELSEIF (UNIX)
	MESSAGE(STATUS "Operation System is UNIX-like OS's")
ENDIF ()

#basic libraries
ADD_SUBDIRECTORY(WTSUtils)
ADD_SUBDIRECTORY(WTSTools)

#contract Loade through ctp channel
ADD_SUBDIRECTORY(CTPLoader)
ADD_SUBDIRECTORY(MiniLoader)

#parser for quote date from ctp channel
ADD_SUBDIRECTORY(ParserCTP)
ADD_SUBDIRECTORY(ParserCTPMini)
ADD_SUBDIRECTORY(ParserFemas)
ADD_SUBDIRECTORY(ParseriTap)
ADD_SUBDIRECTORY(ParserXTP)
ADD_SUBDIRECTORY(WtDataWriter)
ADD_SUBDIRECTORY(WtDtCore)
ADD_SUBDIRECTORY(WtDtPorter)
#quote date processor
ADD_SUBDIRECTORY(QuoteFactory)

#back test core library
ADD_SUBDIRECTORY(WtBtCore)
#back test c porter for outter libraries
ADD_SUBDIRECTORY(WtBtPorter)

ADD_SUBDIRECTORY(WtBtRunner)

ADD_SUBDIRECTORY(ParserUDP)
ADD_SUBDIRECTORY(TraderCTP)
ADD_SUBDIRECTORY(TraderCTPMini)
ADD_SUBDIRECTORY(TraderFemas)
ADD_SUBDIRECTORY(TraderMocker)
ADD_SUBDIRECTORY(TraderXTP)
ADD_SUBDIRECTORY(TraderiTap)
ADD_SUBDIRECTORY(WtExeFact)
ADD_SUBDIRECTORY(WtRiskMonFact)
ADD_SUBDIRECTORY(WtDataReader)

ADD_SUBDIRECTORY(WtCore)
ADD_SUBDIRECTORY(WtPorter)
ADD_SUBDIRECTORY(WtExecMon)

