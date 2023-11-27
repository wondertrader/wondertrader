#pragma once
#include <stdint.h>
#include <string.h>
#include "../Includes/WTSMarcos.h"

#pragma warning(disable:4200)

namespace uft {

#pragma pack(push, 1)

	const char BLK_FLAG[] = "&^%$#@!\0";

	const int FLAG_SIZE = 8;

	typedef struct _BlockHeader
	{
		char		_blk_flag[FLAG_SIZE];
		uint32_t	_type;
		uint32_t	_date;

		uint32_t	_capacity;
		uint32_t	_size;
	} BlockHeader;

	typedef struct _DetailStruct
	{
		char		_exchg[MAX_EXCHANGE_LENGTH];
		char		_code[MAX_INSTRUMENT_LENGTH];
		uint32_t	_direct;	//方向0-多，1-空
		double		_volume;
		double		_open_price;
		uint64_t	_open_time;
		uint32_t	_open_tdate;
		double		_position_profit;

		double		_closed_volume;
		double		_closed_profit;

		_DetailStruct()
		{
			memset(this, 0, sizeof(_DetailStruct));
		}

	}DetailStruct;

	typedef struct _PositionBlock : public BlockHeader
	{
		DetailStruct	_details[0];
	}PositionBlock;

	typedef struct _OrderStruct
	{
		char		_exchg[MAX_EXCHANGE_LENGTH];
		char		_code[MAX_INSTRUMENT_LENGTH];
		uint32_t	_direct;
		uint32_t	_offset;

		double		_volume;
		double		_price;

		double		_traded;
		double		_left;
		uint32_t	_state;	//0-有效。1-全部成交，2-已撤单
		uint64_t	_oder_time;
	} OrderStruct;

	typedef struct _OrderBlock : public BlockHeader
	{
		OrderStruct	_orders[0];
	} OrderBlock;

	typedef struct _TradeStruct
	{
		char		_exchg[MAX_EXCHANGE_LENGTH];
		char		_code[MAX_INSTRUMENT_LENGTH];
		uint32_t	_direct;
		uint32_t	_offset;
		double		_volume;
		double		_price;
		uint32_t	_trading_date;
		uint64_t	_trading_time;
	} TradeStruct;

	typedef struct _TradeBlock : public BlockHeader
	{
		TradeStruct	_trades[0];
	} TradeBlock;

	typedef struct _RoundStruct
	{
		char		_exchg[MAX_EXCHANGE_LENGTH];
		char		_code[MAX_INSTRUMENT_LENGTH];
		uint32_t	_direct;
		double		_open_price;
		uint64_t	_open_time;
		double		_close_price;
		uint64_t	_close_time;
		double		_volume;
		double		_profit;
	} RoundStruct;

	typedef struct _RoundBlock : public BlockHeader
	{
		RoundStruct	_rounds[0];
	} RoundBlock;

#pragma pack(pop)

} //namespace uft