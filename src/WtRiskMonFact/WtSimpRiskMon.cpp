/*!
 * \file WtSimpRiskMon.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtSimpRiskMon.h"

#include "../Includes/WTSRiskDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../Share/fmtlib.h"

extern const char* FACT_NAME;

const char* WtSimpleRiskMon::getName()
{
	return "WtSimpleRiskMon";
}

const char* WtSimpleRiskMon::getFactName()
{
	return FACT_NAME;
}

void WtSimpleRiskMon::init(WtPortContext* ctx, WTSVariant* cfg)
{
	WtRiskMonitor::init(ctx, cfg);

	_calc_span = cfg->getUInt32("calc_span");
	_risk_span = cfg->getUInt32("risk_span");
	_basic_ratio = cfg->getUInt32("basic_ratio");
	_inner_day_fd = cfg->getDouble("inner_day_fd");
	_inner_day_active = cfg->getBoolean("inner_day_active");
	_multi_day_fd = cfg->getDouble("multi_day_fd");
	_multi_day_active = cfg->getBoolean("multi_day_active");
	_base_amount = cfg->getDouble("base_amount");
	_risk_scale = cfg->getDouble("risk_scale");

	//ctx->writeRiskLog(fmt::sprintf("参数初始化完成, 监控时间间隔: %u 秒, 日内回撤: %s(%.2f%%), 多日回撤: %s(%.2f%%), 资金基数: %.1f, 门槛盈利: %.2f%%, 回撤时限: %u mins, 风险控制系数: %.2f",
	//	_calc_span, _inner_day_active ? "开启" : "关闭", _inner_day_fd, _multi_day_active ? "开启" : "关闭", _multi_day_fd, _base_amount, _basic_ratio, _risk_span, _risk_scale);

	ctx->writeRiskLog(fmt::sprintf("Params inited, Checking frequency: %u s, MaxIDD: %s(%.2f%%), MaxMDD: %s(%.2f%%), Capital: %.1f, Profit Boudary: %.2f%%, Calc Span: %u mins, Risk Scale: %.2f",
		_calc_span, _inner_day_active ? "ON" : "OFF", _inner_day_fd, _multi_day_active ? "ON" : "OFF", _multi_day_fd, _base_amount, _basic_ratio, _risk_span, _risk_scale).c_str());
}

void WtSimpleRiskMon::run()
{
	if (_thrd)
		return;

	_thrd.reset(new std::thread([this](){
		while (!_stopped)
		{
			if (_ctx && _ctx->isInTrading())
			{
				WTSPortFundInfo* fundInfo = _ctx->getFundInfo();
				const WTSFundStruct& fs = fundInfo->fundInfo();
				/*
				* 条件1: 整体盘子的浮动收益比上一交易日结束时（收盘价计）, 增长 1% 以上
				*		组合盘的动态权益 ≥ 上日收盘时的动态权益的 101%
				* 条件2: 30min以内, 从今日高点回调到 80%以下
				*		30min以内, 今日收益从高点回调到 80%以下
				*
				* 动作: 
				* 方式A:  所有品种减仓（减少到 30% 仓位）, 下一交易日重新按策略新仓位补齐
				* 方式B:  所有盈利品种都 平仓, 下一交易日重新按策略新仓位补齐
				*/

				if (_inner_day_active && fs._max_dyn_bal != DBL_MAX)
				{
					double predynbal = fundInfo->predynbalance() + _base_amount;	//上日动态权益
					double maxBal = fs._max_dyn_bal + _base_amount;					//当日最大动态权益
					double curBal = fs._balance + fs._dynprofit + _base_amount;		//当前动态权益

					double rate = 0.0;
					if(!decimal::eq(maxBal, predynbal))
						rate = (maxBal - curBal) * 100 / (maxBal - predynbal);	//当日盈利回撤比例

					//如果当日最大权益超过止盈边界条件
					if (maxBal > (_basic_ratio*predynbal / 100.0))
					{
						
						/*
						 *	这里要转成日内分钟数处理
						 *	不然如果遇到午盘启动或早盘启动, 
						 *	可能会因为中途休息时间过长, 而不触发风控
						 *	导致更大风险的发生
						 */
						uint32_t maxTime = _ctx->transTimeToMin(fundInfo->max_dynbal_time());	
						uint32_t curTime = _ctx->transTimeToMin(_ctx->getCurTime());			//转成日内分钟数

						if (rate >= _inner_day_fd && curTime - maxTime <= _risk_span && !_limited)
						{
							_ctx->writeRiskLog(fmt::sprintf("Current IDD %.2f%%, ≥MaxIDD %.2f%%, Position down to %.1f%%", rate, _inner_day_fd, _risk_scale).c_str());
							_ctx->setVolScale(_risk_scale);
							_limited = true;
						}
						else
						{
							_ctx->writeRiskLog(fmt::sprintf("Current PR: %.2f%%, Current IDD: %.2f%%", curBal*100.0 / predynbal, rate).c_str());
							//_limited = false;
						}
					}
					else
					{
						//如果当日最大权益没有超过盈利边界条件
						_ctx->writeRiskLog(fmt::sprintf("Current PR: %.2f%%", curBal*100.0 / predynbal).c_str());
						//_limited = false;
					}
				}

				if (_multi_day_active && fs._max_md_dyn_bal._date != 0)
				{
					double maxBal = fs._max_md_dyn_bal._dyn_balance + _base_amount;
					double curBal = fs._balance + fs._dynprofit + _base_amount;

					if (curBal < maxBal)
					{
						double rate = (maxBal - curBal) * 100 / maxBal;
						if (rate >= _multi_day_fd)
						{
							_ctx->writeRiskLog(fmt::sprintf("Current MDD %.2f%%, ≥MaxMDD %.2f%%, Position down to 0.0%%", rate, _multi_day_fd).c_str());
							_ctx->setVolScale(0.0);
						}
					}
				}
			}

			_last_time = TimeUtils::getLocalTimeNow();

			while (!_stopped)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				uint64_t now = TimeUtils::getLocalTimeNow();;
				if(now - _last_time >= _calc_span*1000)
					break;
			}
		}
	}));
}

void WtSimpleRiskMon::stop()
{
	_stopped = true;
	if (_thrd)
		_thrd->join();
}