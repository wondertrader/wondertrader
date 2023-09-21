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
#include "ActionPolicyMgr.h"

#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSRiskDef.hpp"

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
	, _risk_mon_enabled(false)
	, _stat_map(NULL)
{
}


TraderAdapter::~TraderAdapter()
{
	if (_stat_map)
		_stat_map->release();
}

bool TraderAdapter::initExt(const char* id, ITraderApi* api, IBaseDataMgr* bdMgr, ActionPolicyMgr* policyMgr)
{
	_policy_mgr = policyMgr;
	_bd_mgr = bdMgr;
	_id = id;

	_order_pattern = StrUtil::printf("wtp.%s", id);

	api->init(NULL);
	_trader_api = api;
	return true;
}

bool TraderAdapter::init(const char* id, WTSVariant* params, IBaseDataMgr* bdMgr, ActionPolicyMgr* policyMgr)
{
	if (params == NULL)
		return false;

	_policy_mgr = policyMgr;
	_bd_mgr = bdMgr;
	_id = id;

	_order_pattern = StrUtil::printf("wtp.%s", id);

	if (_cfg != NULL)
		return false;

	_cfg = params;
	_cfg->retain();

	//这里解析流量风控参数
	WTSVariant* cfgRisk = params->get("riskmon");
	if (cfgRisk)
	{
		if (cfgRisk->getBoolean("active"))
		{
			_risk_mon_enabled = true;

			WTSVariant* cfgPolicy = cfgRisk->get("policy");
			auto keys = cfgPolicy->memberNames();
			for (auto it = keys.begin(); it != keys.end(); it++)
			{
				const char* product = (*it).c_str();
				WTSVariant*	vProdItem = cfgPolicy->get(product);
				RiskParams& rParam = _risk_params_map[product];
				rParam._cancel_total_limits = vProdItem->getUInt32("cancel_total_limits");
				rParam._cancel_times_boundary = vProdItem->getUInt32("cancel_times_boundary");
				rParam._cancel_stat_timespan = vProdItem->getUInt32("cancel_stat_timespan");

				rParam._order_total_limits = vProdItem->getUInt32("order_total_limits");
				rParam._order_times_boundary = vProdItem->getUInt32("order_times_boundary");
				rParam._order_stat_timespan = vProdItem->getUInt32("order_stat_timespan");

				WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[{}] Risk control rule {} of trading channel loaded", _id.c_str(), product);
			}

			auto it = _risk_params_map.find("default");
			if (it == _risk_params_map.end())
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[{}] Some instruments may not be monitored due to no default risk control rule of trading channel", _id.c_str());
			}
		}
		else
		{
			WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[{}] Risk control rule of trading channel not activated", _id.c_str());
		}
	}
	else
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[{}] No risk control rule setup of trading channel", _id.c_str());
	}

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
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[{}] Loading trading module %s failed", _id.c_str(), dllpath.c_str());
		return false;
	}
	else
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[{}] Trader module {} loaded", _id.c_str(), dllpath.c_str());
	}

	FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)DLLHelper::get_symbol(hInst, "createTrader");
	if (NULL == pFunCreateTrader)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_FATAL, "[{}] Entrance function createTrader not found", _id.c_str());
		return false;
	}

	_trader_api = pFunCreateTrader();
	if (NULL == _trader_api)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_FATAL, "[{}] Creating trading api failed", _id.c_str());
		return false;
	}

	_remover = (FuncDeleteTrader)DLLHelper::get_symbol(hInst, "deleteTrader");

	if (!_trader_api->init(params))
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[{}] Entrance function deleteTrader not found", id);
		return false;
	}

	return true;
}

bool TraderAdapter::run()
{
	if (_trader_api == NULL)
		return false;

	if (_stat_map == NULL)
		_stat_map = TradeStatMap::create();

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

double TraderAdapter::enumPosition(const char* stdCode /* = "" */)
{
	/*
	 *	By Wesley @ 2022.03.19
	 *	这里改成回调的方式
	 *	不然接口会设计得很复杂
	 */
	double ret = 0;
	bool bAll = (strlen(stdCode) == 0);
	for (auto it = _positions.begin(); it != _positions.end(); it++)
	{
		if (!bAll && strcmp(it->first.c_str(), stdCode) != 0)
			continue;

		const PosItem& pItem = it->second;
		for (auto sink : _sinks)
		{
			sink->on_position(stdCode, true, pItem.l_prevol, pItem.l_preavail, pItem.l_newvol, pItem.l_newavail, _trading_day);
			sink->on_position(stdCode, false, pItem.s_prevol, pItem.s_preavail, pItem.s_newvol, pItem.s_newavail, _trading_day);
		}
		ret += pItem.total_pos(true) + pItem.total_pos(false);
	}

	return ret;
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

	SpinLock lock(_mtx_orders);
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

void TraderAdapter::updateUndone(const char* stdCode, double qty)
{
	double& undone = _undone_qty[stdCode];
	double oldQty = undone;
	undone += qty;
}

uint32_t TraderAdapter::doEntrust(WTSEntrust* entrust)
{
	_trader_api->makeEntrustID(entrust->getEntrustID(), 64);

	const char* stdCode = entrust->getCode();
	std::size_t pos = StrUtil::findFirst(entrust->getCode(), '.');
	entrust->setExchange(stdCode, pos);
	entrust->setCode(stdCode + pos + 1);
	//if(entrust->getContractInfo() == NULL)
	//{
	//	WTSContractInfo* cInfo = _bd_mgr->getContract(entrust->getCode(), entrust->getExchg());
	//	entrust->setContractInfo(cInfo);
	//}

	uint32_t localid = makeLocalOrderID();
	char* usertag = entrust->getUserTag();
	wt_strcpy(usertag, _order_pattern.c_str(), _order_pattern.size());
	usertag[_order_pattern.size()] = '.';
	fmtutil::format_to(usertag + _order_pattern.size() + 1, "{}", localid);
	
	int32_t ret = _trader_api->orderInsert(entrust);
	if(ret < 0)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[{}] Order placing failed: {}", _id, ret);
		return UINT_MAX;
	}
	//else if(_risk_mon_enabled)
	//{
	//	int64_t now = TimeUtils::getLocalTimeNow();
	//	_order_time_cache[entrust->getCode()].emplace_back(now);
	//}
	return localid;
}

WTSContractInfo* TraderAdapter::getContract(const char* stdCode)
{
	char buf[64] = { 0 };
	wt_strcpy(buf, stdCode);
	auto idx = StrUtil::findFirst(buf, '.');
	buf[idx] = '\0';
	return _bd_mgr->getContract(buf + idx + 1, buf);
}

bool TraderAdapter::doCancel(WTSOrderInfo* ordInfo)
{
	if (ordInfo == NULL || !ordInfo->isAlive())
		return false;

	WTSContractInfo* cInfo = ordInfo->getContractInfo();
	if(cInfo == NULL)
		cInfo = _bd_mgr->getContract(ordInfo->getCode(), ordInfo->getExchg());

	//撤单频率检查
	//if (_risk_mon_enabled && !checkCancelLimits(ordInfo->getCode()))
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
		SpinLock lock(_mtx_orders);
		ordInfo = (WTSOrderInfo*)_orders->grab(localid);
		if (ordInfo == NULL)
			return false;
	}
	
	bool bRet = doCancel(ordInfo);

	//if(_risk_mon_enabled)
	//	_cancel_time_cache[ordInfo->getCode()].emplace_back(TimeUtils::getLocalTimeNow());

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
					//if (_risk_mon_enabled)
					//	_cancel_time_cache[cInfo->getCode()].emplace_back(TimeUtils::getLocalTimeNow());
				}
			}
		}
	}

	return ret;
}

uint32_t TraderAdapter::getInfos(const char* stdCode)
{
	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo == NULL)
		return 0;

	return statInfo->infos();
}

OrderIDs TraderAdapter::buy(const char* stdCode, double price, double qty, int flag, bool bForceClose, WTSContractInfo* cInfo /* = NULL */)
{
	OrderIDs ret;
	if (qty == 0)
		return ret;

	//if(_risk_mon_enabled && !checkOrderLimits(stdCode))
	//{
	//	WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "{} is forbidden to trade", stdCode);
	//	return ret;
	//}

	if (cInfo == NULL) cInfo = getContract(stdCode);
	WTSCommodityInfo* commInfo = cInfo->getCommInfo();

	WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG, "[{}] Buying {} of quantity {}", _id.c_str(), stdCode, qty);

	const PosItem& pItem = _positions[stdCode];
	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo == NULL)
	{
		statInfo = WTSTradeStateInfo::create(stdCode);
		_stat_map->add(stdCode, statInfo, false);
	}
	TradeStatInfo& statItem = statInfo->statInfo();
	const ActionRuleGroup& ruleGP = _policy_mgr->getActionRules(commInfo->getFullPid());

	double left = qty;

	double unitQty = (price == 0.0) ? cInfo->getMaxMktVol() : cInfo->getMaxLmtVol();
	if (decimal::eq(unitQty, 0))
		unitQty = DBL_MAX;

	for (auto it = ruleGP.begin(); it != ruleGP.end(); it++)
	{
		const ActionRule& curRule = (*it);
		if (curRule._atype == AT_Open && !bForceClose)
		{
			//先检查是否已经到了限额
			//买入开仓, 即开多仓
			double maxQty = left;

			if (curRule._limit_l != 0)
			{
				if (statItem.l_openvol >= curRule._limit_l)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[{}] {} long position opened today is up to limit {}", _id.c_str(), stdCode, curRule._limit_l);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit_l - statItem.l_openvol);
				}
			}

			if (curRule._limit != 0)
			{
				if (statItem.l_openvol + statItem.s_openvol >= curRule._limit)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[{}] {} position opened today is up to limit {}", _id.c_str(), stdCode, curRule._limit);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit - statItem.l_openvol - statItem.s_openvol);
				}
			}

			//这里还要考虑单笔最大委托数量
			double leftQty = maxQty;
			for (;;)
			{
				double curQty = min(leftQty, unitQty);
				uint32_t localid = openLong(stdCode, price, curQty, flag);
				ret.emplace_back(localid);

				leftQty -= curQty;

				if (decimal::eq(leftQty, 0))
					break;
			}

			left -= maxQty;

			WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
				"[{}] Signal of buying {} of quantity {} triggered: Opening long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
		}
		else if (curRule._atype == AT_CloseToday)
		{
			double maxQty = 0;
			//如果要区分平昨平今的品种, 则只读取可平今仓即可
			//如果不区分平昨平今的品种, 则读取全部可平, 因为读取可拼今仓也没意义
			if (commInfo->getCoverMode() == CM_CoverToday)
				maxQty = min(left, pItem.s_newavail);	//先看看可平今仓
			else
				maxQty = min(left, pItem.avail_pos(false));


			//如果要检查净今仓，但是昨仓不为0，则跳过该条规则
			if (!bForceClose && curRule._pure && !decimal::eq(pItem.s_prevol, 0.0))
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN,
					"[{}] Closing new short position of {} skipped because of non-zero pre short position", _id.c_str(), stdCode);
				continue;
			}

			//这里还要考虑单笔最大委托数量
			//if (maxQty > 0)
			if (decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeShort(stdCode, price, curQty, (commInfo->getCoverMode() == CM_CoverToday), flag);//如果不支持平今, 则直接下平仓标记即可
					ret.emplace_back(localid);

					leftQty -= curQty;

					//if (leftQty == 0)
					if (decimal::eq(leftQty, 0))
						break;
				}
				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of buying {} of quantity {} triggered: Closing new short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
				else
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of buying {} of quantity {} triggered: Closing short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
			}

		}
		else if (curRule._atype == AT_CloseYestoday)
		{
			//平昨比较简单, 因为不需要区分标记
			double maxQty = min(left, pItem.s_preavail);

			//如果要检查净昨仓，但是今仓不为0，则跳过该条规则
			if (!bForceClose && curRule._pure && !decimal::eq(pItem.s_newvol, 0.0))
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN,
					"[{}] Closing pre short position of {} skipped because of non-zero new short position", _id.c_str(), stdCode);
				continue;
			}

			//这里还要考虑单笔最大委托数量
			//if (maxQty > 0)
			if (decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeShort(stdCode, price, curQty, false, flag);//如果不支持平今, 则直接下平仓标记即可
					ret.emplace_back(localid);

					leftQty -= curQty;

					//if (leftQty == 0)
					if (decimal::eq(leftQty, 0))
						break;
				}

				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of buying {} of quantity {} triggered: Closing old short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
				else
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of buying {} of quantity {} triggered: Closing short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
			}
		}
		else if (curRule._atype == AT_Close)
		{
			//如果只是平仓, 则分情况处理
			//如果区分平昨平今, 则要先平昨再平今
			//如果不区分平昨平今, 则统一平仓
			if (commInfo->getCoverMode() != CM_CoverToday)
			{
				double maxQty = min(pItem.avail_pos(false), left);

				//if (maxQty > 0)
				if (decimal::gt(maxQty, 0))
				{
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeShort(stdCode, price, curQty, false, flag);
						ret.emplace_back(localid);

						leftQty -= curQty;

						//if (leftQty == 0)
						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of buying {} of quantity {} triggered: Closing short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
			}
			else
			{
				//if (pItem.s_preavail > 0)
				if (decimal::gt(pItem.s_preavail, 0))
				{
					//先将可平昨仓平仓
					double maxQty = min(pItem.s_preavail, qty);
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeShort(stdCode, price, curQty, false, flag);
						ret.emplace_back(localid);

						leftQty -= curQty;

						//if (leftQty == 0)
						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of buying {} of quantity {} triggered: Closing old short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}

				//if (left > 0 && pItem.s_newavail > 0)
				if (decimal::gt(left, 0) && decimal::gt(pItem.s_newavail, 0))
				{
					//再将可平今仓平仓
					//TODO: 这里还有一个控制, 就是强制锁今仓的话, 这段逻辑就跳过去了
					double maxQty = min(pItem.s_newavail, left);
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeShort(stdCode, price, curQty, true, flag);
						ret.emplace_back(localid);

						leftQty -= curQty;

						//if (leftQty == 0)
						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of buying {} of quantity {} triggered: Closing new short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
			}
		}

		if (left == 0)
			break;
	}

	if (left > 0)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,
			"[{}] Signal of buying {} of quantity {} left quantity of {} not triggered", _id.c_str(), stdCode, qty, left);
	}

	return ret;
}

OrderIDs TraderAdapter::sell(const char* stdCode, double price, double qty, int flag, bool bForceClose, WTSContractInfo* cInfo /* = NULL */)
{
	OrderIDs ret;
	if (qty == 0)
		return ret;

	//if (_risk_mon_enabled && !checkOrderLimits(stdCode))
	//{
	//	WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "{} is forbidden to trade", stdCode);
	//	return ret;
	//}

	if (cInfo == NULL) cInfo = getContract(stdCode);
	WTSCommodityInfo* commInfo = cInfo->getCommInfo();

	const PosItem& pItem = _positions[stdCode];
	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo == NULL)
	{
		statInfo = WTSTradeStateInfo::create(stdCode);
		_stat_map->add(stdCode, statInfo, false);
	}
	TradeStatInfo& statItem = statInfo->statInfo();

	const ActionRuleGroup& ruleGP = _policy_mgr->getActionRules(commInfo->getFullPid());

	double left = qty;

	double unitQty = (price == 0.0) ? cInfo->getMaxMktVol() : cInfo->getMaxLmtVol();
	if (decimal::eq(unitQty, 0))
		unitQty = DBL_MAX;

	for (auto it = ruleGP.begin(); it != ruleGP.end(); it++)
	{
		const ActionRule& curRule = (*it);
		if (curRule._atype == AT_Open && !bForceClose)
		{
			//先检查是否已经到了限额
			//买入开仓, 即开多仓
			double maxQty = left;

			if (curRule._limit_s != 0)
			{
				if (statItem.s_openvol >= curRule._limit_s)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[{}] {} short position opened today is up to limit {}", _id.c_str(), stdCode, curRule._limit_l);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit_s - statItem.s_openvol);
				}
			}

			if (curRule._limit != 0)
			{
				if (statItem.l_openvol + statItem.s_openvol >= curRule._limit)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[{}] {} position opened today is up to limit {}", _id.c_str(), stdCode, curRule._limit);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit - statItem.l_openvol - statItem.s_openvol);
				}
			}

			//这里还要考虑单笔最大委托数量
			double leftQty = maxQty;
			for (;;)
			{
				double curQty = min(leftQty, unitQty);
				uint32_t localid = openShort(stdCode, price, curQty, flag);
				ret.emplace_back(localid);

				leftQty -= curQty;

				if (decimal::eq(leftQty, 0))
					break;
			}

			left -= maxQty;

			WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
				"[{}] Signal of selling {} of quantity {} triggered: Opening short of quantity {}", _id.c_str(), stdCode, qty, maxQty);
		}
		else if (curRule._atype == AT_CloseToday)
		{
			double maxQty = 0;
			//如果要区分平昨平今的品种, 则只读取可平今仓即可
			//如果不区分平昨平今的品种, 则读取全部可平, 因为读取可平今仓也没意义
			if (commInfo->getCoverMode() == CM_CoverToday)
				maxQty = min(left, pItem.l_newavail);	//先看看可平今仓
			else
				maxQty = min(left, pItem.avail_pos(true));

			//如果要检查净今仓，但是昨仓不为0，则跳过该条规则
			if (!bForceClose && curRule._pure && !decimal::eq(pItem.l_prevol, 0.0))
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN,
					"[{}] Closing new long position of {} skipped because of non-zero pre long position", _id.c_str(), stdCode);
				continue;
			}

			//这里还要考虑单笔最大委托数量
			if (decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeLong(stdCode, price, curQty, (commInfo->getCoverMode() == CM_CoverToday), flag);//如果不支持平今, 则直接下平仓标记即可
					ret.emplace_back(localid);

					leftQty -= curQty;

					if (decimal::eq(leftQty, 0))
						break;
				}
				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of selling {} of quantity {} triggered: Closing new long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
				else
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of selling {} of quantity {} triggered: Closing long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
			}

		}
		else if (curRule._atype == AT_CloseYestoday)
		{
			//平昨比较简单, 因为不需要区分标记
			double maxQty = min(left, pItem.l_preavail);

			//如果要检查净昨仓，但是今仓不为0，则跳过该条规则
			if (!bForceClose && curRule._pure && !decimal::eq(pItem.l_newvol, 0.0))
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN,
					"[{}] Closing pre long position of {} skipped because of non-zero new long position", _id.c_str(), stdCode);
				continue;
			}

			//这里还要考虑单笔最大委托数量
			if (decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeLong(stdCode, price, curQty, false, flag);//如果不支持平今, 则直接下平仓标记即可
					ret.emplace_back(localid);

					leftQty -= curQty;

					if (decimal::eq(leftQty, 0))
						break;
				}
				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of selling {} of quantity {} triggered: Closing old long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
				else
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of selling {} of quantity {} triggered: Closing long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}
			}
		}
		else if (curRule._atype == AT_Close)
		{
			//如果只是平仓, 则分情况处理
			//如果区分平昨平今, 则要先平昨再平今
			//如果不区分平昨平今, 则统一平仓
			if (commInfo->getCoverMode() != CM_CoverToday)
			{
				double maxQty = min(pItem.avail_pos(true), left);	//不区分平昨平今, 则读取全部可平量
				if (decimal::gt(maxQty, 0))
				{
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeLong(stdCode, price, curQty, false, flag);
						ret.emplace_back(localid);

						leftQty -= curQty;

						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
						"[{}] Signal of selling {} of quantity {} triggered: Closing long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
				}

			}
			else
			{
				if (decimal::gt(left, 0) && decimal::gt(pItem.l_preavail, 0))
				{
					//先将可平昨仓平仓
					double maxQty = min(pItem.l_preavail, qty);
					if (decimal::gt(maxQty, 0))
					{
						double leftQty = maxQty;
						for (;;)
						{
							double curQty = min(leftQty, unitQty);
							uint32_t localid = closeLong(stdCode, price, curQty, false, flag);
							ret.emplace_back(localid);

							leftQty -= curQty;

							if (decimal::eq(leftQty, 0))
								break;
						}
						left -= maxQty;

						WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
							"[{}] Signal of selling {} of quantity {} triggered: Closing old long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
					}

				}

				if (decimal::gt(left, 0) && decimal::gt(pItem.l_newavail, 0))
				{
					//再将可平今仓平仓
					//TODO: 这里还有一个控制, 就是强制锁今仓的话, 这段逻辑就跳过去了
					double maxQty = min(pItem.l_newavail, left);
					if (decimal::gt(maxQty, 0))
					{
						double leftQty = maxQty;
						for (;;)
						{
							double curQty = min(leftQty, unitQty);
							uint32_t localid = closeLong(stdCode, price, curQty, true, flag);
							ret.emplace_back(localid);

							leftQty -= curQty;

							if (decimal::eq(leftQty, 0))
								break;
						}
						left -= maxQty;

						WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
							"[{}] Signal of selling {} of quantity {} triggered: Closing new long of quantity {}", _id.c_str(), stdCode, qty, maxQty);
					}

				}
			}
		}

		if (left == 0)
			break;
	}

	if (left > 0)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,
			"[{}] Signal of buying {} of quantity {} left quantity of {} not triggered", _id.c_str(), stdCode, qty, left);
	}

	return ret;
}


uint32_t TraderAdapter::openLong(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	//if (_risk_mon_enabled && !checkOrderLimits(stdCode))
	//{
	//	WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "{} is forbidden to trade", stdCode);
	//	return 0;
	//}

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
	//if (_risk_mon_enabled && !checkOrderLimits(stdCode))
	//{
	//	WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "{} is forbidden to trade", stdCode);
	//	return 0;
	//}

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
	//if (_risk_mon_enabled && !checkOrderLimits(stdCode))
	//{
	//	WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "{} is forbidden to trade", stdCode);
	//	return 0;
	//}

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
	//if (_risk_mon_enabled && !checkOrderLimits(stdCode))
	//{
	//	WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "{} is forbidden to trade", stdCode);
	//	return 0;
	//}

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
			WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[{}] Trading channel connecting failed: {}", _id.c_str(), ec);
		}
	}
	else if(e == WTE_Close)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[{}] Trading channel disconnected: {}", _id.c_str(), ec);
		for (auto sink : _sinks)
			sink->on_channel_lost();
	}
}

void TraderAdapter::onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
{
	if(!bSucc)
	{
		_state = AS_LOGINFAILED;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[{}] Trader login failed: {}", _id.c_str(), msg);
	}
	else
	{
		_state = AS_LOGINED;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[{}] Trader login succeed, trading date: {}", _id.c_str(), tradingdate);
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

		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, 
			"[{}] Order placing failed: {}, instrument: {}, action: {}, qty: {}", _id.c_str(), err->getMessage(), entrust->getCode(), action.c_str(), qty);

		//如果下单失败, 要更新未完成数量
		//实盘中发现错误单有时候会推送两次
		//所以这里加一个检查未完成单的逻辑
		//如果有错单，正常情况下未完成单一定不为0
		//如果未完成订单为0，则说明这一次是重复通知，则不再处理了
		double oldQty = _undone_qty[stdCode];
		if (decimal::eq(oldQty, 0))
			return;

		updateUndone(stdCode.c_str(), -qty);


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

		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[{}] Trading channel ready", _id.c_str());
		for (auto sink : _sinks)
			sink->on_channel_ready(_trading_day);
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

	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[{}] Position data updated", _id.c_str());

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

			//更新统计信息
			WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
			if (statInfo == NULL)
			{
				statInfo = WTSTradeStateInfo::create(stdCode.c_str());
				_stat_map->add(stdCode, statInfo, false);
			}
			TradeStatInfo& statItem = statInfo->statInfo();
			statItem._infos++;	//无论什么状态，挂单信息量+1
			if (isBuy)
			{
				statItem.b_orders++;
				statItem.b_ordqty += orderInfo->getVolume();

				if (orderInfo->isError())
				{
					statItem.b_wrongs++;
					statItem.b_wrongqty += orderInfo->getVolume() - orderInfo->getVolTraded();
				}
				else if (orderInfo->getOrderState() == WOS_Canceled)
				{
					if (orderInfo->getOrderFlag() == WOF_NOR)
					{
						statItem.b_cancels++;
						statItem.b_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
					}
					else
					{
						statItem.b_auto_cancels++;
						statItem.b_auto_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
					}

					//撤单信息量+1
					statItem._infos++;
				}

			}
			else
			{
				statItem.s_orders++;
				statItem.s_ordqty += orderInfo->getVolume();

				if (orderInfo->isError())
				{
					statItem.s_wrongs++;
					statItem.s_wrongqty += orderInfo->getVolume() - orderInfo->getVolTraded();
				}
				else if (orderInfo->getOrderState() == WOS_Canceled)
				{
					if (orderInfo->getOrderFlag() == WOF_NOR)
					{
						statItem.s_cancels++;
						statItem.s_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
					}
					else
					{
						statItem.s_auto_cancels++;
						statItem.s_auto_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
					}

					//撤单信息量+1
					statItem._infos++;
				}
			}

			if (!orderInfo->isAlive())
				continue;

			if (!StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern.c_str(), true))
				continue;;

			char* userTag = (char*)orderInfo->getUserTag();
			userTag += _order_pattern.size() + 1;
			uint32_t localid = strtoul(userTag, NULL, 10);

			{
				SpinLock lock(_mtx_orders);
				_orders->add(localid, orderInfo);
			}

			double& curQty = _undone_qty[stdCode];
			curQty += orderInfo->getVolLeft();
		}

		for (auto it = _undone_qty.begin(); it != _undone_qty.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const double& curQty = _undone_qty[stdCode];

			WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, 
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
	WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG, "[{}] {} position updated, long:{}[{}]|{}[{}], short:{}[{}]|{}[{}]",
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

			WTSContractInfo* cInfo = tInfo->getContractInfo();
			if (cInfo == NULL)
				continue;

			const char* stdCode = cInfo->getFullCode();

			WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
			if (statInfo == NULL)
			{
				statInfo = WTSTradeStateInfo::create(stdCode);
				_stat_map->add(stdCode, statInfo, false);
			}
			TradeStatInfo& statItem = statInfo->statInfo();

			bool isLong = (tInfo->getDirection() == WDT_LONG);
			bool isOpen = (tInfo->getOffsetType() == WOT_OPEN);
			bool isCloseT = (tInfo->getOffsetType() == WOT_CLOSETODAY);
			double qty = tInfo->getVolume();

			if (isLong)
			{
				if (isOpen)
					statItem.l_openvol += qty;
				else if (isCloseT)
					statItem.l_closetvol += qty;
				else
					statItem.l_closevol += qty;
			}
			else
			{
				if (isOpen)
					statItem.s_openvol += qty;
				else if (isCloseT)
					statItem.s_closetvol += qty;
				else
					statItem.s_closevol += qty;
			}
		}

		for (auto it = _stat_map->begin(); it != _stat_map->end(); it++)
		{
			const char* stdCode = it->first.c_str();
			WTSTradeStateInfo* pItem = (WTSTradeStateInfo*)it->second;
			WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,
				"[{}] {} action stats updated, long opened: {}, long closed: {}, new long closed: {}, short opened: {}, short closed: {}, new short closed: {}",
				_id.c_str(), stdCode, pItem->open_volume_long(), pItem->close_volume_long(), pItem->closet_volume_long(),
				pItem->open_volume_short(), pItem->close_volume_short(), pItem->closet_volume_short());
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

	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
	if (statInfo == NULL)
	{
		statInfo = WTSTradeStateInfo::create(stdCode.c_str());
		_stat_map->add(stdCode, statInfo, false);
	}
	TradeStatInfo& statItem = statInfo->statInfo();

	//撤销的话, 要更新统计数据
	if (orderInfo->getOrderState() == WOS_Canceled)
	{
		statItem._infos++;	//撤单成功信息量+1
		if (isBuy)
		{
			if (orderInfo->isError())//错单要和撤单区分开
			{
				statItem.b_wrongs++;
				statItem.b_wrongqty += orderInfo->getVolume() - orderInfo->getVolTraded();
			}
			else
			{
				//只有普通订单的撤单才计入统计
				if(orderInfo->getOrderFlag() == WOF_NOR)
				{
					statItem.b_cancels++;
					statItem.b_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
				}
				else
				{
					statItem.b_auto_cancels++;
					statItem.b_auto_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
				}
			}
		}
		else
		{
			if (orderInfo->isError())//错单要和撤单区分开
			{
				statItem.s_wrongs++;
				statItem.s_wrongqty += orderInfo->getVolume() - orderInfo->getVolTraded();
			}
			else
			{
				if (orderInfo->getOrderFlag() == WOF_NOR)
				{
					statItem.s_cancels++;
					statItem.s_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
				}
				else
				{
					statItem.s_auto_cancels++;
					statItem.s_auto_canclqty += orderInfo->getVolume() - orderInfo->getVolTraded();
				}
			}
		}
	}


	WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,"[{}] Order notified, instrument: {}, usertag: {}, state: {}", _id.c_str(), stdCode.c_str(), orderInfo->getUserTag(), stateToName(orderInfo->getOrderState()));

	//如果订单撤销, 并且是wt的订单, 则要先更新未完成数量
	if (orderInfo->getOrderState() == WOS_Canceled && StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern.c_str(), true))
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
		//WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[{}] {} qty of undone order updated, {} -> {}", _id.c_str(), stdCode.c_str(), oldQty, newQty);
		updateUndone(stdCode.c_str(), -qty);

		WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG, "[{}] Order {} of {} canceled:{}, action: {}, leftqty: {}",
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
			statItem._infos++;	//下单成功信息量+1

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
	if (StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern.c_str(), true))
	{
		char* userTag = (char*)orderInfo->getUserTag();
		userTag += _order_pattern.size() + 1;
		localid = strtoul(userTag, NULL, 10);
	}

	//如果是wt发出去的单子则需要更新内部数据
	if(localid != 0)
	{
		{
			SpinLock lock(_mtx_orders);
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

	WTSLogger::log_dyn("trader", _id.c_str(), LL_DEBUG,
		"[{}] Trade notified, instrument: {}, usertag: {}, trdqty: {}, trdprice: {}", 
			_id.c_str(), stdCode.c_str(), tradeRecord->getUserTag(), tradeRecord->getVolume(), tradeRecord->getPrice());

	//如果是自己的订单，则更新未完成单
	uint32_t localid = 0;
	if (StrUtil::startsWith(tradeRecord->getUserTag(), _order_pattern.c_str(), true))
	{
		char* userTag = (char*)tradeRecord->getUserTag();
		userTag += _order_pattern.size() + 1;
		localid = strtoul(userTag, NULL, 10);

		//double oldQty = _undone_qty[stdCode];
		//double newQty = oldQty - tradeRecord->getVolume()*(isBuy ? 1 : -1);
		//_undone_qty[stdCode] = newQty;
		//WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, 
		//	"[{}] {} qty of undone orders updated, {} -> {}", _id.c_str(), stdCode.c_str(), oldQty, newQty);
		updateUndone(stdCode.c_str(), -tradeRecord->getVolume());
	}

	PosItem& pItem = _positions[stdCode];
	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
	if (statInfo == NULL)
	{
		statInfo = WTSTradeStateInfo::create(stdCode.c_str());
		_stat_map->add(stdCode, statInfo, false);
	}

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

void TraderAdapter::onTraderError(WTSError* err, void* pData /* = NULL */)
{
	if(err)
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[{}] Error of trading channel occured: {}", _id.c_str(), err->getMessage());
}

IBaseDataMgr* TraderAdapter::getBaseDataMgr()
{
	return _bd_mgr;
}

void TraderAdapter::handleTraderLog(WTSLogLevel ll, const char* message)
{
	WTSLogger::log_dyn_raw("trader", _id.c_str(), ll, message);
}

bool TraderAdapter::checkCancelLimits(const char* stdCode)
{
	if (_exclude_codes.find(stdCode) != _exclude_codes.end())
		return false;

	const RiskParams* riskPara = getRiskParams(stdCode);
	if (riskPara == NULL)
		return true;

	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo && riskPara->_cancel_total_limits != 0 && statInfo->total_cancels() >= riskPara->_cancel_total_limits)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[{}] {} cancel {} times totaly, beyond boundary {} times, adding to excluding list",
			_id.c_str(), stdCode, statInfo->total_cancels(), riskPara->_cancel_total_limits);
		_exclude_codes.insert(stdCode);
		return false;
	}

	//撤单频率检查
	auto it = _cancel_time_cache.find(stdCode);
	if (it != _cancel_time_cache.end())
	{
		TimeCacheList& cache = (TimeCacheList&)it->second;
		uint32_t cnt = cache.size();
		if (cnt >= riskPara->_cancel_times_boundary)
		{
			uint64_t eTime = cache[cnt - 1];
			uint64_t sTime = eTime - riskPara->_cancel_stat_timespan * 1000;
			auto tit = std::lower_bound(cache.begin(), cache.end(), sTime);
			auto sIdx = tit - cache.begin();
			auto times = cnt - sIdx - 1;
			if (times > riskPara->_cancel_times_boundary)
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[{}] {} cancel {} times within {} seconds, beyond boundary {} times, adding to excluding list",
					_id.c_str(), stdCode, times, riskPara->_cancel_stat_timespan, riskPara->_cancel_times_boundary);
				_exclude_codes.insert(stdCode);
				return false;
			}

			//这里必须要清理一下, 没有特别好的办法
			//不然随着时间推移, vector长度会越来越长
			if (tit != cache.begin())
			{
				cache.erase(cache.begin(), tit);
			}
		}
	}

	return true;
}

bool TraderAdapter::isTradeEnabled(const char* stdCode) const
{
	if (!_risk_mon_enabled)
		return true;

	if (_exclude_codes.find(stdCode) != _exclude_codes.end())
		return false;

	return true;
}

bool TraderAdapter::checkOrderLimits(const char* stdCode)
{
	if (_exclude_codes.find(stdCode) != _exclude_codes.end())
		return false;

	const RiskParams* riskPara = getRiskParams(stdCode);
	if (riskPara == NULL)
		return true;

	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo && riskPara->_order_total_limits != 0 && statInfo->total_orders() >= riskPara->_order_total_limits)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[{}] {} entrust {} times totally, beyond boundary {} times, adding to excluding list",
			_id.c_str(), stdCode, statInfo->total_orders(), riskPara->_order_total_limits);
		_exclude_codes.insert(stdCode);
		return false;
	}

	//撤单频率检查
	auto it = _order_time_cache.find(stdCode);
	if (it != _order_time_cache.end())
	{
		TimeCacheList& cache = (TimeCacheList&)it->second;
		uint32_t cnt = cache.size();
		if (cnt >= riskPara->_order_times_boundary)
		{
			uint64_t eTime = cache[cnt - 1];
			uint64_t sTime = eTime - riskPara->_order_stat_timespan * 1000;
			auto tit = std::lower_bound(cache.begin(), cache.end(), sTime);
			auto sIdx = tit - cache.begin();
			auto times = cnt - sIdx - 1;
			if (times > riskPara->_order_times_boundary)
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[{}] {} entrust {} times within {} seconds, beyond boundary {} times, adding to excluding list",
					_id.c_str(), stdCode, times, riskPara->_order_stat_timespan, riskPara->_order_times_boundary);
				_exclude_codes.insert(stdCode);
				return false;
			}

			//这里必须要清理一下, 没有特别好的办法
			//不然随着时间推移, vector长度会越来越长
			if (tit != cache.begin())
			{
				cache.erase(cache.begin(), tit);
			}
		}
	}

	return true;
}

const TraderAdapter::RiskParams* TraderAdapter::getRiskParams(const char* stdCode)
{
	auto idx = StrUtil::findFirst(stdCode, '.');
	auto eIdx = idx + 1;
	while (isalpha(stdCode[eIdx]))
		eIdx++;


	auto it = _risk_params_map.find(std::string(stdCode + idx + 1, eIdx - idx + 1));
	if (it != _risk_params_map.end())
		return &it->second;

	it = _risk_params_map.find("default");
	return &it->second;
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
		WTSLogger::error("Same name of trading channels: {}", tname);
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

	WTSLogger::info("{} trading channels started", _adapters.size());
}

void TraderAdapterMgr::release()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->release();
	}

	_adapters.clear();
}