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

typedef std::function<void(const char*, bool, double, double, double, double)> FuncEnumChnlPosCallBack;

class WtStockMinImpactExeUnit : public ExecuteUnit
{
private:
	const char* cbondStr = "CBOND";
	const char* stockStr = "STK";

	enum class TargetMode
	{
		stocks = 0,
		amount,
		ratio,
	};

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

	/*
	 *	账户信息回调
	 */
	virtual void on_account(const char* currency, double prebalance, double balance, double dynbalance, double avaliable, double closeprofit, double dynprofit, double margin, double fee, double deposit, double withdraw) override;

private:
	void check_unmanager_order();

private:
	WTSTickData* _last_tick;	//上一笔行情
	double		_target_pos;	//目标仓位
	double		_target_amount;  // 目标金额
	double		_target_ratio;  // 目标持仓比例

	double		_avaliable{ 0 }; // 账户可用

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
	bool		_is_finish;
	uint64_t	_start_time;
	double		_start_price{ 0 };
	bool		_is_first_tick{ true };
	double		_max_cancel_time{ 3 }; //最大撤单次数，如果超过这个次数仍然未撤单，则说明是错单
	double		_total_money{ -1 }; // 总资本
	double		_is_t0{ false }; // 对于转债等来说，这个需要是true，股票为false
	wt_hashmap< uint32_t, uint32_t > _cancel_map{};

	WtOrdMon	_orders_mon;
	//uint32_t	_cancel_cnt;
	uint32_t	_cancel_times;
	bool		_is_cancel_unmanaged_order{ true };
	uint64_t	_last_place_time;
	uint64_t	_last_tick_time;
	bool		_is_clear;
	TargetMode  _target_mode{ TargetMode::stocks };
	bool		_is_KC{ false };			// 是否是科创板股票
	double		_min_hands{ 0 };
	bool		_is_ready{ false };
	bool		_is_total_money_ready{ false };
	std::map<std::string, double> _market_value{};
	uint64_t _now;

public:
	inline int round_hands(double hands, double min_hands)
	{
		return (int)((hands + min_hands / 2) / min_hands) * min_hands;
	}

	inline double get_minOrderQty(std::string stdCode)
	{
		int code = std::stoi(StrUtil::split(stdCode, ".")[2]);
		bool is_KC = false;
		if (code >= 688000)
		{
			is_KC = true;
		}
		WTSCommodityInfo* comm_info = _ctx->getCommodityInfo(stdCode.c_str());
		double min_order = 1.0;
		if (strcmp(comm_info->getProduct(), cbondStr) == 0)
			min_order = 10.0;
		else if (strcmp(comm_info->getProduct(), stockStr) == 0)
			if (is_KC)
				min_order = 200.0;
			else
				min_order = 100.0;
		if (comm_info)
			comm_info->release();
		return min_order;
	}
};

