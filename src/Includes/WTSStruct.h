/*!
 * \file WTSStruct.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt基础结构体定义
 */
#pragma once
#include <memory>
#include <stdint.h>
#include <string.h>
#include "WTSTypes.h"

#pragma warning(disable:4200)

NS_WTP_BEGIN

#pragma pack(push, 1)

struct WTSBarStructOld
{
public:
	WTSBarStructOld()
	{
		memset(this, 0, sizeof(WTSBarStructOld));
	}

	uint32_t	date;
	uint32_t	time;
	double		open;		//开
	double		high;		//高
	double		low;		//低
	double		close;		//收
	double		settle;		//结算
	double		money;		//成交金额

	uint32_t	vol;	//成交量
	uint32_t	hold;	//总持
	int32_t		add;	//增仓
};

struct WTSTickStructOld
{
	char		exchg[10];
	char		code[MAX_INSTRUMENT_LENGTH];

	double		price;				//最新价
	double		open;				//开盘价
	double		high;				//最高价
	double		low;				//最低价
	double		settle_price;		//结算价

	double		upper_limit;		//涨停价
	double		lower_limit;		//跌停价

	uint32_t	total_volume;		//总成交量
	uint32_t	volume;				//成交量
	double		total_turnover;		//总成交额
	double		turn_over;			//成交额
	uint32_t	open_interest;		//总持
	int32_t		diff_interest;		//增仓

	uint32_t	trading_date;		//交易日,如20140327
	uint32_t	action_date;		//自然日期,如20140327
	uint32_t	action_time;		//发生时间,精确到毫秒,如105932000

	double		pre_close;			//昨收价
	double		pre_settle;			//昨结算
	int32_t		pre_interest;		//上日总持

	double		bid_prices[10];		//委买价格
	double		ask_prices[10];		//委卖价格
	uint32_t	bid_qty[10];		//委买量
	uint32_t	ask_qty[10];		//委卖量
	WTSTickStructOld()
	{
		memset(this, 0, sizeof(WTSTickStructOld));
	}
};

#pragma pack(pop)


//By Wesley @ 2021.12.31
//新的结构体，全部改成8字节对齐的方式
#pragma pack(push, 8)

struct WTSBarStruct
{
public:
	WTSBarStruct()
	{
		memset(this, 0, sizeof(WTSBarStruct));
	}

	uint32_t	date;		//日期
	uint32_t	reserve_;	//占位符
	uint64_t	time;		//时间
	double		open;		//开
	double		high;		//高
	double		low;		//低
	double		close;		//收
	double		settle;		//结算
	double		money;		//成交金额

	double		vol;	//成交量
	double		hold;	//总持
	double		add;	//增仓

	//By Wesley @ 2021.12.30
	//直接复制老结构体
	WTSBarStruct& operator = (const WTSBarStructOld& bar)
	{
		date = bar.date;
		time = bar.time;

		open = bar.open;
		high = bar.high;
		low = bar.low;
		close = bar.close;
		settle = bar.settle;
		money = bar.money;

		vol = bar.vol;
		hold = bar.hold;
		add = bar.add;

		return *this;
	}
};

struct WTSTickStruct
{
	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	double		price;				//最新价
	double		open;				//开盘价
	double		high;				//最高价
	double		low;				//最低价
	double		settle_price;		//结算价

	double		upper_limit;		//涨停价
	double		lower_limit;		//跌停价

	double		total_volume;		//总成交量
	double		volume;				//成交量
	double		total_turnover;		//总成交额
	double		turn_over;			//成交额
	double		open_interest;		//总持
	double		diff_interest;		//增仓

	uint32_t	trading_date;		//交易日,如20140327
	uint32_t	action_date;		//自然日期,如20140327
	uint32_t	action_time;		//发生时间,精确到毫秒,如105932000
	uint32_t	reserve_;			//占位符

	double		pre_close;			//昨收价
	double		pre_settle;			//昨结算
	double		pre_interest;		//上日总持

	double		bid_prices[10];		//委买价格
	double		ask_prices[10];		//委卖价格
	double		bid_qty[10];		//委买量
	double		ask_qty[10];		//委卖量
	WTSTickStruct()
	{
		memset(this, 0, sizeof(WTSTickStruct));
	}

	WTSTickStruct& operator = (const WTSTickStructOld& tick)
	{
		strncpy(exchg, tick.exchg, MAX_EXCHANGE_LENGTH);
		strncpy(code, tick.code, MAX_INSTRUMENT_LENGTH);

		price = tick.price;
		open = tick.open;
		high = tick.high;
		low = tick.low;
		settle_price = tick.settle_price;

		upper_limit = tick.upper_limit;
		lower_limit = tick.lower_limit;

		total_volume = tick.total_volume;
		total_turnover = tick.total_turnover;
		open_interest = tick.open_interest;
		volume = tick.volume;
		turn_over = tick.turn_over;
		diff_interest = tick.diff_interest;

		trading_date = tick.trading_date;
		action_date = tick.action_date;
		action_time = tick.action_time;

		pre_close = tick.pre_close;
		pre_interest = tick.pre_interest;
		pre_settle = tick.pre_settle;

		for(int i = 0; i < 10; i++)
		{
			bid_prices[i] = tick.bid_prices[i];
			bid_qty[i] = tick.bid_qty[i];
			ask_prices[i] = tick.ask_prices[i];
			ask_qty[i] = tick.ask_qty[i];
		}

		return *this;
	}
};

struct WTSOrdQueStruct
{
	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	uint32_t	trading_date;		//交易日,如20140327
	uint32_t	action_date;		//自然日期,如20140327
	uint32_t	action_time;		//发生时间,精确到毫秒,如105932000
	WTSBSDirectType	side;			//委托方向

	double		price;			//委托价格
	uint32_t	order_items;	//订单个数
	uint32_t	qsize;			//队列长度
	uint32_t	volumes[50];	//委托明细

	WTSOrdQueStruct()
	{
		memset(this, 0, sizeof(WTSOrdQueStruct));
	}
};

struct WTSOrdDtlStruct
{
	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	uint32_t		trading_date;		//交易日,如20140327
	uint32_t		action_date;		//自然日期,如20140327
	uint32_t		action_time;		//发生时间,精确到毫秒,如105932000

	uint64_t		index;			//委托编号(从1开始,递增1)
	double			price;			//委托价格
	uint64_t		volume;			//委托数量
	WTSBSDirectType		side;		//委托方向
	WTSOrdDetailType	otype;		//委托类型

	WTSOrdDtlStruct()
	{
		memset(this, 0, sizeof(WTSOrdDtlStruct));
	}
};

struct WTSTransStruct
{
	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	uint32_t	trading_date;		//交易日,如20140327
	uint32_t	action_date;		//自然日期,如20140327
	uint32_t	action_time;		//发生时间,精确到毫秒,如105932000
	uint32_t	index;			//成交编号(从1开始,递增1)

	WTSTransType	ttype;			//成交类型: 'C', 0
	WTSBSDirectType	side;			//BS标志

	double			price;			//成交价格
	uint32_t		volume;			//成交数量
	int32_t			askorder;		//叫卖序号
	int32_t			bidorder;		//叫买序号

	WTSTransStruct()
	{
		memset(this, 0, sizeof(WTSTransStruct));
	}
};

#pragma pack(pop)

NS_WTP_END
