#pragma once
#include "WTSTypes.h"

#include <map>

USING_NS_WTP;

typedef struct _Contract
{
	std::string	m_strCode;
	std::string	m_strExchg;
	std::string	m_strName;
	std::string	m_strProduct;

	uint32_t	m_maxMktQty;
	uint32_t	m_maxLmtQty;
	uint32_t	m_minMktQty;
	uint32_t	m_minLmtQty;

	uint32_t	m_uOpenDate;
	uint32_t	m_uExpireDate;

	double		m_dLongMarginRatio;
	double		m_dShortMarginRatio;

	OptionType	m_optType;
	std::string m_strUnderlying;
	double		m_strikePrice;
	double		m_dUnderlyingScale;
} Contract;
typedef std::map<std::string, Contract> ContractMap;

typedef struct _Commodity
{
	std::string	m_strName;
	std::string	m_strExchg;
	std::string	m_strProduct;
	std::string	m_strCurrency;
	std::string m_strSession;

	uint32_t	m_uVolScale;
	double		m_fPriceTick;
	uint32_t	m_uPrecision;

	ContractCategory	m_ccCategory;
	CoverMode			m_coverMode;
	PriceMode			m_priceMode;
	TradingMode			m_tradeMode;

} Commodity;
typedef std::map<std::string, Commodity> CommodityMap;