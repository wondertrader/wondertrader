#pragma once
#include "XTTradeStruct.h"

class ITDErebusApp
{
public:
	virtual		int		BeginWork(const char* param) = 0;

	virtual		void	EndWork() = 0;
};

class ITDErebusSpi
{
public:
	virtual		void	OnFrontConnected(){}

	virtual		void	OnFrontDisconnected(){}

	virtual		void	OnRspUserLogin(tagXTRspUserLoginField* data, tagXTRspInfoField* error, int id, bool last){}

	virtual		void	OnRspQryTradingAccount(tagXTRspAccountField* data, tagXTRspInfoField* error, int id, bool last){}

	virtual		void	OnRspQryInvestorPosition(tagXTRspPositionField* data, tagXTRspInfoField* error, int id, bool last){}

	virtual		void	OnRspQryOrder(tagXTOrderField* data, tagXTRspInfoField* error, int id, bool last){}

	virtual		void	OnRspQryTrade(tagXTTradeField* data, tagXTRspInfoField* error, int id, bool last){}

	virtual		void	OnRspOrderInsert(tagXTReqOrderInsertField* data, tagXTRspInfoField* error, int id, bool last){}

	virtual		void	OnErrRtnOrderInsert(tagXTReqOrderInsertField* data, tagXTRspInfoField* error){}

	virtual		void	OnRspOrderAction(tagXTReqOrderCancelField* data, tagXTRspInfoField* error, int id, bool last){}

	virtual		void	OnErrRtnOrderAction(tagXTReqOrderCancelField* data, tagXTRspInfoField* error){}

	virtual		void	OnRtnOrder(tagXTOrderField* data){}

	virtual		void	OnRtnTrade(tagXTTradeField* data){}
protected:
private:
};

class ITDErebusApi
{
public:
	virtual		void	RegisterSpi(ITDErebusSpi*) = 0;

	virtual		int		ReqLogon(tagXTReqUserLoginField* req) = 0;

	virtual		void	ReqLogout() = 0;

	virtual		int		ReqQryAccount(tagXTReqQryAccountField* req) = 0;

	virtual		int		ReqQryPosition(tagXTReqQryPositionField* req) = 0;

	virtual		int		ReqQryOrder(tagXTReqQryOrderField* req) = 0;

	virtual		int		ReqQryTrade(tagXTReqQryTradeField* req) = 0;

	virtual		int		ReqOrderInsert(tagXTReqOrderInsertField* req) = 0;

	virtual		int		ReqOrderCancel(tagXTReqOrderCancelField* req) = 0;
protected:

private:
};































