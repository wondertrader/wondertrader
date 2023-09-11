/*!
 * \file TraderDD.h
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
#include <unordered_set>
#include <unordered_map>
#include <stdint.h>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../API/FixApi/include/FixApi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

USING_NS_WTP;

class TraderDD : public ITraderApi
{
public:
	TraderDD();
	virtual ~TraderDD();

	typedef std::unordered_map<std::string, std::string>	StringMap;

public:
	typedef enum
	{
		WS_NOTLOGIN,		//未登录
		WS_LOGINING,		//正在登录
		WS_LOGINED,			//已登录
		WS_LOGINFAILED,		//登录失败
		WS_ALLREADY			//全部就绪
	} WrapperState;

private:
	void doLogin();
	void qryGDNo();	//查询股东号
	void qryZJZH();	//查询资金账户

	void reconnect();

	void triggerQuery();

public:
	void OnRtnOrder(HANDLE_CONN hconn, HANDLE_SESSION hsess);
	void OnRtnTrade(HANDLE_CONN hconn, HANDLE_SESSION hsess);

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
	std::string		m_strFront;

	std::string		m_strCommUser;
	std::string		m_strCommPass;

	std::string		m_strUser;
	std::string		m_strPass;
	std::string		m_strNode;

	std::string		m_strSHNO;
	std::string		m_strSZNO;
	std::string		m_strFDNO;

	std::string		m_strProdInfo;

	std::string		m_strTag;

	std::string		m_strToken;  // 登录之后的口令
	std::string		m_strUserName;  // 用户名
	std::string		m_strSystemID;  // 系统ID
	std::string		m_strNodeID;  // Node ID
	std::string		m_strTrust;  // 委托方式

	int				m_nNodeID;

	ITraderSpi*		m_traderSink;
	uint64_t		m_uLastQryTime;

	typedef WTSHashMap<std::string> TradeDataMap;
	TradeDataMap*	m_mapLives;
	std::unordered_set<std::string>	m_tradeids;

	uint32_t					m_lDate;
	std::atomic<uint32_t>		m_orderRef;		//报单引用

	WrapperState				m_wrapperState;

	HANDLE_CONN					m_hConn;
	uint64_t					m_uSessID;

	IBaseDataMgr*				m_bdMgr;

	typedef std::queue<CommonExecuter>	QueryQue;
	QueryQue			m_queQuery;
	bool				m_bInQuery;
	StdUniqueMutex		m_mtxQuery;
	uint64_t			m_lastQryTime;

	boost::asio::io_service		m_asyncIO;
	boost::asio::io_service::strand*	m_strandIO;
	StdThreadPtr		m_thrdWorker;

	std::string		m_strModule;
	DllHandle		m_hInstDD;

	IniHelper		m_iniHelper;
};

