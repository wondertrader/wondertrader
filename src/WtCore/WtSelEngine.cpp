#include "WtSelEngine.h"
#include "WtDtMgr.h"
#include "WtSelTicker.h"
#include "TraderAdapter.h"
#include "WtHelper.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/TimeUtils.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Share/StrUtil.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/decimal.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#include <atomic>

USING_NS_WTP;

inline uint32_t makeTaskId()
{
	static std::atomic<uint32_t> _auto_task_id{ 1 };
	return _auto_task_id.fetch_add(1);
}


WtSelEngine::WtSelEngine()
	: _terminated(false)
	, _cfg(NULL)
{
}


WtSelEngine::~WtSelEngine()
{
}

void WtSelEngine::on_session_end()
{
	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, false);
}

void WtSelEngine::on_session_begin()
{
	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, true);

	_ready = true;
}

void WtSelEngine::on_init()
{
	if (_evt_listener)
		_evt_listener->on_initialize_event();
}

void WtSelEngine::handle_push_quote(WTSTickData* curTick, uint32_t hotFlag)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(curTick, hotFlag);
}

void WtSelEngine::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}-{}-{}", stdCode, period, times);

	const SubList& sids = _bar_sub_map[key];
	for (auto it = sids.begin(); it != sids.end(); it++)
	{
		uint32_t sid = it->first;
		auto cit = _ctx_map.find(sid);
		if (cit != _ctx_map.end())
		{
			SelContextPtr& ctx = (SelContextPtr&)cit->second;
			ctx->on_bar(stdCode, period, times, newBar);
		}
	}

	WTSLogger::info("KBar [{}] @ {} closed", key, period[0] == 'd' ? newBar->date : newBar->time);
}

void WtSelEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	WtEngine::on_tick(stdCode, curTick);

	_data_mgr->handle_push_quote(stdCode, curTick);

	//�������ʵ����, ��Ҫ���ݸ�ִ����
	{
		_exec_mgr.handle_tick(stdCode, curTick);
	}

	/*
	auto sit = _tick_sub_map.find(stdCode);
	if (sit != _tick_sub_map.end())
	{
		const SubList& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end() && curTick->volume())
			{
				SelContextPtr& ctx = (SelContextPtr&)cit->second;
				ctx->on_tick(stdCode, curTick);
			}
		}
	}
	*/

	/*
	 *	By Wesley @ 2022.02.07
	 *	��������һ�����׵ĵ���
	 *	��һ����鶩�ı�ǣ�������Ϊ0�����޸�Ȩģʽ����ֱ�Ӱ���ԭʼ���봥��ontick
	 *	�ڶ���������Ϊ1����ǰ��Ȩģʽ���򽫴���ת��xxxx-���ٴ���ontick
	 *	������������Ϊ2������Ȩģʽ���򽫴���ת��xxxx+���ٰ�tick������һ���������ٴ���ontick
	 */
	if(_ready)
	{
		auto sit = _tick_sub_map.find(stdCode);
		if (sit != _tick_sub_map.end())
		{
			const SubList& sids = sit->second;
			for (auto it = sids.begin(); it != sids.end(); it++)
			{
				uint32_t sid = it->first;


				auto cit = _ctx_map.find(sid);
				if (cit != _ctx_map.end())
				{
					SelContextPtr& ctx = (SelContextPtr&)cit->second;
					uint32_t opt = it->second.second;

					if (opt == 0)
					{
						ctx->on_tick(stdCode, curTick);
					}
					else
					{
						std::string wCode = stdCode;
						wCode = fmt::format("{}{}", stdCode, opt == 1 ? SUFFIX_QFQ : SUFFIX_HFQ);
						if (opt == 1)
						{
							ctx->on_tick(wCode.c_str(), curTick);
						}
						else //(opt == 2)
						{
							WTSTickData* newTick = WTSTickData::create(curTick->getTickStruct());
							WTSTickStruct& newTS = newTick->getTickStruct();
							newTick->setContractInfo(curTick->getContractInfo());

							//������һ����Ȩ���ӵĴ���
							double factor = get_exright_factor(stdCode);
							newTS.open *= factor;
							newTS.high *= factor;
							newTS.low *= factor;
							newTS.price *= factor;

							_price_map[wCode] = newTS.price;

							ctx->on_tick(wCode.c_str(), newTick);
							newTick->release();
						}
					}

				}


			}

		}
	}
}

void WtSelEngine::on_minute_end(uint32_t curDate, uint32_t curTime)
{
	//Ҫ�Ƚ���һ���ӵ�ʱ��
	uint32_t nextTime = TimeUtils::getNextMinute(curTime, 1);
	if (nextTime < curTime)
		curDate = TimeUtils::getNextDate(curDate);

	uint32_t weekDay = TimeUtils::getWeekDay(curDate);

	for (auto& v : _tasks)
	{
		TaskInfoPtr& tInfo = (TaskInfoPtr&)v.second;
		if (tInfo->_time != nextTime)
			continue;

		uint64_t now = (uint64_t)curDate * 10000 + nextTime;
		if (tInfo->_last_exe_time >= now)
			continue;

		if (_base_data_mgr->isHoliday(tInfo->_trdtpl, curDate, true))
			continue;

		//��ȡ��һ�������յ�����
		uint32_t preTDate = TimeUtils::getNextDate(_cur_date, -1);
		bool bHasHoliday = false;
		uint32_t days = 1;
		while (_base_data_mgr->isHoliday(tInfo->_trdtpl, preTDate, true))
		{
			bHasHoliday = true;
			preTDate = TimeUtils::getNextDate(preTDate, -1);
			days++;
		}
		uint32_t preWD = TimeUtils::getWeekDay(preTDate);

		WTSSessionInfo* sInfo = get_session_info(tInfo->_session, false);

		bool bIgnore = true;
		switch (tInfo->_period)
		{
		case TPT_Daily:
			bIgnore = false;
			break;
		case TPT_Minute:
			{
				uint32_t minutes = sInfo->timeToMinutes(curTime);	//�Ƚ�ʱ��ת���ɷ�����
				if(minutes != 0 && (minutes%tInfo->_time == 0))		//����������ܱ�����,�Ҳ�Ϊ0,����Դ���
				{
					bIgnore = false;
				}
			}
			break;
		case TPT_Monthly:
			//if (preTDate % 1000000 < _task->_day && _cur_date % 1000000 >= _task->_day)
			//	fired = true;
			if (_cur_date % 1000000 == tInfo->_day)
				bIgnore = false;
			else if (bHasHoliday)
			{
				//��һ�����������ϸ���,�ҵ�ǰ���ڴ��ڴ�������
				//˵������µĿ�ʼ�����ڽڼ�����,˳�ӵ�����
				if ((preTDate % 10000 / 100 < _cur_date % 10000 / 100) && _cur_date % 1000000 > tInfo->_day)
				{
					bIgnore = false;
				}
				else if (preTDate % 1000000 < tInfo->_day && _cur_date % 1000000 > tInfo->_day)
				{
					//��һ����������ͬһ����,��С�ڴ�������,���ǽ�����ڴ�������,˵����ȷ�������ڵ��ڼ�����,˳�ӵ�����
					bIgnore = false;
				}
			}
			break;
		case TPT_Weekly:
			//if (preWD < _task->_day && weekDay >= _task->_day)
			//	fired = true;
			if (weekDay == tInfo->_day)
				bIgnore = false;
			else if (bHasHoliday)
			{
				if (days >= 7 && weekDay > tInfo->_day)
				{
					bIgnore = false;
				}
				else if (preWD > weekDay && weekDay > tInfo->_day)
				{
					//��һ�������յ����ڴ��ڽ��������,˵������һ����
					bIgnore = false;
				}
				else if (preWD < tInfo->_day && weekDay > tInfo->_day)
				{
					bIgnore = false;
				}
			}
			break;
		case TPT_Yearly:
			if (preTDate % 10000 < tInfo->_day && _cur_date % 10000 >= tInfo->_day)
				bIgnore = false;
			break;
		}

		if (bIgnore)
			continue;

		//TODO: �ص�����
		SelContextPtr ctx = getContext(tInfo->_id);
		StdThreadPtr thrd(new StdThread([ctx, curDate, curTime, nextTime](){
			if (ctx)
				ctx->on_schedule(curDate, curTime, nextTime);
		}));	

		tInfo->_last_exe_time = now;
	}
}

void WtSelEngine::run(bool bAsync /*= false*/)
{
	WTSVariant* cfgProd = _cfg->get("product");
	_tm_ticker = new WtSelRtTicker(this);
	_tm_ticker->init(_data_mgr->reader(), cfgProd->getCString("session"));

	//����֮ǰ,�Ȱ������еĲ������
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		rj::Value jStraList(rj::kArrayType);
		for (auto& m : _ctx_map)
		{
			const SelContextPtr& ctx = m.second;
			jStraList.PushBack(rj::Value(ctx->name(), allocator), allocator);
		}

		root.AddMember("marks", jStraList, allocator);

		rj::Value jChnlList(rj::kArrayType);
		for (auto& m : _adapter_mgr->getAdapters())
		{
			const TraderAdapterPtr& adapter = m.second;
			jChnlList.PushBack(rj::Value(adapter->id(), allocator), allocator);
		}

		root.AddMember("channels", jChnlList, allocator);

		root.AddMember("engine", rj::Value("SEL", allocator), allocator);

		std::string filename = WtHelper::getBaseDir();
		filename += "marker.json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}

	_tm_ticker->run();
}

void WtSelEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier /* = NULL */)
{
	WtEngine::init(cfg, bdMgr, dataMgr, hotMgr, notifier);

	_cfg = cfg;
	_cfg->retain();
}

void WtSelEngine::addContext(SelContextPtr ctx, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict /* = true */, const char* trdtpl /* = "CHINA" */, const char* sessionID/* ="TRADING" */)
{
	if (ctx == NULL)
		return;

	auto it = _tasks.find(ctx->id());
	if(it != _tasks.end())
	{
		WTSLogger::error("Task registration failed: task id {} already registered", ctx->id());
		return;
	}

	TaskInfoPtr tInfo(new TaskInfo);
	wt_strcpy(tInfo->_name, ctx->name());
	wt_strcpy(tInfo->_trdtpl, trdtpl);
	wt_strcpy(tInfo->_session, sessionID);
	tInfo->_day = date;
	tInfo->_time = time;
	tInfo->_period = period;
	tInfo->_strict_time = bStrict;
	tInfo->_id = makeTaskId();

	_tasks[ctx->id()] = tInfo;

	uint32_t sid = ctx->id();
	_ctx_map[sid] = ctx;
}

SelContextPtr WtSelEngine::getContext(uint32_t id)
{
	auto it = _ctx_map.find(id);
	if (it == _ctx_map.end())
		return SelContextPtr();

	return it->second;
}

void WtSelEngine::handle_pos_change(const char* straName, const char* stdCode, double diffQty)
{
	//�����ǳֲ�����,���Բ��ô���δ���˵����,��Ϊ���������,����ı�Ŀ��diffQty
	if (_filter_mgr.is_filtered_by_strategy(straName, diffQty, true))
	{
		//�����־
		WTSLogger::info("[Filters] Target position of {} of strategy {} ignored by strategy filter", stdCode, straName);
		return;
	}

	std::string realCode = stdCode;
	//const char* ruleTag = _hot_mgr->getRuleTag(stdCode);
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _hot_mgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _cur_tdate);
		realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
	}
	//else if (CodeHelper::isStdFutHotCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
	//	realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
	//}
	//else if (CodeHelper::isStdFut2ndCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
	//	realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
	//}

	PosInfo& pItem = _pos_map[realCode];
	double targetPos = pItem._volume + diffQty;

	bool bRiskEnabled = false;
	if (!decimal::eq(_risk_volscale, 1.0) && _risk_date == _cur_tdate)
	{
		WTSLogger::log_by_cat("risk", LL_INFO, "Risk scale of portfolio is {:.2f}", _risk_volscale);
		bRiskEnabled = true;
	}
	if (bRiskEnabled && targetPos != 0)
	{
		double symbol = targetPos / abs(targetPos);
		targetPos = decimal::rnd(abs(targetPos)*_risk_volscale)*symbol;
	}

	append_signal(realCode.c_str(), targetPos, false);
	save_datas();

	const char* execid = _exec_mgr.get_route(straName);
	_exec_mgr.handle_pos_change(realCode.c_str(), targetPos);
}

WTSCommodityInfo* WtSelEngine::get_comm_info(const char* stdCode)
{
	return _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
}

WTSSessionInfo* WtSelEngine::get_sess_info(const char* stdCode)
{
	WTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
	if (cInfo == NULL)
		return NULL;

	return _base_data_mgr->getSession(cInfo->getSession());
}

uint64_t WtSelEngine::get_real_time()
{
	return TimeUtils::makeTime(_cur_date, _cur_raw_time * 100000 + _cur_secs);
}