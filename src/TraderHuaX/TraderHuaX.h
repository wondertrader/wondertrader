/*!
 * \file TraderHuaX.h
 * \project	WonderTrader
 *
 * \author HeJ
 * \date 2022/07/31
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include <string>
#include <boost/asio/io_service.hpp>

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSCollection.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"

#include "../API/HuaX4.03/traderapi/TORATstpTraderApi.h"

USING_NS_WTP;

using namespace TORASTOCKAPI;
typedef CTORATstpTraderSpi HuaXTraderSpi;
typedef CTORATstpTraderApi HuaXTraderApi;

const char* WT_MKT_SH_A = "SSE";
const char* WT_MKT_SZ_A = "SZSE";

class TraderHuaX : public HuaXTraderSpi, public ITraderApi
{
public:
	TraderHuaX();
	virtual ~TraderHuaX();

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
	//HuaX::API::TraderSpi 接口
	virtual void OnFrontConnected() override;
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        -3 连接已断开
	///        -4 网络读失败
	///        -5 网络写失败
	///        -6 订阅流错误
	///        -7 流序号错误
	///        -8 错误的心跳报文
	///        -9 错误的报文
	///		  -15 网络读失败
	///		  -16 网络写失败
	virtual void OnFrontDisconnected(int nReason) override;

	virtual void OnRspError(CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

	virtual void OnRspOrderInsert(CTORATstpInputOrderField* pInputOrderField, CTORATstpRspInfoField* pRspInfoField, int nRequestID) override;

	virtual void OnRtnOrder(CTORATstpOrderField* pOrderField) override;

	virtual void OnErrRtnOrderInsert(CTORATstpInputOrderField* pInputOrderField, CTORATstpRspInfoField* pRspInfoField, int nRequestID) override;

	virtual void OnRtnTrade(CTORATstpTradeField* pTradeField) override;

	virtual void OnErrRtnOrderAction(CTORATstpInputOrderActionField* pInputOrderActionField, CTORATstpRspInfoField* pRspInfoField, int nRequestID) override;

	virtual void OnRspQryOrder(CTORATstpOrderField* pOrderField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryTrade(CTORATstpTradeField* pTradeField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryPosition(CTORATstpPositionField* pPositionField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryTradingAccount(CTORATstpTradingAccountField* pTradingAccountField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

	virtual void OnRspUserLogin(CTORATstpRspUserLoginField* pRspUserLoginField, CTORATstpRspInfoField* pRspInfoField, int nRequestID) override;

	virtual void OnRspGetConnectionInfo(CTORATstpConnectionInfoField* pConnectionInfoField, CTORATstpRspInfoField* pRspInfo, int nRequestID) override;

	virtual void OnRspQryShareholderAccount(CTORATstpShareholderAccountField* pShareholderAccountField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast) override;

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

	inline WTSOrderInfo*	makeOrderInfo(CTORATstpOrderField* orderField);
	inline WTSEntrust*		makeEntrust(CTORATstpInputOrderField* entrustField);
	inline WTSTradeInfo*	makeTradeInfo(CTORATstpTradeField*tradeField);
	inline WTSPositionItem* makePositionItem(CTORATstpPositionField* positionField);
	inline WTSAccountInfo*	makeAccountInfo(CTORATstpTradingAccountField* accountField);

	inline WTSDirectionType wrapDirectionType(TTORATstpDirectionType dirType);
	inline WTSPriceType wrapPriceType(TTORATstpDirectionType priceType);
	inline WTSOrderState wrapOrderState(TTORATstpOrderStatusType orderState);


	inline bool	extractEntrustID(const char* entrustid, int &orderRef);
	inline void	genEntrustID(char* buffer, uint32_t orderRef);

private:
	HuaXTraderApi*	_api;
	ITraderSpi*		_sink;

	typedef WTSHashMap<std::string> PositionMap;
	WTSArray*				_positions;
	WTSArray*				_trades;
	WTSArray*				_orders;
	WTSArray*				_accounts;

	IBaseDataMgr*			_bd_mgr;

	std::string		_user;
	std::string		_pass;
	std::string		_flowdir;
	bool			_encrypt;  // 为网络数据是否加密传输，考虑数据安全性，建议以互联网方式接入的终端设置为加密传输
	std::string		_appid; // 终端名称
	std::string		_sse_share_hold_id; // 沪市股东号
	std::string		_szse_share_hold_id; // 深市股东号
	std::string		_terminal; // 终端类型，默认PC
	std::string		_pub_ip; // 公网IP
	std::string		_pub_port; // 公网IP端口号
	std::string		_trade_ip; // 内网交易IP
	std::string		_mac; // MAC地址，托管服务器网卡MAC
	std::string		_hard_disk; // 硬盘序列号,托管服务器硬盘序列号

	std::string		_front; // 如tcp://210.14.72.16:9500

	bool			_quick; // 流重传方式
	bool			_inited;

	std::string		_productInfo;

	TraderState		_state;

	uint32_t		_tradingday;
	std::atomic<uint32_t>		_reqid;
	std::atomic<int>		_ordref;		//报单引用

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	DllHandle		_hInstHuaX;
	typedef HuaXTraderApi* (*HuaXCreator)(const char*, bool);
	HuaXCreator		_funcCreator;

	//委托单标记缓存器
	WtKVCache		_eidCache;
	//订单标记缓存器
	WtKVCache		_oidCache;
};

