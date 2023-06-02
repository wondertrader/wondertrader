/*!
 * \file TraderMAOpt.h
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
#include <stdint.h>

#include <boost/asio/io_service.hpp>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../API/maCliApi3.9/include/maCliOptTradeApi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"


USING_NS_WTP;

USE_NAMESPACE_MACLI;


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
	//virtual int orderInsertOpt(WTSEntrust* entrust) override;

	//virtual int orderActionOpt(WTSEntrustAction* action) override;

	//virtual int	queryOrdersOpt(WTSBusinessType bType) override;


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


	// 个股期权委托申报响应
	virtual int OnRspOrder(CFirstSetField *p_pFirstSet, CRspOptOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 个股期权委托撤单响应
	virtual int OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 确认回报
	virtual int OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRspField) override;
	//确认回报推送（极速）响应
	virtual int OnRtnOrderConfirmFlash(CRtnOptOrderConfirmFlashField *p_pRtnField);

	// 成交回报--委托信息
	virtual int OnRtnOrder(CRtnOptOrderField* p_pRspField) override;

	// 成交回报--成交信息
	virtual int OnRtnOrderFill(CRtnOptOrderFillField* p_pRtnField) override;
	//成交回报推送(极速）响应
	virtual int OnRtnOrderFillFlash(CRtnOptOrderFillFlashField *p_pRtnField);

	virtual int OnRtnSubTopic(CRspSubTopicField* p_pRspField) override;

	// 成交回报--合约信息
	virtual int OnRtnContract(CRtnOptContractField* p_pRspField) override;

	//客户结算单查询 2015/8/6
	virtual int OnRspQrySettList(CFirstSetField* p_pFirstSet, CRspOptSettListField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 个股期权当日委托查询响应
	virtual int OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, CRspOptCurrDayOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// 个股期权当日成交查询响应
	virtual int OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, CRspOptCurrDayFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

protected:
	/*
	*	检查错误信息
	*/
	bool IsErrorRspInfo(CFirstSetField *pRspInfo);

	WTSOrderInfo*	makeOrderInfo(CRspOptCancelOrderField* orderField);
	WTSOrderInfo*	makeOrderInfo(CRspOptOrderField* orderField);
	WTSOrderInfo*	makeOrderInfo(CRtnOptOrderField* orderField);
	WTSOrderInfo*   makeOrderInfo(CRspOptCurrDayOrderField* daiOrderField);
	WTSEntrust*		makeEntrust(CRspOptOrderField  *entrustField);
	WTSError*		makeError(CFirstSetField* rspInfo);
	WTSTradeInfo*	makeTradeRecord(CRtnOptOrderFillField *tradeField);
	WTSTradeInfo*   makeTradeRecord(CRspOptCurrDayFillField *dayTradeField);
	WTSEntrust*		makeEntrust(CRtnOptOrderFillField  *entrustField);

	void			generateEntrustID(char* buffer, int order_bsn);
	bool			extractEntrustID(const char* entrustid, int &order_bsn);

	uint32_t		genRequestID();

protected:
	std::string		m_strChannel;  // 操作渠道

	std::string		m_strHost;  // IP地址
	int				m_iPort;  // 端口

	std::string		m_strUser;
	std::string		m_strPass;

	std::string		m_strTrdAcct;		// 交易账户
	int64_t			m_llCustCode;		// 客户代码
	int64_t			m_llCuacctCode;		// 资金户代码
	int32_t			m_iInitOrg;

	std::string		m_strAuthData;
	std::string		m_strEncryptKey;

	std::string		m_strUseScope;
	std::string		m_strAuthType;
	std::string		m_strEncryptType;
	std::string		m_strAcctType;
	std::string		m_strOptSite;  // 包头

	std::string		m_strShPBU;  // 交易单元
	std::string		m_strSzPBU;  // 交易单元

	ITraderSpi*					m_bscSink;
	IOptTraderSpi*				m_optSink;

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

	std::string					m_strModule;
	DllHandle					m_hInstMA;

	//委托单标记缓存器
	WtKVCache					m_eidCache;

	//成交单标记缓存器
	WtKVCache					m_oidCache;
};
