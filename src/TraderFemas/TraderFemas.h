/*!
 * \file TraderFemas.h
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
#include <unordered_map>
#include <stdint.h>
#include <atomic>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Share/StdUtils.hpp"
#include "../Includes/WTSVariant.hpp"

//Femas v3.02
#include "../API/Femas3.02/USTPFtdcTraderApi.h"

#include "../Share/DLLHelper.hpp"

USING_NS_WTP;

typedef std::unordered_map<std::string, std::string>	StringMap;

class TraderFemas : public ITraderApi, public CUstpFtdcTraderSpi
{
public:
	TraderFemas();
	virtual ~TraderFemas();

public:
	typedef enum 
	{
		WS_NOTLOGIN,		//δ��¼
		WS_LOGINING,		//���ڵ�¼
		WS_LOGINED,			//�ѵ�¼
		WS_LOGINFAILED,		//��¼ʧ��
		WS_CONFIRM_QRYED,
		WS_CONFIRMED,		//��ȷ��
		WS_ALLREADY			//ȫ������
	} WrapperState;


private:
	void onInitialized();

	int authenticate();

	int doLogin();

//////////////////////////////////////////////////////////////////////////
//ITraderApi�ӿ�
public:
	virtual bool init(WTSVariant* params);

	virtual void release();

	virtual void setTag(const char* tag){m_strTag = tag;}
	virtual const char* getTag(){return m_strTag.c_str();}

	virtual void registerSpi( ITraderSpi *listener );

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
//Femas���׽ӿ�ʵ��
public:
	virtual void OnFrontConnected() override;

	virtual void OnQryFrontConnected() override;

	virtual void OnFrontDisconnected(int nReason) override;

	virtual void OnQryFrontDisconnected(int nReason) override;

	virtual void OnHeartBeatWarning(int nTimeLapse) override;

	virtual void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///��ѯǰ��ϵͳ�û���¼Ӧ��
	virtual void OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///�ɽ��ر�
	virtual void OnRtnTrade(CUstpFtdcTradeField *pTrade) override;

	///�����ر�
	virtual void OnRtnOrder(CUstpFtdcOrderField *pOrder) override;

	///����¼��Ӧ��
	virtual void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///��������Ӧ��
	virtual void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///Ͷ���ֲֲ߳�ѯӦ��
	virtual void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo) override;

	///������������ر�
	virtual void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo) override;

	virtual void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) override;

protected:
	bool IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo);

	int wrapPriceType(WTSPriceType priceType, bool isCFFEX = false);
	int wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offType);
	int wrapOffsetType(WTSOffsetType offType);
	int	wrapTimeCondition(WTSTimeCondition timeCond);
	int wrapActionFlag(WTSActionFlag actionFlag);

	const char* wrapExchg(const char* exchg);


	WTSPriceType		wrapPriceType(TUstpFtdcPriceType priceType);
	WTSDirectionType	wrapDirectionType(TUstpFtdcDirectionType dirType, TUstpFtdcOffsetFlagType offType);
	WTSDirectionType	wrapPosDirection(TUstpFtdcDirectionType dirType);
	WTSOffsetType		wrapOffsetType(TUstpFtdcOffsetFlagType offType);
	WTSTimeCondition	wrapTimeCondition(TUstpFtdcTimeConditionType timeCond);
	WTSOrderState		wrapOrderState(TUstpFtdcOrderStatusType orderState);
	
	WTSOrderInfo*	makeOrderInfo(CUstpFtdcOrderField* orderField);
	WTSEntrust*		makeEntrust(CUstpFtdcInputOrderField *entrustField);
	WTSError*		makeError(CUstpFtdcRspInfoField* rspInfo);
	WTSTradeInfo*	makeTradeRecord(CUstpFtdcTradeField *tradeField);


	uint32_t		genRequestID();
	uint32_t		genLocalOrdID();

	void			triggerQuery();

protected:
	std::string		m_strBroker;
	std::string		m_strFront;
	std::string		m_strQryFront;

	std::string		m_strUser;
	std::string		m_strPass;
	std::string		m_strAppID;
	std::string		m_strAuthCode;
	std::string		m_strFlowDir;

	bool			m_bQuickStart;

	std::string		m_strTag;

	std::string		m_strUserName;

	ITraderSpi*	m_sink;
	uint64_t		m_uLastQryTime;

	long			m_lDate;

	std::atomic<uint32_t>		m_uLocalOrdID;		//��������
	std::string					m_strSessionID;

	WrapperState				m_wrapperState;

	CUstpFtdcTraderApi*			m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	StringMap					m_mapOrderTag;

	WTSArray*					m_ayPosition;
	WTSArray*					m_ayTrades;
	WTSArray*					m_ayOrders;
	WTSArray*					m_ayPosDetail;
	
	IBaseDataMgr*				m_bdMgr;

	bool						m_bQryOnline;
	bool						m_bStopped;

	boost::asio::io_service		m_asyncIO;
	boost::asio::io_service::strand*	m_strandIO;
	StdThreadPtr				m_thrdWorker;

	typedef std::queue<CommonExecuter>	QueryQue;
	QueryQue				m_queQuery;
	bool					m_bInQuery;
	StdUniqueMutex		m_mtxQuery;
	uint64_t				m_lastQryTime;

	std::string		m_strModule;
	DllHandle		m_hInstCTP;
	typedef CUstpFtdcTraderApi* (*APICreator)(const char *);
	APICreator		m_funcCreator;
};

