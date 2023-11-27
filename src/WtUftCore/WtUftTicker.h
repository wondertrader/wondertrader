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

NS_WTP_BEGIN
class WTSSessionInfo;
class WTSTickData;

class WtUftEngine;

class WtUftRtTicker
{
public:
	WtUftRtTicker(WtUftEngine* engine);
	~WtUftRtTicker();

public:
	void	init(const char* sessionID);
	void	on_tick(WTSTickData* curTick);

	void	run();
	void	stop();

private:
	WTSSessionInfo*	_s_info;
	WtUftEngine*	_engine;

	uint32_t	_date;
	uint32_t	_time;

	uint32_t	_cur_pos;

	StdUniqueMutex	_mtx;
	std::atomic<uint64_t>	_next_check_time;
	std::atomic<uint32_t>	_last_emit_pos;

	bool			_stopped;
	StdThreadPtr	_thrd;
};

NS_WTP_END