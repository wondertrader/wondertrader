#pragma once
//v6.7.11
#include "../API/CTP6.7.11/ThostFtdcTraderApi.h"
#include "../Share/StdUtils.hpp"
#include "../Share/SpinMutex.hpp"
#include "../Includes/LoaderDef.hpp"

#include <functional>
#include <queue>
#include <boost/thread.hpp>

using namespace loader;

typedef std::function<bool()> QueryTask;

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	CTraderSpi():_stopped(false){}
	~CTraderSpi()
	{
		_stopped = true;
		if (_worker)
			_worker->join();
	}

public:
	///当客户端与交易后台建立起通信连接时（还未登录前）,该方法被调用。
	virtual void OnFrontConnected();

	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///当客户端与交易后台通信连接断开时,该方法被调用。当发生这个情况后,API会自动重新连接,客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);

	// CTP 6.7.11基本虚函数 - 只添加必需的函数
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) override {}
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) override {}
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) override {}
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) override {}
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) override {}
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {}
	virtual void OnHeartBeatWarning(int nTimeLapse) override {}

private:
	void ReqAuth();
	///用户登录请求
	void ReqUserLogin();
	///请求查询合约
	void ReqQryInstrument();

	void ReqQryCommission(const Contract& cInfo);

	void ReqQryMargin(const Contract& cInfo);

	// 是否收到成功的响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	void DumpToJson();

	void DumpFees();

	void LoadFromJson();

	void AppendQuery(const QueryTask& task);

private:
	int		_trading_day;

	std::queue<QueryTask>	_queries;
	SpinMutex		_mtx;
	std::shared_ptr<boost::thread>	_worker;
	bool			_stopped;
};