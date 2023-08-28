#pragma once
#include <stdint.h>
#include <string.h>

namespace uft {

#pragma pack(push, 1)

	const char BLK_FLAG[] = "&^%$#@!\0";

	const int FLAG_SIZE = 8;

	typedef struct _DetailStruct
	{
		uint32_t	_direct;	//·½Ïò0-¶à£¬1-¿Õ
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

	typedef struct _PositionBlock
	{
		char		_blk_flag[FLAG_SIZE];
		uint32_t	_type;
		uint32_t	_capacity;
		uint32_t	_count;

		DetailStruct	_details[0];
	}PositionBlock;

	typedef struct _OrderStruct
	{

	} OrderStruct;

	typedef struct _OrderBlock
	{
		char		_blk_flag[FLAG_SIZE];
		uint32_t	_type;
		uint32_t	_capacity;
		uint32_t	_count;

		OrderStruct	_orders[0];
	} OrderBlock;

	typedef struct _TradeStruct
	{

	} TradeStruct;

	typedef struct _TradeBlock
	{
		char		_blk_flag[FLAG_SIZE];
		uint32_t	_type;
		uint32_t	_capacity;
		uint32_t	_count;

		TradeStruct	_trades[0];
	} TradeBlock;

	typedef struct _RoundStruct
	{

	} RoundStruct;

	typedef struct _RoundBlock
	{
		char		_blk_flag[FLAG_SIZE];
		uint32_t	_type;
		uint32_t	_capacity;
		uint32_t	_count;

		RoundStruct	_rounds[0];
	} RoundBlock;

#pragma pack(pop)

} //namespace uft