/*!
 * \file WTSLogger.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include <stdio.h>
#include <iostream>

#include "WTSLogger.h"
#include "../Includes/ILogHandler.h"
#include "../Share/JsonToVariant.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/StrUtil.hpp"

#include <boost/filesystem.hpp>

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/async.h>


const char* DYN_PATTERN = "dyn_pattern";

const int	MAX_BUFFER_LENGTH = 2048;
ILogHandler*		WTSLogger::m_logHandler	= NULL;
WTSLogLevel			WTSLogger::m_logLevel	= LL_ALL;
bool				WTSLogger::m_bStopped = false;
SpdLoggerPtr		WTSLogger::m_rootLogger = NULL;
WTSLogger::LogPatterns*	WTSLogger::m_mapPatterns = NULL;

inline spdlog::level::level_enum str_to_level( const char* slvl)
{
	if(strcmp(slvl, "debug"))
	{
		return spdlog::level::debug;
	}
	else if (strcmp(slvl, "info"))
	{
		return spdlog::level::info;
	}
	else if (strcmp(slvl, "warn"))
	{
		return spdlog::level::warn;
	}
	else if (strcmp(slvl, "error"))
	{
		return spdlog::level::err;
	}
	else if (strcmp(slvl, "fatal"))
	{
		return spdlog::level::critical;
	}
	else
	{
		return spdlog::level::off;
	}
}

inline void checkDirs(const char* filename)
{
	std::string s = StrUtil::standardisePath(filename, false);
	std::size_t pos = s.find_last_of('/');

	if (pos == std::string::npos)
		return;

	pos++;

	if (!StdFile::exists(s.substr(0, pos).c_str()))
		boost::filesystem::create_directories(s.substr(0, pos).c_str());
}

void WTSLogger::initLogger(const char* catName, WTSVariant* cfgLogger)
{
	bool bAsync = cfgLogger->getBoolean("async");
	const char* level = cfgLogger->getCString("level");

	WTSVariant* cfgSinks = cfgLogger->get("sinks");
	std::vector<spdlog::sink_ptr> sinks;
	for (uint32_t idx = 0; idx < cfgSinks->size(); idx++)
	{
		WTSVariant* cfgSink = cfgSinks->get(idx);
		const char* type = cfgSink->getCString("type");
		if (strcmp(type, "daily_file_sink") == 0)
		{
			std::string filename = cfgSink->getString("filename");
			StrUtil::replace(filename, "%s", catName);
			checkDirs(filename.c_str());
			auto sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filename, 0, 0);
			sink->set_pattern(cfgSink->getCString("pattern"));
			sinks.push_back(sink);
		}
		else if (strcmp(type, "basic_file_sink") == 0)
		{
			std::string filename = cfgSink->getString("filename");
			StrUtil::replace(filename, "%s", catName);
			checkDirs(filename.c_str());
			auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, cfgSink->getBoolean("truncate"));
			sink->set_pattern(cfgSink->getCString("pattern"));
			sinks.push_back(sink);
		}
		else if (strcmp(type, "console_sink") == 0)
		{
			auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			sink->set_pattern(cfgSink->getCString("pattern"));
			sinks.push_back(sink);
		}
		else if (strcmp(type, "ostream_sink") == 0)
		{
			auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout, true);
			sink->set_pattern(cfgSink->getCString("pattern"));
			sinks.push_back(sink);
		}
	}

	if (!bAsync)
	{
		auto logger = std::make_shared<spdlog::logger>(catName, sinks);
		logger->set_level(str_to_level(cfgLogger->getCString("level")));
		spdlog::register_logger(logger);
	}
	else
	{
		auto logger = std::make_shared<spdlog::async_logger>(catName, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
		logger->set_level(str_to_level(cfgLogger->getCString("level")));
		spdlog::register_logger(logger);
	}
}

void WTSLogger::init(const char* propFile /* = "log4cxx.prop" */,
	ILogHandler* handler /* = NULL */,
	WTSLogLevel logLevel /* = LL_ALL */)
{
	if (!StdFile::exists(propFile))
		return;

	std::string content;
	StdFile::read_file_content(propFile, content);

	rj::Document root;
	if (root.Parse(content.c_str()).HasParseError())
	{
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	jsonToVariant(root, cfg);

	auto keys = cfg->memberNames();
	for (std::string& key : keys)
	{
		WTSVariant* cfgItem = cfg->get(key.c_str());
		if (key == DYN_PATTERN)
		{
			auto pkeys = cfgItem->memberNames();
			for(std::string& pkey : pkeys)
			{
				WTSVariant* cfgPattern = cfgItem->get(pkey.c_str());
				if (m_mapPatterns == NULL)
					m_mapPatterns = LogPatterns::create();

				m_mapPatterns->add(pkey.c_str(), cfgPattern, true);
			}
			continue;
		}

		initLogger(key.c_str(), cfgItem);
	}

	m_rootLogger = getLogger("root");
	spdlog::set_default_logger(m_rootLogger);
	spdlog::flush_every(std::chrono::seconds(2));

	m_logHandler = handler;
	m_logLevel = logLevel;
}

void WTSLogger::registerHandler(ILogHandler* handler /* = NULL */, WTSLogLevel logLevel /* = LL_ALL */)
{
	m_logHandler = handler;
	m_logLevel = logLevel;
}

void WTSLogger::stop()
{
	m_bStopped = true;
	if (m_mapPatterns)
		m_mapPatterns->release();
	spdlog::shutdown();
}

void WTSLogger::debug(const char* format, ...)
{
	if(m_logLevel > LL_DEBUG || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	debug_imp(m_rootLogger, szBuf);
}

void WTSLogger::info(const char* format, ...)
{
	if(m_logLevel > LL_INFO || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	info_imp(m_rootLogger, szBuf);
}

void WTSLogger::warn(const char* format, ...)
{
	if(m_logLevel > LL_WARN || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	warn_imp(m_rootLogger, szBuf);
}

void WTSLogger::error(const char* format, ...)
{
	if(m_logLevel > LL_ERROR || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	error_imp(m_rootLogger, szBuf);
}

void WTSLogger::fatal(const char* format, ...)
{
	if(m_logLevel > LL_FATAL || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	fatal_imp(m_rootLogger, szBuf);
}

void WTSLogger::log(WTSLogLevel ll, const char* format, ...)
{
	if(m_logLevel > ll || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	switch(ll)
	{
	case LL_DEBUG:
		debug_imp(m_rootLogger, szBuf);
		break;
	case LL_INFO:
		info_imp(m_rootLogger, szBuf);
		break;
	case LL_WARN:
		warn_imp(m_rootLogger, szBuf);
		break;
	case LL_ERROR:
		error_imp(m_rootLogger, szBuf);
		break;
	case LL_FATAL:
		fatal_imp(m_rootLogger, szBuf);
		break;
	default:
		break;
	}
}

void WTSLogger::debug_imp(SpdLoggerPtr logger, const char* message)
{
	if (logger)
		logger->debug(message);

	if (logger != m_rootLogger)
		m_rootLogger->debug(message);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_DEBUG, message);
}

void WTSLogger::info_imp(SpdLoggerPtr logger, const char* message)
{
	if (logger)
		logger->info(message);

	if (logger != m_rootLogger)
		m_rootLogger->info(message);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_INFO, message);
}

void WTSLogger::warn_imp(SpdLoggerPtr logger, const char* message)
{
	if (logger)
		logger->warn(message);

	if (logger != m_rootLogger)
		m_rootLogger->warn(message);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_WARN, message);
}

void WTSLogger::error_imp(SpdLoggerPtr logger, const char* message)
{
	if (logger)
		logger->error(message);

	if (logger != m_rootLogger)
		m_rootLogger->error(message);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_ERROR, message);
}

void WTSLogger::fatal_imp(SpdLoggerPtr logger, const char* message)
{
	if (logger)
		logger->critical(message);

	if (logger != m_rootLogger)
		m_rootLogger->critical(message);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_FATAL, message);
}

void WTSLogger::log_raw(WTSLogLevel ll, const char* message)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	auto logger = m_rootLogger;

	if (logger)
	{
		switch (ll)
		{
		case LL_DEBUG:
			debug_imp(logger, message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_DEBUG, message);
			break;
		case LL_INFO:
			info_imp(logger, message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_INFO, message);
			break;
		case LL_WARN:
			warn_imp(logger, message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_WARN, message);
			break;
		case LL_ERROR:
			error_imp(logger, message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_ERROR, message);
			break;
		case LL_FATAL:
			fatal_imp(logger, message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_FATAL, message);
			break;
		default:
			break;
		}
	}
}


void WTSLogger::vlog(WTSLogLevel ll, const char* format, va_list args)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);

	switch (ll)
	{
	case LL_DEBUG:
		debug_imp(m_rootLogger, szBuf);
		break;
	case LL_INFO:
		info_imp(m_rootLogger, szBuf);
		break;
	case LL_WARN:
		warn_imp(m_rootLogger, szBuf);
		break;
	case LL_ERROR:
		error_imp(m_rootLogger, szBuf);
		break;
	case LL_FATAL:
		fatal_imp(m_rootLogger, szBuf);
		break;
	default:
		break;
	}
}

void WTSLogger::debug2(const char* catName, const char* format, ...)
{
	if(m_logLevel > LL_DEBUG || m_bStopped)
		return;

	va_list args;
	va_start(args, format);  
	auto logger = getLogger(catName);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	debug_imp(logger, szBuf);
}

void WTSLogger::info2(const char* catName, const char* format, ...)
{
	if(m_logLevel > LL_INFO || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	auto logger = getLogger(catName);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	info_imp(logger, szBuf);
}

void WTSLogger::warn2(const char* catName, const char* format, ...)
{
	if(m_logLevel > LL_WARN || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	auto logger = getLogger(catName);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	warn_imp(logger, szBuf);
}

void WTSLogger::error2(const char* catName, const char* format, ...)
{
	if(m_logLevel > LL_ERROR || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	auto logger = getLogger(catName);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	error_imp(logger, szBuf);
}

void WTSLogger::fatal2(const char* catName, const char* format, ...)
{
	if(m_logLevel > LL_FATAL || m_bStopped)
		return;

	va_list args;
	va_start(args, format);
	auto logger = getLogger(catName);
	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);
	va_end(args);

	fatal_imp(logger, szBuf);
}

void WTSLogger::log2(const char* catName, WTSLogLevel ll, const char* format, ...)
{
	if(m_logLevel > ll || m_bStopped)
		return;

	va_list args;
	va_start(args, format);        

	log2_direct(catName, ll, format, args);

	va_end(args);
}

void WTSLogger::log2_raw(const char* catName, WTSLogLevel ll, const char* message)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	auto logger = getLogger(catName);

	if (logger)
	{
		switch (ll)
		{
		case LL_DEBUG:
			logger->debug(message); 
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_DEBUG, message);
			break;
		case LL_INFO:
			logger->info(message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_INFO, message);
			break;
		case LL_WARN:
			logger->warn(message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_WARN, message);
			break;
		case LL_ERROR:
			logger->error(message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_ERROR, message);
			break;
		case LL_FATAL:
			logger->critical(message);
			if (m_logHandler)
				m_logHandler->handleLogAppend(LL_FATAL, message);
			break;
		default:
			break;
		}
	}	
}

void WTSLogger::log2_direct(const char* catName, WTSLogLevel ll, const char* format, va_list args)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	auto logger = getLogger(catName);

	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);

	switch (ll)
	{
	case LL_DEBUG:
		debug_imp(logger, szBuf);
		break;
	case LL_INFO:
		info_imp(logger, szBuf);
		break;
	case LL_WARN:
		warn_imp(logger, szBuf);
		break;
	case LL_ERROR:
		error_imp(logger, szBuf);
		break;
	case LL_FATAL:
		fatal_imp(logger, szBuf);
		break;
	default:
		break;
	}
}

void WTSLogger::log_dyn(const char* patttern, const char* catName, WTSLogLevel ll, const char* format, ...)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	va_list args;
	va_start(args, format);

	vlog_dyn(patttern, catName, ll, format, args);

	va_end(args);
}

void WTSLogger::vlog_dyn(const char* patttern, const char* catName, WTSLogLevel ll, const char* format, va_list args)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	auto logger = getLogger(catName, patttern);

	char szBuf[MAX_BUFFER_LENGTH] = { 0 };
	vsprintf(szBuf, format, args);

	switch (ll)
	{
	case LL_DEBUG:
		debug_imp(logger, szBuf);
		break;
	case LL_INFO:
		info_imp(logger, szBuf);
		break;
	case LL_WARN:
		warn_imp(logger, szBuf);
		break;
	case LL_ERROR:
		error_imp(logger, szBuf);
		break;
	case LL_FATAL:
		fatal_imp(logger, szBuf);
		break;
	default:
		break;
	}
}

void WTSLogger::log_dyn_raw(const char* patttern, const char* catName, WTSLogLevel ll, const char* message)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	auto logger = getLogger(catName, patttern);

	if (logger)
	{
		switch (ll)
		{
		case LL_DEBUG:
			debug_imp(logger, message);
			break;
		case LL_INFO:
			info_imp(logger, message);
			break;
		case LL_WARN:
			warn_imp(logger, message);
			break;
		case LL_ERROR:
			error_imp(logger, message);
			break;
		case LL_FATAL:
			fatal_imp(logger, message);
			break;
		default:
			break;
		}
	}
}


SpdLoggerPtr WTSLogger::getLogger(const char* logger, const char* pattern /* = "" */)
{
	SpdLoggerPtr ret = spdlog::get(logger);
	if (ret == NULL && strlen(pattern) > 0)
	{
		//当成动态的日志来处理
		if (m_mapPatterns == NULL)
			return SpdLoggerPtr();

		WTSVariant* cfg = (WTSVariant*)m_mapPatterns->get(pattern);
		if (cfg == NULL)
			return SpdLoggerPtr();

		initLogger(logger, cfg);

		return spdlog::get(logger);
	}

	return ret;
}