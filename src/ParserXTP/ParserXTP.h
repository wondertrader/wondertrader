/*!
 * \file ParserXTP.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <boost/asio/io_service.hpp>
#include "../Includes/IParserApi.h"
#include "../API/XTP2.2.32.2/xtp_quote_api.h"

#include "../Share/DLLHelper.hpp"
#include "../Share/StdUtils.hpp"



NS_WTP_BEGIN
class WTSTickData;
NS_WTP_END

USING_NS_WTP;

class ParserXTP :	public IParserApi, public XTP::API::QuoteSpi
{
public:
	ParserXTP();
	virtual ~ParserXTP();

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
	///当客户端与交易后台通信连接断开时,该方法被调用。当发生这个情况后,API会自动重新连接,客户端可不做处理。
	///@param reason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnDisconnected(int reason) override;

	///错误应答
	virtual void OnError(XTPRI *error_info) override;

	virtual void OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last) override;
	virtual void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) override;
	virtual void OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) override;

	virtual void OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) override;
	virtual void OnTickByTick(XTPTBT *tbt_data) override;

private:
	/*
	 *	发送登录请求
	 */
	void DoLogin();
	/*
	 *	订阅品种行情
	 */
	void DoSubscribeMD();
	/*
	 *	检查错误信息
	 */
	bool IsErrorRspInfo(XTPRI *error_info);


private:
	uint32_t			m_uTradingDate;
	LoginStatus			m_loginState;
	XTP::API::QuoteApi*	m_pUserAPI;

	std::string			m_strHost;
	int32_t				m_iPort;;
	uint32_t			m_uClientID;
	std::string			m_strUser;
	std::string			m_strPass;
	std::string			m_strFlowDir;
	std::string			m_strLocalIP;

	XTP_PROTOCOL_TYPE	m_iProtocol;
	uint32_t			m_uHBInterval;
	uint32_t			m_uBuffSize;

	CodeSet				m_fitSHSubs;
	CodeSet				m_fitSZSubs;

	int					m_iRequestID;

	IParserSpi*			m_sink;
	IBaseDataMgr*		m_pBaseDataMgr;

	DllHandle		m_hInst;
	typedef XTP::API::QuoteApi* (*XTPCreater)(uint8_t, const char *, XTP_LOG_LEVEL log_level);
	//typedef CUstpFtdcMduserApi* (*FemasCreator)(const char *);
	XTPCreater		m_funcCreator;

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;
	typedef std::shared_ptr<boost::asio::io_service::work> BoostWorkerPtr;
	BoostWorkerPtr				_worker;
};

