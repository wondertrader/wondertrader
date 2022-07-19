/*!
 * \file WTSLogger.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief ��־ģ�鶨��
 */
#pragma once
#include "../Includes/WTSTypes.h"
#include "../Includes/WTSCollection.hpp"
#include "../Share/fmtlib.h"

#include <memory>
#include <sstream>
#include <thread>
#include <set>

//By Wesley @ 2022.01.05
//spdlog������1.9.2
//ͬʱʹ���ⲿ��fmt 8.1.0
#include <spdlog/spdlog.h>

typedef std::shared_ptr<spdlog::logger> SpdLoggerPtr;

NS_WTP_BEGIN
class ILogHandler;
class WTSVariant;
NS_WTP_END

USING_NS_WTP;

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
	static SpdLoggerPtr getLogger(const char* logger, const char* pattern = "");

	static void print_message(const char* buffer);

public:
	/*
	 *	ֱ�����
	 */
	static void log_raw(WTSLogLevel ll, const char* message);

	/*
	 *	�������
	 */
	static void log_raw_by_cat(const char* catName, WTSLogLevel ll, const char* message);

	/*
	 *	��̬�������
	 */
	static void log_dyn_raw(const char* patttern, const char* catName, WTSLogLevel ll, const char* message);


//////////////////////////////////////////////////////////////////////////
//fmt::format���ӿ�
public:
	template<typename... Args>
	static void debug(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_DEBUG || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		debug_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void info(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_INFO || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		info_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void warn(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_WARN || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		warn_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void error(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_ERROR || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		error_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void fatal(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_FATAL || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		fatal_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void log(WTSLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		log_raw(ll, m_buffer);
	}

	template<typename... Args>
	static void log_by_cat(const char* catName, WTSLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		log_raw_by_cat(catName, ll, m_buffer);
	}

	template<typename... Args>
	static void log_dyn(const char* patttern, const char* catName, WTSLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, format, args...);

		log_dyn_raw(patttern, catName, ll, m_buffer);
	}

public:
	static void init(const char* propFile = "logcfg.json", bool isFile = true, ILogHandler* handler = NULL);

	static void registerHandler(ILogHandler* handler = NULL);

	static void stop();

	static void freeAllDynLoggers();

private:
	static bool					m_bInited;
	static bool					m_bTpInited;
	static bool					m_bStopped;
	static ILogHandler*			m_logHandler;
	static WTSLogLevel			m_logLevel;

	static SpdLoggerPtr			m_rootLogger;

	typedef WTSHashMap<ShortKey>	LogPatterns;
	static LogPatterns*				m_mapPatterns;
	static std::set<std::string>	m_setDynLoggers;

	static thread_local char	m_buffer[MAX_LOG_BUF_SIZE];
};


