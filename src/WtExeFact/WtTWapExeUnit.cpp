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
inline uint32_t calTmSecs(uint32_t begintime, uint32_t endtime) //����ִ��ʱ�䣺s
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

	_comm_info = ctx->getCommodityInfo(stdCode);//��ȡƷ�ֲ���
	if (_comm_info)
		_comm_info->retain();
	/***---begin---23.5.18---zhaoyk***/
	_sess_info = ctx->getSessionInfo(stdCode);//��ȡ����ʱ��ģ����Ϣ
	if (_sess_info)
		_sess_info->retain();
	/***---end---23.5.18---zhaoyk***/

	_ord_sticky = cfg->getUInt32("ord_sticky");
	_begin_time= cfg->getUInt32("begin_time");
	_end_time = cfg->getUInt32("end_time");
	_total_secs = cfg->getUInt32("total_secs");
	_tail_secs = cfg->getUInt32("tail_secs");
	_total_times = cfg->getUInt32("total_times");
	_price_mode = cfg->getUInt32("price_mode");
	_price_offset = cfg->getUInt32("price_offset");
	_order_lots = cfg->getDouble("lots");		//���η�������
	/***---begin---23.5.22---zhaoyk***/
	if (cfg->has("minopenlots"))
	_min_open_lots = cfg->getDouble("minopenlots");	//��С��������
	_total_secs = calTmSecs(_begin_time, _end_time);//ִ����ʱ�䣺��
	/***---end---23.5.22---zhaoyk***/
	_fire_span = (_total_secs - _tail_secs) / _total_times;		//���η���ʱ����,Ҫȥ��β��ʱ�����,�����Ļ�,���ʣ�����������һ�����׷����Ļ�����

	ctx->writeLog(fmt::format("ִ�е�ԪWtTWapExeUnit[{}] ��ʼ�����,������ʱ {} ��,ִ��ʱ�� {} ��,��βʱ�� {} ��,���ʱ�� {} ��", stdCode, _ord_sticky, _total_secs, _tail_secs, _fire_span).c_str());
}

void WtTWapExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	if (!_orders_mon.has_order(localid))
		return;
	//���ڻر����������������  �жϻر� �Ѿ���������û��ʣ�൥    

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
	//���ȫ�������ѳ���,���ʱ��һ��������Ҫ��ʱ�������ҵ���ʱ�� 
	if (isCanceled && _cancel_cnt == 0)
	{
		
		double realPos = _ctx->getPosition(stdCode);
		if (!decimal::eq(realPos, _this_target))//��ʵ��λ�ͱ���Ŀ���λ�����ʱ��
		{
			/***---begin---23.5.22---zhaoyk***/
			_ctx->writeLog(fmtutil::format("Order {} of {} canceled, re_fire will be done", localid, stdCode));
			_cancel_times++;
			
			//�����Ժ��ط�,һ���Ǽӵ��ط�;����С�µ�����У��
			//fire_at_once(_this_target - realPos);
			fire_at_once(max(_min_open_lots, _this_target - realPos));
			/***---end---23.5.22---zhaoyk***/
		}
	}
	/***---begin---23.5.22---zhaoyk***/
	//���ڣ�isCanceled&&_cancel_cnt!=0�������,bug��Ҫ���ؼ��
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
		//��˵����δ��ɵ����ڼ��֮��,�ȳ���
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
		 *	���δ��ɵ�Ϊ0������OMS�����ж�����
		 *	˵��OMS���Ǵ�����Ҫ���������Ȼ��ʱ�����ͻ����
		 *	���������һ���Ƕ��������Ժ�֮ǰ�³�ȥ�Ķ�������û���������͵���̨
		 *	����������Ҫ��������ض���
		 */
		_ctx->writeLog(fmtutil::format("Local orders of {} not confirmed in trading channel, clear all", _code.c_str()));
		_orders_mon.clear_orders();
	}
	else//�ο�Minimpactunit
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
	//���ԭ����tick��Ϊ��,��Ҫ�ͷŵ�
	if (_last_tick)
	{
		_last_tick->release();
	}
	/***---begin---23.5.18---zhaoyk***/
	else
	{
		isFirstTick = true;
		//�������ʱ�䲻�ڽ���ʱ��,�������һ���Ǽ��Ͼ��۵��������,�µ���ʧ��,����ֱ�ӹ��˵��������
		if (_sess_info != NULL && !_sess_info->isInTradingTime(newTick->actiontime() / 100000))
			return;
	}
	/***---end---23.5.18---zhaoyk***/

	//�µ�tick����,Ҫ����
	_last_tick = newTick;
	_last_tick->retain();

	/*
	*	������Կ���һ��
	*	���д����һ�ζ���ȥ�ĵ��Ӳ����ﵽĿ���λ
	*	��ô���µ��������ݽ�����ʱ������ٴδ��������߼�
	*/

	if (isFirstTick)	//����ǵ�һ��tick,����Ŀ���λ,���������µ�
	{
		double newVol = _target_pos;
		const char* stdCode = _code.c_str();
		double undone = _ctx->getUndoneQty(stdCode);
		double realPos = _ctx->getPosition(stdCode);
		if (!decimal::eq(newVol, undone + realPos)) //��λ�仯Ҫ���� 
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
	//���ô���,������ontick�ﴥ����
}

void WtTWapExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!bSuccess)
	{
		//��������ҷ���ȥ�Ķ���,�ҾͲ�����
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
	//���ݼ۸�ģʽ����,ȷ��ί�л�׼�۸�: 0-���¼�,1-���ż�,2-���ּ�
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

	//��󷢳�ָ��
	OrderIDs ids;
	if (qty > 0)
		ids = _ctx->buy(code, targetPx, abs(qty));
	else
		ids = _ctx->sell(code, targetPx, abs(qty));

	_orders_mon.push_order(ids.data(), ids.size(), now);

	curTick->release();
}

void WtTWapExeUnit::do_calc()
{
	CalcFlag flag(&_in_calc);
	if (flag)
		return;

	if (!_channel_ready)
		return;

	/***---bigin---23.5.22---zhaoyk***/
//�����һ��������Ҫԭ����ʵ�̹����з���
//���޸�Ŀ���λ��ʱ�򣬻ᴥ��һ��do_calc
//��ontickҲ�ᴥ��һ��do_calc�����ε����Ǵ������̷ֱ߳𴥷��ģ����Ի����ͬʱ���������
//������������ͻ���������
//���������ԭ����SimpleExecUnitû�г��֣���ΪSimpleExecUnitֻ��set_position��ʱ�򴥷�
	StdUniqueLock lock(_mtx_calc);
	/***---end---23.5.22---zhaoyk***/
	const char* code = _code.c_str();
	double undone = _ctx->getUndoneQty(code);
	
	/***---begin---23.5.22---zhaoyk***/
	double newVol = get_real_target(_target_pos);//��ʵĿ��۸�
	double realPos = _ctx->getPosition(code);
	double diffQty = newVol - realPos;

	//�����ڳ����Ķ���,���ܽ�����һ�ּ���
	if (_cancel_cnt != 0)
	{
		_ctx->writeLog(fmt::format("{}����δ��ɳ���ָ��,��ʱ�˳�����ִ��", _code).c_str());
		return;
	}

	if (decimal::eq(diffQty, 0))
		return;
	//ÿһ�η���Ҫ���ϳɽ�,���������δ��ɵ�,˵����һ��û���
	//��δ��ɶ�������ʵ�ʲ�λ�䶯�����෴
	//����Ҫ�������ж���
	if (decimal::lt(diffQty * undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);   
		OrderIDs ids = _ctx->cancel(code, isBuy);
		if (!ids.empty())
		{
			_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
			_cancel_cnt += ids.size();
			_ctx->writeLog(fmtutil::format("[{}@{}] live opposite order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));//�෴�Ķ�����ȡ��
		}
		return;
	}
	//��Ϊ����ʷ�������������в���Ҫ������δ��ɵ������ݲ�����
	if (!decimal::eq(undone, 0))
	{
		_ctx->writeLog(fmt::format("{}��һ���йҵ�δ���,��ʱ�˳�����ִ��", _code).c_str());
		return;
	}
	double curPos = realPos;
	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmt::format("{}û������tick����,�˳�ִ���߼�", _code).c_str());
		return;
	}
	if (decimal::eq(curPos, newVol))
	{
		//��ǰ��λ�����²�λƥ��ʱ���������ȫ����ֵ�������ֱ���˳�������
		if (!is_clear(_target_pos))
			return;

		//�������ֵ����󣬻�Ҫ�ٽ��жԱ�
		//�����ͷΪ0��˵���Ѿ�ȫ��������ˣ���ֱ���˳�
		double lPos = _ctx->getPosition(code, true, 1); 
		if (decimal::eq(lPos, 0))
			return;

		//������ж�ͷ��λ����Ŀ���λ����Ϊ��0��ǿ�ƴ���                      
		newVol = -min(lPos, _order_lots);
		_ctx->writeLog(fmtutil::format("Clearing process triggered, target position of {} has been set to {}", _code.c_str(), newVol));
	}

	//�������ϴ�û�и��µ�tick���������Ȳ��µ�����ֹ����ǰ�����µ�����ͨ������
	uint64_t curTickTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog(fmtutil::format("No tick of {} updated, {} <= {}, execute later", _code, curTickTime, _last_tick_time));
		return;
	}
	_last_tick_time = curTickTime;
	double this_qty = _order_lots; 	//���η�������
	/***---end---23.5.26---zhaoyk***/

	uint32_t leftTimes = _total_times - _fired_times;
	/***---begin---23.5.22---zhaoyk***/
	//_ctx->writeLog(fmt::format("�� {} �η���", _fired_times).c_str());
	_ctx->writeLog(fmt::format("�� {} �η���", _fired_times+1).c_str());

	/***---end---23.5.22---zhaoyk***/
	bool bNeedShowHand = false;
	//ʣ�����Ϊ0,ʣ��������Ϊ0,˵��Ҫȫ������ȥ��
	//ʣ�����Ϊ0,˵���Ѿ����˶���ʱ����,������ʱ����δ�������,����Ҫ����
	/***---begin---23.5.22---zhaoyk***/
	/*
	if (leftTimes == 0 && !decimal::eq(diffQty, 0))
		bNeedShowHand = true;

	double curQty = 0;

	//���ʣ��˴�Ϊ0 ,����Ҫȫ���µ�
	//����,ȡ��(ʣ������/ʣ�����)��1�����ֵ,����СΪһ��,����Ҫע����Ŵ���
	if (leftTimes == 0)
		curQty = diffQty;
	else
		curQty = std::max(1.0, round(abs(diffQty) / leftTimes)) * abs(diffQty) / diffQty;//��Сһ���µ����ӷ��� ���ڻ�/��Ʊ   //��С�µ�����
	*/
	//���ʣ��˴�Ϊ0 ,����Ҫȫ���µ�
	//����,ȡ��(ʣ������/ʣ�����)��1�����ֵ,����СΪ_min_open_lots,����Ҫע����Ŵ���
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
	
	//�趨����Ŀ���λ
	_this_target = realPos + curQty;	//�ֲ�λ��+�����µ���

	WTSTickData* curTick = _last_tick;
	uint64_t now = TimeUtils::getLocalTimeNow();
	bool isBuy = decimal::gt(diffQty, 0);
	double targetPx = 0;
	//���ݼ۸�ģʽ����,ȷ��ί�л�׼�۸�: 0-���¼�,1-���ż�,2-���ּ�
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

	//�����Ҫȫ������,��۸�ƫ��5��,�Ա���ִ��
	if (bNeedShowHand) //  last showhand time
	{
		targetPx += _comm_info->getPriceTick() * 5 * (isBuy ? 1 : -1);
	}
	else if (_price_offset != 0)	//��������˼۸�ƫ��,ҲҪ����һ��
	{
		targetPx += _comm_info->getPriceTick() * _price_offset * (isBuy ? 1 : -1);
	}

	//��󷢳�ָ��
	OrderIDs ids;
	if (curQty > 0)
		ids = _ctx->buy(code, targetPx, abs(curQty));
	else
		ids = _ctx->sell(code, targetPx, abs(curQty));

	_orders_mon.push_order(ids.data(), ids.size(), now);
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

