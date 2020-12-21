/*!
* \file WtHftTicker.h
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#pragma once

#include <stdint.h>
#include <atomic>

#include "../Includes/WTSMarcos.h"
#include "../Share/StdUtils.hpp"

NS_OTP_BEGIN
class WTSSessionInfo;
class IDataReader;
class WTSTickData;

class WtSelEngine;

class WtSelRtTicker
{
public:
	WtSelRtTicker(WtSelEngine* engine);
	~WtSelRtTicker();

public:
	void	init(IDataReader* store, const char* sessionID);
	void	on_tick(WTSTickData* curTick, bool isHot = false);

	void	run();
	void	stop();

private:
	void	trigger_price(WTSTickData* curTick, bool isHot);

private:
	WTSSessionInfo*	_s_info;
	WtSelEngine*	_engine;
	IDataReader*		_store;

	uint32_t	_date;
	uint32_t	_time;

	uint32_t	_cur_pos;

	StdUniqueMutex	_mtx;
	std::atomic<uint64_t>	_next_check_time;
	std::atomic<uint32_t>	_last_emit_pos;

	bool			_stopped;
	StdThreadPtr	_thrd;
};

NS_OTP_END