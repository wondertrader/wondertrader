/*!
 * \file TraderCTPMini.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#define WINVER			0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE		0x0501

#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <stdint.h>

#include <boost\asio\io_service.hpp>
#include <boost\asio\strand.hpp>

#include "..\Share\WTSTypes.h"
#include "..\Share\ITraderApi.h"
#include "..\Share\BoostDefine.h"
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"


USING_NS_OTP;

typedef std::unordered_map<std::string, std::string>	StringMap;

class TraderCTPMini : public ITraderApi, public CThostFtdcTraderSpi
{
public:
	TraderCTPMini();
	virtual ~TraderCTPMini();

public:
	typedef enum
	{
		WS_NOTLOGIN,		//未登录
		WS_LOGINING,		//正在登录
		WS_LOGINED,			//已登录
		WS_LOGINFAILED,		//登录失败
		WS_CONFIRM_QRYED,
		WS_CONFIRMED,		//已确认
		WS_ALLREADY			//全部就绪
	} WrapperState;


private:

	int confirm();

	int queryConfirm();

	int authenticate();

	int doLogin();

	//////////////////////////////////////////////////////////////////////////
	//ITraderApi接口
public:
	virtual bool init(WTSParams* params);

	virtual void release();

	virtual void registerListener(ITraderApiListener *listener);

	virtual bool makeEntrustID(char* buffer, int length);

	virtual void connect();

	virtual void disconnect();

	virtual bool isConnected();

	virtual int login(const char* user, const char* pass, const char* productInfo);

	virtual int logout();

	virtual int orderInsert(WTSEntrust* eutrust);

	virtual int orderAction(WTSEntrustAction* action);

	virtual int queryAccount();

	virtual int queryPositions();

	virtual int queryOrders();

	virtual int queryTrades();


	//////////////////////////////////////////////////////////////////////////
	//CTP交易接口实现
public:
	virtual void OnFrontConnected();

	virtual void OnFrontDisconnected(int nReason);

	virtual void OnHeartBeatWarning(int nTimeLapse);

	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

protected:
	/*
	*	检查错误信息
	*/
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	int wrapPriceType(WTSPriceType priceType);
	int wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offType);
	int wrapOffsetType(WTSOffsetType offType);
	int	wrapTimeCondition(WTSTimeCondition timeCond);
	int wrapActionFlag(WTSActionFlag actionFlag);

	WTSPriceType		wrapPriceType(TThostFtdcPriceType priceType);
	WTSDirectionType	wrapDirectionType(TThostFtdcDirectionType dirType, TThostFtdcOffsetFlagType offType);
	WTSDirectionType	wrapPosDirection(TThostFtdcPosiDirectionType dirType);
	WTSOffsetType		wrapOffsetType(TThostFtdcOffsetFlagType offType);
	WTSTimeCondition	wrapTimeCondition(TThostFtdcTimeConditionType timeCond);
	WTSOrderState		wrapOrderState(TThostFtdcOrderStatusType orderState);

	WTSOrderInfo*	makeOrderInfo(CThostFtdcOrderField* orderField);
	WTSEntrust*		makeEntrust(CThostFtdcInputOrderField *entrustField);
	WTSError*		makeError(CThostFtdcRspInfoField* rspInfo);
	WTSTradeInfo*	makeTradeRecord(CThostFtdcTradeField *tradeField);

	std::string		generateEntrustID(uint32_t frontid, uint32_t sessionid, uint32_t orderRef);
	bool			extractEntrustID(const char* entrustid, uint32_t &frontid, uint32_t &sessionid, uint32_t &orderRef);

	//uint64_t		calcCommission(uint32_t qty, uint32_t price, WTSOffsetType flag, WTSContractInfo* ct);
	//uint64_t		calcMargin(uint32_t qty, uint32_t price, WTSDirectionType direct, WTSContractInfo* ct);

	uint32_t		genRequestID();

	void			triggerQuery();

protected:
	std::string		m_strBroker;
	std::string		m_strFront;

	std::string		m_strUser;
	std::string		m_strPass;

	std::string		m_strAppID;
	std::string		m_strAuthCode;

	std::string		m_strProdInfo;

	bool			m_bQuickStart;

	std::string		m_strTag;

	std::string		m_strUserName;

	ITraderApiListener*	m_traderSink;
	uint64_t		m_uLastQryTime;

	uint32_t					m_lDate;
	TThostFtdcFrontIDType		m_frontID;		//前置编号
	TThostFtdcSessionIDType		m_sessionID;	//会话编号
	uint32_t					m_orderRef;		//报单引用

	WrapperState				m_wrapperState;

	CThostFtdcTraderApi*		m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	StringMap					m_mapEntrustTag;
	StringMap					m_mapOrderTag;

	WTSArray*					m_ayPosition;
	WTSArray*					m_ayTrades;
	WTSArray*					m_ayOrders;
	WTSArray*					m_ayPosDetail;

	IBaseDataMgr*				m_bdMgr;

	typedef std::queue<CommonExecuter>	QueryQue;
	QueryQue				m_queQuery;
	bool					m_bInQuery;
	BoostUniqueMutex		m_mtxQuery;
	uint64_t				m_lastQryTime;

	boost::asio::io_service		m_asyncIO;
	boost::asio::io_service::strand*	m_strandIO;
	BoostThreadPtr			m_thrdWorker;

	std::string		m_strModule;
	HINSTANCE		m_hInstCTP;
	typedef CThostFtdcTraderApi* (__cdecl *CTPCreator)(const char *);
	CTPCreator		m_funcCreator;
};

