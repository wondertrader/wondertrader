#ifndef YD_DATA_STRUCT_H
#define YD_DATA_STRUCT_H

#include "ydDataType.h"

/// Data structs for YDApi

class YDSystemParam
{
public:
	YDString Name;
	YDString Target;
	YDString Value;
};

class YDExchangeTradeConstrain
{
public:
	int ExchangeOrderSpeed;
	int ExchangeOrderPeriod;
};

class YDExchange
{
public:
	YDExchangeID ExchangeID;            // Short string identify the exchange, e.g. "CFFEX", "DCE", "SHFE", ...
	int ExchangeRef;                    // An integer uniquely identify the exchange
	int ConnectionCount;                // Number of connections(seats) to the exchange
	int ProductRefStart;
	int ProductRefEnd;
	bool UseTodayPosition;              // Whether today and previous positions are traded separately. True for SHFE and INE
	bool UseArbitragePosition;          // Besides speculation and hedge, whether the exchange maintains separate position for arbitration. True for CFFEX 
	bool CloseTodayFirst;               // When "UseTodayPosition" is false, whether today opened positions are closed first. The commission may be different. True for CFFEX
	bool SingleSideMargin;              // Whether margin is calculated using larger side of the product or product group. True for SHFE, INE, and CFFEX
	int OptionExecutionSupport;			// 0 for not support, 1 for support without risk control, 2 for support with risk constrol
	int OptionAbandonExecutionSupport;	// 0 for not support, 1 for support without risk control, 2 for support with risk constrol
	int QuoteVolumeRestriction;			// 0 for allow single side, 1 for allow different volume, 2 for require same volume
	short FutureMarginType;             // 0(default) buy and sell side margin are calculatd separately; 1 for max(buy, sell) side margin (ZCE);
	bool CombineHedgeSpecPosition;      // When true, hedge position are combined into speculation position(CZCE)
	bool TradeStockOptions;	            // whether this exchange is trading stock/etf options(SSE/SZSE)
	YDExchangeTradeConstrain TradeConstrains[YD_MaxHedgeFlag];
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDTradeConstraint
{
public:
	int OpenLimit;
	int CancelLimit;
	int PositionLimit;
	int TradeVolumeLimit;
	int DirectionOpenLimit[2];
	int DirectionPositionLimit[2];
};

class YDProduct
{
public:
	YDProductID ProductID;              // Short string identify the product, e.g. "cu", "IC", "cu_o", ...
	int ProductRef;
	int ExchangeRef;
	int ProductClass;                   // Refer to "Product Class" section of ydDataType.h
	int Multiple;                       // Contract size
	double Tick;                        // Minimum amount for price change
	double UnderlyingMultiply;          // Volume of corresponding underlying instrument. Used for options, normally equals to 1
	int MaxMarketOrderVolume;
	int MinMarketOrderVolume;
	int MaxLimitOrderVolume;
	int MinLimitOrderVolume;
	int InstrumentRefStart;
	int InstrumentRefEnd;
	int SystemUse7[32];
	const YDProduct *m_pMarginProduct;  // Points to the product that represents the product group, when single side margin is used
	const YDExchange *m_pExchange;
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDMarketData;
class YDMarginRate;

class YDInstrument
{
public:
	YDInstrumentID InstrumentID;        // Short string identify the instrument, e.g. "cu2101", "IC2212", "c2103-C-2620", ...
	int InstrumentRef;                  // An integer uniquely identify the instrument
	int ProductRef;
	int ExchangeRef;
	int ProductClass;                   // Refer to "Product Class" section of ydDataType.h
	int DeliveryYear;
	int DeliveryMonth;
	int MaxMarketOrderVolume;
	int MinMarketOrderVolume;
	int MaxLimitOrderVolume;
	int MinLimitOrderVolume;
	double Tick;                        // Minimum amount for price change
	int Multiple;                       // Contract size
	int ExpireDate;
	double StrikePrice;
	int UnderlyingInstrumentRef;
	int OptionsType;                    // Refer to "Options Type" section of ydDataType.h
	double UnderlyingMultiply;          // Volume of corresponding underlying instrument. Used for options, normally equals to 1
	int SystemUse7[32];
	bool SingleSideMargin;              // Whether margin is calculated using larger side of the product or product group. True for SHFE, INE, and CFFEX
	char SystemUse6;
	short ExpireTradingDayCount;
	int MarginCalcMethod;               // Option margin calculation method, 1 for CFFEX, 2 for SSE & SZSE, 0 for others 
	YDInstrumentID InstrumentHint;        // Hint of this instrument, useful for options of SSE/SZSE, e.g. "510050C2009M02350"
	const YDInstrument *m_pUnderlyingInstrument;  // Points to underlying instrument, only valid for options
	const YDExchange *m_pExchange;
	const YDProduct *m_pProduct;
	const YDMarketData *m_pMarketData;
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
	bool AutoSubscribed;
	bool UserSubscribed;
	/// following fields are only valid when using ydExtendedApi
	bool IsStaticMargin;
	int MarginBasePriceType;
	int OrderPremiumBasePriceType[2];
	int OrderMarginBasePriceType;
	int MarginBasePriceAsUnderlyingType;
	double MarginBasePrice;
	const YDMarginRate *m_pExchangeMarginRate[YD_MaxHedgeFlag];
};

class YDMarketData
{
public:
	int InstrumentRef;
	int TradingDay;
	double PreSettlementPrice;
	double PreClosePrice;
	double PreOpenInterest;
	double UpperLimitPrice;
	double LowerLimitPrice;
	double LastPrice;
	double BidPrice;
	double AskPrice;
	int BidVolume;
	int AskVolume;
	double Turnover;
	double OpenInterest;
	int Volume;
	int TimeStamp;
	double AveragePrice;
	const YDInstrument *m_pInstrument;
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDCombPositionDef
{
public:
	YDInstrumentID CombPositionID;
	int CombPositionRef;
	int ExchangeRef;
	int Priority;
	short CombHedgeFlag;
	short CombPositionType;
	const YDExchange *m_pExchange;
	const YDInstrument *m_pInstrument[2];
	int PositionDirection[2];
	int HedgeFlag[2];
	int PositionDate[2];
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDAccount
{
public:
	int SystemUse1;
	int AccountRef;
	YDAccountID AccountID;              // User account name in broker
	double PreBalance;
	double WarningLevel1;
	double WarningLevel2;
	double MaxMoneyUsage;               // Rate of fund that can be used for trading, should be less than 1
	double Deposit;
	double Withdraw;
	double FrozenWithdraw;
	int TradingRight;
	int MaxOrderCount;                  // Maximum number of orders that can be placed, read only
	int MaxLoginCount;                  // Maximum number of current login allowed, read only
	int LoginCount;                     // Number of active logins, read only
	int AccountFlag;                    // Bit map, refer to "Account Flag" section of ydDataType.h
	int SystemUse3;
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDPrePosition
{
public:
	int SystemUse1;
	int AccountRef;
	int InstrumentRef;
	int PositionDirection;
	int HedgeFlag;
	int PrePosition;
	double PreSettlementPrice;
	const YDInstrument *m_pInstrument;
	const YDAccount *m_pAccount;
};

class YDCombPosition
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int AccountRef;
	int CombPositionRef;
	int Position;
	int CombPositionDetailID;
	int SystemUse6;
};

class YDInputOrder
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int SystemUse4;
	int SystemUse5;
	char Direction;                     // Refer to "Direction" section of ydDataType.h
	char OffsetFlag;                    // Refer to "Offset Flag" section of ydDataType.h
	char HedgeFlag;                     // Refer to "Hedge Flag" section of ydDataType.h
	char ConnectionSelectionType;       // Refer to "Connection Selection Type" section of ydDataType.h
	union
	{
		double Price;                   // Price for limit orders
		struct
		{
			int CombPositionDetailID;   // Set only when YDOrderFlag is YD_YOF_CombPosition, and Direction is YD_D_Split for SSE or SZSE
			int SystemUse6;
		};
	};
	int OrderVolume;
	int OrderRef;                       // User defined reference of the order. Will be passed back in later order and trade notifications 
	char OrderType;                     // Refer to "Order Type" section of ydDataType.h
	char YDOrderFlag;                   // Refer to "YD OrderFlag" section of ydDataType.h
	char ConnectionID;                  // Target connection ID if ConnectionSelectionType is not YD_CS_Any
	char RealConnectionID;              // Meaningful only in order notifications
	int ErrorNo;                        // Set by ydAPI, refer to "ydError.h"
};

class YDOrder: public YDInputOrder
{
public:
	int ExchangeRef;
	YDSysOrderID OrderSysID;
	int OrderStatus;                    // Refer to "Order Status" section of ydDataType.h
	int TradeVolume;
	int InsertTime;
	YDLocalOrderID OrderLocalID;
};

class YDCancelOrder
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int SystemUse4;
	YDSysOrderID OrderSysID;
	char SystemUse5;
	char ConnectionSelectionType;       // Refer to "Connection Selection Type" section of ydDataType.h
	char ConnectionID;                  // Target connection ID if ConnectionSelectionType is not YD_CS_Any
	char YDOrderFlag;                   // Refer to "YD OrderFlag" section of ydDataType.h
};

class YDFailedCancelOrder
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int AccountRef;
	YDSysOrderID OrderSysID;
	char ExchangeRef;
	char SystemUse6;
	char SystemUse7;
	char YDOrderFlag;                   // Refer to "YD OrderFlag" section of ydDataType.h
	int ErrorNo;                        // Set by ydAPI, refer to "ydError.h"
	int SystemUse8;
};

class YDTrade
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int AccountRef;
	int InstrumentRef;
	char Direction;                     // Refer to "Direction" section of ydDataType.h
	char OffsetFlag;                    // Refer to "Offset Flag" section of ydDataType.h
	char HedgeFlag;                     // Refer to "Hedge Flag" section of ydDataType.h
	char SystemUse6;
	YDTradeID TradeID;
	YDSysOrderID OrderSysID;
	double Price;
	int Volume;
	int TradeTime;
	double Commission;
	YDLocalOrderID OrderLocalID;
	int OrderRef;                       // User defined reference of the order. Will be passed back in later order and trade notifications 
};

class YDInputQuote
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int SystemUse4;
	int SystemUse5;
	char BidOffsetFlag;                 // Refer to "Offset Flag" section of ydDataType.h
	char BidHedgeFlag;                  // Refer to "Offset Flag" section of ydDataType.h
	char AskOffsetFlag;                 // Refer to "Offset Flag" section of ydDataType.h
	char AskHedgeFlag;                  // Refer to "Offset Flag" section of ydDataType.h
	double BidPrice;
	double AskPrice;
	int BidVolume;
	int AskVolume;
	int OrderRef;                       // User defined reference of the order. Will be passed back in later order and trade notifications 
	char ConnectionSelectionType;       // Refer to "Connection Selection Type" section of ydDataType.h
	char ConnectionID;                  // Target connection ID if ConnectionSelectionType is not YD_CS_Any
	char RealConnectionID;              // Meaningful only in quote notifications
	char YDQuoteFlag;					// Refer to "YD QuoteFlag" section of ydDataType.h, 
	int Reserved;
	int ErrorNo;                        // Set by ydAPI, refer to "ydError.h"
};

class YDQuote: public YDInputQuote
{
public:
	int ExchangeRef;
	YDSysOrderID QuoteSysID;
	YDSysOrderID BidOrderSysID;
	YDSysOrderID AskOrderSysID;
	YDRFQID RequestForQuoteID;
	int SystemUse1;
};

class YDCancelQuote
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int SystemUse4;
	YDSysOrderID QuoteSysID;
	char SystemUse5;
	char ConnectionSelectionType;       // Refer to "Connection Selection Type" section of ydDataType.h
	char ConnectionID;                  // Target connection ID if ConnectionSelectionType is not YD_CS_Any
	char SystemUse6;
};

class YDFailedCancelQuote
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int AccountRef;
	YDSysOrderID QuoteSysID;
	char ExchangeRef;
	char SystemUse6;
	char SystemUse7;
	char SystemUse8;
	int ErrorNo;                        // Set by ydAPI, refer to "ydError.h"
	int SystemUse9;
};

class YDRequestForQuote
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int SystemUse4;
	int InstrumentRef;
	int RequestTime;
	YDRFQID RequestForQuoteID;
	int SystemUse6;
};

class YDMarginRate
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int SystemUse4;
	int HedgeFlag;                     // Refer to "Hedge Flag" section of ydDataType.h
	int SystemUse5;
	union 
	{
		double LongMarginRatioByMoney;
		double PutMarginRatioByMoney;
	};
	union
	{
		double LongMarginRatioByVolume;
		double PutMarginRatioByVolume;
		double BaseMarginRate;
	};
	union
	{
		double ShortMarginRatioByMoney;
		double CallMarginRatioByMoney;
		double LinearFactor;
	};
	union
	{
		double ShortMarginRatioByVolume;
		double CallMarginRatioByVolume;
		double LowerBoundaryCoef;
	};

	const YDInstrument *m_pInstrument;
	const YDProduct *m_pProduct;
	const YDAccount *m_pAccount;
};

class YDCommissionRate
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int SystemUse4;
	int HedgeFlag;                     // Refer to "Hedge Flag" section of ydDataType.h
	int SystemUse5;
	double OpenRatioByMoney;
	double OpenRatioByVolume;
	double CloseRatioByMoney;
	double CloseRatioByVolume;
	double CloseTodayRatioByMoney;
	double CloseTodayRatioByVolume;
	double OrderCommByVolume;
	double OrderActionCommByVolume;
	const YDInstrument *m_pInstrument;
	const YDProduct *m_pProduct;
	const YDAccount *m_pAccount;
};

class YDIDFromExchange
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int AccountRef;
	int ExchangeRef;
	int IDType;							// Refer to "IDType in IDFromExchange" section of ydDataType.h
	int IDInSystem;
	int SystemUse6;
	char IDFromExchange[24];
};

class YDUpdateMarginRate
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int AccountRef;
	int ProductRef;
	int InstrumentRef;
	int UnderlyingInstrumentRef;
	int HedgeFlagSet;
	int ExpireDate;
	int Multiple;
	int OptionTypeSet;
	int SystemUse4;
	union
	{
		double LongMarginRatioByMoney;
		double PutMarginRatioByMoney;
	};
	union
	{
		double LongMarginRatioByVolume;
		double PutMarginRatioByVolume;
		double BaseMarginRate;
	};
	union
	{
		double ShortMarginRatioByMoney;
		double CallMarginRatioByMoney;
		double LinearFactor;
	};
	union
	{
		double ShortMarginRatioByVolume;
		double CallMarginRatioByVolume;
		double LowerBoundaryCoef;
	};
};

class YDAccountExchangeInfo
{
public:
	int SystemUse1;
	int AccountRef;
	int ExchangeRef;
	int TradingRight;                   // Refer to "Trade Right" section of ydDataType.h
	const YDAccount *m_pAccount;
	const YDExchange *m_pExchange;
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDAccountProductInfo
{
public:
	int SystemUse1;
	int AccountRef;
	int ProductRef;
	int TradingRight;                   // Refer to "Trade Right" section of ydDataType.h
	YDTradeConstraint TradingConstraints[YD_MaxHedgeFlag];
	const YDAccount *m_pAccount;
	const YDProduct *m_pProduct;
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDAccountInstrumentInfo
{
public:
	int SystemUse1;
	int AccountRef;
	int InstrumentRef;
	int TradingRight;
	YDTradeConstraint TradingConstraints[YD_MaxHedgeFlag];
	const YDAccount *m_pAccount;
	const YDInstrument *m_pInstrument;
	const YDMarginRate *m_pMarginRate[YD_MaxHedgeFlag];
	const YDCommissionRate *m_pCommissionRate[YD_MaxHedgeFlag];
	mutable void *pUser;
	mutable double UserFloat;
	mutable int UserInt1;
	mutable int UserInt2;
};

class YDGeneralRiskParam
{
public:
	int SystemUse1;
	int SystemUse2;
	int SystemUse3;
	int AccountRef;
	int GeneralRiskParamType;
	int ExtendedRef;
	double FloatValue;
	int IntValue1;
	int IntValue2;
};

/// Data structs for YDExtendedApi

class YDExtendedOrder: public YDOrder
{
public:
	union
	{
		const YDInstrument *m_pInstrument;
		const YDCombPositionDef *m_pCombPositionDef;
	};
	const YDAccount *m_pAccount;
	const YDInstrument *m_pInstrument2;
};

class YDExtendedTrade: public YDTrade
{
public:
	const YDInstrument *m_pInstrument;
	const YDAccount *m_pAccount;
};

class YDExtendedQuote: public YDQuote
{
public:
	bool BidOrderFinished,AskOrderFinished;
	const YDInstrument *m_pInstrument;
	const YDAccount *m_pAccount;
};

class YDExtendedRequestForQuote: public YDRequestForQuote
{
public:
	const YDInstrument *m_pInstrument;
};

class YDExtendedAccount
{
public:
	double CloseProfit;
	double CashIn;
	double OtherCloseProfit;
	double Commission;
	double Margin;
	double PositionProfit;
	double Balance;
	double Available;
	unsigned UsedOrderCount;
	int Useless;
	const YDAccount *m_pAccount;
	double ExecMargin;
	double OptionLongPositionCost;
	double OptionLongPositionCostLimit;

	double useable(void) const
	{
		if (PositionProfit>0)
		{
			return Available;
		}
		else
		{
			return Available+PositionProfit;
		}
	}
	bool canUse(double value) const
	{
		if (PositionProfit>0)
		{
			return Available>value;
		}
		else
		{
			return Available+PositionProfit>value;
		}
	}
};


class YDExtendedPosition
{
public:
	class CPositionDetail
	{
	public:
		CPositionDetail *m_pNext;
		double Price;
		int Volume;
		// TradeID<0 indicates history position
		YDTradeID TradeID;
	};
	
	int PositionDate;
	int PositionDirection;
	int HedgeFlag;
	int OpenFrozen;
	int Position;
	int PositionByOrder;
	// Yesterday position used for commission calculation only, based on each exchange's commission rules.
	int YDPositionForCommission;
	int CloseFrozen;
	int PossibleOpenVolume;
	int ExecFrozen;
	double TotalOpenPrice;
	double Margin;
	double PositionProfit;
	// MarginPerlot is margin when price is preSettlementPrice
	double MarginPerLot;
	double CloseProfit;
	double OtherCloseProfit;
	const YDAccountInstrumentInfo *m_pAccountInstrumentInfo;
	CPositionDetail *PositionDetailList;
	void *Padding1;
	int TotalCombPositions;
	int Padding2;
	unsigned CombPositionCount;

	const YDAccount *getAccount(void) const
	{
		return m_pAccountInstrumentInfo->m_pAccount;
	}
	const YDInstrument *getInstrument(void) const
	{
		return m_pAccountInstrumentInfo->m_pInstrument;
	}
	const YDMarginRate *getMarginRate(void) const
	{
		return m_pAccountInstrumentInfo->m_pMarginRate[HedgeFlag-1];
	}
	const YDCommissionRate *getCommissionRate(void) const
	{
		return m_pAccountInstrumentInfo->m_pCommissionRate[HedgeFlag-1];
	}

	//DEPRECATED: This function is deprecated and will be remove in future release. Use getOpenPrice() instead
	double OpenPrice(void) const
	{
		return getOpenPrice();
	}

	double getOpenPrice(void) const
	{
		if (Position>0)
		{
			return TotalOpenPrice/Position;
		}
		else
		{
			return 0;
		}
	}

	// Return yesterday position calculated according to "first open, first close" rule
	int getYDPosition() const
	{
		int position=0;
		CPositionDetail *p=PositionDetailList;
		while (p && (p->TradeID<0))
		{
			position+=p->Volume;
			p = p->m_pNext;
		}
		return position;
	}
};

class YDExtendedCombPositionDetail
{
public:
	const YDAccount *m_pAccount;
	const YDCombPositionDef *m_pCombPositionDef;
	int Position;
	int CombPositionDetailID;
	//YDExtendedPosition *m_pPositions[2];
};

class YDExtendedPositionFilter
{
public:
	/// -1 in following 3 fields indicates any
	int PositionDate;
	int PositionDirection;
	int HedgeFlag;

	/// NULL in following 4 fields indicates any
	const YDInstrument *pInstrument;
	const YDProduct *pProduct;
	const YDExchange *pExchange;
	const YDAccount *pAccount;
};

class YDOrderFilter
{
public:
	/// -1 in following 2 fields indicates any
	int StartTime;
	int EndTime;

	/// or of several (1<<YDOrderFlag)
	int YDOrderFlags;

	/// NULL in following 5 fields indicates any
	const YDCombPositionDef* pCombPositionDef;
	const YDInstrument *pInstrument;
	const YDProduct *pProduct;
	const YDExchange *pExchange;
	const YDAccount *pAccount;
};

class YDQuoteFilter
{
public:
	/// NULL in following 4 fields indicates any
	const YDInstrument *pInstrument;
	const YDProduct *pProduct;
	const YDExchange *pExchange;
	const YDAccount *pAccount;
};

class YDTradeFilter
{
public:
	/// -1 in following 2 fields indicates any
	int StartTime;
	int EndTime;

	/// NULL in following 4 fields indicates any
	const YDInstrument *pInstrument;
	const YDProduct *pProduct;
	const YDExchange *pExchange;
	const YDAccount *pAccount;
};

class YDCombPositionDetailFilter
{
public:
	/// NULL or 0 in following 4 fields indicates any
	const YDCombPositionDef *pCombPositionDef;
	const YDAccount *pAccount;
	/// if following 2 fields are specified, it should match one leg of this combination position
	const YDInstrument *pInstrument;
	int PositionDirection;
	/// if IncludeSplit is false, only current valid combPositions are included
	bool IncludeSplit;
};

#endif
