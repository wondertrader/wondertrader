/*!
 * \file ITraderApi.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 交易通道对接接口定义文件
 */
#pragma once
#include <set>
#include <map>
#include <stdint.h>
#include <functional>
#include "WTSTypes.h"

NS_WTP_BEGIN
class WTSVariant;
class WTSEntrust;
class WTSOrderInfo;
class WTSTradeInfo;
class WTSEntrustAction;
class WTSAccountInfo;
class WTSPositionItem;
class WTSContractInfo;
class WTSError;
class WTSTickData;
class WTSNotify;
class WTSArray;
class IBaseDataMgr;

typedef std::function<void()>	CommonExecuter;

#pragma region "Stock Trading API definations"
/*
 *	股票交易接口回调
 *	Added By Wesley @ 2020/05/06
 */
class IStkTraderSpi
{

};

/*
 *	股票交易接口
 *	Added By Wesley @ 2020/05/06
 *	预留起来以后使用,先把接口的相互框架搭建起来
 *	主要提供融资融券等股票特有接口
 */
class IStkTraderApi
{
};
#pragma endregion

#pragma region "Option Trading API definations"
/*
 *	期权交易接口回调
 *	Added By Wesley @ 2020/05/06
 */
class IOptTraderSpi
{
public:
	virtual void onRspEntrustOpt(WTSEntrust* entrust, WTSError *err) {}
	virtual void onRspOrdersOpt(const WTSArray* ayOrders) {}
	virtual void onPushOrderOpt(WTSOrderInfo* orderInfo) {}
};

/*
 *	期权交易接口
 *	Added By Wesley @ 2020/05/06
 *	预留起来以后使用,先把接口的相互框架搭建起来
 *	主要提供报价、行权等期权特有接口
 */
class IOptTraderApi
{
public:
	/*
	 *	下单接口
	 *	entrust 下单的具体数据结构
	 */
	virtual int orderInsertOpt(WTSEntrust* eutrust) { return -1; }

	/*
	 *	订单操作接口
	 *	action	操作的具体数据结构
	 */
	virtual int orderActionOpt(WTSEntrustAction* action) { return -1; }

	/*
	 *	查询期权订单
	 */
	virtual int	queryOrdersOpt(WTSBusinessType bType) { return -1; }
};
#pragma endregion


//委托回调接口
class ITraderSpi
{
public:
	virtual IBaseDataMgr*	getBaseDataMgr() = 0;
	virtual void handleTraderLog(WTSLogLevel ll, const char* message){}

	virtual IStkTraderSpi* getStkSpi(){ return NULL; }
	virtual IOptTraderSpi* getOptSpi(){ return NULL; }

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

	virtual void onTraderError(WTSError* err){}
};

//下单接口管理接口
class ITraderApi
{
public:
	virtual ~ITraderApi(){}

	virtual IStkTraderApi* getStkTrader() { return NULL; }
	virtual IOptTraderApi* getOptTrader() { return NULL; }

public:
	/*
	 *	初始化解析管理器
	 */
	virtual bool init(WTSVariant *params) { return false; }

	/*
	 *	释放解析管理器
	 */
	virtual void release(){}

	/*
	 *	注册回调接口
	 */
	virtual void registerSpi(ITraderSpi *listener) {}


	//////////////////////////////////////////////////////////////////////////
	//业务逻辑接口

	/*
	 *	连接服务器
	 */
	virtual void connect() {}

	/*
	 *	断开连接
	 */
	virtual void disconnect() {}

	virtual bool isConnected() { return false; }

	/*
	 *	生成委托单号
	 */
	virtual bool makeEntrustID(char* buffer, int length){ return false; }

	/*
	 *	登录接口
	 */
	virtual int login(const char* user, const char* pass, const char* productInfo) { return -1; }

	/*
	 *	注销接口
	 */
	virtual int logout() { return -1; }

	/*
	 *	下单接口
	 *	entrust 下单的具体数据结构
	 */
	virtual int orderInsert(WTSEntrust* eutrust) { return -1; }

	/*
	 *	订单操作接口
	 *	action	操作的具体数据结构
	 */
	virtual int orderAction(WTSEntrustAction* action) { return -1; }

	/*
	 *	查询账户信息
	 */
	virtual int queryAccount() { return -1; }

	/*
	 *	查询持仓信息
	 */
	virtual int queryPositions() { return -1; }

	/*
	 *	查询所有订单
	 */
	virtual int queryOrders() { return -1; }

	/*
	 *	查询成交明细
	 */
	virtual int	queryTrades() { return -1; }

	/*
	 *	查询结算单
	 */
	virtual int querySettlement(uint32_t uDate){ return 0; }

};

NS_WTP_END

//获取IDataMgr的函数指针类型
typedef wtp::ITraderApi* (*FuncCreateTrader)();
typedef void(*FuncDeleteTrader)(wtp::ITraderApi* &trader);
