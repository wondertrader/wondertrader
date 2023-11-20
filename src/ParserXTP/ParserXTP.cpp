/*!
 * \file ParserXTP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserXTP.h"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(IParserSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	memset(buffer, 0, 512);
	fmt::format_to(buffer, format, args...);

	sink->handleParserLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserXTP* parser = new ParserXTP();
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

inline uint32_t strToTime(const char* strTime)
{
	std::string str;
	const char *pos = strTime;
	while(strlen(pos) > 0)
	{
		if(pos[0] != ':')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	return strtoul(str.c_str(), NULL, 10);
}

inline double checkValid(double val)
{
	if (val == DBL_MAX || val == FLT_MAX)
		return 0;

	return val;
}

ParserXTP::ParserXTP()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
{
}


ParserXTP::~ParserXTP()
{
	m_pUserAPI = NULL;
}

bool ParserXTP::init(WTSVariant* config)
{
	m_strHost	= config->getCString("host");
	m_iPort		= config->getInt32("port");
	m_strUser = config->getCString("user");
	m_strPass = config->getCString("pass");
	m_iProtocol = (XTP_PROTOCOL_TYPE)config->getUInt32("protocol");
	m_uClientID = config->getUInt32("clientid");
	m_uHBInterval = config->getUInt32("hbinterval");
	m_uBuffSize = config->getUInt32("buffsize");
	m_strFlowDir = config->getCString("flowdir");
	m_strLocalIP = config->getCString("local_ip");

	if (m_strFlowDir.empty())
		m_strFlowDir = "XTPMDFlow";

	m_strFlowDir = StrUtil::standardisePath(m_strFlowDir);

	std::string module = config->getCString("xtpmodule");
	if (module.empty())
		module = "xtpquoteapi";

	std::string path = StrUtil::printf("%s/%s/", m_strFlowDir.c_str(), m_strUser.c_str());
	boost::filesystem::create_directories(path.c_str());

	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;
	m_hInst = DLLHelper::load_library(dllpath.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateQuoteApi@QuoteApi@API@XTP@@SAPEAV123@EPEBDW4XTP_LOG_LEVEL@@@Z";
#	else
	const char* creatorName = "?CreateQuoteApi@QuoteApi@API@XTP@@SAPAV123@EPBDW4XTP_LOG_LEVEL@@@Z";
#	endif
#else
	const char* creatorName = "_ZN3XTP3API8QuoteApi14CreateQuoteApiEhPKc13XTP_LOG_LEVEL";
#endif
	m_funcCreator = (XTPCreater)DLLHelper::get_symbol(m_hInst, creatorName);
	m_pUserAPI = m_funcCreator(m_uClientID, path.c_str(), XTP_LOG_LEVEL_DEBUG);
	m_pUserAPI->RegisterSpi(this);

	return true;
}

void ParserXTP::release()
{
	disconnect();
}

bool ParserXTP::connect()
{
	DoLogin();

	if (_thrd_worker == NULL)
	{
		//boost::asio::io_service::work work(_asyncio);
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

	return true;
}

bool ParserXTP::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	return true;
}

void ParserXTP::OnError(XTPRI *error_info)
{
	if(IsErrorRspInfo(error_info))
	{
		write_log(m_sink, LL_ERROR, "[ParserXTP] Error occured: ({}){}", error_info->error_id, error_info->error_msg);
	}
}

void ParserXTP::OnDisconnected(int nReason)
{
	if(m_sink)
	{
		write_log(m_sink, LL_ERROR, "[ParserXTP] Market data server disconnected: {}...", nReason);
		m_sink->handleEvent(WPE_Close, 0);
	}

	_asyncio.post([this]() {
		write_log(m_sink, LL_WARN, "[ParserXTP] Connection lost, relogin in 2 seconds...");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		DoLogin();
	});
}

void ParserXTP::OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last)
{

}

void ParserXTP::OnTickByTick(XTPTBT *tbt_data)
{
	if (m_pBaseDataMgr == NULL)
	{
		return;
	}

	std::string exchg;
	if (tbt_data->exchange_id == XTP_EXCHANGE_SH)
	{
		exchg = "SSE";
	}
	else
	{
		exchg = "SZSE";
	}

	WTSContractInfo* ct = m_pBaseDataMgr->getContract(tbt_data->ticker, exchg.c_str());
	if (ct == NULL)
	{
		if (m_sink)
			write_log(m_sink, LL_ERROR, "[ParserXTP] Instrument {}.{} not exists...", exchg.c_str(), tbt_data->ticker);
		return;
	}
	WTSCommodityInfo* commInfo = ct->getCommInfo();

	uint32_t actDate = (uint32_t)(tbt_data->data_time / 1000000000);
	uint32_t actTime = tbt_data->data_time % 1000000000;
	uint32_t actHour = actTime / 10000000;

	if(tbt_data->type == XTP_TBT_ENTRUST)
	{
		WTSOrdDtlData *ordDtl = WTSOrdDtlData::create(tbt_data->ticker);
		ordDtl->setContractInfo(ct);
		WTSOrdDtlStruct& ts = ordDtl->getOrdDtlStruct();
		strcpy(ts.exchg, commInfo->getExchg());

		const XTPTickByTickEntrust& eInfo = tbt_data->entrust;

		ts.trading_date = actDate;
		ts.action_date = actDate;
		ts.action_time = actTime;

		ts.index = eInfo.seq;
		ts.side = eInfo.side;
		ts.otype = eInfo.ord_type;

		ts.price = eInfo.price;
		ts.volume = (uint32_t)eInfo.qty;

		if (m_sink)
			m_sink->handleOrderDetail(ordDtl);
	}
	else if (tbt_data->type == XTP_TBT_TRADE)
	{
		WTSTransData *trans = WTSTransData::create(tbt_data->ticker);
		trans->setContractInfo(ct);
		WTSTransStruct& ts = trans->getTransStruct();
		strcpy(ts.exchg, commInfo->getExchg());

		const XTPTickByTickTrade& tInfo = tbt_data->trade;

		ts.trading_date = actDate;
		ts.action_date = actDate;
		ts.action_time = actTime;

		ts.index = tInfo.seq;
		ts.side = tInfo.trade_flag;
		ts.ttype = tInfo.trade_flag == '4' ? TT_Cancel : TT_Match;

		ts.price = tInfo.price;
		ts.volume = (uint32_t)tInfo.qty;
		ts.bidorder = tInfo.bid_no;
		ts.askorder = tInfo.ask_no;

		if (m_sink)
			m_sink->handleTransaction(trans);
	}
}

void ParserXTP::OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count)
{	
	if(m_pBaseDataMgr == NULL)
	{
		return;
	}

	uint32_t actDate = (uint32_t)(market_data->data_time / 1000000000);
	uint32_t actTime = market_data->data_time % 1000000000;
	uint32_t actHour = actTime / 10000000;

	std::string code, exchg;
	if (market_data->exchange_id == XTP_EXCHANGE_SH)
	{
		exchg = "SSE";
	}
	else
	{
		exchg = "SZSE";
	}
	code = market_data->ticker;

	WTSContractInfo* ct = m_pBaseDataMgr->getContract(code.c_str(), exchg.c_str());
	if(ct == NULL)
	{
		if (m_sink)
			write_log(m_sink, LL_ERROR, "[ParserXTP] Instrument {}.{} not exists...", exchg.c_str(), market_data->ticker);
		return;
	}
	WTSCommodityInfo* commInfo = ct->getCommInfo();

	WTSTickData* tick = WTSTickData::create(code.c_str());
	tick->setContractInfo(ct);
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, commInfo->getExchg());
	
	quote.action_date = actDate;
	quote.action_time = actTime;
	
	quote.price = checkValid(market_data->last_price);
	quote.open = checkValid(market_data->open_price);
	quote.high = checkValid(market_data->high_price);
	quote.low = checkValid(market_data->low_price);
	quote.total_volume = (uint32_t)market_data->qty;
	quote.trading_date = m_uTradingDate;
	quote.total_turnover = market_data->turnover;

	if (commInfo->getCategoty() == CC_Future)
	{
		quote.settle_price = market_data->settl_price;
		quote.open_interest = (uint32_t)market_data->total_long_positon;

		quote.pre_settle = checkValid(market_data->pre_settl_price);
		quote.pre_interest = (uint32_t)market_data->pre_total_long_positon;
	}

	quote.upper_limit = checkValid(market_data->upper_limit_price);
	quote.lower_limit = checkValid(market_data->lower_limit_price);

	quote.pre_close = checkValid(market_data->pre_close_price);	

	//委卖价格
	for (int i = 0; i < 10; i++)
	{
		quote.ask_prices[i] = checkValid(market_data->ask[i]);
		quote.ask_qty[i] = (uint32_t)market_data->ask_qty[i];

		quote.bid_prices[i] = checkValid(market_data->bid[i]);
		quote.bid_qty[i] = (uint32_t)market_data->bid_qty[i];
	}

	if(m_sink)
		m_sink->handleQuote(tick, 1);

	//处理逐笔
	if(bid1_count > 0)
	{
		WTSOrdQueData* buyQue = WTSOrdQueData::create(code.c_str());
		buyQue->setContractInfo(ct);

		WTSOrdQueStruct& buyOS = buyQue->getOrdQueStruct();
		strcpy(buyOS.exchg, commInfo->getExchg());

		buyOS.trading_date = m_uTradingDate;
		buyOS.action_date = actDate;
		buyOS.action_time = actTime;

		buyOS.side = BDT_Buy;
		buyOS.price = quote.bid_prices[0];
		buyOS.order_items = max_bid1_count;
		buyOS.qsize = std::min(bid1_count,50);

		for (uint32_t i = 0; i < buyOS.qsize; i++)
		{
			buyOS.volumes[i] = (uint32_t)bid1_qty[i];
		}

		if (m_sink)
			m_sink->handleOrderQueue(buyQue);

		buyQue->release();
	}
	
	if(ask1_count > 0)
	{
		WTSOrdQueData* sellQue = WTSOrdQueData::create(code.c_str());
		sellQue->setContractInfo(ct);

		WTSOrdQueStruct& sellOS = sellQue->getOrdQueStruct();
		strcpy(sellOS.exchg, commInfo->getExchg());

		sellOS.trading_date = m_uTradingDate;
		sellOS.action_date = actDate;
		sellOS.action_time = actTime;

		sellOS.side = BDT_Sell;
		sellOS.price = quote.ask_prices[0];
		sellOS.order_items = max_ask1_count;
		sellOS.qsize = std::min(ask1_count, 50);

		for (uint32_t i = 0; i < sellOS.qsize; i++)
		{
			sellOS.volumes[i] = (uint32_t)ask1_qty[i];
		}

		if (m_sink)	
			m_sink->handleOrderQueue(sellQue);

		sellQue->release();
	}

	tick->release();
}

void ParserXTP::OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last)
{
	if (!IsErrorRspInfo(error_info))
	{

	}
	else
	{
		if(m_sink)
			write_log(m_sink, LL_ERROR, "[ParserXTP] Market data subscribe failed, code: {}.{}, err code: {}, err msg: {}", ticker->exchange_id == XTP_EXCHANGE_SH ? "SSE" : "SZSE", ticker->ticker, error_info->error_id, error_info->error_msg);
	}
}

void ParserXTP::OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last)
{
	if (!IsErrorRspInfo(error_info))
	{

	}
	else
	{
		if (m_sink)
			write_log(m_sink, LL_ERROR, "[ParserXTP] Tick-by-tick data subscribe failed, code: {}.{}, err code: {}, err msg: {}", ticker->exchange_id == XTP_EXCHANGE_SH ? "SSE" : "SZSE", ticker->ticker, error_info->error_id, error_info->error_msg);
	}
}

void ParserXTP::DoLogin()
{
	if(m_pUserAPI == NULL)
	{
		return;
	}

	m_pUserAPI->SetHeartBeatInterval(m_uHBInterval);
	m_pUserAPI->SetUDPBufferSize(m_uBuffSize);
	int iResult = m_pUserAPI->Login(m_strHost.c_str(), m_iPort, m_strUser.c_str(), m_strPass.c_str(), m_iProtocol, m_strLocalIP.c_str());
	if(iResult != 0)
	{
		if (m_sink)
		{
			auto error_info = m_pUserAPI->GetApiLastError();
			if(iResult == -1)
			{
				_asyncio.post([this, iResult] {
					m_sink->handleEvent(WPE_Connect, iResult);
				});

				write_log(m_sink, LL_ERROR, "[ParserXTP] Connecting server failed: {}", error_info->error_msg);
			}
			else
			{
				m_sink->handleEvent(WPE_Connect, 0);
				_asyncio.post([this, iResult] {
					m_sink->handleEvent(WPE_Connect, 0);
				});

				write_log(m_sink, LL_ERROR, "[ParserXTP] Sending login request failed: {}", error_info->error_msg);
			}			
		}
	}
	else
	{
		m_uTradingDate = strToTime(m_pUserAPI->GetTradingDay());
		_asyncio.post([this] {
			if (m_sink)
			{
				m_sink->handleEvent(WPE_Connect, 0);
				m_sink->handleEvent(WPE_Login, 0);
			}
		});
		

		write_log(m_sink, LL_INFO, "[ParserXTP] Connecting server successed: {}, begin to subscibe data ...", iResult);

		DoSubscribeMD();
	}
}

void ParserXTP::DoSubscribeMD()
{
	CodeSet codeFilter = m_fitSHSubs;
	if(!codeFilter.empty())
	{
		char ** subscribe = new char*[codeFilter.size()];
		int nCount = 0;
		CodeSet::iterator it = codeFilter.begin();
		for (; it != codeFilter.end(); it++)
		{
			subscribe[nCount++] = (char*)(*it).c_str();
		}

		if (m_pUserAPI && nCount > 0)
		{
			int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SH);
			if (iResult != 0)
			{
				if (m_sink)
					write_log(m_sink, LL_ERROR, "[ParserXTP] Sending md subscribe request of SSE failed: {}", iResult);
			}
			else
			{
				if (m_sink)
					write_log(m_sink, LL_INFO, "[ParserXTP] Market data of {} instruments of SSE subscribed", nCount);
			}

			iResult = m_pUserAPI->SubscribeTickByTick(subscribe, nCount, XTP_EXCHANGE_SH);
			if (iResult != 0)
			{
				if (m_sink)
					write_log(m_sink, LL_ERROR, "[ParserXTP] Sending tick_by_tick subscribe request of SSE failed: {}", iResult);
			}
			else
			{
				if (m_sink)
					write_log(m_sink, LL_INFO, "[ParserXTP] Tick_by_tick data of {} instruments of SSE subscribed", nCount);
			}
		}
		codeFilter.clear();
		delete[] subscribe;
		//int iResult = m_pUserAPI->SubscribeAllMarketData(XTP_EXCHANGE_SH);
	}

	codeFilter = m_fitSZSubs;
	if (!codeFilter.empty())
	{
		char ** subscribe = new char*[codeFilter.size()];
		int nCount = 0;
		CodeSet::iterator it = codeFilter.begin();
		for (; it != codeFilter.end(); it++)
		{
			subscribe[nCount++] = (char*)(*it).c_str();
		}

		if (m_pUserAPI && nCount > 0)
		{
			int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SZ);
			if (iResult != 0)
			{
				if (m_sink)
					write_log(m_sink, LL_ERROR, "[ParserXTP] Sending md subscribe request of SZSE failed: {}", iResult);
			}
			else
			{
				if (m_sink)
					write_log(m_sink, LL_INFO, "[ParserXTP] Market data of {} instruments of SZSE subscribed", nCount);
			}

			iResult = m_pUserAPI->SubscribeTickByTick(subscribe, nCount, XTP_EXCHANGE_SZ);
			if (iResult != 0)
			{
				if (m_sink)
					write_log(m_sink, LL_ERROR, "[ParserXTP] Sending tick_by_tick subscribe request of SZSE failed: {}", iResult);
			}
			else
			{
				if (m_sink)
					write_log(m_sink, LL_INFO, "[ParserXTP] Tick_by_tick data of {} instruments of SZSE subscribed", nCount);
			}
		}
		codeFilter.clear();
		delete[] subscribe;
		//int iResult = m_pUserAPI->SubscribeAllMarketData(XTP_EXCHANGE_SZ);
	}
}

bool ParserXTP::IsErrorRspInfo(XTPRI *error_info)
{
	if (error_info == NULL || error_info->error_id ==0)
		return false;

	return true;
}

void ParserXTP::subscribe(const CodeSet &vecSymbols)
{
	if(m_uTradingDate == 0)
	{
		for(auto& code : vecSymbols)
		{
			if (strncmp(code.c_str(), "SSE.", 4) == 0)
			{
				m_fitSHSubs.insert(code.c_str() + 4);
			}
			else if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				m_fitSZSubs.insert(code.c_str() + 5);
			}
		}
	}
	else
	{
		CodeSet setSH, setSZ;
		for (auto& code : vecSymbols)
		{
			if (strncmp(code.c_str(), "SSE.", 4) == 0)
			{
				m_fitSHSubs.insert(code.c_str() + 4);
				setSH.insert(code.c_str() + 4);
			}
			else if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				m_fitSZSubs.insert(code.c_str() + 5);
				setSZ.insert(code.c_str() + 5);
			}
		}

		if (!setSH.empty())
		{
			char ** subscribe = new char*[setSH.size()];
			int nCount = 0;
			CodeSet::iterator it = setSH.begin();
			for (; it != setSH.end(); it++)
			{
				subscribe[nCount++] = (char*)(*it).c_str();
			}

			if (m_pUserAPI && nCount > 0)
			{
				int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SH);
				if (iResult != 0)
				{
					if (m_sink)
						write_log(m_sink, LL_ERROR, "[ParserXTP] Sending md subscribe request of SSE failed: {}", iResult);
				}
				else
				{
					if (m_sink)
						write_log(m_sink, LL_INFO, "[ParserXTP] Market data of {} instruments of SSE subscribed", nCount);
				}
			}
			delete[] subscribe;
		}

		if (!setSZ.empty())
		{
			char ** subscribe = new char*[setSZ.size()];
			int nCount = 0;
			CodeSet::iterator it = setSZ.begin();
			for (; it != setSZ.end(); it++)
			{
				subscribe[nCount++] = (char*)(*it).c_str();
			}

			if (m_pUserAPI && nCount > 0)
			{
				int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SZ);
				if (iResult != 0)
				{
					if (m_sink)
						write_log(m_sink, LL_ERROR, "[ParserXTP] Sending md subscribe request of SZSE failed: {}", iResult);
				}
				else
				{
					if (m_sink)
						write_log(m_sink, LL_INFO, "[ParserXTP] Market data of {} instruments of SZSE subscribed", nCount);
				}
			}
			delete[] subscribe;
		}
	}
}

void ParserXTP::unsubscribe(const CodeSet &vecSymbols)
{
}

bool ParserXTP::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserXTP::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if(m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}