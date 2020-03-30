#pragma once
#include <set>
#include <map>
#include <stdint.h>
#include <functional>
#include "WTSTypes.h"

NS_OTP_BEGIN
class WTSParams;
class WTSEntrust;
class WTSOrderInfo;
class WTSTradeInfo;
class WTSEntrustAction;
class WTSAccountInfo;
class WTSInvestorInfo;
class WTSPositionItem;
class WTSPositionDetail;
class WTSContractInfo;
class WTSError;
class WTSTickData;
class WTSNotify;
class WTSArray;
class IBaseDataMgr;

typedef std::function<void()>	CommonExecuter;


//委托回调接口
class ITraderApiListener
{
public:
	virtual void handleEvent(WTSTraderEvent e, int32_t ec) = 0;
	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate) = 0;
	virtual void onLogout(){}
	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err){}
	virtual void onRspAccount(WTSArray* ayAccounts) {}
	virtual void onRspPosition(const WTSArray* ayPositions){}
	virtual void onRspOrders(const WTSArray* ayOrders){}
	virtual void onRspTrades(const WTSArray* ayTrades){}
	virtual void onRspSettlementInfo(uint32_t uDate, const char* content){}

	virtual void onPushOrder(WTSOrderInfo* orderInfo){}
	virtual void onPushTrade(WTSTradeInfo* tradeRecord){}

	virtual void onTraderError(WTSError*	err){}

public:
	virtual IBaseDataMgr*	getBaseDataMgr() = 0;
	virtual void handleTraderLog(WTSLogLevel ll, const char* format, ...){}
};

//下单接口管理接口
class ITraderApi
{
public:
	virtual ~ITraderApi(){}

public:
	/*
	*	初始化解析管理器
	*/
	virtual bool init(WTSParams *params) = 0;

	/*
	*	释放解析管理器
	*/
	virtual void release() = 0;

	/*
	*	注册回调接口
	*/
	virtual void registerListener(ITraderApiListener *listener) = 0;


	//////////////////////////////////////////////////////////////////////////
	//业务逻辑接口

	/*
	*	连接服务器
	*/
	virtual void connect() = 0;

	/*
	*	断开连接
	*/
	virtual void disconnect() = 0;

	virtual bool isConnected() = 0;

	/*
	*	生成委托单号
	*/
	virtual bool makeEntrustID(char* buffer, int length){ return false; }

	/*
	*	登录接口
	*/
	virtual int login(const char* user, const char* pass, const char* productInfo) = 0;

	/*
	*	注销接口
	*/
	virtual int logout() = 0;

	/*
	*	下单接口
	*	entrust 下单的具体数据结构
	*/
	virtual int orderInsert(WTSEntrust* eutrust) = 0;

	/*
	*	订单操作接口
	*	action	操作的具体数据结构
	*/
	virtual int orderAction(WTSEntrustAction* action) = 0;

	/*
	*	查询账户信息
	*/
	virtual int queryAccount() = 0;

	/*
	*	查询持仓信息
	*/
	virtual int queryPositions() = 0;

	/*
	*	查询所有订单
	*/
	virtual int queryOrders() = 0;

	/*
	*	查询成交明细
	*/
	virtual int	queryTrades() = 0;

	/*
	 *	查询结算单
	 */
	virtual int querySettlement(uint32_t uDate){ return 0; }

};

NS_OTP_END

//获取IDataMgr的函数指针类型
typedef otp::ITraderApi* (*FuncCreateTrader)();
typedef void(*FuncDeleteTrader)(otp::ITraderApi* &trader);
