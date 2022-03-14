/*!
 * \file HisDataReplayer.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "HisDataReplayer.h"
#include "EventNotifier.h"
#include "WtHelper.h"

#include <fstream>

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"
#include "../WTSTools/CsvHelper.h"

#include "../WTSUtils/WTSCmpHelper.hpp"
#include "../WTSUtils/WTSCfgLoader.h"

#include "../Share/CodeHelper.hpp"

#include <boost/filesystem.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

using namespace std;

/*
 *	处理块数据
 */
bool proc_block_data(const char* tag, std::string& content, bool isBar, bool bKeepHead = true)
{
	BlockHeader* header = (BlockHeader*)content.data();

	bool bCmped = header->is_compressed();
	bool bOldVer = header->is_old_version();

	//如果既没有压缩，也不是老版本结构体，则直接返回
	if (!bCmped && !bOldVer)
	{
		if (!bKeepHead)
			content.erase(0, BLOCK_HEADER_SIZE);
		return true;
	}

	std::string buffer;
	if (bCmped)
	{
		BlockHeaderV2* blkV2 = (BlockHeaderV2*)content.c_str();

		if (content.size() != (sizeof(BlockHeaderV2) + blkV2->_size))
		{
			WTSLogger::error_f("Size check failed while processing {} data of {}", isBar ? "bar" : "tick", tag);
			return false;
		}

		//将文件头后面的数据进行解压
		buffer = WTSCmpHelper::uncompress_data(content.data() + BLOCK_HEADERV2_SIZE, (uint32_t)blkV2->_size);
	}
	else
	{
		if (!bOldVer)
		{
			//如果不是老版本，直接返回
			if (!bKeepHead)
				content.erase(0, BLOCK_HEADER_SIZE);
			return true;
		}
		else
		{
			buffer.append(content.data() + BLOCK_HEADER_SIZE, content.size() - BLOCK_HEADER_SIZE);
		}
	}

	if (bOldVer)
	{
		if (isBar)
		{
			std::string bufV2;
			uint32_t barcnt = buffer.size() / sizeof(WTSBarStructOld);
			bufV2.resize(barcnt * sizeof(WTSBarStruct));
			WTSBarStruct* newBar = (WTSBarStruct*)bufV2.data();
			WTSBarStructOld* oldBar = (WTSBarStructOld*)buffer.data();
			for (uint32_t idx = 0; idx < barcnt; idx++)
			{
				newBar[idx] = oldBar[idx];
			}
			buffer.swap(bufV2);

			WTSLogger::debug_f("{} bars of {} transferd to new version...", barcnt, tag);
		}
		else
		{
			uint32_t tick_cnt = buffer.size() / sizeof(WTSTickStructOld);
			std::string bufv2;
			bufv2.resize(sizeof(WTSTickStruct)*tick_cnt);
			WTSTickStruct* newTick = (WTSTickStruct*)bufv2.data();
			WTSTickStructOld* oldTick = (WTSTickStructOld*)buffer.data();
			for (uint32_t i = 0; i < tick_cnt; i++)
			{
				newTick[i] = oldTick[i];
			}
			buffer.swap(bufv2);

			WTSLogger::debug_f("{} ticks of {} transferd to new version...", tick_cnt, tag);
		}
	}

	if (bKeepHead)
	{
		//原来的缓存，resize到文件头大小，再追加最终的数据
		content.resize(BLOCK_HEADER_SIZE);
		content.append(buffer);

		//修改数据块的版本号
		header = (BlockHeader*)content.data();
		header->_version = BLOCK_VERSION_RAW_V2;
	}
	else
	{
		//不保留块头，直接跟数据做一个swap
		content.swap(buffer);
	}

	return true;
}

HisDataReplayer::HisDataReplayer()
	: _listener(NULL)
	, _cur_date(0)
	, _cur_time(0)
	, _cur_secs(0)
	, _cur_tdate(0)
	, _tick_enabled(true)
	, _opened_tdate(0)
	, _closed_tdate(0)
	, _tick_simulated(true)
	, _running(false)
	, _begin_time(0)
	, _end_time(0)
	, _bt_loader(NULL)
{
}


HisDataReplayer::~HisDataReplayer()
{
}


bool HisDataReplayer::init(WTSVariant* cfg, EventNotifier* notifier /* = NULL */, IBtDataLoader* dataLoader /* = NULL */)
{
	_notifier = notifier;
	_bt_loader = dataLoader;

	_mode = cfg->getCString("mode");
	/*
	 *	By Wesley @ 2022.01.11
	 *	因为store可能会变复杂，所以这里做一个兼容处理
	 *	如果有store就读取store的path，如果没有store，就还读取root的path
	 */
	if (cfg->has("store"))
	{
		_base_dir = StrUtil::standardisePath(cfg->get("store")->getCString("path"));
	}
	else
	{
		_base_dir = StrUtil::standardisePath(cfg->getCString("path"));
	}
	
	if(_mode == "storage" || _mode == "bin")
	{
		if (cfg->has("store"))
		{
			_his_dt_mgr.init(cfg->get("store"));
		}
		else
		{
			WTSVariant* item = WTSVariant::createObject();
			item->append("path", _base_dir.c_str());
			_his_dt_mgr.init(item);
			item->release();
		}
	}
	
	bool isRangeCfg = (_begin_time == 0 || _end_time == 0);//是否从配置文件读取回测区间
	if(_begin_time == 0)
		_begin_time = cfg->getUInt64("stime");

	if(_end_time == 0)
		_end_time = cfg->getUInt64("etime");

	WTSLogger::info_f("Backtest time range is set to be [{},{}] via config", _begin_time, _end_time);

	_tick_enabled = cfg->getBoolean("tick");
	WTSLogger::info_f("Tick data replaying is {}", _tick_enabled ? "enabled" : "disabled");

	//基础数据文件
	WTSVariant* cfgBF = cfg->get("basefiles");
	//基础配置文件的编码，这样可以兼容原来的配置
	bool isUTF8 = cfgBF->getBoolean("utf-8");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"), isUTF8);

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for(uint32_t i = 0; i < cfgItem->size(); i ++)
			{
				_bd_mgr.loadCommodities(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadContracts(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	if (cfgBF->get("holiday"))
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));

	if (cfgBF->get("hot"))
		_hot_mgr.loadHots(cfgBF->getCString("hot"));

	if (cfgBF->get("second"))
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));

	loadFees(cfg->getCString("fees"));


	/*
	 *	By Wesley @ 2021.12.20
	 *	先从extloader加载除权因子
	 *	如果加载失败，并且配置了除权因子文件，再加载除权因子文件
	 */
	bool bLoaded = loadStkAdjFactorsFromLoader();

	if (!bLoaded && cfg->has("adjfactor"))
		loadStkAdjFactorsFromFile(cfg->getCString("adjfactor"));

	return true;
}

bool HisDataReplayer::loadStkAdjFactorsFromLoader()
{
	if (NULL == _bt_loader)
		return false;

	bool ret = _bt_loader->loadAllAdjFactors(this, [](void* obj, const char* stdCode, uint32_t* dates, double* factors, uint32_t count) {
		HisDataReplayer* replayer = (HisDataReplayer*)obj;
		AdjFactorList& fctrLst = replayer->_adj_factors[stdCode];

		for (uint32_t i = 0; i < count; i++)
		{
			AdjFactor adjFact;
			adjFact._date = dates[i];
			adjFact._factor = factors[i];

			fctrLst.emplace_back(adjFact);
		}

		//一定要把第一条加进去，不然如果是前复权的话，可能会漏处理最早的数据
		AdjFactor adjFact;
		adjFact._date = 19900101;
		adjFact._factor = 1;
		fctrLst.emplace_back(adjFact);

		std::sort(fctrLst.begin(), fctrLst.end(), [](const AdjFactor& left, const AdjFactor& right) {
			return left._date < right._date;
		});
	});

	if (ret) WTSLogger::info_f("Adjusting factors of {} contracts loaded via extended loader", _adj_factors.size());
	return ret;
}

bool HisDataReplayer::loadStkAdjFactorsFromFile(const char* adjfile)
{
	if (!StdFile::exists(adjfile))
	{
		WTSLogger::error_f("Adjust factor file {} not exists, skipped", adjfile);
		return false;
	}

	std::string content;
	StdFile::read_file_content(adjfile, content);

	WTSVariant* doc = WTSCfgLoader::load_from_file(adjfile, true);
	if (doc == NULL)
	{
		WTSLogger::error_f("Parsing adjust factor file {} faield", adjfile);
		return false;
	}

	uint32_t stk_cnt = 0;
	uint32_t fct_cnt = 0;
	for (const std::string& exchg : doc->memberNames())
	{
		WTSVariant* itemExchg = doc->get(exchg);
		for (const std::string& code : itemExchg->memberNames())
		{
			WTSVariant* ayFacts = itemExchg->get(code);
			if (!ayFacts->isArray())
				continue;

			/*
			 *	By Wesley @ 2021.12.21
			 *	先检查code的格式是不是包含PID，如STK.600000
			 *	如果包含PID，则直接格式化，如果不包含，则强制为STK
			 */
			bool bHasPID = (code.find('.') != std::string::npos);

			std::string key;
			if(bHasPID)
				key = fmt::format("{}.{}", exchg, code);
			else
				key = fmt::format("{}.STK.{}", exchg, code);
			stk_cnt++;

			AdjFactorList& fctrLst = _adj_factors[key];
			for (uint32_t i = 0; i < ayFacts->size(); i++)
			{
				WTSVariant* fItem = ayFacts->get(i);
				AdjFactor adjFact;
				adjFact._date = fItem->getUInt32("date");
				adjFact._factor = fItem->getDouble("factor");

				fctrLst.emplace_back(adjFact);
				fct_cnt++;
			}

			//一定要把第一条加进去，不然如果是前复权的话，可能会漏处理最早的数据
			AdjFactor adjFact;
			adjFact._date = 19900101;
			adjFact._factor = 1;
			fctrLst.emplace_back(adjFact);

			std::sort(fctrLst.begin(), fctrLst.end(), [](const AdjFactor& left, const AdjFactor& right) {
				return left._date < right._date;
			});
		}
	}

	WTSLogger::info_f("{} items of adjust factors for {} tickers loaded from {}", fct_cnt, stk_cnt, adjfile);
	doc->release();
	return true;
}

void HisDataReplayer::register_task(uint32_t taskid, uint32_t date, uint32_t time, const char* period, const char* trdtpl /* = "CHINA" */, const char* session /* = "TRADING" */)
{
	TaskPeriodType ptype;
	if (wt_stricmp(period, "d") == 0)
		ptype = TPT_Daily;
	else if (wt_stricmp(period, "w") == 0)
		ptype = TPT_Weekly;
	else if (wt_stricmp(period, "m") == 0)
		ptype = TPT_Monthly;
	else if (wt_stricmp(period, "y") == 0)
		ptype = TPT_Yearly;
	else if (wt_stricmp(period, "min") == 0)
		ptype = TPT_Minute;
	else
		ptype = TPT_None;

	_task.reset(new TaskInfo);
	strcpy(_task->_name, "sel");
	strcpy(_task->_trdtpl, trdtpl);
	strcpy(_task->_session, session);
	_task->_day = date;
	_task->_time = time;
	_task->_id = taskid;
	_task->_period = ptype;
	_task->_strict_time = true;

	WTSLogger::info_f("Timed task registration succeed, frequency: {}", period);
}

void HisDataReplayer::clear_cache()
{
	_ticks_cache.clear();
	_orddtl_cache.clear();
	_ordque_cache.clear();
	_trans_cache.clear();

	_bars_cache.clear();

	WTSLogger::log_raw(LL_WARN, "All cached data cleared");
}

void HisDataReplayer::reset()
{
	//重置不会清除掉缓存，而是将读取的标记还原，这样不用重复加载主句
	for(auto& m : _ticks_cache)
	{
		HftDataList<WTSTickStruct>& cacheItem = (HftDataList<WTSTickStruct>&)m.second;
		cacheItem._cursor = UINT_MAX;
	}

	for (auto& m : _orddtl_cache)
	{
		HftDataList<WTSOrdDtlStruct>& cacheItem = (HftDataList<WTSOrdDtlStruct>&)m.second;
		cacheItem._cursor = UINT_MAX;
	}

	for (auto& m : _ordque_cache)
	{
		HftDataList<WTSOrdQueStruct>& cacheItem = (HftDataList<WTSOrdQueStruct>&)m.second;
		cacheItem._cursor = UINT_MAX;
	}

	for (auto& m : _trans_cache)
	{
		HftDataList<WTSTransStruct>& cacheItem = (HftDataList<WTSTransStruct>&)m.second;
		cacheItem._cursor = UINT_MAX;
	}

	for (auto& m : _bars_cache)
	{
		BarsList& cacheItem = (BarsList&)m.second;
		cacheItem._cursor = UINT_MAX;

		WTSLogger::info_f("Reading flag of {} has been reset", m.first.c_str());
	}

	_unbars_cache.clear();

	_day_cache.clear();
	_ticker_keys.clear();

	_tick_sub_map.clear();
	_ordque_sub_map.clear();
	_orddtl_sub_map.clear();
	_trans_sub_map.clear();

	_price_map.clear();

	_main_key = "";
	_min_period = "";

	_cur_date = 0;
	_cur_time = 0;
	_cur_secs = 0;
	_cur_tdate = 0;
	_opened_tdate = 0;
	_closed_tdate = 0;
	_tick_simulated = true;
}

void HisDataReplayer::dump_btstate(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint64_t stime, uint64_t etime, double progress, int64_t elapse)
{
	std::string output;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("code", rj::Value(stdCode, allocator), allocator);

		std::stringstream ss;
		if (period == KP_DAY)
			ss << "d";
		else if (period == KP_Minute1)
			ss << "m" << times;
		else
			ss << "m" << times * 5;

		root.AddMember("period", rj::Value(ss.str().c_str(), allocator), allocator);
		
		root.AddMember("stime", stime, allocator);
		root.AddMember("etime", etime, allocator);
		root.AddMember("progress", progress, allocator);
		root.AddMember("elapse", elapse, allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		output = sb.GetString();
	}

	std::string folder = WtHelper::getOutputDir();
	folder += _stra_name;
	folder += "/";
	boost::filesystem::create_directories(folder.c_str());
	std::string filename = folder + "btenv.json";
	StdFile::write_file_content(filename.c_str(), output.c_str(), output.size());
}

void HisDataReplayer::notify_state(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint64_t stime, uint64_t etime, double progress)
{
	if (!_notifier)
		return;

	std::string output;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("code", rj::Value(stdCode, allocator), allocator);

		std::stringstream ss;
		if (period == KP_DAY)
			ss << "d";
		else if (period == KP_Minute1)
			ss << "m" << times;
		else
			ss << "m" << times * 5;

		root.AddMember("period", rj::Value(ss.str().c_str(), allocator), allocator);

		root.AddMember("stime", stime, allocator);
		root.AddMember("etime", etime, allocator);
		root.AddMember("progress", progress, allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		output = sb.GetString();
	}

	_notifier->notifyData("BT_STATE", (void*)output.c_str(), output.size());
}

uint32_t HisDataReplayer::locate_barindex(const std::string& key, uint64_t now, bool bUpperBound /* = false */)
{
	uint32_t curDate, curTime;
	curDate = (uint32_t)(now / 10000);
	curTime = (uint32_t)(now % 10000);

	BarsListPtr& barsList = _bars_cache[key];
	if (barsList == NULL)
		return UINT32_MAX;

	bool isDay = (barsList->_period == KP_DAY);

	WTSBarStruct bar;
	bar.date = curDate;
	bar.time = (curDate - 19900000) * 10000 + curTime;
	auto it = std::lower_bound(barsList->_bars.begin(), barsList->_bars.end(), bar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});

	uint32_t idx;
	if (it == barsList->_bars.end())
		idx = barsList->_bars.size() - 1;
	else
	{
		if(bUpperBound)
		{//如果是找上边界，则要比较时间向下修正，因为lower_bound函数找的是大于等于curTime的K线
			if ((isDay && it->date > bar.date) || (!isDay && it->time > bar.time))
			{
				it--;
			}
		}

		idx = it - barsList->_bars.begin();
	}
	
	return idx;
}

void HisDataReplayer::stop()
{
	if(!_running)
	{
		WTSLogger::log_raw(LL_ERROR, "Backtesting is not running, no need to stop");
		return;
	}

	if (_terminated)
		return;

	_terminated = true;
	WTSLogger::log_raw(LL_WARN, "Terminating flag reset to true, backtesting will quit at next round");
}

bool HisDataReplayer::prepare()
{
	if (_running)
	{
		WTSLogger::log_raw(LL_ERROR, "Cannot run more than one backtesting task at the same time");
		return false;
	}

	_running = true;
	_terminated = false;
	reset();

	_cur_date = (uint32_t)(_begin_time / 10000);
	_cur_time = (uint32_t)(_begin_time % 10000);
	_cur_secs = 0;
	_cur_tdate = _bd_mgr.calcTradingDate(DEFAULT_SESSIONID, _cur_date, _cur_time, true);

	if (_notifier)
		_notifier->notifyEvent("BT_START");

	_listener->handle_init();

	if (!_tick_enabled)
		checkUnbars();

	return true;
}

void HisDataReplayer::run(bool bNeedDump/* = false*/)
{
	if(_task == NULL)
	{
		//如果没有时间调度任务,则采用主K线回放的模式

		//如果没有确定主K线,则确定一个周期最短的主K线
		if (_main_key.empty() && !_bars_cache.empty())
		{
			WTSKlinePeriod minPeriod = KP_DAY;
			uint32_t minTimes = 1;
			for(auto& m : _bars_cache)
			{
				const BarsListPtr& barsList = m.second;
				if (barsList->_period < minPeriod)
				{
					minPeriod = barsList->_period;
					minTimes = barsList->_times;
					_main_key = m.first;
				}
				else if(barsList->_period == minPeriod)
				{
					if(barsList->_times < minTimes)
					{
						_main_key = m.first;
						minTimes = barsList->_times;
					}
				}
			}

			WTSLogger::info_f("Main K bars automatic determined: {}", _main_key.c_str());
		}

		if(!_main_key.empty())
		{
			//如果订阅了K线，则按照主K线进行回放
			run_by_bars(bNeedDump);
		}
		else if(_tick_enabled)
		{
			run_by_ticks(bNeedDump);
		}
		else
		{
			WTSLogger::log_raw(LL_INFO, "Main K bars not subscribed and backtesting of tick data not available , replaying done");
			_listener->handle_replay_done();
			if (_notifier)
				_notifier->notifyEvent("BT_END");
		}
	}
	else //if(_task != NULL)
	{
		run_by_tasks(bNeedDump);
	}

	_running = false;
}

void HisDataReplayer::run_by_ticks(bool bNeedDump /* = false */)
{
	//如果没有订阅K线，且tick回测是打开的，则按照每日的tick进行回放
	uint32_t edt = (uint32_t)(_end_time / 10000);
	uint32_t etime = (uint32_t)(_end_time % 10000);
	uint64_t end_tdate = _bd_mgr.calcTradingDate(DEFAULT_SESSIONID, edt, etime, true);

	while (_cur_tdate <= end_tdate && !_terminated)
	{
		if (checkAllTicks(_cur_tdate))
		{
			WTSLogger::info_f("Start to replay tick data of {}...", _cur_tdate);
			_listener->handle_session_begin(_cur_tdate);
			replayHftDatasByDay(_cur_tdate);
			_listener->handle_session_end(_cur_tdate);
		}

		_cur_tdate = TimeUtils::getNextDate(_cur_tdate);
	}

	if (_terminated)
		WTSLogger::debug("Replaying by ticks terminated forcely");

	WTSLogger::log_raw(LL_INFO, "All back data replayed, replaying done");
	_listener->handle_replay_done();
	if (_notifier)
		_notifier->notifyEvent("BT_END");
}

void HisDataReplayer::run_by_bars(bool bNeedDump /* = false */)
{
	int64_t now = TimeUtils::getLocalTimeNano();

	BarsListPtr barsList = _bars_cache[_main_key];
	WTSSessionInfo* sInfo = get_session_info(barsList->_code.c_str(), true);
	std::string commId = CodeHelper::stdCodeToStdCommID(barsList->_code.c_str());

	uint32_t sIdx = locate_barindex(_main_key, _begin_time, false);
	uint32_t eIdx = locate_barindex(_main_key, _end_time, true);

	uint32_t total_barcnt = eIdx - sIdx + 1;
	uint32_t replayed_barcnt = 0;

	notify_state(barsList->_code.c_str(), barsList->_period, barsList->_times, _begin_time, _end_time, 0);

	if (bNeedDump)
		dump_btstate(barsList->_code.c_str(), barsList->_period, barsList->_times, _begin_time, _end_time, 100.0, TimeUtils::getLocalTimeNano() - now);

	WTSLogger::info_f("Start to replay back data from {}...", _begin_time);

	for (; !_terminated;)
	{
		bool isDay = barsList->_period == KP_DAY;
		if (barsList->_cursor != UINT_MAX)
		{
			uint64_t nextBarTime = 0;
			if (isDay)
				nextBarTime = (uint64_t)barsList->_bars[barsList->_cursor].date * 10000 + sInfo->getCloseTime();
			else
			{
				nextBarTime = (uint64_t)barsList->_bars[barsList->_cursor].time;
				nextBarTime += 199000000000;
			}

			if (nextBarTime > _end_time)
			{
				WTSLogger::info_f("{} is beyond ending time {},replaying done", nextBarTime, _end_time);
				break;
			}

			uint32_t nextDate = (uint32_t)(nextBarTime / 10000);
			uint32_t nextTime = (uint32_t)(nextBarTime % 10000);

			//By Wesley @ 2022.01.10
			//如果和收盘时间一样，进行这个判断
			//主要针对7*24小时的品种，其他的品种不需要
			uint32_t nextTDate = _opened_tdate;
			if(isDay || (!isDay && sInfo->offsetTime(nextTime, false) != sInfo->getCloseTime(true)))
			{
				nextTDate = _bd_mgr.calcTradingDate(commId.c_str(), nextDate, nextTime, false);
				if (_opened_tdate != nextTDate)
				{
					WTSLogger::debug_f("Tradingday {} begins", nextTDate);
					_listener->handle_session_begin(nextTDate);
					_opened_tdate = nextTDate;
					_cur_tdate = nextTDate;
				}
			}			

			uint64_t curBarTime = (uint64_t)_cur_date * 10000 + _cur_time;
			if (_tick_enabled)
			{
				//如果开启了tick回放,则直接回放tick数据
				//如果tick回放失败，说明tick数据不存在，则需要模拟tick
				_tick_simulated = !replayHftDatas(curBarTime, nextBarTime);
			}

			_cur_date = nextDate;
			_cur_time = nextTime;
			_cur_secs = 0;

			bool isEndTDate = (sInfo->offsetTime(_cur_time, false) >= sInfo->getCloseTime(true));

			if (!_tick_enabled)
			{
				checkUnbars();
				replayUnbars(curBarTime, nextBarTime, (isDay || isEndTDate) ? nextTDate : 0);
			}

			onMinuteEnd(nextDate, nextTime, (isDay || isEndTDate) ? nextTDate : 0, _tick_simulated);

			replayed_barcnt += 1;

			if (isEndTDate && _closed_tdate != _cur_tdate)
			{
				WTSLogger::debug_f("Tradingday {} ends", _cur_tdate);
				_listener->handle_session_end(_cur_tdate);
				_closed_tdate = _cur_tdate;
				_day_cache.clear();
			}

			notify_state(barsList->_code.c_str(), barsList->_period, barsList->_times, _begin_time, _end_time, replayed_barcnt*100.0 / total_barcnt);

			if (barsList->_cursor >= barsList->_bars.size())
			{
				WTSLogger::log_raw(LL_INFO, "All back data replayed, replaying done");
				break;
			}
		}
		else
		{
			WTSLogger::log_raw(LL_ERROR, "No back data initialized, replaying canceled");
			break;
		}
	}

	if (_terminated)
		WTSLogger::debug("Replaying by bars terminated forcely");

	notify_state(barsList->_code.c_str(), barsList->_period, barsList->_times, _begin_time, _end_time, 100);
	if (_notifier)
		_notifier->notifyEvent("BT_END");

	if (_closed_tdate != _cur_tdate)
	{
		WTSLogger::debug_f("Tradingday {} ends", _cur_tdate);
		_listener->handle_session_end(_cur_tdate);
	}

	if (bNeedDump)
	{
		dump_btstate(barsList->_code.c_str(), barsList->_period, barsList->_times, _begin_time, _end_time, 100.0, TimeUtils::getLocalTimeNano() - now);
	}

	_listener->handle_replay_done();
}

void HisDataReplayer::run_by_tasks(bool bNeedDump /* = false */)
{
	//时间调度任务不为空,则按照时间调度任务回放
	WTSSessionInfo* sInfo = NULL;
	const char* DEF_SESS = (strlen(_task->_session) == 0) ? DEFAULT_SESSIONID : _task->_session;
	sInfo = _bd_mgr.getSession(DEF_SESS);
	WTSLogger::info_f("Start to backtest with task frequency from {}...", _begin_time);

	//分钟即任务和日级别任务分开写
	if (_task->_period != TPT_Minute)
	{
		uint32_t endtime = TimeUtils::getNextMinute(_task->_time, -1);
		bool bIsPreDay = endtime > _task->_time;
		if (bIsPreDay)
			_cur_date = TimeUtils::getNextDate(_cur_date, -1);

		for (; !_terminated;)
		{
			bool fired = false;
			//获取上一个交易日的日期
			uint32_t preTDate = TimeUtils::getNextDate(_cur_tdate, -1);
			if (_cur_time == endtime)
			{
				if (!_bd_mgr.isHoliday(_task->_trdtpl, _cur_date, true))
				{
					uint32_t weekDay = TimeUtils::getWeekDay(_cur_date);


					bool bHasHoliday = false;
					uint32_t days = 1;
					while (_bd_mgr.isHoliday(_task->_trdtpl, preTDate, true))
					{
						bHasHoliday = true;
						preTDate = TimeUtils::getNextDate(preTDate, -1);
						days++;
					}
					uint32_t preWD = TimeUtils::getWeekDay(preTDate);

					switch (_task->_period)
					{
					case TPT_Daily:
						fired = true;
						break;
					case TPT_Minute:
						break;
					case TPT_Monthly:
						//if (preTDate % 1000000 < _task->_day && _cur_date % 1000000 >= _task->_day)
						//	fired = true;
						if (_cur_date % 1000000 == _task->_day)
							fired = true;
						else if (bHasHoliday)
						{
							//上一个交易日在上个月,且当前日期大于触发日期
							//说明这个月的开始日期在节假日内,顺延到今天
							if ((preTDate % 10000 / 100 < _cur_date % 10000 / 100) && _cur_date % 1000000 > _task->_day)
							{
								fired = true;
							}
							else if (preTDate % 1000000 < _task->_day && _cur_date % 1000000 > _task->_day)
							{
								//上一个交易日在同一个月,且小于触发日期,但是今天大于触发日期,说明正确触发日期到节假日内,顺延到今天
								fired = true;
							}
						}
						break;
					case TPT_Weekly:
						//if (preWD < _task->_day && weekDay >= _task->_day)
						//	fired = true;
						if (weekDay == _task->_day)
							fired = true;
						else if (bHasHoliday)
						{
							if (days >= 7 && weekDay > _task->_day)
							{
								fired = true;
							}
							else if (preWD > weekDay && weekDay > _task->_day)
							{
								//上一个交易日的星期大于今天的星期,说明换了一周了
								fired = true;
							}
							else if (preWD < _task->_day && weekDay > _task->_day)
							{
								fired = true;
							}
						}
						break;
					case TPT_Yearly:
						if (preTDate % 10000 < _task->_day && _cur_date % 10000 >= _task->_day)
							fired = true;
						break;
					}
				}
			}

			if (!fired)
			{
				//调整时间
				//如果当前时间小于任务时间,则直接赋值即可
				//如果当前时间大于任务时间,则至少要等下一天
				if (_cur_time < endtime)
				{
					_cur_time = endtime;
					continue;
				}

				uint32_t newTDate = _bd_mgr.calcTradingDate(DEF_SESS, _cur_date, _cur_time, true);

				if (newTDate != _cur_tdate)
				{
					_cur_tdate = newTDate;
					if (_listener)
						_listener->handle_session_begin(newTDate);
					if (_listener)
						_listener->handle_session_end(newTDate);
				}
			}
			else
			{
				//用前一分钟作为结束时间
				uint32_t curDate = _cur_date;
				uint32_t curTime = endtime;
				bool bEndSession = sInfo->offsetTime(curTime, true) >= sInfo->getCloseTime(true);
				if (_listener)
					_listener->handle_session_begin(_cur_tdate);
				onMinuteEnd(curDate, curTime, bEndSession ? _cur_tdate : preTDate);
				if (_listener)
					_listener->handle_session_end(_cur_tdate);
			}

			_cur_date = TimeUtils::getNextDate(_cur_date);
			_cur_time = endtime;
			_cur_tdate = _bd_mgr.calcTradingDate(DEF_SESS, _cur_date, _cur_time, true);

			uint64_t nextTime = (uint64_t)_cur_date * 10000 + _cur_time;
			if (nextTime > _end_time)
			{
				WTSLogger::log_raw(LL_INFO, "Backtesting with task frequency is done");
				if (_listener)
				{
					_listener->handle_session_end(_cur_tdate);
					_listener->handle_replay_done();
					if (_notifier)
						_notifier->notifyEvent("BT_END");
				}

				break;
			}
		}
	}
	else
	{
		if (_listener)
			_listener->handle_session_begin(_cur_tdate);

		for (; !_terminated;)
		{
			//要考虑到跨日的情况
			uint32_t mins = sInfo->timeToMinutes(_cur_time);
			//如果一开始不能整除,则直接修正一下
			if (mins % _task->_time != 0)
			{
				mins = mins / _task->_time + _task->_time;
				_cur_time = sInfo->minuteToTime(mins);
			}

			bool bNewTDate = false;
			if (mins < sInfo->getTradingMins())
			{
				onMinuteEnd(_cur_date, _cur_time, 0);
			}
			else
			{
				bNewTDate = true;
				mins = sInfo->getTradingMins();
				_cur_time = sInfo->getCloseTime();

				onMinuteEnd(_cur_date, _cur_time, _cur_tdate);

				if (_listener)
					_listener->handle_session_end(_cur_tdate);
			}


			if (bNewTDate)
			{
				//换日了
				mins = _task->_time;
				uint32_t nextTDate = _bd_mgr.getNextTDate(_task->_trdtpl, _cur_tdate, 1, true);

				if (sInfo->getOffsetMins() != 0)
				{
					if (sInfo->getOffsetMins() > 0)
					{
						//真实时间后移,说明夜盘算作下一天的
						_cur_date = _cur_tdate;
						_cur_tdate = nextTDate;
					}
					else
					{
						//真实时间前移,说明夜盘是上一天的,这种情况就不需要动了
						_cur_tdate = nextTDate;
						_cur_date = _cur_tdate;
					}
				}

				_cur_time = sInfo->minuteToTime(mins);

				if (_listener)
					_listener->handle_session_begin(nextTDate);
			}
			else
			{
				mins += _task->_time;
				uint32_t newTime = sInfo->minuteToTime(mins);
				bool bNewDay = newTime < _cur_time;
				if (bNewDay)
					_cur_date = TimeUtils::getNextDate(_cur_date);

				uint32_t dayMins = _cur_time / 100 * 60 + _cur_time % 100;
				uint32_t nextDMins = newTime / 100 * 60 + newTime % 100;

				//是否到了一个新的小节
				bool bNewSec = (nextDMins - dayMins > _task->_time) && !bNewDay;

				while (bNewSec && _bd_mgr.isHoliday(_task->_trdtpl, _cur_date, true))
					_cur_date = TimeUtils::getNextDate(_cur_date);

				_cur_time = newTime;
			}

			uint64_t nextTime = (uint64_t)_cur_date * 10000 + _cur_time;
			if (nextTime > _end_time)
			{
				WTSLogger::log_raw(LL_INFO, "Backtesting with task frequency is done");
				if (_listener)
				{
					_listener->handle_session_end(_cur_tdate);
					_listener->handle_replay_done();
					if (_notifier)
						_notifier->notifyEvent("BT_END");
				}
				break;
			}
		}
	}
}
void HisDataReplayer::replayUnbars(uint64_t stime, uint64_t nowTime, uint32_t endTDate /* = 0 */)
{
	//uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	uint32_t uDate = (uint32_t)(stime / 10000);

	for (auto& item : _unbars_cache)
	{
		BarsListPtr& barsList = (BarsListPtr&)item.second;
		if (barsList->_period != KP_DAY)
		{
			//如果历史数据指标不在尾部, 说明是回测模式, 要继续回放历史数据
			if (barsList->_bars.size() > barsList->_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList->_bars[barsList->_cursor];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						if(barTime <= stime)
							continue;

						//开高低收
						WTSTickStruct& curTS = _day_cache[barsList->_code];
						strcpy(curTS.code, barsList->_code.c_str());
						curTS.action_date = _cur_date;
						curTS.action_time = _cur_time * 100000;

						curTS.price = nextBar.open;
						curTS.volume = nextBar.vol;

						//更新开高低三个字段
						if (decimal::eq(curTS.open, 0))
							curTS.open = curTS.price;
						curTS.high = max(curTS.price, curTS.high);
						if (decimal::eq(curTS.low, 0))
							curTS.low = curTS.price;
						else
							curTS.low = min(curTS.price, curTS.low);


						WTSTickData* curTick = WTSTickData::create(curTS);
						_listener->handle_tick(barsList->_code.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.high;
						curTS.volume = nextBar.vol;
						curTS.high = max(curTS.price, curTS.high);
						curTS.low = min(curTS.price, curTS.low);
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(barsList->_code.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.low;
						curTS.high = max(curTS.price, curTS.high);
						curTS.low = min(curTS.price, curTS.low);
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(barsList->_code.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.close;
						curTS.high = max(curTS.price, curTS.high);
						curTS.low = min(curTS.price, curTS.low);
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(barsList->_code.c_str(), curTick);
						curTick->release();
					}
					else
					{
						break;
					}

					barsList->_cursor++;

					if (barsList->_cursor == barsList->_bars.size())
						break;
				}
			}
		}
		else
		{
			if (barsList->_bars.size() > barsList->_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList->_bars[barsList->_cursor];

					if (nextBar.date <= endTDate)
					{
						if (nextBar.date <= uDate)
							continue;

						CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(barsList->_code.c_str());
						WTSCommodityInfo* commInfo = _bd_mgr.getCommodity(cInfo._exchg, cInfo._product);

						std::string realCode = barsList->_code;
						if (commInfo->isStock() && cInfo.isExright())
							realCode = realCode.substr(0, realCode.size() - 1);

						WTSSessionInfo* sInfo = get_session_info(realCode.c_str(), true);
						uint32_t curTime = sInfo->getOpenTime();
						//开高低收
						WTSTickStruct curTS;
						strcpy(curTS.code, realCode.c_str());
						curTS.action_date = _cur_date;
						curTS.action_time = curTime * 100000;

						curTS.price = nextBar.open;
						curTS.volume = nextBar.vol;
						WTSTickData* curTick = WTSTickData::create(curTS);
						_listener->handle_tick(realCode.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.high;
						curTS.volume = nextBar.vol;
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(realCode.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.low;
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(realCode.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.close;
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(realCode.c_str(), curTick);
						curTick->release();
					}
					else
					{
						break;
					}

					barsList->_cursor++;

					if (barsList->_cursor >= barsList->_bars.size())
						break;
				}
			}
		}
	}
}

uint64_t HisDataReplayer::getNextTickTime(uint32_t curTDate, uint64_t stime /* = UINT64_MAX */)
{
	uint64_t nextTime = UINT64_MAX;
	for (auto& v : _tick_sub_map)
	{
		const char* stdCode = v.first.c_str();
		if (!checkTicks(stdCode, curTDate))
			continue;

		WTSSessionInfo* sInfo = get_session_info(stdCode, true);

		auto& tickList = _ticks_cache[stdCode];
		if (tickList._cursor == UINT_MAX)
		{
			if (stime == UINT64_MAX)
			{
				//By Wesley @ 2022.03.06
				//检查一下时间戳，如果不是交易时间的，就不回放了
				uint32_t idx = 0;
				while(true)
				{
					uint32_t tickMin = tickList._items[idx].action_time / 100000;
					if (sInfo->isInTradingTime(tickMin))
					{
						tickList._cursor = idx + 1;
						break;
					}
				}
			}
			else
			{
				uint32_t uDate = (uint32_t)(stime / 10000);
				uint32_t uTime = (uint32_t)(stime % 10000);

				WTSTickStruct curTick;
				curTick.action_date = uDate;
				curTick.action_time = uTime * 100000;

				auto tit = std::lower_bound(tickList._items.begin(), tickList._items.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
					if (a.action_date != b.action_date)
						return a.action_date < b.action_date;
					else
						return a.action_time < b.action_time;
				});

				uint32_t idx = tit - tickList._items.begin();
				tickList._cursor = idx + 1;
			}
		}

		if (tickList._cursor >= tickList._count)
			continue;

		const WTSTickStruct& nextTick = tickList._items[tickList._cursor - 1];
		//By Wesley @ 2022.03.06
		//检查一下时间戳，如果不是交易时间的，就不回放了
		if(!sInfo->isInTradingTime(nextTick.action_time/100000))
			continue;
		uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;

		nextTime = min(lastTime, nextTime);
	}

	return nextTime;
}


uint64_t HisDataReplayer::getNextTransTime(uint32_t curTDate, uint64_t stime /* = UINT64_MAX */)
{
	uint64_t nextTime = UINT64_MAX;
	for (auto v : _trans_sub_map)
	{
		std::string stdCode = v.first;
		if (!checkTransactions(stdCode.c_str(), curTDate))
			continue;

		auto& itemList = _trans_cache[stdCode];
		if (itemList._cursor == UINT_MAX)
		{
			if (stime == UINT64_MAX)
				itemList._cursor = 1;
			else
			{
				uint32_t uDate = (uint32_t)(stime / 10000);
				uint32_t uTime = (uint32_t)(stime % 10000);

				WTSTransStruct curItem;
				curItem.action_date = uDate;
				curItem.action_time = uTime * 100000;

				auto tit = std::lower_bound(itemList._items.begin(), itemList._items.end(), curItem, [](const WTSTransStruct& a, const WTSTransStruct& b) {
					if (a.action_date != b.action_date)
						return a.action_date < b.action_date;
					else
						return a.action_time < b.action_time;
				});

				uint32_t idx = tit - itemList._items.begin();
				itemList._cursor = idx + 1;
			}
		}

		if (itemList._cursor >= itemList._count)
			continue;

		const auto& nextItem = itemList._items[itemList._cursor - 1];
		uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;

		nextTime = min(lastTime, nextTime);
	}

	return nextTime;
}


uint64_t HisDataReplayer::getNextOrdDtlTime(uint32_t curTDate, uint64_t stime /* = UINT64_MAX */)
{
	uint64_t nextTime = UINT64_MAX;
	for (auto v : _orddtl_sub_map)
	{
		std::string stdCode = v.first;
		if (!checkOrderDetails(stdCode.c_str(), curTDate))
			continue;

		auto& itemList = _orddtl_cache[stdCode];
		if (itemList._cursor == UINT_MAX)
		{
			if (stime == UINT64_MAX)
				itemList._cursor = 1;
			else
			{
				uint32_t uDate = (uint32_t)(stime / 10000);
				uint32_t uTime = (uint32_t)(stime % 10000);

				WTSOrdDtlStruct curItem;
				curItem.action_date = uDate;
				curItem.action_time = uTime * 100000;

				auto tit = std::lower_bound(itemList._items.begin(), itemList._items.end(), curItem, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
					if (a.action_date != b.action_date)
						return a.action_date < b.action_date;
					else
						return a.action_time < b.action_time;
				});

				uint32_t idx = tit - itemList._items.begin();
				itemList._cursor = idx + 1;
			}
		}

		if (itemList._cursor >= itemList._count)
			continue;

		const auto& nextItem = itemList._items[itemList._cursor - 1];
		uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;

		nextTime = min(lastTime, nextTime);
	}

	return nextTime;
}


uint64_t HisDataReplayer::getNextOrdQueTime(uint32_t curTDate, uint64_t stime /* = UINT64_MAX */)
{
	uint64_t nextTime = UINT64_MAX;
	for (auto v : _ordque_sub_map)
	{
		std::string stdCode = v.first;
		if (!checkOrderQueues(stdCode.c_str(), curTDate))
			continue;

		auto& itemList = _ordque_cache[stdCode];
		if (itemList._cursor == UINT_MAX)
		{
			if (stime == UINT64_MAX)
				itemList._cursor = 1;
			else
			{
				uint32_t uDate = (uint32_t)(stime / 10000);
				uint32_t uTime = (uint32_t)(stime % 10000);

				WTSOrdQueStruct curItem;
				curItem.action_date = uDate;
				curItem.action_time = uTime * 100000;

				auto tit = std::lower_bound(itemList._items.begin(), itemList._items.end(), curItem, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
					if (a.action_date != b.action_date)
						return a.action_date < b.action_date;
					else
						return a.action_time < b.action_time;
				});

				uint32_t idx = tit - itemList._items.begin();
				itemList._cursor = idx + 1;
			}
		}

		if (itemList._cursor >= itemList._count)
			continue;

		const auto& nextItem = itemList._items[itemList._cursor - 1];
		uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;

		nextTime = min(lastTime, nextTime);
	}

	return nextTime;
}

uint64_t HisDataReplayer::replayHftDatasByDay(uint32_t curTDate)
{
	uint64_t total_ticks = 0;
	for (;!_terminated;)
	{
		//先确定下一笔tick的时间
		uint64_t nextTime = min(UINT64_MAX, getNextTickTime(curTDate));
		nextTime = min(nextTime, getNextOrdDtlTime(curTDate));
		nextTime = min(nextTime, getNextOrdQueTime(curTDate));
		nextTime = min(nextTime, getNextTransTime(curTDate));

		if(nextTime == UINT64_MAX)
			break;

		/*
		 *	By Wesley @ 2022.03.06
		 *	下面的回放逻辑，都改成先修改光标cursor，再触发回调
		 *	这个逻辑也符合实盘情况
		 */

		//再根据时间回放tick数据
		_cur_date = (uint32_t)(nextTime / 1000000000);
		_cur_time = nextTime % 1000000000 / 100000;
		_cur_secs = nextTime % 100000;

		//1、首先回放委托明细
		for (auto& v : _orddtl_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _orddtl_cache[stdCode];
			//By Wesley @ 2022.03.06 
			//这里加了一个数据的判断
			//如果数据为空，则不再进行回放
			if (itemList._items.empty() || itemList._cursor > itemList._count)
				continue;

			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				itemList._cursor++;

				WTSOrdDtlData* newData = WTSOrdDtlData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_order_detail(stdCode, newData);
				newData->release();

				total_ticks++;
			}
		}

		//2、其次再回放成交明细
		for (auto& v : _trans_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _trans_cache[stdCode];
			//By Wesley @ 2022.03.06 
			//这里加了一个数据的判断
			//如果数据为空，则不再进行回放
			if (itemList._items.empty())
				continue;

			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				itemList._cursor++;

				WTSTransData* newData = WTSTransData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_transaction(stdCode, newData);
				newData->release();
				
				total_ticks++;
			}
		}

		//3、第三步再回放tick数据
		for (auto& v : _tick_sub_map)
		{
			//std::string stdCode = v.first;
			const char* stdCode = v.first.c_str();
			HftDataList<WTSTickStruct>& tickList = _ticks_cache[stdCode];
			//By Wesley @ 2022.03.06 
			//这里加了一个数据的判断
			//如果数据为空，则不再进行回放
			if(tickList._items.empty())
				continue;

			WTSTickStruct& nextTick = tickList._items[tickList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;
			if (lastTime <= nextTime)
			{
				tickList._cursor++;

				update_price(stdCode, nextTick.price);
				WTSTickData* newTick = WTSTickData::create(nextTick);
				newTick->setCode(stdCode);
				_listener->handle_tick(stdCode, newTick);
				newTick->release();
				
				total_ticks++;
			}
		}
		
		//4、最后回放委托队列
		for (auto& v : _ordque_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _ordque_cache[stdCode];
			//By Wesley @ 2022.03.06 
			//这里加了一个数据的判断
			//如果数据为空，则不再进行回放
			if (itemList._items.empty())
				continue;

			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				itemList._cursor++;

				WTSOrdQueData* newData = WTSOrdQueData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_order_queue(stdCode, newData);
				newData->release();
				
				total_ticks++;
			}
		}
	}

	return total_ticks;
}

bool HisDataReplayer::replayHftDatas(uint64_t stime, uint64_t etime)
{	
	for (;;)
	{
		uint64_t nextTime = min(UINT64_MAX, getNextTickTime(_cur_tdate, stime));
		if (nextTime == UINT64_MAX)
			return false;

		nextTime = min(nextTime, getNextOrdDtlTime(_cur_tdate, stime));
		nextTime = min(nextTime, getNextOrdQueTime(_cur_tdate, stime));
		nextTime = min(nextTime, getNextTransTime(_cur_tdate, stime));

		if (nextTime/100000 >= etime)
			break;

		_cur_date = (uint32_t)(nextTime / 1000000000);
		_cur_time = nextTime % 1000000000 / 100000;
		_cur_secs = nextTime % 100000;
		
		//1、首先回放委托明细
		for (auto& v : _orddtl_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _orddtl_cache[stdCode];
			if (itemList._cursor >= itemList._count)
				continue;
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdDtlData* newData = WTSOrdDtlData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_order_detail(stdCode, newData);
				newData->release();

				itemList._cursor++;
			}
		}

		//2、其次再回放成交明细
		for (auto& v : _trans_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _trans_cache[stdCode];
			if (itemList._cursor >= itemList._count)
				continue;
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSTransData* newData = WTSTransData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_transaction(stdCode, newData);
				newData->release();

				itemList._cursor++;
			}
		}

		//3、第三步再回放tick数据
		for (auto& v : _tick_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _ticks_cache[stdCode];
			if (itemList._cursor >= itemList._count)
				continue;
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				update_price(stdCode, nextItem.price);
				WTSTickData* newData = WTSTickData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_tick(stdCode, newData);
				newData->release();

				itemList._cursor++;
			}
		}

		//4、最后回放委托队列
		for (auto& v : _ordque_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _ordque_cache[stdCode];
			if (itemList._cursor >= itemList._count)
				continue;
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdQueData* newData = WTSOrdQueData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_order_queue(stdCode, newData);
				newData->release();

				itemList._cursor++;
			}
		}
	}

	return true;
}

void HisDataReplayer::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */, bool tickSimulated /* = true */)
{
	//这里应该触发检查
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsListPtr& barsList = (BarsListPtr&)it->second;
		double factor = barsList->_factor;
		if (barsList->_period != KP_DAY)
		{
			//如果历史数据指标不在尾部, 说明是回测模式, 要继续回放历史数据
			if (barsList->_bars.size() > barsList->_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList->_bars[barsList->_cursor];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						//if (_task == NULL)
						{
							if (tickSimulated)
							{
								const std::string& ticker = _ticker_keys[barsList->_code];
								if (ticker == it->first)
								{
									//开高低收
									WTSTickStruct& curTS = _day_cache[barsList->_code];
									strcpy(curTS.code, barsList->_code.c_str());
									curTS.action_date = _cur_date;
									curTS.action_time = _cur_time * 100000;

									curTS.price = nextBar.open / factor;
									curTS.volume = nextBar.vol;

									//更新开高低三个字段
									if (decimal::eq(curTS.open, 0))
										curTS.open = curTS.price;
									curTS.high = max(curTS.price, curTS.high);
									if (decimal::eq(curTS.low, 0))
										curTS.low = curTS.price;
									else
										curTS.low = min(curTS.price, curTS.low);

									update_price(barsList->_code.c_str(), curTS.price);
									WTSTickData* curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList->_code.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.high / factor;
									curTS.volume = nextBar.vol;
									curTS.high = max(curTS.price, curTS.high);
									curTS.low = min(curTS.price, curTS.low);
									update_price(barsList->_code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList->_code.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.low / factor;
									curTS.high = max(curTS.price, curTS.high);
									curTS.low = min(curTS.price, curTS.low);
									update_price(barsList->_code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList->_code.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.close / factor;
									curTS.high = max(curTS.price, curTS.high);
									curTS.low = min(curTS.price, curTS.low);
									update_price(barsList->_code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList->_code.c_str(), curTick);
								}
							}

							uint32_t times = barsList->_times;
							if (barsList->_period == KP_Minute5)
								times *= 5;
							_listener->handle_bar_close(barsList->_code.c_str(), "m", times, &nextBar);
						}
					}
					else
					{
						break;
					}

					barsList->_cursor++;

					if (barsList->_cursor == barsList->_bars.size())
						break;
				}
			}
		}
		else
		{
			if (barsList->_bars.size() > barsList->_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList->_bars[barsList->_cursor];

					if (nextBar.date <= endTDate)
					{
						//if (_task == NULL)
						{
							if (tickSimulated)
							{
								const std::string& ticker = _ticker_keys[barsList->_code];
								if (ticker == it->first)
								{
									CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(barsList->_code.c_str());
									WTSCommodityInfo* commInfo = _bd_mgr.getCommodity(cInfo._exchg, cInfo._product);

									std::string realCode = barsList->_code;
									if (commInfo->isStock() && cInfo._exright)
										realCode = realCode.substr(0, realCode.size() - 1);

									WTSSessionInfo* sInfo = get_session_info(realCode.c_str(), true);
									uint32_t curTime = sInfo->getCloseTime();
									//开高低收
									WTSTickStruct curTS;
									strcpy(curTS.code, realCode.c_str());
									curTS.action_date = _cur_date;
									curTS.action_time = curTime * 100000;

									curTS.price = nextBar.open / factor;
									curTS.volume = nextBar.vol;
									update_price(barsList->_code.c_str(), curTS.price);
									WTSTickData* curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.high / factor;
									curTS.volume = nextBar.vol;
									update_price(barsList->_code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.low / factor;
									update_price(barsList->_code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.close / factor;
									update_price(barsList->_code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
								}
							}

							_listener->handle_bar_close(barsList->_code.c_str(), "d", barsList->_times, &nextBar);
						}
					}
					else
					{
						break;
					}

					barsList->_cursor++;

					if (barsList->_cursor >= barsList->_bars.size())
						break;
				}
			}
		}
	}

	if (_listener)
		_listener->handle_schedule(uDate, uTime);
}

WTSKlineSlice* HisDataReplayer::get_kline_slice(const char* stdCode, const char* period, uint32_t count, uint32_t times /* = 1 */, bool isMain /* = false */)
{
	std::string key = StrUtil::printf("%s#%s#%u", stdCode, period, times);
	if (isMain)
		_main_key = key;

	//if(!_tick_enabled)
	//不做判断,主要为了防止没有tick数据,而采用第二方案
	{
		if(_ticker_keys.find(stdCode) == _ticker_keys.end())
			_ticker_keys[stdCode] = key;
		else
		{
			std::string oldKey = _ticker_keys[stdCode];
			oldKey = oldKey.substr(strlen(stdCode) + 1);
			if (strcmp(period, "m") == 0 && oldKey.at(0) == 'd')
			{
				_ticker_keys[stdCode] = key;
				_min_period = period;
			}
			else if (oldKey.at(0) == period[0] && times < strtoul(oldKey.substr(2, 1).c_str(), NULL, 10))
			{
				_ticker_keys[stdCode] = key;
				_min_period = period;
			}
		}
	}

	WTSKlinePeriod kp;
	uint32_t realTimes = times;
	uint32_t baseTimes = 1;
	if (strcmp(period, "m") == 0)
	{
		if(times % 5 == 0)
		{
			kp = KP_Minute5;
			baseTimes = 5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	bool isDay = kp == KP_DAY;

	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	bool bHasCache = (it != _bars_cache.end());
	if (!bHasCache)
	{
		if (realTimes != 1)
		{
			std::string rawKey = StrUtil::printf("%s#%s#%u", stdCode, period, baseTimes);
			if (_bars_cache.find(rawKey) == _bars_cache.end())
			{
				/*
				 *	By Wesley @ 2021.12.20
				 *	先从extloader加载数据，如果加载不到，再走原来的历史数据存储引擎加载
				 */
				if(NULL != _bt_loader)
					bHasHisData = cacheFinalBarsFromLoader(rawKey, stdCode, kp);
				
				if(!bHasHisData)
				{
					if (_mode == "csv")
					{
						bHasHisData = cacheRawBarsFromCSV(rawKey, stdCode, kp);
					}
					else
					{
						bHasHisData = cacheRawBarsFromBin(rawKey, stdCode, kp);
					}
				}
				
			}
			else
			{
				bHasHisData = true;
			}
		}
		else
		{
			/*
			 *	By Wesley @ 2021.12.20
			 *	先从extloader加载数据，如果加载不到，再走原来的历史数据存储引擎加载
			 */
			if (NULL != _bt_loader)
			{
				bHasHisData = cacheFinalBarsFromLoader(key, stdCode, kp);
			}

			if(!bHasHisData)
			{
				if (_mode == "csv")
				{
					bHasHisData = cacheRawBarsFromCSV(key, stdCode, kp);
				}
				else
				{
					bHasHisData = cacheRawBarsFromBin(key, stdCode, kp);
				}
			}
			
		}
	}
	else
	{
		bHasHisData = true;
	}

	if (!bHasHisData)
		return NULL;

	WTSSessionInfo* sInfo = get_session_info(stdCode, true);
	if(sInfo == NULL)
	{
		WTSLogger::error_f("Cannot find corresponding session of {}", stdCode);
		return NULL;
	}

	bool isClosed = (sInfo->offsetTime(_cur_time, true) >= sInfo->getCloseTime(true));
	if (realTimes != 1 && !bHasCache)
	{	
		std::string rawKey = StrUtil::printf("%s#%s#%u", stdCode, period, baseTimes);
		_bars_cache[rawKey].reset(new BarsList());
		BarsListPtr& rawBars = _bars_cache[rawKey];
		WTSKlineSlice* rawKline = WTSKlineSlice::create(stdCode, kp, realTimes, &rawBars->_bars[0], rawBars->_bars.size());
		rawKline->setCode(stdCode);

		static WTSDataFactory dataFact;
		WTSKlineData* kData = dataFact.extractKlineData(rawKline, kp, realTimes, sInfo, true);
		rawKline->release();

		if(kData)
		{
			_bars_cache[key].reset(new BarsList());
			BarsListPtr barsList = _bars_cache[key];
			barsList->_code = stdCode;
			barsList->_period = kp;
			barsList->_times = realTimes;
			barsList->_count = kData->size();
			barsList->_bars.swap(kData->getDataRef());
			kData->release();
			WTSLogger::info_f("{} resampled {}{} back kline of {} ready", barsList->_bars.size(), period, times, stdCode);
		}
		else
		{
			WTSLogger::error_f("Resampling {}{} back kline of {} failed", period, times, stdCode);
			return NULL;
		}
	}

	BarsListPtr& kBlkPair = _bars_cache[key];
	if(kBlkPair == NULL)
	{
		return NULL;
	}

	if (kBlkPair->_cursor == UINT_MAX)
	{
		//还没有经过初始定位
		WTSBarStruct bar;
		bar.date = _cur_tdate;
		if(kp != KP_DAY)
			bar.time = (_cur_date - 19900000) * 10000 + _cur_time;
		
		auto it = std::lower_bound(kBlkPair->_bars.begin(), kBlkPair->_bars.end(), bar, [isDay, isClosed](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				if (!isClosed)
					return a.date < b.date;
				else 
					return a.date <= b.date;
			else
				return a.time < b.time;
		});

		uint32_t eIdx = it - kBlkPair->_bars.begin();

		if (it != kBlkPair->_bars.end())
		{
			WTSBarStruct& curBar = *it;
			if (isDay)
			{
				if (curBar.date >= _cur_tdate && !isClosed)
				{
					if (eIdx > 0)
					{
						it--;
						eIdx--;
					}

				}
				else if (curBar.date > _cur_tdate && isClosed)
				{
					if (eIdx > 0)
					{
						it--;
						eIdx--;
					}

				}
			}
			else
			{
				if (curBar.time > bar.time)
				{
					if (eIdx > 0)
					{
						it--;
						eIdx--;
					}
				}
			}

			kBlkPair->_cursor = eIdx + 1;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		uint32_t curMin = (_cur_date - 19900000) * 10000 + _cur_time;
		if (isDay)
		{
			if (kBlkPair->_cursor <= kBlkPair->_count)
			{
				if(!isClosed)
				{
					while (kBlkPair->_bars[kBlkPair->_cursor - 1].date < _cur_tdate  && kBlkPair->_cursor < kBlkPair->_count && kBlkPair->_bars[kBlkPair->_cursor].date < _cur_tdate)
					{
						kBlkPair->_cursor++;
					}
				}
				else
				{
					while (kBlkPair->_bars[kBlkPair->_cursor - 1].date <= _cur_tdate  && kBlkPair->_cursor < kBlkPair->_count && kBlkPair->_bars[kBlkPair->_cursor].date <= _cur_tdate)
					{
						kBlkPair->_cursor++;
					}
				}
				

				if (kBlkPair->_bars[kBlkPair->_cursor - 1].date > _cur_tdate)
					kBlkPair->_cursor--;
			}
		}
		else
		{
			if (kBlkPair->_cursor <= kBlkPair->_count)
			{
				while (kBlkPair->_bars[kBlkPair->_cursor-1].time < curMin && kBlkPair->_cursor < kBlkPair->_count)
				{
					kBlkPair->_cursor++;
				}

				if (kBlkPair->_bars[kBlkPair->_cursor - 1].time > curMin)
					kBlkPair->_cursor--;
			}
		}
	}


	if (kBlkPair->_cursor == 0)
		return NULL;

	uint32_t sIdx = 0;
	if (kBlkPair->_cursor > count)
		sIdx = kBlkPair->_cursor - count;

	uint32_t realCnt = kBlkPair->_cursor - sIdx;
	WTSKlineSlice* kline = WTSKlineSlice::create(stdCode, kp, 1, kBlkPair->_bars.data() + sIdx, realCnt);
	return kline;
}

WTSTickSlice* HisDataReplayer::get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime)
{
	if (!_tick_enabled)
		return NULL;

	if (!checkTicks(stdCode, _cur_tdate))
		return NULL;

	auto& tickList = _ticks_cache[stdCode];
	if (tickList._cursor == 0)
		return NULL;

	if (tickList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;

		if (etime != 0)
		{
			uDate = (uint32_t)(etime / 10000);
			uTime = (uint32_t)(etime % 10000 * 100000);
		}

		WTSTickStruct curTick;
		curTick.action_date = uDate;
		curTick.action_time = uTime;

		auto tit = std::lower_bound(tickList._items.begin(), tickList._items.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		if(tit == tickList._items.end())
		{
			tickList._cursor = tickList._items.size();
		}
		else
		{
			
			uint32_t idx = tit - tickList._items.begin();
			const WTSTickStruct& thisTick = *tit;
			if (thisTick.action_date > uDate || (thisTick.action_date == uDate && thisTick.action_time > uTime))
			{
				if(idx > 0)
				{
					tit--;
					idx--;
				}
				else
				{
					return NULL;
				}
			}

			tickList._cursor = idx + 1;
		}
	}
	
	//cursor是下一笔tick的index+1，大于当前截止时间的
	//所以要获取当前截止时间之前的最后一笔tick，需要-2
	uint32_t eIdx = tickList._cursor - 2;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSTickSlice* ticks = WTSTickSlice::create(stdCode, tickList._items.data() + sIdx, realCnt);
	return ticks;
}

WTSOrdDtlSlice* HisDataReplayer::get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (!checkOrderDetails(stdCode, _cur_tdate))
		return NULL;

	auto& dataList = _orddtl_cache[stdCode];
	if (dataList._cursor == 0)
		return NULL;

	if (dataList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;

		if (etime != 0)
		{
			uDate = (uint32_t)(etime / 10000);
			uTime = (uint32_t)(etime % 10000 * 100000);
		}

		WTSOrdDtlStruct curItem;
		curItem.action_date = uDate;
		curItem.action_time = uTime;

		auto tit = std::lower_bound(dataList._items.begin(), dataList._items.end(), curItem, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - dataList._items.begin();
		dataList._cursor = idx + 1;
	}

	uint32_t eIdx = dataList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSOrdDtlSlice* ticks = WTSOrdDtlSlice::create(stdCode, dataList._items.data() + sIdx, realCnt);
	return ticks;
}

WTSOrdQueSlice* HisDataReplayer::get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (!checkOrderQueues(stdCode, _cur_tdate))
		return NULL;

	auto& dataList = _ordque_cache[stdCode];
	if (dataList._cursor == 0)
		return NULL;

	if (dataList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;

		if (etime != 0)
		{
			uDate = (uint32_t)(etime / 10000);
			uTime = (uint32_t)(etime % 10000 * 100000);
		}

		WTSOrdQueStruct curItem;
		curItem.action_date = uDate;
		curItem.action_time = uTime;

		auto tit = std::lower_bound(dataList._items.begin(), dataList._items.end(), curItem, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - dataList._items.begin();
		dataList._cursor = idx + 1;
	}

	uint32_t eIdx = dataList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSOrdQueSlice* ticks = WTSOrdQueSlice::create(stdCode, dataList._items.data() + sIdx, realCnt);
	return ticks;
}

WTSTransSlice* HisDataReplayer::get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (!checkTransactions(stdCode, _cur_tdate))
		return NULL;

	auto& dataList = _trans_cache[stdCode];
	if (dataList._cursor == 0)
		return NULL;

	if (dataList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;

		if (etime != 0)
		{
			uDate = (uint32_t)(etime / 10000);
			uTime = (uint32_t)(etime % 10000 * 100000);
		}

		WTSTransStruct curItem;
		curItem.action_date = uDate;
		curItem.action_time = uTime;

		auto tit = std::lower_bound(dataList._items.begin(), dataList._items.end(), curItem, [](const WTSTransStruct& a, const WTSTransStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - dataList._items.begin();
		dataList._cursor = idx + 1;
	}

	uint32_t eIdx = dataList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSTransSlice* ticks = WTSTransSlice::create(stdCode, dataList._items.data() + sIdx, realCnt);
	return ticks;
}

bool HisDataReplayer::checkAllTicks(uint32_t uDate)
{
	bool bHasTick = false;
	for (auto& v : _tick_sub_map)
	{
		bHasTick = bHasTick || checkTicks(v.first.c_str(), uDate);
	}

	return bHasTick;
}

bool HisDataReplayer::checkOrderDetails(const char* stdCode, uint32_t uDate)
{
	bool bNeedCache = false;
	auto it = _ticks_cache.find(stdCode);
	if (it == _ticks_cache.end())
		bNeedCache = true;
	else
	{
		auto& tickList = it->second;
		if (tickList._date != uDate)
			bNeedCache = true;
	}


	if (bNeedCache)
	{
		bool hasTicks = false;
		if(NULL != _bt_loader)
		{
			hasTicks = cacheRawTicksFromLoader(stdCode, stdCode, uDate);
		}
		else if (_mode == "csv")
		{
			hasTicks = cacheRawTicksFromCSV(stdCode, stdCode, uDate);
		}
		else
		{
			hasTicks = cacheRawTicksFromBin(stdCode, stdCode, uDate);
		}

		if (!hasTicks)
			return false;
	}

	return true;
}

bool HisDataReplayer::checkOrderQueues(const char* stdCode, uint32_t uDate)
{
	bool bNeedCache = false;
	auto it = _ticks_cache.find(stdCode);
	if (it == _ticks_cache.end())
		bNeedCache = true;
	else
	{
		auto& tickList = it->second;
		if (tickList._date != uDate)
			bNeedCache = true;
	}


	if (bNeedCache)
	{
		bool hasTicks = false;
		if (NULL != _bt_loader)
		{
			hasTicks = cacheRawTicksFromLoader(stdCode, stdCode, uDate);
		}
		else if (_mode == "csv")
		{
			hasTicks = cacheRawTicksFromCSV(stdCode, stdCode, uDate);
		}
		else
		{
			hasTicks = cacheRawTicksFromBin(stdCode, stdCode, uDate);
		}

		if (!hasTicks)
			return false;
	}

	return true;
}

bool HisDataReplayer::checkTransactions(const char* stdCode, uint32_t uDate)
{
	bool bNeedCache = false;
	auto it = _ticks_cache.find(stdCode);
	if (it == _ticks_cache.end())
		bNeedCache = true;
	else
	{
		auto& tickList = it->second;
		if (tickList._date != uDate)
			bNeedCache = true;
	}


	if (bNeedCache)
	{
		bool hasTicks = false;
		if (_mode == "csv")
		{
			hasTicks = cacheRawTicksFromCSV(stdCode, stdCode, uDate);
		}
		else
		{
			hasTicks = cacheRawTicksFromBin(stdCode, stdCode, uDate);
		}

		if (!hasTicks)
			return false;
	}

	return true;
}

bool HisDataReplayer::checkTicks(const char* stdCode, uint32_t uDate)
{
	if (strlen(stdCode) == 0)
		return false;

	bool bNeedCache = false;
	auto it = _ticks_cache.find(stdCode);
	if (it == _ticks_cache.end())
		bNeedCache = true;
	else
	{
		HftDataList<WTSTickStruct>& tickList = (HftDataList<WTSTickStruct>&)it->second;
		if (tickList._date != uDate)
		{
			tickList._items.clear();
			bNeedCache = true;
		}
		else if (tickList._count == 0)
			return false;
	}
	
	if (bNeedCache)
	{
		bool hasTicks = false;
		if (NULL != _bt_loader)
		{
			hasTicks = cacheRawTicksFromLoader(stdCode, stdCode, uDate);
		}
		else if (_mode == "csv")
		{
			hasTicks = cacheRawTicksFromCSV(stdCode, stdCode, uDate);
		}
		else
		{
			hasTicks = cacheRawTicksFromBin(stdCode, stdCode, uDate);
		}

		if (!hasTicks)
		{
			auto& ticksList = _ticks_cache[stdCode];
			ticksList._items.resize(0);
			ticksList._cursor = UINT_MAX;
			ticksList._code = stdCode;
			ticksList._date = uDate;
			ticksList._count = 0;
			return false;
		}
	}


	return true;
}

WTSTickData* HisDataReplayer::get_last_tick(const char* stdCode)
{
	if (!checkTicks(stdCode, _cur_tdate))
		return NULL;

	auto& tickList = _ticks_cache[stdCode];
	if (tickList._cursor == 0)
		return NULL;

	if (tickList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;

		WTSTickStruct curTick;
		curTick.action_date = uDate;
		curTick.action_time = uTime;

		auto tit = std::lower_bound(tickList._items.begin(), tickList._items.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - tickList._items.begin();
		tickList._cursor = idx + 1;
	}
	else if (tickList._cursor > tickList._count)
		return NULL;

	return WTSTickData::create(tickList._items[tickList._cursor - 1]);
}

WTSCommodityInfo* HisDataReplayer::get_commodity_info(const char* stdCode)
{
	return _bd_mgr.getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
}

WTSSessionInfo* HisDataReplayer::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _bd_mgr.getSession(sid);

	WTSCommodityInfo* cInfo = _bd_mgr.getCommodity(CodeHelper::stdCodeToStdCommID(sid).c_str());
	if (cInfo == NULL)
		return NULL;

	return _bd_mgr.getSession(cInfo->getSession());
}

void HisDataReplayer::loadFees(const char* filename)
{
	if (strlen(filename) == 0)
		return;

	if (!StdFile::exists(filename))
	{
		WTSLogger::error_f("Fees template file {} not exists", filename);
		return;
	}

	WTSVariant* cfg = WTSCfgLoader::load_from_file(filename, true);
	if (cfg == NULL)
	{
		WTSLogger::error_f("Converting fees template file {} failed", filename);
		return;
	}

	auto keys = cfg->memberNames();
	for (const std::string& key : keys)
	{
		WTSVariant* cfgItem = cfg->get(key.c_str());
		FeeItem& fItem = _fee_map[key];
		fItem._by_volume = cfgItem->getBoolean("byvolume");
		fItem._open = cfgItem->getDouble("open");
		fItem._close = cfgItem->getDouble("close");
		fItem._close_today = cfgItem->getDouble("closetoday");
	}

	cfg->release();

	WTSLogger::info_f("{} items of fees template loaded", _fee_map.size());
}


double HisDataReplayer::calc_fee(const char* stdCode, double price, double qty, uint32_t offset)
{
	std::string stdPID = CodeHelper::stdCodeToStdCommID(stdCode);
	auto it = _fee_map.find(stdPID);
	if (it == _fee_map.end())
		return 0.0;

	double ret = 0.0;
	WTSCommodityInfo* commInfo = _bd_mgr.getCommodity(stdPID.c_str());
	const FeeItem& fItem = it->second;
	if (fItem._by_volume)
	{
		switch (offset)
		{
		case 0: ret = fItem._open*qty; break;
		case 1: ret = fItem._close*qty; break;
		case 2: ret = fItem._close_today*qty; break;
		default: ret = 0.0; break;
		}
	}
	else
	{
		double amount = price*qty*commInfo->getVolScale();
		switch (offset)
		{
		case 0: ret = fItem._open*amount; break;
		case 1: ret = fItem._close*amount; break;
		case 2: ret = fItem._close_today*amount; break;
		default: ret = 0.0; break;
		}
	}

	return (int32_t)(ret * 100 + 0.5) / 100.0;
}

double HisDataReplayer::get_cur_price(const char* stdCode)
{
	auto it = _price_map.find(stdCode);
	if (it == _price_map.end())
	{
		return 0.0;
	}
	else
	{
		return it->second;
	}
}

void HisDataReplayer::sub_tick(uint32_t sid, const char* stdCode)
{
	if (strlen(stdCode) == 0)
		return;

	SIDSet& sids = _tick_sub_map[stdCode];
	sids.insert(sid);
}

void HisDataReplayer::sub_order_detail(uint32_t sid, const char* stdCode)
{
	if (strlen(stdCode) == 0)
		return;

	SIDSet& sids = _orddtl_sub_map[stdCode];
	sids.insert(sid);
}

void HisDataReplayer::sub_order_queue(uint32_t sid, const char* stdCode)
{
	if (strlen(stdCode) == 0)
		return;

	SIDSet& sids = _ordque_sub_map[stdCode];
	sids.insert(sid);
}

void HisDataReplayer::sub_transaction(uint32_t sid, const char* stdCode)
{
	if (strlen(stdCode) == 0)
		return;

	SIDSet& sids = _trans_sub_map[stdCode];
	sids.insert(sid);
}

void HisDataReplayer::checkUnbars()
{
	for(auto& m : _tick_sub_map)
	{
		const char* stdCode = m.first.c_str();
		bool bHasBars = false;
		for (auto& m : _unbars_cache)
		{
			const std::string& key = m.first;
			auto ay = StrUtil::split(key, "#");
			if (ay[0].compare(stdCode) == 0)
			{
				bHasBars = true;
				break;
			}
		}
		if(bHasBars)
			continue;

		for (auto& m : _bars_cache)
		{
			const std::string& key = m.first;
			auto ay = StrUtil::split(key, "#");
			if (ay[0].compare(stdCode) == 0)
			{
				bHasBars = true;
				break;
			}
		}

		if (bHasBars)
			continue;

		//如果订阅了tick,但是没有对应的K线数据,则自动加载1分钟线到内存中
		bool bHasHisData = false;
		std::string key = StrUtil::printf("%s#m#1", stdCode);

		/*
		 *	By Wesley @ 2021.12.20
		 *	先从extloader加载最终的K线数据
		 *	如果加载不到，再从配置的历史数据存储引擎加载数据
		 */
		if (NULL != _bt_loader)
		{
			bHasHisData = cacheFinalBarsFromLoader(key, stdCode, KP_Minute1, false);
		}

		if(!bHasHisData)
		{
			if (_mode == "csv")
			{
				bHasHisData = cacheRawBarsFromCSV(key, stdCode, KP_Minute1, false);
			}
			else
			{
				bHasHisData = cacheRawBarsFromBin(key, stdCode, KP_Minute1, false);
			}
		}
		
		
		if (!bHasHisData)
			continue;

		WTSSessionInfo* sInfo = get_session_info(stdCode, true);

		BarsListPtr& kBlkPair = _unbars_cache[key];
		
		//还没有经过初始定位
		WTSBarStruct bar;
		bar.date = _cur_tdate;
		bar.time = (_cur_date - 19900000) * 10000 + _cur_time;

		auto it = std::lower_bound(kBlkPair->_bars.begin(), kBlkPair->_bars.end(), bar, [](const WTSBarStruct& a, const WTSBarStruct& b) {
			return a.time < b.time;
		});

		uint32_t eIdx = it - kBlkPair->_bars.begin();

		if (it != kBlkPair->_bars.end())
		{
			WTSBarStruct& curBar = *it;
			if (curBar.time > bar.time)
			{
				if (eIdx > 0)
				{
					it--;
					eIdx--;
				}
			}

			kBlkPair->_cursor = eIdx + 1;
		}
	}
}

uint32_t strToTime(const char* strTime, bool bHasSec = false)
{
	std::string str;
	const char *pos = strTime;
	while (strlen(pos) > 0)
	{
		if (pos[0] != ':')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	uint32_t ret = strtoul(str.c_str(), NULL, 10);
	if (str.size() > 4 && !bHasSec)
		ret /= 100;

	return ret;
}

uint32_t strToDate(const char* strDate)
{
	StringVector ay = StrUtil::split(strDate, "/");
	if(ay.size() == 1)
		ay = StrUtil::split(strDate, "-");
	std::stringstream ss;
	if (ay.size() > 1)
	{
		auto pos = ay[2].find(" ");
		if (pos != std::string::npos)
			ay[2] = ay[2].substr(0, pos);
		ss << ay[0] << (ay[1].size() == 1 ? "0" : "") << ay[1] << (ay[2].size() == 1 ? "0" : "") << ay[2];
	}
	else
		ss << ay[0];

	return strtoul(ss.str().c_str(), NULL, 10);
}

bool HisDataReplayer::cacheRawTicksFromBin(const std::string& key, const char* stdCode, uint32_t uDate)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);
	
	std::string rawCode = cInfo._code;
	if (cInfo.isHot())
	{
		rawCode = _hot_mgr.getRawCode(cInfo._exchg, cInfo._product, uDate);
	}
	else if(cInfo.isSecond())
	{
		rawCode = _hot_mgr.getSecondRawCode(cInfo._exchg, cInfo._product, uDate);
	}

	std::string content;
	bool bHit = false;
	//先检查有没有HOT、SND的主力次主力的tick文件
	if(!cInfo.isFlat())
	{
		const char* hot_flag = cInfo.isHot() ? FILE_SUF_HOT : FILE_SUF_2ND;
		//std::stringstream ss;
		//ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << cInfo._product << hot_flag << ".dsb";
		//filename = ss.str();
		//if (StdFile::exists(filename.c_str()))
		//	bHit = true;
		/*
		 *	By Wesley @ 2022.01.11
		 *	这里将直接从文件读取，改成从HisDtMgr封装的接口加载
		 */
		std::string wrappCode = StrUtil::printf("%s%s", cInfo._product, hot_flag);
		bHit = _his_dt_mgr.load_raw_ticks(cInfo._exchg, wrappCode.c_str(), uDate, [&content](std::string& data) {
			content.swap(data);
		});
	}

	//如果没有找到，则读取分月合约
	if (!bHit)
	{
		//std::stringstream ss;
		//ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << rawCode << ".dsb";
		//filename = ss.str();
		/*
		 *	By Wesley @ 2022.01.11
		 *	这里将直接从文件读取，改成从HisDtMgr封装的接口加载
		 */
		bHit = _his_dt_mgr.load_raw_ticks(cInfo._exchg, rawCode.c_str(), uDate, [&content](std::string& data) {
			content.swap(data);
		});
	}

	if(!bHit)
	{
		WTSLogger::warn_f("No ticks data of {} on {} found", stdCode, uDate);
		return false;
	}

	auto& ticksList = _ticks_cache[key];
	uint32_t tickcnt = 0;
	tickcnt = content.size() / sizeof(WTSTickStruct);
	ticksList._items.resize(tickcnt);
	memcpy(ticksList._items.data(), content.data(), content.size());
	
	ticksList._cursor = UINT_MAX;
	ticksList._code = stdCode;
	ticksList._date = uDate;
	ticksList._count = tickcnt;

	return true;
}

bool HisDataReplayer::cacheRawTicksFromLoader(const std::string& key, const char* stdCode, uint32_t uDate)
{
	if (NULL == _bt_loader)
		return false;

	auto& ticksList = _ticks_cache[key];
	ticksList._cursor = UINT_MAX;
	ticksList._code = stdCode;
	ticksList._date = uDate;
	ticksList._count = 0;

	bool bSucc = _bt_loader->loadRawHisTicks(&ticksList, stdCode, uDate, [](void* obj, WTSTickStruct* firstTick, uint32_t count) {
		HftDataList<WTSTickStruct>* ticks = (HftDataList<WTSTickStruct>*)obj;
		ticks->_items.resize(count);
		ticks->_count = count;
		memcpy(ticks->_items.data(), firstTick, sizeof(WTSTickStruct)*count);
	});

	if (!bSucc)
		return false;

	WTSLogger::info_f("{} items of back tick data of {} on {} loaded via extended loader", ticksList._count, stdCode, uDate);

	return true;
}

bool HisDataReplayer::cacheRawTicksFromCSV(const std::string& key, const char* stdCode, uint32_t uDate)
{
	if (strlen(stdCode) == 0)
		return false;

	std::stringstream ss;
	ss << _base_dir << "bin/ticks/";
	std::string path = ss.str();
	if (!StdFile::exists(path.c_str()))
		boost::filesystem::create_directories(path.c_str());
	ss << stdCode << "_tick_" << uDate << ".dsb";
	std::string filename = ss.str();
	if (StdFile::exists(filename.c_str()))
	{
		//如果有格式化的历史数据文件, 则直接读取
		WTSLogger::info_f("Reading data from {}...", filename);
		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisTickBlockV2))
		{
			WTSLogger::error_f("Sizechecking of back tick data file {} failed", filename);
			return false;
		}

		WTSLogger::info_f("Processing file content of {}...", filename);
		proc_block_data(filename.c_str(), content, false, false);
		uint32_t tickcnt = content.size() / sizeof(WTSTickStruct);
		auto& ticksList = _ticks_cache[key];
		ticksList._items.resize(tickcnt);
		memcpy(ticksList._items.data(), content.data(), content.size());
		ticksList._cursor = UINT_MAX;
		ticksList._code = stdCode;
		ticksList._date = uDate;
		ticksList._count = tickcnt;
	}
	else
	{
		//如果没有格式化的历史数据文件, 则从csv加载
		std::stringstream ss;
		ss << _base_dir << "csv/ticks/" << stdCode << "_tick_" << uDate << ".csv";
		std::string csvfile = ss.str();

		if (!StdFile::exists(csvfile.c_str()))
		{
			WTSLogger::error_f("Back tick data file {} not exists", csvfile.c_str());
			return false;
		}

		std::ifstream ifs;
		ifs.open(csvfile.c_str());

		WTSLogger::info_f("Reading data from {}...", csvfile.c_str());

		char buffer[1024];
		bool headerskipped = false;
		auto& tickList = _ticks_cache[key];
		tickList._code = stdCode;
		tickList._date = uDate;
		while (!ifs.eof())
		{
			ifs.getline(buffer, 1024);
			if (strlen(buffer) == 0)
				continue;

			//跳过头部
			if (!headerskipped)
			{
				headerskipped = true;
				continue;
			}

			//逐行读取
			StringVector ay = StrUtil::split(buffer, ",");
			WTSTickStruct ticks;
			ticks.action_date = strToDate(ay[0].c_str());
			ticks.action_time = strToTime(ay[1].c_str(), true) * 1000;
			ticks.price = strtod(ay[2].c_str(), NULL);
			ticks.volume = strtod(ay[3].c_str(), NULL);
			tickList._items.emplace_back(ticks);

			if (tickList._items.size() % 1000 == 0)
			{
				WTSLogger::info_f("{} items of data loaded", tickList._items.size());
			}
		}
		tickList._count = tickList._items.size();
		ifs.close();
		WTSLogger::info_f("Data file {} all loaded, totally {} items", csvfile.c_str(), tickList._items.size());

		/*
		 *	By Wesley @ 2021.12.14
		 *	这一段之前有bug，之前没有把文件头写到文件里，所以转储的dsb解析的时候会抛出异常
		 */
		std::string content;
		content.resize(sizeof(HisTickBlockV2));
		HisTickBlockV2 *pBlk = (HisTickBlockV2*)content.data();
		strcpy(pBlk->_blk_flag, BLK_FLAG);
		pBlk->_type = BT_HIS_Ticks;
		pBlk->_version = BLOCK_VERSION_CMP_V2;

		std::string cmpData = WTSCmpHelper::compress_data(tickList._items.data(), sizeof(WTSTickStruct)*tickList._count);
		pBlk->_size = cmpData.size();
		content.append(cmpData);

		StdFile::write_file_content(filename.c_str(), content.c_str(), content.size());
		WTSLogger::info_f("Ticks transfered to file {}", filename.c_str());
	}

	return true;
}

bool HisDataReplayer::cacheFinalBarsFromLoader(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed /* = true */)
{
	if (NULL == _bt_loader)
		return false;

	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _bd_mgr.getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	std::string pname;
	std::string dirname;
	switch (period)
	{
	case KP_Minute1: pname = "m1"; dirname = "min1"; break;
	case KP_Minute5: pname = "m5"; dirname = "min5"; break;
	case KP_DAY: pname = "d"; dirname = "day"; break;
	default: pname = ""; break;
	}

	bool isDay = (period == KP_DAY);

	std::stringstream ss;
	ss << _base_dir << "his/" << dirname << "/" << cInfo._exchg << "/";
	if (!StdFile::exists(ss.str().c_str()))
        boost::filesystem::create_directories(ss.str().c_str());

	if (cInfo.isHot() && commInfo->isFuture())
	{
		ss << cInfo._exchg << "." << cInfo._product << "_HOT.dsb";
	}
	else if (cInfo.isSecond() && commInfo->isFuture())
	{
		ss << cInfo._exchg << "." << cInfo._product << "_2ND.dsb";
	}
	else if (cInfo.isExright() && commInfo->isStock())
	{

	}
	else
		ss << cInfo._code << ".dsb";
	std::string filename = ss.str();

	bool bHit = false;
	if(_bt_loader->isAutoTrans() && StdFile::exists(filename.c_str()))
	{
		//如果支持自动转储，则先读取已经转储的dsb文件
		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisKlineBlockV2))
		{
			WTSLogger::error_f("Sizechecking of back kbar data file {} failed", filename.c_str());
		}
		else
		{
			HisKlineBlockV2* kBlock = (HisKlineBlockV2*)content.c_str();
			std::string rawData = WTSCmpHelper::uncompress_data(kBlock->_data, (uint32_t)kBlock->_size);
			uint32_t barcnt = rawData.size() / sizeof(WTSBarStruct);

			if (bSubbed)
				_bars_cache[key].reset(new BarsList);
			else
				_unbars_cache[key].reset(new BarsList);

			BarsListPtr& barsList = bSubbed ? _bars_cache[key] : _unbars_cache[key];
			barsList->_bars.resize(barcnt);
			memcpy(barsList->_bars.data(), rawData.data(), rawData.size());
			barsList->_cursor = UINT_MAX;
			barsList->_code = stdCode;
			barsList->_period = period;
			barsList->_count = barcnt;

			uint64_t stime = isDay ? barsList->_bars[0].date : barsList->_bars[0].time;
			uint64_t etime = isDay ? barsList->_bars[barcnt - 1].date : barsList->_bars[barcnt - 1].time;

			WTSLogger::info_f("{} items of back {} data of {} directly loaded from dsb file, from {} to {}", barcnt, pname.c_str(), stdCode, stime, etime);
			bHit = true;
		}
	}

	if(!bHit)
	{
		//如果没有转储的历史数据文件, 则从csv加载
		WTSLogger::log_raw(LL_INFO, "Reading data via extended loader...");

		if (bSubbed)
			_bars_cache[key].reset(new BarsList);
		else
			_unbars_cache[key].reset(new BarsList);

		BarsListPtr& barsList = bSubbed ? _bars_cache[key] : _unbars_cache[key];
		barsList->_code = stdCode;
		barsList->_period = period;
		barsList->_cursor = UINT_MAX;
		barsList->_count = 0;

		std::string buffer;
		bool bSucc = _bt_loader->loadFinalHisBars(barsList.get(), stdCode, period, [](void* obj, WTSBarStruct* firstBar, uint32_t count) {
			BarsList* bars = (BarsList*)obj;
			bars->_count = count;
			bars->_bars.resize(count);
			memcpy((void*)bars->_bars.data(), firstBar, sizeof(WTSBarStruct)*count);
		});

		if (!bSucc)
			return false;

		bool isDay = (period == KP_DAY);

		uint64_t stime = isDay ? barsList->_bars[0].date : barsList->_bars[0].time;
		uint64_t etime = isDay ? barsList->_bars[barsList->_count - 1].date : barsList->_bars[barsList->_count - 1].time;

		WTSLogger::info_f("{} items of back {} data of {} loaded via extended loader, from {} to {}", barsList->_count, pname.c_str(), stdCode, stime, etime);

		if(_bt_loader->isAutoTrans())
		{
			BlockType btype;
			switch (period)
			{
			case KP_Minute1: btype = BT_HIS_Minute1; break;
			case KP_Minute5: btype = BT_HIS_Minute5; break;
			default: btype = BT_HIS_Day; break;
			}

			/*
			 *	By Wesley @ 2021.12.14
			 *	这一段之前有bug，之前没有把文件头写到文件里，所以转储的dsb解析的时候会抛出异常
			 */
			std::string content;
			content.resize(sizeof(HisKlineBlockV2));
			HisKlineBlockV2 *kBlock = (HisKlineBlockV2*)content.data();
			strcpy(kBlock->_blk_flag, BLK_FLAG);
			kBlock->_type = btype;
			kBlock->_version = BLOCK_VERSION_CMP_V2;

			std::string cmpData = WTSCmpHelper::compress_data(barsList->_bars.data(), sizeof(WTSBarStruct)*barsList->_count);
			kBlock->_size = cmpData.size();
			content.append(cmpData);

			StdFile::write_file_content(filename.c_str(), content.c_str(), content.size());
			WTSLogger::info_f("Bars transfered to file {}", filename);
		}
	}

	return true;
}

bool HisDataReplayer::cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed/* = true*/)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _bd_mgr.getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	std::string p_suffix;
	std::string dirname = PERIOD_NAME[period];
	switch (period)
	{
	case KP_Minute1: p_suffix = "m1"; break;
	case KP_Minute5: p_suffix = "m5"; break;
	case KP_DAY: p_suffix = "d"; break;
	default: p_suffix = ""; break;
	}

	bool isDay = (period == KP_DAY);

	std::stringstream ss;
	ss << _base_dir << "his/" << dirname << "/" << cInfo._exchg << "/";

	//这里自动创建，是因为后面转储需要
	if (!StdFile::exists(ss.str().c_str()))
		boost::filesystem::create_directories(ss.str().c_str());

	if(cInfo.isHot() && commInfo->isFuture())
	{
		ss << cInfo._exchg << "." << cInfo._product << "_HOT.dsb";
	}
	else if (cInfo.isSecond() && commInfo->isFuture())
	{
		ss << cInfo._exchg << "." << cInfo._product << "_2ND.dsb";
	}
	else if (cInfo.isExright() && commInfo->isStock())
	{

	}
	else
		ss << cInfo._code << ".dsb";
	std::string filename = ss.str();
	if (StdFile::exists(filename.c_str()))
	{
		//如果有格式化的历史数据文件, 则直接读取
		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisKlineBlockV2))
		{
			WTSLogger::error_f("Sizechecking of back kbar data file {} failed", filename);
			return false;
		}

		//By Wesley @ 2021.12.30
		//转储的数据不做检查，直接重新生成即可
		proc_block_data(filename.c_str(), content, true, false);
		uint32_t barcnt = content.size() / sizeof(WTSBarStruct);

		if (bSubbed)
			_bars_cache[key].reset(new BarsList);
		else
			_unbars_cache[key].reset(new BarsList);

		BarsListPtr& barsList = bSubbed ? _bars_cache[key] : _unbars_cache[key];
		barsList->_bars.resize(barcnt);
		memcpy(barsList->_bars.data(), content.data(), content.size());
		barsList->_cursor = UINT_MAX;
		barsList->_code = stdCode;
		barsList->_period = period;
		barsList->_count = barcnt;

		uint64_t stime = isDay ? barsList->_bars[0].date : barsList->_bars[0].time;
		uint64_t etime = isDay ? barsList->_bars[barcnt-1].date : barsList->_bars[barcnt-1].time;

		WTSLogger::info_f("{} items of back {} data of {} directly loaded from dsb file, from {} to {}", barcnt, p_suffix.c_str(), stdCode, stime, etime);
	}
	else
	{
		//如果没有格式化的历史数据文件, 则从csv加载
		std::stringstream ss;
		ss << _base_dir << "csv/" << stdCode << "_" << p_suffix << ".csv";
		std::string csvfile = ss.str();

		if (!StdFile::exists(csvfile.c_str()))
		{
			WTSLogger::error_f("Back kbar data file {} not exists", csvfile);
			return false;
		}

		CsvReader reader;
		reader.load_from_file(csvfile.c_str());

		WTSLogger::info_f("Reading data from {}, with fields: {}...", csvfile, reader.fields());

		if (bSubbed)
			_bars_cache[key].reset(new BarsList);
		else
			_unbars_cache[key].reset(new BarsList);

		BarsListPtr& barsList = bSubbed ? _bars_cache[key] : _unbars_cache[key];
		barsList->_code = stdCode;
		barsList->_period = period;
		while (reader.next_row())
		{
			//逐行读取
			WTSBarStruct bs;
			bs.date = strToDate(reader.get_string("date"));
			if (period != KP_DAY)
				bs.time = TimeUtils::timeToMinBar(bs.date, strToTime(reader.get_string("time")));
			bs.open = reader.get_double("open");
			bs.high = reader.get_double("high");
			bs.low = reader.get_double("low");
			bs.close = reader.get_double("close");
			bs.vol = reader.get_uint32("volume");
			bs.money = reader.get_double("turnover");
			bs.hold = reader.get_uint32("open_interest");
			bs.add = reader.get_int32("diff_interest");
			bs.settle = reader.get_double("settle");
			barsList->_bars.emplace_back(bs);

			if (barsList->_bars.size() % 1000 == 0)
			{
				WTSLogger::info_f("{} lines of data loaded", barsList->_bars.size());
			}
		}
		barsList->_count = barsList->_bars.size();

		uint64_t stime = isDay ? barsList->_bars[0].date : barsList->_bars[0].time;
		uint64_t etime = isDay ? barsList->_bars[barsList->_count - 1].date : barsList->_bars[barsList->_count - 1].time;

		WTSLogger::info_f("Data file {} all loaded, totally {} items, from {} to {}", csvfile.c_str(), barsList->_bars.size(), stime, etime);

		BlockType btype;
		switch (period)
		{
		case KP_Minute1: btype = BT_HIS_Minute1; break;
		case KP_Minute5: btype = BT_HIS_Minute5; break;
		default: btype = BT_HIS_Day; break;
		}

		/*
		 *	By Wesley @ 2021.12.14
		 *	这一段之前有bug，之前没有把文件头写到文件里，所以转储的dsb解析的时候会抛出异常
		 */
		std::string content;
		content.resize(sizeof(HisKlineBlockV2));
		HisKlineBlockV2 *kBlock = (HisKlineBlockV2*)content.data();
		strcpy(kBlock->_blk_flag, BLK_FLAG);
		kBlock->_type = btype;
		kBlock->_version = BLOCK_VERSION_CMP_V2;

		std::string cmpData = WTSCmpHelper::compress_data(barsList->_bars.data(), sizeof(WTSBarStruct)*barsList->_count);
		kBlock->_size = cmpData.size();
		content.append(cmpData);

		StdFile::write_file_content(filename.c_str(), content.c_str(), content.size());
		WTSLogger::info_f("Bars transfered to file {}", filename);
	}

	return true;
}

bool HisDataReplayer::cacheIntegratedFutBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed /* = true */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _bd_mgr.calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "min1"; break;
	case KP_Minute5: pname = "min5"; break;
	default: pname = "day"; break;
	}

	_bars_cache[key].reset(new BarsList());
	BarsListPtr& barsList = _bars_cache[key];
	barsList->_code = stdCode;
	barsList->_period = period;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;
	bool isDay = (period == KP_DAY);

	const char* hot_flag = cInfo.isHot() ? FILE_SUF_HOT : FILE_SUF_2ND;

	std::vector<WTSBarStruct>* hotAy = NULL;
	uint64_t lastHotTime = 0;
	do
	{
		/*
		 *	By Wesley @ 2021.12.20
		 *	本来这里是要先调用_loader->loadRawHisBars从外部加载器读取主力合约数据的
		 *	但是上层会调用一次loadFinalHisBars，这里再调用loadRawHisBars就冗余了，所以直接跳过
		 *
		 *	@ 2022.01.11
		 *	将直接从文件读取，改成从HisDtMgr读取
		 */
		std::string content;
		std::string wrappCode = StrUtil::printf("%s.%s%s", cInfo._exchg, cInfo._product, hot_flag);
		bool bSucc = _his_dt_mgr.load_raw_bars(cInfo._exchg, wrappCode.c_str(), period, [&content](std::string& data) {
			content.swap(data);
		});

		if(!bSucc)
		{
			WTSLogger::error_f("Loading {} bars of {} via HisDtMgr failed", PERIOD_NAME[period], stdCode);
			break;
		}

		uint32_t barcnt = content.size() / sizeof(WTSBarStruct);
		if (barcnt <= 0)
			break;

		hotAy = new std::vector<WTSBarStruct>();
		hotAy->resize(barcnt);
		memcpy(hotAy->data(), content.data(), content.size());

		if (period != KP_DAY)
			lastHotTime = hotAy->at(barcnt - 1).time;
		else
			lastHotTime = hotAy->at(barcnt - 1).date;

		uint64_t stime = isDay ? barsList->_bars[0].date : barsList->_bars[0].time;
		uint64_t etime = isDay ? barsList->_bars[barcnt - 1].date : barsList->_bars[barcnt - 1].time;

		WTSLogger::info_f("{} items of back {} data of hot contract {} directly loaded, from {} to {}", barcnt, pname.c_str(), stdCode, stime, etime);

	} while (false);

	HotSections secs;
	if (cInfo.isHot())
	{
		if (!_hot_mgr.splitHotSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
			return false;
	}
	else if (cInfo.isSecond())
	{
		if (!_hot_mgr.splitSecondSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
			return false;
	}

	if (secs.empty())
		return false;

	bool bAllCovered = false;
	for (auto it = secs.rbegin(); it != secs.rend(); it++)
	{
		const HotSection& hotSec = *it;
		const char* curCode = hotSec._code.c_str();
		uint32_t rightDt = hotSec._e_date;
		uint32_t leftDt = hotSec._s_date;

		//要先将日期转换为边界时间
		WTSBarStruct sBar, eBar;
		if (period != KP_DAY)
		{
			uint64_t sTime = _bd_mgr.getBoundaryTime(stdPID.c_str(), leftDt, false, true);
			uint64_t eTime = _bd_mgr.getBoundaryTime(stdPID.c_str(), rightDt, false, false);

			sBar.date = leftDt;
			sBar.time = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

			if (sBar.time < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
			{
				bAllCovered = true;
				sBar.time = lastHotTime + 1;
			}

			eBar.date = rightDt;
			eBar.time = ((uint32_t)(eTime / 10000) - 19900000) * 10000 + (uint32_t)(eTime % 10000);

			if (eBar.time <= lastHotTime)	//右边界时间小于最后一条Hot时间, 说明全部交叉了, 没有再找的必要了
				break;
		}
		else
		{
			sBar.date = leftDt;
			if (sBar.date < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
			{
				bAllCovered = true;
				sBar.date = (uint32_t)lastHotTime + 1;
			}

			eBar.date = rightDt;

			if (eBar.date <= lastHotTime)
				break;
		}

		/*
		 *	By Wesley @ 2021.12.20
		 *	先从extloader读取分月合约的K线数据
		 *	如果没有读到，再从文件读取
		 */
		bool bLoaded = false;
		std::string buffer;
		if (NULL != _bt_loader)
		{
			//分月合约代码
			std::string wCode = StrUtil::printf("%s.%s.%s", cInfo._exchg, cInfo._product, (char*)curCode + strlen(cInfo._product));
			bLoaded = _bt_loader->loadRawHisBars(&buffer, wCode.c_str(), period, [](void* obj, WTSBarStruct* bars, uint32_t count) {
				std::string* buff = (std::string*)obj;
				buff->resize(sizeof(WTSBarStruct)*count);
				memcpy((void*)buff->c_str(), bars, sizeof(WTSBarStruct)*count);
			});

		}

		if (!bLoaded)
		{
			bLoaded = _his_dt_mgr.load_raw_bars(cInfo._exchg, curCode, period, [&buffer](std::string& data) {
				buffer.swap(data);
			});

			if (!bLoaded)
			{
				WTSLogger::error_f("Loading {} bars of {} via HisDtMgr failed", PERIOD_NAME[period], curCode);
				break;
			}
		}

		if (buffer.empty())
			break;

		uint32_t barcnt = buffer.size() / sizeof(WTSBarStruct);

		WTSBarStruct* firstBar = (WTSBarStruct*)buffer.data();

		WTSBarStruct* pBar = std::lower_bound(firstBar, firstBar + (barcnt - 1), sBar, [period](const WTSBarStruct& a, const WTSBarStruct& b) {
			if (period == KP_DAY)
			{
				return a.date < b.date;
			}
			else
			{
				return a.time < b.time;
			}
		});

		uint32_t sIdx = pBar - firstBar;
		if ((period == KP_DAY && pBar->date < sBar.date) || (period != KP_DAY && pBar->time < sBar.time))	//早于边界时间
		{
			//早于边界时间, 说明没有数据了, 因为lower_bound会返回大于等于目标位置的数据
			continue;
		}

		pBar = std::lower_bound(firstBar + sIdx, firstBar + (barcnt - 1), eBar, [period](const WTSBarStruct& a, const WTSBarStruct& b) {
			if (period == KP_DAY)
			{
				return a.date < b.date;
			}
			else
			{
				return a.time < b.time;
			}
		});
		uint32_t eIdx = pBar - firstBar;
		if ((period == KP_DAY && pBar->date > eBar.date) || (period != KP_DAY && pBar->time > eBar.time))
		{
			pBar--;
			eIdx--;
		}

		if (eIdx < sIdx)
			continue;

		uint32_t curCnt = eIdx - sIdx + 1;
		std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
		tempAy->resize(curCnt);
		memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
		realCnt += curCnt;

		barsSections.emplace_back(tempAy);

		if (bAllCovered)
			break;
	}

	if (hotAy)
	{
		barsSections.emplace_back(hotAy);
		realCnt += hotAy->size();
	}

	if (realCnt > 0)
	{
		barsList->_bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barsList->_bars.data() + curIdx, tempAy->data(), tempAy->size() * sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barsSections.clear();
	}

	WTSLogger::info_f("{} items of back {} data of {} cached", realCnt, pname, stdCode);

	return true;
}

const HisDataReplayer::AdjFactorList& HisDataReplayer::getAdjFactors(const char* code, const char* exchg, const char* pid /* = "" */)
{
	char key[20] = { 0 };
	sprintf(key, "%s.%s.%s", exchg, pid, code);

	auto it = _adj_factors.find(key);
	if (it == _adj_factors.end())
	{
		//By Wesley @ 2021.12.21
		//如果没有复权因子，就从extloader按需读一次
		if (_bt_loader)
		{
            WTSLogger::info_f("No adjusting factors of {} cached, searching via extented loader...", key);
			_bt_loader->loadAdjFactors(this, key, [](void* obj, const char* stdCode, uint32_t* dates, double* factors, uint32_t count) {
				HisDataReplayer* self = (HisDataReplayer*)obj;
				AdjFactorList& fctrLst = self->_adj_factors[stdCode];

				for (uint32_t i = 0; i < count; i++)
				{
					AdjFactor adjFact;
					adjFact._date = dates[i];
					adjFact._factor = factors[i];

					fctrLst.emplace_back(adjFact);
				}

				//一定要把第一条加进去，不然如果是前复权的话，可能会漏处理最早的数据
				AdjFactor adjFact;
				adjFact._date = 19900101;
				adjFact._factor = 1;
				fctrLst.emplace_back(adjFact);

				std::sort(fctrLst.begin(), fctrLst.end(), [](const AdjFactor& left, const AdjFactor& right) {
					return left._date < right._date;
				});

                WTSLogger::info_f("{} items of adjusting factors of {} loaded via extended loader", count, stdCode);
			});
		}
	}

	return _adj_factors[key];
}

bool HisDataReplayer::cacheAdjustedStkBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed /* = true */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _bd_mgr.calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	_bars_cache[key].reset(new BarsList());
	BarsListPtr& barsList = _bars_cache[key];
	barsList->_code = stdCode;
	barsList->_period = period;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;

	std::vector<WTSBarStruct>* adjustedBars = NULL;
	uint64_t lastQTime = 0;

	WTSLogger::info_f("Loading adjusted bars of {}...", stdCode);
	do
	{
		//先直接读取复权过的历史数据,路径如/his/day/sse/SH600000Q.dsb

		/*
		 *	By Wesley @ 2021.12.20
		 *	本来这里是要先调用_loader->loadRawHisBars从外部加载器读取主力合约数据的
		 *	但是上层会调用一次loadFinalHisBars，这里再调用loadRawHisBars就冗余了，所以直接跳过
		 *	
		 *	@ 2022.01.11
		 *	这里将文件读取改为从HisDtMgr封装的接口读取
		 */
		std::string wrappCode = fmt::format("{}{}", cInfo._code, (cInfo._exright == 1 ? SUFFIX_QFQ : SUFFIX_HFQ));
		std::string content;
		bool bSucc = _his_dt_mgr.load_raw_bars(cInfo._exchg, wrappCode.c_str(), period, [&content](std::string& data) {
			content.swap(data);
		});

		if(!bSucc)
		{
			WTSLogger::error_f("Loading {} bars of {} via HisDtMgr failed", PERIOD_NAME[period], stdCode);
			break;
		}

		uint32_t barcnt = content.size() / sizeof(WTSBarStruct);
		if (barcnt <= 0)
			break;

		adjustedBars = new std::vector<WTSBarStruct>();
		adjustedBars->resize(barcnt);
		memcpy(adjustedBars->data(), content.data(), content.size());

		if (period != KP_DAY)
			lastQTime = adjustedBars->at(barcnt - 1).time;
		else
			lastQTime = adjustedBars->at(barcnt - 1).date;

		WTSLogger::info_f("{} items of adjusted back {} data of {} directly loaded", barcnt, PERIOD_NAME[period], stdCode);
	} while (false);

	WTSLogger::info_f("Loading raw bars of {}...", stdCode);
	do
	{
		const char* curCode = cInfo._code;

		//要先将日期转换为边界时间
		WTSBarStruct sBar;
		if (period != KP_DAY)
		{
			sBar.date = TimeUtils::minBarToDate(lastQTime);

			sBar.time = lastQTime + 1;
		}
		else
		{
			sBar.date = (uint32_t)lastQTime + 1;
		}

		/*
		 *	By Wesley @ 2021.12.20
		 *	先从extloader读取未复权K线数据
		 *	如果没有读到，再从文件读取
		 */
		bool bLoaded = false;
		std::string buffer;
		if (NULL != _bt_loader)
		{
			std::string wCode = StrUtil::printf("%s.%s.%s", cInfo._exchg, cInfo._product, curCode);
			bLoaded = _bt_loader->loadRawHisBars(&buffer, wCode.c_str(), period, [](void* obj, WTSBarStruct* bars, uint32_t count) {
				std::string* buff = (std::string*)obj;
				buff->resize(sizeof(WTSBarStruct)*count);
				memcpy((void*)buff->c_str(), bars, sizeof(WTSBarStruct)*count);
			});

			if (bLoaded)
				WTSLogger::debug_f("Raw bars of {} loaded via extended loader", stdCode);
		}

		if (!bLoaded)
		{
			/*
			 *	By Wesley @ 2022.01.11
			 *	这里将文件读取改为从HisDtMgr封装的接口读取
			 */
			bLoaded = _his_dt_mgr.load_raw_bars(cInfo._exchg, curCode, period, [&buffer](std::string& data) {
				buffer.swap(data);
			});

			if (!bLoaded)
			{
				WTSLogger::warn_f("Loading {} bars of {} via HisDtMgr failed", PERIOD_NAME[period], curCode);
				continue;
			}
		}

		if (buffer.empty())
			break;
		
		uint32_t barcnt = buffer.size() / sizeof(WTSBarStruct);

		WTSBarStruct* firstBar = (WTSBarStruct*)buffer.data();

		WTSBarStruct* pBar = std::lower_bound(firstBar, firstBar + (barcnt - 1), sBar, [period](const WTSBarStruct& a, const WTSBarStruct& b) {
			if (period == KP_DAY)
			{
				return a.date < b.date;
			}
			else
			{
				return a.time < b.time;
			}
		});

		if (pBar != NULL)
		{
			uint32_t sIdx = pBar - firstBar;
			uint32_t curCnt = barcnt - sIdx;
			std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
			tempAy->resize(curCnt);
			memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
			realCnt += curCnt;

			auto& ayFactors = getAdjFactors(cInfo._code, cInfo._exchg, cInfo._product);
			if (!ayFactors.empty())
			{
				WTSLogger::info_f("Adjusting bars of {} with adjusting factors...", stdCode);
				//做复权处理
				int32_t lastIdx = curCnt;
				WTSBarStruct bar;
				firstBar = tempAy->data();

				//根据复权类型确定基础因子
				//如果是前复权，则历史数据会变小，以最后一个复权因子为基础因子
				//如果是后复权，则新数据会变大，基础因子为1
				double baseFactor = 1.0;
				if (cInfo._exright == 1)
					baseFactor = ayFactors.back()._factor;
				else if (cInfo._exright == 2)
					barsList->_factor = ayFactors.back()._factor;

				for (auto it = ayFactors.rbegin(); it != ayFactors.rend(); it++)
				{
					const AdjFactor& adjFact = *it;
					bar.date = adjFact._date;

					//调整因子
					double factor = adjFact._factor / baseFactor;

					WTSBarStruct* pBar = NULL;
					pBar = std::lower_bound(firstBar, firstBar + lastIdx - 1, bar, [period](const WTSBarStruct& a, const WTSBarStruct& b) {
						return a.date < b.date;
					});

					if (pBar->date < bar.date)
						continue;

					WTSBarStruct* endBar = pBar;
					if (pBar != NULL)
					{
						int32_t curIdx = pBar - firstBar;
						while (pBar && curIdx < lastIdx)
						{
							pBar->open *= factor;
							pBar->high *= factor;
							pBar->low *= factor;
							pBar->close *= factor;

							pBar++;
							curIdx++;
						}
						lastIdx = endBar - firstBar;
					}

					if (lastIdx == 0)
						break;
				}
			}
			else
			{
				WTSLogger::info_f("No adjusting factors of {} found, ajusting task skipped...", stdCode);
			}

			barsSections.emplace_back(tempAy);
		}
	} while (false);

	if (adjustedBars)
	{
		barsSections.emplace_back(adjustedBars);
		realCnt += adjustedBars->size();
	}

	if (realCnt > 0)
	{
		barsList->_bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barsList->_bars.data() + curIdx, tempAy->data(), tempAy->size() * sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barsSections.clear();
	}

	WTSLogger::info_f("{} items of back {} data of {} cached", realCnt, PERIOD_NAME[period], stdCode);

	return true;
}


bool HisDataReplayer::cacheRawBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed/* = true*/)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _bd_mgr.getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _bd_mgr.calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	bool isDay = (period == KP_DAY);

	if (bSubbed)
		_bars_cache[key].reset(new BarsList);
	else
		_unbars_cache[key].reset(new BarsList);

	BarsListPtr& barsList = bSubbed ? _bars_cache[key] : _unbars_cache[key];
	barsList->_code = stdCode;
	barsList->_period = period;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;
	if (!cInfo.isFlat() && commInfo->isFuture())//如果是读取期货主力连续数据
	{
		return cacheIntegratedFutBarsFromBin(key, stdCode, period);
	}
	else if (cInfo.isExright() && commInfo->isStock())//如果是读取股票复权数据
	{
		return cacheAdjustedStkBarsFromBin(key, stdCode, period, bSubbed);
	}
	

	/*
	 *	By Wesley @ 2021.12.20
	 *	先从extloader读取
	 *	如果没有读到，再从文件读取
	 */
	bool bLoaded = false;
	std::string buffer;
	if (NULL != _bt_loader)
	{
		bLoaded = _bt_loader->loadRawHisBars(&buffer, stdCode, period, [](void* obj, WTSBarStruct* bars, uint32_t count) {
			std::string* buff = (std::string*)obj;
			buff->resize(sizeof(WTSBarStruct)*count);
			memcpy((void*)buff->c_str(), bars, sizeof(WTSBarStruct)*count);
		});
	}

	if(!bLoaded)
	{
		//读取历史的
		//std::stringstream ss;
		//ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._code << ".dsb";
		//std::string filename = ss.str();
		//if (StdFile::exists(filename.c_str()))
		//{
		//	//如果有格式化的历史数据文件, 则直接读取
		//	std::string content;
		//	StdFile::read_file_content(filename.c_str(), content);
		//	if (content.size() < sizeof(HisKlineBlock))
		//	{
		//		WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
		//		return false;
		//	}

		//	proc_block_data(filename.c_str(), content, true, false);
		//	buffer.swap(content);
		//}
		bLoaded = _his_dt_mgr.load_raw_bars(cInfo._exchg, cInfo._code, period, [&buffer](std::string& data) {
			buffer.swap(data);
		});

		if(!bLoaded)
		{
			WTSLogger::warn_f("Loading {} bars of {} via HisDtMgr failed", PERIOD_NAME[period], stdCode);
		}
	}

	if (buffer.empty())
		return false;

	uint32_t barcnt = buffer.size() / sizeof(WTSBarStruct);

	WTSBarStruct* firstBar = (WTSBarStruct*)buffer.data();
	if (barcnt > 0)
	{

		uint32_t sIdx = 0;
		uint32_t idx = barcnt - 1;
		uint32_t curCnt = (idx - sIdx + 1);

		std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
		tempAy->resize(curCnt);
		memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
		realCnt += curCnt;

		barsSections.emplace_back(tempAy);
	}

	if (realCnt > 0)
	{
		barsList->_bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barsList->_bars.data() + curIdx, tempAy->data(), tempAy->size()*sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barsList->_count = barsList->_bars.size();
		barsSections.clear();
	}

	WTSLogger::info_f("{} items of back {} data of {} cached", realCnt, PERIOD_NAME[period], stdCode);
	return true;
}