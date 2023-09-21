/*!
 * \file TraderAdapter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "TraderAdapter.h"
#include "WtHelper.h"
#include "Dumper.h"

#include <atomic>

#include "../WTSTools/WTSLogger.h"

#include "../Share/CodeHelper.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSRiskDef.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../Includes/WTSSessionInfo.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/StdUtils.hpp"


extern Dumper& getDumper();
extern std::string getBaseFolder();


TraderAdapter::TraderAdapter(TraderAdapterMgr* mgr)
	: _id("")
	, _cfg(NULL)
	, _trader_api(NULL)
	, _mgr(mgr)
	, _done(false)
{
}


TraderAdapter::~TraderAdapter()
{
}

bool TraderAdapter::init(const char* id, WTSVariant* params, IBaseDataMgr* bdMgr)
{
	if (params == NULL)
		return false;

	_bd_mgr = bdMgr;
	_id = id;

	if (_cfg != NULL)
		return false;

	_cfg = params;
	_cfg->retain();


	if (params->getString("module").empty())
		return false;


	//先看工作目录下是否有交易模块
	std::string module = DLLHelper::wrap_module(params->getCString("module"), "lib");;

	if (!StdFile::exists(module.c_str()))
	{
		module = WtHelper::get_module_dir();
		module += "traders/";
		module += DLLHelper::wrap_module(params->getCString("module"), "lib");
	}

	DllHandle hInst = DLLHelper::load_library(module.c_str());
	if (hInst == NULL)
	{
		WTSLogger::error("[{}]交易模块{}加载失败", _id.c_str(), module.c_str());
		return false;
	}

	FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)DLLHelper::get_symbol(hInst, "createTrader");
	if (NULL == pFunCreateTrader)
	{
		WTSLogger::error("[{}]交易接口创建函数读取失败", _id.c_str());
		return false;
	}

	_trader_api = pFunCreateTrader();
	if (NULL == _trader_api)
	{
		WTSLogger::error("[{}]交易接口创建失败", _id.c_str());
		return false;
	}

	_remover = (FuncDeleteTrader)DLLHelper::get_symbol(hInst, "deleteTrader");
	
	//这里要强制把quick改成true，不查全部成交和订单
	params->append("quick", true);
	if (!_trader_api->init(params))
	{
		WTSLogger::error("[{}]交易接口启动失败: 交易接口初始化失败", id);
		return false;
	}

	WTSLogger::info("[{}]交易接口初始化成功", id);
	return true;
}

bool TraderAdapter::run()
{
	if (_trader_api == NULL)
		return false;

	_trader_api->registerSpi(this);

	_trader_api->connect();
	return true;
}

void TraderAdapter::release()
{
	if (_trader_api)
	{
		_trader_api->registerSpi(NULL);
		_trader_api->release();
	}
}

#pragma region "ITraderSpi接口"
void TraderAdapter::handleEvent(WTSTraderEvent e, int32_t ec)
{
	if(e == WTE_Connect)
	{
		if(ec == 0)
		{
			_trader_api->login(_cfg->getCString("user"), _cfg->getCString("pass"), _cfg->getCString("product"));
		}
		else
		{
			WTSLogger::error("[{}]交易账号连接失败: {}", _id.c_str(), ec);
			_mgr->decAlive();
			_done = true;
		}
	}
	else if(e == WTE_Close)
	{
		WTSLogger::error("[{}]交易账号连接已断开: {}", _id.c_str(), ec);
	}
}

void TraderAdapter::onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
{
	if(!bSucc)
	{
		WTSLogger::error("[{}]交易账号登录失败: {}", _id.c_str(), msg);
		_mgr->decAlive();
		_done = true;
	}
	else
	{
		_date = tradingdate;
		WTSLogger::info("[{}]交易账号登录成功, 当前交易日:{}", _id.c_str(), tradingdate);

		_trader_api->queryPositions();	//查持仓
	}
}

void TraderAdapter::onLogout()
{
	
}

void TraderAdapter::queryFund()
{
	if (_date == 0)
		return;

	_trader_api->queryAccount();
}

void TraderAdapter::queryPosition()
{
	if (_date == 0)
		return;

	_trader_api->queryPositions();
}

void TraderAdapter::onRspAccount(WTSArray* ayAccounts)
{
	if(ayAccounts && ayAccounts->size() > 0)
	{
		for (std::size_t idx = 0; idx < ayAccounts->size(); idx++)
		{
			WTSAccountInfo* accInfo = (WTSAccountInfo*)ayAccounts->at(idx);

			getDumper().on_account(_id.c_str(), _date, accInfo->getCurrency(), accInfo->getPreBalance(), accInfo->getBalance(), accInfo->getBalance() + accInfo->getDynProfit(),
				accInfo->getCloseProfit(), accInfo->getDynProfit(), accInfo->getCommission(), accInfo->getMargin(), accInfo->getDeposit(), accInfo->getWithdraw(), idx == ayAccounts->size()-1);
		}
	}

	WTSLogger::info("[{}]资金数据已更新", _id.c_str());

	if(!_done)
		_trader_api->queryTrades();
}

void TraderAdapter::onPushTrade(WTSTradeInfo* tInfo)
{
	WTSContractInfo* cInfo = _bd_mgr->getContract(tInfo->getCode(), tInfo->getExchg());
	if (cInfo == NULL)
		return;

	getDumper().on_trade(_id.c_str(), cInfo->getExchg(), cInfo->getCode(), _date, tInfo->getTradeID(), tInfo->getRefOrder(),
		(uint32_t)tInfo->getDirection(), (uint32_t)tInfo->getOffsetType(), tInfo->getVolume(), tInfo->getPrice(), tInfo->getAmount(),
		(uint32_t)tInfo->getOrderType(), (uint32_t)tInfo->getTradeType(), tInfo->getTradeTime(), true);
}

void TraderAdapter::onRspTrades(const WTSArray* ayTrades)
{
	if (ayTrades && ayTrades->size() > 0)
	{
		for (std::size_t idx = 0; idx < ayTrades->size(); idx++)
		{
			WTSTradeInfo* pItem = (WTSTradeInfo*)((WTSArray*)ayTrades)->at(idx);
			WTSContractInfo* cInfo = _bd_mgr->getContract(pItem->getCode(), pItem->getExchg());
			if (cInfo == NULL)
				continue;

			getDumper().on_trade(_id.c_str(), cInfo->getExchg(), cInfo->getCode(), _date, pItem->getTradeID(), pItem->getRefOrder(),
				(uint32_t)pItem->getDirection(), (uint32_t)pItem->getOffsetType(), pItem->getVolume(), pItem->getPrice(), pItem->getAmount(),
				(uint32_t)pItem->getOrderType(), (uint32_t)pItem->getTradeType(), pItem->getTradeTime(), idx == ayTrades->size()-1);
		}
	}

	WTSLogger::info("[{}]成交明细已更新", _id.c_str());

	_trader_api->queryOrders();
}

void TraderAdapter::onRspOrders(const WTSArray* ayOrders)
{
	if (ayOrders && ayOrders->size() > 0)
	{
		for (std::size_t idx = 0; idx < ayOrders->size(); idx++)
		{
			WTSOrderInfo* pItem = (WTSOrderInfo*)((WTSArray*)ayOrders)->at(idx);
			WTSContractInfo* cInfo = _bd_mgr->getContract(pItem->getCode(), pItem->getExchg());
			if (cInfo == NULL)
				continue;

			getDumper().on_order(_id.c_str(), cInfo->getExchg(), cInfo->getCode(), _date, pItem->getOrderID(), pItem->getDirection(),
				pItem->getOffsetType(), pItem->getVolume(), pItem->getVolLeft(), pItem->getVolTraded(), pItem->getPrice(),
				pItem->getOrderType(), pItem->getPriceType(), pItem->getOrderTime(), pItem->getOrderState(), pItem->getStateMsg(), idx == ayOrders->size()-1);
		}
	}

	WTSLogger::info("[{}]订单明细已更新", _id.c_str());
	_mgr->decAlive();
	_done = true;
}

void TraderAdapter::onPushOrder(WTSOrderInfo* oInfo)
{
	WTSContractInfo* cInfo = _bd_mgr->getContract(oInfo->getCode(), oInfo->getExchg());
	if (cInfo == NULL)
		return;

	//如果订单回报中，订单状态是已结束，则刷新资金和持仓
	if (!oInfo->isAlive())
	{
		_trader_api->queryAccount();
		_trader_api->queryPositions();
	}

	getDumper().on_order(_id.c_str(), cInfo->getExchg(), cInfo->getCode(), _date, oInfo->getOrderID(), oInfo->getDirection(),
		oInfo->getOffsetType(), oInfo->getVolume(), oInfo->getVolLeft(), oInfo->getVolTraded(), oInfo->getPrice(),
		oInfo->getOrderType(), oInfo->getPriceType(), oInfo->getOrderTime(), oInfo->getOrderState(), oInfo->getStateMsg(), true);
}

void TraderAdapter::onRspPosition(const WTSArray* ayPositions)
{
	if (ayPositions && ayPositions->size() > 0)
	{
		for (std::size_t idx = 0; idx < ((WTSArray*)ayPositions)->size(); idx++)
		{
			WTSPositionItem* pItem = (WTSPositionItem*)(((WTSArray*)ayPositions)->at(idx));
			WTSContractInfo* cInfo = _bd_mgr->getContract(pItem->getCode());
			if (cInfo == NULL)
				continue;
			WTSCommodityInfo* commInfo = cInfo->getCommInfo();

			getDumper().on_position(_id.c_str(), cInfo->getExchg(), cInfo->getCode(), _date, (pItem->getDirection() == WDT_LONG ? 0 : 1),
				pItem->getTotalPosition(), pItem->getPositionCost(), pItem->getMargin(), pItem->getAvgPrice(),
				pItem->getDynProfit(), commInfo->getVolScale(), idx == ayPositions->size() - 1);
		}
	}

	WTSLogger::info("[{}]持仓数据已更新", _id.c_str());

	if (!_done)
		_trader_api->queryAccount();
}

void TraderAdapter::onTraderError(WTSError* err, void* pData /* = NULL */)
{
	if(err)
		WTSLogger::error("[{}]交易通道出现错误: {}", _id.c_str(), err->getMessage());
}

IBaseDataMgr* TraderAdapter::getBaseDataMgr()
{
	return _bd_mgr;
}

void TraderAdapter::handleTraderLog(WTSLogLevel ll, const char* message)
{
	WTSLogger::log_raw(ll, message);
}

#pragma endregion "ITraderSpi接口"


//////////////////////////////////////////////////////////////////////////
//TraderAdapterMgr
bool TraderAdapterMgr::addAdapter(const char* tname, TraderAdapterPtr& adapter)
{
	if (adapter == NULL || strlen(tname) == 0)
		return false;

	auto it = _adapters.find(tname);
	if(it != _adapters.end())
	{
		WTSLogger::error("交易通道名称相同: {}", tname);
		return false;
	}

	_adapters[tname] = adapter;

	return true;
}

TraderAdapterPtr TraderAdapterMgr::getAdapter(const char* tname)
{
	auto it = _adapters.find(tname);
	if (it != _adapters.end())
	{
		return it->second;
	}

	return TraderAdapterPtr();
}

void TraderAdapterMgr::run()
{
	_live_cnt = _adapters.size();
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->run();
	}

	WTSLogger::info("{}个交易通道已启动", _adapters.size());
}

void TraderAdapterMgr::release()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->release();
	}

	_adapters.clear();
}

void TraderAdapterMgr::decAlive()
{
	_mutex.lock();
	auto left = _live_cnt.fetch_sub(1);
	_mutex.unlock();
	if (left > 0)
		left--;
	if(left <= 2 && left > 0)
	{
		for (auto it = _adapters.begin(); it != _adapters.end(); it++)
		{
			TraderAdapterPtr trader = it->second;
			if (!trader->isDone())
				WTSLogger::info("{} is still undone", trader->id());
		}
	}

	WTSLogger::info("{}/{}", left, _adapters.size());
}

void TraderAdapterMgr::refresh()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		TraderAdapterPtr trader = it->second;
		if (!trader->isDone())
			continue;
		trader->queryPosition();
		trader->queryFund();
	}
}