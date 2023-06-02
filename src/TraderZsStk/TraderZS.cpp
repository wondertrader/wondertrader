/*!
 * \file TraderZS.cpp
 * \project	WonderTrader
 *
 * \author Sunseeeeeeker
 * \date 2023/05/07
 *
 * \brief
 */
#include "TraderZS.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

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
		TraderZS *instance = new TraderZS();
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

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/zsTradeApi1.6.0/x64/zsTradeApi.lib")
#else
#pragma comment(lib, "../API/zsTradeApi1.6.0/x86/zsTradeApi.lib")
#endif
#endif

inline WTSOffsetType wrapOffsetType(int offType)
{
	if (100 == offType || 700 == offType || 702 == offType || 1100 == offType)  // 证券买入\质押出库\正回购\跨市场ETF申购 担保品买入  融资买入 市价买入
		return WOT_OPEN;
	else
		return WOT_CLOSE;
}

inline int wrapOffsetType(WTSOffsetType offType)
{
	if (offType == WOT_OPEN)
		return 100;  // 买入
	else
		return 101;  // 卖出
}

inline WTSPriceType wrapPriceType(int priceType)
{
	if (1100 == priceType || 1101 == priceType)
		return WPT_ANYPRICE;
	else
		return WPT_LIMITPRICE;
}

inline WTSOrderState wrapOrderState(char orderState)
{
	switch (orderState)
	{
	case 'A':  // 待报，未开市
		return WOS_NotTraded_NotQueuing;
	case '8':  // 已成
		return WOS_AllTraded;
	case '7':  // 部成
		return WOS_PartTraded_Queuing;
	case '2':  // 已报
		return WOS_NotTraded_Queuing;
	case '5':  // 部成部撤
	case '6':  // 已撤
		return WOS_Canceled;
	case '3':  // 已报待撤
	case '4':  // 部成待撤
		return WOS_Cancelling;
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


TraderZS::TraderZS()
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
	, _logined(false)
	, _acctCode(0)
{
	
}

TraderZS::~TraderZS()
{

}

int TraderZS::OnDisconnected(int p_nReason, const char *p_pszErrInfo)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, p_nReason);

	_asyncio.post([this]() {
		write_log(_sink, LL_WARN, "[TraderZS] Connection lost, relogin in 2 seconds...");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		doLogin();
	});

	return 0;
}

int TraderZS::OnConnected()
{
	_asyncio.post([this] {
		if (_sink)
		{
			write_log(_sink, LL_INFO, "[TraderZS] Connected to server");

			_sink->handleEvent(WTE_Connect, 0);
		}
	});

	return 0;
}

int TraderZS::OnAcctRegister(STFirstSet *p_pFirstSet, STRspAcctRegister *p_pRspAcctLoginState, LONGLONG  p_llReqId, int p_iNum)
{
	if (0 == p_pFirstSet->iCode && NULL != p_pRspAcctLoginState)
	{
		for (int i = 0; i < p_iNum; i++)
		{
			auto &rspData = p_pRspAcctLoginState[i];
			
			write_log(_sink, LL_INFO, "[OnAcctRegister] 资产账户: {}, 登录状态: {}, 允许策略交易: {}", rspData.llCuacctCode, rspData.chLoginStatus, rspData.iAllowStrategy);

			if (rspData.chLoginStatus == LOGIN_ONLINE)
			{
				_logined = true;
				write_log(_sink, LL_INFO, "[{}] 注册成功", rspData.llCuacctCode);
			}
			else
			{
				write_log(_sink, LL_ERROR, "[{}] 注册失败，请登录资产账户后重试", rspData.llCuacctCode);
			}
		}
	}

	return 0;
}

int TraderZS::OnRtnOrderConfirm(STRtnOrderConfirm *p_pRtnOrderConfirm)
{
	if (NULL != p_pRtnOrderConfirm)
	{
		const char* exchg = strcmp(p_pRtnOrderConfirm->szStkbd, "SH") == 0 ? "SSE" : "SZSE";
		WTSContractInfo* contract = _bd_mgr->getContract(p_pRtnOrderConfirm->szStkCode, exchg);
		if (contract == NULL)
			return 0;

		WTSOrderInfo* orderInfo = WTSOrderInfo::create();
		orderInfo->setContractInfo(contract);
		orderInfo->setPrice(atof(p_pRtnOrderConfirm->szOrderId));
		orderInfo->setVolume((uint32_t)p_pRtnOrderConfirm->llOrderQty);
		//orderInfo->setDirection(wrapDirectionType(p_pRtnOrderConfirm->iStkBiz));
		orderInfo->setDirection(WDT_LONG);
		orderInfo->setPriceType(WPT_LIMITPRICE);
		orderInfo->setOrderFlag(WOF_NOR);
		orderInfo->setOffsetType(wrapOffsetType(p_pRtnOrderConfirm->iStkBiz));

		orderInfo->setCode(p_pRtnOrderConfirm->szStkCode);
		orderInfo->setExchange(contract->getExchg());

		orderInfo->setOrderDate(TimeUtils::getCurDate());
		orderInfo->setOrderTime(TimeUtils::getLocalTimeNow());

		orderInfo->setOrderState(wrapOrderState(p_pRtnOrderConfirm->chOrderStatus));
		if (p_pRtnOrderConfirm->chOrderStatus == '9')
			orderInfo->setError(true);

		genEntrustID(orderInfo->getEntrustID(), p_pRtnOrderConfirm->iOrderBsn);
		fmtutil::format_to(orderInfo->getOrderID(), "{}", p_pRtnOrderConfirm->szOrderId);

		orderInfo->setStateMsg("");

		const char* usertag = m_eidCache.get(orderInfo->getEntrustID());
		if (strlen(usertag) == 0)
		{
			orderInfo->setUserTag(orderInfo->getEntrustID());
		}
		else
		{
			orderInfo->setUserTag(usertag);

			if (strlen(orderInfo->getOrderID()) > 0)
			{
				m_oidCache.put(StrUtil::trim(orderInfo->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
					write_log(_sink, LL_ERROR, message);
				});
			}
		}

		{
			if (_sink)
				_sink->onPushOrder(orderInfo);

			orderInfo->release();
		}
	}

	return 0;
}

int TraderZS::OnRtnOrderFill(STRtnOrderFill *p_pRtnOrderFill)
{
	if (p_pRtnOrderFill != NULL)
	{
		write_log(_sink, LL_DEBUG, "[OnRtnOrderFill] code: {}, trading price: {}, trading qty: {}, order qty: {}, holding qty: {}, stkbiz: {}, matched type: {}, matchedsn: {}, orderid: {}, orderbsn: {}",
			p_pRtnOrderFill->szStkCode, p_pRtnOrderFill->szMatchedPrice, p_pRtnOrderFill->llMatchedQty, p_pRtnOrderFill->llOrderQty, p_pRtnOrderFill->llStkQty, p_pRtnOrderFill->iStkBiz, p_pRtnOrderFill->chMatchedType, p_pRtnOrderFill->szMatchedSn, p_pRtnOrderFill->szOrderId, p_pRtnOrderFill->iOrderBsn);

		const char* exchg = strcmp(p_pRtnOrderFill->szStkbd, "SH") == 0 ? "SSE" : "SZSE";
		WTSContractInfo* contract = _bd_mgr->getContract(p_pRtnOrderFill->szStkCode, exchg);
		if (contract == NULL)
			return 0;

		WTSTradeInfo *trdInfo = WTSTradeInfo::create(p_pRtnOrderFill->szStkCode, contract->getExchg());
		trdInfo->setVolume(p_pRtnOrderFill->llMatchedQty);
		trdInfo->setPrice(atof(p_pRtnOrderFill->szMatchedPrice));
		trdInfo->setTradeID(p_pRtnOrderFill->szMatchedSn);
		trdInfo->setContractInfo(contract);

		std::string strTime = p_pRtnOrderFill->szMatchedTime;
		StrUtil::replace(strTime, ":", "");
		uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
		uint32_t uDate = p_pRtnOrderFill->iMatchedDate;

		//如果是夜盘时间，并且成交日期等于交易日，说明成交日期是需要修正
		//因为夜盘是提前的，成交日期必然小于交易日
		//但是这里只能做一个简单修正了
		if (uTime >= 210000 && uDate == _tradingday)
		{
			uDate = TimeUtils::getNextDate(uDate, -1);
		}

		trdInfo->setTradeDate(uDate);
		trdInfo->setTradeTime(TimeUtils::makeTime(uDate, uTime * 1000));

		//WTSDirectionType dType = wrapDirectionType(p_pRtnOrderFill->iStkBiz);
		//trdInfo->setDirection(dType);
		trdInfo->setDirection(WDT_LONG);
		trdInfo->setOffsetType(wrapOffsetType(p_pRtnOrderFill->iStkBiz));
		trdInfo->setRefOrder(p_pRtnOrderFill->szOrderId);
		trdInfo->setTradeType(WTT_Common);

		trdInfo->setAmount(atof(p_pRtnOrderFill->szMatchedAmt));

		const char* usertag = m_oidCache.get(StrUtil::trim(trdInfo->getRefOrder()).c_str());
		if (strlen(usertag))
			trdInfo->setUserTag(usertag);

		if (_sink)
			_sink->onPushTrade(trdInfo);

		trdInfo->release();
	}

	return 0;
}

int TraderZS::OnOrder(STFirstSet *p_pFirstSet, STRspOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		write_log(_sink, LL_DEBUG, "[OnOrder] code: {}, price: {}, qty: {}, amount: {}, order bsn: {}, orderid: {}", p_pRsp->szStkCode, p_pRsp->szOrderPrice, p_pRsp->llOrderQty, p_pRsp->szOrderAmt, p_pRsp->iOrderBsn, p_pRsp->szOrderId);

		const char* exchg = strcmp(p_pRsp->szStkbd, "SH") == 0 ? "SSE" : "SZSE";
		WTSContractInfo* contract = _bd_mgr->getContract(p_pRsp->szStkCode, exchg);
		if (contract == NULL)
			return 0;

		WTSOrderInfo* orderInfo = WTSOrderInfo::create();
		orderInfo->setContractInfo(contract);
		orderInfo->setPrice(atof(p_pRsp->szOrderId));
		orderInfo->setVolume((uint32_t)p_pRsp->llOrderQty);
		//orderInfo->setDirection(wrapDirectionType(p_pRtnOrderConfirm->iStkBiz));
		orderInfo->setDirection(WDT_LONG);
		orderInfo->setPriceType(WPT_LIMITPRICE);
		orderInfo->setOrderFlag(WOF_NOR);
		orderInfo->setOffsetType(wrapOffsetType(p_pRsp->iStkBiz));

		orderInfo->setCode(p_pRsp->szStkCode);
		orderInfo->setExchange(contract->getExchg());

		orderInfo->setOrderDate(TimeUtils::getCurDate());
		orderInfo->setOrderTime(TimeUtils::getLocalTimeNow());

		//orderInfo->setOrderState(wrapOrderState(p_pRsp->chOrderStatus));
		//if (p_pRsp->chOrderStatus == '9')
		//	orderInfo->setError(true);

		genEntrustID(orderInfo->getEntrustID(), p_pRsp->iOrderBsn);
		fmtutil::format_to(orderInfo->getOrderID(), "{}", p_pRsp->szOrderId);

		orderInfo->setStateMsg("");

		const char* usertag = m_eidCache.get(orderInfo->getEntrustID());
		if (strlen(usertag) == 0)
		{
			orderInfo->setUserTag(orderInfo->getEntrustID());
		}
		else
		{
			orderInfo->setUserTag(usertag);

			if (strlen(orderInfo->getOrderID()) > 0)
			{
				m_oidCache.put(StrUtil::trim(orderInfo->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
					write_log(_sink, LL_ERROR, message);
				});
			}
		}

		{
			if (_sink)
				_sink->onPushOrder(orderInfo);

			orderInfo->release();
		}
	}
	else if (NULL != p_pRsp)
	{
		const char* code = p_pRsp->szStkCode;
		const char* exchg = strcmp(p_pRsp->szStkbd, "SH") == 0 ? "SSE" : "SZSE";

		WTSContractInfo* ct = _bd_mgr->getContract(code, exchg);
		if (ct == NULL)
			return 0;

		WTSEntrust* entrust = WTSEntrust::create(
			code,
			(uint32_t)p_pRsp->llOrderQty,
			atof(p_pRsp->szOrderPrice),
			ct->getExchg());
		entrust->setContractInfo(ct);
		//pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
		entrust->setDirection(WDT_LONG);
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setOffsetType(wrapOffsetType(p_pRsp->iStkBiz));
		entrust->setOrderFlag(WOF_NOR);

		genEntrustID(entrust->getEntrustID(), p_pRsp->iOrderBsn);

		const char* usertag = m_eidCache.get(entrust->getEntrustID());
		if (strlen(usertag) > 0)
			entrust->setUserTag(usertag);

		WTSError* error = WTSError::create(WEC_ORDERINSERT, p_pFirstSet->szText);
		_sink->onRspEntrust(entrust, error);
		
		error->release();
		entrust->release();
	}

	return 0;
}

int TraderZS::OnCancelOrder(STFirstSet *p_pFirstSet, STRspCancelOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		write_log(_sink, LL_DEBUG, "[OnCancelOrder] code: {}, price: {}, qty: {}, amount: {}, order bsn: {}, orderid: {}", p_pRsp->szStkCode, p_pRsp->szOrderPrice, p_pRsp->llOrderQty, p_pRsp->szOrderAmt, p_pRsp->iOrderBsn, p_pRsp->szOrderId);

		const char* exchg = strcmp(p_pRsp->szStkbd, "SH") == 0 ? "SSE" : "SZSE";
		WTSContractInfo* contract = _bd_mgr->getContract(p_pRsp->szStkCode, exchg);
		if (contract == NULL)
			return 0;

		WTSOrderInfo* orderInfo = WTSOrderInfo::create();
		orderInfo->setContractInfo(contract);
		orderInfo->setPrice(atof(p_pRsp->szOrderPrice));
		orderInfo->setVolume((uint32_t)p_pRsp->llOrderQty);
		//orderInfo->setDirection(wrapDirectionType(p_pRtnOrderConfirm->iStkBiz));
		orderInfo->setDirection(WDT_LONG);
		orderInfo->setPriceType(WPT_LIMITPRICE);
		orderInfo->setOrderFlag(WOF_NOR);
		orderInfo->setOffsetType(wrapOffsetType(p_pRsp->iStkBiz));

		orderInfo->setCode(p_pRsp->szStkCode);
		orderInfo->setExchange(contract->getExchg());

		orderInfo->setOrderDate(TimeUtils::getCurDate());
		orderInfo->setOrderTime(TimeUtils::getLocalTimeNow());

		//orderInfo->setOrderState(wrapOrderState(p_pRsp->chOrderStatus));
		//if (p_pRsp->chOrderStatus == '9')
		//	orderInfo->setError(true);

		genEntrustID(orderInfo->getEntrustID(), p_pRsp->iOrderBsn);
		fmtutil::format_to(orderInfo->getOrderID(), "{}", p_pRsp->szOrderId);

		orderInfo->setStateMsg(p_pRsp->szMsgOk);

		const char* usertag = m_eidCache.get(orderInfo->getEntrustID());
		if (strlen(usertag) == 0)
		{
			orderInfo->setUserTag(orderInfo->getEntrustID());
		}
		else
		{
			orderInfo->setUserTag(usertag);

			if (strlen(orderInfo->getOrderID()) > 0)
			{
				m_oidCache.put(StrUtil::trim(orderInfo->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
					write_log(_sink, LL_ERROR, message);
				});
			}
		}

		{
			if (_sink)
				_sink->onPushOrder(orderInfo);

			orderInfo->release();
		}
	}
	else if (NULL != p_pRsp)
	{
		const char* code = p_pRsp->szStkCode;
		const char* exchg = strcmp(p_pRsp->szStkbd, "SH") == 0 ? "SSE" : "SZSE";

		WTSContractInfo* ct = _bd_mgr->getContract(code, exchg);
		if (ct == NULL)
			return 0;

		WTSEntrust* entrust = WTSEntrust::create(
			code,
			(uint32_t)p_pRsp->llOrderQty,
			atof(p_pRsp->szOrderPrice),
			ct->getExchg());
		entrust->setContractInfo(ct);
		//pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
		entrust->setDirection(WDT_LONG);
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setOffsetType(wrapOffsetType(p_pRsp->iStkBiz));
		entrust->setOrderFlag(WOF_NOR);

		genEntrustID(entrust->getEntrustID(), p_pRsp->iOrderBsn);

		const char* usertag = m_eidCache.get(entrust->getEntrustID());
		if (strlen(usertag) > 0)
			entrust->setUserTag(usertag);

		WTSError* error = WTSError::create(WEC_ORDERCANCEL, p_pFirstSet->szText);
		_sink->onRspEntrust(entrust, error);

		error->release();
		entrust->release();

		//WTSError* error = WTSError::create(WEC_ORDERCANCEL, p_pFirstSet->szText);
		//_sink->onTraderError(error);
		//error->release();
	}

	return 0;
}

int TraderZS::OnQryOrder(STFirstSet *p_pFirstSet, STRspQryOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (p_pFirstSet->iCode == 100)
	{
		write_log(_sink, LL_WARN, "[TraderZS] 返回内容[无记录]");
	}

	if (NULL == _orders)
		_orders = WTSArray::create();

	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryOrder] code: {}, order status: {}, qty: {}, withdraw qty: {}, trade qty: {}, orderid: {}, raw orderid: {}, orderbsn: {}", p_pRsp[i].szStkCode, p_pRsp[i].chOrderStatus, p_pRsp[i].llOrderQty, p_pRsp[i].llWithdrawnQty, p_pRsp[i].llMatchedQty, p_pRsp[i].szOrderId, p_pRsp[i].szRawOrderId, p_pRsp[i].iOrderBsn);

			const char* code = p_pRsp[i].szStkCode;
			const char* exchg = strcmp(p_pRsp[i].szStkbd, "SH") == 0 ? "SSE" : "SZSE";

			WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);
			if (contract != NULL)
			{
				WTSOrderInfo* orderInfo = WTSOrderInfo::create();

				orderInfo->setContractInfo(contract);
				orderInfo->setPrice(atof(p_pRsp[i].szOrderPrice));
				orderInfo->setVolume(p_pRsp[i].llOrderQty);
				//orderInfo->setDirection(wrapDirectionType(p_pRsp[i].iStkBiz));
				orderInfo->setDirection(WDT_LONG);
				orderInfo->setOffsetType(wrapOffsetType(p_pRsp[i].iStkBiz));
				orderInfo->setOrderFlag(WOF_NOR);
				orderInfo->setPriceType(WPT_LIMITPRICE);

				orderInfo->setVolTraded((uint32_t)p_pRsp[i].llMatchedQty);
				orderInfo->setVolLeft(p_pRsp[i].llOrderQty - p_pRsp[i].llMatchedQty);

				orderInfo->setCode(code);
				orderInfo->setExchange(contract->getExchg());

				orderInfo->setOrderDate((atol(p_pRsp[i].szOrderTime)) / 1000000000);
				uint32_t uTime = (atol(p_pRsp[i].szOrderTime)) % 1000000000;
				orderInfo->setOrderTime(TimeUtils::makeTime(orderInfo->getOrderDate(), uTime));

				orderInfo->setOrderState(wrapOrderState(p_pRsp[i].chOrderStatus));
				if (p_pRsp[i].chOrderStatus == '9')
					orderInfo->setError(true);

				genEntrustID(orderInfo->getEntrustID(), atol(p_pRsp[i].szRawOrderId));
				fmtutil::format_to(orderInfo->getOrderID(), "{}", p_pRsp[i].szOrderId);

				orderInfo->setStateMsg(p_pFirstSet->szText);

				const char* usertag = m_eidCache.get(orderInfo->getEntrustID());
				if (strlen(usertag) == 0)
				{
					orderInfo->setUserTag(orderInfo->getEntrustID());
				}
				else
				{
					orderInfo->setUserTag(usertag);

					if (strlen(orderInfo->getOrderID()) > 0)
					{
						m_oidCache.put(StrUtil::trim(orderInfo->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
							write_log(_sink, LL_ERROR, message);
						});
					}
				}

				_orders->append(orderInfo, false);
			}
		}
	}

	if (_sink)
		_sink->onRspOrders(_orders);

	if (_orders)
		_orders->clear();

	return 0;
}

int TraderZS::OnQryFill(STFirstSet *p_pFirstSet, STRspQryFill *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (p_pFirstSet->iCode == 100)
	{
		write_log(_sink, LL_WARN, "[TraderZS] 返回内容[无记录]");
	}

	if (NULL == _trades)
		_trades = WTSArray::create();

	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryFill] code: {}, order status: {}, qty: {}, trade qty: {}, order price: {}, trading price: {}, orderid: {}, matched sn: {}, orderbsn: {}", p_pRsp[i].szStkCode, p_pRsp[i].chOrderStatus, p_pRsp[i].llOrderQty, p_pRsp[i].szMatchedQty, p_pRsp[i].szOrderPrice, p_pRsp[i].szMatchedPrice, p_pRsp[i].szOrderId, p_pRsp[i].szMatchedSn, p_pRsp[i].iOrderBsn);

			const char* code = p_pRsp[i].szStkCode;
			const char* exchg = strcmp(p_pRsp[i].szStkbd, "SH") == 0 ? "SSE" : "SZSE";

			WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);

			if (contract != NULL)
			{
				WTSTradeInfo *trdInfo = WTSTradeInfo::create(code, exchg);
				trdInfo->setVolume(atof(p_pRsp[i].szMatchedQty));  // 成交数量
				trdInfo->setPrice(atof(p_pRsp[i].szMatchedPrice));  // 成交价格
				trdInfo->setTradeID(p_pRsp[i].szMatchedSn);  // 成交编号
				trdInfo->setContractInfo(contract);

				uint32_t uTime = (uint32_t)(p_pRsp[i].szMatchedTime);
				uint32_t uDate = (uint32_t)(p_pRsp[i].iOrderDate);

				trdInfo->setTradeDate(uDate);
				trdInfo->setTradeTime(TimeUtils::makeTime(uDate, uTime));

				//WTSDirectionType dType = wrapDirectionType(p_pRsp[i].iStkBiz);
				trdInfo->setDirection(WDT_LONG);

				trdInfo->setOffsetType(wrapOffsetType(p_pRsp[i].iStkBiz));
				fmtutil::format_to(trdInfo->getRefOrder(), "{}", p_pRsp[i].szOrderId);  // 合同序号
				trdInfo->setTradeType(WTT_Common);

				trdInfo->setAmount(atof(p_pRsp[i].szMatchedAmt));

				const char* usertag = m_oidCache.get(StrUtil::trim(trdInfo->getRefOrder()).c_str());
				if (strlen(usertag))
					trdInfo->setUserTag(usertag);

				_trades->append(trdInfo, false);
			}
		}
	}

	{
		if (_sink)
			_sink->onRspTrades(_trades);

		if (_trades)
			_trades->clear();
	}

	return 0;
}

int TraderZS::OnQryHolding(STFirstSet *p_pFirstSet, STRspQryHolding *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (NULL == _positions)
		_positions = PositionMap::create();

	if (p_pFirstSet->iCode == 100)
	{
		write_log(_sink, LL_WARN, "[TraderZS] 返回内容[无记录]");
	}

	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryHolding] code: {},avail qty: {}, left qty: {}, frz qty: {}, holding qty: {}, trd qty: {}, cost price: {}, buy cost price: {}", p_pRsp[i].szStkCode, p_pRsp[i].llStkAvl, p_pRsp[i].llStkBln, p_pRsp[i].llStkFrz, p_pRsp[i].llStkAvl, p_pRsp[i].llStkTrdOtd, p_pRsp[i].szCostPrice, p_pRsp[i].szStkBcostRlt);

			const char* exchg = (strcmp(p_pRsp[i].szStkbd, "SH") == 0) ? "SSE" : "SZSE";
			const char* code = p_pRsp[i].szStkCode;

			WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);
			if (contract)
			{
				WTSCommodityInfo* commInfo = contract->getCommInfo();
				std::string key = fmt::format("{}-{}", code, WDT_LONG);

				WTSPositionItem* pos = (WTSPositionItem*)_positions->get(key);

				if (pos == NULL)
				{
					pos = WTSPositionItem::create(code, commInfo->getCurrency(), commInfo->getExchg());
					pos->setContractInfo(contract);
					_positions->add(key, pos, false);
				}

				pos->setDirection(WDT_LONG);

				// 今持 = 总持 - （昨持 - 平昨）= 总持 - 昨可用
				pos->setNewPosition((double)(p_pRsp[i].llStkQty - p_pRsp[i].llStkAvl));  
				pos->setPrePosition((double)p_pRsp[i].llStkBln);  // 证券余额是指的是股票昨日余额

				pos->setMargin(atof(p_pRsp[i].szMktVal));
				pos->setDynProfit(atof(p_pRsp[i].szProIncome));
				pos->setPositionCost(atof(p_pRsp[i].szStkBcostRlt));

				pos->setAvgPrice(atof(p_pRsp[i].szCostPrice));

				pos->setAvailNewPos(0);  // 股票是T+1，今可用为0
				pos->setAvailPrePos((double)p_pRsp[i].llStkAvl);
			}
		}
	}

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

	return 0;
}

int TraderZS::OnQryMoney(STFirstSet *p_pFirstSet, STRspQryMoney *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		WTSArray * ay = WTSArray::create();

		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryMoney] acct: {}, avail: {}, balance: {}", p_pRsp[i].llCuacctCode, p_pRsp[i].szFundAvl, p_pRsp[i].szFundBln);

			WTSAccountInfo* accountInfo = WTSAccountInfo::create();

			accountInfo->setPreBalance(0);
			accountInfo->setCloseProfit(0);
			accountInfo->setDynProfit(0);
			accountInfo->setMargin(0);
			accountInfo->setAvailable(atof(p_pRsp[i].szFundAvl));
			accountInfo->setFrozenMargin(atof(p_pRsp[i].szFundTrdOtd));
			accountInfo->setFrozenCommission(0);
			accountInfo->setBalance(atof(p_pRsp[i].szFundBln));
			accountInfo->setCurrency("CNY");

			ay->append(accountInfo, false);
		}

		if (_sink)
			_sink->onRspAccount(ay);

		ay->release();
	}
	else
	{
		write_log(_sink, LL_ERROR, "[OnQryMoney] code: {}, msg: {}", p_pFirstSet->iCode, p_pFirstSet->szText);
	}

	return 0;
}

int TraderZS::OnQryHolder(STFirstSet *p_pFirstSet, STRspQryHolder *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (p_pFirstSet->iCode == 0 && p_pRsp != NULL)
	{
		for (int i = 0; i < p_iNum; i++)
			write_log(_sink, LL_DEBUG, "[OnQryHolder] asset fund: {}, stk bd: {}, trd acct: {}", p_pRsp[i].llCuacctCode, p_pRsp[i].szStkbd, p_pRsp[i].szTrdacct);
	}

	return 0;
}

#pragma region "ITraderApi"
bool TraderZS::init(WTSVariant *params)
{
	_user = params->getCString("user");
	_pass = params->getCString("pass");

	_host = params->getCString("host");
	_port = params->getInt32("port");

	_acctCode = params->getInt64("asset_acct");

	_flowdir = params->getCString("flowdir");

	if (_flowdir.empty())
		_flowdir = "zsTrade";

	_flowdir = StrUtil::standardisePath(_flowdir);

	std::string module = params->getCString("zsmodule");
	if (module.empty()) module = "zsTradeApi";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "");;

	m_hInstZS = DLLHelper::load_library(dllpath.c_str());

	return true;
}

void TraderZS::release()
{
	if (_api)
	{
		_api->Exit();
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

void TraderZS::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderZS::reconnect()
{
	if (_api)
	{
		_api->Exit();
		_api = NULL;
	}

	std::stringstream ss;
	ss << _flowdir << "flows/" << _user << "/";
	boost::filesystem::create_directories(ss.str().c_str());

	// 创建UserApi
	_api = new CTradeApi;
	_api->RegisterCallback(this);

	// 注册api
	_api->RegisterServer(_host.c_str(), _port, WAITRSP_TIMEOUT);
	int iRet = _api->Init();

	if (iRet != 0)
	{
		if (_sink)
			_sink->handleEvent(WTE_Connect, -1);

		write_log(_sink, LL_ERROR, "[TraderZS] zsTradeApi init failed, {}", _api->GetLastError());

		StdThreadPtr thrd(new StdThread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			write_log(_sink, LL_WARN, "[TraderZS] {} reconnecting...", _user.c_str());
			reconnect();
		}));
	}
	else
		write_log(_sink, LL_INFO, "[TraderZS] zsTradeApi init success");

}

void TraderZS::connect()
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

void TraderZS::disconnect()
{
	release();
}

bool TraderZS::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderZS::genEntrustID(char* buffer, uint32_t orderRef)
{
	//这里不再使用sessionid，因为每次登陆会不同，如果使用的话，可能会造成不唯一的情况
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderZS::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderZS::makeEntrustID(char* buffer, int length)
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

void TraderZS::doLogin()
{
	_state = TS_LOGINING;

	int iResult = 0;
	while (1)
	{
		//_asyncio.post([this, &iResult] {
		//	STReqAcctRegister stReq = { 0 };
		//	strncpy(stReq.szUserName, _user.c_str(), sizeof(stReq.szUserName));
		//	stReq.llCuacctCode = _acctCode;

		//	iResult = _api->AcctRegister(&stReq, genRequestID());

		//	std::this_thread::sleep_for(std::chrono::seconds(1));
		//});

		STReqAcctRegister stReq = { 0 };
		strncpy(stReq.szUserName, _user.c_str(), sizeof(stReq.szUserName));
		stReq.llCuacctCode = _acctCode;

		write_log(_sink, LL_DEBUG, "[{}][{}] logining ...", stReq.szUserName, stReq.llCuacctCode);

		iResult = _api->AcctRegister(&stReq, genRequestID());

		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (_logined)
			break;
	}

	//int iResult = _api->AcctRegister(&stReq, genRequestID());
	if (iResult)
	{
		std::string msg = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZS] Login failed: {}", msg);
		_state = TS_LOGINFAILED;
		_asyncio.post([this, msg] {
			_sink->onLoginResult(false, msg.c_str(), 0);
		});
	}
	else
	{
		if (!_inited)
		{
			_tradingday = TimeUtils::getCurDate();

			{
				//初始化委托单缓存器
				std::stringstream ss;
				ss << "./zsdata/local/";
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
				ss << "./zsdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_oid.sc";
				m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			write_log(_sink, LL_INFO, "[TraderZS] [{}] Login succeed, trading date: {}...", _user.c_str(), _tradingday);

			_state = TS_LOGINED;
			_inited = true;
			_asyncio.post([this] {
				_sink->onLoginResult(true, 0, _tradingday);
				_state = TS_ALLREADY;
			});
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderZS] [{}] Connection recovered", _user.c_str());
			_state = TS_ALLREADY;
		}
	}
}

int TraderZS::login(const char* user, const char* pass, const char* productInfo)
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

int TraderZS::logout()
{
	if (_api == NULL)
		return -1;

	_api->Exit();
	return 0;
}

int TraderZS::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	STReqOrder stReq = { 0 };
	stReq.llCuacctCode = _acctCode;

	uint32_t orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);

	strncpy(stReq.szStkCode, entrust->getCode(), sizeof(stReq.szStkCode));
	strncpy(stReq.szStkbd, wt_stricmp(entrust->getExchg(), "SSE") == 0 ? "SH" : "SZ", sizeof(stReq.szStkbd));
	strncpy(stReq.szOrderPrice, std::to_string(entrust->getPrice()).c_str(), sizeof(stReq.szOrderPrice));
	stReq.llOrderQty = (long long)entrust->getVolume();
	stReq.iStkBiz = wrapOffsetType(entrust->getOffsetType());

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
			write_log(_sink, LL_WARN, message);
		});
	}

	int iResult = _api->Order(&stReq, genRequestID());
	if (iResult)
	{
		std::string msg = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZS] Order inserting failed: {}", msg);
	}

	return 0;
}

int TraderZS::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	STReqCancelOrder stReq = { 0 };
	stReq.llCuacctCode = _acctCode;
	strncpy(stReq.szStkbd, wt_stricmp(action->getExchg(), "SSE") == 0 ? "SH" : "SZ", sizeof(stReq.szStkbd));
	strncpy(stReq.szOrderId, action->getOrderID(), sizeof(stReq.szOrderId));

	int iResult = _api->CancelOrder(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZS] Order cancelling failed: {}", error_info);
	}

	return 0;
}

uint32_t TraderZS::genRequestID()
{
	return _reqid.fetch_add(1) + 1;
}

int TraderZS::queryAccount()
{
	STReqQryMoney stReq = { 0 };
	stReq.llCuacctCode = _acctCode;

	int iResult = _api->QryMoney(&stReq, genRequestID());
	if (iResult != 0)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZS] Account querying failed: {}", error_info);
	}

	return 0;
}

int TraderZS::queryPositions()
{
	STReqQryHolding stReq = { 0 };
	stReq.llCuacctCode = _acctCode;

	int iResult = _api->QryHolding(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZS] Positions querying failed: {}", error_info);
	}

	return 0;
}

int TraderZS::queryOrders()
{
	STReqQryOrder stReq = { 0 };
	stReq.llCuacctCode = _acctCode;
	strncpy(stReq.szStkCode, "", sizeof(stReq.szStkCode));  // 查询全部

	int iResult = _api->QryOrder(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZS] Orders querying failed: {}", error_info);
	}

	return 0;
}

int TraderZS::queryTrades()
{
	STReqQryFill stReq = { 0 };
	stReq.llCuacctCode = _acctCode;
	strncpy(stReq.szStkCode, "", sizeof(stReq.szStkCode));  // 查询全部请输A

	int iResult = _api->QryFill(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZS] Trades querying failed: {}", error_info);
	}

	return 0;
}
#pragma endregion "ITraderApi"