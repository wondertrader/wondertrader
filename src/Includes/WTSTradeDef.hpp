/*!
 * \file WTSTradeDef.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt交易数据对象定义,包括委托、订单、成交、持仓、资金、持仓明细等数据
 */
#pragma once
#include "WTSObject.hpp"
#include "WTSTypes.h"
#include "WTSMarcos.h"

#include <time.h>
#include <string>
#include <map>
#include<chrono>

NS_WTP_BEGIN
class WTSContractInfo;

typedef struct _WTSEntrustStruct
{
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];
	double			m_dVolume;
	double			m_iPrice;

	bool			m_bIsNet;
	bool			m_bIsBuy;

	WTSDirectionType	m_direction;
	WTSPriceType		m_priceType;
	WTSOrderFlag		m_orderFlag;
	WTSOffsetType		m_offsetType;

	char				m_strEntrustID[64] = { 0 };
	char				m_strUserTag[64] = { 0 };

	WTSBusinessType		m_businessType;

	_WTSEntrustStruct()
		: m_iPrice(0)
		, m_dVolume(0)
		, m_businessType(BT_CASH)
		, m_direction(WDT_LONG)
		, m_priceType(WPT_ANYPRICE)
		, m_orderFlag(WOF_NOR)
		, m_offsetType(WOT_OPEN)
		, m_bIsNet(false)
		, m_bIsBuy(true){}

} WTSEntrustStruct;

//////////////////////////////////////////////////////////////////////////
//委托数据结构,用户客户端向服务端发起
class WTSEntrust : public WTSEntrustStruct, public WTSPoolObject<WTSEntrust>
{
public:
	WTSEntrust()
		: m_pContract(NULL)
	{
	}

	virtual ~WTSEntrust(){}

public:
	static inline WTSEntrust* create(const char* code, double vol, double price, const char* exchg = "", WTSBusinessType bType = BT_CASH) noexcept
	{
		WTSEntrust* pRet = WTSEntrust::allocate();
		if(pRet)
		{
			wt_strcpy(pRet->m_strExchg, exchg);
			wt_strcpy(pRet->m_strCode, code);

			pRet->m_dVolume = vol;
			pRet->m_iPrice = price;
			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

public:
	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept
	{
		wt_strcpy(m_strExchg, exchg, len);
    }

	inline void setCode(const char* code, std::size_t len = 0) noexcept
	{
		wt_strcpy(m_strCode, code, len);
    }

	constexpr inline void setDirection(WTSDirectionType dType)noexcept {m_direction = dType;}
	constexpr inline void setPriceType(WTSPriceType pType)noexcept {m_priceType = pType;}
	constexpr inline void setOrderFlag(WTSOrderFlag oFlag)noexcept {m_orderFlag = oFlag;}
	constexpr inline void setOffsetType(WTSOffsetType oType)noexcept {m_offsetType = oType;}

	constexpr inline WTSDirectionType	getDirection() const noexcept {return m_direction;}
	constexpr inline WTSPriceType		getPriceType() const noexcept {return m_priceType;}
	constexpr inline WTSOrderFlag		getOrderFlag() const noexcept {return m_orderFlag;}
	constexpr inline WTSOffsetType	getOffsetType() const noexcept {return m_offsetType;}

	constexpr inline void setBusinessType(WTSBusinessType bType) noexcept { m_businessType = bType; }
	constexpr inline WTSBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	constexpr inline void setVolume(double volume) noexcept { m_dVolume = volume; }
	constexpr inline void setPrice(double price) noexcept { m_iPrice = price; }

	constexpr inline double getVolume() const noexcept { return m_dVolume; }
	constexpr inline double getPrice() const noexcept { return m_iPrice; }

	constexpr inline const char* getCode() const noexcept { return m_strCode; }
	constexpr inline const char* getExchg() const  noexcept { return m_strExchg; }

	inline void setEntrustID(const char* eid) noexcept { wt_strcpy(m_strEntrustID, eid); }
	constexpr inline const char* getEntrustID() const  noexcept { return m_strEntrustID; }
	constexpr inline char* getEntrustID() noexcept { return m_strEntrustID; }

	inline void setUserTag(const char* tag)  noexcept { wt_strcpy(m_strUserTag, tag); }
	constexpr inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	constexpr inline char* getUserTag()  noexcept { return m_strUserTag; }

	constexpr inline void setNetDirection(bool isBuy) noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	constexpr inline bool isNet() const  noexcept { return m_bIsNet; }
	constexpr inline bool isBuy() const  noexcept { return m_bIsBuy; }

	constexpr inline void setContractInfo(WTSContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	constexpr inline WTSContractInfo* getContractInfo() const  noexcept { return m_pContract; }

protected:
	WTSContractInfo*	m_pContract;
};

typedef struct _WTSActionStruct
{
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];

	char			m_strEnturstID[64] = { 0 };
	WTSActionFlag	m_actionFlag;

	char			m_strOrderID[64] = { 0 };
	char			m_strUserTag[64] = { 0 };

	WTSBusinessType		m_businessType;

	_WTSActionStruct()
		: m_actionFlag(WAF_CANCEL)
		, m_businessType(BT_CASH)
	{}
} WTSActionStruct;

//////////////////////////////////////////////////////////////////////////
//委托操作: 撤单、改单
class WTSEntrustAction : public WTSActionStruct, public WTSPoolObject<WTSEntrustAction>
{
public:
	virtual ~WTSEntrustAction(){}

public:
	static inline WTSEntrustAction* create(const char* code, const char* exchg = "", WTSBusinessType bType = BT_CASH) noexcept
	{
		WTSEntrustAction* pRet = WTSEntrustAction::allocate();
		if(pRet)
		{
			wt_strcpy(pRet->m_strExchg, exchg);
			wt_strcpy(pRet->m_strCode, code);
			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

	static inline WTSEntrustAction* createCancelAction(const char* eid, const char* oid) noexcept
	{
		WTSEntrustAction* pRet = new WTSEntrustAction;
		if(pRet)
		{
			wt_strcpy(pRet->m_strEnturstID, eid);
			wt_strcpy(pRet->m_strOrderID, oid);
			return pRet;
		}

		return NULL;
	}

public:
	constexpr inline const char* getExchg() const  noexcept { return m_strExchg; }
	constexpr inline const char* getCode() const noexcept {return m_strCode;}

	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept {
		wt_strcpy(m_strExchg, exchg, len);
	}

	inline void setCode(const char* code, std::size_t len = 0) noexcept {
		wt_strcpy(m_strCode, code, len);
	}

	constexpr inline void setActionFlag(WTSActionFlag af) noexcept {m_actionFlag = af;}
	constexpr inline WTSActionFlag getActionFlag() const noexcept {return m_actionFlag;}

	inline void setEntrustID(const char* eid) noexcept { wt_strcpy(m_strEnturstID, eid); }
	constexpr inline const char* getEntrustID() const noexcept {return m_strEnturstID;}
	constexpr inline char* getEntrustID() noexcept { return m_strEnturstID; }

	inline void setOrderID(const char* oid) noexcept { wt_strcpy(m_strOrderID, oid); }
	constexpr inline const char* getOrderID() const noexcept {return m_strOrderID;}

	constexpr inline void setBusinessType(WTSBusinessType bType) noexcept { m_businessType = bType; }
	constexpr inline WTSBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	inline void setUserTag(const char* tag)  noexcept { wt_strcpy(m_strUserTag, tag); }
	constexpr inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	constexpr inline char* getUserTag()  noexcept { return m_strUserTag; }

	constexpr inline void setContractInfo(WTSContractInfo* cInfo)  noexcept { m_pContract = cInfo; }
	constexpr inline WTSContractInfo* getContractInfo() const  noexcept { return m_pContract; }

protected:
	WTSContractInfo*	m_pContract = NULL;
};

typedef struct _WTSOrderStruct
{
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];
	double			m_dVolume;
	double			m_iPrice;

	bool			m_bIsNet;
	bool			m_bIsBuy;

	WTSDirectionType	m_direction;
	WTSPriceType		m_priceType;
	WTSOrderFlag		m_orderFlag;
	WTSOffsetType		m_offsetType;

	char				m_strEntrustID[64] = { 0 };
	char				m_strUserTag[64] = { 0 };

	WTSBusinessType		m_businessType;

	//这部分是Order专有的成员
	uint32_t	m_uInsertDate;
	uint64_t	m_uInsertTime;
	double		m_dVolTraded;
	double		m_dVolLeft;
	bool		m_bIsError;

	WTSOrderState	m_orderState;
	WTSOrderType	m_orderType;
	char			m_strOrderID[64] = { 0 };
	char			m_strStateMsg[64] = { 0 };

	_WTSOrderStruct()
		:m_orderState(WOS_Submitting)
		, m_orderType(WORT_Normal)
		, m_uInsertDate(0)
		, m_uInsertTime(0)
		, m_dVolTraded(0)
		, m_dVolLeft(0)
		, m_bIsError(false)
	{

	}
} WTSOrderStruct;

//////////////////////////////////////////////////////////////////////////
//订单信息,查看订单状态变化等
class WTSOrderInfo : public WTSOrderStruct, public WTSPoolObject<WTSOrderInfo>
{
public:
	virtual ~WTSOrderInfo(){}

public:
	static inline WTSOrderInfo* create(WTSEntrust* entrust = NULL) noexcept
	{
		WTSOrderInfo *pRet = WTSOrderInfo::allocate();

		if(entrust != NULL)
		{
			wt_strcpy(pRet->m_strCode, entrust->getCode());
			wt_strcpy(pRet->m_strExchg,entrust->getExchg());
			pRet->m_iPrice = entrust->getPrice();
			pRet->m_dVolume = entrust->getVolume();

			pRet->m_direction = entrust->getDirection();
			pRet->m_offsetType = entrust->getOffsetType();
			pRet->m_orderFlag = entrust->getOrderFlag();
			pRet->m_priceType = entrust->getPriceType();
			wt_strcpy(pRet->m_strEntrustID, entrust->getEntrustID());
			wt_strcpy(pRet->m_strUserTag, entrust->getUserTag());

			pRet->m_dVolLeft = entrust->getVolume();
			pRet->m_businessType = entrust->getBusinessType();
		}

		return pRet;
	}

public:
	//这部分是和WTSEntrust同步的
	inline void setExchange(const char* exchg, std::size_t len = 0)  noexcept {
		wt_strcpy(m_strExchg, exchg, len);
	}

	inline void setCode(const char* code, std::size_t len = 0) noexcept {
		wt_strcpy(m_strCode, code, len);
	}

	constexpr inline void setDirection(WTSDirectionType dType)  noexcept { m_direction = dType; }
	constexpr inline void setPriceType(WTSPriceType pType)  noexcept { m_priceType = pType; }
	constexpr inline void setOrderFlag(WTSOrderFlag oFlag)  noexcept { m_orderFlag = oFlag; }
	constexpr inline void setOffsetType(WTSOffsetType oType) noexcept { m_offsetType = oType; }

	constexpr inline WTSDirectionType	getDirection() const  noexcept { return m_direction; }
	constexpr inline WTSPriceType		getPriceType() const  noexcept { return m_priceType; }
	constexpr inline WTSOrderFlag		getOrderFlag() const  noexcept { return m_orderFlag; }
	constexpr inline WTSOffsetType		getOffsetType() const  noexcept { return m_offsetType; }

	constexpr inline uint32_t getOffsetFlag() const noexcept
	{
		switch (m_offsetType)
		{
		case WOT_OPEN:	return 0;
		case WOT_CLOSEYESTERDAY:
		case WOT_CLOSE:
			return 1;
		default:
			return 2;
		}
	}

	constexpr inline bool isBuyAction() const noexcept
	{
		return (m_direction == WDT_LONG && m_offsetType == WOT_OPEN) || (m_direction == WDT_SHORT && m_offsetType != WOT_OPEN);
	}

	constexpr inline void setBusinessType(WTSBusinessType bType)  noexcept { m_businessType = bType; }
	constexpr inline WTSBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	constexpr inline void setVolume(double volume)  noexcept { m_dVolume = volume; }
	constexpr inline void setPrice(double price)  noexcept { m_iPrice = price; }

	constexpr inline double getVolume() const  noexcept { return m_dVolume; }
	constexpr inline double getPrice() const  noexcept { return m_iPrice; }

	constexpr inline const char* getCode() const noexcept { return m_strCode; }
	constexpr inline const char* getExchg() const  noexcept { return m_strExchg; }

	inline void setEntrustID(const char* eid)  noexcept { wt_strcpy(m_strEntrustID, eid); }
	constexpr inline const char* getEntrustID() const  noexcept { return m_strEntrustID; }
	constexpr inline char* getEntrustID()  noexcept { return m_strEntrustID; }

	inline void setUserTag(const char* tag)  noexcept { wt_strcpy(m_strUserTag, tag); }
	constexpr inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	constexpr inline char* getUserTag() noexcept { return m_strUserTag; }

	constexpr inline void setNetDirection(bool isBuy)  noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	constexpr inline bool isNet() const  noexcept { return m_bIsNet; }
	constexpr inline bool isBuy() const  noexcept { return m_bIsBuy; }

	constexpr inline void setContractInfo(WTSContractInfo* cInfo)  noexcept { m_pContract = cInfo; }
	constexpr inline WTSContractInfo* getContractInfo() const  noexcept { return m_pContract; }

public:
	constexpr inline void	setOrderDate(uint32_t uDate) noexcept {m_uInsertDate = uDate;}
	constexpr inline void	setOrderTime(uint64_t uTime) noexcept {m_uInsertTime = uTime;}
	constexpr inline void	setVolTraded(double vol) noexcept { m_dVolTraded = vol; }
	constexpr inline void	setVolLeft(double vol) noexcept { m_dVolLeft = vol; }
	
	inline void	setOrderID(const char* oid) noexcept { wt_strcpy(m_strOrderID, oid); }
	constexpr inline void	setOrderState(WTSOrderState os) noexcept {m_orderState = os;}
	constexpr inline void	setOrderType(WTSOrderType ot) noexcept {m_orderType = ot;}

	constexpr inline uint32_t getOrderDate() const noexcept {return m_uInsertDate;}
	constexpr inline uint64_t getOrderTime() const noexcept {return m_uInsertTime;}
	constexpr inline double getVolTraded() const noexcept { return m_dVolTraded; }
	constexpr inline double getVolLeft() const noexcept { return m_dVolLeft; }
    
	constexpr inline WTSOrderState	getOrderState() const  noexcept { return m_orderState; }
	constexpr inline WTSOrderType	getOrderType() const  noexcept { return m_orderType; }
	constexpr inline const char*	getOrderID() const  noexcept { return m_strOrderID; }
	constexpr inline char*			getOrderID()  noexcept { return m_strOrderID; }

	inline void	setStateMsg(const char* msg) noexcept {wt_strcpy(m_strStateMsg, msg);}
	constexpr inline const char*	getStateMsg() const noexcept {return m_strStateMsg;}
	constexpr inline char*			getStateMsg() noexcept { return m_strStateMsg; }

	constexpr inline bool	isAlive() const noexcept
	{
		if (m_bIsError)
			return false;

		switch(m_orderState)
		{
		case WOS_AllTraded:
		case WOS_Canceled:
			return false;
		default:
			return true;
		}
	}

	constexpr inline void	setError(bool bError = true) noexcept { m_bIsError = bError; }
	constexpr inline bool	isError() const noexcept { return m_bIsError; }

private:
	WTSContractInfo*	m_pContract = NULL;
};

typedef struct _WTSTradeStruct
{
	char	m_strExchg[MAX_EXCHANGE_LENGTH];	//市场
	char	m_strCode[MAX_INSTRUMENT_LENGTH];	//代码
	char	m_strTradeID[64] = { 0 };			//成交单号
	char	m_strRefOrder[64] = { 0 };			//本地委托序列号
	char	m_strUserTag[64] = { 0 };			//用户标签

	uint32_t	m_uTradeDate;
	uint64_t	m_uTradeTime;
	double		m_dVolume;
	double		m_dPrice;

	bool		m_bIsNet;
	bool		m_bIsBuy;

	WTSDirectionType	m_direction;
	WTSOffsetType		m_offsetType;
	WTSOrderType		m_orderType;
	WTSTradeType		m_tradeType;

	double		m_uAmount;

	WTSBusinessType		m_businessType;

	_WTSTradeStruct()
		: m_orderType(WORT_Normal)
		, m_tradeType(WTT_Common)
		, m_uAmount(0)
		, m_dPrice(0)
		, m_businessType(BT_CASH)
	{}
}WTSTradeStruct;

//////////////////////////////////////////////////////////////////////////
class WTSTradeInfo : public WTSTradeStruct, public WTSPoolObject<WTSTradeInfo>
{
public:
	
	virtual ~WTSTradeInfo(){}

public:
	static inline WTSTradeInfo* create(const char* code, const char* exchg = "", WTSBusinessType bType = BT_CASH)
	{
		WTSTradeInfo *pRet = WTSTradeInfo::allocate();
		wt_strcpy(pRet->m_strExchg, exchg);
		wt_strcpy(pRet->m_strCode, code);
		pRet->m_businessType = bType;

		return pRet;
	}

	inline void setTradeID(const char* tradeid) noexcept { wt_strcpy(m_strTradeID, tradeid); }
	inline void setRefOrder(const char* oid) noexcept { wt_strcpy(m_strRefOrder, oid); }
	
	constexpr inline void setDirection(WTSDirectionType dType)noexcept {m_direction = dType;}
	constexpr inline void setOffsetType(WTSOffsetType oType)noexcept {m_offsetType = oType;}
	constexpr inline void setOrderType(WTSOrderType ot)noexcept {m_orderType = ot;}
	constexpr inline void setTradeType(WTSTradeType tt)noexcept {m_tradeType = tt;}

	constexpr inline void setVolume(double volume)noexcept {m_dVolume = volume;}
	constexpr inline void setPrice(double price)noexcept { m_dPrice = price; }

	constexpr inline void setTradeDate(uint32_t uDate)noexcept {m_uTradeDate = uDate;}
	constexpr inline void setTradeTime(uint64_t uTime)noexcept {m_uTradeTime = uTime;}

	constexpr inline void setAmount(double amount) noexcept { m_uAmount = amount; }

	constexpr inline WTSDirectionType	getDirection() const noexcept {return m_direction;}
	constexpr inline WTSOffsetType		getOffsetType() const noexcept {return m_offsetType;}
	constexpr inline WTSOrderType		getOrderType() const noexcept {return m_orderType;}
	constexpr inline WTSTradeType		getTradeType() const noexcept {return m_tradeType;}

	constexpr inline uint32_t getOffsetFlag() const noexcept
	{
		switch (m_offsetType)
		{
		case WOT_OPEN:	return 0;
		case WOT_CLOSEYESTERDAY:
		case WOT_CLOSE:
			return 1;
		default:
			return 2;
		}
	}

	constexpr inline bool isBuyAction() const noexcept
	{
		return (m_direction == WDT_LONG && m_offsetType == WOT_OPEN) || (m_direction == WDT_SHORT && m_offsetType != WOT_OPEN);
	}

	constexpr inline double getVolume() const { return m_dVolume; }
	constexpr inline double getPrice() const noexcept { return m_dPrice; }

	constexpr inline const char*	getCode() const noexcept { return m_strCode; }
	constexpr inline const char*	getExchg() const noexcept { return m_strExchg; }
	constexpr inline const char*	getTradeID() const noexcept { return m_strTradeID; }
	constexpr inline const char*	getRefOrder() const noexcept { return m_strRefOrder; }

	constexpr inline char*	getTradeID() noexcept { return m_strTradeID; }
	constexpr inline char*	getRefOrder() noexcept { return m_strRefOrder; }

	constexpr inline uint32_t getTradeDate() const noexcept {return m_uTradeDate;}
	constexpr inline uint64_t getTradeTime() const noexcept {return m_uTradeTime;}

	constexpr inline double getAmount() const noexcept { return m_uAmount; }

	inline void setUserTag(const char* tag) noexcept { wt_strcpy(m_strUserTag, tag); }
	constexpr inline const char* getUserTag() const noexcept { return m_strUserTag; }

	constexpr inline void setBusinessType(WTSBusinessType bType) noexcept  { m_businessType = bType; }
	constexpr inline WTSBusinessType	getBusinessType() const noexcept { return m_businessType; }

	constexpr inline void setNetDirection(bool isBuy) noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	constexpr inline bool isNet() const noexcept { return m_bIsNet; }
	constexpr inline bool isBuy() const noexcept { return m_bIsBuy; }

	constexpr inline void setContractInfo(WTSContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	constexpr inline WTSContractInfo* getContractInfo() const noexcept { return m_pContract; }

protected:
	WTSContractInfo*	m_pContract = NULL;
};

typedef struct _WTSPositionStruct
{
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];
	char			m_strCurrency[8] = { 0 };

	WTSDirectionType	m_direction;//多空方向
	double		m_dPrePosition;		//昨仓
	double		m_dNewPosition;		//今仓
	double		m_dAvailPrePos;		//可平昨仓
	double		m_dAvailNewPos;		//可平今仓
	double		m_dTotalPosCost;	//持仓总成本
	double		m_dMargin;			//占用保证金
	double		m_dAvgPrice;		//持仓均价
	double		m_dDynProfit;		//浮动盈亏

	WTSBusinessType		m_businessType;

	_WTSPositionStruct()
		: m_direction(WDT_LONG)
		, m_dPrePosition(0)
		, m_dNewPosition(0)
		, m_dAvailPrePos(0)
		, m_dAvailNewPos(0)
		, m_dMargin(0)
		, m_dAvgPrice(0)
		, m_dDynProfit(0)
		, m_dTotalPosCost(0)
		, m_businessType(BT_CASH)
	{}
}WTSPositionStruct;

//////////////////////////////////////////////////////////////////////////
//持仓信息
class WTSPositionItem : public WTSPositionStruct, public WTSPoolObject<WTSPositionItem>
{
public:
	static inline WTSPositionItem* create(const char* code, const char* currency = "CNY", const char* exchg = "", WTSBusinessType bType = BT_CASH)
	{
		WTSPositionItem *pRet = WTSPositionItem::allocate();
		wt_strcpy(pRet->m_strExchg, exchg);
		wt_strcpy(pRet->m_strCode, code);
		wt_strcpy(pRet->m_strCurrency, currency);
		pRet->m_businessType = bType;

		return pRet;
	}

	constexpr inline void setDirection(WTSDirectionType dType) noexcept{m_direction = dType;}
	constexpr inline void setPrePosition(double prePos) noexcept { m_dPrePosition = prePos; }
	constexpr inline void setNewPosition(double newPos) noexcept { m_dNewPosition = newPos; }
	constexpr inline void setAvailPrePos(double availPos) noexcept { m_dAvailPrePos = availPos; }
	constexpr inline void setAvailNewPos(double availPos) noexcept { m_dAvailNewPos = availPos; }
	constexpr inline void setPositionCost(double cost) noexcept {m_dTotalPosCost = cost;}
	constexpr inline void setMargin(double margin) noexcept { m_dMargin = margin; }
	constexpr inline void setAvgPrice(double avgPrice) noexcept { m_dAvgPrice = avgPrice; }
	constexpr inline void setDynProfit(double profit) noexcept { m_dDynProfit = profit; }

	constexpr inline WTSDirectionType getDirection() const noexcept {return m_direction;}
	constexpr inline double	getPrePosition() const noexcept { return m_dPrePosition; }
	constexpr inline double	getNewPosition() const noexcept { return m_dNewPosition; }
	constexpr inline double	getAvailPrePos() const noexcept { return m_dAvailPrePos; }
	constexpr inline double	getAvailNewPos() const noexcept { return m_dAvailNewPos; }

	constexpr inline double	getTotalPosition() const noexcept { return m_dPrePosition + m_dNewPosition; }
	constexpr inline double	getAvailPosition() const noexcept { return m_dAvailPrePos + m_dAvailNewPos; }

	constexpr inline double	getFrozenPosition() const noexcept { return getTotalPosition() - getAvailPosition(); }
	constexpr inline double	getFrozenNewPos() const noexcept { return m_dNewPosition - m_dAvailNewPos; }
	constexpr inline double	getFrozenPrePos() const noexcept { return m_dPrePosition - m_dAvailPrePos; }

	constexpr inline double		getPositionCost() const noexcept { return m_dTotalPosCost; }
	constexpr inline double		getMargin() const noexcept { return m_dMargin; }
	constexpr inline double		getAvgPrice() const noexcept { return m_dAvgPrice; }
	constexpr inline double		getDynProfit() const noexcept { return m_dDynProfit; }

	constexpr inline const char* getCode() const noexcept { return m_strCode; }
	constexpr inline const char* getCurrency() const noexcept { return m_strCurrency; }
	constexpr inline const char* getExchg() const noexcept { return m_strExchg; }

	constexpr inline void setBusinessType(WTSBusinessType bType) noexcept { m_businessType = bType; }
	constexpr inline WTSBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	constexpr inline void setContractInfo(WTSContractInfo* cInfo)  noexcept { m_pContract = cInfo; }
	constexpr inline WTSContractInfo* getContractInfo() const  noexcept { return m_pContract; }

public:
	virtual ~WTSPositionItem(){}

protected:
	WTSContractInfo*	m_pContract = NULL;
};

typedef struct _WTSAccountStruct
{
	char		m_strCurrency[8];

	double		m_dBalance;					//余额
	double		m_dPreBalance;				//上次结算准备金
	double		m_uMargin;					//当前保证金总额
	double		m_dCommission;				//手续费
	double		m_dFrozenMargin;			//冻结的保证金
	double		m_dFrozenCommission;		//冻结的手续费
	double		m_dCloseProfit;				//平仓盈亏
	double		m_dDynProfit;				//持仓盈亏
	double		m_dDeposit;					//入金金额
	double		m_dWithdraw;				//出金金额
	double		m_dAvailable;				//可用资金

	_WTSAccountStruct()
		: m_strCurrency("CNY")
		, m_dBalance(0)
		, m_dPreBalance(0)
		, m_dCommission(0)
		, m_dFrozenMargin(0)
		, m_dFrozenCommission(0)
		, m_dCloseProfit(0)
		, m_dDynProfit(0)
		, m_dDeposit(0)
		, m_dWithdraw(0)
		, m_dAvailable(0)
	{
	}
}WTSAccountStruct;
//////////////////////////////////////////////////////////////////////////
//账户信息
class WTSAccountInfo : public WTSAccountStruct, public WTSPoolObject<WTSAccountInfo>
{
public:
	
	virtual ~WTSAccountInfo(){}

public:
	static inline WTSAccountInfo* create(){return WTSAccountInfo::allocate();}

	inline void	setCurrency(const char* currency) noexcept { strcpy(m_strCurrency, currency); }

	constexpr inline void	setBalance(double balance) noexcept {m_dBalance = balance;}
	constexpr inline void	setPreBalance(double prebalance) noexcept {m_dPreBalance = prebalance;}
	constexpr inline void	setMargin(double margin) noexcept {m_uMargin = margin;}
	constexpr inline void	setFrozenMargin(double frozenmargin) noexcept {m_dFrozenMargin = frozenmargin;}
	constexpr inline void	setCloseProfit(double closeprofit) noexcept {m_dCloseProfit = closeprofit;}
	constexpr inline void	setDynProfit(double dynprofit) noexcept {m_dDynProfit = dynprofit;}
	constexpr inline void	setDeposit(double deposit) noexcept {m_dDeposit = deposit;}
	constexpr inline void	setWithdraw(double withdraw) noexcept {m_dWithdraw = withdraw;}
	constexpr inline void	setCommission(double commission) noexcept {m_dCommission = commission;}
	constexpr inline void	setFrozenCommission(double frozencommission) noexcept {m_dFrozenCommission = frozencommission;}
	constexpr inline void	setAvailable(double available) noexcept {m_dAvailable = available;}

	constexpr inline double	getBalance() const noexcept {return m_dBalance;}
	constexpr inline double	getPreBalance() const noexcept {return m_dPreBalance;}
	constexpr inline double	getMargin() const noexcept {return m_uMargin;}
	constexpr inline double	getFrozenMargin() const noexcept {return m_dFrozenMargin;}
	constexpr inline double	getCloseProfit() const noexcept {return m_dCloseProfit;}
	constexpr inline double	getDynProfit() const noexcept {return m_dDynProfit;}
	constexpr inline double	getDeposit() const noexcept {return m_dDeposit;}
	constexpr inline double	getWithdraw() const noexcept {return m_dWithdraw;}
	constexpr inline double	getCommission() const noexcept {return m_dCommission;}
	constexpr inline double	getFrozenCommission() const noexcept {return m_dFrozenCommission;}
	constexpr inline double	getAvailable() const noexcept {return m_dAvailable;}

	inline const char* getCurrency() const noexcept { return m_strCurrency; }
};


NS_WTP_END
