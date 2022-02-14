/*!
 * \file ParserYD.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Includes/IParserApi.h"
#include "../Share/DLLHelper.hpp"
#include "../API/yd1.108.360/ydApi.h"
#include <map>

NS_WTP_BEGIN
class WTSTickData;
NS_WTP_END

USING_NS_WTP;

class ParserYD :	public IParserApi, public YDListener
{
public:
	ParserYD();
	virtual ~ParserYD();


public:
	virtual void notifyReadyForLogin(bool hasLoginFailed) override;

	virtual void notifyLogin(int errorNo, int maxOrderRef, bool isMonitor) override;

	virtual void notifyFinishInit(void) override;

	virtual void notifyMarketData(const YDMarketData *pMarketData) override;

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



private:
	/*
	 *	发送登录请求
	 */
	void DoLogin();
	/*
	 *	订阅品种行情
	 */
	void DoSubscribe();


private:
	uint32_t			m_uTradingDate;
	YDApi*				m_pUserAPI;
	bool				m_bApiInited;

	std::string			m_strCfgFile;

	std::string			m_strUserID;
	std::string			m_strPassword;

	CodeSet				m_filterSubs;

	IParserSpi*			m_sink;
	IBaseDataMgr*		m_pBaseDataMgr;

	DllHandle			m_hInstYD;
	typedef YDApi* (*YDCreator)(const char *);
	YDCreator			m_funcCreator;
};

