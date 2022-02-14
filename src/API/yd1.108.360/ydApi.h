#ifndef YD_API_H
#define YD_API_H

#include "ydDataStruct.h"

#if defined(_WIN32)
#ifdef LIB_YD_API_EXPORT
#define YD_API_EXPORT __declspec(dllexport)
#else
#define YD_API_EXPORT __declspec(dllimport)
#endif
#else
#define YD_API_EXPORT __attribute__ ((visibility ("default")))
#endif

class YDListener
{
public:
	virtual ~YDListener(void)
	{
	}
	virtual void notifyBeforeApiDestroy(void)
	{
	}
	virtual void notifyAfterApiDestroy(void)
	{
	}
	/// In the event of YD_AE_ServerRestarted, this process will exit after return of this method, as all data
	/// recevied may be not valid and data flow may be changed. If user don't want to stop this process, he can
	/// call startDestroy in this method. Then this process will not exit, but start process of self destroying.
	/// User can create new API afterwards to connect to new server.
	virtual void notifyEvent(int apiEvent)
	{
	}
	virtual void notifyReadyForLogin(bool hasLoginFailed)
	{
	}
	virtual void notifyLogin(int errorNo,int maxOrderRef,bool isMonitor)
	{
	}
	/// notifyFinishInit will be only called once in each instance of API.
	/// It is safe to call all api functions then, but all orders and trades haven't been received
	virtual void notifyFinishInit(void)
	{
	}
	/// notifyCaughtUp means api has received all information up to recent login
	/// It will be called once for each successful login
	/// A typical relationship between notifyFinishInit and notifyCaughtUp in a session with disconnection can be show below
	///		start api
	///		notifyFinishInit
	///		notifyCaughtUp
	///		...
	///		disconnected and reconnected
	///		notifyCaughtUp
	virtual void notifyCaughtUp(void)
	{
	}
	virtual void notifyTradingSegment(const YDExchange *pExchange,int segmentTime)
	{
	}
	virtual void notifyCombPosition(const YDCombPosition *pCombPosition,const YDCombPositionDef *pCombPositionDef,const YDAccount *pAccount)
	{
	}
	virtual void notifyOrder(const YDOrder *pOrder,const YDInstrument *pInstrument,const YDAccount *pAccount)
	{
	}
	virtual void notifyTrade(const YDTrade *pTrade,const YDInstrument *pInstrument,const YDAccount *pAccount)
	{
	}
	virtual void notifyFailedOrder(const YDInputOrder *pFailedOrder,const YDInstrument *pInstrument,const YDAccount *pAccount)
	{
	}
	virtual void notifyFailedCancelOrder(const YDFailedCancelOrder *pFailedCancelOrder,const YDExchange *pExchange,const YDAccount *pAccount)
	{
	}
	virtual void notifyQuote(const YDQuote *pQuote,const YDInstrument *pInstrument,const YDAccount *pAccount)
	{
	}
	virtual void notifyFailedQuote(const YDInputQuote *pFailedQuote,const YDInstrument *pInstrument,const YDAccount *pAccount)
	{
	}
	virtual void notifyFailedCancelQuote(const YDFailedCancelQuote *pFailedCancelQuote,const YDExchange *pExchange,const YDAccount *pAccount)
	{
	}
	virtual void notifyRequestForQuote(const YDRequestForQuote *pRequestForQuote,const YDInstrument *pInstrument)
	{
	}
	virtual void notifyCombPositionOrder(const YDOrder *pOrder,const YDCombPositionDef *pCombPositionDef,const YDAccount *pAccount)
	{
	}
	virtual void notifyFailedCombPositionOrder(const YDInputOrder *pFailedOrder,const YDCombPositionDef *pCombPositionDef,const YDAccount *pAccount)
	{
	}
	virtual void notifyOptionExecTogetherOrder(const YDOrder *pOrder,const YDInstrument *pInstrument,const YDInstrument *pInstrument2,const YDAccount *pAccount)
	{
	}
	virtual void notifyFailedOptionExecTogetherOrder(const YDInputOrder *pFailedOrder,const YDInstrument *pInstrument,const YDInstrument *pInstrument2, const YDAccount *pAccount)
	{
	}
	virtual void notifyMarketData(const YDMarketData *pMarketData)
	{
	}
	virtual void notifyAccount(const YDAccount *pAccount)
	{
	}
	virtual void notifyAccountExchangeInfo(const YDAccountExchangeInfo *pAccountExchangeInfo)
	{
	}
	virtual void notifyAccountProductInfo(const YDAccountProductInfo *pAccountProductInfo)
	{
	}
	virtual void notifyAccountInstrumentInfo(const YDAccountInstrumentInfo *pAccountInstrumentInfo)
	{
	}
	virtual void notifyIDFromExchange(const YDIDFromExchange *pIDFromExchange,const YDExchange *pExchange)
	{
	}
	virtual void notifyUpdateMarginRate(const YDUpdateMarginRate *pUpdateMarginRate)
	{
	}
	virtual void notifyChangePassword(int errorNo)
	{
	}
	virtual void notifyResponse(int errorNo,int requestType)
	{
	}
	// when using YDApi, this is suggested time for recalculating margin and position profit
	// when using YDExtendedApi, this is after each automatic recalculation
	virtual void notifyRecalcTime(void)
	{
	}
};

class YDApi
{
public:
	virtual ~YDApi(void)
	{
	}
	virtual bool start(YDListener *pListener)=0;
	///	startDestroy will start a process to destroy this api instance
	///	After calling this method, api will try to close all connections and  stop all threads belong to this api.
	///	In this period, all trading functions can not be used any more, and no trading notification will be received,
	/// but all functions of getting data can still be used. At the end of this period, notifyBeforeApiDestroy will 
	/// be sent to listener.
	/// Then start the second period to free all data structure of api. All functions of api cannot be used in
	/// this period, and all data pointers given by api are not valid any more. At the end of second period,
	/// notifyAfterApiDestroy will send to listener, indicating all resources of api are freed. User should not
	/// ever try to delete api, but the listener itself should be deleted by user after this.
	virtual void startDestroy(void)=0;
	virtual void disconnect(void)=0;
	virtual bool login(const char *username,const char *password,const char *appID,const char *authCode)=0;
	virtual bool insertOrder(YDInputOrder *pInputOrder,const YDInstrument *pInstrument,const YDAccount *pAccount=NULL)=0;
	virtual bool cancelOrder(YDCancelOrder *pCancelOrder,const YDExchange *pExchange,const YDAccount *pAccount=NULL)=0;
	virtual bool insertQuote(YDInputQuote *pInputQuote,const YDInstrument *pInstrument,const YDAccount *pAccount=NULL)=0;
	virtual bool cancelQuote(YDCancelQuote *pCancelQuote,const YDExchange *pExchange,const YDAccount *pAccount=NULL)=0;
	virtual bool insertCombPositionOrder(YDInputOrder *pInputOrder,const YDCombPositionDef *pCombPositionDef,const YDAccount *pAccount=NULL)=0;
	virtual bool insertOptionExecTogetherOrder(YDInputOrder *pInputOrder,const YDInstrument *pInstrument,const YDInstrument *pInstrument2,const YDAccount *pAccount)=0;
	/// send multiple insert orders in same time. inputOrders and instruments must have size of count, return true if all are successful
	/// maximum value of count is 16
	virtual bool insertMultiOrders(unsigned count,YDInputOrder inputOrders[],const YDInstrument *instruments[],const YDAccount *pAccount=NULL)=0;
	/// send multiple cancel orders in same time. cancelOrders and exchanges must have size of count, return true if all are successful
	/// maximum value of count is 16
	virtual bool cancelMultiOrders(unsigned count,YDCancelOrder cancelOrders[],const YDExchange *exchanges[],const YDAccount *pAccount=NULL)=0;
	/// send multiple insert quotes in same time. inputQuotes and instruments must have size of count, return true if all are successful
	/// maximum value of count is 16
	virtual bool insertMultiQuotes(unsigned count,YDInputQuote inputQuotes[],const YDInstrument *instruments[],const YDAccount *pAccount=NULL)=0;
	/// send multiple cancel quotes in same time. cancelQuotes and exchanges must have size of count, return true if all are successful
	/// maximum value of count is 16
	virtual bool cancelMultiQuotes(unsigned count,YDCancelQuote cancelQuotes[],const YDExchange *exchanges[],const YDAccount *pAccount=NULL)=0;
	virtual bool subscribe(const YDInstrument *pInstrument)=0;
	virtual bool unsubscribe(const YDInstrument *pInstrument)=0;
	/// setTradingRight, alterMoney and updateMarginRate can only be used by administor, trader should not call them
	virtual bool setTradingRight(const YDAccount *pAccount,const YDInstrument *pInstrument,const YDProduct *pProduct,const YDExchange *pExchange,int tradingRight)=0;
	virtual bool alterMoney(const YDAccount *pAccount,int alterMoneyType,double alterValue)=0;
	virtual bool updateMarginRate(const YDUpdateMarginRate *pUpdateMarginRate)=0;
	virtual bool changePassword(const char *username,const char *oldPassword,const char *newPassword)=0;
	/// tell server how to select connection if use YD_CS_Any, can only be used by administrator or accounts with YD_AF_SelectConnection flag
	/// connectionList is or of serial of 4 bits, lowest one has highest priority, must cover all connections of specified exchange
	virtual bool selectConnections(const YDExchange *pExchange,unsigned long long connectionList)=0;

	virtual bool hasFinishedInit(void)=0;

	virtual int getSystemParamCount(void)=0;
	virtual const YDSystemParam *getSystemParam(int pos)=0;
	virtual const YDSystemParam *getSystemParamByName(const char *name,const char *target)=0;

	virtual int getExchangeCount(void)=0;
	virtual const YDExchange *getExchange(int pos)=0;
	virtual const YDExchange *getExchangeByID(const char *exchangeID)=0;

	virtual int getProductCount(void)=0;
	virtual const YDProduct *getProduct(int pos)=0;
	virtual const YDProduct *getProductByID(const char *productID)=0;

	virtual int getInstrumentCount(void)=0;
	virtual const YDInstrument *getInstrument(int pos)=0;
	virtual const YDInstrument *getInstrumentByID(const char *instrumentID)=0;

	virtual int getCombPositionDefCount(void)=0;
	virtual const YDCombPositionDef *getCombPositionDef(int pos)=0;
	virtual const YDCombPositionDef *getCombPositionDefByID(const char *combPositionID,int combHedgeFlag)=0;

	/// following 3 functions for YDAccount can only be used by monitor
	virtual int getAccountCount(void)=0;
	virtual const YDAccount *getAccount(int pos)=0;
	virtual const YDAccount *getAccountByID(const char *accountID)=0;
	/// following function for YDAccount can only be used by trader
	virtual const YDAccount *getMyAccount(void)=0;

	virtual int getPrePositionCount(void)=0;
	virtual const YDPrePosition *getPrePosition(int pos)=0;

	/// it is not neccessary to use following 4 functions, as all these rates can be found in YDAccountInstrumentInfo
	virtual int getMarginRateCount(void)=0;
	virtual const YDMarginRate *getMarginRate(int pos)=0;
	virtual int getCommissionRateCount(void)=0;
	virtual const YDCommissionRate *getCommissionRate(int pos)=0;

	/// when trader call following 3 functions, pAccount should be NULL
	virtual const YDAccountExchangeInfo *getAccountExchangeInfo(const YDExchange *pExchange,const YDAccount *pAccount=NULL)=0;
	virtual const YDAccountProductInfo *getAccountProductInfo(const YDProduct *pProduct,const YDAccount *pAccount=NULL)=0;
	virtual const YDAccountInstrumentInfo *getAccountInstrumentInfo(const YDInstrument *pInstrument,const YDAccount *pAccount=NULL)=0;

	virtual int getGeneralRiskParamCount(void)=0;
	virtual const YDGeneralRiskParam *getGeneralRiskParam(int pos)=0;

	virtual void writeLog(const char *format,...)=0;
	virtual const char *getVersion(void)=0;

	enum YDPacketType
	{
		YD_CLIENT_PACKET_INSERT_ORDER, 
		YD_CLIENT_PACKET_CANCEL_ORDER
	};

	virtual int getClientPacketHeader(YDPacketType type,unsigned char *pHeader,int len)=0;

	/// can get trading day after login successfully
	virtual int getTradingDay(void)=0;
};

/// If using YDExtendedApi, pUser pointers in all classes are reserved to be used by this API
class YDExtendedListener
{
public:
	virtual ~YDExtendedListener(void)
	{
	}
	// all address of parameters in following methods are fixed
	virtual void notifyExtendedOrder(const YDExtendedOrder *pOrder)
	{
	}
	virtual void notifyExtendedTrade(const YDExtendedTrade *pTrade)
	{
	}
	virtual void notifyExtendedQuote(const YDExtendedQuote *pQuote)
	{
	}
	virtual void notifyExtendedPosition(const YDExtendedPosition *pPosition)
	{
	}
	virtual void notifyExtendedAccount(const YDExtendedAccount *pAccount)
	{
	}
	// notifyExchangeCombPositionDetail will only be used when trading SSE/SZSE
	virtual void notifyExchangeCombPositionDetail(const YDExtendedCombPositionDetail *pCombPositionDetail)
	{
	}
};

class YDExtendedApi: public YDApi
{
public:
	// startExtended method can be used when notification of extended message are required.
	// There is no other difference between start and startExtended.
	// User can also use start method if extended message are not required
	virtual bool startExtended(YDListener *pListener,YDExtendedListener *pExtendedListener)=0;
	/*
		insertOrder will not do local validation before sending to server.
			YDExtendedApi will not froze position and money until notification of these orders are received
			This may lead to a temporary inaccuracy.
		checkAndInsertOrder will do local validation before sending to server
			Position and money will be frozen if validation and sending is successful
			This will ensure all data are accurate at any moment.
		After first order notification, there is no difference between these two methods
		User can mix-use these two methods, though it is not recommended.
		checkAndInsertOrder will set OrderRef to next unused value automatically, and user should get it from pInputOrder after calling
		Both insertOrder and checkAndInsertOrder will set ErrorNo if error occurs
	*/
	virtual bool checkAndInsertOrder(YDInputOrder *pInputOrder,const YDInstrument *pInstrument,const YDAccount *pAccount=NULL)=0;
	///	checkOrder will only check whether pInputOrder is OK, and will not send to server
	/// ErrorNo will be set if check failed
	virtual bool checkOrder(YDInputOrder *pInputOrder,const YDInstrument *pInstrument,const YDAccount *pAccount=NULL)=0;
	/// send multiple insert orders in same time. inputOrders and instruments must have size of count
	/// orders will only be sent if validation of all orders are successful, return true if all are successful
	/// maximum value of count is 16
	virtual bool checkAndInsertMultiOrders(unsigned count,YDInputOrder inputOrders[],const YDInstrument *instruments[],const YDAccount *pAccount=NULL)=0;

	virtual bool checkAndInsertCombPositionOrder(YDInputOrder *pInputOrder,const YDCombPositionDef *pCombPositionDef,const YDAccount *pAccount=NULL)=0;
	virtual bool checkCombPositionOrder(YDInputOrder *pInputOrder,const YDCombPositionDef *pCombPositionDef,const YDAccount *pAccount=NULL)=0;

	virtual bool checkAndInsertQuote(YDInputQuote *pInputQuote,const YDInstrument *pInstrument,const YDAccount *pAccount=NULL)=0;
	virtual bool checkQuote(YDInputQuote *pInputQuote,const YDInstrument *pInstrument,const YDAccount *pAccount=NULL)=0;

	virtual const YDExtendedAccount *getExtendedAccount(const YDAccount *pAccount=NULL)=0;

	virtual const YDExtendedPosition *getExtendedPosition(int positionDate,int positionDirection,int hedgeFlag,
		const YDInstrument *pInstrument,const YDAccount *pAccount=NULL,bool create=false)=0;
	/// positions must have spaces of count, return real number of positions(may be greater than count). Only partial will be set if no enough space
	virtual unsigned findExtendedPositions(const YDExtendedPositionFilter *pFilter,unsigned count,const YDExtendedPosition *positions[])=0;

	/// recalculate margin and position profit. if "RecalcMode" is "auto", there is no need to call this method
	virtual void recalcMarginAndPositionProfit(void)=0;

	virtual const YDMarginRate *getInstrumentMarginRate(const YDInstrument *pInstrument,int hedgeFlag,const YDAccount *pAccount=NULL)=0;
	virtual const YDCommissionRate *getInstrumentCommissionRate(const YDInstrument *pInstrument,int hedgeFlag,const YDAccount *pAccount=NULL)=0;

	/// getOrder by orderRef can only be used for orders using checkAndInsertOrder
	virtual const YDExtendedOrder *getOrder(int orderRef,const YDAccount *pAccount)=0;
	/// getOrder by orderSysID can only be used for orders have been accepted by exchange
	virtual const YDExtendedOrder *getOrder(int orderSysID,const YDExchange *pExchange,int YDOrderFlag=YD_YOF_Normal)=0;
	/// orders must have spaces of count, return real number of orders(may be greater than count). Only partial will be set if no enough space
	/// Only orders accepted by exchange can be found in this function
	virtual unsigned findOrders(const YDOrderFilter *pFilter,unsigned count,const YDExtendedOrder *orders[])=0;

	/// getQuote by orderRef can only be used for quotes using checkAndInsertQuote
	virtual const YDExtendedQuote *getQuote(int orderRef,const YDAccount *pAccount)=0;
	/// getQuote by quoteSysID can only be used for quotes have been accepted by exchange
	virtual const YDExtendedQuote *getQuote(int quoteSysID,const YDExchange *pExchange)=0;
	/// quotes must have spaces of count, return real number of quotes(may be greater than count). Only partial will be set if no enough space
	/// Only quotes accepted by exchange can be found in this function
	virtual unsigned findQuotes(const YDQuoteFilter *pFilter,unsigned count,const YDExtendedQuote *quotes[])=0;

	/// trades must have spaces of count, return real number of trades(may be greater than count). Only partial will be set if no enough space
	virtual unsigned findTrades(const YDTradeFilter *pFilter,unsigned count,const YDExtendedTrade *trades[])=0;

	/// getCombPositionDetail can only be used in SSE/SZSE
	virtual const YDExtendedCombPositionDetail *getCombPositionDetail(int combPositionDetailID)=0;
	/// combPositionDetails must have spaces of count, return real number of combPositionDetails(may be greater than count). Only partial will be set if no enough space
	virtual unsigned findCombPositionDetails(const YDCombPositionDetailFilter *pFilter,unsigned count,const YDExtendedCombPositionDetail *combPositionDetails[])=0;

	/// get original ID from exchange based on ID in yd system, return NULL if there is no real difference betewwn these 2 IDs
	virtual const char *getIDFromExchange(const YDExchange *pExchange,int idType,int idInSystem)=0;

	/// findExtendedPositions, findOrders, findQuotes and findTrades are slow

	virtual double getOptionsShortMarginPerLot(const YDInstrument *pInstrument,int hedgeFlag,bool includePremium,const YDAccount *pAccount=NULL)=0;
	virtual double getCombPositionMarginPerLot(const YDCombPositionDef *pCombPositionDef,const YDAccount *pAccount=NULL)=0;
	/// anyDirection can use Direction or PositionDirection
	virtual double getMarginPerLot(const YDInstrument *pInstrument,int hedgeFlag,int anyDirection,double openPrice,const YDAccount *pAccount=NULL)=0;
	virtual double getMarginPerLot(const YDExtendedPosition *pPosition,double openPrice)=0;
	virtual double getCombPositionMarginSaved(const YDExtendedCombPositionDetail *pCombPositionDetail,double legMargins[])=0;

	/// may return NULL if not exist
	virtual const YDExtendedRequestForQuote *getRequestForQuote(const YDInstrument *pInstrument)=0;

	/// Search for an opportunity to create a new DCE comb position. "combTypes" is a list of comb position types to be processed.
	/// "combTypes" should be ended with -1 and sorted by priotity. If NULL is given, all types are processed
	/// If an opportunity was found and the order was sent successfully, pointer to the comb position definition is returned. Otherwise NULL is returned
	virtual const YDCombPositionDef* autoCreateCombPosition(const int *combTypes)=0;

};

extern "C"
{
YD_API_EXPORT YDApi *makeYDApi(const char *configFilename);
YD_API_EXPORT YDExtendedApi *makeYDExtendedApi(const char *configFilename);
/// Same as getVersion inside YDApi, put here to get version without make api
YD_API_EXPORT const char *getYDVersion(void);
// Returns nanoseconds elapsed since current process starts up
YD_API_EXPORT unsigned long long getYDNanoTimestamp();
}

#endif
