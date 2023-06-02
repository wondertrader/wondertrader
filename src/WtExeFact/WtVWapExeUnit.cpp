/*
23.5.23--zhaoyk--VWAP
*/
#include "WtVWapExeUnit.h"

#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/fmtlib.h"
#include "../Includes/WTSSessionInfo.hpp"


extern const char* FACT_NAME;

WtVWapExeUnit::WtVWapExeUnit()
	:_last_tick(NULL)
	,_comm_info(NULL)//Ʒ����Ϣ
	,_ord_sticky(0)
	,_cancel_cnt(0)
	,_channel_ready(false)
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
{
}

WtVWapExeUnit::~WtVWapExeUnit()
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
inline bool is_clear(double target)
{
	return (target == DBL_MAX);
}
inline uint32_t calTmSecs(uint32_t begintime, uint32_t endtime) //����ִ��ʱ�䣺s
{
	return   ((endtime / 100) * 3600 + (endtime % 100) * 60) - ((begintime / 100) * 3600 + (begintime % 100) * 60);

}
inline double calTmStamp(uint32_t actiontime) //����tickʱ�������ĸ�ʱ�䵥Ԫ
{
	string timestamp = to_string(actiontime);
	int hour = stoi(timestamp.substr(0, 2));
	int minute = stoi(timestamp.substr(2, 2));
	double total_minute = 0;
	if (hour < 9 || (hour == 9 && minute < 30)) {
		total_minute = 0;
	}
	else if (hour < 11 || (hour == 11 && minute <= 30)) {
		total_minute = (hour - 9) * 60 + minute - 30;
	}
	else if (hour < 13 || (hour == 13 && minute < 30)) {
		total_minute = 120 + (hour - 11) * 60 + minute;
	}
	else if (hour < 15 || (hour == 15 && minute <= 0)) {
		total_minute = 240 + (hour - 13) * 60 + minute - 30;
	}
	else {
		total_minute = 240;
	}
	if (timestamp >= "113000000" && timestamp < "130000000") {
		total_minute = 120;
	}
	total_minute += stoi(timestamp.substr(4, 2)) / 60;
	total_minute += stoi(timestamp.substr(6, 3)) / 60000;
	return total_minute ;//����Ӧ��+1����Ӧvector ������-1
}
const char * WtVWapExeUnit::getFactName()
{
	return FACT_NAME;
}

const char * WtVWapExeUnit::getName()
{
	return "WtVWapExeUnit";
}

void WtVWapExeUnit::init(ExecuteContext * ctx, const char * stdCode, WTSVariant * cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);//��ȡƷ�ֲ���
	if (_comm_info)
		_comm_info->retain();
	
	_sess_info = ctx->getSessionInfo(stdCode);//��ȡ����ʱ��ģ����Ϣ
	if (_sess_info)
		_sess_info->retain();
	_begin_time = cfg->getUInt32("begin_time");
	_end_time = cfg->getUInt32("end_time");
	_ord_sticky = cfg->getUInt32("ord_sticky");//�ҵ�ʱ��
	_tail_secs = cfg->getUInt32("tail_secs");//ִ��β��ʱ��
	_total_times = cfg->getUInt32("total_times");//��ִ�д���
	_price_mode = cfg->getUInt32("price_mode");
	_price_offset = cfg->getUInt32("price_offset");
	_order_lots = cfg->getDouble("lots");		//���η�������
	if (cfg->has("minopenlots"))
	_min_open_lots = cfg->getDouble("minopenlots");	//��С��������
	_fire_span = (_total_secs - _tail_secs) / _total_times;		//���η���ʱ����,Ҫȥ��β��ʱ�����,�����Ļ�,���ʣ�����������һ�����׷����Ļ�����

	ctx->writeLog(fmt::format("ִ�е�ԪWtVWapExeUnit[{}] ��ʼ�����,������ʱ {} ��,ִ��ʱ�� {} ��,��βʱ�� {} ��", stdCode, _ord_sticky, _total_secs, _tail_secs).c_str());
	_total_secs = calTmSecs(_begin_time, _end_time);//ִ����ʱ�䣺��

	std::string filename = "Vwap_";
	filename += _comm_info->getName();
	filename += ".txt";
	if (!StdFile::exists(filename.c_str()))
	{
		_ctx->writeLog(fmtutil::format("Vwap file {} not exists ,check and return.", filename.c_str()));
		return;
	}

	ifstream file(filename.c_str());
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			stringstream s(line);
			string prz;
			while (getline(s, prz, ',')) {
				VwapAim.push_back(stod(prz));
			}
		}
		file.close();
	}
} 

void WtVWapExeUnit::on_order(uint32_t localid, const char * stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	if (!_orders_mon.has_order(localid))
		return;
	if (isCanceled || leftover == 0)
	{
		_orders_mon.erase_order(localid);
		if (_cancel_cnt > 0)
		{
			_cancel_cnt--;
			_ctx->writeLog(fmtutil::format("[{}@{}] Order of {} cancelling done, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));
		}
	}
	
	if (leftover == 0 && !isCanceled) {
		_cancel_times = 0;
		_ctx->writeLog(fmtutil::format("Order {} has filled", localid));
	}
	//���ȫ�������ѳ���,���ʱ��һ��������Ҫ��ʱ�������ҵ���ʱ�� 
	if (isCanceled && _cancel_cnt == 0)
	{
		double realPos = _ctx->getPosition(stdCode);
		if (!decimal::eq(realPos, _this_target))
		{
			_ctx->writeLog(fmtutil::format("Order {} of {} canceled, re_fire will be done", localid, stdCode));
			_cancel_times++;
			//�����Ժ��ط�,һ���Ǽӵ��ط�;����С�µ�����У��
			fire_at_once(max(_min_open_lots, _this_target - realPos));
		}
	}

	if (!isCanceled&&_cancel_cnt != 0) {//һ��������⣬��Ҫ���ؼ��  �������� cnt++,onorder��Ӧ����Ż�--
		_ctx->writeLog(fmtutil::format("Order {} of {}  hasn't canceled, error will be return ", localid, stdCode));
		return;
	}
}

void WtVWapExeUnit::on_channel_ready()
{
	_channel_ready = true;
	double undone = _ctx->getUndoneQty(_code.c_str());
	if (!decimal::eq(undone, 0) && !_orders_mon.has_order())
	{//δ��ɵ����ڼ���У�����
		/*
		 *	���δ��ɵ���Ϊ0����OMSû�ж���
		 *	��˵����δ��ɵ����ڼ��֮��,ȫ��������
		 *	��Ϊ��Щ����û�б��ض����ţ��޷�ֱ�ӽ��й���
		 *	������������Ǹ�������ʱ���ϴε�δ��ɵ������ⲿ�Ĺҵ�
		 */
		_ctx->writeLog(fmt::format("{} unmanaged orders of {},cancel all", undone, _code).c_str());
		
		bool isBuy = (undone > 0);
		OrderIDs ids =_ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();

		_ctx->writeLog(fmtutil::format("[{}@{}]cancelcnt -> {}", __FILE__, __LINE__, _cancel_cnt));
	}
	else if (decimal::eq(undone, 0) && _orders_mon.has_order())
	{	/*
		 *	By Wesey @ 2021.12.13
		 *	���δ��ɵ�Ϊ0������OMS�����ж�����
		 *	˵��OMS���Ǵ�����Ҫ���������Ȼ��ʱ�����ͻ����
		 *	���������һ���Ƕ��������Ժ�֮ǰ�³�ȥ�Ķ�������û���������͵���̨
		 *	����������Ҫ��������ض���
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


void WtVWapExeUnit::on_tick(WTSTickData * newTick)
{
	if (newTick == NULL || _code.compare(newTick->code()) != 0)
		return;

	bool isFirstTick = false;
	//ԭ��tick��Ϊ�� ��Ҫ�ͷŵ�
	if (_last_tick) {
		_last_tick->release();
	}
	else {
		isFirstTick = true;
		//�������ʱ�䲻�ڽ���ʱ��,�������һ���Ǽ��Ͼ��۵��������,�µ���ʧ��,����ֱ�ӹ��˵��������
		if (_sess_info != NULL && !_sess_info->isInTradingTime(newTick->actiontime() / 100000))
			return;
	}
	//�µ�tick������Ҫ����
	_last_tick = newTick;
	_last_tick->retain();

	if (isFirstTick)//����ǵ�һ��tick,����Ŀ���λ,���������µ�
	{
		double newVol = _target_pos;
		const char* stdCode = _code.c_str();
		double undone = _ctx->getUndoneQty(stdCode);
		double realPos = _ctx->getPosition(stdCode);
		if (!decimal::eq(newVol, undone + realPos))
		{//����ǵ�һ��TICK����Ŀ����==δ���+��λ���˳� 
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
					_ctx->writeLog(fmt::format("Order expired, cancelcnt updated to {}", _cancel_cnt).c_str());//�������ڣ�����������
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

void WtVWapExeUnit::on_trade(uint32_t localid, const char * stdCode, bool isBuy, double vol, double price)
{//��ontick�д���
}
/*
�µ�����ر�
*/
void WtVWapExeUnit::on_entrust(uint32_t localid, const char * stdCode, bool bSuccess, const char * message)
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
void WtVWapExeUnit::do_calc()
{
	CalcFlag flag(&_in_calc);
	if (flag)
		return;

	StdUniqueLock lock(_mtx_calc);
	const char* code = _code.c_str();
	double undone = _ctx->getUndoneQty(code);
	double newVol = get_real_target(_target_pos);
	double realPos = _ctx->getPosition(code);
	double diffQty = newVol - realPos;//Ŀ���
	if (!_channel_ready)
		return;
	//�����ڳ����Ķ���,���ܽ�����һ�ּ���
	if (_cancel_cnt != 0)
	{
		_ctx->writeLog(fmt::format("{}����δ��ɵĳ���ָ���ʱ�˳�����ִ��", _code).c_str());
		return;
	}
	if (decimal::eq(diffQty, 0))
		return;
	//ÿһ�η���Ҫ���ϳɽ�,���������δ��ɵ�,˵����һ��û���
	//��δ��ɶ���&&��ʵ�ʲ�λ�䶯�����෴
	//����Ҫ�������ж���
	if (decimal::lt(diffQty*undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);//undone>0,isbuy=1
		OrderIDs ids = _ctx->cancel(code, isBuy);
		if (!ids.empty())
		{
			_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
			_cancel_cnt += ids.size();
			_ctx->writeLog(fmtutil::format("[{}@{}] live opposite order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));//�෴�Ķ���ȡ��
		}
		return;
	}
	if (!decimal::eq(undone, 0))
	{
		_ctx->writeLog(fmt::format("{}��һ���йҵ�δ���,��ʱ�˳�����ִ��", _code).c_str());
		return;
	}
	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmt::format("{}û�����µ�tick���ݣ��˳�ִ���߼�", _code).c_str());
		return;
	}
	double curPos = realPos;

	if (decimal::eq(curPos, newVol))
	{
		//��ǰ��λ������Ŀ���λƥ��ʱ���������ȫ����ֵ�������ֱ���˳�������
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
	double InminsTm = calTmStamp(_last_tick->actiontime());//��ǰtick����vwap240�����ڵĵڼ�(-1)����
	double aimQty = VwapAim[InminsTm];//ȡ����Ӧʱ�̵�Ŀ��vwapaim ��������

	uint32_t leftTimes = _total_times - _fired_times;
	_ctx->writeLog(fmt::format("�� {} �η���", _fired_times+1).c_str());
	_Vwap_vol = aimQty - curPos;//���ڱ����ӷ�������Ӧ��VWapVol
	bool bNeedShowHand = false;
	double curQty = 0;
	if (leftTimes == 0 && !decimal::eq(diffQty, 0))
	{
		bNeedShowHand = true;
		curQty = max(diffQty, _min_open_lots);
	}
	else {
		curQty = max(_Vwap_vol, _min_open_lots)* abs(diffQty) / diffQty;//curqty=��λԤ����sum
	}
	//�趨����Ŀ���λ
	_this_target = realPos + curQty;

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
	else
	{
		targetPx = isBuy ? curTick->askprice(0) : curTick->bidprice(0);
	}
	if (bNeedShowHand) {
		targetPx += _comm_info->getPriceTick() * 5 * (isBuy ? 1 : -1);
	}
	else if (_price_offset != 0)
	{
		targetPx += _comm_info->getPriceTick() * _price_offset * (isBuy ? 1 : -1);
	}

	OrderIDs ids;
	if (curQty > 0)
	{
		ids = _ctx->buy(code, targetPx, abs(curQty));
	}
	else{
		ids = _ctx->sell(code, targetPx, abs(curQty));
	}
	_orders_mon.push_order(ids.data(), ids.size(), now);
	_last_fire_time = now;
	_fired_times += 1;

	curTick->release();
}

void WtVWapExeUnit::fire_at_once(double qty)
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
	if (_price_mode == 0) {
		targetPx = curTick->price();
	}
	else if (_price_mode == 1) {
		targetPx = isBuy ? curTick->askprice(0) : curTick->bidprice(0);
	}
	else // if(_price_mode == 2)
	{
		targetPx = isBuy ? curTick->askprice(0) : curTick->bidprice(0);//���뷽�����ۣ������������
	}

	targetPx += _comm_info->getPriceTick()*_cancel_times *(isBuy ? 1 : -1);//���Ӽ۸�ƫ��
	//��󷢳�ָ��
	OrderIDs ids;
	if (qty > 0)
		ids = _ctx->buy(code, targetPx, abs(qty));
	else
		ids = _ctx->sell(code, targetPx, abs(qty));

	_orders_mon.push_order(ids.data(), ids.size(), now);

	curTick->release();
}

void WtVWapExeUnit::set_position(const char * stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	if (decimal::eq(newVol, _target_pos))
		return;
	
	_target_pos = newVol;
	
	_fired_times = 0;//��ִ�д���

	do_calc();
}

void WtVWapExeUnit::on_channel_lost()
{
}
