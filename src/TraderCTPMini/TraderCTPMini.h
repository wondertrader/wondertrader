/*!
 * \file TraderCTP.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <string>
#include <queue>
#include <stdint.h>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../API/CTPMini1.5.8/ThostFtdcTraderApi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

USING_NS_WTP;

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
	int authenticate();

	int doLogin();

	//////////////////////////////////////////////////////////////////////////
	//ITraderApi接口
public:
	virtual bool init(WTSVariant* params) override;

	virtual void release() override;

	virtual void registerSpi(ITraderSpi *listener) override;

	virtual bool makeEntrustID(char* buffer, int length) override;

	virtual void connect() override;

	virtual void disconnect() override;

	virtual bool isConnected() override;

	virtual int login(const char* user, const char* pass, const char* productInfo) override;

	virtual int logout() override;

	virtual int orderInsert(WTSEntrust* eutrust) override;

	virtual int orderAction(WTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;


	//////////////////////////////////////////////////////////////////////////
	//CTP交易接口实现
public:
	virtual void OnFrontConnected() override;

	virtual void OnFrontDisconnected(int nReason) override;

	virtual void OnHeartBeatWarning(int nTimeLapse) override;

	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///请求查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) override;

	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) override;

	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) override;

protected:
	/*
	*	检查错误信息
	*/
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	int wrapPriceType(WTSPriceType priceType, bool isCFFEX = false);
	int wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offType);
	int wrapOffsetType(WTSOffsetType offType);
	int	wrapTimeCondition(WTSTimeCondition timeCond);
	int wrapActionFlag(WTSActionFlag actionFlag);

	WTSPriceType		wrapPriceType(TThostFtdcOrderPriceTypeType priceType);
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

	//void			triggerQuery();

protected:
	std::string		m_strBroker;
	std::string		m_strFront;

	std::string		m_strUser;
	std::string		m_strPass;

	std::string		m_strAppID;
	std::string		m_strAuthCode;
	std::string		m_strFlowDir;

	std::string		m_strProdInfo;

	bool			m_bQuickStart;

	std::string		m_strTag;

	std::string		m_strUserName;

	ITraderSpi*	m_sink;
	uint64_t		m_uLastQryTime;

	uint32_t					m_lDate;
	TThostFtdcFrontIDType		m_frontID;		//前置编号
	TThostFtdcSessionIDType		m_sessionID;	//会话编号
	std::atomic<uint32_t>		m_orderRef;		//报单引用

	WrapperState				m_wrapperState;

	CThostFtdcTraderApi*		m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*				m_mapPosition;
	WTSArray*					m_ayTrades;
	WTSArray*					m_ayOrders;
	WTSArray*					m_ayPosDetail;
	WTSArray*					m_ayFunds;

	IBaseDataMgr*				m_bdMgr;

	typedef std::queue<CommonExecuter>	QueryQue;
	QueryQue				m_queQuery;
	bool					m_bInQuery;
	StdUniqueMutex			m_mtxQuery;
	uint64_t				m_lastQryTime;

	bool					m_bStopped;
	StdThreadPtr			m_thrdWorker;

	std::string		m_strModule;
	DllHandle		m_hInstCTP;
	typedef CThostFtdcTraderApi* (*CTPCreator)(const char *);
	CTPCreator		m_funcCreator;

	IniHelper		m_iniHelper;
};
