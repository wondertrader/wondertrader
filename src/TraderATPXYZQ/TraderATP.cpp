/*!
 * \file TraderATP.cpp
 * \project	WonderTrader
 *
 * \author Sunnnnker
 * \date 2022/10/30
 *
 * \brief 
 * \ this is for XYZQ
 */
#include "TraderATP.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>
#include <iostream>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/AtpTraderApiXYZQ3.1.10.6/x64/atptradeapi.lib")
#else
#pragma comment(lib, "../API/AtpTraderApiXYZQ3.1.10.6/x86/atptradeapi.lib")
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
	, _sh_total_num(0)
	, _sz_total_num(0)
	, ayOrders(NULL)
	, ayTrades(NULL)
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
		return ATPOrdTypeConst::kFixedNew;  // �޼�ί�С���ǿ�޼�

	if (WPT_LIMITPRICE == priceType && flag == WOF_FOK)
		return ATPOrdTypeConst::kFixedFullDealOrCancel;  // �޼�ȫ��ɽ�����

	if (WPT_ANYPRICE == priceType && flag == WOF_NOR)
		return ATPOrdTypeConst::kMarketTransferFixed;  // �м�ʣ��ת�޼�

	if (WPT_ANYPRICE == priceType && flag == WOF_FAK)
		return ATPOrdTypeConst::kImmediateDealTransferCancel;  // �����ɽ�ʣ�೷��

	if (WPT_ANYPRICE == priceType && flag == WOF_FOK)
		return ATPOrdTypeConst::kFullDealOrCancel;  // ȫ��ɽ�����

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
	std::string code, exchg;
	if (order_info->market_id == ATPMarketIDConst::kShangHai)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_info->security_id;
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
	std::string code, exchg;
	if (order_info->market_id == 101)
		exchg = "SSE";
	else if (order_info->market_id == 102)
		exchg = "SZSE";

	code = order_info->security_id;
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

WTSOrderInfo* TraderATP::makeOrderInfo(const ATPRspOrderStatusAckMsg *order_status_ack)
{
	std::string code, exchg;
	if (order_status_ack->market_id == ATPMarketIDConst::kShangHai)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_status_ack->security_id;
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
	if (order_status_ack->ord_status == ATPOrdStatusConst::kReject || order_status_ack->ord_status == ATPOrdStatusConst::kUnSend) {
		pRet->setError(true);
		pRet->setOrderState(WOS_Canceled);
	}

	genEntrustID(pRet->getEntrustID(), order_status_ack->client_seq_id);
	fmtutil::format_to(pRet->getOrderID(), "{}", order_status_ack->cl_ord_no);

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

WTSTradeInfo* TraderATP::makeTradeInfo(const APITradeOrderUnit* trade_info)
{
	std::string code, exchg;
	if (trade_info->market_id == ATPMarketIDConst::kShangHai)
		exchg = "SSE";
	else if (trade_info->market_id == ATPMarketIDConst::kShenZhen)
		exchg = "SZSE";

	code = trade_info->security_id;
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
	std::string code, exchg;
	if (cash_auction_trade_er->market_id == ATPMarketIDConst::kShangHai)
		exchg = "SSE";
	else if (cash_auction_trade_er->market_id == ATPMarketIDConst::kShenZhen)
		exchg = "SZSE";

	code = cash_auction_trade_er->security_id;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	pRet->setVolume((uint32_t)cash_auction_trade_er->last_qty / 100);  // �ɽ�����
	pRet->setPrice(cash_auction_trade_er->last_px / 10000.0);  // �ɽ��۸�
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

	double amount = cash_auction_trade_er->total_value_traded / 10000.0;   //�ɽ����
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

	_state = TS_LOGINED;  // �ѵ�¼
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
	write_log(_sink, LL_INFO, "[TraderATP] OnRspCustLoginResp Recv: {}", static_cast<uint32_t>(cust_login_resp.permisson_error_code));

	//write_log(_sink, LL_INFO, "[TraderATP] client_seq_id: {}, permission_error_code: {}, user_info: {}", cust_login_resp->client_seq_id, cust_login_resp->permisson_error_code, cust_login_resp->user_info);

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
			//��ʼ��ί�е�������
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
			//��ʼ��������ǻ�����
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

		write_log(_sink, LL_ERROR, "[TraderATP] CustLogin Fail, permisson_error_code : {}", static_cast<uint32_t>(cust_login_resp.permisson_error_code));
	}
}

void TraderATP::OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg &cust_logout_resp)
{
	write_log(_sink, LL_INFO, "[TraderATP] OnRspCustLogoutResp Recv: {}", static_cast<uint32_t>(cust_logout_resp.permisson_error_code));

	if (cust_logout_resp.permisson_error_code == 0)
	{
		_state = TS_NOTLOGIN;
		if (_sink)
			_sink->onLogout();

		write_log(_sink, LL_INFO, "[TraderATP][{}] CustLogout Success!", cust_logout_resp.account_id);
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderATP][{}] Logout failed: {}", cust_logout_resp.account_id,static_cast<uint32_t>(cust_logout_resp.permisson_error_code));
	}
}

// �����´��ڲ���Ӧ
void TraderATP::OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg& order_status_ack)
{
	write_log(_sink, LL_INFO, "[TraderATP][{}] cl_ord_no: {}, security_id: {}, ord_status: {}, price: {}, order_qty: {}, leaves_qty: {}, cum_qty: {}, cl_ord_id: {}, order_type: {}", order_status_ack.account_id,
		order_status_ack.cl_ord_no, order_status_ack.security_id, (int32_t)order_status_ack.ord_status, order_status_ack.price, order_status_ack.order_qty, order_status_ack.leaves_qty,
		order_status_ack.cum_qty, order_status_ack.cl_ord_id, order_status_ack.order_type);

	if (order_status_ack.reject_reason_code != ATPRejectReasonCodeConst::kNormal)
	{
		WTSEntrust* entrust = makeEntrust(&order_status_ack);

		if (order_status_ack.orig_cl_ord_no != 0)  // ���Ը���orig_cl_ord_no�Ƿ�Ϊ0���ж��Ƿ�Ϊ����ί�У���Ҫע�⳷�������orig_cl_ord_no����Ϊ0
		{
			// ��Ӧ�����ص�
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

	// ����ر������š���ţ����ڶ�������ʱָ�����յ����»ر����
	report_sync[order_status_ack.partition] = order_status_ack.index;
}

// �����´ｻ����ȷ��
void TraderATP::OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg& order_status_ack)
{
	write_log(_sink, LL_INFO, "[TraderATP][{}] account_id: {}, cl_ord_no: {}, code: {}, price: {}, order_qty: {}, ord_status: {}, side: {}", order_status_ack.account_id,order_status_ack.account_id,
		order_status_ack.cl_ord_no, order_status_ack.security_id, order_status_ack.price, order_status_ack.order_qty, (int32_t)order_status_ack.ord_status, order_status_ack.side);

	if (order_status_ack.reject_reason_code != ATPRejectReasonCodeConst::kNormal)
	{
		WTSEntrust* entrust = makeEntrust(&order_status_ack);

		if (order_status_ack.orig_cl_ord_no != 0)  // ���Ը���orig_cl_ord_no�Ƿ�Ϊ0���ж��Ƿ�Ϊ����ί�У���Ҫע�⳷�������orig_cl_ord_no����Ϊ0
		{
			// ��Ӧ�����ص�
			write_log(_sink, LL_ERROR, "[TraderATP][{}] Cancelling order error, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id,order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);
			WTSError* error = WTSError::create(WEC_ORDERCANCEL, order_status_ack.ord_rej_reason);
			_sink->onRspEntrust(entrust, error);
			error->release();

			entrust->release();
		}
		else
		{
			write_log(_sink, LL_ERROR, "[TraderATP][{}] Inserting order error, cl_ord_no: {}, orig_cl_ord_no: {}, reject code: {}, reject reason: {}", order_status_ack.account_id, order_status_ack.cl_ord_no, order_status_ack.orig_cl_ord_no, order_status_ack.reject_reason_code, order_status_ack.ord_rej_reason);
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

	// ����ر������š���ţ����ڶ�������ʱָ�����յ����»ر����
	report_sync[order_status_ack.partition] = order_status_ack.index;
}

// �ɽ��ر�
void TraderATP::OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg& cash_auction_trade_er)
{
	std::cout <<
		//"partition : " << (int32_t)cash_auction_trade_er.partition <<
		//" index : " << cash_auction_trade_er.index <<
		//" business_type : " << (int32_t)cash_auction_trade_er.business_type <<
		" cl_ord_no : " << cash_auction_trade_er.cl_ord_no <<
		" security_id : " << cash_auction_trade_er.security_id <<
		//" market_id : " << cash_auction_trade_er.market_id <<
		//" exec_type : " << cash_auction_trade_er.exec_type <<
		" ord_status : " << (int32_t)cash_auction_trade_er.ord_status <<
		" cust_id : " << cash_auction_trade_er.cust_id <<
		//" fund_account_id : " << cash_auction_trade_er.fund_account_id <<
		" account_id : " << cash_auction_trade_er.account_id <<
		" price : " << cash_auction_trade_er.price <<
		" order_qty : " << cash_auction_trade_er.order_qty <<
		" leaves_qty : " << cash_auction_trade_er.leaves_qty <<
		" cum_qty : " << cash_auction_trade_er.cum_qty <<
		" side : " << cash_auction_trade_er.side <<
		" transact_time : " << cash_auction_trade_er.transact_time <<
		//" user_info : " << cash_auction_trade_er.user_info <<
		" order_id : " << cash_auction_trade_er.order_id <<
		" cl_ord_id : " << cash_auction_trade_er.cl_ord_id <<
		//" exec_id : " << cash_auction_trade_er.exec_id <<
		//" last_px : " << cash_auction_trade_er.last_px <<
		//" last_qty : " << cash_auction_trade_er.last_qty <<
		//" total_value_traded : " << cash_auction_trade_er.total_value_traded <<
		" fee : " << cash_auction_trade_er.fee << std::endl;
	//	" cash_margin : " << cash_auction_trade_er.cash_margin << std::endl;

	WTSTradeInfo *tRecord = makeTradeRecord(&cash_auction_trade_er);
	if (tRecord)
	{
		_asyncio.post([this, tRecord] {
			if (_sink)
				_sink->onPushTrade(tRecord);

			tRecord->release();
		});
	}

	// ����ر������š���ţ����ڶ�������ʱָ�����յ����»ر����
	report_sync[cash_auction_trade_er.partition] = cash_auction_trade_er.index;
}

// �����´��ڲ��ܾ�
void TraderATP::OnRspBizRejection(const ATPRspBizRejectionOtherMsg& biz_rejection)
{
	if (_sink)
		write_log(_sink, LL_ERROR, "[TraderATP] cliend_seq_id: {}, reject_code: {}, reject_msg: {}", biz_rejection.client_seq_id, biz_rejection.reject_reason_code, biz_rejection.business_reject_text);
}

void TraderATP::OnRspFundQueryResult(const ATPRspFundQueryResultMsg &msg)
{
	write_log(_sink, LL_INFO, "[TraderATP][{}] OnRspFundQueryResult.", msg.account_id);

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
	write_log(_sink, LL_INFO, "[TraderATP][{}] OnRspOrderQueryResult, last index: {}, total nums: {}", msg.account_id, msg.last_index, msg.total_num);

	if (strcmp(msg.account_id, _sh_acctid.c_str()) == 0)
		_sh_total_num = msg.total_num;
	else
		_sz_total_num = msg.total_num;

	if (ayOrders == NULL)
		ayOrders = WTSArray::create();

	for (const auto& unit : msg.order_array)
	{
		WTSOrderInfo* ordInfo = makeOrderInfo(&unit);
		if (ordInfo == NULL)
			continue;

		ayOrders->append(ordInfo, false);
	}

	if ((msg.last_index + 1) == msg.total_num)  // ��ѯ���
	{
		if (!_sz_acctid.empty() && (strcmp(msg.account_id, _sz_acctid.c_str()) == 0))
		{
			if (_sink)
				_sink->onRspOrders(ayOrders);

			ayOrders->clear();
		}
	}
	else if (_sz_acctid.empty())
	{
		if (_sink)
			_sink->onRspOrders(ayOrders);

		ayOrders->clear();
	}
}

void TraderATP::OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg &msg)
{
	if (strcmp(msg.account_id, _sh_acctid.c_str()) == 0)
		_sh_total_num = msg.total_num;
	else
		_sz_total_num = msg.total_num;

	if (ayTrades == NULL)
		ayTrades = WTSArray::create();

	//write_log(_sink, LL_INFO, "[TraderATP][{}] OnRspTradeOrderQueryResult, last index: {}, total nums: {}", msg.account_id, msg.last_index, msg.total_num);

	for (const auto& unit : msg.order_array)
	{
		WTSTradeInfo* trdInfo = makeTradeInfo(&unit);
		if (trdInfo == NULL)
			continue;

		ayTrades->append(trdInfo, false);
	}
	
	if ((msg.last_index + 1) == msg.total_num)  // ��ѯ���
	{
		if (!_sz_acctid.empty() && (strcmp(msg.account_id, _sz_acctid.c_str()) == 0))
		{
			// ��ʱ���������н��
			if (_sink)
				_sink->onRspTrades(ayTrades);

			ayTrades->clear();
		}
		else if (_sz_acctid.empty())
		{
			// ��ʱ���������н��
			if (_sink)
				_sink->onRspTrades(ayTrades);

			ayTrades->clear();
		}
	}
}

void TraderATP::OnRspShareQueryResult(const ATPRspShareQueryResultMsg &msg)
{
	if (strcmp(msg.account_id, _sh_acctid.c_str()) == 0)
		_sh_total_num = msg.total_num;
	else
		_sz_total_num = msg.total_num;

	if (ayTrades == NULL)
		ayTrades = WTSArray::create();

	write_log(_sink, LL_INFO, "[TraderATP][{}] OnRspShareQueryResult, last index: {}, total nums: {}", msg.account_id, msg.last_index, msg.total_num);

	if (NULL == _positions)
		_positions = PositionMap::create();

	for (auto &unit : msg.order_array) {
		std::string exchg;
		if (unit.market_id == ATPMarketIDConst::kShangHai)
			exchg = "SSE";
		else
			exchg = "SZSE";

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

			pos->setNewPosition((double)(unit.leaves_qty / 100.0));  // ʣ��ɷ�����N15(2)�������������벿�֣���ֺϲ����깺���
			pos->setPrePosition((double)unit.init_qty / 100.0);  // �ճ��ֲ���N15(2)

			pos->setMargin(unit.market_value / 10000.0);
			pos->setDynProfit(unit.profit_loss / 10000.0);
			pos->setPositionCost(unit.market_value / 10000.0);
			pos->setAvailNewPos(unit.available_qty / 100.0);  // ���ùɷ�����N15(2)

			pos->setAvgPrice(unit.cost_price / 100.0);

			pos->setAvailPrePos((double)unit.init_qty / 100.0);

			//write_log(_sink, LL_INFO, "[{}][Position] {}.{}, {}[{}]", msg.account_id, exchg, pos->getCode(), pos->getNewPosition(), pos->getPrePosition());
		}
	}

	if ((msg.last_index + 1) == msg.total_num)  // ��ѯ���
	{
		if (!_sz_acctid.empty() && strcmp(msg.account_id, _sz_acctid.c_str()) == 0)
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
		else if (_sz_acctid.empty())
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
}

#pragma region "ITraderApi"
bool TraderATP::init(WTSVariant *params)
{
	write_log(_sink, LL_INFO, "Initalizing TraderATP ...");

	_user = params->getCString("user");
	_pass = params->getCString("pass");
	_loginmode = params->getInt32("login_mode");
	_sh_acctid = params->getCString("sh_acct_id");
	_sz_acctid = params->getCString("sz_acct_id");
	_accpasswd = params->getCString("account_key");
	_fund_accountid = params->getCString("fund_account_id");
	_cust_id = params->getCString("cust_id");
	_branchid = params->getCString("branch_id");
	_node_id = params->getCString("node_id");

	_front = params->getCString("front");
	_front2 = params->getCString("front_backup");

	std::string module = params->getCString("atpmodule");
	if (module.empty()) module = "atptradeapi";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;
	m_hInstATP = DLLHelper::load_library(dllpath.c_str());

	static bool inited = false;
	if (!inited)
	{
		// ��ʼ��API
		const std::string station_name = ""; // վ����Ϣ�����ֶ��Ѿ���ʹ��
		const std::string cfg_path = ".";      // �����ļ�·��
		const std::string log_dir_path = ""; // ��־·��
		bool record_all_flag = true;         // �Ƿ��¼����ί����Ϣ
		//std::tr1::unordered_map<std::string, std::string> encrypt_cfg;  // ���ܿ�����
		//bool connection_retention_flag = false;   // �Ƿ����ûỰ����

		//// encrypt_cfg������д��
		//encrypt_cfg["ENCRYPT_SCHEMA"] = "0";              // �ַ� 0 ��ʾ ������Ϣ�е����� password ����
		//encrypt_cfg["ATP_ENCRYPT_PASSWORD"] = "";         // �����뼰�����޸���������Ϣ�������ֶμ����㷨
		//encrypt_cfg["ATP_LOGIN_ENCRYPT_PASSWORD"] = "";   // ���뼰�����޸���Ϣ�������ֶεļ����㷨so·��
		//encrypt_cfg["GM_SM2_PUBLIC_KEY_PATH"] = "";       // ���ù����㷨ʱ��ͨ����key���� GM�㷨���ü���ʹ�õĹ�Կ·��
		//encrypt_cfg["RSA_PUBLIC_KEY_PATH"] = "";          // ���ʹ��rsa�㷨���ܣ�ͨ����key���� rsa�㷨���ü���ʹ�õĹ�Կ·��

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

	// ����������Ϣ
	ATPConnectProperty prop;
	prop.user = _user;												// �����û���
	prop.password = _pass;											// �����û�����
	prop.locations = locations;										// ���������ڵ�ĵ�ַ+�˿�
	prop.heartbeat_interval_milli = 5000;                           // ����������ʱ��������λ������
	prop.connect_timeout_milli = 5000;                              // ���ӳ�ʱʱ�䣬��λ������
	prop.reconnect_time = 10;                                       // �������Ӵ���
	prop.client_name = "WonderTrader";                              // �ͻ��˳�������
	prop.client_version = "V1.0.0";									// �ͻ��˳���汾
	prop.report_sync = report_sync;									// �ر�ͬ�����ݷ�����+��ţ��״��ǿգ���������ʱ������ǽ��ܵ������·�����+���
	prop.mode = 0;													// ģʽ0-ͬ���ر�ģʽ��ģʽ1-���ٵ�¼ģʽ����ͬ���ر�

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
		boost::asio::io_service::work work(_asyncio);
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
	//���ﲻ��ʹ��sessionid����Ϊÿ�ε�½�᲻ͬ�����ʹ�õĻ������ܻ���ɲ�Ψһ�����
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

	// ���õ�����Ϣ
	ATPReqCustLoginOtherMsg login_msg;
	
	wt_strcpy(login_msg.fund_account_id, _fund_accountid.c_str());           // �ʽ��˻�ID
	wt_strcpy(login_msg.password, _accpasswd.c_str());                  // �ͻ�������
	strncpy(login_msg.user_info, _user.c_str(), 17);
	login_msg.login_mode = ATPCustLoginModeType::kFundAccountIDMode;	// ��¼ģʽ���ʽ��˺ŵ�¼
	login_msg.login_mode = _loginmode;  // ��¼ģʽ
	login_msg.client_seq_id = genRequestID();							// �ͻ�ϵͳ��Ϣ��
	login_msg.order_way = '0';											// ί�з�ʽ������ί��
	//login_msg.client_feature_code = productInfo;						// �ն�ʶ����
	
	login_msg.client_feature_code = _node_id;						// �ն�ʶ����

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
	wt_strcpy(logout_msg.fund_account_id, _fund_accountid.c_str());	// �ʽ��˻�ID
	logout_msg.client_seq_id = genRequestID();              // �ͻ�ϵͳ��Ϣ��
	//logout_msg.client_feature_code = _product;				// �ն�ʶ����
	
	logout_msg.client_feature_code = _node_id;				// �ն�ʶ����

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

	// �����г���Ʊ�޼�ί��
	thread_local static ATPReqCashAuctionOrderMsg p;
	//ATPReqCashAuctionOrderMsg p;
	//memset(&p, 0, sizeof(p));

	uint32_t orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);

	bool isSh = false;
	if (strcmp(entrust->getExchg(), "SSE") == 0)
		isSh = true;

	wt_strcpy(p.security_id, entrust->getCode());                   // ֤ȯ����
	p.market_id = isSh ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;             // �г�ID���Ϻ�
	wt_strcpy(p.cust_id, _cust_id.c_str());                 // �ͻ���ID
	wt_strcpy(p.fund_account_id, _fund_accountid.c_str());       // �ʽ��˻�ID

	if (isSh)
		wt_strcpy(p.account_id, _sh_acctid.c_str());                 // �˻�ID
	else
		wt_strcpy(p.account_id, _sz_acctid.c_str());

	//p.side = (entrust->getDirection() == WOT_OPEN) ? ATPSideConst::kBuy : ATPSideConst::kSell;      // ����������
	p.side = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	//p.order_type = ATPOrdTypeConst::kFixedNew;				// �������ͣ��޼�
	p.order_type = wrapOrdType(entrust->getPriceType(), entrust->getOrderFlag());
	p.price = (int32_t)(entrust->getPrice() * 10000);         // ί�м۸� N13(4)��21.0000Ԫ
	p.order_qty = (int32_t)(entrust->getVolume() * 100);            // �걨����N15(2)����ƱΪ�ɡ�����Ϊ�ݡ��Ϻ�ծȯĬ��Ϊ�ţ�ʹ��ʱ�������ȯ��ȷ�ϣ�������Ϊ�ţ�1000.00��
	p.client_seq_id = genRequestID();						// �û�ϵͳ��Ϣ���
	p.order_way = 0;										// ί�з�ʽ������ί��
	//p.client_feature_code = _product;						// �ն�ʶ����
	p.client_feature_code = _node_id;
	strncpy(p.password, _pass.c_str(), 129);							// �ͻ�����
	fmt::format_to(p.user_info, "{}", orderref);

	//const int totals = 3000;
	//for (int i = 0; i < totals; i++)
	{
		//std::cout << "order info: " << std::endl;
		std::cout << "security id: " << p.security_id << std::endl;
		std::cout << "market id: " << p.market_id << std::endl;
		//std::cout << "cust id: " << p.cust_id << std::endl;
		std::cout << "account id: " << p.account_id << std::endl;
		std::cout << "order_type: " << p.order_type << std::endl;
		std::cout << "side id: " << p.side << std::endl;
		std::cout << "order price: " << p.price << std::endl;
		std::cout << "order quantity: " << p.order_qty << std::endl;

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
	//ATPReqCancelOrderMsg p;
	//memset(&p, 0, sizeof(p));

	bool isSH = strcmp(action->getExchg(), "SSE") == 0;

	p.market_id = isSH ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;
	wt_strcpy(p.cust_id, _cust_id.c_str());                    // �ͻ���ID
	wt_strcpy(p.fund_account_id, _fund_accountid.c_str());          // �ʽ��˻�ID
	wt_strcpy(p.user_info, _user.c_str());                   // �˻�ID

	if (isSH)	
		wt_strcpy(p.account_id, _sh_acctid.c_str());  // ֤ȯ�˻�
	else
		wt_strcpy(p.account_id, _sz_acctid.c_str());  // ֤ȯ�˻�

	wt_strcpy(p.password, _accpasswd.c_str());  // ��������
	p.client_seq_id = genRequestID();
	p.order_way = '0';
	p.orig_cl_ord_no = strtoull(action->getOrderID(), NULL, 10);
	
	p.client_feature_code = _node_id;

	//write_log(_sink, LL_ERROR, "[TraderATP][{}]�˻�����������ֵ��", p.account_id);
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
	// �����ʽ��˻�
	if (!_sh_acctid.empty())
	{
		ATPReqFundQueryMsg p;

		strncpy(p.cust_id, _cust_id.c_str(), 17);
		strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
		strncpy(p.account_id, _sh_acctid.c_str(), 13);
		p.client_seq_id = genRequestID();
		strncpy(p.user_info, _user.c_str(), 17);
		strncpy(p.password, _pass.c_str(), 129);
		strncpy(p.currency, "CNY", 5);
		p.client_feature_code = _node_id;

		//std::cout << "send seq_id of order : " << p.client_seq_id << std::endl;
		//std::cout << "cust_id: " << p.cust_id << " fund_account_id: " << p.fund_account_id
		//	<< " account_id: " << p.account_id << " user: " << p.user_info << std::endl;

		ATPRetCodeType ec = _api->ReqFundQuery(&p);
		if (ec != ErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "[TraderATP][{}] Query account failed: {}", _sh_acctid, ec);
		}
	}

	// �����ʽ��˻�
	if (!_sz_acctid.empty())
	{
		ATPReqFundQueryMsg p1;

		strncpy(p1.cust_id, _cust_id.c_str(), 17);
		strncpy(p1.fund_account_id, _fund_accountid.c_str(), 17);
		strncpy(p1.account_id, _sz_acctid.c_str(), 13);
		p1.client_seq_id = genRequestID();
		strncpy(p1.user_info, _user.c_str(), 17);
		strncpy(p1.password, _pass.c_str(), 129);
		strncpy(p1.currency, "CNY", 5);
		p1.client_feature_code = _node_id;

		//std::cout << "send seq_id of order : " << p.client_seq_id << std::endl;
		//std::cout << "cust_id: " << p.cust_id << " fund_account_id: " << p.fund_account_id
		//	<< " account_id: " << p.account_id << " user: " << p.user_info << std::endl;

		ATPRetCodeType ec1 = _api->ReqFundQuery(&p1);
		if (ec1 != ErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "[TraderATP][{}] Query account failed: {}", _sz_acctid, ec1);
		}
	}

	return 0;
}

int TraderATP::queryPositions()
{
	//if (!_sh_acctid.empty())
	//{
	//	ATPReqShareQueryMsg p;
	//	strncpy(p.cust_id, _cust_id.c_str(), 17);
	//	strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
	//	strncpy(p.account_id, _sh_acctid.c_str(), 13);
	//	p.client_seq_id = genRequestID();
	//	strncpy(p.user_info, _user.c_str(), 17);
	//	strncpy(p.password, _pass.c_str(), 129);
	//	p.client_feature_code = _node_id;

	//	APIAccountIDUnit api_account_unit_sh;
	//	strncpy(api_account_unit_sh.account_id, _sh_acctid.c_str(), 13);
	//	p.account_id_array.push_back(api_account_unit_sh);

	//	//p->business_type = business_type;
	//	//p->market_id = market_id;
	//	//strncpy(p->security_id, security_id.c_str(), 9);
	//	//p->query_index = query_index;
	//	//p->return_num = return_num;

	//	ATPRetCodeType ec = _api->ReqShareQuery(&p);
	//	if (ec != ErrorCode::kSuccess)
	//	{
	//		write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _sh_acctid, ec);
	//	}
	//}

	//if (!_sz_acctid.empty())
	//{
	//	ATPReqShareQueryMsg p1;
	//	strncpy(p1.cust_id, _cust_id.c_str(), 17);
	//	strncpy(p1.fund_account_id, _fund_accountid.c_str(), 17);
	//	strncpy(p1.account_id, _sz_acctid.c_str(), 13);
	//	p1.client_seq_id = genRequestID();
	//	strncpy(p1.user_info, _user.c_str(), 17);
	//	strncpy(p1.password, _pass.c_str(), 129);
	//	p1.client_feature_code = _node_id;

	//	APIAccountIDUnit api_account_unit_sz;
	//	strncpy(api_account_unit_sz.account_id, _sz_acctid.c_str(), 13);
	//	p1.account_id_array.push_back(api_account_unit_sz);

	//	ATPRetCodeType ec = _api->ReqShareQuery(&p1);
	//	if (ec != ErrorCode::kSuccess)
	//	{
	//		write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _sz_acctid, ec);
	//	}
	//}

	/// �黦��
	_asyncio.post([this]() {
		if (!_sh_acctid.empty())
		{
			ATPReqShareQueryMsg p;
			strncpy(p.cust_id, _cust_id.c_str(), 17);
			strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p.account_id, _sh_acctid.c_str(), 13);
			p.client_seq_id = genRequestID();
			strncpy(p.user_info, _user.c_str(), 17);
			strncpy(p.password, _pass.c_str(), 129);
			p.client_feature_code = _node_id;
			p.query_index = 0;

			APIAccountIDUnit api_account_unit_sh;
			strncpy(api_account_unit_sh.account_id, _sh_acctid.c_str(), 13);
			p.account_id_array.push_back(api_account_unit_sh);

			ATPRetCodeType ec = _api->ReqShareQuery(&p);
			if (ec != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _sh_acctid, ec);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			if (_sh_total_num > 100)  // ���ҳ����Ϊ100��
			{
				// ��Ҫ��ҳ��ѯ
				if (_sh_total_num % 100 > 0)
					_sh_total_num += 100;

				int times = _sh_total_num / 100;

				for (int i = 0; i < times; i++)
				{
					p.query_index = (i + 1) * 100;

					ec = _api->ReqShareQuery(&p);
					if (ec != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _sh_acctid, ec);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		/// ������
		if (!_sz_acctid.empty())
		{
			ATPReqShareQueryMsg p1;
			strncpy(p1.cust_id, _cust_id.c_str(), 17);
			strncpy(p1.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p1.account_id, _sz_acctid.c_str(), 13);
			p1.client_seq_id = genRequestID();
			strncpy(p1.user_info, _user.c_str(), 17);
			strncpy(p1.password, _pass.c_str(), 129);
			p1.client_feature_code = _node_id;
			p1.query_index = 0;

			APIAccountIDUnit api_account_unit_sz;
			strncpy(api_account_unit_sz.account_id, _sz_acctid.c_str(), 13);
			p1.account_id_array.push_back(api_account_unit_sz);

			ATPRetCodeType ec1 = _api->ReqShareQuery(&p1);
			if (ec1 != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _sz_acctid, ec1);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			if (_sz_total_num >= 100)
			{
				// ��Ҫ��ҳ��ѯ
				if (_sz_total_num % 100 > 0)
					_sz_total_num += 100;

				int times = _sz_total_num / 100;

				for (int i = 0; i < times; i++)
				{
					p1.query_index = (i + 1) * 100;

					ATPRetCodeType ec1 = _api->ReqShareQuery(&p1);
					if (ec1 != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query positions failed: {}", _sz_acctid, ec1);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}
	});

	return 0;
}

int TraderATP::queryOrders()
{
	/// �黦��
	_asyncio.post([this](){
		if (!_sh_acctid.empty())
		{
			ATPReqOrderQueryMsg p;
			strncpy(p.cust_id, _cust_id.c_str(), 17);
			strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p.account_id, _sh_acctid.c_str(), 13);
			p.client_seq_id = genRequestID();
			p.business_abstract_type = 1;
			strncpy(p.user_info, _user.c_str(), 17);
			strncpy(p.password, _pass.c_str(), 129);
			p.client_feature_code = _node_id;
			p.query_index = 0;

			ATPRetCodeType ec = _api->ReqOrderQuery(&p);
			if (ec != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query orders failed: {}", _sh_acctid, ec);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			if (_sh_total_num > 100)
			{
				// ��Ҫ��ҳ��ѯ
				if (_sh_total_num % 100 > 0)
					_sh_total_num += 100;

				int times = _sh_total_num / 100;

				for (int i = 0; i < times; i++)
				{
					p.query_index = (i + 1) * 100;

					ec = _api->ReqOrderQuery(&p);
					if (ec != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query orders failed: {}", _sh_acctid, ec);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(300));

		/// ������
		if (!_sz_acctid.empty())
		{
			ATPReqOrderQueryMsg p1;
			strncpy(p1.cust_id, _cust_id.c_str(), 17);
			strncpy(p1.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p1.account_id, _sz_acctid.c_str(), 13);
			p1.client_seq_id = genRequestID();
			p1.business_abstract_type = 1;
			strncpy(p1.user_info, _user.c_str(), 17);
			strncpy(p1.password, _pass.c_str(), 129);
			p1.client_feature_code = _node_id;
			p1.query_index = 0;

			ATPRetCodeType ec1 = _api->ReqOrderQuery(&p1);
			if (ec1 != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query orders failed: {}", _sz_acctid, ec1);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			if (_sz_total_num >= 100)
			{
				// ��Ҫ��ҳ��ѯ
				if (_sz_total_num % 100 > 0)
					_sz_total_num += 100;

				int times = _sz_total_num / 100;

				for (int i = 0; i < times; i++)
				{
					p1.query_index = (i + 1) * 100;

					ATPRetCodeType ec1 = _api->ReqOrderQuery(&p1);
					if (ec1 != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query orders failed: {}", _sz_acctid, ec1);
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
	//// ��ѯ����
	//if (!_sh_acctid.empty())
	//{
	//	ATPReqTradeOrderQueryMsg p;
	//	strncpy(p.cust_id, _cust_id.c_str(), 17);
	//	strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
	//	strncpy(p.account_id, _sh_acctid.c_str(), 13);
	//	p.client_seq_id = genRequestID();
	//	p.business_abstract_type = 1;
	//	strncpy(p.user_info, _user.c_str(), 17);
	//	strncpy(p.password, _pass.c_str(), 129);
	//	p.client_feature_code = _product1;

	//	ATPRetCodeType ec = _api->ReqTradeOrderQuery(&p);
	//	if (ec != ErrorCode::kSuccess)
	//	{
	//		write_log(_sink, LL_ERROR, "[TraderATP] Query trades failed: {}", ec);
	//	}
	//}

	//// ��ѯ����
	//if (!_sz_acctid.empty())
	//{
	//	ATPReqTradeOrderQueryMsg p1;
	//	strncpy(p1.cust_id, _cust_id.c_str(), 17);
	//	strncpy(p1.fund_account_id, _fund_accountid.c_str(), 17);
	//	strncpy(p1.account_id, _sz_acctid.c_str(), 13);
	//	p1.client_seq_id = genRequestID();
	//	p1.business_abstract_type = 1;
	//	strncpy(p1.user_info, _user.c_str(), 17);
	//	strncpy(p1.password, _pass.c_str(), 129);
	//	p1.client_feature_code = _product1;

	//	ATPRetCodeType ec1 = _api->ReqTradeOrderQuery(&p1);
	//	if (ec1 != ErrorCode::kSuccess)
	//	{
	//		write_log(_sink, LL_ERROR, "[TraderATP] Query trades failed: {}", ec1);
	//	}
	//}

	_asyncio.post([this]() {
		if (!_sh_acctid.empty())  /// �黦��
		{
			ATPReqTradeOrderQueryMsg p;
			strncpy(p.cust_id, _cust_id.c_str(), 17);
			strncpy(p.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p.account_id, _sh_acctid.c_str(), 13);
			p.client_seq_id = genRequestID();
			p.business_abstract_type = 1;
			strncpy(p.user_info, _user.c_str(), 17);
			strncpy(p.password, _pass.c_str(), 129);
			p.client_feature_code = _node_id;
			p.query_index = 0;

			ATPRetCodeType ec = _api->ReqTradeOrderQuery(&p);
			if (ec != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query trades failed: {}", _sh_acctid, ec);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));  // �ȴ��ص����

			if (_sh_total_num > 100)
			{
				// ��Ҫ��ҳ��ѯ
				if (_sh_total_num % 100 > 0)
					_sh_total_num += 100;

				int times = _sh_total_num / 100;

				for (int i = 0; i < times; i++)
				{
					p.query_index = (i + 1) * 100;

					ec = _api->ReqTradeOrderQuery(&p);
					if (ec != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query trades failed: {}", _sh_acctid, ec);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		/// ������
		if (!_sz_acctid.empty())
		{
			ATPReqTradeOrderQueryMsg p1;
			strncpy(p1.cust_id, _cust_id.c_str(), 17);
			strncpy(p1.fund_account_id, _fund_accountid.c_str(), 17);
			strncpy(p1.account_id, _sz_acctid.c_str(), 13);
			p1.client_seq_id = genRequestID();
			p1.business_abstract_type = 1;
			strncpy(p1.user_info, _user.c_str(), 17);
			strncpy(p1.password, _pass.c_str(), 129);
			p1.client_feature_code = _node_id;
			p1.query_index = 0;

			ATPRetCodeType ec1 = _api->ReqTradeOrderQuery(&p1);
			if (ec1 != ErrorCode::kSuccess)
			{
				write_log(_sink, LL_ERROR, "[TraderATP][{}] Query trades failed: {}", _sz_acctid, ec1);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			if (_sz_total_num >= 100)
			{
				// ��Ҫ��ҳ��ѯ
				if (_sz_total_num % 100 > 0)
					_sz_total_num += 100;

				int times = _sz_total_num / 100;

				for (int i = 0; i < times; i++)
				{
					p1.query_index = (i + 1) * 100;

					ATPRetCodeType ec1 = _api->ReqTradeOrderQuery(&p1);
					if (ec1 != ErrorCode::kSuccess)
					{
						write_log(_sink, LL_ERROR, "[TraderATP][{}] Query trades failed: {}", _sz_acctid, ec1);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}
	});

	return 0;
}
#pragma endregion "ITraderApi"
