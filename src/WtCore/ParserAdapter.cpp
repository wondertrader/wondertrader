/*!
 * \file ParserAdapter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserAdapter.h"
#include "WtEngine.h"
#include "WtCtaTicker.h"
#include "WtHelper.h"

#include "../Share/DLLHelper.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/WTSContractInfo.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/IBaseDataMgr.h"
#include "../Share/IHotMgr.h"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSLogger.h"

USING_NS_OTP;

//////////////////////////////////////////////////////////////////////////
//ParserAdapter
ParserAdapter::ParserAdapter()
	: _parser_api(NULL)
	, _remover(NULL)
	, _stopped(false)
	, _bd_mgr(NULL)
	, _stub(NULL)
	, _cfg(NULL)
{
}


ParserAdapter::~ParserAdapter()
{
}

bool ParserAdapter::init(const char* id, WTSVariant* cfg, IParserStub* stub, IBaseDataMgr* bgMgr, IHotMgr* hotMgr/* = NULL*/)
{
	if (cfg == NULL)
		return false;

	_stub = stub;
	_bd_mgr = bgMgr;
	_hot_mgr = hotMgr;
	_id = id;

	if (_cfg != NULL)
		return false;

	_cfg = cfg;
	_cfg->retain();

	{
		//加载模块
		if (cfg->getString("module").empty())
			return false;

		const char* module = cfg->getCString("module");

		//先看工作目录下是否有行情模块
		std::string dllpath = WtHelper::getModulePath(module, "parsers", true);
		//如果没有，则再看模块目录，即dll同目录下
		if(!StdFile::exists(dllpath.c_str()))
			dllpath = WtHelper::getModulePath(module, "parsers", false);

		DllHandle hInst = DLLHelper::load_library(dllpath.c_str());
		if (hInst == NULL)
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_ERROR, "[%s]行情模块%s加载失败", _id.c_str(), dllpath.c_str());
			return false;
		}

		FuncCreateParser pFuncCreateParser = (FuncCreateParser)DLLHelper::get_symbol(hInst, "createParser");
		if (NULL == pFuncCreateParser)
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_FATAL, "[%s]交易接口创建函数读取失败", _id.c_str());
			return false;
		}

		_parser_api = pFuncCreateParser();
		if (NULL == _parser_api)
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_FATAL, "[%s]行情接口创建失败", _id.c_str());
			return false;
		}

		_remover = (FuncDeleteParser)DLLHelper::get_symbol(hInst, "deleteParser");
	}
	

	const std::string& strFilter = cfg->getString("filter");
	if (!strFilter.empty())
	{
		const StringVector &ayFilter = StrUtil::split(strFilter, ",");
		auto it = ayFilter.begin();
		for (; it != ayFilter.end(); it++)
		{
			_exchg_filter.insert(*it);
		}
	}

	if (_parser_api)
	{
		_parser_api->registerListener(this);

		WTSParams* params = cfg->toParams();
		if (_parser_api->init(params))
		{
			ContractSet contractSet;
			if (!_exchg_filter.empty())
			{
				ExchgFilter::iterator it = _exchg_filter.begin();
				for (; it != _exchg_filter.end(); it++)
				{
					WTSArray* ayContract =_bd_mgr->getContracts((*it).c_str());
					WTSArray::Iterator it = ayContract->begin();
					for (; it != ayContract->end(); it++)
					{
						WTSContractInfo* contract = STATIC_CONVERT(*it, WTSContractInfo*);
						WTSCommodityInfo* pCommInfo = _bd_mgr->getCommodity(contract);
						if (pCommInfo->getCategoty() == CC_Future || pCommInfo->getCategoty() == CC_Option || pCommInfo->getCategoty() == CC_Stock)
							contractSet.insert(contract->getFullCode());
					}

					ayContract->release();
				}
			}
			else
			{
				WTSArray* ayContract =_bd_mgr->getContracts();
				WTSArray::Iterator it = ayContract->begin();
				for (; it != ayContract->end(); it++)
				{
					WTSContractInfo* contract = STATIC_CONVERT(*it, WTSContractInfo*);
					WTSCommodityInfo* pCommInfo =_bd_mgr->getCommodity(contract);
					if (pCommInfo->getCategoty() == CC_Future || pCommInfo->getCategoty() == CC_Option || pCommInfo->getCategoty() == CC_Stock)
						contractSet.insert(contract->getFullCode());
				}

				ayContract->release();
			}

			_parser_api->subscribe(contractSet);
			contractSet.clear();
		}
		else
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_ERROR, "[%s]行情模块初始化失败,模块接口初始化失败...", _id.c_str());
		}

		params->release();
	}
	else
	{
		WTSLogger::log_dyn("parser", _id.c_str(), LL_ERROR, "[%s]行情模块初始化失败,获取模块接口失败...", _id.c_str());
	}

	return true;
}

void ParserAdapter::release()
{
	_stopped = true;
	if (_parser_api)
	{
		_parser_api->release();
	}

	_remover(_parser_api);
}

bool ParserAdapter::run()
{
	if (_parser_api == NULL)
		return false;

	_parser_api->connect();
	return true;
}

void ParserAdapter::handleQuote(WTSTickData *quote, bool bNeedSlice)
{
	if (_stopped)
		return;

	if (!_exchg_filter.empty() && (_exchg_filter.find(quote->exchg()) == _exchg_filter.end()))
		return;

	if (quote->actiondate() == 0 || quote->tradingdate() == 0)
		return;

	bool isHot = false;

	WTSContractInfo* cInfo = _bd_mgr->getContract(quote->code(), quote->exchg());
	WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
	std::string stdCode;
	if (commInfo->getCategoty() == CC_Future)
	{
		stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
		std::string hotCode = _hot_mgr->getHotCode(quote->exchg(), quote->code(), quote->tradingdate());
		isHot = !hotCode.empty();
	}
	else if(commInfo->getCategoty() == CC_Stock)
	{
		stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	}
	else if (commInfo->getCategoty() == CC_Option)
	{
		stdCode = CodeHelper::bscFutOptCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	}
	quote->setCode(stdCode.c_str());

	_stub->handle_push_quote(quote, isHot);

}

void ParserAdapter::handleParserLog(WTSLogLevel ll, const char* format, ...)
{
	if (_stopped)
		return;

	va_list args;
	va_start(args, format);
	WTSLogger::log_dyn_direct("parser", _id.c_str(), ll, format, args);
	va_end(args);
}


//////////////////////////////////////////////////////////////////////////
//ParserAdapterMgr
void ParserAdapterMgr::release()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->release();
	}

	_adapters.clear();
}

bool ParserAdapterMgr::addAdapter(const char* id, ParserAdapterPtr& adapter)
{
	if (adapter == NULL || strlen(id) == 0)
		return false;

	auto it = _adapters.find(id);
	if (it != _adapters.end())
	{
		WTSLogger::error("行情通道名称相同: %s", id);
		return false;
	}

	_adapters[id] = adapter;

	return true;
}


ParserAdapterPtr ParserAdapterMgr::getAdapter(const char* tname)
{
	auto it = _adapters.find(tname);
	if (it != _adapters.end())
	{
		return it->second;
	}

	return ParserAdapterPtr();
}

void ParserAdapterMgr::run()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->run();
	}

	WTSLogger::info("%u个行情通道已启动", _adapters.size());
}