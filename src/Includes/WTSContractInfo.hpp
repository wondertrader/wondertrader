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

	inline void		setFeeRates(double open, double close, double closeToday, bool byVolume)
	{
		m_dOpenFee = open;
		m_dCloseFee = close;
		m_dCloseTFee = closeToday;
		m_nFeeAlg = byVolume ? 0 : 1;
	}

	inline void		setMarginRate(double rate) { m_dMarginRate = rate; }
	inline double	getMarginRate() const { return m_dMarginRate; }

	inline double	calcFee(double price, double qty, uint32_t offset)
	{
		if (m_nFeeAlg == -1)
			return 0.0;

		double ret = 0.0;
		if (m_nFeeAlg == 0)
		{
			switch (offset)
			{
			case 0: ret = m_dOpenFee * qty; break;
			case 1: ret = m_dCloseFee * qty; break;
			case 2: ret = m_dCloseTFee * qty; break;
			default: ret = 0.0; break;
			}
		}
		else if(m_nFeeAlg == 1)
		{
			double amount = price * qty * m_uVolScale;
			switch (offset)
			{
			case 0: ret = m_dOpenFee * amount; break;
			case 1: ret = m_dCloseFee * amount; break;
			case 2: ret = m_dCloseTFee * amount; break;
			default: ret = 0.0; break;
			}
		}

		return (int32_t)(ret * 100 + 0.5) / 100.0;
	}

protected:
	WTSCommodityInfo():m_nFeeAlg(-1){}
	virtual ~WTSCommodityInfo() {}

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

	double		m_dLotTick;		//数量精度
	double		m_dMinLots;		//最小交易数量

	ContractCategory	m_ccCategory;	//品种分类，期货、股票、期权等
	CoverMode			m_coverMode;	//平仓类型
	PriceMode			m_priceMode;	//价格类型
	TradingMode			m_tradeMode;	//交易类型

	CodeSet				m_setCodes;

	WTSSessionInfo*		m_pSession;

	double	m_dOpenFee;		//开仓手续费
	double	m_dCloseFee;	//平仓手续费
	double	m_dCloseTFee;	//平今手续费
	int		m_nFeeAlg;		//手续费算法，默认为-1，不计算,0是按成交量，1为按成交额
	double	m_dMarginRate;	//保证金率
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


	inline void	setVolumeLimits(uint32_t maxMarketVol, uint32_t maxLimitVol, uint32_t minMarketVol = 1, uint32_t minLimitVol = 1)
	{
		m_maxMktQty = maxMarketVol;
		m_maxLmtQty = maxLimitVol;

		m_minLmtQty = minLimitVol;
		m_minMktQty = minMarketVol;
	}

	inline void setDates(uint32_t openDate, uint32_t expireDate)
	{
		m_openDate = openDate;
		m_expireDate = expireDate;
	}

	inline void setMarginRatios(double longRatio, double shortRatio, uint32_t flag = 0)
	{
		m_lMarginRatio = longRatio;
		m_sMarginRatio = shortRatio;
		m_uMarginFlag = flag;
	}

	inline const char* getCode()	const{return m_strCode.c_str();}
	inline const char* getExchg()	const{return m_strExchg.c_str();}
	inline const char* getName()	const{return m_strName.c_str();}
	inline const char* getProduct()	const{return m_strProduct.c_str();}

	inline const char* getFullCode()	const{ return m_strFullCode.c_str(); }
	inline const char* getFullPid()	const{ return m_strFullPid.c_str(); }

	inline uint32_t	getMaxMktVol() const{ return m_maxMktQty; }
	inline uint32_t	getMaxLmtVol() const{ return m_maxLmtQty; }
	inline uint32_t	getMinMktVol() const { return m_minMktQty; }
	inline uint32_t	getMinLmtVol() const { return m_minLmtQty; }

	inline uint32_t	getOpenDate() const { return m_openDate; }
	inline uint32_t	getExpireDate() const { return m_expireDate; }

	inline double	getLongMarginRatio() const { 
		if (m_uMarginFlag == 1)
			return m_lMarginRatio;

		static double commRate = m_commInfo->getMarginRate();
		return commRate == 0.0 ? m_lMarginRatio : m_commInfo->getMarginRate();
	}

	inline double	getShortMarginRatio() const {
		if (m_uMarginFlag == 1)
			return m_sMarginRatio;

		static double commRate = m_commInfo->getMarginRate();
		return commRate == 0.0 ? m_sMarginRatio : m_commInfo->getMarginRate();
	}

	inline void setCommInfo(WTSCommodityInfo* commInfo) { m_commInfo = commInfo; }
	inline WTSCommodityInfo* getCommInfo() const { return m_commInfo; }

	inline void		setFeeRates(double open, double close, double closeToday, bool byVolume)
	{
		m_dOpenFee = open;
		m_dCloseFee = close;
		m_dCloseTFee = closeToday;
		m_nFeeAlg = byVolume ? 0 : 1;
	}

	inline double	calcFee(double price, double qty, uint32_t offset)
	{
		//如果合约没有手续费率，则调用品种的手续费率
		if (m_nFeeAlg == -1)
			return m_commInfo->calcFee(price, qty, offset);

		double ret = 0.0;
		if (m_nFeeAlg == 0)
		{
			switch (offset)
			{
			case 0: ret = m_dOpenFee * qty; break;
			case 1: ret = m_dCloseFee * qty; break;
			case 2: ret = m_dCloseTFee * qty; break;
			default: ret = 0.0; break;
			}
		}
		else if (m_nFeeAlg == 1)
		{
			double amount = price * qty * m_commInfo->getVolScale();
			switch (offset)
			{
			case 0: ret = m_dOpenFee * amount; break;
			case 1: ret = m_dCloseFee * amount; break;
			case 2: ret = m_dCloseTFee * amount; break;
			default: ret = 0.0; break;
			}
		}

		return (int32_t)(ret * 100 + 0.5) / 100.0;
	}

	inline void setHotFlag(uint32_t hotFlag, const char* hotCode = "") 
	{ 
		m_uHotFlag = hotFlag; 
		m_strHotCode = hotCode;
	}
	inline bool isFlat() const { return m_uHotFlag == 0; }
	inline bool isHot() const { return m_uHotFlag == 1; }
	inline bool isSecond() const { return m_uHotFlag == 2; }
	inline const char* getHotCode() const { return m_strHotCode.c_str(); }

	inline void	setTotalIndex(uint32_t idx) noexcept { m_uTotalIdx = idx; }
	inline uint32_t getTotalIndex() const noexcept { return m_uTotalIdx; }

	inline void setExtData(void* pExtData) noexcept { m_pExtData = pExtData; }
	template<typename T>
	inline T*	getExtData() noexcept { return static_cast<T*>(m_pExtData); }

protected:
	WTSContractInfo()
		: m_commInfo(NULL), m_openDate(19900101), m_expireDate(30991231)
		, m_lMarginRatio(0), m_sMarginRatio(0), m_nFeeAlg(-1), m_uMarginFlag(0)
		, m_uHotFlag(0), m_uTotalIdx(UINT_MAX), m_pExtData(NULL){}
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
	uint32_t	m_minMktQty;
	uint32_t	m_minLmtQty;

	uint32_t	m_openDate;		//上市日期
	uint32_t	m_expireDate;	//交割日

	double		m_lMarginRatio;	//交易所多头保证金率
	double		m_sMarginRatio;	//交易所空头保证金率
	uint32_t	m_uMarginFlag;	//0-合约信息读取的，1-手工设置的

	double		m_dOpenFee;		//开仓手续费
	double		m_dCloseFee;	//平仓手续费
	double		m_dCloseTFee;	//平今手续费
	int			m_nFeeAlg;		//手续费算法，默认为-1，不计算,0是按成交量，1为按成交额

	WTSCommodityInfo*	m_commInfo;
	uint32_t	m_uHotFlag;
	std::string	m_strHotCode;

	uint32_t	m_uTotalIdx;	//合约全局索引，每次启动可能不同，只能在内存里用
	void*		m_pExtData;		//扩展数据，主要是绑定一些和合约相关的数据，这样可以避免在很多地方建map，导致多次查找
};


NS_WTP_END