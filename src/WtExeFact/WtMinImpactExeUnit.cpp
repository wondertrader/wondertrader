/*!
 * \file WtMinImpactExeUnit.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "WtMinImpactExeUnit.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share//fmtlib.h"

extern const char* FACT_NAME;

const char* PriceModeNames[] =
{
	"BESTPX",		//最优价
	"LASTPX",		//最新价
	"MARKET",		//对手价
	"AUTOPX"		//自动
};

inline double get_real_target(double target)
{
	if (target == DBL_MAX)			 
		return 0;

	return target;
}

inline bool is_clear(double target)
{
	return (target == DBL_MAX);
}


WtMinImpactExeUnit::WtMinImpactExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _price_mode(0)
	, _price_offset(0)
	, _expire_secs(0)
	, _cancel_cnt(0)
	, _target_pos(0)
	, _cancel_times(0)
	, _last_place_time(0)
	, _last_tick_time(0)
	, _in_calc(false)
	, _min_open_lots(1)
{
}


WtMinImpactExeUnit::~WtMinImpactExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtMinImpactExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtMinImpactExeUnit::getName()
{
	return "WtMinImpactExeUnit";
}

void WtMinImpactExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);//获取品种参数
	if (_comm_info)
		_comm_info->retain();

	_sess_info = ctx->getSessionInfo(stdCode);//获取交易时间模板信息
	if (_sess_info)
		_sess_info->retain();

	_price_offset = cfg->getInt32("offset");	//价格偏移跳数，一般和订单同方向
	_expire_secs = cfg->getUInt32("expire");	//订单超时秒数
	_price_mode = cfg->getInt32("pricemode");	//价格类型,0-最新价,-1-最优价,1-对手价,2-自动,默认为0
	_entrust_span = cfg->getUInt32("span");		//发单时间间隔，单位毫秒
	_by_rate = cfg->getBoolean("byrate");		//是否按照对手的挂单数的比例下单，如果是true，则rate字段生效，如果是false则lots字段生效
	_order_lots = cfg->getDouble("lots");		//单次发单手数
	_qty_rate = cfg->getDouble("rate");			//下单手数比例

	if (cfg->has("minopenlots"))  
		_min_open_lots = cfg->getDouble("minopenlots");	//最小开仓数量

	ctx->writeLog(fmtutil::format("MiniImpactExecUnit of {} inited, order price @ {}±{} ticks, expired after {} secs, reorder after {} millisec, lots policy: {} @ {:.2f}, min open lots: {}",
		stdCode, PriceModeNames[_price_mode + 1], _price_offset, _expire_secs, _entrust_span, _by_rate ? "byrate" : "byvol", _by_rate ? _qty_rate : _order_lots, _min_open_lots));
}
/*
 *	订单回报
 *	localid	本地单号
 *	code	合约代码
 *	isBuy	买or卖
 *	leftover	剩余数量
 *	price	委托价格
 *	isCanceled	是否已撤销
 */
void WtMinImpactExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	{
		if (!_orders_mon.has_order(localid)) //如果没有对应订单 返回
			return;

		if (isCanceled || leftover == 0)  //已撤销或剩余订单为0
		{
			_orders_mon.erase_order(localid);
			if (_cancel_cnt > 0)
			{
				_cancel_cnt--;
				_ctx->writeLog(fmtutil::format("[{}@{}] Order of {} cancelling done, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));
			}
		}

		if (leftover == 0 && !isCanceled)
			_cancel_times = 0;
	}

	//如果有撤单,也触发重新计算
	if (isCanceled)
	{
		_ctx->writeLog(fmtutil::format("Order {} of {} canceled, recalc will be done", localid, stdCode));
		_cancel_times++;
		do_calc();
	}
}
/*
 *	交易通道就绪回调
 */
void WtMinImpactExeUnit::on_channel_ready()
{
	double undone = _ctx->getUndoneQty(_code.c_str());//获取未完成数量

	if(!decimal::eq(undone, 0) && !_orders_mon.has_order())
	{
		/*
		 *	如果未完成单不为0，而OMS没有订单
		 *	这说明有未完成单不在监控之中,全部撤销掉
		 *	因为这些订单没有本地订单号，无法直接进行管理
		 *	这种情况，就是刚启动的时候，上次的未完成单或者外部的挂单
		 */
		_ctx->writeLog(fmtutil::format("Unmanaged live orders with qty {} of {} found, cancel all", undone, _code.c_str()));

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);//根据本地订单号撤单
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime()); //push into orderpair
		_cancel_cnt += ids.size();

		_ctx->writeLog(fmtutil::format("[{}@{}]cancelcnt -> {}", __FILE__, __LINE__, _cancel_cnt));
	}
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
	else
	{
		_ctx->writeLog(fmtutil::format("Unrecognized condition while channle ready, {:.2f} live orders of {} exists, local orders {}exist",
			undone, _code.c_str(), _orders_mon.has_order() ? "" : "not "));
	}


	do_calc();
}
/*
	 *	交易通道丢失回调
	 */
void WtMinImpactExeUnit::on_channel_lost()
{
	
}
/*
 *	tick数据回调
 *	newTick	最新的tick数据
 */
void WtMinImpactExeUnit::on_tick(WTSTickData* newTick)
{
	if (newTick == NULL || _code.compare(newTick->code()) != 0)
		return;

	//如果原来的tick不为空,则要释放掉
	if (_last_tick)
	{
		_last_tick->release();
	}
	else
	{
		//如果行情时间不在交易时间,这种情况一般是集合竞价的行情进来,下单会失败,所以直接过滤掉这笔行情
		if (_sess_info != NULL && !_sess_info->isInTradingTime(newTick->actiontime() / 100000))
			return;
	}

	//新的tick数据,要保留
	_last_tick = newTick;
	_last_tick->retain();

	/*
	 *	这里可以考虑一下
	 *	如果写的上一次丢出去的单子不够达到目标仓位
	 *	那么在新的行情数据进来的时候可以再次触发核心逻辑
	 */
		//*********在ontick中对订单管理进行校验。 例如有不活跃合约，校验减少  反之增多
	if(_expire_secs != 0 && _orders_mon.has_order() && _cancel_cnt==0)  //订单超时秒数！=0&&hasorder && 撤单量==0
	{
		uint64_t now = _ctx->getCurTime();

		_orders_mon.check_orders(_expire_secs, now, [this](uint32_t localid) {
			if (_ctx->cancel(localid))
			{
				_cancel_cnt++;
				_ctx->writeLog(fmtutil::format("[{}@{}] Expired order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));
			}
		});
	}
	
	do_calc();
}
/*
 *	成交回报
 *	code	合约代码
 *	isBuy	买or卖
 *	vol		成交数量,这里没有正负,通过isBuy确定买入还是卖出
 *	price	成交价格
 */
void WtMinImpactExeUnit::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	//不用触发,这里在ontick里触发吧
}

/*
 *	下单结果回报
 */
void WtMinImpactExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
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

void WtMinImpactExeUnit::do_calc()
{
	CalcFlag flag(&_in_calc);
	if (flag)
		return;

	if (_cancel_cnt != 0)
		return;

	//这里加一个锁，主要原因是实盘过程中发现
	//在修改目标仓位的时候，会触发一次do_calc
	//而ontick也会触发一次do_calc，两次调用是从两个线程分别触发的，所以会出现同时触发的情况
	//如果不加锁，就会引起问题
	//这种情况在原来的SimpleExecUnit没有出现，因为SimpleExecUnit只在set_position的时候触发
	StdUniqueLock lock(_mtx_calc );

	double newVol = get_real_target(_target_pos);//真实价格目标仓位  newvol
	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);  //undone  未完成订单量
	double realPos = _ctx->getPosition(stdCode);  //realpos 获取仓位 
	double diffPos = newVol - realPos;			  //diffpos = 真实目标仓位-获取仓位

	//有未完成订单，与实际仓位变动方向相反
	//则需要撤销现有订单
	if (decimal::lt(diffPos * undone, 0)) //diff*undone 剩余需要完成*未完成订单 <0   true   -> cancel
	{
		bool isBuy = decimal::gt(undone, 0);   //未完成单>0   isbuy = 1 买方
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		if(!ids.empty())
		{
			_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
			_cancel_cnt += ids.size();
			_ctx->writeLog(fmtutil::format("[{}@{}] live opposite order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));//相反的订单已取消
		}
		return;
	}

	//因为是逐笔发单，所以如果有不需要撤销的未完成单，则暂不发单
	//****此处逐笔指 拆单的回合中上一笔订单量未完结，则不发单， 逐笔发单指每个回合成交的单
	if (!decimal::eq(undone, 0))
		return;

	double curPos = realPos;   //realPos获取仓位   curPos  为获取现在的仓位

	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmtutil::format("No lastest tick data of {}, execute later", _code.c_str()));
		return;
	}

	//检查下单时间间隔
	uint64_t now = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());
	if (now - _last_place_time < _entrust_span) //当前时间-上ticktime <发单时间间隔
		return;

	if (decimal::eq(curPos, newVol)) 
	{
		//当前仓位和最新仓位匹配时，如果不是全部清仓的需求，则直接退出计算了
		if (!is_clear(_target_pos))
			return;

		//如果是清仓的需求，还要再进行对比
		//如果多头为0，说明已经全部清理掉了，则直接退出
		double lPos = _ctx->getPosition(stdCode, true , 1); //获取（多头）持仓  可用持仓   多头 //返回值	轧平后的仓位: 多仓>0, 空仓<0
		if (decimal::eq(lPos, 0))
			return;

		//如果还有多头仓位，则将目标仓位设置为非0，强制触发                      
		newVol = -min(lPos, _order_lots);//  -min(获取（多头）持仓     单次发单手数) 
		_ctx->writeLog(fmtutil::format("Clearing process triggered, target position of {} has been set to {}", _code.c_str(), newVol));
	}

	bool bForceClose = is_clear(_target_pos); //target==del_max  return 1

	bool isBuy = decimal::gt(newVol, curPos);//如何判断买卖方向    真实目标仓位-获取持仓   > 0 多头

	//如果相比上次没有更新的tick进来，则先不下单，防止开盘前集中下单导致通道被封
	uint64_t curTickTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog(fmtutil::format("No tick of {} updated, {} <= {}, execute later", _code, curTickTime, _last_tick_time));
		return;
	}

	_last_tick_time = curTickTime;

	double this_qty = _order_lots; 	//单次发单手数
	if (_by_rate)//是否按照对手的挂单数的比例下单，如果是true，则rate字段生效，如果是false则lots字段生效
	{
		this_qty = isBuy ? _last_tick->askqty(0) : _last_tick->bidqty(0);	//isbuy ture   askqty 买价量
		this_qty = round(this_qty*_qty_rate);
		if (decimal::lt(this_qty, 1))  //this_qty <0 时，return this_qty =1 
			this_qty = 1;
	}

	//By Wesley @ 2022.09.13
	//这里要对下单数量做一个修正
	this_qty = min(this_qty, abs(newVol - curPos));//真实-获取仓位			

	//是否开仓，如果持仓大于等于0且买入，或者持仓小于等于0且卖出，就是开仓
	bool isOpen = (isBuy && decimal::ge(curPos, 0)) || (!isBuy && decimal::le(curPos, 0));

	//如果平仓的话
	//对单次下单做一个修正，保证平仓和开仓不会同时下单
	if (!isOpen)
	{
		this_qty = min(this_qty, abs(curPos)); //curPos 现在的仓位
	}									

	/*
	 *	By Wesley @ 2022.12.15
	 *	增加一个对最小下单数量的修正逻辑
	 */
	if (isOpen && decimal::lt(this_qty, _min_open_lots))//if 开仓&&this_qty<min_open_lots（最小开仓数量）
	{
		this_qty = _min_open_lots; 
		_ctx->writeLog(fmtutil::format("Lots of {} changed from {} to {} due to minimum open lots", _code, this_qty, _min_open_lots));
	}

	double buyPx, sellPx;
	if (_price_mode == 2)//价格类型,0-最新价,-1-最优价,1-对手价,2-自动,默认为0
	{
		double mp = (_last_tick->bidqty(0) - _last_tick->askqty(0))*1.0 / (_last_tick->bidqty(0) + _last_tick->askqty(0));
		bool isUp = (mp > 0);
		if (isUp)
		{
			buyPx = _last_tick->askprice(0);
			sellPx = _last_tick->askprice(0);
		}
		else
		{
			buyPx = _last_tick->bidprice(0);
			sellPx = _last_tick->bidprice(0);
		}

		/*
		 *	By Wesley @ 2022.03.07
		 *	如果最后价格为0，再做一个修正			价格为0，可能当日没有交易，所以取上一个交易日的收盘价
		 */																		
		if (decimal::eq(buyPx, 0.0)) //buypx==0，return ture
			buyPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price(); //如果上一tick ==0  取lasttick 收盘价

		if (decimal::eq(sellPx, 0.0))
			sellPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

		buyPx += _comm_info->getPriceTick() * _cancel_times;   // // * 撤单次数    扩大追单步幅
		sellPx -= _comm_info->getPriceTick() * _cancel_times;
	}
	else
	{
		if (_price_mode == -1)//0 最新价 -1 最优价 1 对手价 
		{
			buyPx = _last_tick->bidprice(0);
			sellPx = _last_tick->askprice(0);
		}
		else if (_price_mode == 0)
		{
			buyPx = _last_tick->price();
			sellPx = _last_tick->price();
		}
		else if (_price_mode == 1)
		{
			buyPx = _last_tick->askprice(0);
			sellPx = _last_tick->bidprice(0);
		}

		/*
		 *	By Wesley @ 2022.03.07
		 *	如果最后价格为0，再做一个修正
		 */
		if (decimal::eq(buyPx, 0.0))
			buyPx = decimal::eq(_last_tick->price(), 0.0)? _last_tick->preclose(): _last_tick->price();

		if (decimal::eq(sellPx, 0.0))
			sellPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

		buyPx += _comm_info->getPriceTick() * _price_offset;//价格偏移跳数
		sellPx -= _comm_info->getPriceTick() * _price_offset;
	}
	

	//检查涨跌停价
	bool isCanCancel = true;  
	if (!decimal::eq(_last_tick->upperlimit(), 0) && decimal::gt(buyPx, _last_tick->upperlimit())) //upperlimit涨停价
	{
		_ctx->writeLog(fmtutil::format("Buy price {} of {} modified to upper limit price", buyPx, _code.c_str(), _last_tick->upperlimit()));//买入价，，，改为上限价
		buyPx = _last_tick->upperlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}
	
	if (!decimal::eq(_last_tick->lowerlimit(), 0) && decimal::lt(sellPx, _last_tick->lowerlimit()))
	{
		_ctx->writeLog(fmtutil::format("Sell price {} of {} modified to lower limit price", sellPx, _code.c_str(), _last_tick->lowerlimit()));
		sellPx = _last_tick->lowerlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}

	if (isBuy)
	{
		OrderIDs ids = _ctx->buy(stdCode, buyPx, this_qty, bForceClose);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}
	else
	{
		OrderIDs ids  = _ctx->sell(stdCode, sellPx, this_qty, bForceClose);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}

	_last_place_time = now;
}
/*
 *	设置新的目标仓位
 *	code	合约代码
 *	newVol	新的目标仓位
 */
void WtMinImpactExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0) //code和stdcode不相等  return 
		return;

	//如果原来的目标仓位是DBL_MAX，说明已经进入清理逻辑
	//如果这个时候又设置为0，则直接跳过了
	if (is_clear(_target_pos) && decimal::eq(newVol, 0))
	{
		_ctx->writeLog(fmtutil::format("{} is in clearing processing, position can not be set to 0", stdCode));
		return;
	}

	if (decimal::eq(_target_pos, newVol))
		return;

	_target_pos = newVol;

	if (is_clear(_target_pos))
		_ctx->writeLog(fmtutil::format("{} is set to be in clearing processing", stdCode));
	else
		_ctx->writeLog(fmtutil::format("Target position of {} is set tb be {}", stdCode, _target_pos));

	do_calc();
}

void WtMinImpactExeUnit::clear_all_position(const char* stdCode)
{
	if (_code.compare(stdCode) != 0)
		return;

	_target_pos = DBL_MAX;

	do_calc();
}
