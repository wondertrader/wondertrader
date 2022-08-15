/*!
 * \file TraderAresClt.h
 * \project	WonderTrader
 *
 * \author Suuuuunseeker
 * \date 2022/08/11
 *
 * \brief
 */
#pragma once

#include <string>
#include <queue>
#include <stdint.h>
#include <boost/asio/io_service.hpp>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../API/AresClt/include/ITDErebusApi.h"


#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

USING_NS_WTP;

class TraderAresClt : public ITraderApi, public IOptTraderApi, public ITDErebusSpi
{
public:
	TraderAresClt();
	virtual ~TraderAresClt();

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
	virtual bool init(WTSVariant* params) override;

	virtual void release() override;

	virtual void registerSpi(ITraderSpi *listener) override;

	virtual bool makeEntrustID(char* buffer, int length) override;

	virtual void connect() override;

	virtual void disconnect() override;

	virtual bool isConnected() override;

	virtual int login(const char* user, const char* pass, const char* productInfo) override;

	virtual int logout() override;

	virtual int orderInsert(WTSEntrust* entrust) override;

	virtual int orderAction(WTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;

	virtual int querySettlement(uint32_t uDate) override;


	//////////////////////////////////////////////////////////////////////////
	//IOptTraderApi
	//virtual int orderInsertOpt(WTSEntrust* entrust) override;

	//virtual int orderActionOpt(WTSEntrustAction* action) override;

	//virtual int	queryOrdersOpt(WTSBusinessType bType) override;


	//////////////////////////////////////////////////////////////////////////
	//AresClt交易接口实现
public:
	virtual		void	OnFrontConnected() override;
	virtual		void	OnFrontDisconnected() override;

	virtual		void	OnRspUserLogin(tagXTRspUserLoginField* data, tagXTRspInfoField* error, int id, bool last) override;

	virtual		void	OnRspQryTradingAccount(tagXTRspAccountField* data, tagXTRspInfoField* error, int id, bool last) override;
	virtual		void	OnRspQryInvestorPosition(tagXTRspPositionField* data, tagXTRspInfoField* error, int id, bool last) override;
	virtual		void	OnRspQryOrder(tagXTOrderField* data, tagXTRspInfoField* error, int id, bool last) override;
	virtual		void	OnRspQryTrade(tagXTTradeField* data, tagXTRspInfoField* error, int id, bool last) override;

	virtual		void	OnRspOrderInsert(tagXTReqOrderInsertField* data, tagXTRspInfoField* error, int id, bool last) override;
	virtual		void	OnErrRtnOrderInsert(tagXTReqOrderInsertField* data, tagXTRspInfoField* error) override;

	virtual		void	OnRspOrderAction(tagXTReqOrderCancelField* data, tagXTRspInfoField* error, int id, bool last) override;
	virtual		void	OnErrRtnOrderAction(tagXTReqOrderCancelField* data, tagXTRspInfoField* error) override;

	virtual		void	OnRtnOrder(tagXTOrderField* data) override;
	virtual		void	OnRtnTrade(tagXTTradeField* data) override;

public:
	void				FreeApi(ITDErebusApi* p);
	void				FreeApp(ITDErebusApp* p);

private:
	//void				reconnect();

protected:
	/*
	*	检查错误信息
	*/
	bool IsErrorRspInfo(tagXTRspInfoField *pRspInfo);

	WTSOrderInfo*	makeOrderInfo(tagXTOrderField* orderField);
	//WTSOrderInfo*	makeOrderInfo(tagXTReqOrderInsertField* orderInsertField);
	WTSEntrust*		makeEntrust(tagXTReqOrderInsertField *entrustField);
	WTSEntrust*		makeEntrust(tagXTReqOrderCancelField *entrustField);
	WTSError*		makeError(tagXTRspInfoField* rspInfo);
	WTSTradeInfo*	makeTradeRecord(tagXTTradeField *tradeField);

	void			generateEntrustID(char* buffer, uint32_t orderRef);
	bool			extractEntrustID(const char* entrustid, uint32_t &orderRef);

	uint32_t		genRequestID();

protected:
	int				m_nLine;
	std::string		m_strUserType;
	std::string		m_strUserID;
	std::string		m_strInvestorID;

	std::string		m_strPasswd;
	std::string		m_strExtraPasswd;

	std::string		m_strIP;
	int				m_nPort;

	std::string		m_strFlowDir;

	std::string		m_strTag;

	uint32_t		m_nCurIdx;  // 下单本地编码，需要手动生成

	std::string		m_strErrInfo;  // 返回错误信息

	ITraderSpi*		m_bscSink;
	IOptTraderSpi*	m_optSink;
	uint64_t		m_uLastQryTime;

	uint32_t					m_lDate;
	std::atomic<uint32_t>		m_orderRef;		//报单引用

	WrapperState				m_wrapperState;

	ITDErebusApi*				m_pUserAPI;
	ITDErebusApp*				m_pApp;  // //[3]创建环境App并初始化
	std::atomic<uint32_t>		m_iRequestID;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*				m_mapPosition;
	WTSArray*					m_ayTrades;
	WTSArray*					m_ayOrders;
	WTSArray*					m_ayExecOrds;
	WTSArray*					m_ayPosDetail;

	IBaseDataMgr*				m_bdMgr;

	//typedef std::queue<CommonExecuter>	QueryQue;
	//QueryQue				m_queQuery;
	bool					m_bInQuery;
	StdUniqueMutex			m_mtxQuery;
	uint64_t				m_lastQryTime;

	bool					m_bStopped;
	StdThreadPtr			m_thrdWorker;

	boost::asio::io_service		_asyncio;

	std::string		m_strModule;
	DllHandle		m_hInstAres;
	typedef void*	(*pfnCreateObj)(const char*, const char*);
	typedef void(*pfnDestroyObj)(void*);
	pfnCreateObj	m_pfnCreateObj;
	pfnDestroyObj	m_pfnDestroyObj;

	IniHelper		m_iniHelper;
};

