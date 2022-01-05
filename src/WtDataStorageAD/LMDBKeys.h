#pragma once
#include <stdint.h>
#include <string.h>
#include "../Includes/WTSMarcos.h"

static uint16_t reverseEndian(uint16_t src)
{
	uint16_t up = (src & 0x00FF) << 8;
	uint16_t low = (src & 0xFF00) >> 8;
	return up + low;
}

static uint32_t reverseEndian(uint32_t src)
{
	uint32_t x = (src & 0x000000FF) << 24;
	uint32_t y = (src & 0x0000FF00) << 8;
	uint32_t z = (src & 0x00FF0000) >> 8;
	uint32_t w = (src & 0xFF000000) >> 24;
	return x + y + z + w;
}

#pragma pack(push, 1)
typedef struct _LMDBHftKey
{
	char		_exchg[MAX_EXCHANGE_LENGTH];
	char		_code[MAX_INSTRUMENT_LENGTH];
	uint32_t	_date;
	uint32_t	_time;

	_LMDBHftKey(const char* exchg, const char* code, uint32_t date, uint32_t time)
	{
		memset(this, 0, sizeof(_LMDBHftKey));
		strcpy(_exchg, exchg);
		strcpy(_code, code);
		_date = reverseEndian(date);
		_time = reverseEndian(time);
	}
} LMDBHftKey;

typedef struct  _LMDBBarKey
{
public:
	char		_exchg[MAX_EXCHANGE_LENGTH];
	char		_code[MAX_INSTRUMENT_LENGTH];
	uint32_t	_bartime;

	_LMDBBarKey(const char* exchg, const char* code, uint32_t bartime)
	{
		memset(this, 0, sizeof(_LMDBBarKey));
		strcpy(_exchg, exchg);
		strcpy(_code, code);
		_bartime = reverseEndian(bartime);
	}
} LMDBBarKey;
#pragma pack(pop)