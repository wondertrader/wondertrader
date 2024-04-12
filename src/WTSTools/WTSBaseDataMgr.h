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

USING_NS_WTP;

typedef wt_hashmap<std::string, TradingDayTpl>	TradingDayTplMap;

typedef WTSHashMap<std::string>		WTSStringMap;

typedef wt_hashmap<std::string, CodeSet> SessionCodeMap;

class WTSBaseDataMgr : public IBaseDataMgr
{
public:
	WTSBaseDataMgr();
	~WTSBaseDataMgr();

public:
	virtual WTSCommodityInfo*	getCommodity(const char* stdPID) override;
	virtual WTSCommodityInfo*	getCommodity(const char* exchg, const char* pid) override;

	virtual WTSContractInfo*	getContract(const char* code, const char* exchg = "", uint32_t uDate = 0) override;
	virtual WTSArray*			getContracts(const char* exchg = "", uint32_t uDate = 0) override;

	virtual WTSSessionInfo*		getSession(const char* sid) override;
	virtual WTSSessionInfo*		getSessionByCode(const char* code, const char* exchg = "") override;
	virtual WTSArray*			getAllSessions() override;
	virtual bool				isHoliday(const char* stdPID, uint32_t uDate, bool isTpl = false) override;

	virtual uint32_t			calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession = false) override;
	virtual uint64_t			getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession = false, bool isStart = true) override;

	virtual uint32_t			getContractSize(const char* exchg = "", uint32_t uDate = 0) override;

	virtual uint32_t			getGlobalSize() override { return (uint32_t)_code_list->size(); }

	virtual WTSContractInfo*	getContractByIndex(uint32_t idx) override;


	bool		loadSessions(const char* filename);
	bool		loadCommodities(const char* filename);
	bool		loadHolidays(const char* filename);

	bool		loadContracts(const char* filename);

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
	TradingDayTplMap	_trading_days;

	WTSStringMap*		_session_map;
	SessionCodeMap		_session_to_pids;

	WTSArray*			_code_list;
	WTSArray*			_comm_list;

	WTSStringMap*		_code_map;
	WTSStringMap*		_fullcode_map;

	WTSStringMap*		_pid_map;
	WTSStringMap*		_fullpid_map;
};

