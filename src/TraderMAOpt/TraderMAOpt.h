/*!
 * \file TraderMAOpt.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2022/07/14
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

#include "../API/mCliApi3.7/maCliOptTradeApi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"


 // 用户角色
typedef enum
{
	MA_CLIENT = '1',
	MA_TELLER = '2'
} MA_USER_ROLE;

// 交易市场
typedef enum
{
	MA_SZSE = '0',
	MA_SSE
} MA_STK_EX;

// 交易板块
typedef enum
{
	MA_SZSE_A = '00',
	MA_SZSE_B = '01',
	MA_THREE_BOARD = '02',
	MA_SZSE_OPT = '05',
	MA_SSE_A = '10',
	MA_SSE_B = '11',
	MA_SSE_OPT = '15'
} MA_STK_BD;

// 证券类别
typedef enum
{
	MA_STOCK = 'A',
	MA_ETF = 'D',
	MA_STK_OPT = 'U'
} MA_STK_CLS;

// 货币代码
typedef enum
{
	MA_RMB = '0',
	MA_HK = '1',
	MA_US = '2'
} MA_CURRENCY;

// 证券业务
typedef enum
{
	MA_BuyOpen = 400,  // 买开
	MA_SellClose,	   // 卖平
	MA_SellOpen,	  // 卖开
	MA_BuyClose,       // 买平
	MA_OPT_CALL = 406,  // 认购行权
	MA_OPT_PUT        // 认沽行权
} MA_STK_BIZ;

// 证券业务活动
typedef enum
{
	MA_OPT_LIMITPRICE_GFD = 130,  // 股票期权申报-限价GFD
	MA_OPT_LIMITPRICE_FOK = 131,  // 股票期权申报-限价FOK
	MA_OPT_ANYPRICE_FOK = 133,  // 股票期权申报-市价FOK
	MA_OPT_ANYPRICE_IOC = 134,  // 股票期权申报-市价IOC
	MA_OPT_ANYPRICE_LEFT_LIMIT = 132,  // 股票期权申报-市价剩转限价GFD
	MA_ORDER = 100,  // 委托申报
	MA_CANCEL_ORDER = 101,  // 撤单申报
	MA_BESTFIVELEVELPRICE = 121,  // 市价最优五档即时成交剩余撤销
	MA_MARKEPRICE_ALLORDRAW = 122,  // 市价全额成交或撤销
	MA_BESTPRICE_THIS_SIDE = 123,  // 市价本方最优价格
	MA_BESTPRICE_OTHER_SIDE = 124,  // 市价对手方最优价格
	MA_MARKETPRICE_LEFT_CANCEL = 125  // 市价即时成交剩余撤销
} MA_STK_BIZ_ACTION;

typedef enum
{
	MA_GFD = 600,  // 当日有效
	MA_FOK,  // 全成或者全撤
	MA_IOC  // 立即成交，否则取消
} MATimeConditionType;

// 撤单标志
typedef enum
{
	MA_ORDER_CANCEL = 'T',  // 撤单
	MA_ORDER_NORMAL = 'F'  // 正常
} MA_IS_WITHDRAW;

// 委托有效标志
typedef enum
{
	MA_ORDER_VALID = '0',  // 无效
	MA_ORDER_INVALID = '1'  // 有效
} MA_ORDER_VALID_FLAG;

// 委托状态
typedef enum tagOrderState
{
	MA_Nottouched = '0',  // 未报, 写入委托的最初阶段，该笔业务还没有到申报时间
	MA_Submitting, // 正报，该标志是漏单扫描的中间状态，表示正在判断中
	MA_Submitted, // 已报，已经成功写入接口库
	MA_Withdraw_NotFilled, // 已报待撤，该笔业务被撤单，但撤单未成交
	MA_PartFilled_ToWithdraw, // 部成待撤，该笔业务部分成交，剩余部分等待撤单
	MA_PartWithdraw, // 部撤，部分撤单
	MA_Withdawn, // 已撤，撤单已经成交
	MA_PartFilled_Withdrawn, // 部成已撤，部分成交
	MA_AllFilled, // 已成，已经成交
	MA_Canceled, // 废单，被交易所自动撤单
	MA_ToSubmit = 'A', // 待报，写入报盘队列未成功
	MA_OFFER_CONFIRM = 'B'  // 报盘机确认
}  MAOrderState;

// 持仓方向
typedef enum
{
	MA_LONG = 'L',   // 权力仓
	MA_SHORT = 'S',  // 义务仓
	MA_COVERED_CALL = 'C'  // 备兑策略持仓
} MA_OPT_SIDE;

// 合约类型
typedef enum
{
	MA_OPT_TYPE_CALL = 'C',  // 认购
	MA_OPT_TYPE_PUT = 'P'  // 认沽
} MA_OPT_TYPE;

// 结算单类型
typedef enum
{
	MA_DAY_SETTLEMENT = 'D',  // 日结算单
	MA_MONTH_SETTLEMENT = 'M',  // 月结算单
	MA_TEMP_SETTLEMENT = 'T'  // 临时结算单
} MA_SETT_LIST_TYPE;

// 冲销状态
typedef enum
{
	MA_CANCEL_NORMAL = '0',  // 正常
	MA_CANCEL_LOCKED,  // 被冲锁定
	MA_CANCELED,  // 已被冲销
	MA_FZN_CANCEL  // 已冻结取消
} MA_CANCEL_STATUS;


USE_NAMESPACE_MACLI

USING_NS_WTP;

class TraderMAOpt : public ITraderApi, public IOptTraderApi, public CCliOptTradeSpi
{
public:
	TraderMAOpt();
	virtual ~TraderMAOpt();

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

	virtual int orderInsert(WTSEntrust* entrust) override;

	virtual int orderAction(WTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;

	virtual int querySettlement(uint32_t uDate) override;


	//////////////////////////////////////////////////////////////////////////
	//IOptTraderApi
	virtual int orderInsertOpt(WTSEntrust* entrust) override;

	virtual int orderActionOpt(WTSEntrustAction* action) override;

	virtual int	queryOrdersOpt(WTSBusinessType bType) override;


	//////////////////////////////////////////////////////////////////////////
	//maCliOpt交易接口实现
public:
	// 客户端与服务器成功建立通信连接后，该方法被调用
	virtual int OnConnected(void) override;

	// 客户端与服务器成功的通信连接断开时，该方法被调用
	virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) override;

	// 心跳响应
	virtual int OnRspHeartBeat(CFirstSetField *p_pFirstSet, CRspHeartBeatField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 用户登录请求响应
	virtual int OnRspUserLogin(CFirstSetField *p_pFirstSet, CRspOptUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 可用资金查询响应
	virtual int OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, CRspOptExpendableFundField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 可用合约资产查询响应
	virtual int OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, CRspOptExpendableCuField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 个股期权合约账户查询响应
	virtual int OnRspQryAcct(CFirstSetField* p_pFirstSetField, CRspOptAcctField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 个股期权委托撤单响应
	virtual int OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 确认回报
	virtual int OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRspField) override;

	// 成交回报--委托信息
	virtual int OnRtnOrder(CRtnOptOrderField* p_pRspField) override;

	// 成交回报--成交信息
	virtual int OnRtnOrderFill(CRtnOptOrderFillField* p_pRtnField) override;

	// 成交回报--合约信息
	virtual int OnRtnContract(CRtnOptContractField* p_pRspField) override;

	// 个股期权当日委托查询响应
	virtual int OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, CRspOptCurrDayOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 个股期权当日成交查询响应
	virtual int OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, CRspOptCurrDayFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	//可用组合持仓明细查询响应 2015/8/5
	virtual int OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSetField, CRspOptCombStraPosDetailField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	/////////////////// 自定义接口
	// 资金账户查询
	int RspQryTradingAccount(void);

	// 持仓查询
	int RspQryPosition();

protected:
	/*
	*	检查错误信息
	*/
	//bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);


	WTSOrderInfo*	makeOrderInfo(CRspOptOrderField* orderField);
	WTSOrderInfo*	makeOrderInfo(CRtnOptOrderField* orderField);
	WTSOrderInfo*   makeOrderInfo(CRspOptCurrDayOrderField* daiOrderField);
	//WTSOrderInfo*   makeOrderInfo(CRspOptCancelOrderField* cancelOrderField);
	WTSEntrust*		makeEntrust(CReqOptOrderField  *entrustField);
	//WTSError*		makeError(CThostFtdcRspInfoField* rspInfo);
	WTSTradeInfo*	makeTradeRecord(CRtnOptOrderFillField *tradeField);
	WTSTradeInfo*   makeTradeRecord(CRspOptCurrDayFillField *dayTradeField);
	WTSEntrust*		makeEntrust(CRtnOptOrderFillField  *entrustField);

	void			generateEntrustID(char* buffer, long long ll_cust_code, long long ll_cuacct_code, int order_bsn);
	bool			extractEntrustID(const char* entrustid, long long &ll_cust_code, long long &ll_cuacct_code, int &order_bsn);

	uint32_t		genRequestID();

protected:
	int64_t			m_llCustCode;  // 客户代码
	int64_t			m_llCuacctCode;  // 资金户代码
	int				m_iInitOrg;  // 机构代码
	std::string		m_strChannel;  // 操作渠道

	std::string		m_strHost;  // IP地址
	int				m_iPort;  // 端口

	std::string		m_strUser;
	std::string		m_strPass;
	std::string		m_strTrdAcct;  // 交易账户

	std::string		m_strAuthData;
	std::string		m_strEncryptKey;

	std::string		m_strUseScope;
	std::string		m_strAuthType;
	std::string		m_strEncryptType;
	std::string		m_strAcctType;

	std::string		m_strStkBD;  // 交易板块
	std::string		m_strStkPBU;  // 交易单元

	ITraderSpi*		m_bscSink;
	IOptTraderSpi*	m_optSink;

	uint32_t					m_lDate;
	std::atomic<uint32_t>		m_orderRef;		//报单引用

	WrapperState				m_wrapperState;

	CCliOptTradeApi*			m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*				m_mapPosition;
	WTSArray*					m_ayTrades;
	WTSArray*					m_ayOrders;

	IBaseDataMgr*				m_bdMgr;

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	std::string		m_strModule;
	DllHandle		m_hInstMA;

	WTSAccountInfo*         m_accountInfo;

	//委托单标记缓存器
	WtKVCache		m_eidCache;
	//订单标记缓存器
	WtKVCache		m_oidCache;
};
