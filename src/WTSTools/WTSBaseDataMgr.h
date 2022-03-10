/*!
 * \file WTSBaseDataMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 基础数据管理器实现
 */
#pragma once
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSCollection.hpp"
#include "../Includes/FasterDefs.h"

#include <boost/container_hash/hash.hpp>

USING_NS_WTP;
typedef struct _Longkey //char[32]
{
	uint64_t	x[4];

	_Longkey()
	{
		memset(this, 0, sizeof(_Longkey));
	}

	bool operator ==(const _Longkey& b) const
	{
		return (x[0] == b.x[0] && x[1] == b.x[1] && x[2] == b.x[2] && x[3] == b.x[3]);
	}
} LongKey;

typedef struct _ShortKey //char[32]
{
	uint64_t	x[2];

	_ShortKey()
	{
		memset(this, 0, sizeof(_ShortKey));
	}

	bool operator ==(const _ShortKey& b) const
	{
		return (x[0] == b.x[0] && x[1] == b.x[1]);
	}
} ShortKey;


namespace std
{
	template<>
	struct hash<LongKey>
	{
		size_t operator()(const LongKey& key) const
		{
			size_t ret = 17;
			ret = ret * 31 + hash<uint64_t>()(key.x[0]);
			ret = ret * 31 + hash<uint64_t>()(key.x[1]);
			ret = ret * 31 + hash<uint64_t>()(key.x[2]);
			ret = ret * 31 + hash<uint64_t>()(key.x[3]);
			return ret;
		}
	};

	template<>
	struct hash<ShortKey>
	{
		size_t operator()(const ShortKey& key) const
		{
			size_t ret = 17;
			ret = ret * 31 + hash<uint64_t>()(key.x[0]);
			ret = ret * 31 + hash<uint64_t>()(key.x[1]);
			return ret;
		}
	};
}

inline LongKey makeLongKey(const char* s)
{
	LongKey key;
	strcpy((char*)&key, s);
	return key;
}

inline ShortKey makeShortKey(const char* s)
{
	ShortKey key;
	strcpy((char*)&key, s);
	return key;
}

typedef faster_hashmap<std::string, TradingDayTpl>	TradingDayTplMap;

typedef WTSHashMap<LongKey>		WTSContractList;
typedef WTSHashMap<ShortKey>	WTSExchgContract;
typedef WTSHashMap<LongKey>		WTSContractMap;

typedef WTSHashMap<std::string>		WTSSessionMap;
typedef WTSHashMap<std::string>		WTSCommodityMap;

typedef faster_hashset<std::string> CodeSet;
typedef faster_hashmap<std::string, CodeSet> SessionCodeMap;


class WTSBaseDataMgr : public IBaseDataMgr
{
public:
	WTSBaseDataMgr();
	~WTSBaseDataMgr();

public:
	virtual WTSCommodityInfo*	getCommodity(const char* stdPID) override;
	virtual WTSCommodityInfo*	getCommodity(const char* exchg, const char* pid) override;

	virtual WTSContractInfo*	getContract(const char* code, const char* exchg = "") override;
	virtual WTSArray*			getContracts(const char* exchg = "") override;

	virtual WTSSessionInfo*		getSession(const char* sid) override;
	virtual WTSSessionInfo*		getSessionByCode(const char* code, const char* exchg = "") override;
	virtual WTSArray*			getAllSessions() override;
	virtual bool				isHoliday(const char* stdPID, uint32_t uDate, bool isTpl = false) override;


	virtual uint32_t			calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession = false) override;
	virtual uint64_t			getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession = false, bool isStart = true) override;
	void		release();


	bool		loadSessions(const char* filename, bool isUTF8);
	bool		loadCommodities(const char* filename, bool isUTF8);
	bool		loadContracts(const char* filename, bool isUTF8);
	bool		loadHolidays(const char* filename);

public:
	uint32_t	getTradingDate(const char* stdPID, uint32_t uOffDate = 0, uint32_t uOffMinute = 0, bool isTpl = false);
	uint32_t	getNextTDate(const char* stdPID, uint32_t uDate, int days = 1, bool isTpl = false);
	uint32_t	getPrevTDate(const char* stdPID, uint32_t uDate, int days = 1, bool isTpl = false);
	bool		isTradingDate(const char* stdPID, uint32_t uDate, bool isTpl = false);
	void		setTradingDate(const char* stdPID, uint32_t uDate, bool isTpl = false);

	CodeSet*	getSessionComms(const char* sid);

private:
	const char* getTplIDByPID(const char* stdPID);

private:
	TradingDayTplMap	m_mapTradingDay;

	SessionCodeMap		m_mapSessionCode;

	WTSExchgContract*	m_mapExchgContract;
	WTSSessionMap*		m_mapSessions;
	WTSCommodityMap*	m_mapCommodities;
	WTSContractMap*		m_mapContracts;
};

