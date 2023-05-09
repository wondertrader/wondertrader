/*!
 * \file TraderZS.h
 * \project	WonderTrader
 *
 * \author Sunseeeeeeker
 * \date 2023/05/07
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include <boost/asio/io_service.hpp>

#include "../API/zsTradeApi1.6.0/include/tradeApi.h"

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"

#define WAITRSP_TIMEOUT 15


USING_NS_WTP;

class TraderZS : public CTradeCallback, public ITraderApi
{
public:
	TraderZS();
	virtual ~TraderZS();

	typedef enum
	{
		TS_NOTLOGIN,		//未登录
		TS_LOGINING,		//正在登录
		TS_LOGINED,			//已登录
		TS_LOGINFAILED,		//登录失败
		TS_ALLREADY			//全部就绪
	} TraderState;

public:
	//////////////////////////////////////////////////////////////////////////
	//CTradeCallback 接口
	virtual int OnConnected(void) override;

	virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) override;

	virtual int OnAcctRegister(STFirstSet *p_pFirstSet, STRspAcctRegister *p_pRspAcctLoginState, LONGLONG  p_llReqId, int p_iNum);

	// 委托响应
	virtual int OnOrder(STFirstSet *p_pFirstSet, STRspOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum);

	// 撤单响应
	virtual int OnCancelOrder(STFirstSet *p_pFirstSet, STRspCancelOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum);

	// 持仓查询响应
	virtual int OnQryHolding(STFirstSet *p_pFirstSet, STRspQryHolding *p_pRsp, LONGLONG  p_llReqId, int p_iNum);

	// 成交查询响应
	virtual int OnQryFill(STFirstSet *p_pFirstSet, STRspQryFill *p_pRsp, LONGLONG  p_llReqId, int p_iNum);

	// 资金查询响应
	virtual int OnQryMoney(STFirstSet *p_pFirstSet, STRspQryMoney *p_pRsp, LONGLONG  p_llReqId, int p_iNum);

	// 委托查询响应
	virtual int OnQryOrder(STFirstSet *p_pFirstSet, STRspQryOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum);

	// 股东账户查询响应
	virtual int OnQryHolder(STFirstSet *p_pFirstSet, STRspQryHolder *p_pRsp, LONGLONG  p_llReqId, int p_iNum);

	// 确认回报
	virtual int OnRtnOrderConfirm(STRtnOrderConfirm *p_pRtnOrderConfirm);

	// 成交回报
	virtual int OnRtnOrderFill(STRtnOrderFill *p_pRtnOrderFill);

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderApi 接口
	virtual bool init(WTSVariant *params) override;

	virtual void release() override;

	virtual void registerSpi(ITraderSpi *listener) override;

	virtual void connect() override;

	virtual void disconnect() override;

	virtual bool isConnected() override;

	virtual bool makeEntrustID(char* buffer, int length) override;

	virtual int login(const char* user, const char* pass, const char* productInfo) override;

	virtual int logout() override;

	virtual int orderInsert(WTSEntrust* eutrust) override;

	virtual int orderAction(WTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;

private:
	void		reconnect();
	inline uint32_t			genRequestID();
	void					doLogin();

	inline bool	extractEntrustID(const char* entrustid, uint32_t &orderRef);
	inline void	genEntrustID(char* buffer, uint32_t orderRef);

private:
	CTradeApi*		_api;
	ITraderSpi*		_sink;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*			_positions;
	WTSArray*				_trades;
	WTSArray*				_orders;

	IBaseDataMgr*			_bd_mgr;

	std::string		_user;
	std::string		_pass;  // 实际登录需要配合ZSClient使用，接口登录时不需要密码
	std::string		_flowdir;

	std::string		_host;
	int				_port;

	bool			_inited;
	bool			_logined;

	long long		_acctCode;

	TraderState		_state;

	uint32_t		_tradingday;
	std::atomic<uint32_t>		_reqid;
	std::atomic<uint32_t>		_ordref;		//报单引用

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	DllHandle		m_hInstZS;

	//委托单标记缓存器
	WtKVCache		m_eidCache;
	//订单标记缓存器
	WtKVCache		m_oidCache;
};

