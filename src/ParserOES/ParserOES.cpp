/*!
 * \file ParserOES.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserOES.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/ModuleHelper.hpp"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"

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
inline void write_log(IParserSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	fmtutil::format_to(buffer, format, args...);

	sink->handleParserLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserOES* parser = new ParserOES();
		return parser;
	}

	EXPORT_FLAG void deleteParser(IParserApi* &parser)
	{
		if (NULL != parser)
		{
			delete parser;
			parser = NULL;
		}
	}
};

inline char wrapOrdDtlSide(char side)
{
	switch (side)
	{
	case '1': return BDT_Buy;
	case '2': return BDT_Sell;
	default:
		return side;
	}
}

inline char wrapTransSide(char side)
{
	switch (side)
	{
	case 'N': return BDT_Unknown;
	default:
		return side;
	}
}

static int on_connect(MdsAsyncApiChannelT *pAsyncChannel, void *pCallbackParams)
{
	ParserOES* parser = (ParserOES*)pCallbackParams;
	if (parser == NULL)
		return 0;

	parser->doOnConnected(pAsyncChannel);

	return 0;
}

static int on_disconnect(MdsAsyncApiChannelT *pAsyncChannel, void *pCallbackParams)
{
	ParserOES* parser = (ParserOES*)pCallbackParams;
	if (parser == NULL)
		return 0;

	parser->doOnDisconnected(pAsyncChannel);

	return 0;
}

static int on_message(MdsApiSessionInfoT *pSessionInfo, SMsgHeadT *pMsgHead, void *pMsgItem, void *pCallbackParams)
{
	ParserOES* parser = (ParserOES*)pCallbackParams;
	if (parser == NULL)
		return 0;

	parser->doOnMessage(pMsgHead, pMsgItem);

	return 0;
}


ParserOES::ParserOES()
	: _inited(false)
	, _sink(NULL)
	, _udp(false)
	, _context(NULL)
{
}


ParserOES::~ParserOES()
{
}

bool ParserOES::init( WTSVariant* config )
{
	_config = config->getCString("config");
	_gpsize = config->getUInt32("gpsize");
	_udp = config->getBoolean("udp");
	if (_gpsize == 0)
		_gpsize = 1000;

	std::string module = config->getCString("oesmodule");
	if (module.empty())
		module = "oes_api";

	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");
	DLLHelper::load_library(dllpath.c_str());

	return true;
}

void ParserOES::release()
{
	if (_context == NULL)
		return;

	MdsAsyncApi_Stop(_context);

	while (!MdsAsyncApi_IsAllTerminated(_context)) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	MdsAsyncApi_ReleaseContext(_context);
	_context = NULL;
}

void ParserOES::doSubscribe()
{

}

bool ParserOES::connect()
{
	if (_context != NULL)
		return true;

	_context = MdsAsyncApi_CreateContext(_config.c_str());
	if(_context == NULL)
	{
		write_log(_sink, LL_ERROR, "[ParserOES] Creating api failed");
		return false;
	}

	if(_udp)
	{
		MdsAsyncApiChannelT *channel = MdsAsyncApi_AddChannelFromFile(
			_context, "udp_snap1_SH",
			_config.c_str(), MDSAPI_CFG_DEFAULT_SECTION,
			MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP1,
			on_message, this,
			on_connect, this,
			on_disconnect, this);
		if (!channel) 
		{
			write_log(_sink, LL_WARN, "[ParserOES] Loading section {} from config failed", MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP1);
		}

		channel = MdsAsyncApi_AddChannelFromFile(
			_context, "udp_snap1_SZ",
			_config.c_str(), MDSAPI_CFG_DEFAULT_SECTION,
			MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP2,
			on_message, this,
			on_connect, this,
			on_disconnect, this);
		if (!channel)
		{
			write_log(_sink, LL_WARN, "[ParserOES] Loading section {} from config failed", MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP2);
		}

		channel = MdsAsyncApi_AddChannelFromFile(
			_context, "udp_tick1_SH",
			_config.c_str(), MDSAPI_CFG_DEFAULT_SECTION,
			MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK1,
			on_message, this,
			on_connect, this,
			on_disconnect, this);
		if (!channel)
		{
			write_log(_sink, LL_WARN, "[ParserOES] Loading section {} from config failed", MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK1);
		}

		channel = MdsAsyncApi_AddChannelFromFile(
			_context, "udp_tick1_SZ",
			_config.c_str(), MDSAPI_CFG_DEFAULT_SECTION,
			MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK2,
			on_message, this,
			on_connect, this,
			on_disconnect, this);
		if (!channel)
		{
			write_log(_sink, LL_WARN, "[ParserOES] Loading section {} from config failed", MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK2);
		}
	}
	else
	{
		MdsAsyncApiChannelT *channel = MdsAsyncApi_AddChannelFromFile(
			_context, "tcp_channel",
			_config.c_str(), MDSAPI_CFG_DEFAULT_SECTION,
			MDSAPI_CFG_DEFAULT_KEY_TCP_ADDR,
			on_message, this,
			on_connect, this,
			on_disconnect, this);
		if (!channel)
		{
			write_log(_sink, LL_WARN, "[ParserOES] Loading section {} from config failed", MDSAPI_CFG_DEFAULT_KEY_TCP_ADDR);
		}
	}

	MdsAsyncApi_Start(_context);

	return true;
}

bool ParserOES::disconnect()
{
	MdsAsyncApi_Stop(_context);
	return true;
}

bool ParserOES::isConnected()
{
	return MdsAsyncApi_IsRunning(_context);
}

void ParserOES::subscribe( const CodeSet &vecSymbols )
{
	auto cit = vecSymbols.begin();
	for(; cit != vecSymbols.end(); cit++)
	{
		const auto &code = *cit;
		if(_set_subs.find(code) == _set_subs.end())
		{
			_set_subs.insert(code);
		}
	}
}

void ParserOES::unsubscribe(const CodeSet &setSymbols)
{

}

void ParserOES::registerSpi( IParserSpi* listener )
{
	bool bReplaced = (_sink!=NULL);
	_sink = listener;
	if(bReplaced && _sink)
	{
		write_log(_sink, LL_WARN, "Listener is replaced");
	}

	if (_sink)
		_bd_mgr = _sink->getBaseDataMgr();
}

void ParserOES::doOnConnected(MdsAsyncApiChannelT *pAsyncChannel)
{
	if (_udp)
	{
		write_log(_sink, LL_ERROR, "[ParserOES] channel {} connected", pAsyncChannel->pChannelCfg->channelTag);

		if (_inited)
			return;

		/*
		 *	By Wesley @ 2022.07.26
		 *	UDP模式下会出现多次调用的情况
		 */
		_inited = true;
		if (_sink)
		{
			_sink->handleEvent(WPE_Connect, 0);
			_sink->handleEvent(WPE_Login, 0);
		}
		return;
	}

	//TCP模式下，只会有一次调用
	if (_sink)
	{
		_sink->handleEvent(WPE_Connect, 0);
		_sink->handleEvent(WPE_Login, 0);
	}

	/* 订阅的数据类型 (dataTypes) 会以最后一次订阅为准, 所以每次都需要指定为所有待订阅的数据类型 */
	int32                   dataTypes =
		MDS_SUB_DATA_TYPE_INDEX_SNAPSHOT
		| MDS_SUB_DATA_TYPE_OPTION_SNAPSHOT
		| MDS_SUB_DATA_TYPE_L2_SNAPSHOT
		| MDS_SUB_DATA_TYPE_L2_BEST_ORDERS
		| MDS_SUB_DATA_TYPE_L2_ORDER
		| MDS_SUB_DATA_TYPE_L2_SSE_ORDER
		| MDS_SUB_DATA_TYPE_L2_TRADE;

	/* 设置SubscribeByString接口使用的数据模式 (tickType=1) */
	MdsApi_SetThreadSubscribeTickType(MDS_TICK_TYPE_LATEST_TIMELY);

	/* 设置SubscribeByString接口使用的逐笔数据的数据重建标识 (实时行情+重建数据) */
	MdsApi_SetThreadSubscribeTickRebuildFlag(
		MDS_TICK_REBUILD_FLAG_INCLUDE_REBUILDED);

	/* 设置SubscribeByString接口使用的初始快照订阅标志 (isRequireInitialMktData) */
	MdsApi_SetThreadSubscribeRequireInitMd(FALSE);

	/* 订阅所有上海股票/债券/基金的 Level-2 行情 */
	if (!MdsAsyncApi_SubscribeByString(pAsyncChannel,
		(char *)NULL, (char *)NULL,
		MDS_EXCH_SSE, MDS_MD_PRODUCT_TYPE_STOCK, MDS_SUB_MODE_SET,
		dataTypes)) 
	{
		write_log(_sink, LL_WARN, "[ParserOES] Subscribe stock quotes of SSE failed");
	}

	/* 追加订阅所有上海指数行情 */
	if (!MdsAsyncApi_SubscribeByString(pAsyncChannel,
		(char *)NULL, (char *)NULL,
		MDS_EXCH_SSE, MDS_MD_PRODUCT_TYPE_INDEX, MDS_SUB_MODE_APPEND,
		dataTypes)) 
	{
		write_log(_sink, LL_WARN, "[ParserOES] Subscribe index quotes of SSE failed");
	}

	/* 追加订阅所有上海期权行情 */
	if (!MdsAsyncApi_SubscribeByString(pAsyncChannel,
		(char *)NULL, (char *)NULL,
		MDS_EXCH_SSE, MDS_MD_PRODUCT_TYPE_OPTION, MDS_SUB_MODE_APPEND,
		dataTypes)) 
	{
		write_log(_sink, LL_WARN, "[ParserOES] Subscribe option quotes of SSE failed");
	}

	/* 追加订阅所有深圳股票/债券/基金的 Level-2 行情 */
	if (!MdsAsyncApi_SubscribeByString(pAsyncChannel,
		(char *)NULL, (char *)NULL,
		MDS_EXCH_SZSE, MDS_MD_PRODUCT_TYPE_STOCK, MDS_SUB_MODE_APPEND,
		dataTypes)) 
	{
		write_log(_sink, LL_WARN, "[ParserOES] Subscribe stock quotes of SZSE failed");
	}

	/* 追加订阅所有深圳指数行情 */
	if (!MdsAsyncApi_SubscribeByString(pAsyncChannel,
		(char *)NULL, (char *)NULL,
		MDS_EXCH_SZSE, MDS_MD_PRODUCT_TYPE_INDEX, MDS_SUB_MODE_APPEND,
		dataTypes)) 
	{
		write_log(_sink, LL_WARN, "[ParserOES] Subscribe index quotes of SZSE failed");
	}

	/* 追加订阅所有深圳期权行情 */
	if (!MdsAsyncApi_SubscribeByString(pAsyncChannel,
		(char *)NULL, (char *)NULL,
		MDS_EXCH_SZSE, MDS_MD_PRODUCT_TYPE_OPTION, MDS_SUB_MODE_APPEND,
		dataTypes)) 
	{
		write_log(_sink, LL_WARN, "[ParserOES] Subscribe option quotes of SZSE failed");
	}
}

void ParserOES::doOnDisconnected(MdsAsyncApiChannelT *pAsyncChannel)
{
	if(_udp)
	{
		write_log(_sink, LL_ERROR, "[ParserOES] channel {} disconnected", pAsyncChannel->pChannelCfg->channelTag);
		return;
	}

	if (_sink)
	{
		_sink->handleEvent(WPE_Close, 0);
	}
}

#define wrapPrice(x) ((x)/10000.0)

void ParserOES::doOnMessage(SMsgHeadT *pMsgHead, void *pMsgItem)
{
	if (pMsgHead == NULL || pMsgItem == NULL)
		return;

	if (_bd_mgr == NULL)
		return;


	MdsMktRspMsgBodyT   *pRspMsg = (MdsMktRspMsgBodyT *)pMsgItem;

	/* 根据消息类型对行情消息进行处理 */
	switch (pMsgHead->msgId) {
	case MDS_MSGTYPE_L2_TRADE:
		/* 处理Level2逐笔成交消息 @see MdsL2TradeT */
		{
			std::string code, exchg;
			if (pRspMsg->trade.exchId == MDS_EXCH_SSE)
			{
				exchg = "SSE";
			}
			else
			{
				exchg = "SZSE";
			}
			code = pRspMsg->trade.SecurityID;

			WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
			if (ct == NULL)
			{
				return;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			WTSTransData *trans = WTSTransData::create(code.c_str());
			WTSTransStruct& ts = trans->getTransStruct();
			strcpy(ts.exchg, commInfo->getExchg());

			ts.trading_date = pRspMsg->trade.tradeDate;
			ts.action_date = pRspMsg->trade.tradeDate;
			ts.action_time = pRspMsg->trade.TransactTime;

			ts.index = pRspMsg->trade.ApplSeqNum;
			ts.side = wrapTransSide(pRspMsg->trade.TradeBSFlag);
			ts.ttype = pRspMsg->trade.ExecType == 'F' ? TT_Match : TT_Cancel;

			ts.price = wrapPrice(pRspMsg->trade.TradePrice);
			ts.volume = pRspMsg->trade.TradeQty;
			ts.bidorder = pRspMsg->trade.BidApplSeqNum;
			ts.askorder = pRspMsg->trade.OfferApplSeqNum;

			if (_sink)
				_sink->handleTransaction(trans);

			static uint32_t recv_cnt = 0;
			recv_cnt++;
			if (recv_cnt % _gpsize == 0)
				write_log(_sink, LL_DEBUG, "[ParserOES] {} transactions received in total", recv_cnt);
		}
		break;

	case MDS_MSGTYPE_L2_ORDER:
	case MDS_MSGTYPE_L2_SSE_ORDER:
		/* 处理Level2逐笔委托消息 @see MdsL2OrderT */
		{
			std::string code, exchg;
			if (pRspMsg->order.exchId == MDS_EXCH_SSE)
			{
				exchg = "SSE";
			}
			else
			{
				exchg = "SZSE";
			}
			code = pRspMsg->order.SecurityID;

			WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
			if (ct == NULL)
			{
				return;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			WTSOrdDtlData *ordDtl = WTSOrdDtlData::create(code.c_str());
			WTSOrdDtlStruct& ts = ordDtl->getOrdDtlStruct();
			strcpy(ts.exchg, commInfo->getExchg());

			ts.trading_date = pRspMsg->order.tradeDate;
			ts.action_date = pRspMsg->order.tradeDate;
			ts.action_time = pRspMsg->order.TransactTime;

			ts.index = pRspMsg->order.SseOrderNo;
			ts.side = wrapOrdDtlSide(pRspMsg->order.Side);
			ts.otype = pRspMsg->order.OrderType;

			ts.price = wrapPrice(pRspMsg->order.Price);
			ts.volume = pRspMsg->order.OrderQty;

			if (_sink)
				_sink->handleOrderDetail(ordDtl);

			static uint32_t recv_cnt = 0;
			recv_cnt++;
			if (recv_cnt % _gpsize == 0)
				write_log(_sink, LL_DEBUG, "[ParserOES] {} orders received in total", recv_cnt);
		}
		break;

	case MDS_MSGTYPE_L2_MARKET_DATA_SNAPSHOT:
		/* 处理Level2快照行情消息 @see MdsL2StockSnapshotBodyT */
		{
			std::string code, exchg;
			if (pRspMsg->mktDataSnapshot.head.exchId == MDS_EXCH_SSE)
			{
				exchg = "SSE";
			}
			else
			{
				exchg = "SZSE";
			}
			code = pRspMsg->mktDataSnapshot.l2Stock.SecurityID;

			WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
			if (ct == NULL)
			{
				//if (_sink)
				//	write_log(_sink, LL_ERROR, "[ParserXTP] Instrument {}.{} not exists...", exchg.c_str(), code);
				return;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			WTSTickData* tick = WTSTickData::create(code.c_str());
			tick->setContractInfo(ct);
			WTSTickStruct& quote = tick->getTickStruct();
			strcpy(quote.exchg, commInfo->getExchg());

			quote.trading_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_time = pRspMsg->mktDataSnapshot.head.updateTime;

			quote.price = wrapPrice(pRspMsg->mktDataSnapshot.l2Stock.TradePx);
			quote.open = wrapPrice(pRspMsg->mktDataSnapshot.l2Stock.OpenPx);
			quote.high = wrapPrice(pRspMsg->mktDataSnapshot.l2Stock.HighPx);
			quote.low = wrapPrice(pRspMsg->mktDataSnapshot.l2Stock.LowPx);
			quote.total_volume = (double)pRspMsg->mktDataSnapshot.l2Stock.TotalVolumeTraded;
			quote.total_turnover = pRspMsg->mktDataSnapshot.l2Stock.TotalValueTraded / 10000.0;

			quote.pre_close = wrapPrice(pRspMsg->mktDataSnapshot.l2Stock.PrevClosePx);

			//委卖价格
			for (int i = 0; i < 10; i++)
			{
				quote.ask_prices[i] = wrapPrice(pRspMsg->mktDataSnapshot.l2Stock.OfferLevels[i].Price);
				quote.ask_qty[i] = (double)pRspMsg->mktDataSnapshot.l2Stock.OfferLevels[i].OrderQty;

				quote.bid_prices[i] = wrapPrice(pRspMsg->mktDataSnapshot.l2Stock.BidLevels[i].Price);
				quote.bid_qty[i] = (double)pRspMsg->mktDataSnapshot.l2Stock.BidLevels[i].OrderQty;
			}

			if (_sink)
				_sink->handleQuote(tick, 1);

			tick->release();

			static uint32_t recv_cnt = 0;
			recv_cnt++;
			if (recv_cnt % _gpsize == 0)
				write_log(_sink, LL_DEBUG, "[ParserOES] {} L2 ticks received in total", recv_cnt);
		}
		break;

	case MDS_MSGTYPE_L2_BEST_ORDERS_SNAPSHOT:
		/* 处理Level2委托队列消息(买一／卖一前五十笔委托明细) @see MdsL2BestOrdersSnapshotBodyT */
		{
			std::string code, exchg;
			if (pRspMsg->mktDataSnapshot.head.exchId == MDS_EXCH_SSE)
			{
				exchg = "SSE";
			}
			else
			{
				exchg = "SZSE";
			}
			code = pRspMsg->mktDataSnapshot.l2BestOrders.SecurityID;

			WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
			if (ct == NULL)
			{
				//if (_sink)
				//	write_log(_sink, LL_ERROR, "[ParserXTP] Instrument {}.{} not exists...", exchg.c_str(), code);
				return;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			WTSOrdQueData* buyQue = WTSOrdQueData::create(code.c_str());
			buyQue->setContractInfo(ct);

			WTSOrdQueData* sellQue = WTSOrdQueData::create(code.c_str());
			sellQue->setContractInfo(ct);

			WTSOrdQueStruct& buyOS = buyQue->getOrdQueStruct();
			strcpy(buyOS.exchg, commInfo->getExchg());

			WTSOrdQueStruct& sellOS = sellQue->getOrdQueStruct();
			strcpy(sellOS.exchg, commInfo->getExchg());

			buyOS.trading_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			buyOS.action_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			buyOS.action_time = pRspMsg->mktDataSnapshot.head.updateTime;

			sellOS.trading_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			sellOS.action_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			sellOS.action_time = pRspMsg->mktDataSnapshot.head.updateTime;

			buyOS.side = BDT_Buy;
			buyOS.price = wrapPrice(pRspMsg->mktDataSnapshot.l2BestOrders.BestBidPrice);
			buyOS.order_items = pRspMsg->mktDataSnapshot.l2BestOrders.NoBidOrders;

			sellOS.side = BDT_Sell;
			sellOS.price = wrapPrice(pRspMsg->mktDataSnapshot.l2BestOrders.BestOfferPrice);
			sellOS.order_items = pRspMsg->mktDataSnapshot.l2BestOrders.NoOfferOrders;

			for(int i = 0; i < 50; i++)
			{
				if(pRspMsg->mktDataSnapshot.l2BestOrders.BidOrderQty[i] != 0)
				{
					buyOS.volumes[i] = pRspMsg->mktDataSnapshot.l2BestOrders.BidOrderQty[i];
					buyOS.qsize++;
				}
				
				if (pRspMsg->mktDataSnapshot.l2BestOrders.OfferOrderQty[i] != 0)
				{
					sellOS.volumes[i] = pRspMsg->mktDataSnapshot.l2BestOrders.OfferOrderQty[i];
					sellOS.qsize++;
				}
			}

			if (_sink)
			{
				_sink->handleOrderQueue(buyQue);
				_sink->handleOrderQueue(sellQue);
			}

			buyQue->release();
			sellQue->release();

			static uint32_t recv_cnt = 0;
			recv_cnt += 2;
			if (recv_cnt % _gpsize == 0)
				write_log(_sink, LL_DEBUG, "[ParserOES] {} queues received in total", recv_cnt);
		}
		break;

	case MDS_MSGTYPE_MARKET_DATA_SNAPSHOT_FULL_REFRESH:
		/* 处理Level1快照行情消息 @see MdsStockSnapshotBodyT */
		{
			std::string code, exchg;
			if (pRspMsg->mktDataSnapshot.head.exchId == MDS_EXCH_SSE)
			{
				exchg = "SSE";
			}
			else
			{
				exchg = "SZSE";
			}
			code = pRspMsg->mktDataSnapshot.stock.SecurityID;

			WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
			if (ct == NULL)
			{
				//if (_sink)
				//	write_log(_sink, LL_ERROR, "[ParserXTP] Instrument {}.{} not exists...", exchg.c_str(), code);
				return;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			WTSTickData* tick = WTSTickData::create(code.c_str());
			tick->setContractInfo(ct);
			WTSTickStruct& quote = tick->getTickStruct();
			strcpy(quote.exchg, commInfo->getExchg());

			quote.trading_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_time = pRspMsg->mktDataSnapshot.head.updateTime;

			quote.price = wrapPrice(pRspMsg->mktDataSnapshot.stock.TradePx);
			quote.open = wrapPrice(pRspMsg->mktDataSnapshot.stock.OpenPx);
			quote.high = wrapPrice(pRspMsg->mktDataSnapshot.stock.HighPx);
			quote.low = wrapPrice(pRspMsg->mktDataSnapshot.stock.LowPx);
			quote.total_volume = (double)pRspMsg->mktDataSnapshot.stock.TotalVolumeTraded;
			quote.total_turnover = pRspMsg->mktDataSnapshot.stock.TotalValueTraded / 10000.0;

			quote.pre_close = wrapPrice(pRspMsg->mktDataSnapshot.stock.PrevClosePx);

			//委卖价格
			for (int i = 0; i < 5; i++)
			{
				quote.ask_prices[i] = wrapPrice(pRspMsg->mktDataSnapshot.stock.OfferLevels[i].Price);
				quote.ask_qty[i] = (double)pRspMsg->mktDataSnapshot.stock.OfferLevels[i].OrderQty;

				quote.bid_prices[i] = wrapPrice(pRspMsg->mktDataSnapshot.stock.BidLevels[i].Price);
				quote.bid_qty[i] = (double)pRspMsg->mktDataSnapshot.stock.BidLevels[i].OrderQty;
			}

			if (_sink)
				_sink->handleQuote(tick, 1);

			tick->release();

			static uint32_t recv_cnt = 0;
			recv_cnt++;
			if (recv_cnt % _gpsize == 0)
				write_log(_sink, LL_DEBUG, "[ParserOES] {} L1 ticks received in total", recv_cnt);
		}
		break;

	case MDS_MSGTYPE_OPTION_SNAPSHOT_FULL_REFRESH:
		/* 处理期权快照行情消息 @see MdsStockSnapshotBodyT */
		{
			std::string code, exchg;
			if (pRspMsg->mktDataSnapshot.head.exchId == MDS_EXCH_SSE)
			{
				exchg = "SSE";
			}
			else
			{
				exchg = "SZSE";
			}
			code = pRspMsg->mktDataSnapshot.option.SecurityID;

			WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
			if (ct == NULL)
			{
				//if (_sink)
				//	write_log(_sink, LL_ERROR, "[ParserXTP] Instrument {}.{} not exists...", exchg.c_str(), code);
				return;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			WTSTickData* tick = WTSTickData::create(code.c_str());
			tick->setContractInfo(ct);
			WTSTickStruct& quote = tick->getTickStruct();
			strcpy(quote.exchg, commInfo->getExchg());

			quote.trading_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_time = pRspMsg->mktDataSnapshot.head.updateTime;

			quote.price = wrapPrice(pRspMsg->mktDataSnapshot.option.TradePx);
			quote.open = wrapPrice(pRspMsg->mktDataSnapshot.option.OpenPx);
			quote.high = wrapPrice(pRspMsg->mktDataSnapshot.option.HighPx);
			quote.low = wrapPrice(pRspMsg->mktDataSnapshot.option.LowPx);
			quote.total_volume = (double)pRspMsg->mktDataSnapshot.option.TotalVolumeTraded;
			quote.total_turnover = pRspMsg->mktDataSnapshot.option.TotalValueTraded / 10000.0;

			quote.pre_close = wrapPrice(pRspMsg->mktDataSnapshot.option.PrevClosePx);
			quote.open_interest = (double)pRspMsg->mktDataSnapshot.option.TotalLongPosition;

			//委卖价格
			for (int i = 0; i < 5; i++)
			{
				quote.ask_prices[i] = wrapPrice(pRspMsg->mktDataSnapshot.option.OfferLevels[i].Price);
				quote.ask_qty[i] = (double)pRspMsg->mktDataSnapshot.option.OfferLevels[i].OrderQty;

				quote.bid_prices[i] = wrapPrice(pRspMsg->mktDataSnapshot.option.BidLevels[i].Price);
				quote.bid_qty[i] = (double)pRspMsg->mktDataSnapshot.option.BidLevels[i].OrderQty;
			}

			if (_sink)
				_sink->handleQuote(tick, 1);

			tick->release();

			static uint32_t recv_cnt = 0;
			recv_cnt++;
			if (recv_cnt % _gpsize == 0)
				write_log(_sink, LL_DEBUG, "[ParserOES] {} Option ticks received in total", recv_cnt);
		}
		break;

	case MDS_MSGTYPE_INDEX_SNAPSHOT_FULL_REFRESH:
		/* 处理指数行情消息 @see MdsIndexSnapshotBodyT */
		{
			std::string code, exchg;
			if (pRspMsg->mktDataSnapshot.head.exchId == MDS_EXCH_SSE)
			{
				exchg = "SSE";
			}
			else
			{
				exchg = "SZSE";
			}
			code = pRspMsg->mktDataSnapshot.index.SecurityID;

			WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
			if (ct == NULL)
			{
				return;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			WTSTickData* tick = WTSTickData::create(code.c_str());
			tick->setContractInfo(ct);
			WTSTickStruct& quote = tick->getTickStruct();
			strcpy(quote.exchg, commInfo->getExchg());

			quote.trading_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_date = pRspMsg->mktDataSnapshot.head.tradeDate;
			quote.action_time = pRspMsg->mktDataSnapshot.head.updateTime;

			quote.price = wrapPrice(pRspMsg->mktDataSnapshot.index.LastIdx);
			quote.open = wrapPrice(pRspMsg->mktDataSnapshot.index.OpenIdx);
			quote.high = wrapPrice(pRspMsg->mktDataSnapshot.index.HighIdx);
			quote.low = wrapPrice(pRspMsg->mktDataSnapshot.index.LowIdx);
			quote.total_volume = (double)pRspMsg->mktDataSnapshot.index.TotalVolumeTraded;
			quote.total_turnover = pRspMsg->mktDataSnapshot.index.TotalValueTraded/10000.0;

			quote.pre_close = wrapPrice(pRspMsg->mktDataSnapshot.index.PrevCloseIdx);

			if (_sink)
				_sink->handleQuote(tick, 1);

			tick->release();

			static uint32_t recv_cnt = 0;
			recv_cnt++;
			if (recv_cnt % _gpsize == 0)
				write_log(_sink, LL_DEBUG, "[ParserOES] {} index ticks received in total", recv_cnt);
		}
		break;

	case MDS_MSGTYPE_SECURITY_STATUS:
		/* 处理(深圳)证券状态消息 @see MdsSecurityStatusMsgT */
		break;

	case MDS_MSGTYPE_TRADING_SESSION_STATUS:
		/* 处理(上证)市场状态消息 @see MdsTradingSessionStatusMsgT */
		break;

	case MDS_MSGTYPE_MARKET_DATA_REQUEST:
		/* 处理行情订阅请求的应答消息 @see MdsMktDataRequestRspT */
		if (pMsgHead->status != 0)
		{
			write_log(_sink, LL_ERROR, "recv subscribe-request response, subscription failed! errCode[{} - {}]", pMsgHead->status, pMsgHead->detailStatus);
		}
		break;
	case MDS_MSGTYPE_HEARTBEAT:
		/* 直接忽略心跳消息即可 */
		write_log(_sink, LL_DEBUG, "[ParserOES] Heartbeating");
		break;

	case MDS_MSGTYPE_COMPRESSED_PACKETS:
		/* @note 接收到了压缩后的行情数据!
		 * - 对接压缩行情需要使用 MdsApi_WaitOnMsgCompressible 等 Compressible 接口
		 * - 对于异步API需要检查是否开启了 isCompressible 标志
		 */
		break;

	default:
		break;
	}
}