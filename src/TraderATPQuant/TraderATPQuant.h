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
#include "../API/AtpQuantV1.3.6//include/atp_quant_api.h"

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"

USING_NS_WTP;
using namespace atp::quant_api;

class TraderATPQuant : public ATPQuantHandler, public ITraderApi
{
public:
	TraderATPQuant();
	virtual ~TraderATPQuant();

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
	// ATPQuantApi登入回调

	/**
	* @brief 现货委托响应
	*/
	virtual void OnRspCashAuctionOrder(const ATPRspCashAuctionOrderMsg& msg, const ATPRspErrorInfo& error_info, const int64_t request_id) override;

	/**
	 * @brief 现货委托回报
	 */
	virtual void OnRtnCashAuctionOrder(const ATPRtnCashAuctionOrderMsg& msg) override;

	/**
	 * @brief 现货撤单响应
	 */
	virtual void OnRspCashCancelOrder(const ATPRspCashCancelOrderMsg& msg, const ATPRspErrorInfo& error_info, const int64_t request_id) override;

	/**
	 * @brief 现货订单查询结果
	 */
	virtual void OnRspCashOrderQueryResult(const ATPRspCashOrderQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) override;

	/**
	 * @brief 现货股份查询结果
	 */
	virtual void OnRspCashShareQueryResult(const ATPRspCashShareQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) override;

	/**
	 * @brief 现货成交查询结果
	 */
	virtual void OnRspCashTradeOrderQueryResult(const ATPRspCashTradeOrderQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) override;

	/**
	 * @brief 现货成交查询结果
	 */
	//virtual void OnRspCashTradeCollectQueryResult(const ATPRspCashCollectQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) override;

	/**
	 * @brief 现货资金查询结果
	 */
	virtual void OnRspCashFundQueryResult(const ATPRspCashFundQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) override;

	/**
	 * @brief 现货资产查询结果
	 */
	virtual void OnRspCashAssetQueryResult(const ATPRspCashAssetQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) override;

	/**
	 * @brief 登录成功
	 */
	//virtual void OnLogin(const ATPCustomerInfo& msg) override;  // 可以直接通过调用api获取结果

	/**
	 * @brief 登出成功
	 */
	virtual void OnLogout(const char* desc) override;

	/**
	 * @brief 自动重连
	 */
	virtual void OnRecovering(const char* desc) override;

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
	void		doLogin();

	inline WTSOrderInfo*	makeOrderInfo(const ATPRspCashAuctionOrderMsg& orderField);
	//inline WTSOrderInfo*	makeOrderInfo(const ATPRspCashCancelOrderMsg& orderField);
	inline WTSEntrust*		makeEntrust(const ATPRspCashAuctionOrderMsg& entrustField);
	//inline WTSEntrust*		makeEntrust(const ATPRspCashCancelOrderMsg& entrustField);
	inline WTSTradeInfo*	makeTradeInfo(const ATPRspCashTradeOrderQueryResultMsg& tradeField);
	inline WTSOrderInfo*	makeOrderInfo(const ATPRspCashOrderQueryResultMsg& orderField);
	inline WTSTradeInfo*	makeTradeRecord(const ATPRtnCashAuctionOrderMsg& orderField);
	inline WTSEntrust*		makeEntrust(const ATPRtnCashAuctionOrderMsg& msg);
	inline WTSOrderInfo*	makeOrderInfo(const ATPRtnCashAuctionOrderMsg& msg);

	inline bool	extractEntrustID(const char* entrustid, uint32_t &orderRef);
	inline void	genEntrustID(char* buffer, uint32_t orderRef);

private:
	ATPQuantAPI*	_api;
	ITraderSpi*		_sink;
	bool			_close_flag;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*			_positions;
	WTSArray*				_trades;
	WTSArray*				_orders;

	IBaseDataMgr*			_bd_mgr;

	std::string 		_agw_user;
	std::string 		_agw_pass;
	std::string		_user;  // 客户号
	std::string		_pass;  // 客户密码
	int			_loginmode;  // 账户的登录模式
	int 			_encryptmode;  // 0=touchuanmoshi 1=jiamimoshi
	int			_order_way;  // 委托方式
	std::string		_client_code;  // 终端识别码
	std::string		_bind_ip;  // 本地网卡地址
	
	std::string 	_product;

	//std::string		_sh_acctid;  // 上海证券账户
	//std::string		_sz_acctid;  // 深圳证券账户
	std::string		_acctid;  // 证券账户
	bool			_is_acct_sh;  // 账户是上海户还是深圳户
	std::string		_fund_accountid;  // 资金账户
	std::string		_fund_pass;  // 交易密码
	std::string		_branchid;   //营业部ID （当登录模式为2-资金账户登录模式时，该字段是否必填请咨询券商）  

	std::string		_front;  // 交易网关  AGW地址 格式为"ip:port;ip:port"
	
	std::string		_cur_user;

	TraderState		_state;

	uint64_t		_sessionid;
	
	uint32_t		_tradingday;
	std::atomic<uint32_t>		_reqid;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	std::atomic<uint32_t>		_ordref;		//报单引用

	std::string		_cust_id;  // 客户号

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;
	//boost::asio::io_service::work work;

	DllHandle		m_hInstATP;

	//委托单标记缓存器
	WtKVCache		m_eidCache;
	//订单标记缓存器
	WtKVCache		m_oidCache;

	std::map<int32_t, int32_t> report_sync;

	WTSArray* ayOrders;  // 订单查询结果
	WTSArray* ayTrades;  // 成交查询结果
	WTSArray* ayFunds;  // 用户资金查询结果

	std::atomic<int> _id_counts;
};

