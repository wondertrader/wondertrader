/*!
 * \file WTSContractInfo.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt品种信息、合约信息定义文件
 */
#pragma once
#include "WTSObject.hpp"
#include "WTSTypes.h"
#include "FasterDefs.h"
#include <string>
#include <sstream>

NS_WTP_BEGIN
class WTSSessionInfo;

class WTSCommodityInfo: public WTSObject
{
public:
	static WTSCommodityInfo* create(const char* pid, const char* name, const char* exchg, const char* session, const char* trdtpl, const char* currency = "CNY")
	{
		WTSCommodityInfo* ret = new WTSCommodityInfo;
		ret->m_strName = name;
		ret->m_strExchg = exchg;
		ret->m_strProduct = pid;
		ret->m_strCurrency = currency;
		ret->m_strSession = session;
		ret->m_strTrdTpl = trdtpl;

		std::stringstream ss;
		ss << exchg << "." << pid;
		ret->m_strFullPid = ss.str();

		return ret;
	}

	inline void	setVolScale(uint32_t volScale){ m_uVolScale = volScale; }
	inline void	setPriceTick(double pxTick){ m_dPriceTick = pxTick; }
	inline void	setCategory(ContractCategory cat){ m_ccCategory = cat; }
	inline void	setCoverMode(CoverMode cm){ m_coverMode = cm; }
	inline void	setPriceMode(PriceMode pm){ m_priceMode = pm; }
	inline void	setTradingMode(TradingMode tm) { m_tradeMode = tm; }

	inline bool canShort() const { return m_tradeMode == TM_Both; }
	inline bool isT1() const { return m_tradeMode == TM_LongT1; }

	inline const char* getName()	const{ return m_strName.c_str(); }
	inline const char* getExchg()	const{ return m_strExchg.c_str(); }
	inline const char* getProduct()	const{ return m_strProduct.c_str(); }
	inline const char* getCurrency()	const{ return m_strCurrency.c_str(); }
	inline const char* getSession()	const{ return m_strSession.c_str(); }
	inline const char* getTradingTpl()	const{ return m_strTrdTpl.c_str(); }
	inline const char* getFullPid()	const{ return m_strFullPid.c_str(); }

	inline uint32_t	getVolScale()	const{ return m_uVolScale; }
	inline double	getPriceTick()	const{ return m_dPriceTick; }
	//inline uint32_t	getPrecision()	const{ return m_uPrecision; }

	inline ContractCategory		getCategoty() const{ return m_ccCategory; }
	inline CoverMode			getCoverMode() const{ return m_coverMode; }
	inline PriceMode			getPriceMode() const{ return m_priceMode; }
	inline TradingMode			getTradingMode() const { return m_tradeMode; }

	inline void		addCode(const char* code){ m_setCodes.insert(code); }
	inline const CodeSet& getCodes() const{ return m_setCodes; }

	inline void	setLotsTick(double lotsTick){ m_dLotTick = lotsTick; }
	inline void	setMinLots(double minLots) { m_dMinLots = minLots; }

	inline bool isOption() const
	{
		return (m_ccCategory == CC_FutOption || m_ccCategory == CC_ETFOption || m_ccCategory == CC_SpotOption);
	}

	inline bool isFuture() const
	{
		return m_ccCategory == CC_Future;
	}

	inline bool isStock() const
	{
		return m_ccCategory == CC_Stock;
	}

	inline double	getLotsTick() const { return m_dLotTick; }
	inline double	getMinLots() const { return m_dMinLots; }

	inline void		setSessionInfo(WTSSessionInfo* sInfo) { m_pSession = sInfo; }
	inline WTSSessionInfo* getSessionInfo() const { return m_pSession; }

private:
	std::string	m_strName;		//品种名称
	std::string	m_strExchg;		//交易所代码
	std::string	m_strProduct;	//品种ID
	std::string	m_strCurrency;	//币种
	std::string m_strSession;	//交易时间模板
	std::string m_strTrdTpl;	//节假日模板
	std::string m_strFullPid;	//全品种ID，如CFFEX.IF

	uint32_t	m_uVolScale;	//合约放大倍数
	double		m_dPriceTick;	//最小价格变动单位
	//uint32_t	m_uPrecision;	//价格精度

	double		m_dLotTick;		//数量精度
	double		m_dMinLots;		//最小交易数量

	ContractCategory	m_ccCategory;	//品种分类，期货、股票、期权等
	CoverMode			m_coverMode;	//平仓类型
	PriceMode			m_priceMode;	//价格类型
	TradingMode			m_tradeMode;	//交易类型

	CodeSet				m_setCodes;

	WTSSessionInfo*		m_pSession;
};

class WTSContractInfo :	public WTSObject
{
public:
	static WTSContractInfo* create(const char* code, const char* name, const char* exchg, const char* pid)
	{
		WTSContractInfo* ret = new WTSContractInfo;
		ret->m_strCode = code;
		ret->m_strName = name;
		ret->m_strProduct = pid;
		ret->m_strExchg = exchg;

		std::stringstream ss;
		ss << exchg << "." << code;
		ret->m_strFullCode = ss.str();

		std::stringstream sss;
		sss << exchg << "." << pid;
		ret->m_strFullPid = sss.str();

		return ret;
	}


	inline void	setVolumeLimits(uint32_t maxMarketVol, uint32_t maxLimitVol)
	{
		m_maxMktQty = maxMarketVol;
		m_maxLmtQty = maxLimitVol;
	}

	inline const char* getCode()	const{return m_strCode.c_str();}
	inline const char* getExchg()	const{return m_strExchg.c_str();}
	inline const char* getName()	const{return m_strName.c_str();}
	inline const char* getProduct()	const{return m_strProduct.c_str();}

	inline const char* getFullCode()	const{ return m_strFullCode.c_str(); }
	inline const char* getFullPid()	const{ return m_strFullPid.c_str(); }

	inline uint32_t	getMaxMktVol() const{ return m_maxMktQty; }
	inline uint32_t	getMaxLmtVol() const{ return m_maxLmtQty; }

	inline void setCommInfo(WTSCommodityInfo* commInfo) { m_commInfo = commInfo; }
	inline WTSCommodityInfo* getCommInfo() const { return m_commInfo; }

protected:
	WTSContractInfo():m_commInfo(NULL){}
	virtual ~WTSContractInfo(){}

private:
	std::string	m_strCode;
	std::string	m_strExchg;
	std::string	m_strName;
	std::string	m_strProduct;

	std::string m_strFullPid;
	std::string m_strFullCode;

	uint32_t	m_maxMktQty;
	uint32_t	m_maxLmtQty;

	WTSCommodityInfo*	m_commInfo;
};


NS_WTP_END