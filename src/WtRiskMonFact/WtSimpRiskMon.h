/*!
 * \file WtSimpRiskMon.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#pragma once
#include <thread>
#include <memory>

#include "../Includes/RiskMonDefs.h"

USING_NS_WTP;

class WtSimpleRiskMon : public WtRiskMonitor
{
public:
	WtSimpleRiskMon() :_stopped(false), _limited(false){}

public:
	virtual const char* getName() override;

	virtual const char* getFactName() override;

	virtual void init(WtPortContext* ctx, WTSVariant* cfg) override;

	virtual void run() override;

	virtual void stop() override;

private:
	typedef std::shared_ptr<std::thread> ThreadPtr;
	ThreadPtr		_thrd;
	bool			_stopped;
	bool			_limited;

	uint64_t		_last_time;

	uint32_t		_calc_span;			//计算时间间隔,单位s
	uint32_t		_risk_span;			//回撤比较时间
	double			_basic_ratio;		//基础盈利率
	double			_risk_scale;		//风险控制系数
	double			_inner_day_fd;		//日内高点回撤边界
	bool			_inner_day_active;	//日内风控启用
	double			_multi_day_fd;		//多日高点回撤边界
	bool			_multi_day_active;	//多日风控启用
	double			_base_amount;		//基础资金规模
};