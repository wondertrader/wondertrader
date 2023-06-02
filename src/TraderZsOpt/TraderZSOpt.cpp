/*!
 * \file TraderZSOpt.cpp
 * \project	WonderTrader
 *
 * \author Sunseeeeeeker
 * \date 2023/05/08
 *
 * \brief
 */
#include "TraderZSOpt.h"

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
		TraderZSOpt *instance = new TraderZSOpt();
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
//#include <wtypes.h>
//HMODULE	g_dllModule = NULL;
//
//BOOL APIENTRY DllMain(
//	HANDLE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved
//)
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//		g_dllModule = (HMODULE)hModule;
//		break;
//	}
//	return TRUE;
//}
//#else
//#include <dlfcn.h>
//
//char PLATFORM_NAME[] = "UNIX";
//
//std::string	g_moduleName;
//
//const std::string& getInstPath()
//{
//	static std::string moduleName;
//	if (moduleName.empty())
//	{
//		Dl_info dl_info;
//		dladdr((void *)inst_hlp, &dl_info);
//		moduleName = dl_info.dli_fname;
//	}
//
//	return moduleName;
//}
#endif

inline int wrapDirectionType(WTSDirectionType dType, WTSOffsetType offType)
{
	if (dType == WDT_LONG)
		if (offType == WOT_OPEN)
			return 400;  // 买入开仓
		else
			return 401;  // 卖出平仓
	else
		if (offType == WOT_OPEN)
			return 402;  // 卖出开仓
		else
			return 403;  // 买入平仓
}

inline WTSDirectionType wrapDirectionType(int stkBiz)
{
	if (stkBiz == 400 || stkBiz == 403)
		return WDT_LONG;
	else if (stkBiz == 402 || stkBiz == 401)
		return WDT_SHORT;
	else
		return WDT_NET;
}

inline WTSOffsetType wrapOffsetType(int offType)
{
	if (400 == offType || 402 == offType)
		return WOT_OPEN;
	else if (401 == offType || 403 == offType)
		return WOT_CLOSE;
	else
		return WOT_FORCECLOSE;
}

inline int wrapPriceType(WTSPriceType priceType, bool isSH /* = false */)
{
	if (WPT_ANYPRICE == priceType)
		return isSH ? 132 : 124;
	else if (WPT_LIMITPRICE == priceType)
		return 130;
	else if (WPT_BESTPRICE == priceType)
		return isSH ? 132 : 123;
	else
		return 131;
}

inline WTSDirectionType wrapPosDirection(char dirType)
{
	if ('L' == dirType)
		return WDT_LONG;
	else
		return WDT_SHORT;
}

inline WTSPriceType wrapPriceType(int priceType)
{
	if (132 == priceType || 133 == priceType || 134 == priceType || 122 == priceType || 125 == priceType)
		return WPT_ANYPRICE;
	else if (130 == priceType || 131 == priceType)
		return WPT_LIMITPRICE;
	else if (121 == priceType || 123 == priceType || 124 == priceType)
		return WPT_BESTPRICE;
	else
		return WPT_LASTPRICE;
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

TraderZSOpt::TraderZSOpt()
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

TraderZSOpt::~TraderZSOpt()
{

}

int TraderZSOpt::OnDisconnected(int p_nReason, const char *p_pszErrInfo)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, p_nReason);

	_asyncio.post([this]() {
		write_log(_sink, LL_WARN, "[TraderZSOpt] Connection lost, relogin in 2 seconds...");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		doLogin();
	});

	return 0;
}

int TraderZSOpt::OnConnected()
{
	_asyncio.post([this] {
		if (_sink)
		{
			write_log(_sink, LL_INFO, "[TraderZSOpt] Connected to server");

			_sink->handleEvent(WTE_Connect, 0);
		}
	});

	return 0;
}

int TraderZSOpt::OnAcctRegister(STFirstSet *p_pFirstSet, STRspAcctRegister *p_pRspAcctLoginState, LONGLONG  p_llReqId, int p_iNum)
{
	//write_log(_sink, LL_INFO, "交易账号登录状态查询及注册响应: {} [{}]", p_pFirstSet->iCode, p_pFirstSet->szText);

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

int TraderZSOpt::OnRtnOrderConfirm(STRtnOrderConfirm *p_pRtnOrderConfirm)
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
		orderInfo->setDirection(wrapDirectionType(p_pRtnOrderConfirm->iStkBiz));
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

int TraderZSOpt::OnRtnOrderFill(STRtnOrderFill *p_pRtnOrderFill)
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

		trdInfo->setDirection(wrapDirectionType(p_pRtnOrderFill->iStkBiz));
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

int TraderZSOpt::OnOrderOpt(STFirstSet *p_pFirstSet, STRspOrderOpt *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		write_log(_sink, LL_DEBUG, "[OnOrder] code: {}, price: {}, qty: {}, amount: {}, order bsn: {}, orderid: {}", p_pRsp->szOptNum, p_pRsp->szOrderPrice, p_pRsp->llOrderQty, p_pRsp->szOrderAmt, p_pRsp->iOrderBsn, p_pRsp->szOrderId);

		const char* exchg = strcmp(p_pRsp->szStkbd, "SH") == 0 ? "SSE" : "SZSE";
		WTSContractInfo* contract = _bd_mgr->getContract(p_pRsp->szOptNum, exchg);
		if (contract == NULL)
			return 0;

		WTSOrderInfo* orderInfo = WTSOrderInfo::create();
		orderInfo->setContractInfo(contract);
		orderInfo->setPrice(atof(p_pRsp->szOrderId));
		orderInfo->setVolume((uint32_t)p_pRsp->llOrderQty);
		orderInfo->setDirection(wrapDirectionType(p_pRsp->iStkBiz));
		orderInfo->setPriceType(wrapPriceType(p_pRsp->iStkBizAction));
		orderInfo->setOrderFlag(WOF_NOR);
		orderInfo->setOffsetType(wrapOffsetType(p_pRsp->iStkBiz));

		orderInfo->setCode(p_pRsp->szOptNum);
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
		const char* code = p_pRsp->szOptNum;
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
		entrust->setDirection(wrapDirectionType(p_pRsp->iStkBiz));
		entrust->setPriceType(wrapPriceType(p_pRsp->iStkBizAction));
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

int TraderZSOpt::OnOptCancelOrder(STFirstSet *p_pFirstSet, STRspOptCancelOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
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
		orderInfo->setDirection(wrapDirectionType(p_pRsp->iStkBiz));
		orderInfo->setPriceType(wrapPriceType(p_pRsp->iStkBizAction));
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
		entrust->setDirection(wrapDirectionType(p_pRsp->iStkBiz));
		entrust->setPriceType(wrapPriceType(p_pRsp->iStkBizAction));
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

int TraderZSOpt::OnOptQryOrder(STFirstSet *p_pFirstSet, STRspOptQryOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (p_pFirstSet->iCode == 100)
	{
		write_log(_sink, LL_WARN, "[TraderZSOpt] 返回内容[无记录]");
	}

	if (NULL == _orders)
		_orders = WTSArray::create();

	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryOrder] opt num: {}, order status: {}, qty: {}, withdraw qty: {}, trade qty: {}, orderid: {}, raw orderid: {}, orderbsn: {}", p_pRsp[i].szOptNum, p_pRsp[i].chOrderStatus, p_pRsp[i].llOrderQty, p_pRsp[i].llWithdrawnQty, p_pRsp[i].llMatchedQty, p_pRsp[i].szOrderId, p_pRsp[i].szRawOrderId, p_pRsp[i].iOrderBsn);

			const char* code = p_pRsp[i].szOptNum;
			const char* exchg = strcmp(p_pRsp[i].szStkbd, "SH") == 0 ? "SSE" : "SZSE";

			WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);
			if (contract != NULL)
			{
				WTSOrderInfo* orderInfo = WTSOrderInfo::create();

				orderInfo->setContractInfo(contract);
				orderInfo->setPrice(atof(p_pRsp[i].szOrderPrice));
				orderInfo->setVolume(p_pRsp[i].llOrderQty);
				orderInfo->setDirection(wrapDirectionType(p_pRsp[i].iStkBiz));
				orderInfo->setOffsetType(wrapOffsetType(p_pRsp[i].iStkBiz));
				orderInfo->setOrderFlag(WOF_NOR);
				orderInfo->setPriceType(wrapPriceType(p_pRsp[i].iStkBizAction));

				orderInfo->setVolTraded((uint32_t)p_pRsp[i].llMatchedQty);
				orderInfo->setVolLeft(p_pRsp[i].llOrderQty - p_pRsp[i].llMatchedQty);

				orderInfo->setCode(code);
				orderInfo->setExchange(contract->getExchg());

				orderInfo->setOrderDate(((uint32_t)p_pRsp[i].szOrderTime) / 1000000000);
				uint32_t uTime = ((uint32_t)p_pRsp[i].szOrderTime) % 1000000000;
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

int TraderZSOpt::OnOptQryFill(STFirstSet *p_pFirstSet, STRspOptQryFill *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (p_pFirstSet->iCode == 100)
	{
		write_log(_sink, LL_WARN, "[TraderZSOpt] 返回内容[无记录]");
	}

	if (NULL == _trades)
		_trades = WTSArray::create();

	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryFill] code: {}, qty: {}, trade qty: {}, order price: {}, trading price: {}, orderid: {}, matched sn: {}, orderbsn: {}", p_pRsp[i].szOptNum, p_pRsp[i].llOrderQty, p_pRsp[i].llMatchedQty, p_pRsp[i].szOrderPrice, p_pRsp[i].szMatchedPrice, p_pRsp[i].szOrderId, p_pRsp[i].szMatchedSn, p_pRsp[i].iOrderBsn);

			const char* code = p_pRsp[i].szOptNum;
			const char* exchg = strcmp(p_pRsp[i].szStkbd, "SH") == 0 ? "SSE" : "SZSE";

			WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);

			if (contract != NULL)
			{
				WTSTradeInfo *trdInfo = WTSTradeInfo::create(code, exchg);
				trdInfo->setVolume(p_pRsp[i].llMatchedQty);  // 成交数量
				trdInfo->setPrice(atof(p_pRsp[i].szMatchedPrice));  // 成交价格
				trdInfo->setTradeID(p_pRsp[i].szMatchedSn);  // 成交编号
				trdInfo->setContractInfo(contract);

				std::string strTime(p_pRsp[i].szMatchedTime);
				StrUtil::replace(strTime, ":", "");
				uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
				uint32_t uDate = (uint32_t)(p_pRsp[i].iOrderDate);

				trdInfo->setTradeDate(uDate);
				trdInfo->setTradeTime(TimeUtils::makeTime(uDate, uTime));

				trdInfo->setDirection(wrapDirectionType(p_pRsp[i].iStkBiz));
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

int TraderZSOpt::OnOptQryHolding(STFirstSet *p_pFirstSet, STRspOptQryHolding *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (NULL == _positions)
		_positions = PositionMap::create();

	if (p_pFirstSet->iCode == 100)
	{
		write_log(_sink, LL_WARN, "[TraderZSOpt] 返回内容[无记录]");
	}

	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryHolding] code: {}, avail qty: {}, pre qty: {}, frz qty: {}, holding qty: {}, trd open qty: {}, trd close qty: {}", 
				p_pRsp[i].szOptNum, p_pRsp[i].llOptAvl, p_pRsp[i].llOptBln, p_pRsp[i].llOptFrz, p_pRsp[i].llOptRealPosi, p_pRsp[i].llOptDailyOpenRlt, p_pRsp[i].llOptClsUnmatched);

			const char* exchg = (strcmp(p_pRsp[i].szStkbd, "SH") == 0) ? "SSE" : "SZSE";
			const char* code = p_pRsp[i].szOptNum;

			WTSContractInfo* contract = _bd_mgr->getContract(code, exchg);
			if (contract)
			{
				WTSCommodityInfo* commInfo = contract->getCommInfo();
				std::string key = fmt::format("{}-{}", code, p_pRsp[i].chOptSide);

				WTSPositionItem* pos = (WTSPositionItem*)_positions->get(key);

				if (pos == NULL)
				{
					pos = WTSPositionItem::create(code, commInfo->getCurrency(), commInfo->getExchg());
					pos->setContractInfo(contract);
					_positions->add(key, pos, false);
				}

				pos->setDirection(wrapPosDirection(p_pRsp[i].chOptSide));  // 合约持仓方向
				pos->setMargin(atof(p_pRsp[i].szOptMargin));
				pos->setDynProfit(atof(p_pRsp[i].szOptFloatProfit));
				pos->setPositionCost(atof(p_pRsp[i].szOptBcostRlt));
				pos->setAvgPrice(atof(p_pRsp[i].szCostPrice));

				// 合约余额 = 昨持，合约昨日余额 = 昨昨持，合约今持 = 合约实际持仓
				// 今总持 = 昨持 + 今开 - 昨平
				// 平昨 = 今持 - 昨持 - 开今
				double clsYd = p_pRsp[i].llOptRealPosi - p_pRsp[i].llOptBln - p_pRsp[i].llOptDailyOpenRlt;
				if (clsYd < 0) clsYd = 0;

				// 昨可用 = 昨持 - 昨平
				double preAvail = p_pRsp[i].llOptBln - clsYd;
				if (preAvail < 0) preAvail = 0;

				// 今可用 = 可用 - 昨可用
				double curAvail = p_pRsp[i].llOptAvl - preAvail;
				if (curAvail < 0) curAvail = 0;

				// 今仓 = 今总持 - 昨可用
				double curPos = p_pRsp[i].llOptRealPosi - preAvail;
				if (curPos < 0) curPos = 0;

				pos->setNewPosition(curPos);  
				pos->setPrePosition(p_pRsp[i].llOptBln);  // 合约昨持

				pos->setAvailNewPos(curAvail);  // 今可用
				pos->setAvailPrePos(preAvail);  // 昨可用

				write_log(_sink, LL_DEBUG, "[{}]|[{}] cur pos: {}, pre pos: {}, cur avail: {}, pre avail: {}", p_pRsp[i].szOptNum, p_pRsp[i].chOptSide, curPos, p_pRsp[i].llOptBln, curAvail, preAvail);
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

int TraderZSOpt::OnOptQryMoney(STFirstSet *p_pFirstSet, STRspOptQryMoney *p_pRsp, LONGLONG  p_llReqId, int p_iNum)
{
	if (0 == p_pFirstSet->iCode && NULL != p_pRsp)
	{
		WTSArray * ay = WTSArray::create();

		for (int i = 0; i < p_iNum; i++)
		{
			write_log(_sink, LL_DEBUG, "[OnQryMoney] acct: {}, avail: {}, balance: {}", p_pRsp[i].llCuacctCode, p_pRsp[i].szFundAvl, p_pRsp[i].szFundBln);

			WTSAccountInfo* accountInfo = WTSAccountInfo::create();

			accountInfo->setPreBalance(atof(p_pRsp[i].szFundPrebln));
			accountInfo->setCloseProfit(0);
			accountInfo->setDynProfit(0);
			accountInfo->setMargin(atof(p_pRsp[i].szMarginUsed));
			accountInfo->setAvailable(atof(p_pRsp[i].szFundRealAvl));
			accountInfo->setFrozenMargin(atof(p_pRsp[i].szMarginInclDyn));
			accountInfo->setFrozenCommission(atof(p_pRsp[i].szFundFeeFrz));
			accountInfo->setBalance(atof(p_pRsp[i].szFundBln));
			accountInfo->setCurrency("CNY");
			accountInfo->setWithdraw(atof(p_pRsp[i].szDailyInAmt));  // 出金
			accountInfo->setDeposit(atof(p_pRsp[i].szDailyInAmt));  // 入金

			ay->append(accountInfo, false);
		}

		if (_sink)
			_sink->onRspAccount(ay);

		ay->release();
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderZSOpt] query fail, code: {}, msg: {}", p_pFirstSet->iCode, p_pFirstSet->szText);
	}

	return 0;
}

#pragma region "ITraderApi"
bool TraderZSOpt::init(WTSVariant *params)
{
	_user = params->getCString("user");
	_pass = params->getCString("pass");

	_host = params->getCString("host");
	_port = params->getInt32("port");

	_acctCode = params->getInt64("account");

	_flowdir = params->getCString("flowdir");

	if (_flowdir.empty())
		_flowdir = "zsOptTrade";

	_flowdir = StrUtil::standardisePath(_flowdir);

	std::string module = params->getCString("zsmodule");
	if (module.empty()) module = "zsTradeApi";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "");;

	m_hInstZS = DLLHelper::load_library(dllpath.c_str());

	return true;
}

void TraderZSOpt::release()
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

void TraderZSOpt::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderZSOpt::reconnect()
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

		write_log(_sink, LL_ERROR, "[TraderZSOpt] zsTradeApi init failed, {}", _api->GetLastError());

		StdThreadPtr thrd(new StdThread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			write_log(_sink, LL_WARN, "[TraderZSOpt] {} reconnecting...", _user.c_str());
			reconnect();
		}));
	}
}

void TraderZSOpt::connect()
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

void TraderZSOpt::disconnect()
{
	release();
}

bool TraderZSOpt::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderZSOpt::genEntrustID(char* buffer, uint32_t orderRef)
{
	//这里不再使用sessionid，因为每次登陆会不同，如果使用的话，可能会造成不唯一的情况
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderZSOpt::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderZSOpt::makeEntrustID(char* buffer, int length)
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

void TraderZSOpt::doLogin()
{
	_state = TS_LOGINING;

	int iResult = 0;
	while (1)
	{
		STReqAcctRegister stReq = { 0 };
		strncpy(stReq.szUserName, _user.c_str(), sizeof(stReq.szUserName));
		stReq.llCuacctCode = _acctCode;

		write_log(_sink, LL_DEBUG, "[{}][{}] login ...", stReq.szUserName, stReq.llCuacctCode);

		iResult = _api->AcctRegister(&stReq, genRequestID());

		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (_logined)
			break;
	}

	if (iResult)
	{
		std::string msg = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZSOpt] Login failed: {}", msg);
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
				ss << "./zsoptdata/local/";
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
				ss << "./zsoptdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_oid.sc";
				m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			write_log(_sink, LL_INFO, "[TraderZSOpt] [{}] Login succeed, trading date: {}...", _user.c_str(), _tradingday);

			_state = TS_LOGINED;
			_inited = true;
			_asyncio.post([this] {
				_sink->onLoginResult(true, 0, _tradingday);
				_state = TS_ALLREADY;
			});
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderZSOpt] [{}] Connection recovered", _user.c_str());
			_state = TS_ALLREADY;
		}
	}
}

int TraderZSOpt::login(const char* user, const char* pass, const char* productInfo)
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

int TraderZSOpt::logout()
{
	if (_api == NULL)
		return -1;

	_api->Exit();
	return 0;
}

int TraderZSOpt::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	STReqOrderOpt stReq = { 0 };
	stReq.llCuacctCode = _acctCode;

	uint32_t orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);

	strncpy(stReq.szOptNum, entrust->getCode(), sizeof(stReq.szOptNum));

	bool isSH = wt_stricmp(entrust->getExchg(), "SSE") == 0 ? true : false;
	strncpy(stReq.szStkbd, isSH ? "SH" : "SZ", sizeof(stReq.szStkbd));
	strncpy(stReq.szOrderPrice, std::to_string(entrust->getPrice()).c_str(), sizeof(stReq.szOrderPrice));
	stReq.llOrderQty = (long long)entrust->getVolume();
	stReq.iStkBiz = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());  //买入开仓400；卖出平仓401；卖出开仓402；买入平仓403
	stReq.iStkBizAction = wrapPriceType(WPT_LIMITPRICE, isSH);  // 期权证券业务行为
	write_log(_sink, LL_INFO, "[orderInsert] code: {}, price: {}, vol: {}, stkbiz: {}, stkbizaction: {}", stReq.szOptNum, stReq.szOrderPrice, stReq.llOrderQty, stReq.iStkBiz, stReq.iStkBizAction);

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
			write_log(_sink, LL_WARN, message);
		});
	}

	int iResult = _api->OrderOpt(&stReq, genRequestID());
	if (iResult)
	{
		std::string msg = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZSOpt] Order inserting failed: {}", msg);
	}
	else
		write_log(_sink, LL_INFO, "[TraderZSOpt] Order inserting success");

	return 0;
}

int TraderZSOpt::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	STReqOptCancelOrder stReq = { 0 };
	stReq.llCuacctCode = _acctCode;
	strncpy(stReq.szStkbd, wt_stricmp(action->getExchg(), "SSE") == 0 ? "SH" : "SZ", sizeof(stReq.szStkbd));
	strncpy(stReq.szOrderId, action->getOrderID(), sizeof(stReq.szOrderId));

	int iResult = _api->OptCancelOrder(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZSOpt] Order cancelling failed: {}", error_info);
	}

	return 0;
}

uint32_t TraderZSOpt::genRequestID()
{
	return _reqid.fetch_add(1) + 1;
}

int TraderZSOpt::queryAccount()
{
	STReqOptQryMoney stReq = { 0 };
	stReq.llCuacctCode = _acctCode;

	int iResult = _api->OptQryMoney(&stReq, genRequestID());
	if (iResult != 0)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZSOpt] Account querying failed: {}", error_info);
	}

	return 0;
}

int TraderZSOpt::queryPositions()
{
	STReqOptQryHolding stReq = { 0 };
	stReq.llCuacctCode = _acctCode;

	int iResult = _api->OptQryHolding(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZSOpt] Positions querying failed: {}", error_info);
	}

	return 0;
}

int TraderZSOpt::queryOrders()
{
	STReqOptQryOrder stReq = { 0 };
	stReq.llCuacctCode = _acctCode;
	//strncpy(stReq.szOptNum, "", sizeof(stReq.szOptNum));  // 查询全部

	int iResult = _api->OptQryOrder(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZSOpt] Orders querying failed: {}", error_info);
	}

	return 0;
}

int TraderZSOpt::queryTrades()
{
	STReqOptQryFill stReq = { 0 };
	stReq.llCuacctCode = _acctCode;
	//strncpy(stReq.szOptNum, "", sizeof(stReq.szOptNum));  // 查询全部

	int iResult = _api->OptQryFill(&stReq, genRequestID());
	if (iResult)
	{
		std::string error_info = _api->GetLastError();
		write_log(_sink, LL_ERROR, "[TraderZSOpt] Trades querying failed: {}", error_info);
	}

	return 0;
}
#pragma endregion "ITraderApi"