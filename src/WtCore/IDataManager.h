#pragma once
#include "../Share/WTSTypes.h"

NS_OTP_BEGIN
class WTSTickSlice;
class WTSKlineSlice;
class WTSTickData;


class IDataManager
{
public:
	virtual WTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual WTSKlineSlice* get_kline_slice(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) { return NULL; }

	virtual WTSTickData* grab_last_tick(const char* code) { return NULL; }
};

NS_OTP_END
