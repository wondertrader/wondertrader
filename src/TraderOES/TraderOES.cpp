/*!
 * \file TraderOES.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "TraderOES.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/oesApi0.17.5.8/x64/oes_api.lib")
#else
#pragma comment(lib, "../API/oesApi0.17.5.8/x86/oes_api.lib")
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
		TraderOES *instance = new TraderOES();
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

inline WTSOrderState wrapOrderState(int8 orderState)
{
	switch (orderState)
	{
	case OES_ORD_STATUS_PENDING:
	case OES_ORD_STATUS_NEW:
		return WOS_Submitting;
	case OES_ORD_STATUS_DECLARED:
		return WOS_NotTraded_NotQueuing;
	case OES_ORD_STATUS_FILLED:
		return WOS_AllTraded;
	case OES_ORD_STATUS_PARTIALLY_FILLED:
		return WOS_PartTraded_Queuing;
	case OES_ORD_STATUS_PARTIALLY_CANCELED:
	case OES_ORD_STATUS_CANCELED:
		return WOS_Canceled;
	case OES_ORD_STATUS_INVALID_OES:
	case OES_ORD_STATUS_INVALID_EXCHANGE:
	case OES_ORD_STATUS_INVALID_TGW_REJECT:
	case OES_ORD_STATUS_INVALID_TGW_COMM:
	case OES_ORD_STATUS_INVALID_TGW_TRY_AGAIN:
		return WOS_Canceled;
	default:
		return WOS_Nottouched;
	}
}

static inline int on_rpt_connect(OesAsyncApiChannelT *pAsyncChannel, void *pCallbackParams)
{
	//OesAsyncApi_DefaultOnConnect(pAsyncChannel, NULL);
	OesAsyncApi_SendReportSynchronization(pAsyncChannel, -1, -1, INT_MAX);
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_rpt_connected(pAsyncChannel);
	return 0;
}

static inline int on_rpt_disconnect(OesAsyncApiChannelT *pAsyncChannel, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_rpt_disconnected(pAsyncChannel);

	return 0;
}

static inline int on_rpt_message(OesApiSessionInfoT *pSessionInfo, SMsgHeadT *pMsgHead, void *pMsgItem, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_rpt_message(pMsgHead, pMsgItem);

	return 0;
}

static inline int on_ord_connect(OesAsyncApiChannelT *pAsyncChannel, void *pCallbackParams)
{
	OesAsyncApi_DefaultOnConnect(pAsyncChannel, NULL);

	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_ord_connected(pAsyncChannel);

	return 0;
}

static inline int on_ord_disconnect(OesAsyncApiChannelT *pAsyncChannel, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_ord_disconnected(pAsyncChannel);

	return 0;
}

static inline int on_ord_message(OesApiSessionInfoT *pSessionInfo, SMsgHeadT *pMsgHead, void *pMsgItem, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_ord_message(pMsgHead, pMsgItem);

	return 0;
}

static inline int on_rsp_orders(OesApiSessionInfoT *pQryChannel, SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_rsp_orders(pMsgHead, pMsgItem, pQryCursor);

	return 0;
}

static inline int on_rsp_account(OesApiSessionInfoT *pQryChannel, SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_rsp_account(pMsgHead, pMsgItem, pQryCursor);

	return 0;
}

static inline int on_rsp_positions(OesApiSessionInfoT *pQryChannel, SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_rsp_positions(pMsgHead, pMsgItem, pQryCursor);

	return 0;
}

static inline int on_rsp_trades(OesApiSessionInfoT *pQryChannel, SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor, void *pCallbackParams)
{
	TraderOES* trader = (TraderOES*)pCallbackParams;
	if (trader == NULL)
		return 0;

	trader->handle_rsp_trades(pMsgHead, pMsgItem, pQryCursor);

	return 0;
}

TraderOES::TraderOES()
	: _context(NULL)
	, _sink(NULL)
	, _bd_mgr(NULL)
	, _tradingday(0)
	, _trades(NULL)
	, _orders(NULL)
	, _funds(NULL)
	, _positions(NULL)
{
}


TraderOES::~TraderOES()
{
}

#pragma region "ITraderApi"
bool TraderOES::init(WTSVariant *params)
{
	_config = params->getCString("config");

	std::string module = params->getCString("oesmodule");
	if (module.empty()) module = "oes_api";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");

	DLLHelper::load_library(dllpath.c_str());

	return true;
}

void TraderOES::release()
{
	if (_context == NULL)
		return;

	OesAsyncApi_Stop(_context);

	while (!OesAsyncApi_IsAllTerminated(_context)) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	OesAsyncApi_ReleaseContext(_context);
	_context = NULL;

	if (_orders)
		_orders->clear();

	if (_positions)
		_positions->clear();

	if (_trades)
		_trades->clear();
}

void TraderOES::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderOES::reconnect()
{
	if (_context)
	{
		OesAsyncApi_Stop(_context);

		while (!OesAsyncApi_IsAllTerminated(_context))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		OesAsyncApi_ReleaseContext(_context);
		_context = NULL;
	}

	_context = OesAsyncApi_CreateContext(_config.c_str());
	if (_context == NULL)
	{
		write_log(_sink, LL_ERROR, "[TraderOES] Creating api failed");
		return;
	}

	OesAsyncApiChannelT* channel = OesAsyncApi_AddChannelFromFile(
		_context, OESAPI_CHANNEL_TYPE_REPORT, "async_rpt_channel1",
		_config.c_str(), OESAPI_CFG_DEFAULT_SECTION,
		OESAPI_CFG_DEFAULT_KEY_RPT_ADDR,
		on_rpt_message, this,
		on_rpt_connect, this,
		on_rpt_disconnect, this);
	if (!channel)
	{
		write_log(_sink, LL_WARN, "[TraderOES] Loading section {} from config failed", OESAPI_CFG_DEFAULT_KEY_RPT_ADDR);
	}

	_channel = OesAsyncApi_AddChannelFromFile(
		_context, OESAPI_CHANNEL_TYPE_ORDER, "async_ord_channel1",
		_config.c_str(), OESAPI_CFG_DEFAULT_SECTION,
		OESAPI_CFG_DEFAULT_KEY_ORD_ADDR,
		on_ord_message, this,
		on_ord_connect, this,
		on_ord_disconnect, this);
	if (!_channel)
	{
		write_log(_sink, LL_WARN, "[TraderOES] Loading section {} from config failed", OESAPI_CFG_DEFAULT_KEY_ORD_ADDR);
	}

	_asyncio.post([this] {
		if (_sink)
			_sink->handleEvent(WTE_Connect, 0);
	});
}

void TraderOES::connect()
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

void TraderOES::disconnect()
{
	release();
}

bool TraderOES::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderOES::genEntrustID(char* buffer, uint32_t orderRef)
{
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderOES::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderOES::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		uint32_t orderref = (uint32_t)++_channel->lastOutMsgSeq;
		fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderOES::doLogin()
{
	_state = TS_LOGINING;

	OesAsyncApi_Start(_context);
}

int TraderOES::login(const char* user, const char* pass, const char* productInfo)
{
	_user = user;

	if (_context == NULL)
		return -1;

	_state = TS_LOGINING;

	OesAsyncApi_Start(_context);

	return 0;
}

int TraderOES::logout()
{
	if (_context == NULL)
		return -1;

	release();
	return 0;
}

int TraderOES::orderInsert(WTSEntrust* entrust)
{
	if (_context == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	uint32_t ordref;
	extractEntrustID(entrust->getEntrustID(), ordref);
	OesOrdReqT ordReq = { NULLOBJ_OES_ORD_REQ };
	ordReq.clSeqNo = (int32)ordref;
	ordReq.mktId = wt_stricmp(entrust->getExchg(), "SSE") == 0 ? OES_MKT_SH_ASHARE : OES_MKT_SZ_ASHARE;
	ordReq.ordType = OES_ORD_TYPE_LMT;
	ordReq.bsType = (entrust->getOffsetType() == WOT_OPEN) ? OES_BS_TYPE_BUY : OES_BS_TYPE_SELL;
	ordReq.ordQty = (int32)entrust->getVolume();
	ordReq.ordPrice = (int32)round(entrust->getPrice()*10000.0);
	wt_strcpy(ordReq.securityId, entrust->getCode());

	auto ret = OesAsyncApi_SendOrderReq(_channel, &ordReq);
	if(ret < 0)
	{
		write_log(_sink, LL_ERROR, "[TraderOES] Inserting order failed: {}", ret);
	}

	return 0;
}

int TraderOES::orderAction(WTSEntrustAction* action)
{
	if (_context == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}


	OesOrdCancelReqT    cancelReq = { NULLOBJ_OES_ORD_CANCEL_REQ };
	cancelReq.clSeqNo = (int32) ++_channel->lastOutMsgSeq;
	cancelReq.mktId = wt_stricmp(action->getExchg(), "SSE") == 0 ? OES_MKT_SH_ASHARE : OES_MKT_SZ_ASHARE;
	uint32_t ordref;
	extractEntrustID(action->getEntrustID(), ordref);
	cancelReq.origClSeqNo = ordref;
	cancelReq.origClOrdId = strtoll(action->getOrderID(), NULL, 10);

	auto ret = OesAsyncApi_SendOrderCancelReq(_channel, &cancelReq);
	if (ret < 0)
	{
		write_log(_sink, LL_ERROR, "[TraderOES] Canceling order failed: {}", ret);
	}

	return 0;
}


int TraderOES::queryAccount()
{
	if (_context == NULL)
		return -1;

	OesQryCashAssetFilterT qryFilter = { NULLOBJ_OES_QRY_CASH_ASSET_FILTER };
	int32 ret = OesAsyncApi_QueryCashAsset(_channel, &qryFilter, on_rsp_account, this);
	if (ret < 0)
	{
		write_log(_sink, LL_ERROR, "[TraderOES] Query account failed: {}", ret);
	}
	else if (ret == 0)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspAccount(NULL);
		});
	}

	return ret;
}

int TraderOES::queryPositions()
{
	if (_context == NULL)
		return -1;

	OesQryStkHoldingFilterT qryFilter = { NULLOBJ_OES_QRY_STK_HOLDING_FILTER };
	int32 ret = OesAsyncApi_QueryStkHolding(_channel, &qryFilter, on_rsp_positions, this);
	if (ret < 0)
	{
		write_log(_sink, LL_ERROR, "[TraderOES] Query pStkHolding failed: {}", ret);
	}
	else if (ret == 0)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspPosition(NULL);
		});
	}

	return ret;
}

int TraderOES::queryOrders()
{
	if (_context == NULL)
		return -1;

	OesQryOrdFilterT    qryFilter = { NULLOBJ_OES_QRY_ORD_FILTER };
	int32 ret = OesAsyncApi_QueryOrder(_channel, &qryFilter, on_rsp_orders, this);
	if(ret < 0)
	{
		write_log(_sink, LL_ERROR, "[TraderOES] Query order failed: {}", ret);
	}
	else if(ret == 0)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspOrders(NULL);
		});
	}

	return ret;
}

int TraderOES::queryTrades()
{
	if (_context == NULL)
		return -1;

	OesQryTrdFilterT    qryFilter = { NULLOBJ_OES_QRY_TRD_FILTER };
	int32 ret = OesAsyncApi_QueryTrade(_channel, &qryFilter, on_rsp_trades, this);
	if (ret < 0)
	{
		write_log(_sink, LL_ERROR, "[TraderOES] Query trades failed: {}", ret);
	}
	else if (ret == 0)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspTrades(NULL);
		});
	}

	return ret;
}

void TraderOES::handle_ord_connected(OesAsyncApiChannelT *pAsyncChannel)
{
	_state = TS_LOGINED;

	{
		//初始化委托单缓存器
		std::stringstream ss;
		ss << "./oesdata/local/";
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
		ss << "./oesdata/local/";
		std::string path = StrUtil::standardisePath(ss.str());
		if (!StdFile::exists(path.c_str()))
			boost::filesystem::create_directories(path.c_str());
		ss << _user << "_oid.sc";
		m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
			write_log(_sink, LL_WARN, message);
		});
	}

	_asyncio.post([this] {
		_tradingday = OesAsyncApi_GetTradingDay(_channel);
		if (_sink)
			_sink->onLoginResult(true, "", _tradingday);
		_state = TS_ALLREADY;
	});	
}

void TraderOES::handle_ord_disconnected(OesAsyncApiChannelT *pAsyncChannel)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, 0);

	write_log(_sink, LL_DEBUG, "[TraderOES] Order channel disconnected");
}

void TraderOES::handle_ord_message(SMsgHeadT *pMsgHead, void *pMsgItem)
{
}

void TraderOES::handle_rpt_connected(OesAsyncApiChannelT *pAsyncChannel)
{
	write_log(_sink, LL_DEBUG, "[TraderOES] Report channel connected");
}

void TraderOES::handle_rpt_disconnected(OesAsyncApiChannelT *pAsyncChannel)
{
	write_log(_sink, LL_DEBUG, "[TraderOES] Report channel disconnected");
}

WTSOrderInfo* TraderOES::makeOrderInfo(OesOrdCnfmT* pOrdItem)
{
	const char* exchg = (OES_MKT_SH_ASHARE == pOrdItem->mktId) ? "SSE" : "SZSE";
	WTSContractInfo* cInfo = _bd_mgr->getContract(pOrdItem->securityId, exchg);
	if (cInfo == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(cInfo);
	pRet->setPrice(pOrdItem->ordPrice / 10000.0);
	pRet->setVolume(pOrdItem->ordQty);
	pRet->setDirection(WDT_LONG);
	pRet->setPriceType(WPT_LIMITPRICE);
	pRet->setOffsetType(pOrdItem->bsType == OES_BS_TYPE_BUY ? WOT_OPEN : WOT_CLOSE);

	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), pOrdItem->clSeqNo);

	pRet->setVolTraded(pOrdItem->cumQty);
	pRet->setVolLeft(pOrdItem->ordQty - pOrdItem->cumQty);

	pRet->setCode(pOrdItem->securityId);
	pRet->setExchange(cInfo->getExchg());

	pRet->setOrderDate(pOrdItem->ordDate);
	pRet->setOrderTime(TimeUtils::makeTime(pOrdItem->ordDate, pOrdItem->ordTime));

	pRet->setOrderState(wrapOrderState(pOrdItem->ordStatus));
	if (pOrdItem->ordStatus >= __OES_ORD_STATUS_INVALID_MIN)
	{
		pRet->setError(true);
		pRet->setStateMsg(fmt::format("Rejected: {}", pOrdItem->ordRejReason).c_str());
	}

	pRet->setOrderID(fmt::format("{}", pOrdItem->clOrdId).c_str());

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

WTSTradeInfo* TraderOES::makeTradeInfo(OesTrdCnfmT *pTrdItem)
{
	const char* exchg = (OES_MKT_SH_ASHARE == pTrdItem->mktId) ? "SSE" : "SZSE";
	WTSContractInfo* cInfo = _bd_mgr->getContract(pTrdItem->securityId, exchg);
	if (cInfo == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(pTrdItem->securityId, cInfo->getExchg());
	pRet->setVolume(pTrdItem->trdQty);
	pRet->setPrice(pTrdItem->trdPrice / 10000.0);
	pRet->setTradeID(fmt::format("{}", pTrdItem->exchTrdNum).c_str());
	pRet->setContractInfo(cInfo);

	pRet->setTradeDate(pTrdItem->trdDate);
	pRet->setTradeTime(TimeUtils::makeTime(pTrdItem->trdDate, pTrdItem->trdTime));

	pRet->setDirection(WDT_LONG);
	pRet->setOffsetType(pTrdItem->trdSide == OES_BS_TYPE_BUY ? WOT_OPEN : WOT_CLOSE);
	pRet->setRefOrder(fmt::format("{}", pTrdItem->clOrdId).c_str());
	pRet->setTradeType(WTT_Common);

	pRet->setAmount(pTrdItem->trdAmt / 10000.0);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

WTSEntrust* TraderOES::makeEntrust(OesOrdRejectT *entrustField)
{
	const char* exchg = (OES_MKT_SH_ASHARE == entrustField->mktId) ? "SSE" : "SZSE";
	WTSContractInfo* ct = _bd_mgr->getContract(entrustField->securityId, exchg);
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		entrustField->securityId,
		entrustField->ordQty,
		entrustField->ordPrice/10000.0,
		ct->getExchg());
	pRet->setContractInfo(ct);
	pRet->setDirection(WDT_LONG);
	pRet->setPriceType(WPT_LIMITPRICE);
	pRet->setOffsetType(entrustField->bsType == OES_BS_TYPE_BUY ? WOT_OPEN : WOT_CLOSE);
	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), entrustField->clSeqNo);

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

void TraderOES::handle_rpt_message(SMsgHeadT *pMsgHead, void *pMsgItem)
{
	OesRspMsgBodyT*	pRspMsg = (OesRspMsgBodyT *)pMsgItem;
	OesRptMsgT*	pRptMsg = &pRspMsg->rptMsg;

	switch (pMsgHead->msgId) {
	case OESMSG_RPT_ORDER_INSERT:               /* OES委托已生成 (已通过风控检查) @see OesOrdCnfmT */
		//printf(">>> Recv OrdInsertRsp: {clSeqNo: %d, " \
		//	"bsType: %" __SPK_FMT_HH__ "u, " \
		//	"clEnvId: %" __SPK_FMT_HH__ "d, " \
		//	"clOrdId: %" __SPK_FMT_LL__ "d}\n",
		//	pRptMsg->rptBody.ordInsertRsp.clSeqNo,
		//	pRptMsg->rptBody.ordInsertRsp.bsType,
		//	pRptMsg->rptBody.ordInsertRsp.clEnvId,
		//	pRptMsg->rptBody.ordInsertRsp.clOrdId);
		{
			WTSOrderInfo* ordInfo = makeOrderInfo(&pRptMsg->rptBody.ordInsertRsp);
			if(ordInfo)
			{
				if (_sink)
					_sink->onPushOrder(ordInfo);
				ordInfo->release();
			}
		}
		break;

	case OESMSG_RPT_BUSINESS_REJECT:            /* OES业务拒绝 (未通过风控检查等) @see OesOrdRejectT */
		//printf(">>> Recv OrdRejectRsp: {clSeqNo: %d, " \
		//	"bsType: %" __SPK_FMT_HH__ "u, " \
		//	"clEnvId: %" __SPK_FMT_HH__ "d, " \
		//	"ordRejReason: %d}\n",
		//	pRptMsg->rptBody.ordRejectRsp.clSeqNo,
		//	pRptMsg->rptBody.ordRejectRsp.bsType,
		//	pRptMsg->rptBody.ordRejectRsp.clEnvId,
		//	pRptMsg->rptHead.ordRejReason);
		if(pRptMsg->rptBody.ordRejectRsp.origClOrdId == 0)
		{
			WTSEntrust* entrustInfo = makeEntrust(&pRptMsg->rptBody.ordRejectRsp);
			if (entrustInfo)
			{
				WTSError* error = WTSError::create(WEC_ORDERINSERT, fmt::format("Rejected: {}", pRptMsg->rptHead.ordRejReason).c_str());
				if (_sink)
					_sink->onRspEntrust(entrustInfo, error);
				entrustInfo->release();
				error->release();
			}
		}
		else
		{
			WTSError* error = WTSError::create(WEC_ORDERCANCEL, fmt::format("Rejected: {}", pRptMsg->rptHead.ordRejReason).c_str());
			if (_sink)
				_sink->onTraderError(error);
		}
		break;

	case OESMSG_RPT_ORDER_REPORT:               /* 交易所委托回报 (包括交易所委托拒绝、委托确认和撤单完成通知) @see OesOrdCnfmT */
		//printf(">>> Recv OrdCnfm: {clSeqNo: %d, " \
		//	"bsType: %" __SPK_FMT_HH__ "u, " \
		//	"clEnvId: %" __SPK_FMT_HH__ "d, " \
		//	"clOrdId: %" __SPK_FMT_LL__ "d, " \
		//	"ordStatus: %" __SPK_FMT_HH__ "u}\n",
		//	pRptMsg->rptBody.ordCnfm.clSeqNo,
		//	pRptMsg->rptBody.ordCnfm.bsType,
		//	pRptMsg->rptBody.ordCnfm.clEnvId,
		//	pRptMsg->rptBody.ordCnfm.clOrdId,
		//	pRptMsg->rptBody.ordCnfm.ordStatus);
		{
			WTSOrderInfo* ordInfo = makeOrderInfo(&pRptMsg->rptBody.ordCnfm);
			if (ordInfo)
			{
				if (_sink)
					_sink->onPushOrder(ordInfo);
				ordInfo->release();
			}
		}
		break;

	case OESMSG_RPT_TRADE_REPORT:               /* 交易所成交回报 @see OesTrdCnfmT */
		//printf(">>> Recv TrdCnfm: {clSeqNo: %d, " \
		//	"bsType: %" __SPK_FMT_HH__ "u, " \
		//	"clEnvId: %" __SPK_FMT_HH__ "d, " \
		//	"clOrdId: %" __SPK_FMT_LL__ "d, " \
		//	"ordStatus: %" __SPK_FMT_HH__ "u}\n",
		//	pRptMsg->rptBody.trdCnfm.clSeqNo,
		//	pRptMsg->rptBody.trdCnfm.ordBuySellType,
		//	pRptMsg->rptBody.trdCnfm.clEnvId,
		//	pRptMsg->rptBody.trdCnfm.clOrdId,
		//	pRptMsg->rptBody.trdCnfm.ordStatus);
		{
			OesOrdCnfmT ordReport = { NULLOBJ_OES_ORD_CNFM };
			OesHelper_ExtractOrdReportFromTrd(&pRptMsg->rptBody.trdCnfm, &ordReport);

			WTSOrderInfo* ordInfo = makeOrderInfo(&ordReport);
			if (ordInfo)
			{
				if (_sink)
					_sink->onPushOrder(ordInfo);
				ordInfo->release();
			}

			WTSTradeInfo* trdInfo = makeTradeInfo(&pRptMsg->rptBody.trdCnfm);
			if (trdInfo)
			{
				if (_sink)
					_sink->onPushTrade(trdInfo);
				trdInfo->release();
			}
		}
		/* @note 成交回报中已经包含了委托回报的相关信息, 可以通过辅助接口直接从成交回报中提取和生成委托回报数据
		if (pRptMsg->rptBody.trdCnfm.ordStatus == OES_ORD_STATUS_FILLED) {
			OesOrdCnfmT ordReport = {NULLOBJ_OES_ORD_CNFM};
			OesHelper_ExtractOrdReportFromTrd(
					&pRptMsg->rptBody.trdCnfm, &ordReport);
		}
		*/
		break;

	case OESMSG_RPT_CASH_ASSET_VARIATION:       /* 资金变动信息 @see OesCashAssetItemT */
		//printf(">>> Recv CashAsset: {cashAcctId: %s, " \
		//	"currentAvailableBal: %" __SPK_FMT_LL__ "d}\n",
		//	pRptMsg->rptBody.cashAssetRpt.cashAcctId,
		//	pRptMsg->rptBody.cashAssetRpt.currentAvailableBal);
		break;

	case OESMSG_RPT_MARKET_STATE:               /* 市场状态信息 @see OesMarketStateInfoT */
		//printf(">>> Recv MktStatusReport: " \
		//	"{exchId: %" __SPK_FMT_HH__ "u, " \
		//	"platformId: %" __SPK_FMT_HH__ "u, " \
		//	"mktId: %" __SPK_FMT_HH__ "u, " \
		//	"mktState: %" __SPK_FMT_HH__ "u}\n",
		//	pRspMsg->mktStateRpt.exchId,
		//	pRspMsg->mktStateRpt.platformId,
		//	pRspMsg->mktStateRpt.mktId,
		//	pRspMsg->mktStateRpt.mktState);
		break;

	case OESMSG_SESS_HEARTBEAT:                 /* 心跳消息 */
		//printf(">>> Recv heartbeat message.\n");
		break;
	default:
		break;
	}
}

void TraderOES::handle_rsp_account(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor)
{
	OesCashAssetItemT   *pCashAssetItem = (OesCashAssetItemT *)pMsgItem;
	if (NULL == _funds)
		_funds = WTSArray::create();

	WTSAccountInfo* accountInfo = WTSAccountInfo::create();
	accountInfo->setPreBalance(pCashAssetItem->beginningBal/10000.0);
	accountInfo->setCloseProfit(0);
	accountInfo->setDynProfit(0);
	accountInfo->setMargin(0);
	accountInfo->setAvailable(pCashAssetItem->currentAvailableBal/10000.0);
	accountInfo->setCommission(pCashAssetItem->totalFeeAmt/10000.0);
	accountInfo->setFrozenMargin(pCashAssetItem->buyFrzAmt/10000.0);
	accountInfo->setFrozenCommission(pCashAssetItem->feeFrzAmt/10000.0);
	accountInfo->setDeposit(pCashAssetItem->totalDepositAmt/10000.0);
	accountInfo->setWithdraw(pCashAssetItem->totalWithdrawAmt/10000.0);
	accountInfo->setBalance(pCashAssetItem->currentTotalBal/1000.0);

	if (pCashAssetItem->currType == OES_CURR_TYPE_HKD)
		accountInfo->setCurrency("HKD");
	else if (pCashAssetItem->currType == OES_CURR_TYPE_USD)
		accountInfo->setCurrency("USD");
	else
		accountInfo->setCurrency("CNY");

	_funds->append(accountInfo, false);

	if (pQryCursor == NULL || pQryCursor->isEnd)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspAccount(_funds);

			if (NULL != _funds)
				_funds->clear();
		});
	}
}

void TraderOES::handle_rsp_orders(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor)
{
	OesOrdItemT* pOrdItem = (OesOrdItemT *)pMsgItem;
	if (NULL == _orders)
		_orders = WTSArray::create();

	WTSOrderInfo* pRet = makeOrderInfo(pOrdItem);
	if (pRet == NULL)
		return;

	_orders->append(pRet, false);

	if (pQryCursor == NULL || pQryCursor->isEnd)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspOrders(_orders);

			if (NULL != _orders)
				_orders->clear();
		});
	}
}

void TraderOES::handle_rsp_positions(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor)
{
	OesStkHoldingItemT* pStkHolding = (OesStkHoldingItemT *)pMsgItem;
	if (NULL == _positions)
		_positions = WTSArray::create();

	const char* exchg = (OES_MKT_SH_ASHARE == pStkHolding->mktId) ? "SSE" : "SZSE";
	WTSContractInfo* cInfo = _bd_mgr->getContract(pStkHolding->securityId, exchg);
	if (cInfo != NULL)
	{
		WTSCommodityInfo* commInfo = cInfo->getCommInfo();
		WTSPositionItem* pos = WTSPositionItem::create(pStkHolding->securityId, commInfo->getCurrency(), commInfo->getExchg());
		pos->setContractInfo(cInfo);
		pos->setDirection(WDT_LONG);

		double preVol = std::max((double)(pStkHolding->originalHld - pStkHolding->totalSellHld), 0.0);
		double newVol = pStkHolding->originalHld + pStkHolding->totalBuyHld - pStkHolding->totalSellHld - preVol;

		pos->setNewPosition(newVol);
		pos->setPrePosition(preVol);

		pos->setMargin(pos->getTotalPosition()*pStkHolding->costPrice/10000.0);
		pos->setDynProfit(0);
		pos->setPositionCost(pos->getMargin());

		pos->setAvgPrice(pStkHolding->costPrice/10000.0);

		pos->setAvailNewPos(0);
		pos->setAvailPrePos((double)pStkHolding->sellAvlHld);

		_positions->append(pos, false);
	}

	if (pQryCursor == NULL || pQryCursor->isEnd)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspPosition(_positions);

			if (NULL != _positions)
				_positions->clear();
		});
	}
}

void TraderOES::handle_rsp_trades(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor)
{
	OesTrdItemT* pTrdItem = (OesTrdItemT *)pMsgItem;
	if (NULL == _trades)
		_trades = WTSArray::create();

	WTSTradeInfo* pRet = makeTradeInfo(pTrdItem);
	if (pRet == NULL)
		return;

	_trades->append(pRet, false);


	if(pQryCursor == NULL || pQryCursor->isEnd)
	{
		_asyncio.post([this] {
			if (_sink)
				_sink->onRspTrades(_trades);

			if (NULL != _trades)
				_trades->clear();
		});
	}
}
#pragma endregion "ITraderApi"