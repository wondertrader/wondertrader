/*!
 * \file TraderHTS.h
 * \project	WonderTrader
 *
 * \author Suuuunseeker
 * \date 2022/09/06
 *
 * \brief
 */
#pragma once

#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <stdint.h>

#include "../API/HTS5.2.43.0/include/secitpdk.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"


USING_NS_WTP;


class TraderHTS : public ITraderApi
{
	//friend void callback_rtn1(const char* pTime, const char* pMsg, int nType);

public:
	TraderHTS();
	virtual ~TraderHTS();

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
	void InitializeHTS(WTSVariant* params);
	void qryGDNo();	//查询股东号
	//void qryCustInfo();  // 查询客户信息
	//void qryCustNodeInfo();  // 查询客户节点信息

	void reconnect();

	void triggerQuery();

public:
	/* 接收订阅的回报数据 */
	void htsGetCallback(const char* pTime, const char* pMsg, int nType);

	void OnRtnOrder(const char* pMsg);  // 委托信息
	void OnRtnTrade(const char* pMsg);  // 成交信息

private:
	
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
	std::string		m_strUser;  // 客户号
	std::string		m_strPass;  // 密码
	std::string		m_strNode;  // 站点信息

	char			sShGdh[13];                // 上海股东号
	char			sSzGdh[13];                // 深圳股东号
	char			sShZjzh[13];               // 上海资金账号
	char			sSzZjzh[13];               // 深圳资金账号

	std::string		m_strWtfs;  // 委托方式
	std::string		m_strKey;  // 配置文件段名
	std::string		m_strDevName;  // 开发商代码

	std::string		m_strTag;

	ITPDK_CusReqInfo cusreqinfo;  // 登录信息

	ITraderSpi*		m_traderSink;
	uint64_t		m_uLastQryTime;

	bool			m_bUseEX;  // 使用EX后缀
	bool			m_bASync;  // 是否使用异步

	typedef WTSHashMap<std::string> TradeDataMap;
	TradeDataMap*	m_mapLives;
	std::unordered_set<std::string>	m_tradeids;

	uint32_t					m_lDate;
	std::atomic<uint32_t>		m_orderRef;		//报单引用

	WrapperState				m_wrapperState;

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


class HTSCallMgr
{
public:
	HTSCallMgr();
	~HTSCallMgr();

public:
	bool init(WTSVariant* params);
	void setCallbackMsgFunc();
	void setHTSCallPtr(std::string khh, TraderHTS* pTrader);

private:
	static void callbackMsgFunc(const char* pTime, const char* pMsg, int nType);
	static void callbackConnFunc(const char* pKhh, const char* pConnKey, int nEvent, int nType);
};