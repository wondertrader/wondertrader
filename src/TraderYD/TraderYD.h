/*!
 * \file TraderYD.h
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

#include "../API/yd1.108.360/ydApi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"

USING_NS_WTP;

class TraderYD : public ITraderApi, public YDListener
{
public:
	TraderYD();
	virtual ~TraderYD();

public:
	virtual void notifyReadyForLogin(bool hasLoginFailed) override;


	virtual void notifyLogin(int errorNo, int maxOrderRef, bool isMonitor) override;


	virtual void notifyFinishInit() override;


	virtual void notifyOrder(const YDOrder *pOrder, const YDInstrument *pInstrument, const YDAccount *pAccount) override;


	virtual void notifyTrade(const YDTrade *pTrade, const YDInstrument *pInstrument, const YDAccount *pAccount) override;


	virtual void notifyFailedOrder(const YDInputOrder *pFailedOrder, const YDInstrument *pInstrument, const YDAccount *pAccount) override;


	virtual void notifyFailedCancelOrder(const YDFailedCancelOrder *pFailedCancelOrder, const YDExchange *pExchange, const YDAccount *pAccount) override;


	virtual void notifyAccount(const YDAccount *pAccount) override;

	virtual void notifyCaughtUp() override;

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


protected:
	WTSOrderInfo*	makeOrderInfo(const YDOrder* orderField, const YDInstrument* instInfo);
	WTSEntrust*		makeEntrust(const YDInputOrder *entrustField, const YDInstrument* instInfo);
	WTSTradeInfo*	makeTradeRecord(const YDTrade *tradeField, const YDInstrument* instInfo);
	WTSError*		makeError(int errorno, WTSErroCode ec);

	bool			generateEntrustID(uint32_t orderRef, char* buffer);
	bool			extractEntrustID(const char* entrustid, uint32_t &orderRef);

	inline uint32_t		genRequestID()
	{
		return m_iRequestID.fetch_add(1) + 1;
	}

protected:
	std::string		m_strCfgFile;

	std::string		m_strUser;
	std::string		m_strPass;

	bool			m_bCatchup;
	bool			m_bApiInited;

	std::string		m_strUserName;

	ITraderSpi*		m_sink;
	uint64_t		m_uLastQryTime;

	uint32_t					m_lDate;
	std::atomic<uint32_t>		m_orderRef;		//报单引用

	WrapperState				m_wrapperState;

	YDApi*						m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	typedef WTSHashMap<std::string> DataMap;
	DataMap*					m_mapPosition;
	DataMap*					m_mapTrades;
	DataMap*					m_mapOrders;
	WTSArray*					m_ayFunds;

	IBaseDataMgr*				m_bdMgr;

	typedef std::queue<CommonExecuter>	QueryQue;
	QueryQue				m_queQuery;
	StdUniqueMutex			m_mtxQuery;
	bool					m_bStopped;
	StdThreadPtr			m_thrdWorker;

	std::string		m_strModule;
	DllHandle		m_hInstYD;
	typedef YDApi* (*YDCreator)(const char *);
	YDCreator		m_funcCreator;

	//IniHelper		m_iniHelper;
	//委托单标记缓存器
	WtKVCache		m_eidCache;
	//订单标记缓存器
	WtKVCache		m_oidCache;
};

