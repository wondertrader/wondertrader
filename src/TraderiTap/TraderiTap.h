#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdint.h>
#include <string.h>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"
#include "../Share/DLLHelper.hpp"

#include "./iTapTradeApi/iTapTradeAPI.h"

USING_NS_OTP;

using namespace ITapTrade;

typedef std::unordered_set<std::string>	ContractSet;

typedef std::unordered_map<std::string, std::string>	ProductMap;


class TraderiTap : public ITraderApi, public ITapTradeAPINotify
{
public:
	TraderiTap();
	virtual ~TraderiTap();

public:
	typedef enum 
	{
		WS_NOTLOGIN,		//未登录
		WS_LOGINING,		//正在登录
		WS_DYNAUTH,			//动态认证
		WS_LOGINED,			//已登录
		WS_LOGINFAILED,		//登录失败
		WS_APIINITED,		//接口初始化
		WS_COMMQRYED,		//品种已查
		WS_ALLREADY			//全部就绪
	} WrapperState;


private:
	int queryCommodity();

	void reconnect();

	const char* GetErrcodeDesc(int ec);

//////////////////////////////////////////////////////////////////////////
//ISimpleTraderApi接口
public:
	virtual bool init(WTSParams* params);

	virtual void release();

	virtual void setTag(const char* tag){m_strTag = tag;}
	virtual const char* getTag(){return m_strTag.c_str();}

	virtual void registerListener(ITraderApiListener *listener);

	virtual bool makeEntrustID(char* buffer, int length);

	virtual void connect();

	virtual void disconnect();

	virtual bool isConnected();

	virtual int login( const char* user, const char* pass, const char* productInfo );

	virtual int logout();

	virtual int orderInsert( WTSEntrust* eutrust );

	virtual int orderAction( WTSEntrustAction* action );

	virtual int queryContracts();

	virtual int queryAccount();

	virtual int queryPositions();

	virtual int queryOrders();

	virtual int queryTrades();


//////////////////////////////////////////////////////////////////////////
//ITapTradeAPINotify交易接口实现
public:
	virtual void TAP_CDECL OnAPIReady(ITapTrade::TAPIINT32 errorCode) override;

	virtual void TAP_CDECL OnConnect() override;

	virtual void TAP_CDECL OnDisconnect(ITapTrade::TAPIINT32 reasonCode) override;

	virtual void TAP_CDECL OnExpriationDate(ITapTrade::TAPIDATE date, int days) override;

	virtual void TAP_CDECL OnHKMarketQuoteNotice(const ITapTrade::TapAPIOrderQuoteMarketNotice *info) override;

	virtual void TAP_CDECL OnRspAuthPassword(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode) override;

	virtual void TAP_CDECL OnRspChangePassword(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode) override;

	virtual void TAP_CDECL OnRspFillLocalInput(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIFillLocalInputRsp *info) override;

	virtual void TAP_CDECL OnRspFillLocalRemove(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIFillLocalRemoveRsp *info) override;

	virtual void TAP_CDECL OnRspHKMarketOrderDelete(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIOrderMarketDeleteRsp *info) override;

	virtual void TAP_CDECL OnRspHKMarketOrderInsert(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIOrderMarketInsertRsp *info) override;

	virtual void TAP_CDECL OnRspLogin(ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPITradeLoginRspInfo *loginRspInfo) override;

	virtual void TAP_CDECL OnRspOrderAction(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIOrderActionRsp *info) override;

	virtual void TAP_CDECL OnRspOrderLocalInput(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIOrderLocalInputRsp *info) override;

	virtual void TAP_CDECL OnRspOrderLocalModify(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIOrderLocalModifyRsp *info) override;

	virtual void TAP_CDECL OnRspOrderLocalRemove(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIOrderLocalRemoveRsp *info) override;

	virtual void TAP_CDECL OnRspOrderLocalTransfer(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPIOrderLocalTransferRsp *info) override;

	virtual void TAP_CDECL OnRspQryAccount(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIUINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountInfo *info) override;

	virtual void TAP_CDECL OnRspQryAccountCashAdjust(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountCashAdjustQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryAccountFeeRent(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountFeeRentQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryAccountMarginRent(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountMarginRentQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryBill(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIBillQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryCommodity(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPICommodityInfo *info) override;

	virtual void TAP_CDECL OnRspQryContract(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPITradeContractInfo *info) override;

	virtual void TAP_CDECL OnRspQryCurrency(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPICurrencyInfo *info) override;

	virtual void TAP_CDECL OnRspQryExchange(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIExchangeInfo *info) override;

	virtual void TAP_CDECL OnRspQryFill(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIFillInfo *info) override;

	virtual void TAP_CDECL OnRspQryFund(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIFundData *info) override;

	virtual void TAP_CDECL OnRspQryOrder(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIOrderInfo *info) override;

	virtual void TAP_CDECL OnRspQryOrderProcess(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIOrderInfo *info) override;

	virtual void TAP_CDECL OnRspQryPosition(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIPositionInfo *info) override;

	virtual void TAP_CDECL OnRspRequestVertificateCode(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const TapAPIRequestVertificateCodeRsp *rsp) override;

	virtual void TAP_CDECL OnRtnContactInfo(ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TAPISTR_40 ContactInfo) override;

	virtual void TAP_CDECL OnRtnContract(const ITapTrade::TapAPITradeContractInfo *info) override;

	virtual void TAP_CDECL OnRtnFill(const ITapTrade::TapAPIFillInfo *info) override;

	virtual void TAP_CDECL OnRtnFund(const ITapTrade::TapAPIFundData *info) override;

	virtual void TAP_CDECL OnRtnOrder(const ITapTrade::TapAPIOrderInfoNotice *info) override;

	virtual void TAP_CDECL OnRtnPosition(const ITapTrade::TapAPIPositionInfo *info) override;

	virtual void TAP_CDECL OnRtnPositionProfit(const ITapTrade::TapAPIPositionProfitNotice *info) override;

	virtual void TAP_CDECL OnRtnPositionSummary(const TapAPIPositionSummary *info) override;

	virtual void TAP_CDECL OnRtnTradeMessage(const ITapTrade::TapAPITradeMessage *info) override;

	virtual void TAP_CDECL OnRspQryTradingDate(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPITradingCalendarQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryPositionSummary(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIPositionSummary *info) override;

	virtual void TAP_CDECL OnRspQryTradeMessage(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPITradeMessage *info) override;

	virtual void TAP_CDECL OnRspQryHisOrder(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisOrderQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryHisOrderProcess(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisOrderProcessQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryHisMatch(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisMatchQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryHisPosition(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisPositionQryRsp *info) override;

	virtual void TAP_CDECL OnRspQryHisDelivery(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisDeliveryQryRsp *info) override;

	virtual void TAP_CDECL OnRspSetReservedInfo(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TAPISTR_50 info) override;

	virtual void TAP_CDECL OnRspQrySpotLock(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPISpotLockDataRsp *info) override;

	virtual void TAP_CDECL OnRtnSpotLock(const ITapTrade::TapAPISpotLockDataRsp *info) override;

	virtual void TAP_CDECL OnRspSubmitUserLoginInfo(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPISubmitUserLoginRspInfo * info) override;

	virtual void TAP_CDECL OnRspSpecialOrderAction(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPISpecialOrderInfo *info) override;

	virtual void TAP_CDECL OnRtnSpecialOrder(const ITapTrade::TapAPISpecialOrderInfo *info) override;

	virtual void TAP_CDECL OnRspQrySpecialOrder(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPISpecialOrderInfo *info) override;

	virtual void TAP_CDECL OnRspQryCombinePosition(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPICombinePositionInfo *info) override;

	virtual void TAP_CDECL OnRtnCombinePosition(const ITapTrade::TapAPICombinePositionInfo *info) override;


protected:
	int wrapPriceType(WTSPriceType priceType);
	int wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offType);
	int wrapOffsetType(WTSOffsetType offType);
	int	wrapTimeCondition(WTSTimeCondition timeCond);

	WTSPriceType		wrapPriceType(TAPIOrderTypeType priceType);
	WTSTimeCondition	wrapTimeCondition(TAPITimeInForceType timeCond);
	WTSDirectionType	wrapDirectionType(TAPISideType dirType, TAPIPositionEffectType offType);
	WTSDirectionType	wrapPosDirection(TAPISideType dirType);
	WTSOffsetType		wrapOffsetType(TAPIPositionEffectType offType);
	WTSOrderState		wrapOrderState(TAPIOrderStateType orderState);

	WTSTradeInfo*	makeTradeRecord(const TapAPIFillInfo* tradeField);
	WTSOrderInfo*	makeOrderInfo(const TapAPIOrderInfo* orderField);
	WTSOrderInfo*	makeOrderInfo(const TapAPIOrderInfoNotice* orderField);
	WTSEntrust*		makeEntrust(const TapAPIOrderLocalInputRsp* entrustField);
	WTSError*		makeErrorInfo(WTSErroCode errorCode, const char* msg);


	const char* exchgI2O(const char* exchg);
	const char* exchgO2I(const char* exchg);

	const char* productI2O(const char* pid);
	const char* productO2I(const char* pid);

	const char* currencyO2I(const char* currency);

	uint32_t*	makeRequestID();

protected:
	std::string			m_strHost;
	int					m_nPort;
	std::string			m_strUser;
	std::string			m_strPass;
	std::string			m_strCodePath;

	std::string			m_strAppKey;

	std::string			m_strTag;

	std::string			m_strUserName;

	char				m_strContract[41];

	uint64_t			m_uLoginTime;
	uint32_t			m_uDate;
	bool				m_bStopped;
	bool				m_bReconnect;

	ITraderApiListener*	m_traderSink;

	WrapperState	m_wrapperState;

	ITapTradeAPI*	m_pUserAPI;
	char			m_strLocalID[51];

	std::string		m_strModule;
	DllHandle		m_hInstESF;
	typedef ITapTradeAPI* ( *ESFCreator)(const ITapTrade::TapAPIApplicationInfo *appInfo, ITapTrade::TAPIINT32 &iResult);
	typedef void( *ESFDeleter)(ITapTradeAPI*);
	typedef const char*( *ESFGetErrorDesc)(TAPIINT32);


	ESFCreator		m_funcCreator;
	ESFDeleter		m_funcDeleter;
	ESFGetErrorDesc	m_funcGetErrorDesc;

	uint32_t		m_iRequestID;

	ProductMap		m_setContracts;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*	m_mapPosition;
	WTSArray*		m_ayTrades;
	WTSArray*		m_ayOrders;

	typedef WTSMap<std::string>	WTSStringMap;
	WTSStringMap*	m_mapFunds;

	typedef std::map<std::string, double>	WTSRateMap;
	WTSRateMap		m_mapRates;

	std::string		m_strBaseCurrency;

	IBaseDataMgr*	m_bdMgr;

	std::atomic<uint32_t> m_orderId;


	//交易所、品种映射
	typedef std::map<std::string,std::string>	StringMap;
	StringMap			m_mapExchgIToO;
	StringMap			m_mapExchgOToI;

	StringMap			m_mapProductIToO;
	StringMap			m_mapProductOToI;

	StringMap			m_mapCurrencyIToO;
	StringMap			m_mapCurrencyOToI;
};

