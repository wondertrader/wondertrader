/*!
 * \file TraderXTPXAlgo.cpp
 * \project	WonderTrader
 *
 * \author Suuuuuuunseeker
 * \date 2022/08/08
 *
 * \brief
 */
#include "TraderXTPXAlgo.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>
#include <iostream>

//#pragma comment(lib, "../API/XTPXAlgo/x64/xtptraderapi_xalgo.lib")

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(ITraderSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	const char* buffer = fmtutil::format(format, args...);

	sink->handleTraderLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderXTPXAlgo *instance = new TraderXTPXAlgo();
		return instance;
	}

	EXPORT_FLAG void deleteTrader(ITraderApi* &trader)
	{
		if (NULL != trader)
		{
			delete trader;
			trader = NULL;
		}
	}
}

const char* ENTRUST_SECTION = "entrusts";
const char* ORDER_SECTION = "orders";

inline bool IsErrorInfo(XTPRI *error_info)
{
	if (error_info && error_info->error_id != 0) {
		std::cout << "--->>> ErrorID=" << error_info->error_id << ", ErrorMsg=" << error_info->error_msg << std::endl;
		return true;
	}

	return false;
}

inline WTSDirectionType wrapPosDirection(XTP_POSITION_DIRECTION_TYPE dirType)
{
	switch (dirType)
	{
	case XTP_POSITION_DIRECTION_SHORT: return WDT_SHORT;
	default:
		return WDT_LONG;
	}
}

inline XTP_SIDE_TYPE wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if (WDT_LONG == dirType)
		if (offsetType == WOT_OPEN)
			return XTP_SIDE_BUY;
		else
			return XTP_SIDE_SELL;
	else
		if (offsetType == WOT_OPEN)
			return XTP_SIDE_SELL;
		else
			return XTP_SIDE_BUY;
}

inline WTSDirectionType wrapDirectionType(XTP_SIDE_TYPE side, XTP_POSITION_EFFECT_TYPE pe)
{
	if (XTP_SIDE_BUY == side)
		if (pe == XTP_POSITION_EFFECT_OPEN)
			return WDT_LONG;
		else
			return WDT_SHORT;
	else
		if (pe == XTP_POSITION_EFFECT_OPEN)
			return WDT_SHORT;
		else
			return WDT_LONG;
}

inline XTP_POSITION_EFFECT_TYPE wrapOffsetType(WTSOffsetType offType)
{
	if (WOT_OPEN == offType)
		return XTP_POSITION_EFFECT_OPEN;
	else if (WOT_CLOSE == offType)
		return XTP_POSITION_EFFECT_CLOSE;
	else if (WOT_CLOSETODAY == offType)
		return XTP_POSITION_EFFECT_CLOSETODAY;
	else if (WOT_CLOSEYESTERDAY == offType)
		return XTP_POSITION_EFFECT_CLOSE;
	else
		return XTP_POSITION_EFFECT_FORCECLOSE;
}

inline WTSOffsetType wrapOffsetType(XTP_POSITION_EFFECT_TYPE offType)
{
	if (XTP_POSITION_EFFECT_OPEN == offType)
		return WOT_OPEN;
	else if (XTP_POSITION_EFFECT_CLOSE == offType)
		return WOT_CLOSE;
	else if (XTP_POSITION_EFFECT_CLOSETODAY == offType)
		return WOT_CLOSETODAY;
	else if (XTP_POSITION_EFFECT_CLOSE == offType)
		return WOT_CLOSEYESTERDAY;
	else
		return WOT_FORCECLOSE;
}

inline WTSPriceType wrapPriceType(XTP_PRICE_TYPE priceType)
{
	if (XTP_PRICE_LIMIT == priceType)
		return WPT_LIMITPRICE;
	else
		return WPT_ANYPRICE;
}

inline WTSOrderState wrapOrderState(XTP_ORDER_STATUS_TYPE orderState)
{
	switch (orderState)
	{
	case XTP_ORDER_STATUS_INIT:
		return WOS_NotTraded_NotQueuing;
	case XTP_ORDER_STATUS_ALLTRADED:
		return WOS_AllTraded;
	case XTP_ORDER_STATUS_PARTTRADEDQUEUEING:
		return WOS_PartTraded_Queuing;
	case XTP_ORDER_STATUS_NOTRADEQUEUEING:
		return WOS_NotTraded_Queuing;
	case XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING:
	case XTP_ORDER_STATUS_CANCELED:
		return WOS_Canceled;
	default:
		return WOS_Nottouched;
	}
}

inline uint32_t makeRefID()
{
	static std::atomic<uint32_t> auto_refid(0);
	if (auto_refid == 0)
		auto_refid = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20220101, 0)) / 1000 * 100);
	return auto_refid.fetch_add(1);
}


TraderXTPXAlgo::TraderXTPXAlgo()
	: _api(NULL)
	, _sink(NULL)
	, _ordref(makeRefID())
	, _reqid(1)
	, _orders(NULL)
	, _trades(NULL)
	, _positions(NULL)
	, _bd_mgr(NULL)
	, _tradingday(0)
	, _inited(false)
{

}


TraderXTPXAlgo::~TraderXTPXAlgo()
{

}

#pragma region "XTP::API::TraderSpi"

WTSEntrust* TraderXTPXAlgo::makeEntrust(XTPOrderInfo* order_info)
{
	std::string code, exchg;
	if (order_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_info->ticker;
	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		code.c_str(),
		(uint32_t)order_info->quantity,
		order_info->price,
		ct->getExchg());
	pRet->setContractInfo(ct);
	pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
	pRet->setPriceType(wrapPriceType(order_info->price_type));
	pRet->setOffsetType(wrapOffsetType(order_info->position_effect));
	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), order_info->order_client_id);

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSEntrust* TraderXTPXAlgo::makeEntrust(XTPStrategyInfoStruct *stra_info)
{
	uint16_t				m_strategy_type;		///< 策略类型
	XTPStrategyStateType	m_strategy_state;		///< 策略状态
	uint64_t				m_client_strategy_id;	///< 客户策略id
	uint64_t				m_xtp_strategy_id;		///< xtp策略id

	std::string stra_type = std::to_string(stra_info->m_strategy_type);
	std::string stra_id = std::to_string(stra_info->m_xtp_strategy_id);
	WTSContractInfo* ct = _bd_mgr->getContract(stra_type.c_str(), stra_id.c_str());
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(stra_id.c_str(), 0, 0);
	pRet->setContractInfo(ct);

	genEntrustID(pRet->getEntrustID(), stra_info->m_client_strategy_id);

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSOrderInfo* TraderXTPXAlgo::makeOrderInfo(XTPQueryOrderRsp* order_info)
{
	std::string code, exchg;
	if (order_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_info->ticker;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(order_info->price);
	pRet->setVolume((uint32_t)order_info->quantity);
	pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
	pRet->setPriceType(wrapPriceType(order_info->price_type));
	pRet->setOrderFlag(WOF_NOR);
	pRet->setOffsetType(wrapOffsetType(order_info->position_effect));

	pRet->setVolTraded((uint32_t)order_info->qty_traded);
	pRet->setVolLeft((uint32_t)order_info->qty_left);

	pRet->setCode(code.c_str());
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate((uint32_t)(order_info->insert_time / 1000000000));
	uint32_t uTime = order_info->insert_time % 1000000000;
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrderState(order_info->order_status));
	if (order_info->order_status >= XTP_ORDER_STATUS_REJECTED)
		pRet->setError(true);

	genEntrustID(pRet->getEntrustID(), order_info->order_client_id);
	fmtutil::format_to(pRet->getOrderID(), "{}", order_info->order_xtp_id);

	pRet->setStateMsg("");

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) == 0)
	{
		pRet->setUserTag(pRet->getEntrustID());
	}
	else
	{
		pRet->setUserTag(usertag);

		if (strlen(pRet->getOrderID()) > 0)
		{
			m_oidCache.put(StrUtil::trim(pRet->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
				write_log(_sink, LL_ERROR, message);
			});
		}
	}

	return pRet;
}

WTSOrderInfo* TraderXTPXAlgo::makeOrderInfo(XTPStrategyInfoStruct *strategyState)
{
	uint16_t stra_type = strategyState->m_strategy_type;  ///< 策略类型
	uint64_t stra_id = strategyState->m_xtp_strategy_id;  ///< xtp策略id
	WTSContractInfo* contract = _bd_mgr->getContract(std::to_string(stra_type).c_str(), std::to_string(stra_id).c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);

	genEntrustID(pRet->getEntrustID(), strategyState->m_client_strategy_id);
	fmtutil::format_to(pRet->getOrderID(), "{}", strategyState->m_xtp_strategy_id);

	pRet->setStateMsg("");

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) == 0)
	{
		pRet->setUserTag(pRet->getEntrustID());
	}
	else
	{
		pRet->setUserTag(usertag);

		if (strlen(pRet->getOrderID()) > 0)
		{
			m_oidCache.put(StrUtil::trim(pRet->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
				write_log(_sink, LL_ERROR, message);
			});
		}
	}

	return pRet;
}

WTSTradeInfo* TraderXTPXAlgo::makeTradeInfo(XTPQueryTradeRsp* trade_info)
{
	std::string code, exchg;
	if (trade_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = trade_info->ticker;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	pRet->setVolume((uint32_t)trade_info->quantity);
	pRet->setPrice(trade_info->price);
	pRet->setTradeID(trade_info->exec_id);
	pRet->setContractInfo(contract);

	uint32_t uTime = (uint32_t)(trade_info->trade_time % 1000000000);
	uint32_t uDate = (uint32_t)(trade_info->trade_time / 1000000000);

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime));

	WTSDirectionType dType = wrapDirectionType(trade_info->side, trade_info->position_effect);

	pRet->setDirection(dType);
	pRet->setOffsetType(wrapOffsetType(trade_info->position_effect));
	fmtutil::format_to(pRet->getRefOrder(), "{}", trade_info->order_xtp_id);
	pRet->setTradeType(WTT_Common);

	double amount = trade_info->quantity*pRet->getPrice();
	pRet->setAmount(amount);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

void TraderXTPXAlgo::OnDisconnected(uint64_t session_id, int reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, reason);

	_asyncio.post([this]() {
		write_log(_sink, LL_WARN, "[TraderXTPXAlgo] Connection lost, relogin in 2 seconds...");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		doLogin();  // 登录
	});
}

void TraderXTPXAlgo::OnError(XTPRI *error_info)
{
	if (_sink && error_info)
		write_log(_sink, LL_ERROR, error_info->error_msg);
}

void TraderXTPXAlgo::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id)
{
	if (IsErrorInfo(error_info))
	{
		WTSEntrust* entrust = makeEntrust(order_info);

		WTSError* error = WTSError::create(WEC_ORDERINSERT, error_info->error_msg);
		_sink->onRspEntrust(entrust, error);
		error->release();

		entrust->release();
	}
	else
	{
		WTSOrderInfo *orderInfo = makeOrderInfo(order_info);
		if (orderInfo)
		{
			if (_sink)
				_sink->onPushOrder(orderInfo);

			orderInfo->release();
		}
	}

	//std::cout << "------------------------ OnOrderEvent ------------------------" << "\n";

	//std::cout << "order_xtp_id: " << order_info->order_xtp_id;
	//std::cout << " order_client_id: " << order_info->order_client_id;
	//std::cout << " ticker: " << order_info->ticker;
	//std::cout << " market: " << order_info->market;
	//std::cout << " price: " << order_info->price;
	//std::cout << " quantity: " << order_info->quantity;
	//std::cout << " price_type: " << order_info->price_type;
	//std::cout << " business_type: " << order_info->business_type;
	//std::cout << " qty_traded: " << order_info->qty_traded;
	//std::cout << " qty_left: " << order_info->qty_left;
	//std::cout << " trade_amount: " << order_info->trade_amount;
	//std::cout << " order_status: " << order_info->order_status;
	//std::cout << " order_type: " << order_info->order_type;
	//std::cout << " order_submit_status: " << order_info->order_submit_status;
	//std::cout << std::endl;

	//_api->GetAlgorithmIDByOrder(order_info->order_xtp_id, order_info->order_client_id);
}

void TraderXTPXAlgo::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id)
{
	WTSTradeInfo *trdInfo = makeTradeInfo(trade_info);
	if (trdInfo)
	{
		if (_sink)
			_sink->onPushTrade(trdInfo);

		trdInfo->release();
	}

	//std::cout << "------------------------ OnTradeEvent ------------------------" << "\n";

	//std::cout << "order_xtp_id: " << trade_info->order_xtp_id;
	//std::cout << " order_client_id: " << trade_info->order_client_id;
	//std::cout << " ticker: " << trade_info->ticker;
	//std::cout << " market: " << trade_info->market;
	//std::cout << " price: " << trade_info->price;
	//std::cout << " quantity: " << trade_info->quantity;
	//std::cout << " business_type: " << trade_info->business_type;
	//std::cout << " trade_amount: " << trade_info->trade_amount;
	//std::cout << " branch_pbu: " << trade_info->branch_pbu;
	//std::cout << " trade_type: " << trade_info->trade_type;
	//std::cout << std::endl;

	//_api->GetAlgorithmIDByOrder(trade_info->order_xtp_id, trade_info->order_client_id);
}

void TraderXTPXAlgo::OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	//if (is_last)
	//{
	//	m_bInQuery = false;
	//	triggerQuery();
	//}

	if (!IsErrorInfo(error_info) && trade_info)
	{
		if (NULL == _trades)
			_trades = WTSArray::create();

		WTSTradeInfo* trdInfo = makeTradeInfo(trade_info);
		if (trdInfo)
		{
			_trades->append(trdInfo, false);
		}
	}

	if (is_last)
	{
		if (_sink)
			_sink->onRspTrades(_trades);

		if (_trades)
			_trades->clear();
	}
}

void TraderXTPXAlgo::OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	//if (is_last)
	//{
	//	m_bInQuery = false;
	//	triggerQuery();
	//}

	if (!IsErrorInfo(error_info) && position)
	{
		if (NULL == _positions)
			_positions = PositionMap::create();

		std::string code, exchg;
		if (position->market == XTP_MKT_SH_A)
			exchg = "SSE";
		else
			exchg = "SZSE";
		code += position->ticker;
		WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
		if (contract)
		{
			WTSCommodityInfo* commInfo = contract->getCommInfo();
			std::string key = fmt::format("{}-{}", code.c_str(), position->position_direction);
			WTSPositionItem* pos = (WTSPositionItem*)_positions->get(key);
			if (pos == NULL)
			{
				pos = WTSPositionItem::create(code.c_str(), commInfo->getCurrency(), commInfo->getExchg());
				pos->setContractInfo(contract);
				_positions->add(key, pos, false);
			}
			pos->setDirection(wrapPosDirection(position->position_direction));

			pos->setNewPosition((double)(position->total_qty - position->yesterday_position));
			pos->setPrePosition((double)position->yesterday_position);

			pos->setMargin(position->total_qty*position->avg_price);
			pos->setDynProfit(0);
			pos->setPositionCost(position->total_qty*position->avg_price);

			pos->setAvgPrice(position->avg_price);

			pos->setAvailNewPos(0);
			pos->setAvailPrePos((double)position->sellable_qty);
		}
	}

	if (is_last)
	{

		WTSArray* ayPos = WTSArray::create();

		if (_positions && _positions->size() > 0)
		{
			for (auto it = _positions->begin(); it != _positions->end(); it++)
			{
				ayPos->append(it->second, true);
			}
		}

		if (_sink)
			_sink->onRspPosition(ayPos);

		if (_positions)
		{
			_positions->release();
			_positions = NULL;
		}

		ayPos->release();
	}
}

void TraderXTPXAlgo::OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	//if (is_last)
	//{
	//	m_bInQuery = false;
	//	triggerQuery();
	//}

	if (is_last && !IsErrorInfo(error_info) && asset)
	{
		WTSAccountInfo* accountInfo = WTSAccountInfo::create();
		accountInfo->setPreBalance(asset->orig_banlance);
		accountInfo->setCloseProfit(0);
		accountInfo->setDynProfit(0);
		accountInfo->setMargin(0);
		accountInfo->setAvailable(asset->buying_power);
		accountInfo->setCommission(asset->fund_sell_fee);
		accountInfo->setFrozenMargin(asset->withholding_amount);
		accountInfo->setFrozenCommission(0);
		if (asset->deposit_withdraw > 0)
			accountInfo->setDeposit(asset->deposit_withdraw);
		else if (asset->deposit_withdraw < 0)
			accountInfo->setWithdraw(0);
		accountInfo->setBalance(asset->total_asset);
		accountInfo->setCurrency("CNY");

		WTSArray * ay = WTSArray::create();
		ay->append(accountInfo, false);
		if (_sink)
			_sink->onRspAccount(ay);

		ay->release();
	}
}

void TraderXTPXAlgo::OnAlgoDisconnected(int reason)
{
	std::cout << "------------------- OnAlgoDisconnected -----------" << std::endl;
	/// 与算法服务器断线，此时api会自动与算法服务器重连，无需用户重连
}

void TraderXTPXAlgo::OnAlgoConnected()
{
	std::cout << "------------------- OnAlgoConnected -----------" << std::endl;
	/// 与算法服务器重新建立起连接，仅在断连重连成功后通知
}

void TraderXTPXAlgo::OnQueryStrategy(XTPStrategyInfoStruct* strategy_info, char* strategy_param, XTPRI *error_info, int32_t request_id, bool is_last, uint64_t session_id)
{
	std::cout << "------------------- OnQueryStrategy -----------" << std::endl;

	/// 算法单查询结果通知
	if (error_info->error_id != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Query strategy failed: {}", error_info->error_msg);
		return;
	}

	//std::cout << "request_id: " << request_id << ", is_last:" << is_last;
	//std::cout << ",strategy: " << strategy_info->m_xtp_strategy_id << ", client id:" << strategy_info->m_client_strategy_id << ", type:" << strategy_info->m_strategy_type << ", status:" << strategy_info->m_strategy_state;//<< std::endl;
	//std::cout << ",strategy_param: " << strategy_param << std::endl;

	if (!IsErrorInfo(error_info) && strategy_info)
	{
		if (NULL == _orders)
			_orders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(strategy_info);
		if (orderInfo)
		{
			_orders->append(orderInfo, false);
		}
	}

	if (is_last)
	{
		if (_sink)
			_sink->onRspOrders(_orders);

		if (_orders)
			_orders->clear();
	}
}

void TraderXTPXAlgo::OnStrategyStateReport(XTPStrategyStateReportStruct* strategy_state, uint64_t session_id)
{
	std::cout << "------------------- OnStrategyStateReport -----------" << std::endl;

	/// 如果母单执行状态为STOPPED，此时执行算法停止
	/// 如果算法提前执行完毕，则会持续推送到算法设置的最终点以更新滑点信息等
	/// 最后推送一个最终态的母单状态回报
	if (strategy_state->m_strategy_info.m_strategy_state == XTP_STRATEGY_STATE_STOPPED)   
	{
		std::cout << "------------------ Algo Execution Stopped -----------------" << std::endl;

		std::cout << "m_client_strategy_id: " << strategy_state->m_strategy_info.m_client_strategy_id << std::endl;
		std::cout << "m_strategy_type: " << strategy_state->m_strategy_info.m_strategy_type << std::endl;
		std::cout << "m_strategy_qty: " << strategy_state->m_strategy_qty << std::endl;
		std::cout << "m_strategy_ordered_qty: " << strategy_state->m_strategy_ordered_qty << std::endl;
		std::cout << "m_strategy_execution_qty: " << strategy_state->m_strategy_execution_qty << std::endl;
		std::cout << "m_strategy_cancelled_qty: " << strategy_state->m_strategy_cancelled_qty << std::endl;
		std::cout << "m_strategy_unclosed_qty: " << strategy_state->m_strategy_unclosed_qty << std::endl;
		std::cout << "m_strategy_asset: " << strategy_state->m_strategy_asset << std::endl;
		std::cout << "m_strategy_ordered_asset: " << strategy_state->m_strategy_ordered_asset << std::endl;
		std::cout << "m_strategy_execution_asset: " << strategy_state->m_strategy_execution_asset << std::endl;
		std::cout << "m_strategy_execution_price: " << strategy_state->m_strategy_execution_price << std::endl;
		std::cout << "m_strategy_price_diff: " << strategy_state->m_strategy_price_diff << std::endl;
		std::cout << "m_strategy_asset_diff: " << strategy_state->m_strategy_asset_diff << std::endl;
	}
	else 
	{
		std::cout << "------------------ Algo Execution Running -----------------" << std::endl;

		std::cout << "m_client_strategy_id: " << strategy_state->m_strategy_info.m_client_strategy_id << std::endl;
		std::cout << "m_strategy_type: " << strategy_state->m_strategy_info.m_strategy_type << std::endl;
		std::cout << "m_strategy_qty: " << strategy_state->m_strategy_qty << std::endl;
		std::cout << "m_strategy_ordered_qty: " << strategy_state->m_strategy_ordered_qty << std::endl;
		std::cout << "m_strategy_execution_qty: " << strategy_state->m_strategy_execution_qty << std::endl;
		std::cout << "m_strategy_cancelled_qty: " << strategy_state->m_strategy_cancelled_qty << std::endl;
		std::cout << "m_strategy_unclosed_qty: " << strategy_state->m_strategy_unclosed_qty << std::endl;
		std::cout << "m_strategy_asset: " << strategy_state->m_strategy_asset << std::endl;
		std::cout << "m_strategy_ordered_asset: " << strategy_state->m_strategy_ordered_asset << std::endl;
		std::cout << "m_strategy_execution_asset: " << strategy_state->m_strategy_execution_asset << std::endl;
		std::cout << "m_strategy_execution_price: " << strategy_state->m_strategy_execution_price << std::endl;
		std::cout << "m_strategy_price_diff: " << strategy_state->m_strategy_price_diff << std::endl;
		std::cout << "m_strategy_asset_diff: " << strategy_state->m_strategy_asset_diff << std::endl;
	}
}

void TraderXTPXAlgo::OnALGOUserEstablishChannel(char* user, XTPRI* error_info, uint64_t session_id)
{
	std::cout << "--------------- OnALGOUserEstablishChannel -----------" << std::endl;
	/// 建立算法通道的异步通知
	if (error_info->error_id == 0)
	{
		/// 建立算法通道成功后，可以下算法母单
		//std::cout << user << " establish channel success." << std::endl;
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Establish channel failed: {}", error_info->error_msg);
	}
}

void TraderXTPXAlgo::OnInsertAlgoOrder(XTPStrategyInfoStruct* strategy_info, XTPRI *error_info, uint64_t session_id)
{
	std::cout << "----------------- OnInsertAlgoOrder --------------" << std::endl;

	/// 发送算法单后的异步通知
	if (error_info->error_id != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Insert algo order failed: {}", error_info->error_msg);

		WTSEntrust* entrust = makeEntrust(strategy_info);

		WTSError* error = WTSError::create(WEC_ORDERINSERT, error_info->error_msg);
		_sink->onRspEntrust(entrust, error);
		error->release();

		entrust->release();

		return;
	}

	/// 算法单建立成功
	std::cout << "Insert algo order success." << std::endl;
	//std::cout << "strategy:" << strategy_info->m_xtp_strategy_id << ", client id:" << strategy_info->m_client_strategy_id << ", type:" << strategy_info->m_strategy_type << ", status:" << (strategy_info->m_strategy_state - 0) << std::endl;
	
	/// 需要把策略ID缓存起来以便撤单时用
	WTSOrderInfo *orderInfo = makeOrderInfo(strategy_info);
	if (orderInfo)
	{
		if (_sink)
			_sink->onPushOrder(orderInfo);

		orderInfo->release();
	}
}

void TraderXTPXAlgo::OnCancelAlgoOrder(XTPStrategyInfoStruct* strategy_info, XTPRI *error_info, uint64_t session_id)
{
	std::cout << "------------------- OnCancelAlgoOrder-----------" << std::endl;

	/// 算法单撤销结果通知
	if (error_info->error_id != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Cancel algo order failed: {}", error_info->error_msg);

		WTSError* error = WTSError::create(WEC_ORDERCANCEL, error_info->error_msg);
		_sink->onTraderError(error);
		error->release();
		return;
	}

	std::cout << "Cancel algo order success." << std::endl;
	//std::cout << "strategy:" << strategy_info->m_xtp_strategy_id << ", client id:" << strategy_info->m_client_strategy_id << ", type:" << strategy_info->m_strategy_type << ", status:" << (strategy_info->m_strategy_state - 0) << std::endl;
}

#pragma endregion "XTP::API:TraderSpi"

#pragma region "ITraderApi"
bool TraderXTPXAlgo::init(WTSVariant *params)
{
	// 交易服务器登录设置
	_user = params->getCString("user");
	_pass = params->getCString("pass");
	_host_oms = params->getCString("front_oms");
	_port_oms = params->getInt32("port_oms");

	_acct_key = params->getCString("account_key");

	// 算法服务器登陆设置
	_acct_algo = params->getCString("account_algo");
	_password_algo = params->getCString("password_algo");
	_host_algo = params->getCString("front_algo");
	_port_algo = params->getInt32("port_algo");

	// 算法参数设置
	_client = params->getInt32("client_id");
	std::cout << "client: " << _client << std::endl;
	_client = 1;
	std::cout << "client: " << _client << std::endl;
	_algo_type = params->getInt32("algo_type");
	_algo_params = params->getCString("algo_params");
	_resume_type = params->getInt32("resume_type");

	_log_dir = params->getCString("path");

	_flowdir = params->getCString("flowdir");

	if (_flowdir.empty())
		_flowdir = "XAlgoTDFlow";

	_flowdir = StrUtil::standardisePath(_flowdir);

	std::string module = params->getCString("xtpmodule");
	if (module.empty()) module = "xtptraderapi_xalgo";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;

	m_hInstXTP = DLLHelper::load_library(dllpath.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateTraderApi@TraderApi@API@XTP@@SAPEAV123@EPEBDW4XTP_LOG_LEVEL@@@Z";
#	else
	const char* creatorName = "?CreateTraderApi@TraderApi@API@XTP@@SAPAV123@EPBDW4XTP_LOG_LEVEL@@@Z";
#	endif
#else
	const char* creatorName = "_ZN3XTP3API9TraderApi15CreateTraderApiEhPKc13XTP_LOG_LEVEL";
#endif
	m_funcCreator = (XTPCreator)DLLHelper::get_symbol(m_hInstXTP, creatorName);

	return true;
}

void TraderXTPXAlgo::release()
{
	if (_api)
	{
		_api->RegisterSpi(NULL);
		_api->Release();
		_api = NULL;
	}

	if (_orders)
		_orders->clear();

	if (_positions)
		_positions->clear();

	if (_trades)
		_trades->clear();
}

void TraderXTPXAlgo::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderXTPXAlgo::reconnect()
{
	if (_api)
	{
		_api->RegisterSpi(NULL);
		_api->Release();
		_api = NULL;
	}

	std::stringstream ss;
	ss << _flowdir << "flows/" << _user << "/";
	boost::filesystem::create_directories(ss.str().c_str());
	_api = m_funcCreator(_client, ss.str().c_str(), XTP_LOG_LEVEL_DEBUG);			// 创建UserApi
	if (_api == NULL)
	{
		if (_sink)
			_sink->handleEvent(WTE_Connect, -1);
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Module initializing failed");

		StdThreadPtr thrd(new StdThread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			write_log(_sink, LL_WARN, "[TraderXTPXAlgo] {} reconnecting...", _user.c_str());
			reconnect();
		}));
		return;
	}

	//_api->SubscribePublicTopic(_quick ? XTP_TERT_QUICK : XTP_TERT_RESUME);
	_api->SubscribePublicTopic((XTP_TE_RESUME_TYPE)_resume_type);
	_api->SetSoftwareVersion("1.0.0"); //设定此软件的开发版本号,用户自定义
	_api->SetSoftwareKey(_acct_key.c_str());//设定用户的开发代码,在XTP申请开户时,由xtp人员提供
	_api->SetHeartBeatInterval(15);//设定交易服务器超时时间,单位为秒,此为1.1.16新增接口
	_api->RegisterSpi(this);						// 注册事件

	const char* version = _api->GetApiVersion();
	std::cout << "API version is " << version << std::endl;

	if (_sink)
		_sink->handleEvent(WTE_Connect, 0);
}

void TraderXTPXAlgo::connect()
{
	reconnect();

	if (_thrd_worker == NULL)
	{
		_worker.reset(new boost::asio::io_service::work(_asyncio));
		_thrd_worker.reset(new StdThread([this]() {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				_asyncio.run_one();
				//m_asyncIO.run();
			}
		}));
	}
}

void TraderXTPXAlgo::disconnect()
{
	release();
}

bool TraderXTPXAlgo::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderXTPXAlgo::genEntrustID(char* buffer, uint32_t orderRef)
{
	//这里不再使用sessionid，因为每次登陆会不同，如果使用的话，可能会造成不唯一的情况
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderXTPXAlgo::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderXTPXAlgo::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		uint32_t orderref = _ordref.fetch_add(1) + 1;
		fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderXTPXAlgo::doLogin()
{
	_state = TS_LOGINING;

	// 登录XTP交易服务器
	std::cout << _user << " begin to login to OMS." << std::endl;
	uint64_t iResult = _api->Login(_host_oms.c_str(), _port_oms, _user.c_str(), _pass.c_str(), XTP_PROTOCOL_TCP);
	if (iResult == 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Login to OMS failed: {}", error_info->error_msg);
		std::string msg = error_info->error_msg;
		_state = TS_LOGINFAILED;
		_asyncio.post([this, msg] {
			_sink->onLoginResult(false, msg.c_str(), 0);
		});
	}
	else
	{
		_sessionid = iResult;
		if (!_inited)
		{
			_tradingday = strtoul(_api->GetTradingDay(), NULL, 10);

			{
				//初始化委托单缓存器
				std::stringstream ss;
				ss << "./xtpdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_eid.sc";
				m_eidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			{
				//初始化订单标记缓存器
				std::stringstream ss;
				ss << "./xtpdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_oid.sc";
				m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			write_log(_sink, LL_INFO, "[TraderXTPXAlgo] [{}] Login succeed, trading date: {}...", _user.c_str(), _tradingday);

			_state = TS_LOGINED;
			_inited = true;
			_asyncio.post([this] {
				_sink->onLoginResult(true, 0, _tradingday);
				_state = TS_ALLREADY;
			});
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderXTPXAlgo] [{}] Connection recovered", _user.c_str());
			_state = TS_ALLREADY;
		}
	}

	// 登录AlgoBus算法服务器
	std::cout << _acct_algo << " begin to login AlgoBus." << std::endl;
	int login_ret = 0;
	login_ret = _api->LoginALGO(_host_algo.c_str(), _port_algo, _acct_algo.c_str(), _password_algo.c_str(), XTP_PROTOCOL_TCP);

	if (login_ret != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Login AlgoBus failed: {}", error_info->error_msg);
		std::string msg = error_info->error_msg;
		_state = TS_LOGINFAILED;
		_asyncio.post([this, msg] {
			_sink->onLoginResult(false, msg.c_str(), 0);
		});
	}
	else
	{
		_sessionid = iResult;
		if (!_inited)
		{
			_tradingday = strtoul(_api->GetTradingDay(), NULL, 10);

			{
				//初始化委托单缓存器
				std::stringstream ss;
				ss << "./xtpdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_eid.sc";
				m_eidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			{
				//初始化订单标记缓存器
				std::stringstream ss;
				ss << "./xtpdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_oid.sc";
				m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			write_log(_sink, LL_INFO, "[TraderXTPXAlgo] [{}] Login AlgoBus succeed, trading date: {}...", _user.c_str(), _tradingday);

			_state = TS_LOGINED;
			_inited = true;
			_asyncio.post([this] {
				_sink->onLoginResult(true, 0, _tradingday);
				_state = TS_ALLREADY;
			});
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderXTPXAlgo] [{}] Connection recovered", _user.c_str());
			_state = TS_ALLREADY;
		}

		///在用户成功登录交易服务器后，算法用户建立算法通道
		if (_sessionid != 0)
		{
			std::cout << _user << " begin to establish channel." << std::endl;
			int user_ret = _api->ALGOUserEstablishChannel(_host_oms.c_str(), _port_oms, _user.c_str(), _pass.c_str(), _sessionid);

			if (user_ret == 0) {
				write_log(_sink, LL_INFO, "[TraderXTPXAlgo] Establish channel send success: {}", user_ret);
			}
			else {
				auto error_info = _api->GetApiLastError();
				write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Establish channel send error: {}", error_info->error_msg);
				std::string msg = error_info->error_msg;
				_state = TS_LOGINFAILED;
				_asyncio.post([this, msg] {
					_sink->onLoginResult(false, msg.c_str(), 0);
				});
			}
		}
	}
}

int TraderXTPXAlgo::login(const char* user, const char* pass, const char* productInfo)
{
	_user = user;
	_pass = pass;

	if (_api == NULL)
	{
		return -1;
	}

	doLogin();
	return 0;
}

int TraderXTPXAlgo::logout()
{
	if (_api == NULL)
		return -1;

	_api->Logout(_sessionid);
	return 0;
}

int TraderXTPXAlgo::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
			write_log(_sink, LL_WARN, message);
		});
	}

	std::cout << "Begin to insert algo order." << std::endl;
	uint64_t iResult = _api->InsertAlgoOrder(_algo_type, _client, (char*)_algo_params.c_str(), _sessionid);
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Order inserting failed: {}", error_info->error_msg);
	}

	return 0;
}

int TraderXTPXAlgo::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	uint64_t iResult = _api->CancelAlgoOrder(true, strtoull(action->getOrderID(), NULL, 10), _sessionid);
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Order cancelling failed: {}", error_info->error_msg);
	}

	return 0;
}

uint32_t TraderXTPXAlgo::genRequestID()
{
	return _reqid.fetch_add(1) + 1;
}

int TraderXTPXAlgo::queryAccount()
{
	int iResult = _api->QueryAsset(_sessionid, genRequestID());
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Account querying failed: {}", error_info->error_msg);
	}

	return 0;
}

int TraderXTPXAlgo::queryPositions()
{
	int iResult = _api->QueryPosition("", _sessionid, genRequestID());
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Positions querying failed: {}", error_info->error_msg);
	}

	return 0;
}

int TraderXTPXAlgo::queryOrders()
{
	int iResult = _api->QueryStrategy(0, 0, 0, _sessionid, genRequestID());  // 查询所有
	if (iResult != 0)
	{
		auto err_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPAlgo] Query orders failed: {}", err_info->error_msg);
	}

	return 0;
}

int TraderXTPXAlgo::queryTrades()
{
	XTPQueryTraderReq req;
	memset(&req, 0, sizeof(XTPQueryTraderReq));
	int iResult = _api->QueryTrades(&req, _sessionid, genRequestID());
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTPXAlgo] Trades querying failed: {}", error_info->error_msg);
	}

	return 0;
}
#pragma endregion "ITraderApi"
