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

#include "../Share/CodeHelper.hpp"
#include "../Share/TimeUtils.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"

#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;

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

bool ParserAdapter::initExt(const char* id, IParserApi* api, IParserStub* stub, IBaseDataMgr* bgMgr, IHotMgr* hotMgr/* = NULL*/)
{
	if (api == NULL)
		return false;

	_parser_api = api;
	_stub = stub;
	_bd_mgr = bgMgr;
	_hot_mgr = hotMgr;
	_id = id;

	if (_parser_api)
	{
		_parser_api->registerSpi(this);

		if (_parser_api->init(NULL))
		{
			ContractSet contractSet;
			WTSArray* ayContract = _bd_mgr->getContracts();
			WTSArray::Iterator it = ayContract->begin();
			for (; it != ayContract->end(); it++)
			{
				WTSContractInfo* contract = STATIC_CONVERT(*it, WTSContractInfo*);
				contractSet.insert(contract->getFullCode());
			}

			ayContract->release();

			_parser_api->subscribe(contractSet);
			contractSet.clear();
		}
		else
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_ERROR, "[{}] Parser initializing failed: api initializing failed...", _id.c_str());
		}
	}

	return true;
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

	_check_time = cfg->getBoolean("check_time");

	{
		//����ģ��
		if (cfg->getString("module").empty())
			return false;

		std::string module = DLLHelper::wrap_module(cfg->getCString("module"), "lib");;

		//�ȿ�����Ŀ¼���Ƿ��н���ģ��
		std::string dllpath = WtHelper::getModulePath(module.c_str(), "parsers", true);
		//���û��,���ٿ�ģ��Ŀ¼,��dllͬĿ¼��
		if (!StdFile::exists(dllpath.c_str()))
			dllpath = WtHelper::getModulePath(module.c_str(), "parsers", false);

		DllHandle hInst = DLLHelper::load_library(dllpath.c_str());
		if (hInst == NULL)
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_ERROR, "[{}] Parser module {} loading failed", _id.c_str(), dllpath.c_str());
			return false;
		}
		else
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_INFO, "[{}] Parser module {} loaded", _id.c_str(), dllpath.c_str());
		}

		FuncCreateParser pFuncCreateParser = (FuncCreateParser)DLLHelper::get_symbol(hInst, "createParser");
		if (NULL == pFuncCreateParser)
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_FATAL, "[{}] Entrance function createParser not found", _id.c_str());
			return false;
		}

		_parser_api = pFuncCreateParser();
		if (NULL == _parser_api)
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_FATAL, "[{}] Creating parser api failed", _id.c_str());
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

	std::string strCodes = cfg->getString("code");
	if (!strCodes.empty())
	{
		const StringVector &ayCodes = StrUtil::split(strCodes, ",");
		auto it = ayCodes.begin();
		for (; it != ayCodes.end(); it++)
		{
			_code_filter.insert(*it);
		}
	}

	if (_parser_api)
	{
		_parser_api->registerSpi(this);

		if (_parser_api->init(cfg))
		{
			ContractSet contractSet;
			if (!_code_filter.empty())//�����жϺ�Լ������
			{
				ExchgFilter::iterator it = _code_filter.begin();
				for (; it != _code_filter.end(); it++)
				{
					//ȫ����,��ʽ��SSE.600000,�ڻ�����ΪCFFEX.IF2005
					std::string code, exchg;
					auto ay = StrUtil::split((*it).c_str(), ".");
					if (ay.size() == 1)
						code = ay[0];
					else if (ay.size() == 2)
					{
						exchg = ay[0];
						code = ay[1];
					}
					else if (ay.size() == 3)
					{
						exchg = ay[0];
						code = ay[2];
					}
					WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
					if(contract)
						contractSet.insert(contract->getFullCode());
					else
					{
						//�����Ʒ��ID���򽫸�Ʒ����ȫ����Լ���ӵ������б�
						WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(exchg.c_str(), code.c_str());
						if(commInfo)
						{
							const auto& codes = commInfo->getCodes();
							for(const auto& c : codes)
							{
								contractSet.insert(fmt::format("{}.{}", exchg, c.c_str()));
							}							
						}
					}
				}
			}
			else if (!_exchg_filter.empty())
			{
				ExchgFilter::iterator it = _exchg_filter.begin();
				for (; it != _exchg_filter.end(); it++)
				{
					WTSArray* ayContract =_bd_mgr->getContracts((*it).c_str());
					WTSArray::Iterator it = ayContract->begin();
					for (; it != ayContract->end(); it++)
					{
						WTSContractInfo* contract = STATIC_CONVERT(*it, WTSContractInfo*);
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
					contractSet.insert(contract->getFullCode());
				}

				ayContract->release();
			}

			_parser_api->subscribe(contractSet);
			contractSet.clear();
		}
		else
		{
			WTSLogger::log_dyn("parser", _id.c_str(), LL_ERROR, "[{}] Parser initializing failed: api initializing failed...", _id.c_str());
		}
	}
	else
	{
		WTSLogger::log_dyn("parser", _id.c_str(), LL_ERROR, "[{}] Parser initializing failed: creating api failed...", _id.c_str());
	}

	WTSLogger::info("parser", _id.c_str(), LL_INFO, "[{}] Parser initialzied, check_time: {}", _id.c_str(), _check_time);

	return true;
}

void ParserAdapter::release()
{
	_stopped = true;
	if (_parser_api)
	{
		_parser_api->release();
	}

	if (_remover)
		_remover(_parser_api);
	else
		delete _parser_api;
}

bool ParserAdapter::run()
{
	if (_parser_api == NULL)
		return false;

	_parser_api->connect();
	return true;
}

//���������ʱ���
const int RESONABLE_MILLISECS = 60 * 60 * 1000;
void ParserAdapter::handleQuote(WTSTickData *quote, uint32_t procFlag)
{
	if (quote == NULL || _stopped || quote->actiondate() == 0 || quote->tradingdate() == 0)
		return;

	if (!_exchg_filter.empty() && (_exchg_filter.find(quote->exchg()) == _exchg_filter.end()))
		return;

	WTSContractInfo* cInfo = quote->getContractInfo();
	if (cInfo == NULL)
	{
		cInfo = _bd_mgr->getContract(quote->code(), quote->exchg());
		quote->setContractInfo(cInfo);
	}

	if (cInfo == NULL)
		return;

	WTSCommodityInfo* commInfo = cInfo->getCommInfo();
	WTSSessionInfo* sInfo = commInfo->getSessionInfo();

	if (_check_time)
	{
		int64_t tick_time = TimeUtils::makeTime(quote->actiondate(), quote->actiontime());
		int64_t local_time = TimeUtils::getLocalTimeNow();

		/*
		 *	By Wesley @ 2022.04.20
		 *	������µ�tickʱ�䣬�ͱ���ʱ�����̫��
		 *	����Ϊtick��ʱ����Ǵ����
		 *	����Ҫ�󱾵�ʱ����Ҫʱ������У׼��
		 */
		if (tick_time - local_time > RESONABLE_MILLISECS)
		{
			WTSLogger::warn("Tick of {} with wrong timestamp {}.{} received, skipped", cInfo->getFullCode(), quote->actiondate(), quote->actiontime());
			return;
		}
	}

	uint32_t hotflag = 0;

	std::string stdCode;
	if (commInfo->getCategoty() == CC_FutOption)
	{
		stdCode = CodeHelper::rawFutOptCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	}
	else if(CodeHelper::isMonthlyCode(quote->code()))
	{
		//����Ƿ��º�Լ������������ʹ��������ж�
		stdCode = CodeHelper::rawMonthCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
		bool bHot = _hot_mgr->isHot(quote->exchg(), quote->code(), 0);
		bool b2nd = _hot_mgr->isSecond(quote->exchg(), quote->code(), 0);
		hotflag = bHot ? 1 : (b2nd ? 2 : 0);
	}
	else
	{
		stdCode = CodeHelper::rawFlatCodeToStdCode(cInfo->getCode(), cInfo->getExchg(), cInfo->getProduct());
	}
	quote->setCode(stdCode.c_str());

	_stub->handle_push_quote(quote, hotflag);
}

void ParserAdapter::handleOrderQueue(WTSOrdQueData* ordQueData)
{
	if (_stopped)
		return;

	if (!_exchg_filter.empty() && (_exchg_filter.find(ordQueData->exchg()) == _exchg_filter.end()))
		return;

	if (ordQueData->actiondate() == 0 || ordQueData->tradingdate() == 0)
		return;

	WTSContractInfo* cInfo = _bd_mgr->getContract(ordQueData->code(), ordQueData->exchg());
	if (cInfo == NULL)
		return;

	WTSCommodityInfo* commInfo = cInfo->getCommInfo();
	std::string stdCode = CodeHelper::rawFlatCodeToStdCode(cInfo->getCode(), cInfo->getExchg(), commInfo->getProduct());
	ordQueData->setCode(stdCode.c_str());

	if (_stub)
		_stub->handle_push_order_queue(ordQueData);
}

void ParserAdapter::handleOrderDetail(WTSOrdDtlData* ordDtlData)
{
	if (_stopped)
		return;

	if (!_exchg_filter.empty() && (_exchg_filter.find(ordDtlData->exchg()) == _exchg_filter.end()))
		return;

	if (ordDtlData->actiondate() == 0 || ordDtlData->tradingdate() == 0)
		return;

	WTSContractInfo* cInfo = _bd_mgr->getContract(ordDtlData->code(), ordDtlData->exchg());
	if (cInfo == NULL)
		return;

	WTSCommodityInfo* commInfo = cInfo->getCommInfo();
	std::string stdCode = CodeHelper::rawFlatCodeToStdCode(cInfo->getCode(), cInfo->getExchg(), commInfo->getProduct());
	ordDtlData->setCode(stdCode.c_str());

	if (_stub)
		_stub->handle_push_order_detail(ordDtlData);
}

void ParserAdapter::handleTransaction(WTSTransData* transData)
{
	if (_stopped)
		return;

	if (!_exchg_filter.empty() && (_exchg_filter.find(transData->exchg()) == _exchg_filter.end()))
		return;

	if (transData->actiondate() == 0 || transData->tradingdate() == 0)
		return;

	WTSContractInfo* cInfo = _bd_mgr->getContract(transData->code(), transData->exchg());
	if (cInfo == NULL)
		return;

	WTSCommodityInfo* commInfo = cInfo->getCommInfo();
	std::string stdCode = CodeHelper::rawFlatCodeToStdCode(cInfo->getCode(), cInfo->getExchg(), commInfo->getProduct());
	transData->setCode(stdCode.c_str());

	if (_stub)
		_stub->handle_push_transaction(transData);
}


void ParserAdapter::handleParserLog(WTSLogLevel ll, const char* message)
{
	if (_stopped)
		return;

	WTSLogger::log_dyn_raw("parser", _id.c_str(), ll, message);
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
		WTSLogger::error(" Same name of parsers: {}", id);
		return false;
	}

	_adapters[id] = adapter;

	return true;
}


ParserAdapterPtr ParserAdapterMgr::getAdapter(const char* id)
{
	auto it = _adapters.find(id);
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

	WTSLogger::info("{} parsers started", _adapters.size());
}