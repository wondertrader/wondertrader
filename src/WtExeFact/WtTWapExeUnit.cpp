#include "WtTWapExeUnit.h"

#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/fmtlib.h"
/***---begin---23.5.18---zhaoyk***/
#include "../Includes/WTSSessionInfo.hpp"
/***---end---23.5.18---zhaoyk***/
#include <math.h>


extern const char* FACT_NAME;


WtTWapExeUnit::WtTWapExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _ord_sticky(0)
	, _cancel_cnt(0)
	, _channel_ready(false)
	, _last_fire_time(0)
	, _fired_times(0)
	, _total_times(0)
	, _total_secs(0)
	, _price_mode(0)
	, _price_offset(0)
	, _target_pos(0)
	, _cancel_times(0)
	, _begin_time(0)
	, _end_time(0)
	, _last_place_time(0)
	, _last_tick_time(0)
	, isCanCancel{ true }
{
}


WtTWapExeUnit::~WtTWapExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

inline double get_real_target(double _target) {
	if (_target == DBL_MAX)
		return 0;

	return _target;
}
inline uint32_t calTmSecs(uint32_t begintime, uint32_t endtime) //计算执行时间：s
{
	return   ((endtime / 100) * 3600 + (endtime % 100) * 60) - ((begintime / 100) * 3600 + (begintime % 100) * 60);

}
inline bool is_clear(double target)
{
	return (target == DBL_MAX);
}
const char* WtTWapExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtTWapExeUnit::getName()
{
	return "WtTWapExeUnit";
}

void WtTWapExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);//获取品种参数
	if (_comm_info)
		_comm_info->retain();
	_sess_info = ctx->getSessionInfo(stdCode);//获取交易时间模板信息
	if (_sess_info)
		_sess_info->retain();
	_ord_sticky = cfg->getUInt32("ord_sticky");
	_begin_time= cfg->getUInt32("begin_time");
	_end_time = cfg->getUInt32("end_time");
	_total_secs = cfg->getUInt32("total_secs");
	_tail_secs = cfg->getUInt32("tail_secs");
	_total_times = cfg->getUInt32("total_times");
	_price_mode = cfg->getUInt32("price_mode");
	_price_offset = cfg->getUInt32("price_offset");
	_order_lots = cfg->getDouble("lots");		//单次发单手数
	if (cfg->has("minopenlots"))
	_min_open_lots = cfg->getDouble("minopenlots");	//最小开仓数量
	_total_secs = calTmSecs(_begin_time, _end_time);//执行总时间：秒
	_fire_span = (_total_secs - _tail_secs) / _total_times;		//单次发单时间间隔,要去掉尾部时间计算,这样的话,最后剩余的数量就有一个兜底发单的机制了

	ctx->writeLog(fmt::format("执行单元WtTWapExeUnit[{}] 初始化完成,订单超时 {} 秒,执行时限 {} 秒,收尾时间 {} 秒,间隔时间 {} 秒", stdCode, _ord_sticky, _total_secs, _tail_secs, _fire_span).c_str());
}

void WtTWapExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	if (!_orders_mon.has_order(localid))
		return;

	if (isCanceled || leftover == 0)  
	{
		_orders_mon.erase_order(localid);

		if (_cancel_cnt > 0)
			_cancel_cnt--;
		
		_ctx->writeLog(fmt::format("Order {} updated cancelcnt -> {}", localid, _cancel_cnt).c_str());
	}
	/***---begin---23.5.19---zhaoyk***/
	if (leftover == 0 && !isCanceled)
	{
		_cancel_times = 0;
		_ctx->writeLog(fmtutil::format("Order {} has filled", localid));
	}
	/***---end---23.5.19---zhaoyk***/
	//如果全部订单已撤销,这个时候一般是遇到要超时撤单（挂单超时） 
	if (isCanceled && _cancel_cnt == 0)
	{
		
		double realPos = _ctx->getPosition(stdCode);
		if (!decimal::eq(realPos, _this_target))//真实仓位和本轮目标仓位不相等时候
		{
			/***---begin---23.5.22---zhaoyk***/
			_ctx->writeLog(fmtutil::format("Order {} of {} canceled, re_fire will be done", localid, stdCode));
			_cancel_times++;
			
			//撤单以后重发,一般是加点重发;对最小下单量的校验
			//fire_at_once(_this_target - realPos);
			fire_at_once(max(_min_open_lots, _this_target - realPos));
			/***---end---23.5.22---zhaoyk***/
		}
	}
	/***---begin---23.5.22---zhaoyk***/
	//存在（isCanceled&&_cancel_cnt!=0）的情况,bug需要返回检查
	if (isCanceled&&_cancel_cnt!=0)
	{
		_ctx->writeLog(fmtutil::format("Order {} of {}  hasn't canceled, error will be return ", localid, stdCode));
		return;
	}
	/***---end---23.5.22---zhaoyk***/
} 

void WtTWapExeUnit::on_channel_ready()
{
	_channel_ready = true;
	double undone = _ctx->getUndoneQty(_code.c_str());
	/***---begin---23.5.18---zhaoyk***/
	if (!decimal::eq(undone, 0) && !_orders_mon.has_order())
	//if (undone != 0 && !_orders_mon.has_order())
	/***---end---23.5.18---zhaoyk***/
	{
		//这说明有未完成单不在监控之中,先撤掉
		_ctx->writeLog(fmt::format("{} unmanaged orders of {}, cancel all", undone, _code).c_str());

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();

		_ctx->writeLog(fmt::format("Unmanaged order updated cancelcnt to {}", _cancel_cnt).c_str());
	}
	/***---begin---23.5.18---zhaoyk***/
	else if (decimal::eq(undone, 0) && _orders_mon.has_order()) 
	{
		/*
		 *	By Wesey @ 2021.12.13
		 *	如果未完成单为0，但是OMS中是有订单的
		 *	说明OMS中是错单，需要清理掉，不然超时撤单就会出错
		 *	这种情况，一般是断线重连以后，之前下出去的订单，并没有真正发送到柜台
		 *	所以这里需要清理掉本地订单
		 */
		_ctx->writeLog(fmtutil::format("Local orders of {} not confirmed in trading channel, clear all", _code.c_str()));
		_orders_mon.clear_orders();
	}
	else//参考Minimpactunit
	{
		_ctx->writeLog(fmtutil::format("Unrecognized condition while channle ready,{:.2f} live orders of {} exists,local orders {}exist", undone, _code.c_str(), _orders_mon.has_order() ? "" : "not"));
	}
	/***---end---23.5.18---zhaoyk***/
	do_calc();
}

void WtTWapExeUnit::on_channel_lost()
{

}


void WtTWapExeUnit::on_tick(WTSTickData* newTick)
{
	if (newTick == NULL || _code.compare(newTick->code()) != 0)
		return;

	bool isFirstTick = false;
	//如果原来的tick不为空,则要释放掉
	if (_last_tick)
	{
		_last_tick->release();
	}
	/***---begin---23.5.18---zhaoyk***/
	else
	{
		isFirstTick = true;
		//如果行情时间不在交易时间,这种情况一般是集合竞价的行情进来,下单会失败,所以直接过滤掉这笔行情
		if (_sess_info != NULL && !_sess_info->isInTradingTime(newTick->actiontime() / 100000))
			return;
	}
	/***---end---23.5.18---zhaoyk***/

	//新的tick数据,要保留
	_last_tick = newTick;
	_last_tick->retain();

	/*
	*	这里可以考虑一下
	*	如果写的上一次丢出去的单子不够达到目标仓位
	*	那么在新的行情数据进来的时候可以再次触发核心逻辑
	*/

	if (isFirstTick)	//如果是第一笔tick,则检查目标仓位,不符合则下单
	{
		double newVol = _target_pos;
		const char* stdCode = _code.c_str();
		double undone = _ctx->getUndoneQty(stdCode);
		double realPos = _ctx->getPosition(stdCode);
		if (!decimal::eq(newVol, undone + realPos)) //仓位变化要交易 
		{
			do_calc();
		}
	}
	else
	{
		uint64_t now = TimeUtils::getLocalTimeNow();
		bool hasCancel = false;
		if (_ord_sticky != 0 && _orders_mon.has_order()) 
		{			
			_orders_mon.check_orders(_ord_sticky, now, [this, &hasCancel](uint32_t localid) {
				if (_ctx->cancel(localid))
				{
					_cancel_cnt++;
					_ctx->writeLog(fmt::format("Order expired, cancelcnt updated to {}", _cancel_cnt).c_str());
					hasCancel = true;
				}
			});

		}

		if (!hasCancel && (now - _last_fire_time >= _fire_span * 1000)) 
		{
			do_calc();
		}
	}
}

void WtTWapExeUnit::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	//不用触发,这里在ontick里触发吧
}

void WtTWapExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!bSuccess)
	{
		//如果不是我发出去的订单,我就不管了
		if (!_orders_mon.has_order(localid))
			return;

		_orders_mon.erase_order(localid);

		do_calc();
	}
}

void WtTWapExeUnit::fire_at_once(double qty)
{
	if (decimal::eq(qty, 0))
		return;

	_last_tick->retain();
	WTSTickData* curTick = _last_tick;
	const char* code = _code.c_str();
	uint64_t now = TimeUtils::getLocalTimeNow();
	bool isBuy = decimal::gt(qty, 0);
	double targetPx = 0;
	//根据价格模式设置,确定委托基准价格: 0-最新价,1-最优价,2-对手价
	if (_price_mode == 0)
	{
		targetPx = curTick->price();
	}
	else if (_price_mode == 1)
	{
		targetPx = isBuy ? curTick->bidprice(0) : curTick->askprice(0);
	}
	else // if(_price_mode == 2)
	{
		targetPx = isBuy ? curTick->askprice(0) : curTick->bidprice(0);
	}

	/***---begin---23.5.22---zhaoyk***/
	targetPx += _comm_info->getPriceTick() * _cancel_times* (isBuy ? 1 : -1);  
	/***---end---23.5.22---zhaoyk***/
	//检查涨跌停价
	isCanCancel = true;
	if (isBuy && !decimal::eq(_last_tick->upperlimit(), 0) && decimal::gt(targetPx, _last_tick->upperlimit()))
	{
		_ctx->writeLog(fmt::format("Buy price {} of {} modified to upper limit price", targetPx, _code.c_str(), _last_tick->upperlimit()).c_str());
		targetPx = _last_tick->upperlimit();
		isCanCancel = false;//如果价格被修正为涨跌停价，订单不可撤销
	}
	if (isBuy != 1 && !decimal::eq(_last_tick->lowerlimit(), 0) && decimal::lt(targetPx, _last_tick->lowerlimit()))
	{
		_ctx->writeLog(fmt::format("Sell price {} of {} modified to lower limit price", targetPx, _code.c_str(), _last_tick->lowerlimit()).c_str());
		targetPx = _last_tick->lowerlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}

	OrderIDs ids;
	if (qty > 0)
		ids = _ctx->buy(code, targetPx, abs(qty));
	else
		ids = _ctx->sell(code, targetPx, abs(qty));

	_orders_mon.push_order(ids.data(), ids.size(), now, isCanCancel);

	curTick->release();
}

void WtTWapExeUnit::do_calc()
{
	CalcFlag flag(&_in_calc);
	if (flag)
		return;

	if (!_channel_ready)
		return;
//这里加一个锁，主要原因是实盘过程中发现
//在修改目标仓位的时候，会触发一次do_calc
//而ontick也会触发一次do_calc，两次调用是从两个线程分别触发的，所以会出现同时触发的情况
//如果不加锁，就会引起问题
//这种情况在原来的SimpleExecUnit没有出现，因为SimpleExecUnit只在set_position的时候触发
	StdUniqueLock lock(_mtx_calc);

	const char* code = _code.c_str();
	double undone = _ctx->getUndoneQty(code);
	double newVol = get_real_target(_target_pos);//真实目标价格
	double realPos = _ctx->getPosition(code);
	double diffQty = newVol - realPos;

	//有正在撤销的订单,则不能进行下一轮计算
	if (_cancel_cnt != 0)
	{
		_ctx->writeLog(fmt::format("{}尚有未完成撤单指令,暂时退出本轮执行", _code).c_str());
		return;
	}

	if (decimal::eq(diffQty, 0))
		return;
	//每一次发单要保障成交,所以如果有未完成单,说明上一轮没完成
	//有未完成订单，与实际仓位变动方向相反
	//则需要撤销现有订单
	if (decimal::lt(diffQty * undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);   
		OrderIDs ids = _ctx->cancel(code, isBuy);
		if (!ids.empty())
		{
			_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
			_cancel_cnt += ids.size();
			_ctx->writeLog(fmtutil::format("[{}@{}] live opposite order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));//相反的订单已取消
		}
		return;
	}
	//因为是逐笔发单，所以如果有不需要撤销的未完成单，则暂不发单
	if (!decimal::eq(undone, 0))
	{
		_ctx->writeLog(fmt::format("{}上一轮有挂单未完成,暂时退出本轮执行", _code).c_str());
		return;
	}
	double curPos = realPos;
	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmt::format("{}没有最新tick数据,退出执行逻辑", _code).c_str());
		return;
	}
	if (decimal::eq(curPos, newVol))
	{
		//当前仓位和最新仓位匹配时，如果不是全部清仓的需求，则直接退出计算了
		if (!is_clear(_target_pos))
			return;

		//如果是清仓的需求，还要再进行对比
		//如果多头为0，说明已经全部清理掉了，则直接退出
		double lPos = _ctx->getPosition(code, true, 1); 
		if (decimal::eq(lPos, 0))
			return;

		//如果还有多头仓位，则将目标仓位设置为非0，强制触发                      
		newVol = -min(lPos, _order_lots);
		_ctx->writeLog(fmtutil::format("Clearing process triggered, target position of {} has been set to {}", _code.c_str(), newVol));
	}

	//如果相比上次没有更新的tick进来，则先不下单，防止开盘前集中下单导致通道被封
	uint64_t curTickTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog(fmtutil::format("No tick of {} updated, {} <= {}, execute later", _code, curTickTime, _last_tick_time));
		return;
	}
	_last_tick_time = curTickTime;
	double this_qty = _order_lots; 	//单次发单手数
	/***---end---23.5.26---zhaoyk***/

	uint32_t leftTimes = _total_times - _fired_times;
	/***---begin---23.5.22---zhaoyk***/
	//_ctx->writeLog(fmt::format("第 {} 次发单", _fired_times).c_str());
	_ctx->writeLog(fmt::format("第 {} 次发单", _fired_times+1).c_str());

	/***---end---23.5.22---zhaoyk***/
	bool bNeedShowHand = false;
	//剩余次数为0,剩余数量不为0,说明要全部发出去了
	//剩余次数为0,说明已经到了兜底时间了,如果这个时候还有未完成数量,则需要发单
	/***---begin---23.5.22---zhaoyk***/
	//如果剩余此处为0 ,则需要全部下单
	//否则,取整(剩余数量/剩余次数)与1的最大值,即最小为_min_open_lots,但是要注意符号处理
	double curQty = 0;
	if (leftTimes == 0 && !decimal::eq(diffQty, 0))
	{
		bNeedShowHand = true;
		curQty = max(_min_open_lots, diffQty); 
	}
	else {
		curQty = std::max(_min_open_lots, round(abs(diffQty) / leftTimes)) * abs(diffQty) / diffQty;
	}
	
	/***---end---23.5.22---zhaoyk***/
	
	//设定本轮目标仓位
	_this_target = realPos + curQty;	//现仓位量+本轮下单量

	WTSTickData* curTick = _last_tick;
	uint64_t now = TimeUtils::getLocalTimeNow();
	bool isBuy = decimal::gt(diffQty, 0);
	double targetPx = 0;
	//根据价格模式设置,确定委托基准价格: 0-最新价,1-最优价,2-对手价
	if (_price_mode == 0)
	{
		targetPx = curTick->price();
	}
	else if (_price_mode == 1)
	{
		targetPx = isBuy ? curTick->bidprice(0) : curTick->askprice(0);
	}
	else // if(_price_mode == 2)
	{
		targetPx = isBuy ? curTick->askprice(0) : curTick->bidprice(0);
	}

	//如果需要全部发单,则价格偏移5跳,以保障执行
	if (bNeedShowHand) //  last showhand time
	{
		targetPx += _comm_info->getPriceTick() * 5 * (isBuy ? 1 : -1);
	}
	else if (_price_offset != 0)	//如果设置了价格偏移,也要处理一下
	{
		targetPx += _comm_info->getPriceTick() * _price_offset * (isBuy ? 1 : -1);
	}
	// 如果最后价格为0，再做一个修正
	if (decimal::eq(targetPx, 0.0))
		targetPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

	//检查涨跌停价
	isCanCancel = true;
	if (isBuy && !decimal::eq(_last_tick->upperlimit(), 0) && decimal::gt(targetPx, _last_tick->upperlimit()))
	{
		_ctx->writeLog(fmt::format("Buy price {} of {} modified to upper limit price", targetPx, _code.c_str(), _last_tick->upperlimit()).c_str());
		targetPx = _last_tick->upperlimit();
		isCanCancel = false;//如果价格被修正为涨跌停价，订单不可撤销
	}
	if (isBuy != 1 && !decimal::eq(_last_tick->lowerlimit(), 0) && decimal::lt(targetPx, _last_tick->lowerlimit()))
	{
		_ctx->writeLog(fmt::format("Sell price {} of {} modified to lower limit price", targetPx, _code.c_str(), _last_tick->lowerlimit()).c_str());
		targetPx = _last_tick->lowerlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}
	//最后发出指令
	OrderIDs ids;
	if (curQty > 0)
		ids = _ctx->buy(code, targetPx, abs(curQty));
	else
		ids = _ctx->sell(code, targetPx, abs(curQty));

	_orders_mon.push_order(ids.data(), ids.size(), now, isCanCancel);
	_last_fire_time = now;
	_fired_times += 1;

	curTick->release();
}

void WtTWapExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	//double diff = newVol - _target_pos;
	if (decimal::eq(newVol, _target_pos))
		return;

	_target_pos = newVol;

	_fired_times = 0;

	do_calc();
}

