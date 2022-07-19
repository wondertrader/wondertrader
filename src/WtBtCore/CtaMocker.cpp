/*!
 * \file CtaMocker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "CtaMocker.h"
#include "WtHelper.h"
#include "EventNotifier.h"

#include <exception>
#include <boost/filesystem.hpp>

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

const char* CMP_ALG_NAMES[] =
{
	"��",
	">",
	"<",
	">=",
	"<="
};

const char* ACTION_NAMES[] =
{
	"OL",
	"CL",
	"OS",
	"CS",
	"SYN"
};


inline uint32_t makeCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 1 };
	return _auto_context_id.fetch_add(1);
}


CtaMocker::CtaMocker(HisDataReplayer* replayer, const char* name, int32_t slippage /* = 0 */, bool persistData /* = true */, EventNotifier* notifier /* = NULL */)
	: ICtaStraCtx(name)
	, _replayer(replayer)
	, _total_calc_time(0)
	, _emit_times(0)
	, _is_in_schedule(false)
	, _ud_modified(false)
	, _strategy(NULL)
	, _slippage(slippage)
	, _schedule_times(0)
	, _total_closeprofit(0)
	, _notifier(notifier)
	, _has_hook(false)
	, _hook_valid(true)
	, _cur_step(0)
	, _wait_calc(false)
	, _in_backtest(false)
	, _persist_data(persistData)
{
	_context_id = makeCtxId();
}


CtaMocker::~CtaMocker()
{
}

void CtaMocker::dump_stradata()
{
	rj::Document root(rj::kObjectType);

	{//�ֲ����ݱ���
		rj::Value jPos(rj::kArrayType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const PosInfo& pInfo = it->second;

			rj::Value pItem(rj::kObjectType);
			pItem.AddMember("code", rj::Value(stdCode, allocator), allocator);
			pItem.AddMember("volume", pInfo._volume, allocator);
			pItem.AddMember("closeprofit", pInfo._closeprofit, allocator);
			pItem.AddMember("dynprofit", pInfo._dynprofit, allocator);
			pItem.AddMember("lastentertime", pInfo._last_entertime, allocator);
			pItem.AddMember("lastexittime", pInfo._last_exittime, allocator);

			rj::Value details(rj::kArrayType);
			for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
			{
				const DetailInfo& dInfo = *dit;
				rj::Value dItem(rj::kObjectType);
				dItem.AddMember("long", dInfo._long, allocator);
				dItem.AddMember("price", dInfo._price, allocator);
				dItem.AddMember("maxprice", dInfo._max_price, allocator);
				dItem.AddMember("minprice", dInfo._min_price, allocator);
				dItem.AddMember("volume", dInfo._volume, allocator);
				dItem.AddMember("opentime", dInfo._opentime, allocator);
				dItem.AddMember("opentdate", dInfo._opentdate, allocator);

				dItem.AddMember("profit", dInfo._profit, allocator);
				dItem.AddMember("maxprofit", dInfo._max_profit, allocator);
				dItem.AddMember("maxloss", dInfo._max_loss, allocator);
				dItem.AddMember("opentag", rj::Value(dInfo._opentag, allocator), allocator);

				details.PushBack(dItem, allocator);
			}

			pItem.AddMember("details", details, allocator);

			jPos.PushBack(pItem, allocator);
		}

		root.AddMember("positions", jPos, allocator);
	}

	{//�ʽ𱣴�
		rj::Value jFund(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		jFund.AddMember("total_profit", _fund_info._total_profit, allocator);
		jFund.AddMember("total_dynprofit", _fund_info._total_dynprofit, allocator);
		jFund.AddMember("total_fees", _fund_info._total_fees, allocator);
		jFund.AddMember("tdate", _cur_tdate, allocator);

		root.AddMember("fund", jFund, allocator);
	}

	{//�źű���
		rj::Value jSigs(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto& m : _sig_map)
		{
			const char* stdCode = m.first.c_str();
			const SigInfo& sInfo = m.second;

			rj::Value jItem(rj::kObjectType);
			jItem.AddMember("usertag", rj::Value(sInfo._usertag.c_str(), allocator), allocator);

			jItem.AddMember("volume", sInfo._volume, allocator);
			jItem.AddMember("sigprice", sInfo._sigprice, allocator);
			jItem.AddMember("gentime", sInfo._gentime, allocator);

			jSigs.AddMember(rj::Value(stdCode, allocator), jItem, allocator);
		}

		root.AddMember("signals", jSigs, allocator);
	}

	{//����������
		rj::Value jCond(rj::kObjectType);
		rj::Value jItems(rj::kObjectType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it = _condtions.begin(); it != _condtions.end(); it++)
		{
			const char* code = it->first.c_str();
			const CondList& condList = it->second;

			rj::Value cArray(rj::kArrayType);
			for (auto& condInfo : condList)
			{
				rj::Value cItem(rj::kObjectType);
				cItem.AddMember("code", rj::Value(code, allocator), allocator);
				cItem.AddMember("usertag", rj::Value(condInfo._usertag, allocator), allocator);

				cItem.AddMember("field", (uint32_t)condInfo._field, allocator);
				cItem.AddMember("alg", (uint32_t)condInfo._alg, allocator);
				cItem.AddMember("target", condInfo._target, allocator);
				cItem.AddMember("qty", condInfo._qty, allocator);
				cItem.AddMember("action", (uint32_t)condInfo._action, allocator);

				cArray.PushBack(cItem, allocator);
			}

			jItems.AddMember(rj::Value(code, allocator), cArray, allocator);
		}
		jCond.AddMember("settime", _last_cond_min, allocator);
		jCond.AddMember("items", jItems, allocator);

		root.AddMember("conditions", jCond, allocator);
	}

	{
		std::string folder = WtHelper::getOutputDir();
		folder += _name;
		folder += "/";

		std::string filename = folder;
		filename += _name;
		filename += ".json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}
}

void CtaMocker::dump_outputs()
{
	if (!_persist_data)
		return;

	std::string folder = WtHelper::getOutputDir();
	folder += _name;
	folder += "/";
	boost::filesystem::create_directories(folder.c_str());

	std::string filename = folder + "trades.csv";
	std::string content = "code,time,direct,action,price,qty,tag,fee,barno\n";
	if(!_trade_logs.str().empty()) content += _trade_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "closes.csv";
	content = "code,direct,opentime,openprice,closetime,closeprice,qty,profit,maxprofit,maxloss,totalprofit,entertag,exittag,openbarno,closebarno\n";
	if (!_close_logs.str().empty()) content += _close_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "funds.csv";
	content = "date,closeprofit,positionprofit,dynbalance,fee\n";
	if (!_fund_logs.str().empty()) content += _fund_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "signals.csv";
	content = "code,target,sigprice,gentime,usertag\n";
	if (!_sig_logs.str().empty()) content += _sig_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "positions.csv";
	content = "date,code,volume,closeprofit,dynprofit\n";
	if (!_pos_logs.str().empty()) content += _pos_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();
		for (auto it = _user_datas.begin(); it != _user_datas.end(); it++)
		{
			root.AddMember(rj::Value(it->first.c_str(), allocator), rj::Value(it->second.c_str(), allocator), allocator);
		}

		filename = folder;
		filename += "ud_";
		filename += _name;
		filename += ".json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}
}

void CtaMocker::log_signal(const char* stdCode, double target, double price, uint64_t gentime, const char* usertag /* = "" */)
{
	_sig_logs << stdCode << "," << target << "," << price << "," << gentime << "," << usertag << "\n";
}

void CtaMocker::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, const char* userTag, double fee, uint32_t barNo)
{
	_trade_logs << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE") 
		<< "," << price << "," << qty << "," << userTag << "," << fee << "," << barNo << "\n";
}

void CtaMocker::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty, double profit, double maxprofit, double maxloss, 
	double totalprofit /* = 0 */, const char* enterTag /* = "" */, const char* exitTag /* = "" */, uint32_t openBarNo /* = 0 */, uint32_t closeBarNo /* = 0 */)
{
	_close_logs << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
		<< "," << closeTime << "," << closepx << "," << qty << "," << profit << "," << maxprofit << "," << maxloss << ","
		<< totalprofit << "," << enterTag << "," << exitTag << "," << openBarNo << "," << closeBarNo << "\n";
}

bool CtaMocker::init_cta_factory(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	const char* module = cfg->getCString("module");

	DllHandle hInst = DLLHelper::load_library(module);
	if (hInst == NULL)
		return false;

	FuncCreateStraFact creator = (FuncCreateStraFact)DLLHelper::get_symbol(hInst, "createStrategyFact");
	if (creator == NULL)
	{
		DLLHelper::free_library(hInst);
		return false;
	}

	_factory._module_inst = hInst;
	_factory._module_path = module;
	_factory._creator = creator;
	_factory._remover = (FuncDeleteStraFact)DLLHelper::get_symbol(hInst, "deleteStrategyFact");
	_factory._fact = _factory._creator();

	WTSVariant* cfgStra = cfg->get("strategy");
	if (cfgStra)
	{
		_strategy = _factory._fact->createStrategy(cfgStra->getCString("name"), cfgStra->getCString("id"));
		if(_strategy)
		{
			WTSLogger::info("Strategy {}.{} is created,strategy ID: {}", _factory._fact->getName(), _strategy->getName(), _strategy->id());
		}
		_strategy->init(cfgStra->get("params"));
		_name = _strategy->id();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//IDataSink
void CtaMocker::handle_init()
{
	this->on_init();
}

void CtaMocker::handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	this->on_bar(stdCode, period, times, newBar);
}

void CtaMocker::handle_schedule(uint32_t uDate, uint32_t uTime)
{
	this->on_schedule(uDate, uTime);
}

void CtaMocker::handle_session_begin(uint32_t curTDate)
{
	this->on_session_begin(curTDate);
}

void CtaMocker::handle_session_end(uint32_t curTDate)
{
	this->on_session_end(curTDate);
}

void CtaMocker::handle_section_end(uint32_t curTDate, uint32_t curTime)
{
	/*
	 *	By Wesley @ 2022.05.16
	 *	���С�ڽ�����Ҳ��Ҫ������۸񻺴棬��ֹС������
	 *	������Ҫ�����ҹ�̽���
	 */
	_price_map.clear();
}

void CtaMocker::handle_replay_done()
{
	_in_backtest = false;

	if(_emit_times > 0)
	{
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO, 
			"Strategy has been scheduled {} times, totally taking {} us, {:.3f} us each time",
			_emit_times, _total_calc_time, _total_calc_time*1.0 / _emit_times);
	}
	else
	{
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO, 
			"Strategy has been scheduled for {} times", _emit_times);
	}

	dump_outputs();

	dump_stradata();

	if (_has_hook && _hook_valid)
	{
		WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, "Replay done, notify control thread");
		while(_wait_calc)
			_cond_calc.notify_all();
		WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, "Notify control thread the end done");
	}

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Notify strategy the end of backtest");
	this->on_bactest_end();
}

void CtaMocker::proc_tick(const char* stdCode, double last_px, double cur_px)
{
	{
		auto it = _sig_map.find(stdCode);
		if (it != _sig_map.end())
		{
			//if (sInfo->isInTradingTime(_replayer->get_raw_time(), true))
			{
				const SigInfo& sInfo = it->second;
				double price;
				if (decimal::eq(sInfo._desprice, 0.0))
					price = cur_px;
				else
					price = sInfo._desprice;
				do_set_position(stdCode, sInfo._volume, price, sInfo._usertag.c_str(), sInfo._triggered);

				//�������������������ص�on_condition_triggered
				if (sInfo._triggered)
					on_condition_triggered(stdCode, sInfo._volume, cur_px, sInfo._usertag.c_str());
				_sig_map.erase(it);
			}
		}
	}

	update_dyn_profit(stdCode, cur_px);

	//////////////////////////////////////////////////////////////////////////
	//���������
	if (!_condtions.empty())
	{
		auto it = _condtions.find(stdCode);
		if (it == _condtions.end())
			return;

		const CondList& condList = it->second;
		double curPrice = cur_px;
		for (const CondEntrust& entrust : condList)
		{
			/*
			 * ���������tickģʽ���������Ƚ�
			 * �������û�п���tickģʽ���߼��ͷǳ�����
			 * ��Ϊ�����ز��ʱ��tick���ÿ��ߵ���ģ������ģ����ֱ�Ӱ���Ŀ��۸񴥷����������������
			 * ����Ҫ�õ���һ�ʼ۸񣬺͵�ǰ���¼۸���һ���ȼۣ��õ���߽���ұ߽�
			 * ����ֻ�ܼ���ǰ�����ʼ۸�֮���������ģ�������Ҫ�����ʼ۸񶼼����ж�
			 * �������ǵ���ʱ�����Ŀ��۸������ұ߽�֮�䣬˵��Ŀ��۸������ڼ��ǳ��ֹ��ģ�����Ϊ�۸�ƥ��
			 * �������Ǵ��ڵ�ʱ��������Ҫ�ж��ұ߽磬���Դ��ֵ�Ƿ�������������ȡ��߽���Ŀ������Դ����Ϊ��ǰ��
			 * ��������С�ڵ�ʱ��������Ҫ�ж���߽磬����С��ֵ�Ƿ�������������ȡ�ұ߽���Ŀ�������С����Ϊ��ǰ��
			 */

			double left_px = min(last_px, cur_px);
			double right_px = max(last_px, cur_px);

			bool isMatched = false;
			if (!_replayer->is_tick_simulated())
			{
				//���tick���ݲ���ģ��ģ���ʹ�����¼۸�
				switch (entrust._alg)
				{
				case WCT_Equal:
					isMatched = decimal::eq(curPrice, entrust._target);
					break;
				case WCT_Larger:
					isMatched = decimal::gt(curPrice, entrust._target);
					break;
				case WCT_LargerOrEqual:
					isMatched = decimal::ge(curPrice, entrust._target);
					break;
				case WCT_Smaller:
					isMatched = decimal::lt(curPrice, entrust._target);
					break;
				case WCT_SmallerOrEqual:
					isMatched = decimal::le(curPrice, entrust._target);
					break;
				default:
					break;
				}
			}
			else
			{
				//���tick������ģ��ģ���Ҫ����һ��
				switch (entrust._alg)
				{
				case WCT_Equal:
					isMatched = decimal::le(left_px, entrust._target) && decimal::ge(right_px, entrust._target);
					curPrice = entrust._target;
					break;
				case WCT_Larger:
					isMatched = decimal::gt(right_px, entrust._target);
					curPrice = max(left_px, entrust._target);
					break;
				case WCT_LargerOrEqual:
					isMatched = decimal::ge(right_px, entrust._target);
					curPrice = max(left_px, entrust._target);
					break;
				case WCT_Smaller:
					isMatched = decimal::lt(left_px, entrust._target);
					curPrice = min(right_px, entrust._target);
					break;
				case WCT_SmallerOrEqual:
					isMatched = decimal::le(left_px, entrust._target);
					curPrice = min(right_px, entrust._target);
					break;
				default:
					break;
				}
			}


			if (isMatched)
			{
				double price = curPrice;
				double curQty = stra_get_position(stdCode);
				//_replayer->is_tick_enabled() ? newTick->price() : entrust._target;	//���������tick�ز�,����tick���ݵļ۸�,���û�п���,��ֻ�����������۸�
				WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO,
					"Condition order triggered[newprice: {}{}{}], instrument: {}, {} {}",
					cur_px, CMP_ALG_NAMES[entrust._alg], entrust._target, stdCode, ACTION_NAMES[entrust._action], entrust._qty);
				switch (entrust._action)
				{
				case COND_ACTION_OL:
				{
					if (decimal::lt(curQty, 0))
						append_signal(stdCode, entrust._qty, entrust._usertag, price);
					else
						append_signal(stdCode, curQty + entrust._qty, entrust._usertag, price);
				}
				break;
				case COND_ACTION_CL:
				{
					double maxQty = min(curQty, entrust._qty);
					append_signal(stdCode, curQty - maxQty, entrust._usertag, price);
				}
				break;
				case COND_ACTION_OS:
				{
					if (decimal::gt(curQty, 0))
						append_signal(stdCode, -entrust._qty, entrust._usertag, price);
					else
						append_signal(stdCode, curQty - entrust._qty, entrust._usertag, price);
				}
				break;
				case COND_ACTION_CS:
				{
					double maxQty = min(abs(curQty), entrust._qty);
					append_signal(stdCode, curQty + maxQty, entrust._usertag, price);
				}
				break;
				case COND_ACTION_SP:
				{
					append_signal(stdCode, entrust._qty, entrust._usertag, price);
				}
				default: break;
				}

				//ͬһ��bar�������ͬһ����Լ��������,ֻ���ܴ���һ��
				//��������ֱ�����������
				_condtions.erase(it);
				break;
			}
		}
	}
}

void CtaMocker::handle_tick(const char* stdCode, WTSTickData* newTick, bool isBarEnd /* = true */)
{
	double cur_px = newTick->price();

	/*
	 *	By Wesley @ 2022.04.19
	 *	������߼�����һ��
	 *	�������ļ۸񲻴��ڣ�����һ�ʼ۸�������¼�
	 *	������Ҫ��Ϊ��Ӧ�Կ��ռ۸����յ����
	 */
	double last_px = 0;
	auto it = _price_map.find(stdCode);
	if (it != _price_map.end())
		last_px = it->second;
	else
		last_px = cur_px;
	
	_price_map[stdCode] = cur_px;

	//�ȼ���Ƿ�Ҫ�ź�Ҫ����
	//By Wesley @ 2022.04.19
	//��Ȼ����߼�����Ҳ����isBarEnd������һ��
	//������һ�λ���Ҫ����
	proc_tick(stdCode, last_px, cur_px);

	on_tick_updated(stdCode, newTick);

	/*
	 *	By Wesley @ 2022.04.19
	 *	isBarEnd���������tick�طţ������Զ����true����ԶҲ���ᴥ����������߼�
	 *	�����ģ���tick���ݣ������̼�ģ��tick��ʱ��isBarEnd�Ż�Ϊtrue
	 *	����������̼�ģ���tick����ôֱ���ڵ�ǰtick��������߼�
	 *	��������Ŀ����Ϊ������ģ��tick������ontick���µ����ź��ܹ���������
	 *	���������ڻز��ʱ��ɽ���ƫ��̫Զ
	 */
	if(!isBarEnd)
		proc_tick(stdCode, last_px, cur_px);
}


//////////////////////////////////////////////////////////////////////////
//�ص�����
void CtaMocker::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	thread_local static char realPeriod[8] = { 0 };
	fmtutil::format_to(realPeriod, "{}{}", period, times);

	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, realPeriod);

	KlineTag& tag = _kline_tags[key];
	tag._closed = true;

	on_bar_close(stdCode, realPeriod, newBar);
}

void CtaMocker::on_init()
{
	_in_backtest = true;
	if (_strategy)
		_strategy->on_init(this);

	WTSLogger::info("CTA Strategy initialized, with slippage: {}", _slippage);
}

void CtaMocker::update_dyn_profit(const char* stdCode, double price)
{
	auto it = _pos_map.find(stdCode);
	if (it != _pos_map.end())
	{
		PosInfo& pInfo = (PosInfo&)it->second;
		if (pInfo._volume == 0)
		{
			pInfo._dynprofit = 0;
		}
		else
		{
			WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
			double dynprofit = 0;
			for (auto pit = pInfo._details.begin(); pit != pInfo._details.end(); pit++)
			{
				DetailInfo& dInfo = *pit;
				dInfo._profit = dInfo._volume*(price - dInfo._price)*commInfo->getVolScale()*(dInfo._long ? 1 : -1);
				if (dInfo._profit > 0)
					dInfo._max_profit = max(dInfo._profit, dInfo._max_profit);
				else if (dInfo._profit < 0)
					dInfo._max_loss = min(dInfo._profit, dInfo._max_loss);

				dInfo._max_price = std::max(dInfo._max_price, price);
				dInfo._min_price = std::min(dInfo._min_price, price);

				dynprofit += dInfo._profit;
			}

			pInfo._dynprofit = dynprofit;
		}
	}

	double total_dynprofit = 0;
	for (auto& v : _pos_map)
	{
		const PosInfo& pInfo = v.second;
		total_dynprofit += pInfo._dynprofit;
	}

	_fund_info._total_dynprofit = total_dynprofit;
}

void CtaMocker::on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy /* = true */)
{
	//����߼�ȫ��Ǩ�Ƶ�handle_tick��ȥ��
}

void CtaMocker::on_bar_close(const char* code, const char* period, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, newBar);
}

void CtaMocker::on_tick_updated(const char* code, WTSTickData* newTick)
{
	auto it = _tick_subs.find(code);
	if (it == _tick_subs.end())
		return;

	if (_strategy)
		_strategy->on_tick(this, code, newTick);
}

void CtaMocker::on_calculate(uint32_t curDate, uint32_t curTime)
{
	if (_strategy)
		_strategy->on_schedule(this, curDate, curTime);
}

void CtaMocker::enable_hook(bool bEnabled /* = true */)
{
	_hook_valid = bEnabled;

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calculating hook {}", bEnabled?"enabled":"disabled");
}

void CtaMocker::install_hook()
{
	_has_hook = true;

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "CTA hook installed");
}

bool CtaMocker::step_calc()
{
	if (!_has_hook)
	{
		return false;
	}

	//�ܹ���Ϊ4��״̬
	//0-��ʼ״̬��1-oncalc��2-oncalc������3-oncalcdone
	//���ԣ��������0/2����˵��û����ִ���У���Ҫnotify
	bool bNotify = false;
	while (_in_backtest && (_cur_step == 0 || _cur_step == 2))
	{
		_cond_calc.notify_all();
		bNotify = true;
	}

	if(bNotify)
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Notify calc thread, wait for calc done");

	if(_in_backtest)
	{
		_wait_calc = true;
		StdUniqueLock lock(_mtx_calc);
		_cond_calc.wait(_mtx_calc);
		_wait_calc = false;
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done notified");
		_cur_step = (_cur_step + 1) % 4;

		return true;
	}
	else
	{
		_hook_valid = false;
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Backtest exit automatically");
		return false;
	}
}

bool CtaMocker::on_schedule(uint32_t curDate, uint32_t curTime)
{
	_is_in_schedule = true;//��ʼ����,�޸ı��

	_schedule_times++;

	bool isMainUdt = false;
	bool emmited = false;

	for (auto it = _kline_tags.begin(); it != _kline_tags.end(); it++)
	{
		const std::string& key = it->first;
		KlineTag& marker = (KlineTag&)it->second;

		StringVector ay = StrUtil::split(key, "#");
		const char* stdCode = ay[0].c_str();

		if (key == _main_key)
		{
			if (marker._closed)
			{
				isMainUdt = true;
				marker._closed = false;
			}
			else
			{
				isMainUdt = false;
				break;
			}
		}

		WTSSessionInfo* sInfo = _replayer->get_session_info(stdCode, true);

		if (isMainUdt || _kline_tags.empty())
		{
			TimeUtils::Ticker ticker;

			uint32_t offTime = sInfo->offsetTime(curTime, true);
			if (offTime <= sInfo->getCloseTime(true))
			{
				_condtions.clear();
				if(_has_hook && _hook_valid)
				{
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Waiting for resume notify");
					StdUniqueLock lock(_mtx_calc);
					_cond_calc.wait(_mtx_calc);
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc resumed");
					_cur_step = 1;
				}

				on_calculate(curDate, curTime);

				if (_has_hook && _hook_valid)
				{
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done, notify control thread");
					while (_cur_step==1)
						_cond_calc.notify_all();

					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Waiting for resume notify");
					StdUniqueLock lock(_mtx_calc);
					_cond_calc.wait(_mtx_calc);
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc resumed");
					_cur_step = 3;
				}

				if(_has_hook)
					on_calculate_done(curDate, curTime);
				emmited = true;

				if (_condtions.empty())
					_last_cond_min = (uint64_t)curDate * 10000 + curTime;

				_emit_times++;
				_total_calc_time += ticker.micro_seconds();

				if (_has_hook && _hook_valid)
				{
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done, notify control thread");
					while(_cur_step == 3)
						_cond_calc.notify_all();
				}
			}
			else
			{
				WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO, "{} is not trading time,strategy will not be scheduled", curTime);
			}
			break;
		}
	}

	_is_in_schedule = false;//���Ƚ���,�޸ı��
	return emmited;
}


void CtaMocker::on_session_begin(uint32_t curTDate)
{
	_cur_tdate = curTDate;

	//ÿ�������տ�ʼ��Ҫ�Ѷ���ֲ�����
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		PosInfo& pInfo = (PosInfo&)it.second;
		if (!decimal::eq(pInfo._frozen, 0))
		{
			log_debug("{} of {} frozen released on {}", pInfo._frozen, stdCode, curTDate);
			pInfo._frozen = 0;
		}
	}

	/*
	 *	By Wesley @ 2022.04.19
	 *	�½����տ�ʼ��ʱ�򣬼۸񻺴������Ҫ���´���
	 */
	_price_map.clear();

	if (_strategy)
		_strategy->on_session_begin(this, curTDate);
}

void CtaMocker::enum_position(FuncEnumCtaPosCallBack cb)
{
	faster_hashmap<std::string, double> desPos;
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		const PosInfo& pInfo = it.second;
		desPos[stdCode] = pInfo._volume;
	}

	for (auto sit : _sig_map)
	{
		const char* stdCode = sit.first.c_str();
		const SigInfo& sInfo = sit.second;
		desPos[stdCode] = sInfo._volume;
	}

	for (auto v : desPos)
	{
		cb(v.first.c_str(), v.second);
	}
}

void CtaMocker::on_session_end(uint32_t curTDate)
{
	if (_strategy)
		_strategy->on_session_end(this, curTDate);

	uint32_t curDate = curTDate;//_replayer->get_trading_date();

	double total_profit = 0;
	double total_dynprofit = 0;

	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		const PosInfo& pInfo = it->second;
		total_profit += pInfo._closeprofit;
		total_dynprofit += pInfo._dynprofit;

		if(decimal::eq(pInfo._volume, 0.0))
			continue;

		_pos_logs << fmt::format("{},{},{},{:.2f},{:.2f}\n", curDate, stdCode,
			pInfo._volume, pInfo._closeprofit, pInfo._dynprofit);
	}

	_fund_logs << fmt::format("{},{:.2f},{:.2f},{:.2f},{:.2f}\n", curDate,
		_fund_info._total_profit, _fund_info._total_dynprofit,
		_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees);
	
	if (_notifier)
		_notifier->notifyFund("BT_FUND", curDate, _fund_info._total_profit, _fund_info._total_dynprofit,
			_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees);
}

CondList& CtaMocker::get_cond_entrusts(const char* stdCode)
{
	CondList& ce = _condtions[stdCode];
	return ce;
}

//////////////////////////////////////////////////////////////////////////
//���Խӿ�
void CtaMocker::stra_enter_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if(commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	_replayer->sub_tick(_context_id, stdCode);
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ,��ֱ�Ӵ���
	{
		double curQty = stra_get_position(stdCode);
		if(decimal::lt(curQty, 0))
			append_signal(stdCode, qty, userTag);
		else
			append_signal(stdCode, curQty + qty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_SmallerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_LargerOrEqual;
		}

		entrust._action = COND_ACTION_OL;

		condList.emplace_back(entrust);
	}
}

void CtaMocker::stra_enter_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	if(!commInfo->canShort())
	{
		log_error("Cannot short on {}", stdCode);
		return;
	}

	_replayer->sub_tick(_context_id, stdCode);
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ,��ֱ�Ӵ���
	{
		double curQty = stra_get_position(stdCode);
		if(decimal::gt(curQty, 0))
			append_signal(stdCode, -qty, userTag);
		else
			append_signal(stdCode, curQty - qty, userTag);

	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_OS;

		condList.emplace_back(entrust);
	}
}

void CtaMocker::stra_exit_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	//��ȡ��ƽ�ֲ�
	double curQty = stra_get_position(stdCode, true);
	if (decimal::le(curQty, 0))
		return;

	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ,��ֱ�Ӵ���
	{
		double maxQty = min(curQty, qty);
		append_signal(stdCode, curQty - qty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_CL;

		condList.emplace_back(entrust);
	}
}

void CtaMocker::stra_exit_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	if (!commInfo->canShort())
	{
		log_error("Cannot short on {}", stdCode);
		return;
	}

	double curQty = stra_get_position(stdCode);
	if (decimal::ge(curQty, 0))
		return;

	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ,��ֱ�Ӵ���
	{
		double maxQty = min(abs(curQty), qty);
		append_signal(stdCode, curQty + maxQty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_SmallerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_LargerOrEqual;
		}

		entrust._action = COND_ACTION_CS;

		condList.emplace_back(entrust);
	}
}

double CtaMocker::stra_get_price(const char* stdCode)
{
	if (_replayer)
		return _replayer->get_cur_price(stdCode);

	return 0.0;
}

double CtaMocker::stra_get_day_price(const char* stdCode, int flag /* = 0 */)
{
	if (_replayer)
		return _replayer->get_day_price(stdCode, flag);

	return 0.0;
}

void CtaMocker::stra_set_position(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice /* = 0.0 */, double stopprice /* = 0.0 */)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	//����������գ���Ŀ���λ�������ø���
	if (!commInfo->canShort() && decimal::lt(qty, 0))
	{
		log_error("Cannot short on {}", stdCode);
		return;
	}

	double total = stra_get_position(stdCode, false);
	//���Ŀ���λ�͵�ǰ��λ��һ�µģ�ֱ���˳�
	if (decimal::eq(total, qty))
		return;

	if(commInfo->isT1())
	{
		double valid = stra_get_position(stdCode, true);
		double frozen = total - valid;
		//�����T+1������Ŀ���λ����С�ڶ����λ
		if(decimal::lt(qty, frozen))
		{
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_ERROR, "New position of {} cannot be set to {} due to {} being frozen", stdCode, qty, frozen);
			return;
		}
	}

	_replayer->sub_tick(_context_id, stdCode);
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//û�����ô�����������ֱ������ź�
	{
		append_signal(stdCode, qty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		bool isBuy = decimal::gt(qty, total);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);
		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = isBuy ? WCT_SmallerOrEqual : WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = isBuy ? WCT_LargerOrEqual : WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_SP;

		condList.emplace_back(entrust);
	}
}

void CtaMocker::append_signal(const char* stdCode, double qty, const char* userTag /* = "" */, double price/* = 0.0*/)
{
	double curPx = _price_map[stdCode];

	SigInfo& sInfo = _sig_map[stdCode];
	sInfo._volume = qty;
	sInfo._sigprice = curPx;
	sInfo._desprice = price;
	sInfo._usertag = userTag;
	sInfo._gentime = (uint64_t)_replayer->get_date() * 1000000000 + (uint64_t)_replayer->get_raw_time() * 100000 + _replayer->get_secs();
	sInfo._triggered = !_is_in_schedule;

	log_signal(stdCode, qty, curPx, sInfo._gentime, userTag);

	//save_data();
}

void CtaMocker::do_set_position(const char* stdCode, double qty, double price /* = 0.0 */, const char* userTag /* = "" */, bool bTriggered /* = false */)
{
	PosInfo& pInfo = _pos_map[stdCode];
	double curPx = price;
	if (decimal::eq(price, 0.0))
		curPx = _price_map[stdCode];
	uint64_t curTm = (uint64_t)_replayer->get_date() * 10000 + _replayer->get_min_time();
	uint32_t curTDate = _replayer->get_trading_date();

	//����������ò�����
	if (decimal::eq(pInfo._volume, qty))
		return;

	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
		return;

	//�ɽ���
	double trdPx = curPx;

	double diff = qty - pInfo._volume;
	bool isBuy = decimal::gt(diff, 0.0);
	if (decimal::gt(pInfo._volume*diff, 0))//��ǰ�ֲֺͲ�λ�仯����һ��, ����һ����ϸ, ������������
	{
		pInfo._volume = qty;

		//���T+1���򶳽��λҪ����
		if (commInfo->isT1())
		{
			//ASSERT(diff>0);
			pInfo._frozen += diff;
			log_debug("{} frozen position up to {}", stdCode, pInfo._frozen);
		}
		
		if (_slippage != 0)
		{
			trdPx += _slippage * commInfo->getPriceTick()*(isBuy ? 1 : -1);
		}

		DetailInfo dInfo;
		dInfo._long = decimal::gt(qty, 0);
		dInfo._price = trdPx;
		dInfo._max_price = trdPx;
		dInfo._min_price = trdPx;
		dInfo._volume = abs(diff);
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		strcpy(dInfo._opentag, userTag);
		dInfo._open_barno = _schedule_times;
		pInfo._details.emplace_back(dInfo);
		pInfo._last_entertime = curTm;

		double fee = _replayer->calc_fee(stdCode, trdPx, abs(diff), 0);
		_fund_info._total_fees += fee;

		log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(diff), userTag, fee, _schedule_times);
	}
	else
	{//�ֲַ���Ͳ�λ�仯����һ��,��Ҫƽ��
		double left = abs(diff);
		if (_slippage != 0)
			trdPx += _slippage * commInfo->getPriceTick()*(isBuy ? 1 : -1);

		pInfo._volume = qty;
		if (decimal::eq(pInfo._volume, 0))
			pInfo._dynprofit = 0;
		uint32_t count = 0;
		for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
		{
			DetailInfo& dInfo = *it;
			double maxQty = min(dInfo._volume, left);
			if (decimal::eq(maxQty, 0))
				continue;

			double maxProf = dInfo._max_profit * maxQty / dInfo._volume;
			double maxLoss = dInfo._max_loss * maxQty / dInfo._volume;

			dInfo._volume -= maxQty;
			left -= maxQty;

			if (decimal::eq(dInfo._volume, 0))
				count++;

			double profit = (trdPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;
			_total_closeprofit += profit;
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volume / (dInfo._volume + maxQty);//��ӯҲҪ���ȱ�����
			pInfo._last_exittime = curTm;
			_fund_info._total_profit += profit;

			double fee = _replayer->calc_fee(stdCode, trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//����д�ɽ���¼
			log_trade(stdCode, dInfo._long, false, curTm, trdPx, maxQty, userTag, fee, _schedule_times);
			//����дƽ�ּ�¼
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, maxProf, maxLoss, 
				_total_closeprofit - _fund_info._total_fees, dInfo._opentag, userTag, dInfo._open_barno, _schedule_times);

			if (left == 0)
				break;
		}

		//��Ҫ������Ѿ�ƽ�������ϸ
		while (count > 0)
		{
			auto it = pInfo._details.begin();
			pInfo._details.erase(it);
			count--;
		}

		//���,�������ʣ���,����Ҫ������
		if (left > 0)
		{
			left = left * qty / abs(qty);

			//���T+1���򶳽��λҪ����
			if (commInfo->isT1())
			{
				pInfo._frozen += left;
				log_debug("{} frozen position up to {}", stdCode, pInfo._frozen);
			}

			DetailInfo dInfo;
			dInfo._long = decimal::gt(qty, 0);
			dInfo._price = trdPx;
			dInfo._max_price = trdPx;
			dInfo._min_price = trdPx;
			dInfo._volume = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			dInfo._open_barno = _schedule_times;
			strcpy(dInfo._opentag, userTag);
			pInfo._details.emplace_back(dInfo);
 
			//���ﻹ��Ҫдһ�ʳɽ���¼
			double fee = _replayer->calc_fee(stdCode, trdPx, abs(left), 0);
			_fund_info._total_fees += fee;
			log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(left), userTag, fee, _schedule_times);

			pInfo._last_entertime = curTm;
		}
	}
}

WTSKlineSlice* CtaMocker::stra_get_bars(const char* stdCode, const char* period, uint32_t count, bool isMain /* = false */)
{
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, period);

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);
	else
		strcat(key, "1");

	if (isMain)
	{
		if (_main_key.empty())
			_main_key = key;
		else if (_main_key != key)
			throw std::runtime_error("Main k bars can only be setup once");
	}

	WTSKlineSlice* kline = _replayer->get_kline_slice(stdCode, basePeriod, count, times, isMain);

	bool bFirst = (_kline_tags.find(key) == _kline_tags.end());
	KlineTag& tag = _kline_tags[key];
	tag._closed = false;

	if (kline)
	{
		//double lastClose = kline->close(-1);
		CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _replayer->get_hot_mgr());
		WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
		std::string realCode = stdCode;
		if(cInfo.isExright())
			realCode = realCode.substr(0, realCode.size()-1);
		_replayer->sub_tick(id(), realCode.c_str());
	}

	return kline;
}

WTSTickSlice* CtaMocker::stra_get_ticks(const char* stdCode, uint32_t count)
{
	return _replayer->get_tick_slice(stdCode, count);
}

WTSTickData* CtaMocker::stra_get_last_tick(const char* stdCode)
{
	return _replayer->get_last_tick(stdCode);
}

void CtaMocker::stra_sub_ticks(const char* code)
{
	/*
	 *	By Wesley @ 2022.03.01
	 *	��������tick���ڱ��ؼ�һ��
	 *	tick���ݻص���ʱ���ȼ��һ��
	 */
	_tick_subs.insert(code);

	_replayer->sub_tick(_context_id, code);
}

WTSCommodityInfo* CtaMocker::stra_get_comminfo(const char* stdCode)
{
	return _replayer->get_commodity_info(stdCode);
}

std::string CtaMocker::stra_get_rawcode(const char* stdCode)
{
	return _replayer->get_rawcode(stdCode);
}

uint32_t CtaMocker::stra_get_tdate()
{
	return _replayer->get_trading_date();
}

uint32_t CtaMocker::stra_get_date()
{
	return _replayer->get_date();
}

uint32_t CtaMocker::stra_get_time()
{
	return _replayer->get_min_time();
}

double CtaMocker::stra_get_fund_data(int flag)
{
	switch (flag)
	{
	case 0:
		return _fund_info._total_profit - _fund_info._total_fees + _fund_info._total_dynprofit;
	case 1:
		return _fund_info._total_profit;
	case 2:
		return _fund_info._total_dynprofit;
	case 3:
		return _fund_info._total_fees;
	default:
		return 0.0;
	}
}

void CtaMocker::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void CtaMocker::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void CtaMocker::stra_log_warn(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_WARN, message);
}

void CtaMocker::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

const char* CtaMocker::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void CtaMocker::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

uint64_t CtaMocker::stra_get_first_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_UINT64;

	return pInfo._details[0]._opentime;
}

uint64_t CtaMocker::stra_get_last_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_UINT64;

	return pInfo._details[pInfo._details.size() - 1]._opentime;
}

uint64_t CtaMocker::stra_get_last_exittime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	return pInfo._last_exittime;
}

double CtaMocker::stra_get_last_enterprice(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_DOUBLE;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_DOUBLE;

	return pInfo._details[pInfo._details.size() - 1]._price;
}

double CtaMocker::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, const char* userTag /* = "" */)
{
	//By Wesley @ 2022.05.22
	//������źţ�˵��������ָ���û�ȵ���һ��tick�������û����ڶ�ȡ��λ
	//��������û���ȡ������Ҫ����
	auto sit = _sig_map.find(stdCode);
	if (sit != _sig_map.end())
	{
		return sit->second._volume;
	}

	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	double ret = 0;
	const PosInfo& pInfo = it->second;
	if (strlen(userTag) == 0)
	{
		if (bOnlyValid)
		{
			//ֻ��userTagΪ�յ�ʱ��ʱ�򣬲Ż���bOnlyValid
			//���������ϣ�ֻ�ж�ͷ�Ż��������
			//�����ط�Ҫ��֤����ͷ�ֲֵĻ���_frozenҪΪ0
			return pInfo._volume - pInfo._frozen;
		}
		else
			return pInfo._volume;
	}

	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._volume;
	}

	return 0;
}

double CtaMocker::stra_get_position_avgpx(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._volume == 0)
		return 0.0;

	double amount = 0.0;
	for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
	{
		const DetailInfo& dInfo = *dit;
		amount += dInfo._price*dInfo._volume;
	}

	return amount / pInfo._volume;
}

double CtaMocker::stra_get_position_profit(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

uint64_t CtaMocker::stra_get_detail_entertime(const char* stdCode, const char* userTag)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._opentime;
	}

	return 0;
}

double CtaMocker::stra_get_detail_cost(const char* stdCode, const char* userTag)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._price;
	}

	return 0.0;
}

double CtaMocker::stra_get_detail_profit(const char* stdCode, const char* userTag, int flag /* = 0 */)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		switch (flag)
		{
		case 0:
			return dInfo._profit;
		case 1:
			return dInfo._max_profit;
		case -1:
			return dInfo._max_loss;
		case 2:
			return dInfo._max_price;
		case -2:
			return dInfo._min_price;
		}
	}

	return 0.0;
}


