/*!
 * \file TraderATP.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#pragma once

#include <stdint.h>
#include <boost/asio/io_service.hpp>
#include <atomic>

#define TRADE_API_USE_STATIC
#include "../API/AtpTradeApi/include/atp_trade_api.h"

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"


USING_NS_WTP;

class TraderATP : public ATPTradeHandler, public ITraderApi
{
public:
	TraderATP();
	virtual ~TraderATP();

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
	// 登入回调
	virtual void OnLogin(const std::string& reason) override;

	// 登出回调
	virtual void OnLogout(const std::string& reason) override;

	// 连接失败
	virtual void OnConnectFailure(const std::string &reason) override;

	// 连接超时
	virtual void OnConnectTimeOut(const std::string &reason) override;

	// 心跳超时
	virtual void OnHeartbeatTimeout(const std::string &reason) override;

	// 连接关闭
	virtual void OnClosed(const std::string &reason) override;

	// 连接结束回调
	virtual void OnEndOfConnection(const std::string& reason) override;

	// 客户号登入回调
	virtual void OnRspCustLoginResp(const ATPRspCustLoginRespOtherMsg &cust_login_resp) override;

	// 客户号登出回调
	virtual void OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg &cust_logout_resp) override;

	// 订单下达内部响应
	virtual void OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg& order_status_ack) override;

	// 订单下达交易所确认
	virtual void OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg& order_status_ack) override;

	// 成交回报
	virtual void OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg& cash_auction_trade_er) override;

	// 订单下达内部拒绝
	virtual void OnRspBizRejection(const ATPRspBizRejectionOtherMsg& biz_rejection) override;

	/**
	 * @brief 资金查询结果
	 * @param[in] ATPRspFundQueryResultMsg
	 * @type query
	 */
	virtual void OnRspFundQueryResult(const ATPRspFundQueryResultMsg &msg) override;

	/**
	 * @brief 股份查询结果
	 * @param[in] ATPRspShareQueryResultMsg
	 * @type query
	 */
	virtual void OnRspShareQueryResult(const ATPRspShareQueryResultMsg &msg) override;

	/**
	 * @brief 成交查询结果
	 * @param[in] ATPRspTradeOrderQueryResultMsg
	 * @type query
	 */
	virtual void OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg &msg) override;

	/**
	 * @brief 订单查询结果
	 * @param[in] ATPRspOrderQueryResultMsg
	 * @type query
	 */
	virtual void OnRspOrderQueryResult(const ATPRspOrderQueryResultMsg &msg) override;

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
	inline uint32_t	genRequestID();
	void		doLogin(const char* productid);

	inline WTSOrderInfo*	makeOrderInfo(const APIOrderUnit* orderField);
	inline WTSEntrust*		makeEntrust(const ATPRspOrderStatusAckMsg *entrustField);
	inline WTSTradeInfo*	makeTradeInfo(const APITradeOrderUnit *tradeField);
	inline WTSOrderInfo*	makeOrderInfo(const ATPRspOrderStatusAckMsg *order_status_ack);
	inline WTSTradeInfo*	makeTradeRecord(const ATPRspCashAuctionTradeERMsg *cash_auction_trade_er);

	inline bool	extractEntrustID(const char* entrustid, uint32_t &orderRef);
	inline void	genEntrustID(char* buffer, uint32_t orderRef);

private:
	ATPTradeAPI*	_api;
	ITraderSpi*		_sink;
	bool			_close_flag;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*			_positions;
	WTSArray*				_trades;
	WTSArray*				_orders;

	IBaseDataMgr*			_bd_mgr;

	std::string		_user;  // AGW登录账号
	std::string		_pass;  // AGW登录密码
	std::string		_node_id;  // 节点信息
	int				_loginmode;  // 账户的登录模式
	int				_order_way;  // 委托方式

	std::string 	_product;

	std::string		_sh_acctid;  // 上海证券账户
	std::string		_sz_acctid;  // 深圳证券账户
	std::string		_fund_accountid;  // 资金账户
	std::string		_accpasswd;  // 交易密码
	std::string		_branchid;   //营业部ID （当登录模式为2-资金账户登录模式时，该字段是否必填请咨询券商）  

	std::string		_front;  // 交易网关
	std::string		_front2;

	TraderState		_state;

	uint64_t		_sessionid;

	std::string		_acctid;  // 登录账户
	bool			_is_sh;  // 账户支持的交易所

	uint32_t		_tradingday;
	std::atomic<uint32_t>		_reqid;
	std::atomic<uint32_t>		_ordref;		//报单引用

	std::string		_cust_id;  // 客户号

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	DllHandle		m_hInstATP;

	//委托单标记缓存器
	WtKVCache		m_eidCache;
	//订单标记缓存器
	WtKVCache		m_oidCache;

	std::map<int32_t, int32_t> report_sync;

	WTSArray* ayOrders;  // 订单查询结果
	WTSArray* ayTrades;  // 成交查询结果

	std::atomic<int64_t> _return_nums;  // 查询返回条数
};
