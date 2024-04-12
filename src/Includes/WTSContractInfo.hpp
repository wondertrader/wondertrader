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

NS_WTP_BEGIN
class WTSSessionInfo;

class WTSCommodityInfo: public WTSObject
{
public:
	static WTSCommodityInfo* create(const char* pid, const char* name, const char* exchg, const char* session, const char* trdtpl, const char* currency = "CNY") noexcept
	{
		WTSCommodityInfo* ret = new WTSCommodityInfo;
		wt_strcpy(ret->m_strName, name);
		wt_strcpy(ret->m_strExchg, exchg);
		wt_strcpy(ret->m_strProduct, pid);
		wt_strcpy(ret->m_strCurrency, currency);
		wt_strcpy(ret->m_strSession, session);
		wt_strcpy(ret->m_strTrdTpl, trdtpl);

		sprintf(ret->m_strFullPid, "%s.%s", exchg, pid);

		return ret;
	}

	constexpr inline void	setVolScale(uint32_t volScale) noexcept { m_uVolScale = volScale; }
	constexpr inline void	setPriceTick(double pxTick) noexcept { m_dPriceTick = pxTick; }
	constexpr inline void	setCategory(ContractCategory cat) noexcept { m_ccCategory = cat; }
	constexpr inline void	setCoverMode(CoverMode cm) noexcept { m_coverMode = cm; }
	constexpr inline void	setPriceMode(PriceMode pm) noexcept { m_priceMode = pm; }
	constexpr inline void	setTradingMode(TradingMode tm) noexcept { m_tradeMode = tm; }

	constexpr inline bool canShort() const  noexcept { return m_tradeMode == TM_Both; }
	constexpr inline bool isT1() const  noexcept { return m_tradeMode == TM_LongT1; }

	constexpr inline const char* getName()	const noexcept { return m_strName; }
	constexpr inline const char* getExchg()	const noexcept { return m_strExchg; }
	constexpr inline const char* getProduct()	const noexcept { return m_strProduct; }
	constexpr inline const char* getCurrency()	const noexcept { return m_strCurrency; }
	constexpr inline const char* getSession()	const noexcept { return m_strSession; }
	constexpr inline const char* getTradingTpl() const noexcept { return m_strTrdTpl; }
	constexpr inline const char* getFullPid()	const noexcept { return m_strFullPid; }

	constexpr inline uint32_t	getVolScale()	const noexcept { return m_uVolScale; }
	constexpr inline double	getPriceTick()	const noexcept { return m_dPriceTick; }
	//inline uint32_t	getPrecision()	const{ return m_uPrecision; }

	constexpr inline ContractCategory		getCategoty() const noexcept { return m_ccCategory; }
	constexpr inline CoverMode			getCoverMode() const noexcept { return m_coverMode; }
	constexpr inline PriceMode			getPriceMode() const noexcept { return m_priceMode; }
	constexpr inline TradingMode			getTradingMode() const  noexcept { return m_tradeMode; }

	inline void		addCode(const char* code) noexcept { m_setCodes.insert(code); }
	inline const CodeSet& getCodes() const noexcept { return m_setCodes; }

	constexpr inline void	setLotsTick(double lotsTick) noexcept { m_dLotTick = lotsTick; }
	constexpr inline void	setMinLots(double minLots)  noexcept { m_dMinLots = minLots; }

	constexpr inline bool isOption() const noexcept
	{
		return (m_ccCategory == CC_FutOption || m_ccCategory == CC_ETFOption || m_ccCategory == CC_SpotOption);
	}

	constexpr inline bool isFuture() const noexcept
	{
		return m_ccCategory == CC_Future;
	}

	constexpr inline bool isStock() const noexcept
	{
		return m_ccCategory == CC_Stock;
	}

	constexpr inline double	getLotsTick() const noexcept { return m_dLotTick; }
	constexpr inline double	getMinLots() const  noexcept { return m_dMinLots; }

	constexpr inline void		setSessionInfo(WTSSessionInfo* sInfo) noexcept { m_pSession = sInfo; }
	constexpr inline WTSSessionInfo* getSessionInfo() const  noexcept { return m_pSession; }

	constexpr inline void		setFeeRates(double open, double close, double closeToday, bool byVolume) noexcept
	{
		m_dOpenFee = open;
		m_dCloseFee = close;
		m_dCloseTFee = closeToday;
		m_nFeeAlg = byVolume ? 0 : 1;
	}

	constexpr inline void	setMarginRate(double rate) noexcept { m_dMarginRate = rate; }
	constexpr inline double	getMarginRate() const  noexcept { return m_dMarginRate; }

	constexpr inline double	calcFee(double price, double qty, uint32_t offset) noexcept
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
	char m_strName[64];		//品种名称
	char m_strExchg[64];		//交易所代码
	char m_strProduct[64];	//品种ID
	char m_strCurrency[64];	//币种
	char m_strSession[64];	//交易时间模板
	char m_strTrdTpl[64];	//节假日模板
	char m_strFullPid[64];	//全品种ID，如CFFEX.IF

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

	double	m_dOpenFee		= 0.0;	//开仓手续费
	double	m_dCloseFee		= 0.0;	//平仓手续费
	double	m_dCloseTFee	= 0.0;	//平今手续费
	int		m_nFeeAlg		= -1;	//手续费算法，默认为-1，不计算,0是按成交量，1为按成交额
	double	m_dMarginRate	= 0.0;	//保证金率
};

class WTSContractInfo :	public WTSObject
{
public:
	static WTSContractInfo* create(const char* code, const char* name, const char* exchg, const char* pid)
	{
		WTSContractInfo* ret = new WTSContractInfo;
		wt_strcpy(ret->m_strCode, code);
		wt_strcpy(ret->m_strName, name);
		wt_strcpy(ret->m_strProduct, pid);
		wt_strcpy(ret->m_strExchg, exchg);

		sprintf(ret->m_strFullCode, "%s.%s", exchg, code);
		sprintf(ret->m_strFullPid, "%s.%s", exchg, pid);

		return ret;
	}

	inline void setAltCode(const char* altCode) noexcept
	{
		wt_strcpy(m_strAltCode, altCode);
	}

	constexpr inline void	setVolumeLimits(uint32_t maxMarketVol, uint32_t maxLimitVol, uint32_t minMarketVol = 1, uint32_t minLimitVol = 1) noexcept
	{
		m_maxMktQty = maxMarketVol;
		m_maxLmtQty = maxLimitVol;

		m_minLmtQty = minLimitVol;
		m_minMktQty = minMarketVol;
	}

	constexpr inline void setDates(uint32_t openDate, uint32_t expireDate) noexcept
	{
		m_openDate = openDate;
		m_expireDate = expireDate;
	}

	constexpr inline void setMarginRatios(double longRatio, double shortRatio, uint32_t flag = 0) noexcept
	{
		m_lMarginRatio = longRatio;
		m_sMarginRatio = shortRatio;
		m_uMarginFlag = flag;
	}

	constexpr inline const char* getCode()	const noexcept {return m_strCode;}
	constexpr inline const char* getAltCode()	const noexcept { return m_strAltCode; }
	constexpr inline const char* getExchg()	const noexcept {return m_strExchg;}
	constexpr inline const char* getName()	const noexcept {return m_strName;}
	constexpr inline const char* getProduct()	const noexcept {return m_strProduct;}

	constexpr inline const char* getFullCode()	const noexcept { return m_strFullCode; }
	constexpr inline const char* getFullPid()	const noexcept { return m_strFullPid; }

	constexpr inline uint32_t	getMaxMktVol() const noexcept { return m_maxMktQty; }
	constexpr inline uint32_t	getMaxLmtVol() const noexcept { return m_maxLmtQty; }
	constexpr inline uint32_t	getMinMktVol() const  noexcept { return m_minMktQty; }
	constexpr inline uint32_t	getMinLmtVol() const  noexcept { return m_minLmtQty; }

	constexpr inline uint32_t	getOpenDate() const  noexcept { return m_openDate; }
	constexpr inline uint32_t	getExpireDate() const  noexcept { return m_expireDate; }

	constexpr inline double	getLongMarginRatio() const noexcept {
		if (m_uMarginFlag == 1)
			return m_lMarginRatio;

		double commRate = m_commInfo->getMarginRate();
		return commRate == 0.0 ? m_lMarginRatio : m_commInfo->getMarginRate();
	}

	constexpr inline double	getShortMarginRatio() const noexcept {
		if (m_uMarginFlag == 1)
			return m_sMarginRatio;

		double commRate = m_commInfo->getMarginRate();
		return commRate == 0.0 ? m_sMarginRatio : m_commInfo->getMarginRate();
	}

	constexpr inline void setCommInfo(WTSCommodityInfo* commInfo) noexcept { m_commInfo = commInfo; }
	constexpr inline WTSCommodityInfo* getCommInfo() const noexcept { return m_commInfo; }

	constexpr inline void		setFeeRates(double open, double close, double closeToday, bool byVolume) noexcept
	{
		m_dOpenFee = open;
		m_dCloseFee = close;
		m_dCloseTFee = closeToday;
		m_nFeeAlg = byVolume ? 0 : 1;
	}

	constexpr inline double	calcFee(double price, double qty, uint32_t offset) noexcept
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

	inline void setHotFlag(uint32_t hotFlag, const char* hotCode = "") noexcept
	{ 
		m_uHotFlag = hotFlag; 
		wt_strcpy(m_strHotCode, hotCode);
	}
	constexpr inline bool isFlat() const  noexcept { return m_uHotFlag == 0; }
	constexpr inline bool isHot() const  noexcept { return m_uHotFlag == 1; }
	constexpr inline bool isSecond() const  noexcept { return m_uHotFlag == 2; }
	constexpr inline const char* getHotCode() const  noexcept { return m_strHotCode; }

	constexpr inline void	setTotalIndex(uint32_t idx) noexcept { m_uTotalIdx = idx; }
	constexpr inline uint32_t getTotalIndex() const noexcept { return m_uTotalIdx; }

	template<uint32_t INDEX = 0>
	constexpr inline void setExtData(void* pExtData) noexcept { m_pExtData[INDEX] = pExtData; }

	template<typename T, uint32_t INDEX = 0>
	constexpr inline T*	getExtData() noexcept { return static_cast<T*>(m_pExtData[INDEX]); }

protected:
	WTSContractInfo()
		: m_commInfo(NULL), m_openDate(19900101), m_expireDate(30991231)
		, m_lMarginRatio(0), m_sMarginRatio(0), m_nFeeAlg(-1), m_uMarginFlag(0)
		, m_uHotFlag(0), m_uTotalIdx(UINT_MAX){}
	virtual ~WTSContractInfo(){}

private:
	char	m_strCode[32];
	char	m_strExchg[32];
	char	m_strName[64];
	char	m_strProduct[32];

	char	m_strAltCode[32];	//别称代码，这个主要针对郑商所的 By Wesley @ 2024.04.12

	char	m_strFullPid[64];
	char	m_strFullCode[64];

	uint32_t	m_maxMktQty;
	uint32_t	m_maxLmtQty;
	uint32_t	m_minMktQty;
	uint32_t	m_minLmtQty;

	uint32_t	m_openDate;		//上市日期
	uint32_t	m_expireDate;	//交割日

	double		m_lMarginRatio	= 0.0;	//交易所多头保证金率
	double		m_sMarginRatio	= 0.0;	//交易所空头保证金率
	uint32_t	m_uMarginFlag	= 0;	//0-合约信息读取的，1-手工设置的

	double		m_dOpenFee		= 0.0;	//开仓手续费
	double		m_dCloseFee		= 0.0;	//平仓手续费
	double		m_dCloseTFee	= 0.0;	//平今手续费
	int			m_nFeeAlg		= -1;	//手续费算法，默认为-1，不计算,0是按成交量，1为按成交额

	WTSCommodityInfo*	m_commInfo;
	uint32_t	m_uHotFlag;
	char		m_strHotCode[64];

	uint32_t	m_uTotalIdx;				//合约全局索引，每次启动可能不同，只能在内存里用
	void*		m_pExtData[16] = { 0 };		//扩展数据，主要是绑定一些和合约相关的数据，这样可以避免在很多地方建map，导致多次查找
};


NS_WTP_END