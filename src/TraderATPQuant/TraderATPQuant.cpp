/*!
 * \file TraderATPQuant.cpp
 * \project	WonderTrader
 *
 * \author Sunnnnker
 * \date 2022/10/30
 *
 * \brief 
 * \ this is for XYZQ
 */
#include "TraderATPQuant.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>
#include <iostream>


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
		TraderATPQuant *instance = new TraderATPQuant();
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

inline uint32_t makeRefID()
{
	static std::atomic<uint32_t> auto_refid(0);
	if (auto_refid == 0)
		auto_refid = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20220101, 0)) / 1000 * 100);
	return auto_refid.fetch_add(1);
}


TraderATPQuant::TraderATPQuant()
	: _api(NULL)
	, _sink(NULL)
	, _ordref(makeRefID())
	, _reqid(1)
	, _orders(NULL)
	, _trades(NULL)
	, _positions(NULL)
	, _bd_mgr(NULL)
	, _tradingday(0)
	, _close_flag(false)
	, ayOrders(NULL)
	, ayTrades(NULL)
	, ayFunds(NULL)
	, _id_counts(0)
{

}


TraderATPQuant::~TraderATPQuant()
{

}


inline WTSDirectionType wrapATPSide(const ATPSideType side)
{
	if (side == ATPSideConst::kBuy || side == ATPSideConst::kFinancingBuy)
		return WDT_LONG;
	else if (side == ATPSideConst::kSell || side == ATPSideConst::kLoanSell)
		return WDT_SHORT;
	else
		return WDT_NET;
}

inline ATPOrdTypeType wrapOrderType(WTSPriceType priceType, WTSOrderFlag flag)
{

	if (WPT_LIMITPRICE == priceType && flag == WOF_NOR)
		return ATPOrderTypeConst::kFixedNew;  // 限价委托、增强限价

	if (WPT_LIMITPRICE == priceType && flag == WOF_FOK)
		return ATPOrderTypeConst::kFixedFullDealOrCancel;  // 限价全额成交或撤销

	if (WPT_ANYPRICE == priceType && flag == WOF_NOR)
		return ATPOrderTypeConst::kMarketTransferFixed;  // 市价剩余转限价

	if (WPT_ANYPRICE == priceType && flag == WOF_FAK)
		return ATPOrderTypeConst::kImmediateDealTransferCancel;  // 立即成交剩余撤销

	if (WPT_ANYPRICE == priceType && flag == WOF_FOK)
		return ATPOrderTypeConst::kFullDealOrCancel;  // 全额成交或撤销

	return ATPOrderTypeConst::kDefault;
}

inline WTSPriceType wrapOrderType(ATPOrdTypeType ordType)
{
	if (ordType == ATPOrderTypeConst::kFixedNew || ordType == ATPOrderTypeConst::kSzBiddingFixed || ordType == ATPOrderTypeConst::kShBiddingFixed || ordType == ATPOrderTypeConst::kFixedFullDealOrCancel)
		return WPT_LIMITPRICE;
	if (ordType == ATPOrderTypeConst::kImmediateDealTransferCancel || ordType == ATPOrderTypeConst::kFullDealOrCancel || ordType == ATPOrderTypeConst::kMarketTransferFixed)
		return WPT_ANYPRICE;
	else if (ordType == ATPOrderTypeConst::kLocalOptimalNew || ordType == ATPOrderTypeConst::kOptimalFiveLevelFullDealTransferCancel || ordType == ATPOrderTypeConst::kOptimalFiveLevelImmediateDealTransferFixed || ordType == ATPOrderTypeConst::kCountPartyOptimalTransferFixed)
		return WPT_BESTPRICE;
	else
		return WPT_LASTPRICE;
}

inline WTSOrderState wrapOrderStatus(ATPOrdStatusType orderState)
{
	switch (orderState)
	{
	case ATPOrdStatusConst::kFilled:
		return WOS_AllTraded;
	case ATPOrdStatusConst::kPartiallyFilled:
		return WOS_PartTraded_Queuing;
	case ATPOrdStatusConst::kPartiallyFilledPartiallyCancelled:
		return WOS_PartTraded_NotQueuing;
	case ATPOrdStatusConst::kWaitCancelled:
	case ATPOrdStatusConst::kPartiallyFilledWaitCancelled:
		return WOS_Cancelling;
	case ATPOrdStatusConst::kCancelled:
	case ATPOrdStatusConst::kCanceledSuccess:
	case ATPOrdStatusConst::kInternalCanceledOrder:
		return WOS_Canceled;
	case ATPOrdStatusConst::kSended:
	case ATPOrdStatusConst::kNew:
		return WOS_Submitting;
	default:
		return WOS_Nottouched;
	}
}

WTSEntrust* TraderATPQuant::makeEntrust(const ATPRspCashAuctionOrderMsg& order_info)
{
	const std::string code(order_info.GetSecurityId());
	const std::string exchg((order_info.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE"  : "SZSE");

	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		code.c_str(),
		(uint32_t)order_info.GetOrderQty(),
		order_info.GetPrice(),
		ct->getExchg());
	pRet->setContractInfo(ct);
	pRet->setDirection(wrapATPSide(order_info.GetSide()));
	pRet->setPriceType(wrapOrderType(order_info.GetOrderType()));
	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), order_info.GetClOrdNo());

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSEntrust* TraderATPQuant::makeEntrust(const ATPRtnCashAuctionOrderMsg& msg)
{
	const std::string code(msg.GetSecurityId());
	const std::string exchg((msg.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE"  : "SZSE");

	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		code.c_str(),
		(uint32_t)msg.GetOrderQty(),
		msg.GetPrice(),
		ct->getExchg());
	pRet->setContractInfo(ct);
	pRet->setDirection(wrapATPSide(msg.GetSide()));
	pRet->setPriceType(WPT_LIMITPRICE);
	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), msg.GetClOrdNo());

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSOrderInfo* TraderATPQuant::makeOrderInfo(const ATPRspCashOrderQueryResultMsg& order_info)
{
	const std::string code(order_info.GetSecurityId());
	const std::string exchg((order_info.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE");

	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();

	pRet->setContractInfo(contract);
	pRet->setPrice(order_info.GetOrderPrice());
	pRet->setVolume(order_info.GetOrderQty());
	pRet->setDirection(wrapATPSide(order_info.GetSide()));
	pRet->setPriceType(WPT_LIMITPRICE);  // 默认是限价
	pRet->setOrderFlag(WOF_NOR);

	pRet->setVolTraded(order_info.GetCumQty());
	pRet->setVolLeft(order_info.GetLeavesQty());

	pRet->setCode(code.c_str());
	pRet->setExchange(exchg.c_str());

	pRet->setOrderDate((uint32_t)(order_info.GetTransactTime() / 1000000000));
	uint32_t uTime = (uint32_t)(order_info.GetTransactTime() % 1000000000);
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrderStatus(order_info.GetOrdStatus()));
	
	if (order_info.GetOrdStatus() == ATPOrdStatusConst::kReject || order_info.GetOrdStatus() == ATPOrdStatusConst::kUnSend)
	{
		pRet->setError(true);
	}

	genEntrustID(pRet->getEntrustID(), order_info.GetClOrdNo());
	//pRet->setEntrustID(genEntrustID(pRet->getEntrustID(), order_info->client_seq_id).c_str());
	pRet->setOrderID(fmt::format("{}", order_info.GetOrderId()).c_str());

	pRet->setStateMsg(order_info.GetOrderStatusInfo());

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

WTSOrderInfo* TraderATPQuant::makeOrderInfo(const ATPRspCashAuctionOrderMsg& orderField)  // 委托回调，结构体只提供cl_ord_no 客户订单编号
{
	const std::string code(orderField.GetSecurityId());
	const std::string exchg((orderField.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE");

	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);

	pRet->setPrice(orderField.GetPrice());
	pRet->setVolume(orderField.GetOrderQty());
	pRet->setDirection(wrapATPSide(orderField.GetSide()));
	pRet->setPriceType(wrapOrderType(orderField.GetOrderType()));
	pRet->setOrderFlag(WOF_NOR);

	pRet->setCode(code.c_str());
	pRet->setExchange(exchg.c_str());

	pRet->setOrderDate((uint32_t)(orderField.GetTransactTime() / 1000000000));
	uint32_t uTime = orderField.GetTransactTime() % 1000000000;
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	genEntrustID(pRet->getEntrustID(), orderField.GetClOrdNo());  // client_seq_id
	fmtutil::format_to(pRet->getOrderID(), "{}", orderField.GetClOrdNo());  // cl_ord_no

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

WTSOrderInfo* TraderATPQuant::makeOrderInfo(const ATPRtnCashAuctionOrderMsg& orderField)
{
	const std::string code(orderField.GetSecurityId());
	const std::string exchg((orderField.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE");

	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);

	pRet->setPrice(orderField.GetPrice());
	pRet->setVolume(orderField.GetOrderQty());
	pRet->setDirection(wrapATPSide(orderField.GetSide()));
	pRet->setPriceType(WPT_LIMITPRICE);
	pRet->setOrderFlag(WOF_NOR);

	pRet->setVolTraded(orderField.GetOrderQty() - orderField.GetLeavesQty());
	pRet->setVolLeft(orderField.GetLeavesQty());

	pRet->setCode(code.c_str());
	pRet->setExchange(exchg.c_str());

	pRet->setOrderDate((uint32_t)(orderField.GetTransactTime() / 1000000000));
	uint32_t uTime = orderField.GetTransactTime() % 1000000000;
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrderStatus(orderField.GetOrdStatus()));
	if (orderField.GetOrdStatus() == ATPOrdStatusConst::kReject || orderField.GetOrdStatus() == ATPOrdStatusConst::kUnSend) {
		pRet->setError(true);
		//pRet->setOrderState(WOS_Nottouched);  // 未触发
	}

	genEntrustID(pRet->getEntrustID(), orderField.GetClOrdNo());  // 客户订单编号
	fmtutil::format_to(pRet->getOrderID(), "{}", orderField.GetOrderId());  // 交易所订单编号

	pRet->setStateMsg(orderField.GetOrdRejReason());

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

WTSTradeInfo* TraderATPQuant::makeTradeInfo(const ATPRspCashTradeOrderQueryResultMsg& trade_info)
{
	const char* code = trade_info.GetSecurityId();
	const char* exchg = (trade_info.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

	WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code, exchg);
	pRet->setVolume(trade_info.GetLastQty());
	pRet->setPrice(trade_info.GetLastPx());
	pRet->setTradeID(trade_info.GetExecId());
	pRet->setAmount(trade_info.GetTotalValueTraded());
	pRet->setContractInfo(contract);

	uint32_t uTime = (uint32_t)(trade_info.GetTransactTime() % 1000000000);
	uint32_t uDate = (uint32_t)(trade_info.GetTransactTime() / 1000000000);

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime));

	WTSDirectionType dType = wrapATPSide(trade_info.GetSide());
	pRet->setDirection(dType);

	pRet->setOrderType(WORT_Normal);

	pRet->setRefOrder(fmt::format("{}", trade_info.GetClOrdNo()).c_str());  // order_id
	pRet->setTradeType(WTT_Common);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

WTSTradeInfo* TraderATPQuant::makeTradeRecord(const ATPRtnCashAuctionOrderMsg& msg)
{
	const char* code = msg.GetSecurityId();
	const char* exchg = (msg.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

	WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code, exchg);
	pRet->setVolume((uint32_t)msg.GetLastQty());  // 成交数量
	pRet->setPrice(msg.GetLastPx());  // 成交价格
	pRet->setAmount(msg.GetTotalValueTraded());  //成交金额
	pRet->setTradeID(msg.GetExecId());

	pRet->setContractInfo(contract);

	uint32_t uTime = (uint32_t)(msg.GetTransactTime() % 1000000000);
	uint32_t uDate = (uint32_t)(msg.GetTransactTime() / 1000000000);

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime));

	WTSDirectionType dType = wrapATPSide(msg.GetSide());
	pRet->setDirection(dType);
	//pRet->setOffsetType(wrapOffsetType(cash_auction_trade_er->side));
	fmtutil::format_to(pRet->getRefOrder(), "{}", msg.GetClOrdNo());
	pRet->setTradeType(WTT_Common);

	if (msg.GetOrdStatus() == ATPOrdStatusConst::kInternalCanceledOrder || msg.GetOrdStatus() == ATPOrdStatusConst::kReject)
		pRet->setOrderType(WORT_Exception);
	else
		pRet->setOrderType(WORT_Normal);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);
	/*else
	{
		const char* usertag1 = m_eidCache.get(pRet->getEntrustID());
		if (strlen(usertag1) == 0)
		{
			pRet->setUserTag(pRet->getEntrustID());
		}
		else
		{
			pRet->setUserTag(usertag1);

			if (strlen(pRet->getOrderID()) > 0)
			{
				m_oidCache.put(StrUtil::trim(pRet->getOrderID()).c_str(), usertag1, 0, [this](const char* message) {
					write_log(_sink, LL_ERROR, message);
				});
			}
		}
	}*/

	return pRet;
}

/**
 * @brief 登录成功
 */
//void TraderATPQuant::OnLogin(const std::string& reason)
//{
//	if (_sink)
//		_sink->handleEvent(WTE_Connect, 0);
//
//	_state = TS_LOGINED;  // 已登录
//	write_log(_sink, LL_WARN, "[TraderATPQuant] {} login success: {}...", _user, reason);
//}

/**
 * @brief 登出成功
 */
void TraderATPQuant::OnLogout(const char* desc)
{
	if (_sink)
		_sink->handleEvent(WTE_Logout, 0);

	_state = TS_NOTLOGIN;
	write_log(_sink, LL_WARN, "[TraderATPQuant] {} {}.", _user, desc);
}

/**
 * @brief 自动重连
 */
void TraderATPQuant::OnRecovering(const char* desc)
{
	write_log(_sink, LL_WARN, "[TraderATPQuant] Failed to connect to {}, msg: {}. Recovering to connect ...", _front, desc);
}

// 现货通用下单回调
void TraderATPQuant::OnRspCashAuctionOrder(const ATPRspCashAuctionOrderMsg& msg, const ATPRspErrorInfo& error_info, const int64_t request_id)
{
	if (error_info.error_id != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderATPQuant][OnRspCashAuctionOrder] Insert order failed, msg: {}, code: {}", error_info.GetErrorMsg(), error_info.error_id);

		WTSEntrust* entrust = makeEntrust(msg);

		WTSError* error = WTSError::create(WEC_ORDERINSERT, error_info.GetErrorMsg());

		_asyncio.post([this, entrust, error] {
			if (_sink)
				_sink->onRspEntrust(entrust, error);

			error->release();
			entrust->release();
		});
	}
	else
	{
		/*write_log(_sink, LL_DEBUG, "[TraderATPQuant][OnRspCashAuctionOrder] time: {}, code: {}, ord_type: {}, price: {}, order_qty: {}, cl_ord_no: {}, side: {}, business_type: {}",
			msg.GetTransactTime(), msg.GetSecurityId(), msg.GetOrderType(), msg.GetPrice(), msg.GetOrderQty(), msg.GetClOrdNo(), msg.GetSide(), msg.GetBusinessType());*/

		WTSOrderInfo *orderInfo = makeOrderInfo(msg);
		if (orderInfo)
		{
			if (_sink)
				_sink->onPushOrder(orderInfo);

			orderInfo->release();

			//_asyncio.post([this, orderInfo] {
			//	if (_sink)
			//		_sink->onPushOrder(orderInfo);

			//	orderInfo->release();
			//});
		}
	}
}

void TraderATPQuant::OnRtnCashAuctionOrder(const ATPRtnCashAuctionOrderMsg& msg)
{
	if (msg.GetOrdSign() == ATPOrderSignConst::kCancel)  // 撤单
	{
		if (msg.GetRejectReasonCode() != ATPErrorCode::kSuccess)  // 撤单失败
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant][OnRtnCashAuctionOrder] [{}] cancel [{}|{}] fail, reason: {}", msg.GetAccountId(), msg.GetSecurityId(), msg.GetOrderId(), msg.GetOrdRejReason());

			WTSEntrust* entrust = makeEntrust(msg);

			WTSError* error = WTSError::create(WEC_ORDERCANCEL, msg.GetOrdRejReason());
			if (_sink) _sink->onRspEntrust(entrust, error);
			
			error->release();
			entrust->release();
		}
		else
		{
			write_log(_sink, LL_DEBUG, "[TraderATPQuant][OnRtnCashAuctionOrder] [{}] cancel [{}|{}] sucess", msg.GetAccountId(), msg.GetSecurityId(), msg.GetOrderId());
			//write_log(_sink, LL_DEBUG, "[OnRtnCashAuctionOrder] code: {}, orderid: {}, price: {}, volume: {}, trd_price: {}, trd_volume: {}, cl_ord_id: {}", msg.GetSecurityId(), msg.GetOrderId(), msg.GetPrice(), msg.GetOrderQty(), msg.GetLastPx(), msg.GetLastQty(), msg.GetClOrdId());

			// 这里可能需要同时触发onPushOrder
			WTSTradeInfo *tRecord = makeTradeRecord(msg);
			WTSOrderInfo *oRecord = makeOrderInfo(msg);
			if (oRecord)
			{
				_asyncio.post([this, oRecord] {
					if (_sink)
						_sink->onPushOrder(oRecord);

					oRecord->release();
				});
			}

			if (tRecord)
			{
				_asyncio.post([this, tRecord] {
					if (_sink)
						_sink->onPushTrade(tRecord);

					tRecord->release();
				});
			}
		}
	} 
	else  // 委托
	{
		if (msg.GetRejectReasonCode() != ATPErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant] [{}] insert [{}|{}] fail, reason: [{}]", msg.GetAccountId(), msg.GetSecurityId(), msg.GetOrderId(), msg.GetOrdRejReason());

			WTSEntrust* entrust = makeEntrust(msg);

			WTSError* error = WTSError::create(WEC_ORDERINSERT, msg.GetOrdRejReason());
			if (_sink) _sink->onRspEntrust(entrust, error);
			
			error->release();
			entrust->release();
		}
		else
		{
			write_log(_sink, LL_DEBUG, "[OnRtnCashAuctionOrder] code: {}, orderid: {}, price: {}, volume: {}, trd_price: {}, trd_volume: {}, cl_ord_id: {}", msg.GetSecurityId(), msg.GetOrderId(), msg.GetPrice(), msg.GetOrderQty(), msg.GetLastPx(), msg.GetLastQty(), msg.GetClOrdId());

			WTSTradeInfo *tRecord = makeTradeRecord(msg);
			WTSOrderInfo *oRecord = makeOrderInfo(msg);
			if (oRecord)
			{
				_asyncio.post([this, oRecord] {
					if (_sink)
						_sink->onPushOrder(oRecord);

					oRecord->release();
				});
			}

			if (tRecord)
			{
				_asyncio.post([this, tRecord] {
					if (_sink)
						_sink->onPushTrade(tRecord);

					tRecord->release();
				});

				//if (_sink)
				//	_sink->onPushTrade(tRecord);

				//tRecord->release();
			}
		}
	}
}

void TraderATPQuant::OnRspCashCancelOrder(const ATPRspCashCancelOrderMsg& msg, const ATPRspErrorInfo& error_info, const int64_t request_id)
{
	if (error_info.error_id != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderATPQuant][{}] Cancel {} fail, msg: {}", msg.GetAccountId(), msg.GetClOrdNo(), error_info.GetErrorMsg());

		//WTSEntrust* entrust = makeEntrust(msg);

		//WTSError* error = WTSError::create(WEC_ORDERCANCEL, error_info.GetErrorMsg());

		//_asyncio.post([this, entrust, error] {
		//	if (_sink)
		//		_sink->onRspEntrust(entrust, error);

		//	error->release();
		//	entrust->release();
		//});
	}
	else
	{
		write_log(_sink, LL_INFO, "[TraderATPQuant][{}] Cancel {} success", msg.GetAccountId(), msg.GetClOrdNo());
	}
}

void TraderATPQuant::OnRspCashOrderQueryResult(const ATPRspCashOrderQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast)
{
	//if (error_info.error_id != 0)
	//	write_log(_sink, LL_INFO, "[TraderATPQuant][OnRspCashOrderQueryResult] Err msg: {}, Err Code: {}", error_info.GetErrorMsg(), error_info.error_id);

	if (ayOrders == NULL)
		ayOrders = WTSArray::create();

	if (error_info.error_id == 0)
	{
		WTSOrderInfo* ordInfo = makeOrderInfo(msg);
		if (ordInfo != NULL)
			ayOrders->append(ordInfo, false);

		//write_log(_sink, LL_DEBUG, "[OnRspCashOrderQueryResult] acct: {}, code: {}, orderid: {}, price: {}, vol: {}, vol_left: {}, vol_trd: {}, status_info: {}, status: {}, cl_ord_no: {}, cl_ord_id: {}, orig_cl_ord_no: {}",
			//msg.GetAccountId(), msg.GetSecurityId(), msg.GetOrderPrice(), msg.GetOrderId(), msg.GetOrderQty(), msg.GetLeavesQty(), msg.GetCumQty(), msg.GetOrderStatusInfo(), msg.GetOrdStatus(), msg.GetClOrdNo(), msg.GetClOrdId(), msg.GetOrigClOrdNo());
	}

	if (isLast)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspOrders(ayOrders);

			if (ayOrders)
				ayOrders->clear();
		});
	}
}

// 持仓查询
void TraderATPQuant::OnRspCashShareQueryResult(const ATPRspCashShareQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast)
{
	if (error_info.error_id != 0)
		write_log(_sink, LL_ERROR, "[TraderATPQuant][OnRspCashShareQueryResult] Err msg: {}, Err Code: {}", error_info.GetErrorMsg(), error_info.error_id);

	if (NULL == _positions)
		_positions = PositionMap::create();

	if (error_info.error_id == 0)
	{
		const std::string exchg = (msg.GetMarketId() == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

		WTSContractInfo* contract = _bd_mgr->getContract(msg.GetSecurityId(), exchg.c_str());
		if (contract)
		{
			WTSCommodityInfo* commInfo = contract->getCommInfo();
			std::string key = fmt::format("{}-{}", msg.GetSecurityId(), exchg.c_str());
			WTSPositionItem* pos = (WTSPositionItem*)_positions->get(key);
			if (pos == NULL)
			{
				pos = WTSPositionItem::create(msg.GetSecurityId(), commInfo->getCurrency(), commInfo->getExchg());
				pos->setContractInfo(contract);
				_positions->add(key, pos, false);
			}

			pos->setDirection(WDT_LONG);

			double new_pos = msg.GetLeavesQty() - msg.GetInitQty();  // 剩余股份数量N15(2)，包含当日买入部分，拆分合并，申购赎回, 日初仓位
			new_pos = (new_pos >= 0) ? new_pos : 0;
			pos->setNewPosition(new_pos);  // 今日新开仓
			pos->setPrePosition(msg.GetInitQty());  // 日初持仓量N15(2)

			pos->setDynProfit(msg.GetProfitLoss());
			pos->setPositionCost(msg.GetCostPrice() * msg.GetLeavesQty());
			pos->setAvailNewPos(0);  // 可用股份数量N15(2)，股票是T+1，所以今可用一直是0

			pos->setAvgPrice(msg.GetCostPrice());
			pos->setAvailPrePos(msg.GetAvailableQty());  // 股票可用只能是昨可用

			//write_log(_sink, LL_DEBUG, "[{}][Position] {}.{}, pre_qty: {}, cur_qty: {}, avail_qty: {}, profit_loss: {}, buy: {}, sale: {}",
				//msg.GetAccountId(), exchg, msg.GetSecurityId(), msg.GetInitQty(), msg.GetLeavesQty(), msg.GetAvailableQty(), msg.GetProfitLoss(), msg.GetStockBuy(), msg.GetStockSale());
		}
	}

	if (isLast)
	{
		_asyncio.post([this] {
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

			if (ayPos)
				ayPos->clear();
		});
	}
}

void TraderATPQuant::OnRspCashTradeOrderQueryResult(const ATPRspCashTradeOrderQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast)
{
	if (ayTrades == NULL)
		ayTrades = WTSArray::create();

	if (error_info.error_id == 0)
	{
		WTSTradeInfo* trdInfo = makeTradeInfo(msg);
		if (trdInfo != NULL)
			ayTrades->append(trdInfo, false);

		//write_log(_sink, LL_DEBUG, "[OnRspCashTradeOrderQueryResult] acct: {}, code: {}, price: {}, vol: {}, orderid: {}, cl_ord_no: {}, cl_ord_id: {}",
			//msg.GetAccountId(), msg.GetSecurityId(), msg.GetLastPx(), msg.GetTotalValueTraded(), msg.GetOrderId(), msg.GetClOrdNo(), msg.GetClOrdId());
	}
	else
	{
		write_log(_sink, LL_ERROR, "[OnRspCashTradeOrderQueryResult] msg: {}, code: {}", error_info.GetErrorMsg(), error_info.error_id);
	}

	if (isLast)
	{
		_asyncio.post([this] {
				if (_sink)
					_sink->onRspTrades(ayTrades);

				if (ayTrades)
					ayTrades->clear();
		});
	}
}

// 现货成交汇总查询
//void TraderATPQuant::OnRspCashTradeCollectQueryResult(const ATPRspCashCollectQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast)
//{
//
//}

// 现货资金查询结果
void TraderATPQuant::OnRspCashFundQueryResult(const ATPRspCashFundQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast)
{
	if (ayFunds == NULL)
		ayFunds = WTSArray::create();
	
	if (error_info.error_id == 0)
	{
		//write_log(_sink, LL_DEBUG, "[TraderATPQuant] [{}] Leaves value: {}, Init_leaves_value: {}, available_value: {}", msg.GetAccountId(), msg.GetLeavesValue(), msg.GetInitLeavesValue(), msg.GetAvailableT1());

		WTSAccountInfo* fundInfo = WTSAccountInfo::create();
		
		fundInfo->setPreBalance(msg.GetInitLeavesValue());
		fundInfo->setBalance(msg.GetLeavesValue());
		fundInfo->setAvailable(msg.GetAvailableT1());
		fundInfo->setMargin(0);
		fundInfo->setDynProfit(0);
		fundInfo->setFrozenMargin(msg.GetFrozenAll());
		fundInfo->setCloseProfit(0);

		ayFunds->append(fundInfo, false);
	}

	if (isLast)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspAccount(ayFunds);

			if (ayFunds)
				ayFunds->clear();
		});
	}
}

/**
 * @brief 现货资产查询结果
 */
void TraderATPQuant::OnRspCashAssetQueryResult(const ATPRspCashAssetQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast)
{
	//if (error_info.error_id != 0)
	//	write_log(_sink, LL_ERROR, "[TraderATPQuant][OnRspCashAssetQueryResult] Err msg: {}, Err code: {}", error_info.GetErrorMsg(), error_info.error_id);

	if (ayFunds == NULL)
		ayFunds = WTSArray::create();

	if (error_info.error_id == 0)
	{
		//write_log(_sink, LL_DEBUG, "[TraderATPQuant] [{}] Leaves value: {}, Init_leaves_value: {}, available_value: {}", msg.GetAccountId(), msg.GetLeavesValue(), msg.GetInitLeavesValue(), msg.GetAvailableT1());

		WTSAccountInfo* accountInfo = WTSAccountInfo::create();
		accountInfo->setPreBalance(msg.GetInitLeavesValue());
		accountInfo->setCloseProfit(0);
		accountInfo->setDynProfit(0);
		accountInfo->setMargin(0);
		accountInfo->setAvailable(msg.GetAvailableT1());
		accountInfo->setCommission(0);
		accountInfo->setFrozenMargin(msg.GetFrozenAll());
		accountInfo->setFrozenCommission(0);
		accountInfo->setDeposit(0);
		accountInfo->setWithdraw(0);
		accountInfo->setBalance(msg.GetLeavesValue());
		accountInfo->setCurrency("CNY");
		
		ayFunds->append(accountInfo, false);
	}

	if (isLast)
	{
		_asyncio.post([this] {
				if (_sink)
					_sink->onRspAccount(ayFunds);

				if (ayFunds)
					ayFunds->clear();
		});
	}
}


#pragma region "ITraderApi"
bool TraderATPQuant::init(WTSVariant *params)
{
	_user = params->getCString("user");  
	_pass = params->getCString("pass");

	_agw_user = params->getCString("agw_user");
	_agw_pass = params->getCString("agw_pass");

	_loginmode = params->getInt32("login_mode");
	_encryptmode = params->getInt32("encrypt_mode");
	//_sh_acctid = params->getCString("sh_acct_id");
	//_sz_acctid = params->getCString("sz_acct_id");
	_acctid = params->getCString("acct_id");
	_is_acct_sh = params->getBoolean("b_acct_sh");
	_fund_accountid = params->getCString("fund_account_id");
	_branchid = params->getCString("branch_id");
	_order_way = params->getInt32("order_way");
	_client_code = params->getCString("client_code");
	_bind_ip = params->getCString("bind_ip");
	//_fund_pass = params->getCString("fund_pass");

	_front = params->getCString("front");

	std::string module = params->getCString("atpmodule");
	std::string dllpath;

	if (module.empty()) module = "atpquantapi";
	dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");
	write_log(_sink, LL_INFO, "Loading module: {}", dllpath);

	m_hInstATP = DLLHelper::load_library(dllpath.c_str());

	return true;
}

void TraderATPQuant::release()
{
	if (_api)
	{
		// 登出
		int32_t ec = _api->Logout();
		if (ec != ATPErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "[TraderAPTQuant] Logout fail: {}", ec);
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderAPTQuant] Logout success ");
		}

		// 释放资源
		ec = _api->Release();

		if (ec == ATPErrorCode::kSuccess)
		{
			write_log(_sink, LL_INFO, "[TraderATPQuant] API release success");
		}
		else
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant] API release fail, code: {}", ec);
		}

		ec = ATPQuantAPI::Stop();
		if (ec == ATPErrorCode::kSuccess)
		{
			write_log(_sink, LL_INFO, "[TraderATPQuant] API stop success");
		}
		else
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant] API stop fail, code: {}", ec);
		}

		delete _api;
		_api = NULL;
	}

	if (_orders)
		_orders->clear();

	if (_positions)
		_positions->clear();

	if (_trades)
		_trades->clear();

	if (ayOrders)
		ayOrders->release();

	if (ayTrades)
		ayTrades->release();

	if (ayFunds)
		ayFunds->release();
}

void TraderATPQuant::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderATPQuant::reconnect()
{
	if (_api)
	{
		// 释放资源
		int32_t ec = _api->Release();

		while (!_close_flag)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		if (ec == ATPErrorCode::kSuccess)
		{
			write_log(_sink, LL_INFO, "[TraderATPQuant] API release success");
		}
		else
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant] API release fail, code: {}", ec);
		}

		ec = ATPQuantAPI::Stop();
		if (ec == ATPErrorCode::kSuccess)
		{
			write_log(_sink, LL_INFO, "[TraderATPQuant] API stop success");
		}
		else
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant] API stop fail, code: {}", ec);
		}

		delete _api;
		_api = NULL;
	}

	static bool inited = false;
	if (!inited)
	{
		// 初始化API

		int32_t ec = ATPErrorCode::kSuccess;

		// 1、初始化
		ATPProperties* properties = new ATPProperties();

		properties->SetValue(prop::kLogLevel, ATPLogLevel::kDebug);  // 日志级别，非必填，默认值ATPLogLevel::kInfo
		properties->SetValue(prop::kCommonLogPath, "");                              // 业务日志路径，非必填，默认值空，表示当前可执行文件所在目录
		properties->SetValue(prop::kIndicatorLogPath, "");                           // 指标日志路径，非必填，默认值空，表示当前可执行文件所在目录
		properties->SetValue(prop::kIoLogPath, "");                                  // IO日志路径，非必填，默认值空，表示当前可执行文件所在目录
		properties->SetValue(prop::kEncryptSchema, (_encryptmode == 0) ? ATPEncryptModeConst::kTransMode : ATPEncryptModeConst::kEncryptMode); // 加密模式，非必填，默认值ATPEncryptModeConst::kTransMode          
		properties->SetValue(prop::kIsEnableLatency, false);                         // 是否启用时延统计功能，非必填，默认值false

		ec = ATPQuantAPI::Init(properties);

		if (ec != ATPErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "[TraderQuantATP] ATPQuantAPI init failed: {}", ec);
			return;
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderQuantATP] ATPQuantAPI init success");
		}

		//2、构造
		properties->SetValue(prop::kCallbackResourceMode, ATPCallbackResourceModeConst::kLowLatencyMode);   // 回调线程资源模式，非必填，默认值ATPCallbackResourceModeConst::kLowLatencyMode
		properties->SetValue(prop::kCallbackThreadMode, ATPThreadModelConst::kShared);                      // 回调线程模型，非必填，默认值ATPThreadModelConst::kShared-共享
		properties->SetValue(prop::kGroupId, 0);                                                            // 回调线程分组ID，非必填，默认值0，仅资源模式为kCustomMode（自定义模式）时需要设置
		properties->SetValue(prop::kMinResidentMicro, 1);                                                   // 回调线程休眠时间，单位μs，非必填，默认值1μs，仅资源模式为kCustomMode（自定义模式）时需要设置
		properties->SetValue(prop::kRecevieThreadCpu, 255);                                                 // 回调线程绑定CPU，非必填，默认值0xFF，表示不绑定
		properties->SetValue(prop::kIsTcpDirect, false);                                                    // 是否启用TCPDirect，非必填，默认值false
		properties->SetValue(prop::kBindIpAddress, _bind_ip.c_str());                                       // 绑定本地网卡地址，非必填，默认值空，表示不绑定
		properties->SetValue(prop::kLocations, _front.c_str());                                             // 网关地址，格式为"ip:port;ip:port"，必填
		properties->SetValue(prop::kAgwUser, _agw_user.c_str());                                                           // 网关用户名，非必填，默认值空，表示匿名登录模式
		properties->SetValue(prop::kAgwPassword, _agw_pass.c_str());                                                       // 网关用户密码，非必填，默认值空
		properties->SetValue(prop::kRetransmitMode, ATPRetransmitModeConst::kQuickMode);                    // 重传模式，非必填，默认值ATPRetransmitModeConst::kQuickMode（ATP3.2.1.1版本开始支持）
		properties->SetValue(prop::kMultiChannelFlag, ATPMultiChannelFlagConst::kDefault);                  // 多通道自主订阅标志，非必填，默认值0xFF，表示以后台开关为准（ATP3.2.0版本开始支持）
		properties->SetValue(prop::kConnectionProtocol, ATPConnectionProtocolConst::kTCPProtocol);          // 协议类型，非必填，默认值ATPConnectionProtocolConst::kTCPProtocol
		properties->SetValue(prop::kOrderWay, _order_way);                                                   // 委托方式，必填
		properties->SetValue(prop::kClientFeatureCode, _client_code.c_str());

		_api = new ATPQuantAPI(this, properties);

		inited = true;

		write_log(_sink, LL_INFO, "[TraderATPQuant] Success to init api, api version: {}", _api->GetVersion());
	}

	//_asyncio.post([this]() {
	//	if (_sink)
	//		_sink->handleEvent(WTE_Connect, 0);
	//});
}

void TraderATPQuant::connect()
{
	reconnect();

	if (_thrd_worker == NULL)
	{
		static boost::asio::io_service::work work(_asyncio);
		_thrd_worker.reset(new StdThread([this]() {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				_asyncio.run_one();
				//m_asyncIO.run();
			}
		}));
	}

	_asyncio.post([this]() {
		if (_sink)
		{
			_sink->handleEvent(WTE_Connect, 0);
		}
	});
}

void TraderATPQuant::disconnect()
{
	release();
}

bool TraderATPQuant::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderATPQuant::genEntrustID(char* buffer, uint32_t orderRef)
{
	//这里不再使用sessionid，因为每次登陆会不同，如果使用的话，可能会造成不唯一的情况
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderATPQuant::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderATPQuant::makeEntrustID(char* buffer, int length)
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

void TraderATPQuant::doLogin()
{
	_state = TS_LOGINING;

	{
		//3、登录
		ATPLoginProperty* login_pro = ATPLoginProperty::NewMessage();
		login_pro->SetUserId(_user.c_str());  // 客户号
		login_pro->SetPassword(_pass.c_str());  // 客户号密码
		login_pro->SetLoginMode(_loginmode);  // 客户号登录模式 kCustIDMode = 1;  kFundAccountIDMode = 2
		login_pro->SetBranchId(_branchid.c_str());

		int32_t ec = _api->Login(login_pro);
		if (ATPErrorCode::kSuccess == ec)
		{
			write_log(_sink, LL_INFO, "[TraderATPQuant] Login success");
		}
		else
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant] Login fail, code: {}", ec);
		}

		//获取客户信息并打印
		while (true)
		{
			ATPCustomerInfo* customer_info = _api->GetCustomerInfo();
			if (customer_info)
			{
				//if (_sink)
				//	_sink->handleEvent(WTE_Connect, 0);

				_state = TS_LOGINED;  // 已登录
				_tradingday = TimeUtils::getCurDate();

				{
					//初始化委托单缓存器
					std::stringstream ss;
					ss << "./atpquantdata/local/";
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
					ss << "./atpquantdata/local/";
					std::string path = StrUtil::standardisePath(ss.str());
					if (!StdFile::exists(path.c_str()))
						boost::filesystem::create_directories(path.c_str());
					ss << _user << "_oid.sc";
					m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
						write_log(_sink, LL_WARN, message);
					});
				}

				for (uint32_t i = 0; i < customer_info->FundAccountArraySize(); i++)
				{
					for (uint32_t j = 0; j < customer_info->FundAccountArray_AccountArraySize(i); j++)
					{
						_cust_id = customer_info->FundAccountArray_AccountArray_GetAccountId(i, j);
						write_log(_sink, LL_INFO, "[TraderATPQuant] [{}][{}] Login success on {}.", _cust_id, customer_info->FundAccountArray_AccountArray_GetMarketId(i, j), _tradingday);
					}
				}

				_asyncio.post([this]
				{
					if (_sink)
						_sink->onLoginResult(true, "", _tradingday);
					_state = TS_ALLREADY;
				});

				break;
			}
			else
			{
				if (_sink)
					_sink->onLoginResult(false, fmt::format("{}", ec).c_str(), _tradingday);
				_state = TS_NOTLOGIN;

				//write_log(_sink, LL_ERROR, "[TraderATPQuant] CustLogin Fail, permisson_error_code : {}", static_cast<uint32_t>(cust_login_resp.permisson_error_code));
			}
		}

		//获取连接信息并打印
		while (true)
		{
			ATPConnectionInfo* connection_info = _api->GetConnectionInfo();
			if (connection_info)
			{
				write_log(_sink, LL_INFO, "[TraderATPQuant] Instance ID: {}, Multi_channel_result: {}", connection_info->GetInstanceId(), connection_info->GetMultiChannelResult());
				break;
			}
			else
			{
				if (_sink)
					_sink->handleEvent(WTE_Connect, -1);

				_state = TS_LOGINFAILED;
				write_log(_sink, LL_WARN, "[TraderATPQuant] [{}] Connect fail, code: {}", connection_info->GetInstanceId(), ec);
			}
		}
	}//);
}

int TraderATPQuant::login(const char* user, const char* pass, const char* productInfo)
{
	_user = user;
	_pass = pass;
	_product = productInfo;

	if (_api == NULL)
	{
		return -1;
	}

	doLogin();

	return 0;
}

int TraderATPQuant::logout()
{
	if (_api == NULL)
		return -1;

	// 登出
	int32_t ec = _api->Logout();
	if (ec != ATPErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderAPT] Logout fail, code: {}", ec);
		return ec;
	}

	return 0;
}

int TraderATPQuant::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	uint32_t orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);

	bool isSh = strcmp(entrust->getExchg(), "SSE") == 0;
	ATPMarketIDType market = isSh ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;

	//构造现货交易委托消息类(现货集中竞价)，并赋值
	ATPReqCashAuctionOrderMsg* cash_msg = ATPReqCashAuctionOrderMsg::NewMessage(ATPBusinessTypeConst::kCashAuction);
	cash_msg->SetCustId(_user.c_str());
	cash_msg->SetFundAccountId(_fund_accountid.c_str());
	cash_msg->SetBranchId(_branchid.c_str());
	cash_msg->SetAccountId(_acctid.c_str());
	cash_msg->SetSecurityId(entrust->getCode());
	cash_msg->SetMarketId(market);
	cash_msg->SetSide(ATPSideConst::kBuy);
	cash_msg->SetOrderQty(entrust->getVolume());
	cash_msg->SetPrice(entrust->getPrice());
	cash_msg->SetOrderType(ATPOrderTypeConst::kFixedNew);
	cash_msg->SetPassword(_pass.c_str());
	cash_msg->SetBatchClOrdNo(orderref);

	{
		//write_log(_sink, LL_DEBUG, "code: {}, acct: {}, side: {}, price: {}, qty: {}", cash_msg->GetSecurityId(), cash_msg->GetAccountId(), cash_msg->GetSide(), cash_msg->GetPrice(), cash_msg->GetOrderQty());

		if (strlen(entrust->getUserTag()) > 0)
		{
			m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
				write_log(_sink, LL_WARN, message);
			});
		}

		//调用现货交易委托消息接口
		int32_t ec = _api->ReqCashAuctionOrder(cash_msg, genRequestID());
		if (ATPErrorCode::kSuccess == ec)
		{
			write_log(_sink, LL_DEBUG, "[TraderATPQuant] [{}] Order insert [{}] success", cash_msg->GetAccountId(), entrust->getCode());
		}
		else
		{
			write_log(_sink, LL_ERROR, "[TraderATPQuant] [{}] Order insert [{}] fail, code: {}", cash_msg->GetAccountId(), entrust->getCode(), ec);
		}
	}

	//释放资源
	ATPReqCashAuctionOrderMsg::DeleteMessage(cash_msg);

	return 0;
}

int TraderATPQuant::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	bool isSH = strcmp(action->getExchg(), "SSE") == 0;
	ATPMarketIDType market = isSH ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;

	//构造现货通用撤单消息类，并赋值
	ATPReqCashCancelOrderMsg* cancel_order_msg = ATPReqCashCancelOrderMsg::NewMessage();
	cancel_order_msg->SetCustId(_user.c_str());
	cancel_order_msg->SetFundAccountId(_fund_accountid.c_str());
	cancel_order_msg->SetOrigClOrdNo(strtoull(action->getOrderID(), NULL, 10));
	cancel_order_msg->SetPassword(_pass.c_str());
	cancel_order_msg->SetBranchId(_branchid.c_str());
	cancel_order_msg->SetAccountId(_acctid.c_str());
	//////////////////////////////////////////////////////////////////////////
	// 客户自定义委托批号，指示被撤消订单的batch_cl_ord_no。batch_cl_ord_noorig_cl_ord_no项。
	// 若orig_cl_ord_no不为0，则按orig_cl_ord_no撤单；否则，batch_cl_ord_no不为0，按batch_cl_ord_no撤单
	//cancel_order_msg->SetBatchClOrdNo(1); 
	cancel_order_msg->SetMarketId(market);

	//调用现货通用撤单消息接口
	int32_t ec = _api->ReqCashCancelOrder(cancel_order_msg, genRequestID());
	if (ATPErrorCode::kSuccess == ec)
	{
		write_log(_sink, LL_INFO, "[TraderATPQuant] [{}] Cancel [{}] success", cancel_order_msg->GetAccountId(), action->getOrderID());
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderATPQuant] [{}] Cancel [{}] fail, code: {}", cancel_order_msg->GetAccountId(), action->getOrderID(), ec);
	}

	//释放资源
	ATPReqCashCancelOrderMsg::DeleteMessage(cancel_order_msg);

	return 0;
}

uint32_t TraderATPQuant::genRequestID()
{
	return _reqid.fetch_add(1) + 1;
}

int TraderATPQuant::queryAccount()
{
	 // 构造现货资金查询消息类，并赋值
	ATPReqCashAssetQueryMsg* asset_query_msg = ATPReqCashAssetQueryMsg::NewMessage();
	asset_query_msg->SetCustId(_user.c_str());
	asset_query_msg->SetFundAccountId(_fund_accountid.c_str());
	asset_query_msg->SetBranchId(_branchid.c_str());
	asset_query_msg->SetAccountId(_acctid.c_str());
	asset_query_msg->SetPassword(_pass.c_str());
	asset_query_msg->SetMarketId((_is_acct_sh) ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen);

	//调用现货资产查询消息接口
	int32_t ec = _api->ReqCashAssetQuery(asset_query_msg, genRequestID());
	if (ATPErrorCode::kSuccess == ec)
	{
		write_log(_sink, LL_INFO, "[TraderATPQuant] [{}] Query account success", _acctid);
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderATPQuant] [{}] Query account fail, code: {}", _acctid, ec);
	}

	//释放资源
	ATPReqCashAssetQueryMsg::DeleteMessage(asset_query_msg);

	return 0;
}

int TraderATPQuant::queryPositions()
{
	//构造现货股份查询消息类，并赋值
	ATPReqCashShareQueryMsg* share_query_msg = ATPReqCashShareQueryMsg::NewMessage();
	share_query_msg->SetCustId(_user.c_str());
	share_query_msg->SetFundAccountId(_fund_accountid.c_str());
	share_query_msg->SetPassword(_pass.c_str());
	share_query_msg->SetReturnNum(0);  // 0=按能返回的最大数量返回
	share_query_msg->SetSecurityId("");  // 空字符串表示查所有
	share_query_msg->SetMarketId(0);  // 0表示查所有，北交所开通后必填
	share_query_msg->SetAccountId(_acctid.c_str());  

	//调用现货股份查询消息接口
	int32_t ec = _api->ReqCashShareQuery(share_query_msg, genRequestID());
	if (ATPErrorCode::kSuccess == ec)
	{
		write_log(_sink, LL_INFO, "[TraderATPQuant] [{}] Query positions success", _acctid);
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderATPQuant] [{}] Query positions fail, code: {}", _acctid, ec);
	}

	//释放资源
	ATPReqCashShareQueryMsg::DeleteMessage(share_query_msg);

	return 0;
}

int TraderATPQuant::queryOrders()
{
	//构造现货订单查询消息类，并赋值
	ATPReqCashOrderQueryMsg* order_query_msg = ATPReqCashOrderQueryMsg::NewMessage();
	order_query_msg->SetCustId(_user.c_str());
	order_query_msg->SetFundAccountId(_fund_accountid.c_str());
	order_query_msg->SetBranchId(_branchid.c_str());
	order_query_msg->SetPassword(_pass.c_str());
	order_query_msg->SetAccountId(_acctid.c_str()); 
	order_query_msg->SetReturnNum(0);  // 0表示按能返回的最大數量返回
	order_query_msg->SetClOrdNo(0);  // 0表示查所有
	order_query_msg->SetMarketId(0);  // 0表示查所有
	order_query_msg->SetSecurityId("");  // 空字符串表示查所有
	order_query_msg->SetBusinessType(0);  // 0表示查所有
	order_query_msg->SetSide(ATPSideConst::kAll);  // '0'表示查所有
	order_query_msg->SetReturnSeq(ATPReturnSeqConst::kTimeOrderRe); // 返回顺序，按时间倒序返回
	//order_query_msg->SetBatchClOrdNo(1);

	//调用现货订单查询消息接口
	int32_t ec = _api->ReqCashOrderQuery(order_query_msg, genRequestID());
	if (ATPErrorCode::kSuccess == ec)
	{
		write_log(_sink, LL_INFO, "[TraderATPQuant] [{}] Query orders success", _acctid);
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderATPQuant] [{}] Query orders fail, code: {}", _acctid, ec);
	}

	//释放资源
	ATPReqCashOrderQueryMsg::DeleteMessage(order_query_msg);

	return 0;
}

int TraderATPQuant::queryTrades()
{
	//构造现货成交查询消息类，并赋值
	ATPReqCashTradeOrderQueryMsg* api_msg = ATPReqCashTradeOrderQueryMsg::NewMessage();
	api_msg->SetCustId(_user.c_str());
	api_msg->SetAccountId(_acctid.c_str());
	api_msg->SetFundAccountId(_fund_accountid.c_str());
	api_msg->SetPassword(_pass.c_str());
	api_msg->SetReturnNum(0);  // 返回数量
	api_msg->SetBusinessType(ATPBusinessTypeConst::kAll);  // 业务类型,默认所有
	api_msg->SetMarketId(0);  // ((_is_acct_sh) ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen);
	api_msg->SetSecurityId("");  // 证券代码
	api_msg->SetReturnSeq(ATPReturnSeqConst::kTimeOrderRe);  // 逆序
	api_msg->SetClOrdId("");  // 报盘合同号
	api_msg->SetClOrdNo(0);  // 客户订单编号，0表示查所有
	api_msg->SetExecId("");  // 执行编号
	api_msg->SetBatchClOrdNo(0);

	//调用现货成交查询消息接口
	int32_t ec = _api->ReqCashTradeOrderQuery(api_msg, genRequestID());  // 这个是返回成交明细，不动于ReqCashTraderCollectQuery接口
	if (ATPErrorCode::kSuccess == ec)
	{
		write_log(_sink, LL_INFO, "[TraderATPQuant] [{}] Query trades success", _acctid);
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderATPQuant] [{}] Query trades fail, code: {}", _acctid, ec);
	}

	//释放资源
	ATPReqCashTradeOrderQueryMsg::DeleteMessage(api_msg);

	return 0;
}
#pragma endregion "ITraderApi"
