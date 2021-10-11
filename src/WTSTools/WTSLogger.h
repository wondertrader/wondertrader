/*!
 * \file WTSLogger.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 日志模块定义
 */
#pragma once
#include "../Includes/WTSTypes.h"
#include "../Includes/WTSCollection.hpp"

#include <memory>
#include <sstream>
#include <thread>
#include <set>

#include <spdlog/spdlog.h>

typedef std::shared_ptr<spdlog::logger> SpdLoggerPtr;

NS_OTP_BEGIN
class ILogHandler;
class WTSVariant;
NS_OTP_END

USING_NS_OTP;

#define MAX_LOG_BUF_SIZE 2048

class WTSLogger
{
private:
	static void debug_imp(SpdLoggerPtr logger, const char* message);
	static void info_imp(SpdLoggerPtr logger, const char* message);
	static void warn_imp(SpdLoggerPtr logger, const char* message);
	static void error_imp(SpdLoggerPtr logger, const char* message);
	static void fatal_imp(SpdLoggerPtr logger, const char* message);

	static void initLogger(const char* catName, WTSVariant* cfgLogger);

public:
	static void debug(const char* format, ...);
	static void info(const char* format, ...);
	static void warn(const char* format, ...);
	static void error(const char* format, ...);
	static void fatal(const char* format, ...);
	static void log(WTSLogLevel ll, const char* format, ...);
	static void vlog(WTSLogLevel ll, const char* format, va_list& args);
	static void log_raw(WTSLogLevel ll, const char* message);

	static void debug2(const char* catName, const char* format, ...);
	static void info2(const char* catName, const char* format, ...);
	static void warn2(const char* catName, const char* format, ...);
	static void error2(const char* catName, const char* format, ...);
	static void fatal2(const char* catName, const char* format, ...);
	static void log2(const char* catName, WTSLogLevel ll, const char* format, ...);
	static void vlog2(const char* catName, WTSLogLevel ll, const char* format, va_list& args);
	static void log2_raw(const char* catName, WTSLogLevel ll, const char* message);

	static void log_dyn(const char* patttern, const char* catName, WTSLogLevel ll, const char* format, ...);
	static void vlog_dyn(const char* patttern, const char* catName, WTSLogLevel ll, const char* format, va_list& args);
	static void log_dyn_raw(const char* patttern, const char* catName, WTSLogLevel ll, const char* message);

	static void init(const char* propFile = "logcfg.json", bool isFile = true, ILogHandler* handler = NULL, WTSLogLevel logLevel = LL_DEBUG);

	static void registerHandler(ILogHandler* handler = NULL, WTSLogLevel logLevel = LL_DEBUG);

	static void stop();

	static void freeAllDynLoggers();

	static SpdLoggerPtr getLogger(const char* logger, const char* pattern = "");

private:
	static bool					m_bInited;
	static bool					m_bTpInited;
	static bool					m_bStopped;
	static ILogHandler*			m_logHandler;
	static WTSLogLevel			m_logLevel;

	static SpdLoggerPtr			m_rootLogger;

	typedef WTSHashMap<std::string>	LogPatterns;
	static LogPatterns*			m_mapPatterns;
	static std::set<std::string>	m_setDynLoggers;

	static thread_local char	m_buffer[MAX_LOG_BUF_SIZE];
};


