/*!
 * \file WtStockMinImpactExeUnit.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 最小冲击执行单元
 */
#pragma once
#include "../Includes/ExecuteDefs.h"
#include "WtOrdMon.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share/fmtlib.h"

USING_NS_WTP;

#define BESTPX -1 // 己方最优
#define LASTPX 0  // 最新价
#define MARKET 1  // 对手价
#define AUTOPX 2  // 自动

class WtStockMinImpactExeUnit : public ExecuteUnit
{
private:
	std::vector<std::string> PriceModeNames = {
		"BESTPX",		//最优价
		"LASTPX",		//最新价
		"MARKET",		//对手价
		"AUTOPX"		//自动
	};

public:
	WtStockMinImpactExeUnit();
	virtual ~WtStockMinImpactExeUnit();

private:
	void	do_calc();
	bool	is_clear();
public:
	/*
	 *	所属执行器工厂名称
	 */
	virtual const char* getFactName() override;

	/*
	 *	执行单元名称
	 */
	virtual const char* getName() override;

	/*
	 *	初始化执行单元
	 *	ctx		执行单元运行环境
	 *	code	管理的合约代码
	 */
	virtual void init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg) override;

	/*
	 *	订单回报
	 *	localid	本地单号
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	leftover	剩余数量
	 *	price	委托价格
	 *	isCanceled	是否已撤销
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled) override;

	/*
	 *	tick数据回调
	 *	newTick	最新的tick数据
	 */
	virtual void on_tick(WTSTickData* newTick) override;

	/*
	 *	成交回报
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	vol		成交数量,这里没有正负,通过isBuy确定买入还是卖出
	 *	price	成交价格
	 */
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	/*
	 *	下单结果回报
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	/*
	 *	设置新的目标仓位
	 *	code	合约代码
	 *	newVol	新的目标仓位
	 */
	virtual void set_position(const char* stdCode, double newVol) override;

	/*
	 *	设置新的目标仓位
	 *	code	合约代码
	 *	newVol	新的目标仓位
	 */
	virtual void set_amount(const char* stdCode, double newAmount);

	/*
	 *	清理全部持仓
	 *	stdCode	合约代码
	 */
	virtual void clear_all_position(const char* stdCode) override;

	/*
	 *	交易通道就绪回调
	 */
	virtual void on_channel_ready() override;

	/*
	 *	交易通道丢失回调
	 */
	virtual void on_channel_lost() override;

private:
	WTSTickData* _last_tick;	//上一笔行情
	double		_target_pos;	//目标仓位
	double		_target_amount;  // 目标金额
	StdUniqueMutex	_mtx_calc;
	WTSCommodityInfo* _comm_info;
	WTSSessionInfo* _sess_info;

	//////////////////////////////////////////////////////////////////////////
	//执行参数
	int32_t		_price_offset;
	uint32_t	_expire_secs;
	int32_t		_price_mode;
	uint32_t	_entrust_span;
	bool		_by_rate;
	double		_order_lots;
	double		_qty_rate;
	double		_min_order;

	WtOrdMon	_orders_mon;
	uint32_t	_cancel_cnt;
	uint32_t	_cancel_times;

	uint64_t	_last_place_time;
	uint64_t	_last_tick_time;
	bool		_is_clear;
	bool		_is_target_pos;
	bool		_is_KC {false};			// 是否是科创板股票
public:
	inline int round_hands(double hands)
	{
		return (int)((hands + 50) / 100) * 100;
	}
};

