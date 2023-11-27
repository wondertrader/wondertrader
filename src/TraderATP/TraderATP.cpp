/*!
 * \file TraderATP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#include "TraderATP.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSVersion.h"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>
#include <iostream>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/AtpTradeApi/x64/atptradeapi.lib")
#else
#pragma comment(lib, "../API/AtpTradeApi/x86/atptradeapi.lib")
#endif
#endif

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
		TraderATP *instance = new TraderATP();
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


TraderATP::TraderATP()
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
	, _return_nums(0)
{

}


TraderATP::~TraderATP()
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

inline WTSDirectionType wrapDirectionType(ATPSideType side, ATPPositionEffectType pe)
{
	if (ATPSideConst::kBuy == side) {
		if (pe == ATPPositionEffectConst::kOpen)
			return WDT_LONG;
		else
			return WDT_SHORT;
	}
	else {
		if (pe == ATPPositionEffectConst::kOpen)
			return WDT_SHORT;
		else
			return WDT_LONG;
	}
}

inline ATPSideType wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if (WDT_LONG == dirType)
		if (offsetType == WOT_OPEN)
			return ATPSideConst::kBuy;
		else
			return ATPSideConst::kSell;
	else
		if (offsetType == WOT_OPEN)
			return ATPSideConst::kSell;
		else
			return ATPSideConst::kBuy;
}

inline WTSOffsetType wrapOffsetType(const ATPPositionEffectType dirType)
{
	if (dirType == ATPPositionEffectConst::kOpen)
		return WOT_OPEN;
	else
		return WOT_CLOSE;
}

inline ATPPositionEffectType wrapOffsetType(WTSOffsetType offType)
{
	if (WOT_OPEN == offType)
		return ATPPositionEffectConst::kOpen;
	else if (WOT_CLOSE == offType)
		return ATPPositionEffectConst::kClose;
	else if (WOT_CLOSETODAY == offType)
		return ATPPositionEffectConst::kClose;
	else if (WOT_CLOSEYESTERDAY == offType)
		return ATPPositionEffectConst::kClose;
	else
		return ATPPositionEffectConst::kDefault;
}

inline ATPOrdTypeType wrapOrdType(WTSPriceType priceType, WTSOrderFlag flag)
{

	if (WPT_LIMITPRICE == priceType && flag == WOF_NOR)
		return ATPOrdTypeConst::kFixedNew;  // 限价委托、增强限价

	if (WPT_LIMITPRICE == priceType && flag == WOF_FOK)
		return ATPOrdTypeConst::kFixedFullDealOrCancel;  // 限价全额成交或撤销

	if (WPT_ANYPRICE == priceType && flag == WOF_NOR)
		return ATPOrdTypeConst::kMarketTransferFixed;  // 市价剩余转限价

	if (WPT_ANYPRICE == priceType && flag == WOF_FAK)
		return ATPOrdTypeConst::kImmediateDealTransferCancel;  // 立即成交剩余撤销

	if (WPT_ANYPRICE == priceType && flag == WOF_FOK)
		return ATPOrdTypeConst::kFullDealOrCancel;  // 全额成交或撤销

	return ATPOrdTypeConst::kDefault;
}

inline WTSPriceType wrapOrdType(ATPOrdTypeType ordType)
{
	if (ordType == ATPOrdTypeConst::kFixedNew || ordType == ATPOrdTypeConst::kSzBiddingFixed || ordType == ATPOrdTypeConst::kShBiddingFixed || ordType == ATPOrdTypeConst::kFixedFullDealOrCancel || ordType == ATPOrdTypeConst::kFixed)
		return WPT_LIMITPRICE;
	if (ordType == ATPOrdTypeConst::kImmediateDealTransferCancel || ordType == ATPOrdTypeConst::kFullDealOrCancel || ordType == ATPOrdTypeConst::kMarket || ordType == ATPOrdTypeConst::kMarketTransferFixed)
		return WPT_ANYPRICE;
	else if (ordType == ATPOrdTypeConst::kLocalOptimalNew || ordType == ATPOrdTypeConst::kOptimalFiveLevelFullDealTransferCancel || ordType == ATPOrdTypeConst::kOptimalFiveLevelImmediateDealTransferFixed || ordType == ATPOrdTypeConst::kCountPartyOptimalTransferFixed)
		return WPT_BESTPRICE;
	else
		return WPT_LASTPRICE;
}

inline ATPLoginModeType wrapLoginMode(int loginMode)
{
	ATPLoginModeType login_mode;
	switch (loginMode)
	{
	case 1:
		login_mode = ATPCustLoginModeType::kCustIDMode;  // 客户号登录
	case 2:
		login_mode = ATPCustLoginModeType::kFundAccountIDMode;  // 资金账户登录模式
	case 3:
		login_mode = ATPCustLoginModeType::kAccountIDMode;  // 证券账户登录
	default:
		break;
	}

	return login_mode;
}

inline WTSOrderState wrapOrdStatus(ATPOrdStatusType orderState)
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
		return WOS_Canceled;
	case ATPOrdStatusConst::kSended:
	case ATPOrdStatusConst::kNew:
	case ATPOrdStatusConst::kProcessed:
		return WOS_Submitting;
	default:
		return WOS_Nottouched;
	}
}

WTSEntrust* TraderATP::makeEntrust(const ATPRspOrderStatusAckMsg* order_info)
{
	const std::string code(order_info->security_id);
	const std::string exchg = (order_info->market_id == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		code.c_str(),
		(uint32_t)order_info->order_qty,
		order_info->price,
		ct->getExchg());
	pRet->setContractInfo(ct);
	pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
	pRet->setPriceType(wrapOrdType(order_info->order_type));
	pRet->setOffsetType(wrapOffsetType(order_info->position_effect));
	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), order_info->client_seq_id);

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSOrderInfo* TraderATP::makeOrderInfo(const APIOrderUnit* order_info)
{
	const std::string code(order_info->security_id);
	const std::string exchg = (order_info->market_id == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(order_info->order_price / 10000);
	pRet->setVolume((uint32_t)order_info->order_qty / 100);
	pRet->setDirection(wrapATPSide(order_info->side));
	pRet->setPriceType(wrapOrdType(order_info->ord_type));
	pRet->setOrderFlag(WOF_NOR);
	//pRet->setOffsetType(wrapOffsetType(order_info->p));

	pRet->setVolTraded((uint32_t)order_info->cum_qty / 100);
	pRet->setVolLeft((uint32_t)order_info->leaves_qty / 100);

	pRet->setCode(code.c_str());
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate((uint32_t)(order_info->transact_time / 1000000000));
	uint32_t uTime = order_info->transact_time % 1000000000;
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrdStatus(order_info->ord_status));
	if (order_info->ord_status == ATPOrdStatusConst::kReject)
		pRet->setError(true);

	genEntrustID(pRet->getEntrustID(), order_info->client_seq_id);
	//pRet->setEntrustID(genEntrustID(pRet->getEntrustID(), order_info->client_seq_id).c_str());
	pRet->setOrderID(fmt::format("{}", order_info->cl_ord_no).c_str());

	pRet->setStateMsg(order_info->ord_rej_reason);

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

WTSOrderInfo* TraderATP::makeOrderInfo(const ATPRspOrderStatusAckMsg *order_status_ack)
{
	const std::string code(order_status_ack->security_id);
	const std::string exchg = (order_status_ack->market_id == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(order_status_ack->price);
	pRet->setVolume((uint32_t)order_status_ack->order_qty / 100);
	pRet->setDirection(wrapATPSide(order_status_ack->side));
	pRet->setPriceType(wrapOrdType(order_status_ack->order_type));
	pRet->setOrderFlag(WOF_NOR);
	pRet->setOffsetType(wrapOffsetType(order_status_ack->position_effect));

	pRet->setVolTraded((uint32_t)(order_status_ack->order_qty / 100 - order_status_ack->leaves_qty / 100));
	pRet->setVolLeft((uint32_t)order_status_ack->leaves_qty / 100);

	pRet->setCode(code.c_str());
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate((uint32_t)(order_status_ack->transact_time / 1000000000));
	uint32_t uTime = order_status_ack->transact_time % 1000000000;
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrdStatus(order_status_ack->ord_status));
	if (order_status_ack->ord_status == ATPOrdStatusConst::kReject) {
		pRet->setError(true);
		//pRet->setOrderState(WOS_Canceled);
	}

	genEntrustID(pRet->getEntrustID(), order_status_ack->client_seq_id);
	fmtutil::format_to(pRet->getOrderID(), "{}", order_status_ack->cl_ord_no);

	pRet->setStateMsg(order_status_ack->ord_rej_reason);

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

WTSTradeInfo* TraderATP::makeTradeInfo(const APITradeOrderUnit* trade_info)
{
	const std::string code(trade_info->security_id);
	const std::string exchg = (trade_info->market_id == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	pRet->setVolume((uint32_t)trade_info->last_qty / 100.0);
	pRet->setPrice(trade_info->last_px / 10000.0);
	pRet->setTradeID(trade_info->exec_id);
	pRet->setAmount(trade_info->total_value_traded / 100.0);
	pRet->setContractInfo(contract);

	uint32_t uTime = (uint32_t)(trade_info->transact_time % 1000000000);
	uint32_t uDate = (uint32_t)(trade_info->transact_time / 1000000000);

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime));

	WTSDirectionType dType = wrapATPSide(trade_info->side);
	pRet->setDirection(dType);

	//pRet->setOffsetType(wrapOffsetType(trade_info->side));
	pRet->setRefOrder(fmt::format("{}", trade_info->order_id).c_str());
	pRet->setTradeType(WTT_Common);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

WTSTradeInfo* TraderATP::makeTradeRecord(const ATPRspCashAuctionTradeERMsg *cash_auction_trade_er)
{
	const std::string code(cash_auction_trade_er->security_id);
	const std::string exchg = (cash_auction_trade_er->market_id == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	pRet->setVolume((uint32_t)cash_auction_trade_er->last_qty / 100);  // 成交数量
	pRet->setPrice(cash_auction_trade_er->last_px / 10000.0);  // 成交价格
	pRet->setTradeID(cash_auction_trade_er->exec_id);
	pRet->setContractInfo(contract);

	uint32_t uTime = (uint32_t)(cash_auction_trade_er->transact_time % 1000000000);
	uint32_t uDate = (uint32_t)(cash_auction_trade_er->transact_time / 1000000000);

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime));

	WTSDirectionType dType = wrapATPSide(cash_auction_trade_er->side);
	pRet->setDirection(dType);
	//pRet->setOffsetType(wrapOffsetType(cash_auction_trade_er->side));
	fmtutil::format_to(pRet->getRefOrder(), "{}", cash_auction_trade_er->cl_ord_id);
	pRet->setTradeType(WTT_Common);

	double amount = cash_auction_trade_er->total_value_traded / 10000.0;   //成交金额
	pRet->setAmount(amount);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

void TraderATP::OnLogin(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Connect, 0);

	_state = TS_LOGINED;  // 已登录
	write_log(_sink, LL_WARN, "[TraderATP] {} login success: {}...", _user, reason);
}

void TraderATP::OnLogout(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Logout, 0);

	_state = TS_NOTLOGIN;
	write_log(_sink, LL_WARN, "[TraderATP] {} logout: {}...", _user, reason);
}

void TraderATP::OnConnectFailure(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Connect, -1);

	_state = TS_LOGINFAILED;
	write_log(_sink, LL_WARN, "[TraderATP] Connect failed {}...", reason);
}

void TraderATP::OnConnectTimeOut(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Connect, -1);

	_state = TS_LOGINFAILED;
	write_log(_sink, LL_WARN, "[TraderATP] Connect timeout {}...", reason);
}

void TraderATP::OnHeartbeatTimeout(const std::string& reason)
{
	write_log(_sink, LL_WARN, "[TraderATP] Heartbeat timeout {}...", reason);
}

void TraderATP::OnClosed(const std::string& reason)
{
	_state = TS_NOTLOGIN;
	write_log(_sink, LL_WARN, "[TraderATP] Server closed {}...", reason);
}

void TraderATP::OnEndOfConnection(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, -1);

	write_log(_sink, LL_WARN, "[TraderATP] Connection ended {}...", reason);

	_close_flag = true;
}

void TraderATP::OnRspCustLoginResp(const ATPRspCustLoginRespOtherMsg &cust_login_resp)
{
	//std::vector<APIMsgCustLoginRespFundAccountUnit>::iterator it;
	//std::vector<APIMsgCustLoginRespAccountUnit>::iterator it_1;
	int i = 1, j = 1;
	for (auto it = cust_login_resp.fund_account_array.begin();
		it != cust_login_resp.fund_account_array.end(); it++)
	{
		for (auto it_1 = it->account_array.begin();
			it_1 != it->account_array.end(); it_1++)
		{
			write_log(_sink, LL_INFO, "[TraderATP][{}] OnRspCustLoginRsp: fund_account_id: {}, branch_id: {}, market_id: {}", it_1->account_id, it->fund_account_id, it->branch_id, it_1->market_id);

			j++;
		}
		i++;
	}

	if (cust_login_resp.permisson_error_code == 0)
	{
		_tradingday = TimeUtils::getCurDate();
		_cust_id = cust_login_resp.cust_id;

		{
			//初始化委托单缓存器
			std::stringstream ss;
			ss << "./atpdata/local/";
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
			ss << "./atpdata/local/";
			std::string path = StrUtil::standardisePath(ss.str());
			if (!StdFile::exists(path.c_str()))
				boost::filesystem::create_directories(path.c_str());
			ss << _user << "_oid.sc";
			m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
				write_log(_sink, LL_WARN, message);
			});
		}

		if (_sink)
			_sink->onLoginResult(true, "", _tradingday);
		_state = TS_ALLREADY;

		write_log(_sink, LL_INFO, "[TraderATP] CustLogin Success!");
	}
	else
	{
		if (_sink)
			_sink->onLoginResult(false, fmt::format("{}", cust_login_resp.permisson_error_code).c_str(), _tradingday);
		_state = TS_ALLREADY;

		write_log(_sink, LL_ERROR, "[TraderATP] CustLogin Fail, code : {}", static_cast<uint32_t>(cust_login_resp.permisson_error_code));
	}
}

void TraderATP::OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg &cust_logout_resp)
{
	if (cust_logout_resp.permisson_error_code == 0)
	{
		_state = TS_NOTLOGIN;
		if (_sink)
			_sink->onLogout();

		write_log(_sink, LL_INFO, "[TraderATP][{}] CustLogout Success!", cust_logout_resp.account_id);
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderATP][{}] Logout failed: {}", cust_logout_resp.account_id, static_cast<uint32_t>(cust_logout_resp.permisson_error_code));
	}
}

// 订单下达内部响应
void TraderATP::OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg& order_status_ack)
{
	write_log(_sink, LL_INFO, "[TraderATP][{}] cl_ord_no: {}, security_id: {}, ord_status: {}, price: {}, order_qty: {}, leaves_qty: {}, cum_qty: {}, cl_ord_id: {}, order_type: {}", order_status_ack.account_id,
		order_status_ack.cl_ord_no, order_status_ack.security_id, (int32_t)order_status_ack.ord_status, order_status_ack.price, order_status_ack.order_qty, order_status_ack.leaves_qty,
		order_status_ack.cum_qty, order_status_ack.cl_ord_id, order_status_ack.order_type);

	if (order_status_ack.reject_reason_code != ATPRejectReasonCodeConst::kNormal)
	{
		WTSEntrust* entrust = makeEntrust(&order_status_ack);

		if (order_status_ack.orig_cl_ord_no != 0)  // 可以根据orig_cl_ord_no是否为0来判断是否为撤单委托，需要注意撤单传入的orig_cl_ord_no不能为0
		{
			// 对应撤单回调
			write_log(_sink, LL_ERROR, "[TraderATP][{}] Cancelling order error, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id, order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);
			WTSError* error = WTSError::create(WEC_ORDERCANCEL, order_status_ack.ord_rej_reason);
			_sink->onRspEntrust(entrust, error);
			error->release();

			entrust->release();
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderATP][{}] Inserting order error, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id, order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);
			WTSError* error = WTSError::create(WEC_ORDERINSERT, order_status_ack.ord_rej_reason);
			_sink->onRspEntrust(entrust, error);
			error->release();

			entrust->release();
		}
	}
	else
	{
		write_log(_sink, LL_INFO, "[TraderATP][{}] Order success, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id, order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);

		WTSOrderInfo *orderInfo = makeOrderInfo(&order_status_ack);
		if (orderInfo)
		{
			//if (_sink)
			//	_sink->onPushOrder(orderInfo);

			//orderInfo->release();

			_asyncio.post([this, orderInfo] {
				if (_sink)
					_sink->onPushOrder(orderInfo);

				orderInfo->release();
			});
		}
	}

	// 保存回报分区号、序号，用于断线重连时指定已收到最新回报序号
	report_sync[order_status_ack.partition] = order_status_ack.index;
}

// 订单下达交易所确认
void TraderATP::OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg& order_status_ack)
{
	write_log(_sink, LL_INFO, "[OnRspOrderStatusAck][{}] account_id: {}, cl_ord_no: {}, code: {}, price: {}, order_qty: {}, ord_status: {}, side: {}", order_status_ack.account_id, order_status_ack.account_id,
		order_status_ack.cl_ord_no, order_status_ack.security_id, order_status_ack.price, order_status_ack.order_qty, (int32_t)order_status_ack.ord_status, order_status_ack.side);

	if (order_status_ack.reject_reason_code != ATPRejectReasonCodeConst::kNormal)
	{
		WTSEntrust* entrust = makeEntrust(&order_status_ack);

		if (order_status_ack.orig_cl_ord_no != 0)  // 可以根据orig_cl_ord_no是否为0来判断是否为撤单委托，需要注意撤单传入的orig_cl_ord_no不能为0
		{
			// 对应撤单回调
			write_log(_sink, LL_ERROR, "[OnRspOrderStatusAck][{}] Cancelling order error, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id, order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);
			WTSError* error = WTSError::create(WEC_ORDERCANCEL, order_status_ack.ord_rej_reason);
			_sink->onRspEntrust(entrust, error);
			error->release();

			entrust->release();
		}
		else
		{
			write_log(_sink, LL_ERROR, "[OnRspOrderStatusAck][{}] Inserting order error, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id, order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);
			WTSError* error = WTSError::create(WEC_ORDERINSERT, order_status_ack.ord_rej_reason);
			_sink->onRspEntrust(entrust, error);
			error->release();

			entrust->release();
		}
	}
	else
	{
		write_log(_sink, LL_INFO, "[OnRspOrderStatusAck][{}] Order success, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id, order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);

		WTSOrderInfo *orderInfo = makeOrderInfo(&order_status_ack);
		if (orderInfo)
		{
			//if (_sink)
			//	_sink->onPushOrder(orderInfo);

			//orderInfo->release();

			_asyncio.post([this, orderInfo] {
				if (_sink)
					_sink->onPushOrder(orderInfo);

				orderInfo->release();
			});
		}
	}

	// 保存回报分区号、序号，用于断线重连时指定已收到最新回报序号
	report_sync[order_status_ack.partition] = order_status_ack.index;
}

// 成交回报
void TraderATP::OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg& cash_auction_trade_er)
{
	write_log(_sink, LL_INFO, "[OnRspCashAuctionTradeER] code: {}, price: {}, qty: {}, cum_qty: {}, left_qty: {}, order_status: {}, cl_ord_no: {}, order_id: {}, cl_ord_id: {}",
		cash_auction_trade_er.security_id, cash_auction_trade_er.price, cash_auction_trade_er.order_qty, cash_auction_trade_er.cum_qty, cash_auction_trade_er.leaves_qty, cash_auction_trade_er.ord_status,
		cash_auction_trade_er.cl_ord_no, cash_auction_trade_er.order_id, cash_auction_trade_er.cl_ord_id);

	WTSTradeInfo *tRecord = makeTradeRecord(&cash_auction_trade_er);
	if (tRecord)
	{
		_asyncio.post([this, tRecord] {
			if (_sink)
				_sink->onPushTrade(tRecord);

			tRecord->release();
		});
	}

	// 保存回报分区号、序号，用于断线重连时指定已收到最新回报序号
	report_sync[cash_auction_trade_er.partition] = cash_auction_trade_er.index;
}

// 订单下达内部拒绝
void TraderATP::OnRspBizRejection(const ATPRspBizRejectionOtherMsg& biz_rejection)
{
	if (_sink)
		write_log(_sink, LL_ERROR, "[OnRspBizRejection] cliend_seq_id: {}, reject_code: {}, reject_msg: {}", biz_rejection.client_seq_id, biz_rejection.reject_reason_code, biz_rejection.business_reject_text);
}

void TraderATP::OnRspFundQueryResult(const ATPRspFundQueryResultMsg &msg)
{
	write_log(_sink, LL_INFO, "[OnRspFundQueryResult][{}] OnRspFundQueryResult.", msg.account_id);

	WTSArray* ayFunds = WTSArray::create();
	WTSAccountInfo* fundInfo = WTSAccountInfo::create();
	fundInfo->setPreBalance(msg.init_leaves_value / 10000.0);
	fundInfo->setBalance(msg.leaves_value / 10000.0);
	fundInfo->setAvailable(msg.available_tall / 10000.0);
	ayFunds->append(fundInfo, false);

	if (_sink)
		_sink->onRspAccount(ayFunds);

	ayFunds->release();
}

void TraderATP::OnRspOrderQueryResult(const ATPRspOrderQueryResultMsg &msg)
{
	write_log(_sink, LL_INFO, "[OnRspOrderQueryResult][{}] last index: {}, total nums: {}", msg.account_id, msg.last_index, msg.total_num);

	_return_nums = msg.total_num;

	if (ayOrders == NULL)
		ayOrders = WTSArray::create();

	for (const auto& unit : msg.order_array)
	{
		WTSOrderInfo* ordInfo = makeOrderInfo(&unit);
		if (ordInfo == NULL)
			continue;

		_asyncio.post([this, ordInfo] {
			ayOrders->append(ordInfo, false);
		});
	}

	if ((msg.last_index + 1) == msg.total_num)  // 查询完毕
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspOrders(ayOrders);

			ayOrders->clear();
		});
	}
}

void TraderATP::OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg &msg)
{
	_return_nums = msg.total_num;

	if (ayTrades == NULL)
		ayTrades = WTSArray::create();

	//write_log(_sink, LL_INFO, "[TraderATP][{}] OnRspTradeOrderQueryResult, last index: {}, total nums: {}", msg.account_id, msg.last_index, msg.total_num);

	for (const auto& unit : msg.order_array)
	{
		WTSTradeInfo* trdInfo = makeTradeInfo(&unit);
		if (trdInfo == NULL)
			continue;

		_asyncio.post([this, trdInfo] {
			ayTrades->append(trdInfo, false);
		});
	}

	//if ((msg.last_index + 1) == msg.total_num)  // 查询完毕
	//{
	//	if (!_sz_acctid.empty() && (strcmp(msg.account_id, _sz_acctid.c_str()) == 0))
	//	{
	//		// 此时返回了所有结果
	//		if (_sink)
	//			_sink->onRspTrades(ayTrades);

	//		ayTrades->clear();
	//	}
	//	else if (_sz_acctid.empty())
	//	{
	//		// 此时返回了所有结果
	//		if (_sink)
	//			_sink->onRspTrades(ayTrades);

	//		ayTrades->clear();
	//	}
	//}

	if ((msg.last_index + 1) == msg.total_num)  // 查询完毕
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspTrades(ayTrades);

			ayTrades->clear();
		});
	}
}

void TraderATP::OnRspShareQueryResult(const ATPRspShareQueryResultMsg &msg)
{
	_return_nums = msg.total_num;

	if (ayTrades == NULL)
		ayTrades = WTSArray::create();

	if (NULL == _positions)
		_positions = PositionMap::create();

	for (auto &unit : msg.order_array) {
		const std::string exchg = (unit.market_id == ATPMarketIDConst::kShangHai) ? "SSE" : "SZSE";

		WTSContractInfo* contract = _bd_mgr->getContract(unit.security_id, exchg.c_str());
		if (contract)
		{
			WTSCommodityInfo* commInfo = contract->getCommInfo();
			std::string key = fmt::format("{}-{}", unit.security_id, exchg.c_str());
			WTSPositionItem* pos = (WTSPositionItem*)_positions->get(key);
			if (pos == NULL)
			{
				pos = WTSPositionItem::create(unit.security_id, commInfo->getCurrency(), commInfo->getExchg());
				pos->setContractInfo(contract);
				_positions->add(key, pos, false);
			}
			//pos->setDirection(wrapPosDirection(position->position_direction));

			double tmp = (double)(unit.leaves_qty / 100.0 - unit.init_qty / 100.0);

			pos->setNewPosition((tmp >= 0) ? tmp : 0);  // 剩余股份数量N15(2)，包含当日买入部分，拆分合并，申购赎回
			pos->setPrePosition((double)unit.init_qty / 100.0);  // 日初持仓量N15(2)

			pos->setAvailNewPos(0);
			pos->setAvailPrePos(unit.available_qty / 100.0);   // 可用股份数量N15(2)

			pos->setMargin(unit.market_value / 10000.0);
			pos->setDynProfit(unit.profit_loss / 10000.0);
			pos->setPositionCost(unit.market_value / 10000.0);

			pos->setAvgPrice(unit.cost_price / 100.0);

			write_log(_sink, LL_INFO, "[OnRspShareQueryResult][{}] {}.{}, {}[{}]", msg.account_id, exchg, pos->getCode(), pos->getNewPosition(), pos->getPrePosition());
		}
	}

	if ((msg.last_index + 1) == msg.total_num)  // 查询完毕
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

			ayPos->release();
		});
	}
}

#pragma region "ITraderApi"
bool TraderATP::init(WTSVariant *params)
{
	_user = params->getCString("user");
	_pass = params->getCString("pass");
	_loginmode = params->getInt32("login_mode");
	_sh_acctid = params->getCString("sh_acct_id");
	_sz_acctid = params->getCString("sz_acct_id");
	_acctid = params->getCString("acct_id");
	_is_sh = params->getBoolean("is_sh");
	_accpasswd = params->getCString("account_key");
	_fund_accountid = params->getCString("fund_account_id");
	_cust_id = params->getCString("cust_id");
	_branchid = params->getCString("branch_id");
	_node_id = params->getCString("node_id");
	_order_way = params->getInt32("order_way");

	_front = params->getCString("front");
	_front2 = params->getCString("front_backup");

	WTSArray* ayPos = WTSArray::create();

	std::string module = params->getCString("atpmodule");
	std::string dllpath;

	if (module.empty()) module = "atptradeapi";
	dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");

	m_hInstATP = DLLHelper::load_library(dllpath.c_str());

	static bool inited = false;
	if (!inited)
	{
		// 初始化API
		const std::string station_name = ""; // 站点信息，该字段已经不使用
		const std::string cfg_path = ".";      // 配置文件路径
		const std::string log_dir_path = ""; // 日志路径
		bool record_all_flag = true;         // 是否记录所有委托信息
		//std::tr1::unordered_map<std::string, std::string> encrypt_cfg;  // 加密库配置
		//bool connection_retention_flag = false;   // 是否启用会话保持

		//// encrypt_cfg参数填写：
		//encrypt_cfg["ENCRYPT_SCHEMA"] = "0";              // 字符 0 表示 不对消息中的所有 password 加密
		//encrypt_cfg["ATP_ENCRYPT_PASSWORD"] = "";         // 除登入及密码修改外其他消息的密码字段加密算法
		//encrypt_cfg["ATP_LOGIN_ENCRYPT_PASSWORD"] = "";   // 登入及密码修改消息中密码字段的加密算法so路径
		//encrypt_cfg["GM_SM2_PUBLIC_KEY_PATH"] = "";       // 采用国密算法时，通过该key配置 GM算法配置加密使用的公钥路径
		//encrypt_cfg["RSA_PUBLIC_KEY_PATH"] = "";          // 如果使用rsa算法加密，通过该key配置 rsa算法配置加密使用的公钥路径

		ATPRetCodeType ec = ATPTradeAPI::Init(station_name, cfg_path, log_dir_path, record_all_flag);

		if (ec != ErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "ATPTradeAPI init failed: {}", ec);
			return false;
		}

		inited = true;
	}

	return true;
}

void TraderATP::release()
{
	if (_api)
	{
		_api->Close();
		while (!_close_flag)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
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
}

void TraderATP::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderATP::reconnect()
{
	if (_api)
	{
		_api->Close();
		while (!_close_flag)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		delete _api;
		_api = NULL;
	}

	_api = new ATPTradeAPI();

	std::vector<std::string> locations;
	locations.push_back(_front);
	locations.push_back(_front2);

	// 设置连接信息
	ATPConnectProperty prop;
	prop.user = _user;												// 网关用户名
	prop.password = _pass;											// 网关用户密码
	prop.locations = locations;										// 网关主备节点的地址+端口
	prop.heartbeat_interval_milli = 5000;                           // 发送心跳的时间间隔，单位：毫秒
	prop.connect_timeout_milli = 5000;                              // 连接超时时间，单位：毫秒
	prop.reconnect_time = 10;                                       // 重试连接次数
	prop.client_name = _product;									// 客户端程序名字
	prop.report_sync = report_sync;									// 回报同步数据分区号+序号，首次是空，断线重连时填入的是接受到的最新分区号+序号
	prop.mode = 0;													// 模式0-同步回报模式，模式1-快速登录模式，不同步回报

	ATPRetCodeType ec = _api->Connect(prop, this);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderATP] Connect error: {}", ec);
	}
}

void TraderATP::connect()
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
}

void TraderATP::disconnect()
{
	release();
}

bool TraderATP::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderATP::genEntrustID(char* buffer, uint32_t orderRef)
{
	//这里不再使用sessionid，因为每次登陆会不同，如果使用的话，可能会造成不唯一的情况
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderATP::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderATP::makeEntrustID(char* buffer, int length)
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

void TraderATP::doLogin(const char* productInfo)
{
	_state = TS_LOGINING;

	// 设置登入消息
	ATPReqCustLoginOtherMsg login_msg;

	wt_strcpy(login_msg.fund_account_id, _fund_accountid.c_str());           // 资金账户ID
	wt_strcpy(login_msg.password, _accpasswd.c_str());                  // 客户号密码
	strncpy(login_msg.user_info, _user.c_str(), 17);
	login_msg.login_mode = _loginmode;  //  wrapLoginMode(_loginmode);  // ATPCustLoginModeType::kFundAccountIDMode;	// 登录模式，资金账号登录
	login_msg.client_seq_id = genRequestID();							// 客户系统消息号
	login_msg.order_way = std::to_string(_order_way).at(0); //'0';											// 委托方式，自助委托
	login_msg.client_feature_code = _node_id;						// 终端识别码

	strncpy(login_msg.branch_id, _branchid.c_str(), 11);

	ATPRetCodeType ec = _api->ReqCustLoginOther(&login_msg);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderAPT] Login error: {}", ec);
	}
}

int TraderATP::login(const char* user, const char* pass, const char* productInfo)
{
	_user = user;
	_pass = pass;
	_product = productInfo;

	if (_api == NULL)
	{
		return -1;
	}

	doLogin(productInfo);

	return 0;
}

int TraderATP::logout()
{
	if (_api == NULL)
		return -1;

	ATPReqCustLogoutOtherMsg logout_msg;
	wt_strcpy(logout_msg.fund_account_id, _fund_accountid.c_str());	// 资金账户ID
	logout_msg.client_seq_id = genRequestID();              // 客户系统消息号
	//logout_msg.client_feature_code = _product;				// 终端识别码

	logout_msg.client_feature_code = _node_id;				// 终端识别码

	ATPRetCodeType ec = _api->ReqCustLogoutOther(&logout_msg);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderAPT] Logout error: {}", ec);
		return ec;
	}
	return 0;
}

int TraderATP::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	// 沪深市场股票限价委托
	thread_local static ATPReqCashAuctionOrderMsg p;
	//ATPReqCashAuctionOrderMsg p;
	//memset(&p, 0, sizeof(p));

	uint32_t orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);

	//bool isSh = (strcmp(entrust->getExchg(), "SSE") == 0) ? true : false;

	wt_strcpy(p.security_id, entrust->getCode());                   // 证券代码
	p.market_id = _is_sh ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;             // 市场ID，上海
	wt_strcpy(p.cust_id, _cust_id.c_str());                 // 客户号ID
	wt_strcpy(p.fund_account_id, _fund_accountid.c_str());       // 资金账户ID

	wt_strcpy(p.account_id, _acctid.c_str());                 // 账户ID

	//p.side = (entrust->getDirection() == WOT_OPEN) ? ATPSideConst::kBuy : ATPSideConst::kSell;      // 买卖方向，买
	p.side = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	//p.order_type = ATPOrdTypeConst::kFixedNew;				// 订单类型，限价
	p.order_type = wrapOrdType(entrust->getPriceType(), entrust->getOrderFlag());
	p.price = (int32_t)(entrust->getPrice() * 10000);         // 委托价格 N13(4)，21.0000元
	p.order_qty = (int32_t)(entrust->getVolume() * 100);            // 申报数量N15(2)；股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张；1000.00股
	p.client_seq_id = genRequestID();						// 用户系统消息序号
	p.order_way = std::to_string(_order_way).at(0);  // 0;										// 委托方式，自助委托
	p.client_feature_code = _node_id;  // 终端识别码
	strncpy(p.password, _pass.c_str(), 129);							// 客户密码
	fmt::format_to(p.user_info, "{}", orderref);

	//const int totals = 3000;
	//for (int i = 0; i < totals; i++)
	{
		write_log(_sink, LL_INFO, "[orderInsert] code: {}, acct_id: {}, side: {}, price: {}, qty: {}", p.security_id, p.account_id, p.side, p.price, p.order_qty);

		if (strlen(entrust->getUserTag()) > 0)
		{
			m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
				write_log(_sink, LL_WARN, message);
			});
		}


		ATPRetCodeType ec = _api->ReqCashAuctionOrder(&p);
		if (ec != ErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "[TraderATP] Order inserting failed: {}", ec);
		}
	}

	return 0;
}

int TraderATP::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	thread_local static ATPReqCancelOrderMsg p;
	//ATPReqCancelOrderMsg p;0.0
	//memset(&p, 0, sizeof(p));

	//bool isSH = strcmp(action->getExchg(), "SSE") == 0;

	p.market_id = (_is_sh) ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;
	wt_strcpy(p.cust_id, _cust_id.c_str());                    // 客户号ID
	wt_strcpy(p.fund_account_id, _fund_accountid.c_str());          // 资金账户ID
	wt_strcpy(p.user_info, _user.c_str());                   // 账户ID

	wt_strcpy(p.account_id, _acctid.c_str());  // 证券账户

	wt_strcpy(p.password, _accpasswd.c_str());  // 交易密码
	p.client_seq_id = genRequestID();
	p.order_way = std::to_string(_order_way).at(0);
	p.orig_cl_ord_no = strtoull(action->getOrderID(), NULL, 10);

	p.client_feature_code = _node_id;

	//write_log(_sink, LL_ERROR, "[TraderATP][{}]账户超出撤单阈值！", p.account_id);
	//return -1;

	ATPRetCodeType ec = _api->ReqCancelOrder(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderATP] Order cancelling failed: {}", ec);
	}

	return 0;
}

uint32_t TraderATP::genRequestID()
{
	return _reqid.fetch_add(1) + 1;
}

int TraderATP::queryAccount()
{
	ATPReqFundQueryMsg p;

	strncpy(p.cust_id, _cust_id.c_str(), 17);
	strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
	strncpy(p.account_id, _acctid.c_str(), 13);
	p.client_seq_id = genRequestID();
	strncpy(p.user_info, _user.c_str(), 17);
	strncpy(p.password, _pass.c_str(), 129);
	strncpy(p.currency, "CNY", 5);
	p.client_feature_code = _node_id;

	ATPRetCodeType ec = _api->ReqFundQuery(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderATP][{}] Query account failed: {}", _acctid, ec);
	}

	return 0;
}

int TraderATP::queryPositions()
{
	_asyncio.post([this]() {
		{
			ATPReqShareQueryMsg p;
			strncpy(p.cust_id, _cust_id.c_str(), 17);
			strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p.account_id, _acctid.c_str(), 13);
			p.client_seq_id = genRequestID();
			strncpy(p.user_info, _user.c_str(), 17);
			strncpy(p.password, _pass.c_str(), 129);
			p.client_feature_code = _node_id;
			p.query_index = 0;

			APIAccountIDUnit api_account_unit;
			strncpy(api_account_unit.account_id, _acctid.c_str(), 13);
			p.account_id_array.push_back(api_account_unit);

			ATPRetCodeType ec = _api->ReqShareQuery(&p);
			if (ec != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _acctid, ec);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (_return_nums > 100)  // 最大单页容量为100条
			{
				// 需要多页查询
				if (_return_nums % 100 > 0)
					_return_nums += 100;

				int times = _return_nums / 100;

				for (int i = 0; i < times; i++)
				{
					p.query_index = (i + 1) * 100;

					ec = _api->ReqShareQuery(&p);
					if (ec != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _acctid, ec);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	});

	return 0;
}

int TraderATP::queryOrders()
{
	_asyncio.post([this]() {
		{
			ATPReqOrderQueryMsg p;
			strncpy(p.cust_id, _cust_id.c_str(), 17);
			strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p.account_id, _acctid.c_str(), 13);
			p.client_seq_id = genRequestID();
			p.business_abstract_type = 1;
			strncpy(p.user_info, _user.c_str(), 17);
			strncpy(p.password, _pass.c_str(), 129);
			p.client_feature_code = _node_id;
			p.query_index = 0;

			ATPRetCodeType ec = _api->ReqOrderQuery(&p);
			if (ec != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query orders failed: {}", _acctid, ec);
			}
			write_log(_sink, LL_INFO, "return num: {}", _return_nums);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			write_log(_sink, LL_INFO, "return num: {}", _return_nums);

			if (_return_nums > 100)
			{
				// 需要多页查询
				if (_return_nums % 100 > 0)
					_return_nums += 100;

				int times = _return_nums / 100;

				for (int i = 0; i < times; i++)
				{
					p.query_index = (i + 1) * 100;

					ec = _api->ReqOrderQuery(&p);
					if (ec != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query orders failed: {}", _acctid, ec);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}
	});

	return 0;
}

int TraderATP::queryTrades()
{
	_asyncio.post([this]() {
		{
			ATPReqTradeOrderQueryMsg p;
			strncpy(p.cust_id, _cust_id.c_str(), 17);
			strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p.account_id, _acctid.c_str(), 13);
			p.client_seq_id = genRequestID();
			p.business_abstract_type = 1;
			strncpy(p.user_info, _user.c_str(), 17);
			strncpy(p.password, _pass.c_str(), 129);
			p.client_feature_code = _node_id;
			p.query_index = 0;

			ATPRetCodeType ec = _api->ReqTradeOrderQuery(&p);
			if (ec != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query trades failed: {}", _acctid, ec);
			}
			write_log(_sink, LL_INFO, "return num: {}", _return_nums);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			write_log(_sink, LL_INFO, "return num: {}", _return_nums);

			if (_return_nums > 100)
			{
				// 需要多页查询
				if (_return_nums % 100 > 0)
					_return_nums += 100;

				int times = _return_nums / 100;

				for (int i = 0; i < times; i++)
				{
					p.query_index = (i + 1) * 100;

					ec = _api->ReqTradeOrderQuery(&p);
					if (ec != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query trades failed: {}", _acctid, ec);
					}
					//std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	});

	return 0;
}
#pragma endregion "ITraderApi"