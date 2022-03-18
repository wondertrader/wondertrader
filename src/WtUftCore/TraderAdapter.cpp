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
#include "ITrdNotifySink.h"

#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"

#include <atomic>

#include "../WTSTools/WTSLogger.h"
#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/StrUtil.hpp"

#include <exception>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace rj = rapidjson;
using namespace std;

inline uint32_t makeLocalOrderID()
{
	static std::atomic<uint32_t> _auto_order_id{ 0 };
	if (_auto_order_id == 0)
	{
		uint32_t curYear = TimeUtils::getCurDate() / 10000 * 10000 + 101;
		_auto_order_id = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(curYear, 0)) / 1000 * 50);
	}

	return _auto_order_id.fetch_add(1);
}

inline const char* formatAction(WTSDirectionType dType, WTSOffsetType oType)
{
	if(dType == WDT_LONG)
	{
		if (oType == WOT_OPEN)
			return "OL";
		else if (oType == WOT_CLOSE)
			return "CL";
		else
			return "CNL";
	}
	else
	{
		if (oType == WOT_OPEN)
			return "OS";
		else if (oType == WOT_CLOSE)
			return "CS";
		else
			return "CNS";
	}
}

TraderAdapter::TraderAdapter()
	: _id("")
	, _cfg(NULL)
	, _state(AS_NOTLOGIN)
	, _trader_api(NULL)
	, _orders(NULL)
	, _undone_qty(0)
{
}


TraderAdapter::~TraderAdapter()
{

}

bool TraderAdapter::initExt(const char* id, ITraderApi* api, IBaseDataMgr* bdMgr)
{
	_bd_mgr = bdMgr;
	_id = id;

	_order_pattern = StrUtil::printf("wtp.%s", id);

	api->init(NULL);
	_trader_api = api;
	return true;
}

bool TraderAdapter::init(const char* id, WTSVariant* params, IBaseDataMgr* bdMgr)
{
	if (params == NULL)
		return false;

	_bd_mgr = bdMgr;
	_id = id;

	_order_pattern = StrUtil::printf("wtp.%s", id);

	if (_cfg != NULL)
		return false;

	_cfg = params;
	_cfg->retain();

	if (params->getString("module").empty())
		return false;

	std::string module = DLLHelper::wrap_module(params->getCString("module"), "lib");;

	//先看工作目录下是否有交易模块
	std::string dllpath = WtHelper::getModulePath(module.c_str(), "traders", true);
	//如果没有,则再看模块目录,即dll同目录下
	if (!StdFile::exists(dllpath.c_str()))
		dllpath = WtHelper::getModulePath(module.c_str(), "traders", false);
	DllHandle hInst = DLLHelper::load_library(dllpath.c_str());
	if (hInst == NULL)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s] Loading trading module %s failed", _id.c_str(), dllpath.c_str());
		return false;
	}
	else
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s] Trader module %s loaded", _id.c_str(), dllpath.c_str());
	}

	FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)DLLHelper::get_symbol(hInst, "createTrader");
	if (NULL == pFunCreateTrader)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_FATAL, "[%s] Entrance function createTrader not found", _id.c_str());
		return false;
	}

	_trader_api = pFunCreateTrader();
	if (NULL == _trader_api)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_FATAL, "[%s] Creating trading api failed", _id.c_str());
		return false;
	}

	_remover = (FuncDeleteTrader)DLLHelper::get_symbol(hInst, "deleteTrader");

	if (!_trader_api->init(params))
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s] Entrance function deleteTrader not found", id);
		return false;
	}

	return true;
}

bool TraderAdapter::run()
{
	if (_trader_api == NULL)
		return false;

	_trader_api->registerSpi(this);

	_trader_api->connect();
	_state = AS_LOGINING;
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

double TraderAdapter::getPosition(const char* stdCode, bool bValidOnly, int32_t flag /* = 3 */)
{
	auto it = _positions.find(stdCode);
	if (it == _positions.end())
		return 0;

	double ret = 0;
	const PosItem& pItem = it->second;
	if(flag & 1)
	{
		if(bValidOnly)
			ret += (pItem.l_newavail + pItem.l_preavail);
		else
			ret += (pItem.l_newvol + pItem.l_prevol);
	}

	if (flag & 2)
	{
		if (bValidOnly)
			ret -= (pItem.s_newavail + pItem.s_preavail);
		else
			ret -= pItem.s_newvol + pItem.s_prevol;
	}
	return ret;
}

OrderMap* TraderAdapter::getOrders(const char* stdCode)
{
	if (_orders == NULL)
		return NULL;

	bool isAll = strlen(stdCode) == 0;

	StdUniqueLock lock(_mtx_orders);
	OrderMap* ret = OrderMap::create();
	for (auto it = _orders->begin(); it != _orders->end(); it++)
	{
		uint32_t localid = it->first;
		WTSOrderInfo* ordInfo = (WTSOrderInfo*)it->second;

		if (isAll || strcmp(ordInfo->getCode(), stdCode) == 0)
			ret->add(localid, ordInfo);
	}
	return ret;
}

void TraderAdapter::updateUndone(const char* stdCode, double qty, bool bOuput /* = false */)
{
	double& undone = _undone_qty[stdCode];
	double oldQty = undone;
	undone += qty;

	//if(bOuput)
	//	WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, "[{}] {} qty of undone order updated, {} -> {}", _id.c_str(), stdCode, oldQty, undone);
}

uint32_t TraderAdapter::doEntrust(WTSEntrust* entrust)
{
	_trader_api->makeEntrustID(entrust->getEntrustID(), 64);

	const char* stdCode = entrust->getCode();
	std::size_t pos = StrUtil::findFirst(entrust->getCode(), '.');
	entrust->setExchange(stdCode, pos);
	entrust->setCode(stdCode + pos + 1);
	if(entrust->getContractInfo() == NULL)
	{
		WTSContractInfo* cInfo = _bd_mgr->getContract(entrust->getCode(), entrust->getExchg());
		entrust->setContractInfo(cInfo);
	}

	uint32_t localid = makeLocalOrderID();
	char* usertag = entrust->getUserTag();
	memcpy(usertag, _order_pattern.c_str(), _order_pattern.size());
	usertag[_order_pattern.size()] = '.';
	char* tail = fmt::format_to(usertag + _order_pattern.size() + 1, "{}", localid);
	tail[0] = '\0';
	
	int32_t ret = _trader_api->orderInsert(entrust);
	if(ret < 0)
	{
		WTSLogger::log_dyn_f("trader", _id.c_str(), LL_ERROR, "[{}] Order placing failed: {}", _id, ret);
		return UINT_MAX;
	}
	//else
	//{
	//	_order_time_cache[entrust->getCode()].emplace_back(TimeUtils::getLocalTimeNow());
	//}
	return localid;
}

WTSContractInfo* TraderAdapter::getContract(const char* stdCode)
{
	StringVector ay = StrUtil::split(stdCode, ".");
	return _bd_mgr->getContract(ay[1].c_str(), ay[0].c_str());
}

WTSCommodityInfo* TraderAdapter::getCommodify(const char* stdCode)
{
	StringVector ay = StrUtil::split(stdCode, ".");
	WTSContractInfo* cInfo = _bd_mgr->getContract(ay[1].c_str(), ay[0].c_str());
	if (cInfo == NULL)
		return NULL;

	return cInfo->getCommInfo();
}

bool TraderAdapter::doCancel(WTSOrderInfo* ordInfo)
{
	if (ordInfo == NULL || !ordInfo->isAlive())
		return false;

	WTSContractInfo* cInfo = _bd_mgr->getContract(ordInfo->getCode(), ordInfo->getExchg());

	//撤单频率检查
	//if (!checkCancelLimits(stdCode.c_str()))
	//	return false;

	WTSEntrustAction* action = WTSEntrustAction::create(ordInfo->getCode(), cInfo->getExchg());
	action->setEntrustID(ordInfo->getEntrustID());
	action->setOrderID(ordInfo->getOrderID());
	int ret = _trader_api->orderAction(action);
	bool isSent = (ret >= 0);
	action->release();
	return isSent;
}

bool TraderAdapter::cancel(uint32_t localid)
{
	if (_orders == NULL || _orders->size() == 0)
		return false;

	WTSOrderInfo* ordInfo = NULL;
	{
		StdUniqueLock lock(_mtx_orders);
		ordInfo = (WTSOrderInfo*)_orders->grab(localid);
		if (ordInfo == NULL)
			return false;
	}
	
	bool bRet = doCancel(ordInfo);

	ordInfo->release();

	return bRet;
}

OrderIDs TraderAdapter::cancelAll(const char* stdCode)
{
	OrderIDs ret;

	double actQty = 0;
	bool isAll = strlen(stdCode) == 0;
	if (_orders != NULL && _orders->size() > 0)
	{
		for (auto it = _orders->begin(); it != _orders->end(); it++)
		{
			WTSOrderInfo* orderInfo = (WTSOrderInfo*)it->second;
			if(!orderInfo->isAlive())
				continue;

			WTSContractInfo* cInfo = orderInfo->getContractInfo();
			if (isAll || strcmp(stdCode, cInfo->getFullCode()) == 0)
			{
				if(doCancel(orderInfo))
				{
					ret.emplace_back(it->first);
					//_cancel_time_cache[orderInfo->getCode()].emplace_back(TimeUtils::getLocalTimeNow());
				}
			}
		}
	}

	return ret;
}

uint32_t TraderAdapter::openLong(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if(price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
	}
	entrust->setOrderFlag((WTSOrderFlag)(WOF_NOR + flag));

	entrust->setDirection(WDT_LONG);
	entrust->setOffsetType(WOT_OPEN);

	updateUndone(stdCode, qty);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
}

uint32_t TraderAdapter::openShort(const char* stdCode, double price, double qty, int flag/* = 0*/)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if (price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
	}
	entrust->setOrderFlag((WTSOrderFlag)(WOF_NOR + flag));

	entrust->setDirection(WDT_SHORT);
	entrust->setOffsetType(WOT_OPEN);

	updateUndone(stdCode, qty);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
}

uint32_t TraderAdapter::closeLong(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag/* = 0*/)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if (price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
	}
	entrust->setOrderFlag((WTSOrderFlag)(WOF_NOR + flag));

	entrust->setDirection(WDT_LONG);
	entrust->setOffsetType(isToday ? WOT_CLOSETODAY : WOT_CLOSE);

	updateUndone(stdCode, qty);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
}

uint32_t TraderAdapter::closeShort(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag/* = 0*/)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if (price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
	}
	entrust->setOrderFlag((WTSOrderFlag)(WOF_NOR + flag));

	entrust->setDirection(WDT_SHORT);
	entrust->setOffsetType(isToday ? WOT_CLOSETODAY : WOT_CLOSE);

	updateUndone(stdCode, qty);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
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
			WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s] Trading channel connecting failed: %d", _id.c_str(), ec);
		}
	}
	else if(e == WTE_Close)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s] Trading channel disconnected: %d", _id.c_str(), ec);
		for (auto sink : _sinks)
			sink->on_channel_lost();
	}
}

void TraderAdapter::onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
{
	if(!bSucc)
	{
		_state = AS_LOGINFAILED;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s] Trader login failed: %s", _id.c_str(), msg);
	}
	else
	{
		_state = AS_LOGINED;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s] Trader login succeed, trading date: %u", _id.c_str(), tradingdate);
		_trading_day = tradingdate;
		_trader_api->queryPositions();	//查持仓
	}
}

void TraderAdapter::onLogout()
{
	
}

void TraderAdapter::onRspEntrust(WTSEntrust* entrust, WTSError *err)
{
	if (err && err->getErrorCode() != WEC_NONE)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,err->getMessage());
		WTSContractInfo* cInfo = _bd_mgr->getContract(entrust->getCode(), entrust->getExchg());
		if (cInfo == NULL)
			return;

		std::string stdCode = cInfo->getFullCode();

		bool isLong = (entrust->getDirection() == WDT_LONG);
		bool isToday = (entrust->getOffsetType() == WOT_CLOSETODAY);
		bool isOpen = (entrust->getOffsetType() == WOT_OPEN);
		double qty = entrust->getVolume();

		std::string action;
		if (isOpen)
			action = "open ";
		else if (isToday)
			action = "closetoday ";
		else
			action = "close ";
		action += isLong ? "long" : "short";

		WTSLogger::log_dyn_f("trader", _id.c_str(), LL_ERROR, 
			"[{}] Order placing failed: {}, instrument: {}, action: {}, qty: {}", _id.c_str(), err->getMessage(), entrust->getCode(), action.c_str(), qty);

		//如果下单失败, 要更新未完成数量
		//实盘中发现错误单有时候会推送两次
		//所以这里加一个检查未完成单的逻辑
		//如果有错单，正常情况下未完成单一定不为0
		//如果未完成订单为0，则说明这一次是重复通知，则不再处理了
		double oldQty = _undone_qty[stdCode];
		if (decimal::eq(oldQty, 0))
			return;

		//bool isBuy = (isLong&&isOpen) || (!isLong && !isOpen);
		//double newQty = oldQty - qty*(isBuy ? 1 : -1);
		//_undone_qty[stdCode] = newQty;

		//WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, 
		//	"[{}] {} undone order updated, {} -> {}", _id.c_str(), stdCode.c_str(), oldQty, newQty);

		updateUndone(stdCode.c_str(), qty);


		if (strlen(entrust->getUserTag()) > 0)
		{
			char* userTag = (char*)entrust->getUserTag();
			userTag += _order_pattern.size() + 1;
			uint32_t localid = strtoul(userTag, NULL, 10);

			for(auto sink : _sinks)
				sink->on_entrust(localid, stdCode.c_str(), false, err->getMessage());
		}
		
	}
}

void TraderAdapter::onRspAccount(WTSArray* ayAccounts)
{
	if(_state == AS_TRADES_QRYED)
	{
		_state = AS_ALLREADY;

		WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, "[{}] Trading channel ready", _id.c_str());
		for (auto sink : _sinks)
			sink->on_channel_ready();
	}
}

void TraderAdapter::onRspPosition(const WTSArray* ayPositions)
{
	if (ayPositions && ayPositions->size() > 0)
	{
		for (auto it = ayPositions->begin(); it != ayPositions->end(); it++)
		{
			WTSPositionItem* pItem = (WTSPositionItem*)(*it);
			WTSContractInfo* cInfo = _bd_mgr->getContract(pItem->getCode(), pItem->getExchg());
			if (cInfo == NULL)
				continue;

			std::string stdCode = cInfo->getFullCode();

			PosItem& pos = _positions[stdCode];
			if (pItem->getDirection() == WDT_LONG)
			{
				pos.l_newavail = pItem->getAvailNewPos();
				pos.l_newvol = pItem->getNewPosition();
				pos.l_preavail = pItem->getAvailPrePos();
				pos.l_prevol = pItem->getPrePosition();
			}
			else
			{
				pos.s_newavail = pItem->getAvailNewPos();
				pos.s_newvol = pItem->getNewPosition();
				pos.s_preavail = pItem->getAvailPrePos();
				pos.s_prevol = pItem->getPrePosition();
			}
		}

		for (auto it = _positions.begin(); it != _positions.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const PosItem& pItem = it->second;
			printPosition(stdCode, pItem);
			for (auto sink : _sinks)
			{
				sink->on_position(stdCode, true, pItem.l_prevol, pItem.l_preavail, pItem.l_newvol, pItem.l_newavail, _trading_day);
				sink->on_position(stdCode, false, pItem.s_prevol, pItem.s_preavail, pItem.s_newvol, pItem.s_newavail, _trading_day);
			}
		}
	}

	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s] Position data updated", _id.c_str());

	if (_state == AS_LOGINED)
	{
		_state = AS_POSITION_QRYED;

		_trader_api->queryOrders();
	}
}

void TraderAdapter::onRspOrders(const WTSArray* ayOrders)
{
	if (ayOrders)
	{
		if (_orders == NULL)
			_orders = OrderMap::create();

		_undone_qty.clear();

		for (auto it = ayOrders->begin(); it != ayOrders->end(); it++)
		{
			WTSOrderInfo* orderInfo = (WTSOrderInfo*)(*it);
			if (orderInfo == NULL)
				continue;

			WTSContractInfo* cInfo = _bd_mgr->getContract(orderInfo->getCode(), orderInfo->getExchg());
			if (cInfo == NULL)
				continue;

			bool isBuy = (orderInfo->getDirection() == WDT_LONG && orderInfo->getOffsetType() == WOT_OPEN) || (orderInfo->getDirection() == WDT_SHORT && orderInfo->getOffsetType() != WOT_OPEN);

			std::string stdCode = cInfo->getFullCode();

			_orderids.insert(orderInfo->getOrderID());			

			if (!orderInfo->isAlive())
				continue;

			if (!StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern))
				continue;;

			char* userTag = (char*)orderInfo->getUserTag();
			userTag += _order_pattern.size() + 1;
			uint32_t localid = strtoul(userTag, NULL, 10);

			{
				StdUniqueLock lock(_mtx_orders);
				_orders->add(localid, orderInfo);
			}

			double& curQty = _undone_qty[stdCode];
			curQty += orderInfo->getVolLeft();
		}

		for (auto it = _undone_qty.begin(); it != _undone_qty.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const double& curQty = _undone_qty[stdCode];

			WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, 
				"[{}]{} undone quantity {}", _id.c_str(), stdCode, curQty);
		}
	}

	if (_state == AS_POSITION_QRYED)
	{
		_state = AS_ORDERS_QRYED;

		_trader_api->queryTrades();
	}
}

void TraderAdapter::printPosition(const char* code, const PosItem& pItem)
{
	WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, "[{}] {} position updated, long:{}[{}]|{}[{}], short:{}[{}]|{}[{}]",
		_id.c_str(), code, pItem.l_prevol, pItem.l_preavail, pItem.l_newvol, pItem.l_newavail, 
		pItem.s_prevol, pItem.s_preavail, pItem.s_newvol, pItem.s_newavail);
}

void TraderAdapter::onRspTrades(const WTSArray* ayTrades)
{
	if (ayTrades)
	{
		for (auto it = ayTrades->begin(); it != ayTrades->end(); it++)
		{
			WTSTradeInfo* tInfo = (WTSTradeInfo*)(*it);

			WTSContractInfo* cInfo = _bd_mgr->getContract(tInfo->getCode(), tInfo->getExchg());
			if (cInfo == NULL)
				continue;

			std::string stdCode = cInfo->getFullCode();
		}
	}

	if (_state == AS_ORDERS_QRYED)
	{
		_state = AS_TRADES_QRYED;

		_trader_api->queryAccount();
	}
}

inline const char* stateToName(WTSOrderState woState)
{
	if (woState == WOS_AllTraded)
		return "AllTrd";
	else if (woState == WOS_PartTraded_NotQueuing || woState == WOS_PartTraded_Queuing)
		return "PrtTrd";
	else if (woState == WOS_NotTraded_NotQueuing || woState == WOS_NotTraded_Queuing)
		return "UnTrd";
	else if (woState == WOS_Canceled)
		return "Cncld";
	else if (woState == WOS_Submitting)
		return "Smtting";
	else if (woState == WOS_Nottouched)
		return "UnSmt";
	else
		return "Error";
}

void TraderAdapter::onPushOrder(WTSOrderInfo* orderInfo)
{
	if (orderInfo == NULL)
		return;


	WTSContractInfo* cInfo = _bd_mgr->getContract(orderInfo->getCode(), orderInfo->getExchg());
	if (cInfo == NULL)
		return;

	std::string stdCode = cInfo->getFullCode();

	bool isBuy = (orderInfo->getDirection() == WDT_LONG && orderInfo->getOffsetType() == WOT_OPEN) || (orderInfo->getDirection() == WDT_SHORT && orderInfo->getOffsetType() != WOT_OPEN);


	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s] Order notified, instrument: %s, usertag: %s, state: %s", _id.c_str(), stdCode.c_str(), orderInfo->getUserTag(), stateToName(orderInfo->getOrderState()));

	//如果订单撤销, 并且是wt的订单, 则要先更新未完成数量
	if (orderInfo->getOrderState() == WOS_Canceled && StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern, false))
	{
		//撤单的时候, 要更新未完成
		bool isLong = (orderInfo->getDirection() == WDT_LONG);
		bool isOpen = (orderInfo->getOffsetType() == WOT_OPEN);
		bool isToday = (orderInfo->getOffsetType() == WOT_CLOSETODAY);
		double qty = orderInfo->getVolume() - orderInfo->getVolTraded();

		bool isBuy = (isLong&&isOpen) || (!isLong&&!isOpen);
		//double oldQty = _undone_qty[stdCode];
		//double newQty = oldQty - qty*(isBuy ? 1 : -1);
		//_undone_qty[stdCode] = newQty;
		//WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, "[{}] {} qty of undone order updated, {} -> {}", _id.c_str(), stdCode.c_str(), oldQty, newQty);
		updateUndone(stdCode.c_str(), -qty);

		WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, "[{}] Order {} of {} canceled:{}, action: {}, leftqty: {}",
			_id.c_str(), orderInfo->getUserTag(), stdCode.c_str(), orderInfo->getStateMsg(),
			formatAction(orderInfo->getDirection(), orderInfo->getOffsetType()), qty);
	}

	//先检查该订单是不是第一次推送过来
	//如果是第一次推送过来, 则要根据开平更新可平
	if (strlen(orderInfo->getOrderID()) > 0)
	{
		auto it = _orderids.find(orderInfo->getOrderID());
		if (it == _orderids.end())
		{
			//先把订单号缓存起来, 防止重复处理
			_orderids.insert(orderInfo->getOrderID());

			//只有平仓需要更新可平
			if (orderInfo->getOffsetType() != WOT_OPEN)
			{
				//const char* code = stdCode.c_str();
				bool isLong = (orderInfo->getDirection() == WDT_LONG);
				bool isToday = (orderInfo->getOffsetType() == WOT_CLOSETODAY);
				double qty = orderInfo->getVolume();

				PosItem& pItem = _positions[stdCode];
				if (isLong)	//平多
				{
					if (isToday)
					{
						pItem.l_newavail -= min(pItem.l_newavail, qty);	//如果是平今, 则只需要更新可平今仓
					}
					else
					{
						double left = qty;

						//如果是平仓, 则先更新可平昨仓, 还有剩余, 再更新可平今仓
						//如果品种区分平昨平今, 也按照这个流程, 因为平昨的总数量不可能超出昨仓
						double maxQty = min(pItem.l_preavail, qty);
						pItem.l_preavail -= maxQty;
						left -= maxQty;

						if (left > 0)
							pItem.l_newavail -= min(pItem.l_newavail, left);
					}
				}
				else //平空
				{
					if (isToday)
					{
						pItem.s_newavail -= min(pItem.s_newavail, qty);
					}
					else
					{
						double left = qty;

						double maxQty = min(pItem.s_preavail, qty);
						pItem.s_preavail -= maxQty;
						left -= maxQty;

						if (left > 0)
							pItem.s_newavail -= min(pItem.s_newavail, left);
					}
				}
				printPosition(stdCode.c_str(), pItem);
			}
		}
		else if (orderInfo->getOrderState() == WOS_Canceled && orderInfo->getOffsetType() != WOT_OPEN)
		{
			//如果订单不是第一次推送, 且撤销了, 则要更新可平量
			//const char* code = orderInfo->getCode();
			bool isLong = (orderInfo->getDirection() == WDT_LONG);
			bool isToday = (orderInfo->getOffsetType() == WOT_CLOSETODAY);
			double qty = orderInfo->getVolume() - orderInfo->getVolTraded();

			PosItem& pItem = _positions[stdCode];
			if (isLong)	//平多
			{
				if (isToday)
				{
					pItem.l_newavail += qty;	//如果是平今, 则只需要更新可平今仓
				}
				else
				{
					pItem.l_preavail += qty;
					if (pItem.l_preavail > pItem.l_prevol)
					{
						pItem.l_newavail += (pItem.l_preavail - pItem.l_prevol);
						pItem.l_preavail = pItem.l_prevol;
					}
				}
			}
			else //平空
			{
				if (isToday)
				{
					pItem.s_newavail += qty;
				}
				else
				{
					pItem.s_preavail += qty;
					if (pItem.s_preavail > pItem.s_prevol)
					{
						pItem.s_newavail += (pItem.s_preavail - pItem.s_prevol);
						pItem.s_preavail = pItem.s_prevol;
					}
				}
			}
			printPosition(stdCode.c_str(), pItem);
		}
	}

	uint32_t localid = 0;

	//先看看是不是wt发出去的单子
	if (StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern))
	{
		char* userTag = (char*)orderInfo->getUserTag();
		userTag += _order_pattern.size() + 1;
		localid = strtoul(userTag, NULL, 10);
	}

	//如果是wt发出去的单子则需要更新内部数据
	if(localid != 0)
	{
		StdUniqueLock lock(_mtx_orders);
		if (!orderInfo->isAlive() && _orders)
		{
			_orders->remove(localid);
		}
		else
		{
			if (_orders == NULL)
				_orders = OrderMap::create();

			_orders->add(localid, orderInfo);
		}

		uint32_t offset;
		if (orderInfo->getOffsetType() == WOT_OPEN)
			offset = 0;
		else if (orderInfo->getOffsetType() == WOT_CLOSE)
			offset = 1;
		else
			offset = 2;

		//通知所有监听接口
		for (auto sink : _sinks)
			sink->on_order(localid, stdCode.c_str(), orderInfo->getDirection()==WDT_LONG, offset, 
				orderInfo->getVolume(), orderInfo->getVolLeft(), orderInfo->getPrice(), orderInfo->getOrderState() == WOS_Canceled);
	}
}

void TraderAdapter::onPushTrade(WTSTradeInfo* tradeRecord)
{
	WTSContractInfo* cInfo = tradeRecord->getContractInfo();
	cInfo = _bd_mgr->getContract(tradeRecord->getCode(), tradeRecord->getExchg());
	if (cInfo == NULL)
		return;

	WTSCommodityInfo* commInfo = cInfo->getCommInfo();

	bool isLong = (tradeRecord->getDirection() == WDT_LONG);
	bool isOpen = (tradeRecord->getOffsetType() == WOT_OPEN);
	bool isBuy = (tradeRecord->getDirection() == WDT_LONG && tradeRecord->getOffsetType() == WOT_OPEN) || (tradeRecord->getDirection() == WDT_SHORT && tradeRecord->getOffsetType() != WOT_OPEN);

	std::string stdCode = cInfo->getFullCode();

	WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, 
		"[{}] Trade notified, instrument: {}, usertag: {}, trdqty: {}, trdprice: {}", 
			_id.c_str(), stdCode.c_str(), tradeRecord->getUserTag(), tradeRecord->getVolume(), tradeRecord->getPrice());

	//如果是自己的订单，则更新未完成单
	uint32_t localid = 0;
	if (StrUtil::startsWith(tradeRecord->getUserTag(), _order_pattern, false))
	{
		char* userTag = (char*)tradeRecord->getUserTag();
		userTag += _order_pattern.size() + 1;
		localid = strtoul(userTag, NULL, 10);

		//double oldQty = _undone_qty[stdCode];
		//double newQty = oldQty - tradeRecord->getVolume()*(isBuy ? 1 : -1);
		//_undone_qty[stdCode] = newQty;
		//WTSLogger::log_dyn_f("trader", _id.c_str(), LL_INFO, 
		//	"[{}] {} qty of undone orders updated, {} -> {}", _id.c_str(), stdCode.c_str(), oldQty, newQty);
		updateUndone(stdCode.c_str(), -tradeRecord->getVolume());
	}

	PosItem& pItem = _positions[stdCode];
	double vol = tradeRecord->getVolume();
	if(isLong)
	{
		if (isOpen)
		{
			pItem.l_newvol += vol;

			if(!commInfo->isT1())	//如果不是T1，则更新可用持仓
				pItem.l_newavail += vol;
		}
		else if (tradeRecord->getOffsetType() == WOT_CLOSETODAY)
		{
			pItem.l_newvol -= vol;
		}
		else
		{
			double left = vol;
			double maxVol = min(left, pItem.l_prevol);
			pItem.l_prevol -= maxVol;
			left -= maxVol;
			pItem.l_newvol -= left;
		}
	}
	else
	{
		if (isOpen)
		{
			pItem.s_newvol += vol;
			if (!commInfo->isT1())	//如果不是T1，则更新可用持仓
				pItem.s_newavail += vol;
		}
		else if (tradeRecord->getOffsetType() == WOT_CLOSETODAY)
		{
			pItem.s_newvol -= vol;
		}
		else
		{
			double left = vol;
			double maxVol = min(left, pItem.s_prevol);
			pItem.s_prevol -= maxVol;
			left -= maxVol;
			pItem.s_newvol -= left;
		}
	}

	printPosition(stdCode.c_str(), pItem);


	uint32_t offset;
	if (tradeRecord->getOffsetType() == WOT_OPEN)
		offset = 0;
	else if (tradeRecord->getOffsetType() == WOT_CLOSE)
		offset = 1;
	else
		offset = 2;
	for (auto sink : _sinks)
		sink->on_trade(localid, stdCode.c_str(), isLong, offset, vol, tradeRecord->getPrice());

	_trader_api->queryAccount();
}

void TraderAdapter::onTraderError(WTSError* err)
{
	if(err)
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s] Error of trading channel occured: %s", _id.c_str(), err->getMessage());
}

IBaseDataMgr* TraderAdapter::getBaseDataMgr()
{
	return _bd_mgr;
}

void TraderAdapter::handleTraderLog(WTSLogLevel ll, const char* message)
{
	WTSLogger::log_dyn_raw("trader", _id.c_str(), ll, message);
}

#pragma endregion "ITraderSpi接口"


//////////////////////////////////////////////////////////////////////////
//CTPWrapperMgr
bool TraderAdapterMgr::addAdapter(const char* tname, TraderAdapterPtr& adapter)
{
	if (adapter == NULL || strlen(tname) == 0)
		return false;

	auto it = _adapters.find(tname);
	if(it != _adapters.end())
	{
		WTSLogger::error("Same name of trading channels: %s", tname);
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
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->run();
	}

	WTSLogger::info_f("{} trading channels started", _adapters.size());
}

void TraderAdapterMgr::release()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->release();
	}

	_adapters.clear();
}