/*!
 * \file StateMonitor.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 状态控制器定义
 */
#pragma once
#include <vector>
#include "../Share/StdUtils.hpp"
#include "../Includes/FasterDefs.h"
#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSSessionInfo;
NS_WTP_END

USING_NS_WTP;

typedef enum tagSimpleState
{
	SS_ORIGINAL,		//未初始化
	SS_INITIALIZED,		//已初始化
	SS_RECEIVING,		//交易中
	SS_PAUSED,			//休息中
	SS_CLOSED,			//已收盘
	SS_PROCING,			//收盘作业中
	SS_PROCED,			//盘后已处理
	SS_Holiday	= 99	//节假日
} SimpleState;

typedef struct _StateInfo
{
	char		_session[16];
	uint32_t	_init_time;
	uint32_t	_close_time;
	uint32_t	_proc_time;
	SimpleState	_state;
	WTSSessionInfo*	_sInfo;

	typedef struct _Section
	{
		uint32_t _from;
		uint32_t _end;
	} Section;
	std::vector<Section> _sections;

	inline bool isInSections(uint32_t curTime)
	{
		for (auto it = _sections.begin(); it != _sections.end(); it++)
		{
			const Section& sec = *it;
			if (sec._from <= curTime && curTime < sec._end)
				return true;
		}
		return false;
	}

	_StateInfo()
	{
		_session[0] = '\0';
		_init_time = 0;
		_close_time = 0;
		_proc_time = 0;
		_state = SS_ORIGINAL;
		_sInfo = nullptr;
	}
} StateInfo;

typedef std::shared_ptr<StateInfo> StatePtr;
typedef wtp::wt_hashmap<std::string, StatePtr>	StateMap;

class WTSBaseDataMgr;
class DataManager;

class StateMonitor
{
public:
	StateMonitor();
	~StateMonitor();

public:
	bool initialize(const char* filename, WTSBaseDataMgr* bdMgr, DataManager* dtMgr);
	void run();
	void stop();

	inline bool	isAnyInState(SimpleState ss) const
	{
		auto it = _map.begin();
		for (; it != _map.end(); it++)
		{
			const StatePtr& sInfo = it->second;
			if (sInfo->_state == ss)
				return true;
		}

		return false;
	}

	inline bool	isAllInState(SimpleState ss) const
	{
		auto it = _map.begin();
		for (; it != _map.end(); it++)
		{
			const StatePtr& sInfo = it->second;
			if (sInfo->_state != SS_Holiday && sInfo->_state != ss)
				return false;
		}

		return true;
	}

	inline bool	isInState(const char* sid, SimpleState ss) const
	{
		auto it = _map.find(sid);
		if (it == _map.end())
			return false;

		const StatePtr& sInfo = it->second;
		return sInfo->_state == ss;
	}

private:
	StateMap		_map;
	WTSBaseDataMgr*	_bd_mgr;
	DataManager*	_dt_mgr;

	StdThreadPtr	_thrd;

	bool			_stopped;
};
