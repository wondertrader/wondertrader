/*!
 * \file ParserHuaX.h
 * \project	WonderTrader
 *
 * \author HeJ
 * \date 2022/08/10
 *
 * \brief
 */
#pragma once
#include "../Includes/IParserApi.h"
#include "../Share/DLLHelper.hpp"
//#include "../API/HuaX4.03/parserapi/xfastmdapitest/TORATstpXMdApi.h"
#include "../API/HuaX4.03/parserapi/lev2mdapi/TORATstpLev2MdApi.h"
#include <map>

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>
#include "../Share/fmtlib.h"

#define _CRT_SECURE_NO_WARNINGS

NS_WTP_BEGIN
class WTSTickData;
NS_WTP_END

USING_NS_WTP;
//using namespace TORALEV1API; disable lev1 NS
using namespace TORALEV2API; 

/*
Disable lev1 typedef
typedef CTORATstpXMdSpi HuaXParserSpi;
typedef CTORATstpXMdApi HuaXParserApi;
*/
// For lev2 API AND SPI
typedef TORALEV2API::CTORATstpLev2MdSpi HuaXParserLev2Spi;
typedef TORALEV2API::CTORATstpLev2MdApi HuaXParserLev2Api;


const char* WT_MKT_SH_A = "SSE";
const char* WT_MKT_SZ_A = "SZSE";

class ParserHuaX : public HuaXParserLev2Spi, public IParserApi
{
public:
	ParserHuaX();
	virtual ~ParserHuaX();

public:
	enum LoginStatus
	{
		LS_NOTLOGIN,
		LS_LOGINING,
		LS_LOGINED
	};

//IQuoteParser 接口
public:
	virtual bool init(WTSVariant* config) override;

	virtual void release() override;

	virtual bool connect() override;

	virtual bool disconnect() override;

	virtual bool isConnected() override;

	virtual void subscribe(const CodeSet &vecSymbols) override;
	virtual void unsubscribe(const CodeSet &vecSymbols) override;

	virtual void registerSpi(IParserSpi* listener) override;


//CThostFtdcMdSpi 接口
public:
	virtual void OnFrontConnected() override;

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///        -3 连接已断开
	///        -4 网络读失败
	///        -5 网络写失败
	///        -6 订阅流错误
	///        -7 流序号错误
	///        -8 错误的心跳报文
	///        -9 错误的报文
	virtual void OnFrontDisconnected(int nReason) override;

	// bool bIsLast is a new parameter in lev2
	virtual void OnRspUserLogin(CTORATstpRspUserLoginField* pRspUserLoginField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

	// int nRequestID and bool bIsLast is a new parameter in lev2
	virtual void OnRspSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurityField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

	// lev1 : virtual void OnRtnMarketData(CTORATstpMarketDataField* pMarketDataField) override;, lev2 has 4 more parameters
	virtual void OnRtnMarketData(CTORATstpLev2MarketDataField *pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]);

private:
	/*
	 *	发送登录请求
	 */
	void DoLogin();
	/*
	 *	订阅品种行情
	 */
	void DoSubscribeMD();

private:
	uint32_t			_uTradingDate;
	LoginStatus			_loginState;
	HuaXParserLev2Api*		_api;
	std::string			_sub_mode; // TCP连接模式 0  UDP单播模式 1 UDP组播模式 2
	std::string			_derive_sub_mode; // 延伸功能连接模式
	std::string			_front;  // tcp://210.14.72.16:9402
	std::string			_strUser; 
	std::string			_strPass;
	std::string			_strFlowDir;

	CodeSet				_fitSHSubs;
	CodeSet				_fitSZSubs;

	int					_iRequestID;
	bool                bIsLast;

	IParserSpi*			_sink;
	IBaseDataMgr*		_pBaseDataMgr;

	DllHandle			_hInst;
	typedef HuaXParserLev2Api* (*HuaXCreater)(const TTORATstpMDSubModeType&, const TTORATstpMDSubModeType&);
	HuaXCreater			_funcCreator;
};

