#pragma once
#include "../Share/WTSTypes.h"
#include "../Share/WTSCollection.hpp"
#include <memory>

#include <spdlog/spdlog.h>

typedef std::shared_ptr<spdlog::logger> SpdLoggerPtr;

NS_OTP_BEGIN
class ILogHandler;
class WTSVariant;
NS_OTP_END

USING_NS_OTP;


class WTSLogger
{
private:
	static void debug_imp(SpdLoggerPtr logger, const char* format, va_list args);
	static void info_imp(SpdLoggerPtr logger, const char* format, va_list args);
	static void warn_imp(SpdLoggerPtr logger, const char* format, va_list args);
	static void error_imp(SpdLoggerPtr logger, const char* format, va_list args);
	static void fatal_imp(SpdLoggerPtr logger, const char* format, va_list args);	

	static void initLogger(const char* catName, WTSVariant* cfgLogger);

public:
	static void debug(const char* format, ...);
	static void info(const char* format, ...);
	static void warn(const char* format, ...);
	static void error(const char* format, ...);
	static void fatal(const char* format, ...);
	static void log(WTSLogLevel ll, const char* format, ...);
	static void log_direct(WTSLogLevel ll, const char* format, va_list args);
	static void log_raw(WTSLogLevel ll, const char* message);

	static void debug2(const char* catName, const char* format, ...);
	static void info2(const char* catName, const char* format, ...);
	static void warn2(const char* catName, const char* format, ...);
	static void error2(const char* catName, const char* format, ...);
	static void fatal2(const char* catName, const char* format, ...);
	static void log2(const char* catName, WTSLogLevel ll, const char* format, ...);
	static void log2_direct(const char* catName, WTSLogLevel ll, const char* format, va_list args);
	static void log2_raw(const char* catName, WTSLogLevel ll, const char* message);

	static void log_dyn(const char* patttern, const char* catName, WTSLogLevel ll, const char* format, ...);
	static void log_dyn_direct(const char* patttern, const char* catName, WTSLogLevel ll, const char* format, va_list args);
	static void log_dyn_raw(const char* patttern, const char* catName, WTSLogLevel ll, const char* message);

	static void init(const char* propFile = "logcfg.json", ILogHandler* handler = NULL, WTSLogLevel logLevel = LL_INFO);

	static void registerHandler(ILogHandler* handler = NULL, WTSLogLevel logLevel = LL_INFO);

	static void stop();

	static SpdLoggerPtr getLogger(const char* logger, const char* pattern = "");

private:
	static bool					m_bStopped;
	static ILogHandler*			m_logHandler;
	static WTSLogLevel			m_logLevel;

	static SpdLoggerPtr			m_rootLogger;

	typedef WTSHashMap<std::string>	LogPatterns;
	static LogPatterns*			m_mapPatterns;
};

