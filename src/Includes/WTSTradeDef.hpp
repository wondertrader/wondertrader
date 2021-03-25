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

NS_OTP_BEGIN
//////////////////////////////////////////////////////////////////////////
//委托数据结构,用户客户端向服务端发起
class WTSEntrust : public WTSObject
{
protected:
	WTSEntrust()
		: m_iPrice(0)
		, m_strCode("")
		, m_dVolume(0)
		, m_strExchg("")
		, m_uSndTm(0)
		, m_businessType(BT_CASH)
		, m_direction(WDT_LONG)
		, m_priceType(WPT_ANYPRICE)
		, m_timeCond(WTC_GFD)
		, m_offsetType(WOT_OPEN)
	{
	}

	virtual ~WTSEntrust(){}

public:
	static inline WTSEntrust* create(const char* code, double vol, double price, const char* exchg = "", WTSBusinessType bType = BT_CASH)
	{
		WTSEntrust* pRet = new WTSEntrust;
		if(pRet)
		{
			pRet->m_strExchg = exchg;
			pRet->m_strCode = code;
			pRet->m_dVolume = vol;
			pRet->m_iPrice = price;
			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

public:
	inline void setExchange(const char* exchg){ m_strExchg = exchg; }
	inline void setCode(const char* code){m_strCode = code;}

	inline void setDirection(WTSDirectionType dType){m_direction = dType;}
	inline void setPriceType(WTSPriceType pType){m_priceType = pType;}
	inline void setTimeCondition(WTSTimeCondition tCond){m_timeCond = tCond;}
	inline void setOffsetType(WTSOffsetType oType){m_offsetType = oType;}

	inline WTSDirectionType	getDirection() const{return m_direction;}
	inline WTSPriceType		getPriceType() const{return m_priceType;}
	inline WTSTimeCondition	getTimeCondition() const{return m_timeCond;}
	inline WTSOffsetType	getOffsetType() const{return m_offsetType;}

	inline void setBusinessType(WTSBusinessType bType) { m_businessType = bType; }
	inline WTSBusinessType	getBusinessType() const { return m_businessType; }

	inline void setVolume(double volume){ m_dVolume = volume; }
	inline void setPrice(double price){ m_iPrice = price; }

	inline double getVolume() const{ return m_dVolume; }
	inline double getPrice() const{ return m_iPrice; }

	inline const char* getCode() const{return m_strCode.c_str();}
	inline const char* getExchg() const{return m_strExchg.c_str();}

	inline void setEntrustID(const char* eid){m_strEntrustID = eid;}
	inline const char* getEntrustID() const{return m_strEntrustID.c_str();}

	inline void setUserTag(const char* tag){m_strUserTag = tag;}
	inline const char* getUserTag() const{return m_strUserTag.c_str();}

	inline void setSent() { m_uSndTm = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }
	inline uint64_t getSendTime() const { return m_uSndTm; }

protected:
	std::string		m_strExchg;
	std::string		m_strCode;
	double			m_dVolume;
	double			m_iPrice;

	uint64_t		m_uSndTm;

	WTSDirectionType	m_direction;
	WTSPriceType		m_priceType;
	WTSTimeCondition	m_timeCond;
	WTSOffsetType		m_offsetType;
	std::string			m_strEntrustID;

	std::string			m_strUserTag;

	WTSBusinessType		m_businessType;
};


//////////////////////////////////////////////////////////////////////////
//委托操作: 撤单、改单
class WTSEntrustAction : public WTSObject
{
protected:
	WTSEntrustAction()
		: m_iPrice(0)
		, m_strCode("")
		, m_dVolume(0)
		, m_actionFlag(WAF_CANCEL)
		, m_businessType(BT_CASH)
	{

	}

	virtual ~WTSEntrustAction(){}

public:
	static inline WTSEntrustAction* create(const char* code, const char* exchg = "", double vol = 0, double price = 0, WTSBusinessType bType = BT_CASH)
	{
		WTSEntrustAction* pRet = new WTSEntrustAction;
		if(pRet)
		{
			pRet->m_strExchg = exchg;
			pRet->m_strCode = code;
			pRet->m_dVolume = vol;
			pRet->m_iPrice = price;
			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

	static inline WTSEntrustAction* createCancelAction(const char* eid, const char* oid)
	{
		WTSEntrustAction* pRet = new WTSEntrustAction;
		if(pRet)
		{
			pRet->m_strEnturstID = eid;
			pRet->m_strOrderID = oid;
			return pRet;
		}

		return NULL;
	}

public:
	inline void setVolume(double volume){ m_dVolume = volume; }
	inline void setPrice(double price){ m_iPrice = price; }

	inline double getVolume() const{ return m_dVolume; }
	inline double getPrice() const{ return m_iPrice; }

	inline const char* getCode() const{return m_strCode.c_str();}

	inline void setExchg(const char* exchg){ m_strExchg = exchg; }
	inline const char* getExchg() const{ return m_strExchg.c_str(); }

	inline void setActionFlag(WTSActionFlag af){m_actionFlag = af;}
	inline WTSActionFlag getActionFlag() const{return m_actionFlag;}

	inline void setEntrustID(const char* eid){m_strEnturstID = eid;}
	inline const char* getEntrustID() const{return m_strEnturstID.c_str();}

	inline void setOrderID(const char* oid){m_strOrderID = oid;}
	inline const char* getOrderID() const{return m_strOrderID.c_str();}

	inline void setBusinessType(WTSBusinessType bType) { m_businessType = bType; }
	inline WTSBusinessType	getBusinessType() const { return m_businessType; }

protected:
	std::string		m_strExchg;
	std::string		m_strCode;
	double			m_dVolume;
	double			m_iPrice;

	std::string		m_strEnturstID;
	WTSActionFlag	m_actionFlag;

	std::string		m_strOrderID;

	WTSBusinessType	m_businessType;
};

//////////////////////////////////////////////////////////////////////////
//订单信息,查看订单状态变化等
class WTSOrderInfo : public WTSEntrust
{
protected:
	WTSOrderInfo()
		:m_orderState(WOS_Submitting)
		,m_orderType(WORT_Normal)
		,m_uInsertDate(0)
		,m_uInsertTime(0)
		,m_dVolTraded(0)
		,m_dVolLeft(0)
		,m_bIsError(false)
	{

	}

	virtual ~WTSOrderInfo(){}

public:
	static inline WTSOrderInfo* create(WTSEntrust* entrust = NULL)
	{
		WTSOrderInfo *pRet = new WTSOrderInfo;

		if(entrust != NULL)
		{
			pRet->m_strExchg = entrust->getExchg();
			pRet->m_iPrice = entrust->getPrice();
			pRet->m_strCode = entrust->getCode();
			pRet->m_dVolume = entrust->getVolume();

			pRet->m_direction = entrust->getDirection();
			pRet->m_offsetType = entrust->getOffsetType();
			pRet->m_timeCond = entrust->getTimeCondition();
			pRet->m_priceType = entrust->getPriceType();
			pRet->m_strEntrustID = entrust->getEntrustID();
			pRet->m_strUserTag = entrust->getUserTag();

			pRet->m_dVolLeft = entrust->getVolume();
			pRet->m_businessType = entrust->getBusinessType();
		}

		return pRet;
	}

	inline void	setOrderDate(uint32_t uDate){m_uInsertDate = uDate;}
	inline void	setOrderTime(uint64_t uTime){m_uInsertTime = uTime;}
	inline void	setVolTraded(double vol){ m_dVolTraded = vol; }
	inline void	setVolLeft(double vol){ m_dVolLeft = vol; }
	
	inline void	setOrderID(const char* oid){m_strOrderID = oid;/*StrUtil::trim(m_strOrderID);*/}
	inline void	setOrderState(WTSOrderState os){m_orderState = os;}
	inline void	setOrderType(WTSOrderType ot){m_orderType = ot;}

	inline uint32_t getOrderDate() const{return m_uInsertDate;}
	inline uint64_t getOrderTime() const{return m_uInsertTime;}
	inline double getVolTraded() const{ return m_dVolTraded; }
	inline double getVolLeft() const{ return m_dVolLeft; }
    
	inline WTSOrderState		getOrderState() const{return m_orderState;}
	inline WTSOrderType			getOrderType() const{return m_orderType;}
	inline const char*			getOrderID() const{return m_strOrderID.c_str();}

	inline void	setCode(const char* code){m_strCode = code;}
	inline void	setExchg(const char* exchg){ m_strExchg = exchg; }

	inline void	setStateMsg(const char* msg){m_strStateMsg = msg;}
	inline const char* getStateMsg() const{return m_strStateMsg.c_str();}

	inline bool	isAlive() const
	{
		switch(m_orderState)
		{
		case WOS_AllTraded:
		case WOS_Canceled:
			return false;
		default:
			return true;
		}
	}

	inline void	setError(bool bError = true){ m_bIsError = bError; }
	inline bool	isError() const{ return m_bIsError; }

private:
	uint32_t	m_uInsertDate;
	uint64_t	m_uInsertTime;
	double		m_dVolTraded;
	double		m_dVolLeft;
	bool		m_bIsError;

	WTSOrderState	m_orderState;
	WTSOrderType	m_orderType;
	std::string		m_strOrderID;
	std::string		m_strStateMsg;
};

//////////////////////////////////////////////////////////////////////////
class WTSTradeInfo : public WTSObject
{
protected:
	WTSTradeInfo()
		: m_orderType(WORT_Normal)
		, m_tradeType(WTT_Common)
		, m_strExchg("")
		, m_uAmount(0)
		, m_dPrice(0)
		, m_businessType(BT_CASH)
	{}
	virtual ~WTSTradeInfo(){}

public:
	static inline WTSTradeInfo* create(const char* code, const char* exchg = "", WTSBusinessType bType = BT_CASH)
	{
		WTSTradeInfo *pRet = new WTSTradeInfo;
		pRet->m_strExchg = exchg;
		pRet->m_strCode = code;
		pRet->m_businessType = bType;

		return pRet;
	}

	inline void setTradeID(const char* tradeid){m_strTradeID = tradeid;/*StrUtil::trim(m_strTradeID);*/}
	inline void setRefOrder(const char* oid){m_strRefOrder = oid;/*StrUtil::trim(m_strRefOrder);*/}
	
	inline void setDirection(WTSDirectionType dType){m_direction = dType;}
	inline void setOffsetType(WTSOffsetType oType){m_offsetType = oType;}
	inline void setOrderType(WTSOrderType ot){m_orderType = ot;}
	inline void setTradeType(WTSTradeType tt){m_tradeType = tt;}

	inline void setVolume(double volume){m_dVolume = volume;}
	inline void setPrice(double price){ m_dPrice = price; }

	inline void setTradeDate(uint32_t uDate){m_uTradeDate = uDate;}
	inline void setTradeTime(uint64_t uTime){m_uTradeTime = uTime;}

	inline void setAmount(double amount){ m_uAmount = amount; }

	inline WTSDirectionType	getDirection() const{return m_direction;}
	inline WTSOffsetType		getOffsetType() const{return m_offsetType;}
	inline WTSOrderType		getOrderType() const{return m_orderType;}
	inline WTSTradeType		getTradeType() const{return m_tradeType;}

	inline double getVolume() const{ return m_dVolume; }
	inline double getPrice() const{ return m_dPrice; }

	inline const char*	getCode() const{return m_strCode.c_str();}
	inline const char*	getExchg() const{ return m_strExchg.c_str(); }
	inline const char*	getTradeID() const{return m_strTradeID.c_str();}
	inline const char*	getRefOrder() const{return m_strRefOrder.c_str();}

	inline uint32_t getTradeDate() const{return m_uTradeDate;}
	inline uint64_t getTradeTime() const{return m_uTradeTime;}

	inline double getAmount() const{ return m_uAmount; }

	inline void setUserTag(const char* tag){m_strUserTag = tag;}
	inline const char* getUserTag() const{return m_strUserTag.c_str();}

	inline void setBusinessType(WTSBusinessType bType) { m_businessType = bType; }
	inline WTSBusinessType	getBusinessType() const { return m_businessType; }

protected:
	std::string	m_strExchg;		//市场
	std::string m_strCode;			//代码
	std::string m_strTradeID;		//成交单号
	std::string	m_strRefOrder;		//本地委托序列号
	std::string	m_strUserTag;		//用户标签

	uint32_t	m_uTradeDate;
	uint64_t	m_uTradeTime;
	double		m_dVolume;
	double		m_dPrice;

	WTSDirectionType	m_direction;
	WTSOffsetType		m_offsetType;
	WTSOrderType		m_orderType;
	WTSTradeType		m_tradeType;

	double	m_uAmount;

	WTSBusinessType	m_businessType;
};

//////////////////////////////////////////////////////////////////////////
//持仓信息
class WTSPositionItem : public WTSObject
{
public:
	static inline WTSPositionItem* create(const char* code, const char* currency, const char* exchg = "", WTSBusinessType bType = BT_CASH)
	{
		WTSPositionItem *pRet = new WTSPositionItem;
		pRet->m_strExchg = exchg;
		pRet->m_strCode = code;
		pRet->m_strCurrency = currency;
		pRet->m_businessType = bType;

		return pRet;
	}

	inline void setDirection(WTSDirectionType dType){m_direction = dType;}
	inline void setPrePosition(double prePos){ m_dPrePosition = prePos; }
	inline void setNewPosition(double newPos){ m_dNewPosition = newPos; }
	inline void setAvailPrePos(double availPos){ m_dAvailPrePos = availPos; }
	inline void setAvailNewPos(double availPos){ m_dAvailNewPos = availPos; }
	inline void setPositionCost(double cost){m_dTotalPosCost = cost;}
	inline void setMargin(double margin){ m_dMargin = margin; }
	inline void setAvgPrice(double avgPrice){ m_dAvgPrice = avgPrice; }
	inline void setDynProfit(double profit){ m_dDynProfit = profit; }

	inline WTSDirectionType getDirection() const{return m_direction;}
	inline double	getPrePosition() const{ return m_dPrePosition; }
	inline double	getNewPosition() const{ return m_dNewPosition; }
	inline double	getAvailPrePos() const{ return m_dAvailPrePos; }
	inline double	getAvailNewPos() const{ return m_dAvailNewPos; }

	inline double	getTotalPosition() const{ return m_dPrePosition + m_dNewPosition; }
	inline double	getAvailPosition() const{ return m_dAvailPrePos + m_dAvailNewPos; }

	inline double	getFrozenPosition() const{ return getTotalPosition() - getAvailPosition(); }
	inline double	getFrozenNewPos() const{ return m_dNewPosition - m_dAvailNewPos; }
	inline double	getFrozenPrePos() const{ return m_dPrePosition - m_dAvailPrePos; }

	inline double		getPositionCost() const{ return m_dTotalPosCost; }
	inline double		getMargin() const{ return m_dMargin; }
	inline double		getAvgPrice() const{ return m_dAvgPrice; }
	inline double		getDynProfit() const{ return m_dDynProfit; }

	inline const char* getCode() const{ return m_strCode.c_str(); }
	inline const char* getCurrency() const{ return m_strCurrency.c_str(); }
	inline const char* getExchg() const{ return m_strExchg.c_str(); }

	inline void setBusinessType(WTSBusinessType bType) { m_businessType = bType; }
	inline WTSBusinessType	getBusinessType() const { return m_businessType; }

protected:
	WTSPositionItem()
		: m_direction(WDT_LONG)
		, m_dPrePosition(0)
		, m_dNewPosition(0)
		, m_dAvailPrePos(0)
		, m_dAvailNewPos(0)
		, m_dMargin(0)
		, m_dAvgPrice(0)
		, m_dDynProfit(0)
		, m_dTotalPosCost(0)
		, m_strCurrency("CNY")
		, m_businessType(BT_CASH)
	{}
	virtual ~WTSPositionItem(){}

	std::string		m_strExchg;
	std::string		m_strCode;
	std::string		m_strCurrency;

	WTSDirectionType	m_direction;//多空方向
	double		m_dPrePosition;		//昨仓
	double		m_dNewPosition;		//今仓
	double		m_dAvailPrePos;		//可平昨仓
	double		m_dAvailNewPos;		//可平今仓
	double		m_dTotalPosCost;	//持仓总成本
	double		m_dMargin;			//占用保证金
	double		m_dAvgPrice;		//持仓均价
	double		m_dDynProfit;		//浮动盈亏

	WTSBusinessType	m_businessType;
};


//////////////////////////////////////////////////////////////////////////
//持仓明细
class WTSPositionDetail : public WTSObject
{
public:
	static inline WTSPositionDetail* create(const char* code = "", const char* exchg = "", WTSBusinessType bType = BT_CASH)
	{
		WTSPositionDetail* pRet = new WTSPositionDetail();
		pRet->m_strCode = code;
		pRet->m_strExchg = exchg;

		return pRet;
	}

	inline void setCode(const char* code){m_strCode = code;}
	inline void setTradeID(const char* tid){m_strTradeID = tid;}
	inline void setUserTag(const char* tag){m_strUserTag = tag;}

	inline void setDirection(WTSDirectionType dType){m_direction = dType;}

	inline void setOpenDate(uint32_t uDate){m_uOpenDate = uDate;}
	inline void setOpenTime(uint64_t uTime){m_uOpenTime = uTime;}

	inline void setVolume(double vol){ m_dVolume = vol; }
	inline void setOpenPrice(double openpx){ m_dOpenPrice = openpx; }
	inline void setMargin(double margin){ m_dMargin = margin; }
	inline void setCloseVol(double closevol){ m_dCloseVol = closevol; }
	inline void setCloseAmount(double closeamount){ m_dCloseAmount = closeamount; }
	inline void setCloseProfitByDate(double profitbydate){ m_dCloseProfitByDate = profitbydate; }
	inline void setCloseProfitByTrade(double profitbytrade){ m_dCloseProfitByTrade = profitbytrade; }
	inline void setPreSettlePx(double preSettlePx){ m_dPreSettlePx = preSettlePx; }

	inline const char* getCode() const{return m_strCode.c_str();}
	inline const char* getTradeID() const{return m_strTradeID.c_str();}
	inline const char* getUserTag() const{return m_strUserTag.c_str();}

	inline const char* getExchg() const{return m_strExchg.c_str();}

	inline WTSDirectionType getDirection() const{return m_direction;}

	inline uint32_t getOpenDate() const{return m_uOpenDate;}
	inline uint64_t getOpenTime() const{return m_uOpenTime;}

	inline double		getOpenPrice() const{ return m_dOpenPrice; }
	inline double		getVolume() const{ return m_dVolume; }
	inline double		getMargin() const{ return m_dMargin; }
	inline double		getCloseVol() const{ return m_dCloseVol; }
	inline double		getCloseAmount() const{ return m_dCloseAmount; }
	inline double		getCloseProfitByDate() const{ return m_dCloseProfitByDate; }
	inline double		getCloseProfitByTrade() const{ return m_dCloseProfitByTrade; }

	inline void setBusinessType(WTSBusinessType bType) { m_businessType = bType; }
	inline WTSBusinessType	getBusinessType() const { return m_businessType; }


protected:
	WTSPositionDetail()
		: m_direction(WDT_LONG)
		, m_uOpenDate(0)
		, m_uOpenTime(0)
		, m_dVolume(0)
		, m_dOpenPrice(0)
		, m_dMargin(0)
		, m_dCloseVol(0)
		, m_dCloseAmount(0)
		, m_dCloseProfitByDate(0)
		, m_dCloseProfitByTrade(0)
		, m_dPreSettlePx(0)
		, m_businessType(BT_CASH)
	{}
	virtual ~WTSPositionDetail(){}

	std::string	m_strExchg;
	std::string	m_strCode;
	std::string	m_strTradeID;
	std::string m_strUserTag;

	WTSDirectionType	m_direction;//多空方向
	uint32_t	m_uOpenDate;
	uint64_t	m_uOpenTime;
	double		m_dVolume;
	double		m_dOpenPrice;
	double		m_dMargin;
	double		m_dCloseVol;
	double		m_dCloseAmount;
	double		m_dCloseProfitByDate;
	double		m_dCloseProfitByTrade;
	double		m_dPreSettlePx;

	WTSBusinessType	m_businessType;
};


//////////////////////////////////////////////////////////////////////////
//投资者信息
class WTSInvestorInfo : public WTSObject
{
protected:
	WTSInvestorInfo():m_uState(0){}
	~WTSInvestorInfo(){}

public:
	static inline WTSInvestorInfo* create(){return new WTSInvestorInfo;}

	inline void	setDescription(const char* desc){m_strDescription = desc;}
	inline void	setUsername(const char* username){m_strUserName = username;}
	inline void	setState(uint32_t uState){m_uState = uState;}
	inline void	setExtInfo(const char* key, const char* val){ m_mapExts[key] = val; }

	inline const char* getUsername() const{return m_strUserName.c_str();}
	inline const char* getDescription() const{return m_strDescription.c_str();}
	inline uint32_t	getState() const{return m_uState;}

	inline const char* getExtInfo(const char* key)
	{
		auto it = m_mapExts.find(key);
		if (it == m_mapExts.end())
			return "";

		return it->second.c_str();
	}

private:
	std::string	m_strDescription;
	std::string	m_strUserName;
	uint32_t	m_uState;
	std::map<std::string, std::string>	m_mapExts;
};

//////////////////////////////////////////////////////////////////////////
//账户信息
class WTSAccountInfo : public WTSObject
{
protected:
	WTSAccountInfo():m_strCurrency("CNY"){}
	virtual ~WTSAccountInfo(){}

public:
	static inline WTSAccountInfo* create(){return new WTSAccountInfo;}

	inline void	setDescription(const char* desc){m_strDescription = desc;}
	inline void	setCurrency(const char* currency){ m_strCurrency = currency; }

	inline void	setBalance(double balance){m_uBalance = balance;}
	inline void	setPreBalance(double prebalance){m_uPreBalance = prebalance;}
	inline void	setMargin(double margin){m_uMargin = margin;}
	inline void	setFrozenMargin(double frozenmargin){m_uFrozenMargin = frozenmargin;}
	inline void	setCloseProfit(double closeprofit){m_iCloseProfit = closeprofit;}
	inline void	setDynProfit(double dynprofit){m_iDynProfit = dynprofit;}
	inline void	setDeposit(double deposit){m_uDeposit = deposit;}
	inline void	setWithdraw(double withdraw){m_uWithdraw = withdraw;}
	inline void	setCommission(double commission){m_uCommission = commission;}
	inline void	setFrozenCommission(double frozencommission){m_uFrozenCommission = frozencommission;}
	inline void	setAvailable(double available){m_uAvailable = available;}

	inline double	getBalance() const{return m_uBalance;}
	inline double	getPreBalance() const{return m_uPreBalance;}
	inline double	getMargin() const{return m_uMargin;}
	inline double	getFrozenMargin() const{return m_uFrozenMargin;}
	inline double	getCloseProfit() const{return m_iCloseProfit;}
	inline double	getDynProfit() const{return m_iDynProfit;}
	inline double	getDeposit() const{return m_uDeposit;}
	inline double	getWithdraw() const{return m_uWithdraw;}
	inline double	getCommission() const{return m_uCommission;}
	inline double	getFrozenCommission() const{return m_uFrozenCommission;}
	inline double	getAvailable() const{return m_uAvailable;}

	inline const char* getDescription() const{return m_strDescription.c_str();}
	inline const char* getCurrency() const{ return m_strCurrency.c_str(); }

protected:
	std::string	m_strDescription;
	std::string m_strCurrency;

	double		m_uBalance;
	double		m_uPreBalance;
	double		m_uMargin;
	double		m_uCommission;
	double		m_uFrozenMargin;
	double		m_uFrozenCommission;
	double		m_iCloseProfit;
	double		m_iDynProfit;
	double		m_uDeposit;
	double		m_uWithdraw;
	double		m_uAvailable;
};


NS_OTP_END
