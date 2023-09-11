/*!
 * \file ParserHuaX.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#include "ParserHuaX.h"
#include "../Includes/WTSVersion.h"

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
		ParserHuaX* parser = new ParserHuaX();
		return parser;
	}

	EXPORT_FLAG void deleteParser(IParserApi*& parser)
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
	const char* pos = strTime;
	while (strlen(pos) > 0)
	{
		if (pos[0] != ':')
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

ParserHuaX::ParserHuaX()
	:_api(NULL)
	, _iRequestID(0)
	, _uTradingDate(0)
{
}


ParserHuaX::~ParserHuaX()
{
	_api = NULL;
}

bool ParserHuaX::init(WTSVariant* config)
{
	_front = config->getCString("front");
	_strUser = config->getCString("user");
	_strPass = config->getCString("pass");
	_strFlowDir = config->getCString("flowdir");
	_sub_mode = config->getCString("sub_mode");
	_derive_sub_mode = config->getCString("derive_sub_mode");

	if (_strFlowDir.empty())
		_strFlowDir = "HuaXMDFlow";

	if (_sub_mode.empty())
		_sub_mode = "0";

	if (_derive_sub_mode.empty())
		_derive_sub_mode = "0";

	_strFlowDir = StrUtil::standardisePath(_strFlowDir);

	std::string module = config->getCString("xtpmodule");
	if (module.empty())
		module = "xfastmdapi";

	std::string path = StrUtil::printf("%s/%s/", _strFlowDir.c_str(), _strUser.c_str());
	boost::filesystem::create_directories(path.c_str());

	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;
	_hInst = DLLHelper::load_library(dllpath.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateTstpXMdApi@CTORATstpXMdApi@TORALEV1API@@SAPEAV12@AEBD0@Z";
#	else
	const char* creatorName = "?CreateTstpXMdApi@CTORATstpXMdApi@TORALEV1API@@SAPAV12@ABD0@Z";
#	endif
#else
	// TODO linux下未修改
	const char* creatorName = "_ZN11TORALEV1API15CTORATstpXMdApi16CreateTstpXMdApiERKcS2_";
#endif
	_funcCreator = (HuaXCreater)DLLHelper::get_symbol(_hInst, creatorName);
	_api = _funcCreator((TTORATstpMDSubModeType)_sub_mode.c_str()[0], (TTORATstpMDSubModeType)_derive_sub_mode.c_str()[0]);
	_api->RegisterSpi(this);

	return true;
}

void ParserHuaX::release()
{
	disconnect();
}

bool ParserHuaX::connect()
{
	_api->RegisterFront((char*)_front.c_str());
	_api->Init();
	//_api->Join();
	return true;
}

bool ParserHuaX::disconnect()
{
	if (_api)
	{
		_api->RegisterSpi(NULL);
		_api->Release();
		_api = NULL;
	}

	return true;
}

void ParserHuaX::DoLogin()
{
	if (_api == NULL)
	{
		return;
	}
	CTORATstpReqUserLoginField req_user_login_field;
	memset(&req_user_login_field, 0, sizeof(req_user_login_field));

	strcpy(req_user_login_field.UserProductInfo, WT_PRODUCT);

	int iResult = _api->ReqUserLogin(&req_user_login_field, 1);

	if (iResult != 0)
	{
		if (_sink)
		{
			if (iResult == -1)
			{
				_sink->handleEvent(WPE_Connect, iResult);
			}
			else
			{
				_sink->handleEvent(WPE_Connect, 0);

				write_log(_sink, LL_ERROR, "[ParserHuaX] Sending login request failed: {}", iResult);
			}

		}
	}
	else
	{
		_loginState = LS_LOGINING;
	}
}

void ParserHuaX::DoSubscribeMD()
{
	CodeSet codeFilter = _fitSHSubs;
	if (!codeFilter.empty())
	{
		char** subscribe = new char* [codeFilter.size()];
		int nCount = 0;
		CodeSet::iterator it = codeFilter.begin();
		for (; it != codeFilter.end(); it++)
		{
			subscribe[nCount++] = (char*)(*it).c_str();
		}

		if (_api && nCount > 0)
		{
			int iResult = _api->SubscribeMarketData(subscribe, nCount, TORA_TSTP_EXD_SSE);
			if (iResult != 0)
			{
				if (_sink)
					write_log(_sink, LL_ERROR, "[ParserHuaX] Sending md subscribe request of SSE failed: {}", iResult);
			}
			else
			{
				if (_sink)
					write_log(_sink, LL_INFO, "[ParserHuaX] Market data of {} instruments of SSE subscribed", nCount);
			}
		}
		codeFilter.clear();
		delete[] subscribe;
		//int iResult = _api->SubscribeAllMarketData(XTP_EXCHANGE_SH);
	}

	codeFilter = _fitSZSubs;
	if (!codeFilter.empty())
	{
		char** subscribe = new char* [codeFilter.size()];
		int nCount = 0;
		CodeSet::iterator it = codeFilter.begin();
		for (; it != codeFilter.end(); it++)
		{
			subscribe[nCount++] = (char*)(*it).c_str();
		}

		if (_api && nCount > 0)
		{
			int iResult = _api->SubscribeMarketData(subscribe, nCount, TORA_TSTP_EXD_SZSE);
			if (iResult != 0)
			{
				if (_sink)
					write_log(_sink, LL_ERROR, "[ParserHuaX] Sending md subscribe request of SZSE failed: {}", iResult);
			}
			else
			{
				if (_sink)
					write_log(_sink, LL_INFO, "[ParserHuaX] Market data of {} instruments of SZSE subscribed", nCount);
			}
		}
		codeFilter.clear();
		delete[] subscribe;
		//int iResult = _api->SubscribeAllMarketData(XTP_EXCHANGE_SZ);
	}
}

void ParserHuaX::subscribe(const CodeSet& vecSymbols)
{
	if (!isConnected())
	{
		for (auto& code : vecSymbols)
		{
			if (strncmp(code.c_str(), "SSE.", 4) == 0)
			{
				_fitSHSubs.insert(code.c_str() + 4);
			}
			else if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				_fitSZSubs.insert(code.c_str() + 5);
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
				_fitSHSubs.insert(code.c_str() + 4);
				setSH.insert(code.c_str() + 4);
			}
			else if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				_fitSZSubs.insert(code.c_str() + 5);
				setSZ.insert(code.c_str() + 5);
			}
		}

		if (!setSH.empty())
		{
			char** subscribe = new char* [setSH.size()];
			int nCount = 0;
			CodeSet::iterator it = setSH.begin();
			for (; it != setSH.end(); it++)
			{
				subscribe[nCount++] = (char*)(*it).c_str();
			}

			if (_api && nCount > 0)
			{
				int iResult = _api->SubscribeMarketData(subscribe, nCount, TORA_TSTP_EXD_SSE);
				if (iResult != 0)
				{
					if (_sink)
						write_log(_sink, LL_ERROR, "[ParserHuaX] Sending md subscribe request of SSE failed: {}", iResult);
				}
				else
				{
					if (_sink)
						write_log(_sink, LL_INFO, "[ParserHuaX] Market data of {} instruments of SSE subscribed", nCount);
				}
			}
			delete[] subscribe;
		}

		if (!setSZ.empty())
		{
			char** subscribe = new char* [setSZ.size()];
			int nCount = 0;
			CodeSet::iterator it = setSZ.begin();
			for (; it != setSZ.end(); it++)
			{
				subscribe[nCount++] = (char*)(*it).c_str();
			}

			if (_api && nCount > 0)
			{
				int iResult = _api->SubscribeMarketData(subscribe, nCount, TORA_TSTP_EXD_SZSE);
				if (iResult != 0)
				{
					if (_sink)
						write_log(_sink, LL_ERROR, "[ParserHuaX] Sending md subscribe request of SZSE failed: {}", iResult);
				}
				else
				{
					if (_sink)
						write_log(_sink, LL_INFO, "[ParserHuaX] Market data of {} instruments of SZSE subscribed", nCount);
				}
			}
			delete[] subscribe;
		}
	}
}

void ParserHuaX::unsubscribe(const CodeSet& vecSymbols)
{
}

bool ParserHuaX::isConnected()
{
	return _loginState == LS_LOGINED;
}

void ParserHuaX::registerSpi(IParserSpi* listener)
{
	_sink = listener;

	if (_sink)
		_pBaseDataMgr = _sink->getBaseDataMgr();
}

void ParserHuaX::OnFrontConnected()
{
	if (_sink)
		_sink->handleEvent(WPE_Connect, 0);
	DoLogin();
}

void ParserHuaX::OnFrontDisconnected(int nReason)
{
	_loginState = LS_NOTLOGIN;
}

void ParserHuaX::OnRspUserLogin(CTORATstpRspUserLoginField* pRspUserLoginField, CTORATstpRspInfoField* pRspInfoField, int nRequestID)
{
	if (pRspInfoField->ErrorID == 0)
	{
		_loginState = LS_LOGINED;
		_uTradingDate = TimeUtils::getCurDate();

		if (_sink)
		{
			_sink->handleEvent(WPE_Login, 0);
		}

		DoSubscribeMD();
	}
	else
	{
		write_log(_sink, LL_ERROR, "[ParserHuaX] login failed: {}:{}", pRspInfoField->ErrorID, pRspInfoField->ErrorMsg);
		_loginState = LS_NOTLOGIN;
	}
}

void ParserHuaX::OnRspSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurityField, CTORATstpRspInfoField* pRspInfoField)
{
	if (pRspInfoField->ErrorID != 0)
	{
		write_log(_sink, LL_ERROR, "[ParserHuaX] submarket data failed: {}:{}", pRspInfoField->ErrorID, pRspInfoField->ErrorMsg);
		_loginState = LS_NOTLOGIN;
	}
}

void ParserHuaX::OnRtnMarketData(CTORATstpMarketDataField* market_data)
{
	if (_pBaseDataMgr == NULL)
	{
		return;
	}

	uint32_t actDate = (uint32_t)stoi(market_data->TradingDay);
	auto time_vector = StrUtil::split(market_data->UpdateTime, ":");
	uint32_t actTime{ 0 };
	for (std::string time_str : time_vector)
	{
		actTime = actTime * 100;
		actTime += std::stoi(time_str);
	}
	actTime = actTime * 1000 + market_data->UpdateMillisec;
	std::string code, exchg;
	if (market_data->ExchangeID == TORA_TSTP_EXD_SSE)
	{
		exchg = WT_MKT_SH_A;
	}
	else if (market_data->ExchangeID == TORA_TSTP_EXD_SZSE)
	{
		exchg = WT_MKT_SZ_A;
	}
	else
		return;

	code = market_data->SecurityID;

	WTSContractInfo* ct = _pBaseDataMgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
	{
		if (_sink)
			write_log(_sink, LL_ERROR, "[ParserHuaX] Instrument {}.{} not exists...", exchg, code);
		return;
	}
	WTSCommodityInfo* commInfo = ct->getCommInfo();

	WTSTickData* tick = WTSTickData::create(code.c_str());
	tick->setContractInfo(ct);
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, commInfo->getExchg());

	quote.action_date = actDate;
	quote.action_time = actTime;

	quote.price = checkValid(market_data->LastPrice);
	quote.open = checkValid(market_data->OpenPrice);
	quote.high = checkValid(market_data->HighestPrice);
	quote.low = checkValid(market_data->LowestPrice);
	quote.total_volume = (uint32_t)market_data->Volume;
	quote.trading_date = actDate;
	quote.total_turnover = market_data->Turnover;

	quote.upper_limit = checkValid(market_data->UpperLimitPrice);
	quote.lower_limit = checkValid(market_data->LowerLimitPrice);

	quote.pre_close = checkValid(market_data->PreClosePrice);

	//委卖价格
	quote.ask_prices[0] = checkValid(market_data->AskPrice1);
	quote.ask_prices[1] = checkValid(market_data->AskPrice2);
	quote.ask_prices[2] = checkValid(market_data->AskPrice3);
	quote.ask_prices[3] = checkValid(market_data->AskPrice4);
	quote.ask_prices[4] = checkValid(market_data->AskPrice5);

	//委卖量
	quote.ask_qty[0] = checkValid(market_data->AskVolume1);
	quote.ask_qty[1] = checkValid(market_data->AskVolume2);
	quote.ask_qty[2] = checkValid(market_data->AskVolume3);
	quote.ask_qty[3] = checkValid(market_data->AskVolume4);
	quote.ask_qty[4] = checkValid(market_data->AskVolume5);

	//委买价格
	quote.bid_prices[0] = checkValid(market_data->BidPrice1);
	quote.bid_prices[1] = checkValid(market_data->BidPrice2);
	quote.bid_prices[2] = checkValid(market_data->BidPrice3);
	quote.bid_prices[3] = checkValid(market_data->BidPrice4);
	quote.bid_prices[4] = checkValid(market_data->BidPrice5);

	//委买量
	quote.bid_qty[0] = checkValid(market_data->BidVolume1);
	quote.bid_qty[1] = checkValid(market_data->BidVolume2);
	quote.bid_qty[2] = checkValid(market_data->BidVolume3);
	quote.bid_qty[3] = checkValid(market_data->BidVolume4);
	quote.bid_qty[4] = checkValid(market_data->BidVolume5);

	if (_sink)
		_sink->handleQuote(tick, 1);

	tick->release();
}
