#pragma once
#include "WTSObject.hpp"
#include "WTSTypes.h"
#include "WTSMarcos.h"
#include "StrUtil.hpp"

#include <time.h>
#include <string>
#include <map>

NS_OTP_BEGIN
//////////////////////////////////////////////////////////////////////////
//委托数据结构，用户客户端向服务端发起
class WTSEntrust : public WTSObject
{
protected:
	WTSEntrust()
		:m_iPrice(0)
		,m_strCode("")
		,m_uVolumn(0)
		,m_strExchg("")
	{
		m_direction = WDT_LONG;
		m_priceType = WPT_ANYPRICE;
		m_timeCond = WTC_GFD;
		m_offsetType = WOT_OPEN;
	}

	virtual ~WTSEntrust(){}

public:
	static WTSEntrust* create(const char* code, uint32_t vol, double price, const char* exchg = "")
	{
		WTSEntrust* pRet = new WTSEntrust;
		if(pRet)
		{
			pRet->m_strExchg = exchg;
			pRet->m_strCode = code;
			pRet->m_uVolumn = vol;
			pRet->m_iPrice = price;
			return pRet;
		}

		return NULL;
	}

public:
	void setExchange(const char* exchg){ m_strExchg = exchg; }
	void setCode(const char* code){m_strCode = code;}
	void setDirection(WTSDirectionType dType){m_direction = dType;}
	void setPriceType(WTSPriceType pType){m_priceType = pType;}
	void setTimeCondition(WTSTimeCondition tCond){m_timeCond = tCond;}
	void setOffsetType(WTSOffsetType oType){m_offsetType = oType;}

	WTSDirectionType	getDirection() const{return m_direction;}
	WTSPriceType		getPriceType() const{return m_priceType;}
	WTSTimeCondition	getTimeCondition() const{return m_timeCond;}
	WTSOffsetType	getOffsetType() const{return m_offsetType;}

	void setVolumn(uint32_t volumn){m_uVolumn = volumn;}
	void setPrice(double price){ m_iPrice = price; }

	uint32_t getVolumn() const{return m_uVolumn;}
	double getPrice() const{ return m_iPrice; }

	const char* getCode() const{return m_strCode.c_str();}
	const char* getMarket() const{return m_strExchg.c_str();}

	void setEntrustID(const char* eid){m_strEntrustID = eid;}
	const char* getEntrustID() const{return m_strEntrustID.c_str();}

	void setUserTag(const char* tag){m_strUserTag = tag;}
	const char* getUserTag() const{return m_strUserTag.c_str();}

protected:
	std::string		m_strExchg;
	std::string		m_strCode;
	uint32_t		m_uVolumn;
	double			m_iPrice;

	WTSDirectionType	m_direction;
	WTSPriceType		m_priceType;
	WTSTimeCondition	m_timeCond;
	WTSOffsetType		m_offsetType;
	std::string			m_strEntrustID;

	std::string			m_strUserTag;
};


//////////////////////////////////////////////////////////////////////////
//委托操作：撤单、改单
class WTSEntrustAction : public WTSObject
{
protected:
	WTSEntrustAction()
		:m_iPrice(0)
		,m_strCode("")
		,m_uVolumn(0)
		,m_actionFlag(WAF_CANCEL)
	{

	}

	virtual ~WTSEntrustAction(){}

public:
	static WTSEntrustAction* create(const char* code, const char* exchg = "", uint32_t vol = 0, uint32_t price = 0)
	{
		WTSEntrustAction* pRet = new WTSEntrustAction;
		if(pRet)
		{
			pRet->m_strExchg = exchg;
			pRet->m_strCode = code;
			pRet->m_uVolumn = vol;
			pRet->m_iPrice = price;
			return pRet;
		}

		return NULL;
	}

	static WTSEntrustAction* createCancelAction(const char* eid, const char* oid)
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
	void setVolumn(uint32_t volumn){m_uVolumn = volumn;}
	void setPrice(double price){ m_iPrice = price; }

	uint32_t getVolumn() const{return m_uVolumn;}
	double getPrice() const{ return m_iPrice; }

	const char* getCode() const{return m_strCode.c_str();}

	void setExchg(const char* exchg){ m_strExchg = exchg; }
	const char* getExchg() const{ return m_strExchg.c_str(); }

	void setActionFlag(WTSActionFlag af){m_actionFlag = af;}
	WTSActionFlag getActionFlag() const{return m_actionFlag;}

	void setEntrustID(const char* eid){m_strEnturstID = eid;}
	const char* getEntrustID() const{return m_strEnturstID.c_str();}

	void setOrderID(const char* oid){m_strOrderID = oid;}
	const char* getOrderID() const{return m_strOrderID.c_str();}

protected:
	std::string		m_strExchg;
	std::string		m_strCode;
	uint32_t		m_uVolumn;
	double			m_iPrice;

	std::string		m_strEnturstID;
	WTSActionFlag	m_actionFlag;

	std::string		m_strOrderID;
};

//////////////////////////////////////////////////////////////////////////
//订单信息，查看订单状态变化等
class WTSOrderInfo : public WTSEntrust
{
protected:
	WTSOrderInfo()
		:m_orderState(WOS_Submitting)
		,m_orderType(WORT_Normal)
		,m_uInsertDate(0)
		,m_uInsertTime(0)
		,m_uVolTraded(0)
		,m_uVolLeft(0)
		,m_bIsError(false)
	{

	}

	virtual ~WTSOrderInfo(){}

public:
	static WTSOrderInfo* create(WTSEntrust* entrust = NULL)
	{
		WTSOrderInfo *pRet = new WTSOrderInfo;

		if(entrust != NULL)
		{
			pRet->m_strExchg = entrust->getMarket();
			pRet->m_iPrice = entrust->getPrice();
			pRet->m_strCode = entrust->getCode();
			pRet->m_uVolumn = entrust->getVolumn();

			pRet->m_direction = entrust->getDirection();
			pRet->m_offsetType = entrust->getOffsetType();
			pRet->m_timeCond = entrust->getTimeCondition();
			pRet->m_priceType = entrust->getPriceType();
			pRet->m_strEntrustID = entrust->getEntrustID();
			pRet->m_strUserTag = entrust->getUserTag();

			pRet->m_uVolLeft = entrust->getVolumn();
		}

		return pRet;
	}

	void	setOrderDate(uint32_t uDate){m_uInsertDate = uDate;}
	void	setOrderTime(uint64_t uTime){m_uInsertTime = uTime;}
	void	setVolTraded(uint32_t vol){m_uVolTraded = vol;}
	void	setVolLeft(uint32_t vol){m_uVolLeft = vol;}
	
	void	setOrderID(const char* oid){m_strOrderID = oid;/*StrUtil::trim(m_strOrderID);*/}
	void	setOrderState(WTSOrderState os){m_orderState = os;}
	void	setOrderType(WTSOrderType ot){m_orderType = ot;}

	uint32_t getOrderDate() const{return m_uInsertDate;}
	uint64_t getOrderTime() const{return m_uInsertTime;}
	uint32_t getVolTraded() const{return m_uVolTraded;}
	uint32_t getVolLeft() const{return m_uVolLeft;}
	uint32_t getVolumn() const{return m_uVolLeft+m_uVolTraded;}
    
	WTSOrderState		getOrderState() const{return m_orderState;}
	WTSOrderType			getOrderType() const{return m_orderType;}
	const char*			getOrderID() const{return m_strOrderID.c_str();}

	void	setCode(const char* code){m_strCode = code;}
	void	setExchg(const char* exchg){ m_strExchg = exchg; }

	void	setStateMsg(const char* msg){m_strStateMsg = msg;}
	const char* getStateMsg() const{return m_strStateMsg.c_str();}

	bool	isAlive() const
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

	void	setError(bool bError = true){ m_bIsError = bError; }
	bool	isError() const{ return m_bIsError; }

private:
	uint32_t	m_uInsertDate;
	uint64_t	m_uInsertTime;
	uint32_t	m_uVolTraded;
	uint32_t	m_uVolLeft;
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
		, m_uPrice(0)
	{}
	virtual ~WTSTradeInfo(){}

public:
	static WTSTradeInfo* create(const char* code, const char* exchg = "")
	{
		WTSTradeInfo *pRet = new WTSTradeInfo;
		pRet->m_strExchg = exchg;
		pRet->m_strCode = code;

		return pRet;
	}

	void setTradeID(const char* tradeid){m_strTradeID = tradeid;/*StrUtil::trim(m_strTradeID);*/}
	void setRefOrder(const char* oid){m_strRefOrder = oid;/*StrUtil::trim(m_strRefOrder);*/}
	
	void setDirection(WTSDirectionType dType){m_direction = dType;}
	void setOffsetType(WTSOffsetType oType){m_offsetType = oType;}
	void setOrderType(WTSOrderType ot){m_orderType = ot;}
	void setTradeType(WTSTradeType tt){m_tradeType = tt;}

	void setVolumn(uint32_t volumn){m_uVolumn = volumn;}
	void setPrice(double price){ m_uPrice = price; }

	void setTradeDate(uint32_t uDate){m_uTradeDate = uDate;}
	void setTradeTime(uint64_t uTime){m_uTradeTime = uTime;}

	void setAmount(double amount){ m_uAmount = amount; }

	WTSDirectionType	getDirection() const{return m_direction;}
	WTSOffsetType		getOffsetType() const{return m_offsetType;}
	WTSOrderType		getOrderType() const{return m_orderType;}
	WTSTradeType		getTradeType() const{return m_tradeType;}

	uint32_t getVolumn() const{return m_uVolumn;}
	double getPrice() const{ return m_uPrice; }

	const char*	getCode() const{return m_strCode.c_str();}
	const char*	getExchg() const{ return m_strExchg.c_str(); }
	const char*	getTradeID() const{return m_strTradeID.c_str();}
	const char*	getRefOrder() const{return m_strRefOrder.c_str();}

	uint32_t getTradeDate() const{return m_uTradeDate;}
	uint64_t getTradeTime() const{return m_uTradeTime;}

	double getAmount() const{ return m_uAmount; }

	void setUserTag(const char* tag){m_strUserTag = tag;}
	const char* getUserTag() const{return m_strUserTag.c_str();}

protected:
	std::string	m_strExchg;		//市场
	std::string m_strCode;			//代码
	std::string m_strTradeID;		//成交单号
	std::string	m_strRefOrder;		//本地委托序列号
	std::string	m_strUserTag;		//用户标签

	uint32_t	m_uTradeDate;
	uint64_t	m_uTradeTime;
	uint32_t	m_uVolumn;
	double	m_uPrice;

	WTSDirectionType	m_direction;
	WTSOffsetType		m_offsetType;
	WTSOrderType		m_orderType;
	WTSTradeType		m_tradeType;

	double	m_uAmount;
};

//////////////////////////////////////////////////////////////////////////
//持仓信息
class WTSPositionItem : public WTSObject
{
public:
	static WTSPositionItem* create(const char* code, const char* currency, const char* exchg = "")
	{
		WTSPositionItem *pRet = new WTSPositionItem;
		pRet->m_strExchg = exchg;
		pRet->m_strCode = code;
		pRet->m_strCurrency = currency;

		return pRet;
	}

	void setDirection(WTSDirectionType dType){m_direction = dType;}
	void setPrePosition(uint32_t prePos){m_uPrePosition = prePos;}
	void setNewPosition(uint32_t newPos){m_uNewPosition = newPos;}
	void setAvailPrePos(uint32_t availPos){m_uAvailPrePos = availPos;}
	void setAvailNewPos(uint32_t availPos){m_uAvailNewPos = availPos;}
	void setPositionCost(double cost){m_dTotalPosCost = cost;}
	void setMargin(double margin){ m_dMargin = margin; }
	void setAvgPrice(double avgPrice){ m_dAvgPrice = avgPrice; }
	void setDynProfit(double profit){ m_dDynProfit = profit; }

	WTSDirectionType getDirection() const{return m_direction;}
	uint32_t	getPrePosition() const{return m_uPrePosition;}
	uint32_t	getNewPosition() const{return m_uNewPosition;}
	uint32_t	getAvailPrePos() const{return m_uAvailPrePos;}
	uint32_t	getAvailNewPos() const{return m_uAvailNewPos;}

	uint32_t	getTotalPosition() const{return m_uPrePosition + m_uNewPosition;}
	uint32_t	getAvailPosition() const{return m_uAvailPrePos + m_uAvailNewPos;}

	uint32_t	getFrozenPosition() const{return getTotalPosition()-getAvailPosition();}
	uint32_t	getFrozenNewPos() const{return m_uNewPosition-m_uAvailNewPos;}
	uint32_t	getFrozenPrePos() const{return m_uPrePosition-m_uAvailPrePos;}

	double		getPositionCost() const{ return m_dTotalPosCost; }
	double		getMargin() const{ return m_dMargin; }
	double		getAvgPrice() const{ return m_dAvgPrice; }
	double		getDynProfit() const{ return m_dDynProfit; }

	const char* getCode() const{ return m_strCode.c_str(); }
	const char* getCurrency() const{ return m_strCurrency.c_str(); }
	const char* getExchg() const{ return m_strExchg.c_str(); }

protected:
	WTSPositionItem()
		: m_direction(WDT_LONG)
		, m_uPrePosition(0)
		, m_uNewPosition(0)
		, m_uAvailPrePos(0)
		, m_uAvailNewPos(0)
		, m_dMargin(0)
		, m_dAvgPrice(0)
		, m_dDynProfit(0)
		, m_dTotalPosCost(0)
		, m_strCurrency("CNY")
	{}
	virtual ~WTSPositionItem(){}

	std::string		m_strExchg;
	std::string		m_strCode;
	std::string		m_strCurrency;

	WTSDirectionType	m_direction;//多空方向
	uint32_t	m_uPrePosition;		//昨仓
	uint32_t	m_uNewPosition;		//今仓
	uint32_t	m_uAvailPrePos;		//可平昨仓
	uint32_t	m_uAvailNewPos;		//可平今仓
	double		m_dTotalPosCost;	//持仓总成本
	double		m_dMargin;			//占用保证金
	double		m_dAvgPrice;		//持仓均价
	double		m_dDynProfit;		//浮动盈亏
};


//////////////////////////////////////////////////////////////////////////
//持仓明细
class WTSPositionDetail : public WTSObject
{
public:
	static WTSPositionDetail* create(const char* code = "", const char* exchg = "")
	{
		WTSPositionDetail* pRet = new WTSPositionDetail();
		pRet->m_strCode = code;
		pRet->m_strExchg = exchg;

		return pRet;
	}

	void setCode(const char* code){m_strCode = code;}
	void setTradeID(const char* tid){m_strTradeID = tid;}
	void setUserTag(const char* tag){m_strUserTag = tag;}

	void setDirection(WTSDirectionType dType){m_direction = dType;}

	void setOpenDate(uint32_t uDate){m_uOpenDate = uDate;}
	void setOpenTime(uint64_t uTime){m_uOpenTime = uTime;}

	void setVolumn(uint32_t vol){m_uVolumn = vol;}
	void setOpenPrice(double openpx){ m_uOpenPrice = openpx; }
	void setMargin(double margin){ m_uMargin = margin; }
	void setCloseVol(uint32_t closevol){m_uCloseVol = closevol;}
	void setCloseAmount(double closeamount){ m_uCloseAmount = closeamount; }
	void setCloseProfitByDate(double profitbydate){ m_iCloseProfitByDate = profitbydate; }
	void setCloseProfitByTrade(double profitbytrade){ m_iCloseProfitByTrade = profitbytrade; }
	void setPreSettlePx(double preSettlePx){ m_uPreSettlePx = preSettlePx; }

	const char* getCode() const{return m_strCode.c_str();}
	const char* getTradeID() const{return m_strTradeID.c_str();}
	const char* getUserTag() const{return m_strUserTag.c_str();}

	const char* getExchg() const{return m_strExchg.c_str();}

	WTSDirectionType getDirection() const{return m_direction;}

	uint32_t getOpenDate() const{return m_uOpenDate;}
	uint64_t getOpenTime() const{return m_uOpenTime;}

	double		getOpenPrice() const{ return m_uOpenPrice; }
	uint32_t	getVolumn() const{return m_uVolumn;}
	double		getMargin() const{ return m_uMargin; }
	uint32_t	getCloseVol() const{return m_uCloseVol;}
	double		getCloseAmount() const{ return m_uCloseAmount; }
	double		getCloseProfitByDate() const{ return m_iCloseProfitByDate; }
	double		getCloseProfitByTrade() const{ return m_iCloseProfitByTrade; }


protected:
	WTSPositionDetail()
		: m_direction(WDT_LONG)
		, m_uOpenDate(0)
		, m_uOpenTime(0)
		, m_uVolumn(0)
		, m_uOpenPrice(0)
		, m_uMargin(0)
		, m_uCloseVol(0)
		, m_uCloseAmount(0)
		, m_iCloseProfitByDate(0)
		, m_iCloseProfitByTrade(0)
		, m_uPreSettlePx(0)
	{}
	virtual ~WTSPositionDetail(){}

	std::string	m_strExchg;
	std::string	m_strCode;
	std::string	m_strTradeID;
	std::string m_strUserTag;

	WTSDirectionType	m_direction;//多空方向
	uint32_t	m_uOpenDate;
	uint64_t	m_uOpenTime;
	uint32_t	m_uVolumn;
	double		m_uOpenPrice;
	double		m_uMargin;
	uint32_t	m_uCloseVol;
	double		m_uCloseAmount;
	double		m_iCloseProfitByDate;
	double		m_iCloseProfitByTrade;
	double		m_uPreSettlePx;
};


//////////////////////////////////////////////////////////////////////////
//投资者信息
class WTSInvestorInfo : public WTSObject
{
protected:
	WTSInvestorInfo():m_uState(0){}
	~WTSInvestorInfo(){}

public:
	static WTSInvestorInfo* create(){return new WTSInvestorInfo;}

	void	setDescription(const char* desc){m_strDescription = desc;}
	void	setUsername(const char* username){m_strUserName = username;}
	void	setState(uint32_t uState){m_uState = uState;}
	void	setExtInfo(const char* key, const char* val){ m_mapExts[key] = val; }

	const char* getUsername() const{return m_strUserName.c_str();}
	const char* getDescription() const{return m_strDescription.c_str();}
	uint32_t	getState() const{return m_uState;}

	const char* getExtInfo(const char* key)
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
	static WTSAccountInfo* create(){return new WTSAccountInfo;}

	void	setDescription(const char* desc){m_strDescription = desc;}
	void	setCurrency(const char* currency){ m_strCurrency = currency; }

	void	setBalance(double balance){m_uBalance = balance;}
	void	setPreBalance(double prebalance){m_uPreBalance = prebalance;}
	void	setMargin(double margin){m_uMargin = margin;}
	void	setFrozenMargin(double frozenmargin){m_uFrozenMargin = frozenmargin;}
	void	setCloseProfit(double closeprofit){m_iCloseProfit = closeprofit;}
	void	setDynProfit(double dynprofit){m_iDynProfit = dynprofit;}
	void	setDeposit(double deposit){m_uDeposit = deposit;}
	void	setWithdraw(double withdraw){m_uWithdraw = withdraw;}
	void	setCommission(double commission){m_uCommission = commission;}
	void	setFrozenCommission(double frozencommission){m_uFrozenCommission = frozencommission;}
	void	setAvailable(double available){m_uAvailable = available;}

	double	getBalance() const{return m_uBalance;}
	double	getPreBalance() const{return m_uPreBalance;}
	double	getMargin() const{return m_uMargin;}
	double	getFrozenMargin() const{return m_uFrozenMargin;}
	double	getCloseProfit() const{return m_iCloseProfit;}
	double	getDynProfit() const{return m_iDynProfit;}
	double	getDeposit() const{return m_uDeposit;}
	double	getWithdraw() const{return m_uWithdraw;}
	double	getCommission() const{return m_uCommission;}
	double	getFrozenCommission() const{return m_uFrozenCommission;}
	double	getAvailable() const{return m_uAvailable;}

	const char* getDescription() const{return m_strDescription.c_str();}
	const char* getCurrency() const{ return m_strCurrency.c_str(); }

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


//////////////////////////////////////////////////////////////////////////
//预埋单触发条件
class WTSEntrustCondition : public WTSObject
{
protected:
	WTSEntrustCondition():m_pRefEntrust(NULL),m_bFired(false){}
	virtual ~WTSEntrustCondition(){}

public:
	static WTSEntrustCondition* create(const char* instrument, double target, WTSCompareType compType = WCT_Equal, WTSCompareField cField = WCF_NONE)
	{
		WTSEntrustCondition* ret = new WTSEntrustCondition();
		ret->m_strInstrument = instrument;
		ret->m_dTarget = target;
		ret->m_compareType = compType;
		ret->m_compareField = cField;

		return ret;
	}

	uint32_t makeReferenceID()
	{
		static uint32_t global_refid = (uint32_t)time(NULL);
		m_uReferenceID = global_refid++;

		return m_uReferenceID;
	}

	void setReferenceID(uint32_t uRefID)
	{
		m_uReferenceID = uRefID;
	}

	/*
	 *	设置关联委托对象
	 *	增加委托的引用计数
	*/
	void setEntrust(WTSEntrust* entrust, bool bAutoRetain = true)
	{
		if(m_pRefEntrust != NULL)
		{
			m_pRefEntrust->release();
			m_pRefEntrust = NULL;
		}

		m_pRefEntrust = entrust;
		if(bAutoRetain && m_pRefEntrust)
			m_pRefEntrust->retain();
	}

	void reset()
	{
		m_strInstrument = "";
		m_dTarget = 0;
		
		if(m_pRefEntrust != NULL)
		{
			m_pRefEntrust->release();
			m_pRefEntrust = NULL;
		}
	}

	bool isMatched(const char* instrument, double uRealValue)
	{
		if(m_strInstrument.compare(instrument) != 0)
			return false;

		switch(m_compareType)
		{
		case WCT_Equal:
			return (uRealValue == m_dTarget);
		case WCT_Larger:
			return (uRealValue > m_dTarget);
		case WCT_LargerOrEqual:
			return (uRealValue >= m_dTarget);
		case WCT_Smaller:
			return (uRealValue < m_dTarget);
		case WCT_SmallerOrEqual:
			return (uRealValue <= m_dTarget);
		default:
			return false;
		}
	}

	void setCompareType(WTSCompareType cType){m_compareType = cType;}
	void setCompareField(WTSCompareField cField){m_compareField = cField;}
	void setTarget(double iTarget){ m_dTarget = iTarget; }

	double getTarget() const{ return m_dTarget; }
	const char* getInstrument() const{return m_strInstrument.c_str();}
	WTSCompareType getCompareType() const{return m_compareType;}
	WTSCompareField getCompareField() const{return m_compareField;}
	uint32_t getReferenceID(){return m_uReferenceID;}
	WTSEntrust* getEntrust(){return m_pRefEntrust;}

	virtual void release()
	{
		if(m_pRefEntrust != NULL && retainCount() == 1)
		{
			m_pRefEntrust->release();
			m_pRefEntrust = NULL;
		}

		WTSObject::release();
	}

	void		setOrderTime(uint64_t uTime){m_uOrderTime = uTime;}
	uint64_t	getOrderTime() const{return m_uOrderTime;}

	void		fire(bool fired = true){m_bFired = fired;}
	bool		fired() const{return m_bFired;}

protected:
	std::string		m_strInstrument;	//合约代码
	double			m_dTarget;			//目标值
	WTSCompareType	m_compareType;		//比较类型
	WTSCompareField	m_compareField;		//条件类型，价格条件or成交量条件
	WTSEntrust*		m_pRefEntrust;
	uint32_t		m_uReferenceID;
	uint64_t		m_uOrderTime;

	bool			m_bFired;
};

NS_OTP_END
