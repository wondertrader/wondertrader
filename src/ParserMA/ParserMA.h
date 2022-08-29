/*!
 * \file ParserMA.h
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
#include "../API/mCliApi3.7/maCliMktData.h"
#include "../API/mCliApi3.7/maCliMktDataStruct.h"
#include <map>

NS_WTP_BEGIN
class WTSTickData;
NS_WTP_END

USE_NAMESPACE_MACLI

USING_NS_WTP;

class ParserMA :	public IParserApi, public CCliMktDataSpi
{
public:
	ParserMA();
	virtual ~ParserMA();

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
	virtual int OnConnected() override;

	virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) override;

	virtual int OnRspStkUserLogin(CFirstSetField *p_pFirstSet, CRspStkUserLoginField *p_pRspUserLoginField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	virtual int OnRspOptUserLogin(CFirstSetField *p_pFirstSet, CRspOptUserLoginField *p_pRspUserLoginField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	virtual int OnRspHeartBeat(CFirstSetField *p_pFirstSet, CRspHeartBeatField *p_pRspHeartBeatField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	virtual int OnRtnMktData(ST_MKT_DATA* p_pMktData) override;

	virtual int OnRtnSubMktData(CRspSubTopicField* p_pRspSubTopicField) override;

	virtual int OnRtnUnsubMktData(CRspUnsubTopicField* p_pRspUnsubTopicField) override;

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
	uint32_t			m_uTradingDate;
	CCliMktDataApi*		m_pUserAPI;

	std::string			m_strHost;
	std::string			m_strUser;
	std::string			m_strPass;
	int					m_iPort;
	int					m_iMode;

	std::string			m_strAuthData;
	std::string			m_strEncryptKey;

	std::string			m_strUseScope;
	std::string			m_strAuthType;
	std::string			m_strEncryptType;
	std::string			m_strAcctType;

	int					m_iRequestID;

	IParserSpi*			m_sink;
	IBaseDataMgr*		m_pBaseDataMgr;

	DllHandle			m_hInstMA;

	CodeSet				m_filterSubs;
};

