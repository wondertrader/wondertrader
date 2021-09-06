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
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"

#include "../Share/BoostFile.hpp"
#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"
#include "../WTSTools/WTSCmpHelper.hpp"

#include "../Share/JsonToVariant.hpp"
#include "../Share/CodeHelper.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#ifdef _WIN32
#pragma comment(lib, "libmysql.lib")
#endif

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif

uint64_t readFileContent(const char* filename, std::string& content)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	uint32_t length = ftell(f);
	content.resize(length);   // allocate memory for a buffer of appropriate dimension
	fseek(f, 0, 0);
	fread((void*)content.data(), sizeof(char), length, f);
	fclose(f);
	return length;
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
{
}


HisDataReplayer::~HisDataReplayer()
{
}


bool HisDataReplayer::init(WTSVariant* cfg, EventNotifier* notifier /* = NULL */)
{
	_notifier = notifier;

	_mode = cfg->getCString("mode");
	_base_dir = StrUtil::standardisePath(cfg->getCString("path"));

	_begin_time = cfg->getUInt64("stime");
	_end_time = cfg->getUInt64("etime");

	_tick_enabled = cfg->getBoolean("tick");

	//基础数据文件
	WTSVariant* cfgBF = cfg->get("basefiles");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"));

	if (cfgBF->get("commodity"))
		_bd_mgr.loadCommodities(cfgBF->getCString("commodity"));

	if (cfgBF->get("contract"))
		_bd_mgr.loadContracts(cfgBF->getCString("contract"));

	if (cfgBF->get("holiday"))
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));

	if (cfgBF->get("hot"))
		_hot_mgr.loadHots(cfgBF->getCString("hot"));

	if (cfgBF->get("second"))
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));

	loadFees(cfg->getCString("fees"));

	if(_mode.compare("db") == 0)
	{
		WTSVariant* dbConf = cfg->get("db");
		if (dbConf)
		{
			strcpy(_db_conf._host, dbConf->getCString("host"));
			strcpy(_db_conf._dbname, dbConf->getCString("dbname"));
			strcpy(_db_conf._user, dbConf->getCString("user"));
			strcpy(_db_conf._pass, dbConf->getCString("pass"));
			_db_conf._port = dbConf->getInt32("port");

			_db_conf._active = (strlen(_db_conf._host) > 0) && (strlen(_db_conf._dbname) > 0) && (_db_conf._port != 0);
			if (_db_conf._active)
				initDB();
		}
	}

	bool bAdjLoaded = false;
	if (_db_conn)
		bAdjLoaded = loadStkAdjFactorsFromDB();

	if (!bAdjLoaded && cfg->has("adjfactor"))
		loadStkAdjFactors(cfg->getCString("adjfactor"));

	return true;
}

void HisDataReplayer::initDB()
{
	if (!_db_conf._active)
		return;

	_db_conn.reset(new MysqlDb);
	my_bool autoreconnect = true;
	_db_conn->options(MYSQL_OPT_RECONNECT, &autoreconnect);
	_db_conn->options(MYSQL_SET_CHARSET_NAME, "utf8");

	if (_db_conn->connect(_db_conf._dbname, _db_conf._host, _db_conf._user, _db_conf._pass, _db_conf._port, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS))
	{
		WTSLogger::info("Database connection succeed[%s:%d]", _db_conf._host, _db_conf._port);
	}
	else
	{
		WTSLogger::error("Database connection failed[%s:%d]:%s", _db_conf._host, _db_conf._port, _db_conn->errstr());
		_db_conn.reset();
	}
}

bool HisDataReplayer::loadStkAdjFactorsFromDB()
{
	MysqlQuery query(*_db_conn);
	if (!query.exec("SELECT exchange,code,date,factor FROM tb_adj_factors ORDER BY exchange,code,date DESC;"))
	{
		WTSLogger::error("Error occured while querying adjust factors:%s", query.errormsg());
		return false;
	}

	uint32_t stk_cnt = 0;
	uint32_t fct_cnt = 0;
	while (query.fetch_row())
	{
		const char* exchg = query.getstr(0);
		const char* code = query.getstr(1);
		uint32_t uDate = query.getuint(2);
		double factor = query.getdouble(3);

		std::string key = StrUtil::printf("%s.%s", exchg, code);
		if (_adj_factors.find(key) == _adj_factors.end())
			stk_cnt++;

		AdjFactorList& fctrLst = _adj_factors[key];
		AdjFactor adjFact;
		adjFact._date = uDate;
		adjFact._factor = factor;

		fctrLst.emplace_back(adjFact);
		fct_cnt++;
	}

	for(auto& m : _adj_factors)
	{
		AdjFactorList& fctrLst = (AdjFactorList&)m.second;

		//一定要把第一条加进去，不然如果是前复权的话，可能会漏处理最早的数据
		AdjFactor adjFact;
		adjFact._date = 19900101;
		adjFact._factor = 1;
		fctrLst.emplace_back(adjFact);

		std::sort(fctrLst.begin(), fctrLst.end(), [](AdjFactor& left, AdjFactor& right) {
			return left._date < right._date;
		});
	}

	WTSLogger::info("%u items of adjust factors for %u stocks loaded", fct_cnt, stk_cnt);
	return true;
}

bool HisDataReplayer::loadStkAdjFactors(const char* adjfile)
{
	if (!BoostFile::exists(adjfile))
	{
		WTSLogger::error("Adjust factor file %s not exists", adjfile);
		return false;
	}

	std::string content;
	BoostFile::read_file_contents(adjfile, content);

	rj::Document doc;
	doc.Parse(content.c_str());

	if (doc.HasParseError())
	{
		WTSLogger::error("Parsing adjust factor file %s faield", adjfile);
		return false;
	}

	uint32_t stk_cnt = 0;
	uint32_t fct_cnt = 0;
	for (auto& mExchg : doc.GetObject())
	{
		const char* exchg = mExchg.name.GetString();
		const rj::Value& itemExchg = mExchg.value;
		for (auto& mCode : itemExchg.GetObject())
		{
			const char* code = mCode.name.GetString();
			const rj::Value& ayFacts = mCode.value;
			if (!ayFacts.IsArray())
				continue;

			std::string key = StrUtil::printf("%s.%s", exchg, code);
			stk_cnt++;

			AdjFactorList& fctrLst = _adj_factors[key];
			for (auto& fItem : ayFacts.GetArray())
			{
				AdjFactor adjFact;
				adjFact._date = fItem["date"].GetUint();
				adjFact._factor = fItem["factor"].GetDouble();

				fctrLst.emplace_back(adjFact);
				fct_cnt++;
			}

			//一定要把第一条加进去，不然如果是前复权的话，可能会漏处理最早的数据
			AdjFactor adjFact;
			adjFact._date = 19900101;
			adjFact._factor = 1;
			fctrLst.emplace_back(adjFact);

			std::sort(fctrLst.begin(), fctrLst.end(), [](AdjFactor& left, AdjFactor& right) {
				return left._date < right._date;
			});
		}
	}

	WTSLogger::info("%u items of adjust factors for %u stocks loaded", fct_cnt, stk_cnt);
	return true;
}

void HisDataReplayer::register_task(uint32_t taskid, uint32_t date, uint32_t time, const char* period, const char* trdtpl /* = "CHINA" */, const char* session /* = "TRADING" */)
{
	TaskPeriodType ptype;
	if (my_stricmp(period, "d") == 0)
		ptype = TPT_Daily;
	else if (my_stricmp(period, "w") == 0)
		ptype = TPT_Weekly;
	else if (my_stricmp(period, "m") == 0)
		ptype = TPT_Monthly;
	else if (my_stricmp(period, "y") == 0)
		ptype = TPT_Yearly;
	else if (my_stricmp(period, "min") == 0)
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

	WTSLogger::info("Timed task registration succeed, frequency: %s", period);
}

void HisDataReplayer::clear_cache()
{
	_ticks_cache.clear();
	_orddtl_cache.clear();
	_ordque_cache.clear();
	_trans_cache.clear();

	_bars_cache.clear();

	WTSLogger::warn("All cached data cleared");
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

		WTSLogger::info("Reading flag of %s has been reset", m.first.c_str());
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
	BoostFile::write_file_contents(filename.c_str(), output.c_str(), output.size());
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

	BarsList& barsList = _bars_cache[key];
	bool isDay = (barsList._period == KP_DAY);

	WTSBarStruct bar;
	bar.date = curDate;
	bar.time = (curDate - 19900000) * 10000 + curTime;
	auto it = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), bar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});

	uint32_t idx;
	if (it == barsList._bars.end())
		idx = barsList._bars.size() - 1;
	else
	{
		if(bUpperBound)
		{//如果是找上边界，则要比较时间向下修正，因为lower_bound函数找的是大于等于curTime的K线
			if ((isDay && it->date > bar.date) || (!isDay && it->time > bar.time))
			{
				it--;
			}
		}

		idx = it - barsList._bars.begin();
	}
	
	return idx;
}

void HisDataReplayer::stop()
{
	if(!_running)
	{
		WTSLogger::error("Backtesting is not running, no need to stop");
		return;
	}

	if (_terminated)
		return;

	_terminated = true;
	WTSLogger::error("Terminating flag reset to true, backtesting will quit at next bar");
}

void HisDataReplayer::run(bool bNeedDump/* = false*/)
{
	if(_running)
	{
		WTSLogger::error("Cannot run more than one backtesting task at the same time");
		return;
	}

	_running = true;
	_terminated = false;
	reset();

	int64_t now = TimeUtils::getLocalTimeNano();

	_cur_date = (uint32_t)(_begin_time / 10000);
	_cur_time = (uint32_t)(_begin_time % 10000);
	_cur_secs = 0;
	_cur_tdate = _bd_mgr.calcTradingDate(DEFAULT_SESSIONID, _cur_date, _cur_time, true);

	if (_notifier)
		_notifier->notifyEvent("BT_START");

	_listener->handle_init();	

	if (!_tick_enabled)
		checkUnbars();

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
				const BarsList& barList = m.second;
				if (barList._period < minPeriod)
				{
					minPeriod = barList._period;
					minTimes = barList._times;
					_main_key = m.first;
				}
				else if(barList._period == minPeriod)
				{
					if(barList._times < minTimes)
					{
						_main_key = m.first;
						minTimes = barList._times;
					}
				}
			}

			WTSLogger::info("Main K bars automatic determined: %s", _main_key.c_str());
		}

		if(!_main_key.empty())
		{
			BarsList& barList = _bars_cache[_main_key];
			WTSSessionInfo* sInfo = get_session_info(barList._code.c_str(), true);
			std::string commId = CodeHelper::stdCodeToStdCommID(barList._code.c_str());

			uint32_t sIdx = locate_barindex(_main_key, _begin_time, false);
			uint32_t eIdx = locate_barindex(_main_key, _end_time, true);

			uint32_t total_barcnt = eIdx - sIdx + 1;
			uint32_t replayed_barcnt = 0;

			notify_state(barList._code.c_str(), barList._period, barList._times, _begin_time, _end_time, 0);

			if (bNeedDump)
				dump_btstate(barList._code.c_str(), barList._period, barList._times, _begin_time, _end_time, 100.0, TimeUtils::getLocalTimeNano() - now);

			WTSLogger::log_raw(LL_INFO, fmt::format("Start to replay back data from {}...", _begin_time).c_str());

			for (;!_terminated;)
			{
				bool isDay = barList._period == KP_DAY;
				if (barList._cursor != UINT_MAX)
				{
					uint64_t nextBarTime = 0;
					if (isDay)
						nextBarTime = (uint64_t)barList._bars[barList._cursor].date * 10000 + sInfo->getCloseTime();
					else
					{
						nextBarTime = (uint64_t)barList._bars[barList._cursor].time;
						nextBarTime += 199000000000;
					}

					if (nextBarTime > _end_time)
					{
						WTSLogger::log_raw(LL_INFO, fmt::format("{} is beyond ending time {},replaying done", nextBarTime, _end_time).c_str());
						_listener->handle_replay_done();
						notify_state(barList._code.c_str(), barList._period, barList._times, _begin_time, _end_time, 100);
						if (_notifier)
							_notifier->notifyEvent("BT_END");
						break;
					}

					uint32_t nextDate = (uint32_t)(nextBarTime / 10000);
					uint32_t nextTime = (uint32_t)(nextBarTime % 10000);

					uint32_t nextTDate = _bd_mgr.calcTradingDate(commId.c_str(), nextDate, nextTime, false);
					if (_opened_tdate != nextTDate)
					{
						_listener->handle_session_begin(nextTDate);
						_opened_tdate = nextTDate;
						_cur_tdate = nextTDate;
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

					uint32_t offTime = sInfo->offsetTime(_cur_time);
					bool isEndTDate = (offTime >= sInfo->getCloseTime(true));

					if (!_tick_enabled)
					{
						checkUnbars();
						replayUnbars(curBarTime, nextBarTime, (isDay || isEndTDate) ? nextTDate : 0);
					}

					onMinuteEnd(nextDate, nextTime, (isDay || isEndTDate) ? nextTDate : 0, _tick_simulated);

					replayed_barcnt += 1;

					if (isEndTDate && _closed_tdate != _cur_tdate)
					{
						_listener->handle_session_end(_cur_tdate);
						_closed_tdate = _cur_tdate;
						_day_cache.clear();
					}

					notify_state(barList._code.c_str(), barList._period, barList._times, _begin_time, _end_time, replayed_barcnt*100.0 / total_barcnt);

					if (barList._cursor >= barList._bars.size())
					{
						//WTSLogger::info("全部数据都已回放,回放结束");
						WTSLogger::info("All back data replayed, replaying done");
						_listener->handle_replay_done();
						notify_state(barList._code.c_str(), barList._period, barList._times, _begin_time, _end_time, 100);
						if (_notifier)
							_notifier->notifyEvent("BT_END");
						break;
					}
				}
				else
				{
					//WTSLogger::info("数据尚未初始化,回放直接退出");
					WTSLogger::info("No back data initialized, replaying canceled");
					_listener->handle_replay_done();
					notify_state(barList._code.c_str(), barList._period, barList._times, _begin_time, _end_time, 100);
					if (_notifier)
						_notifier->notifyEvent("BT_END");
					break;
				}
			}

			if (_closed_tdate != _cur_tdate)
			{
				_listener->handle_session_end(_cur_tdate);
			}

			if (bNeedDump)
			{
				dump_btstate(barList._code.c_str(), barList._period, barList._times, _begin_time, _end_time, 100.0, TimeUtils::getLocalTimeNano()-now);
			}
		}
		else if(_tick_enabled)
		{
			uint32_t edt = (uint32_t)(_end_time / 10000);
			uint32_t etime = (uint32_t)(_end_time % 10000);
			uint64_t end_tdate = _bd_mgr.calcTradingDate(DEFAULT_SESSIONID, edt, etime, true);

			while(_cur_tdate <= end_tdate)
			{
				if(checkAllTicks(_cur_tdate))
				{
					WTSLogger::info("Start to replay tick data of %u...", _cur_tdate);
					_listener->handle_session_begin(_cur_tdate);
					replayHftDatasByDay(_cur_tdate);
					_listener->handle_session_end(_cur_tdate);
				}

				_cur_tdate = TimeUtils::getNextDate(_cur_tdate);
			}

			WTSLogger::info("All back data replayed, replaying done");
			_listener->handle_replay_done();
			if (_notifier)
				_notifier->notifyEvent("BT_END");
		}
		else
		{
			//WTSLogger::info("没有订阅主力K线且未开放tick回测,回放直接退出");
			WTSLogger::info("Main K bars not subscribed and backtesting of tick data not available , replaying done");
			_listener->handle_replay_done();
			if (_notifier)
				_notifier->notifyEvent("BT_END");
		}
	}
	else //if(_task != NULL)
	{
		//时间调度任务不为空,则按照时间调度任务回放
		WTSSessionInfo* sInfo = NULL;
		const char* DEF_SESS = (strlen(_task->_session) == 0) ? DEFAULT_SESSIONID : _task->_session;
		sInfo = _bd_mgr.getSession(DEF_SESS);
		WTSLogger::log_raw(LL_INFO, fmt::format("Start to backtest with task frequency from {}...", _begin_time).c_str());

		//分钟即任务和日级别任务分开写
		if (_task->_period != TPT_Minute)
		{
			uint32_t endtime = TimeUtils::getNextMinute(_task->_time, -1);
			bool bIsPreDay = endtime > _task->_time;
			if (bIsPreDay)
				_cur_date = TimeUtils::getNextDate(_cur_date, -1);

			for (;!_terminated;)
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
					bool bEndSession = sInfo->offsetTime(curTime) >= sInfo->getCloseTime(true);
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
					//WTSLogger::info("按任务周期回测结束");
					WTSLogger::info("Backtesting with task frequency is done");
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

			for(;!_terminated;)
			{
				//要考虑到跨日的情况
				uint32_t mins = sInfo->timeToMinutes(_cur_time);
				//如果一开始不能整除,则直接修正一下
				if(mins % _task->_time != 0)
				{
					mins = mins / _task->_time + _task->_time;
					_cur_time = sInfo->minuteToTime(mins);
				}

				bool bNewTDate = false;
				if(mins < sInfo->getTradingMins())
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
		
				
				if(bNewTDate)
				{
					//换日了
					mins = _task->_time;
					uint32_t nextTDate = _bd_mgr.getNextTDate(_task->_trdtpl, _cur_tdate, 1, true);

					if(sInfo->getOffsetMins() != 0)
					{
						if(sInfo->getOffsetMins() > 0)
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
					if(bNewDay)
						_cur_date = TimeUtils::getNextDate(_cur_date);

					uint32_t dayMins = _cur_time / 100 * 60 + _cur_time % 100;
					uint32_t nextDMins = newTime / 100 * 60 + newTime % 100;

					//是否到了一个新的小节
					bool bNewSec = (nextDMins - dayMins > _task->_time) && !bNewDay;

					while(bNewSec && _bd_mgr.isHoliday(_task->_trdtpl, _cur_date, true))
						_cur_date = TimeUtils::getNextDate(_cur_date);

					_cur_time = newTime;
				}
				
				uint64_t nextTime = (uint64_t)_cur_date * 10000 + _cur_time;
				if (nextTime > _end_time)
				{
					//WTSLogger::info("按任务周期回测结束");
					WTSLogger::info("Backtesting with task frequency is done");
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

	_running = false;
}

void HisDataReplayer::replayUnbars(uint64_t stime, uint64_t nowTime, uint32_t endTDate /* = 0 */)
{
	//uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	uint32_t uDate = (uint32_t)(stime / 10000);

	for (auto it = _unbars_cache.begin(); it != _unbars_cache.end(); it++)
	{
		BarsList& barsList = (BarsList&)it->second;
		if (barsList._period != KP_DAY)
		{
			//如果历史数据指标不在尾部, 说明是回测模式, 要继续回放历史数据
			if (barsList._bars.size() > barsList._cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._cursor];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						if(barTime <= stime)
							continue;

						//开高低收
						WTSTickStruct& curTS = _day_cache[barsList._code];
						strcpy(curTS.code, barsList._code.c_str());
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
						_listener->handle_tick(barsList._code.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.high;
						curTS.volume = nextBar.vol;
						curTS.high = max(curTS.price, curTS.high);
						curTS.low = min(curTS.price, curTS.low);
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(barsList._code.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.low;
						curTS.high = max(curTS.price, curTS.high);
						curTS.low = min(curTS.price, curTS.low);
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(barsList._code.c_str(), curTick);
						curTick->release();

						curTS.price = nextBar.close;
						curTS.high = max(curTS.price, curTS.high);
						curTS.low = min(curTS.price, curTS.low);
						curTick = WTSTickData::create(curTS);
						_listener->handle_tick(barsList._code.c_str(), curTick);
						curTick->release();
					}
					else
					{
						break;
					}

					barsList._cursor++;

					if (barsList._cursor == barsList._bars.size())
						break;
				}
			}
		}
		else
		{
			if (barsList._bars.size() > barsList._cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._cursor];

					if (nextBar.date <= endTDate)
					{
						if (nextBar.date <= uDate)
							continue;

						CodeHelper::CodeInfo cInfo;
						CodeHelper::extractStdCode(barsList._code.c_str(), cInfo);

						std::string realCode = barsList._code;
						if (cInfo.isStock() && cInfo.isExright())
						{
							realCode = cInfo._exchg;
							realCode += ".";
							realCode += cInfo._code;
						}

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

					barsList._cursor++;

					if (barsList._cursor >= barsList._bars.size())
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

		auto& tickList = _ticks_cache[stdCode];
		if (tickList._cursor == UINT_MAX)
		{
			if (stime == UINT64_MAX)
				tickList._cursor = 1;
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
	for (;;)
	{
		//先确定下一笔tick的时间
		uint64_t nextTime = min(UINT64_MAX, getNextTickTime(curTDate));
		nextTime = min(nextTime, getNextOrdDtlTime(curTDate));
		nextTime = min(nextTime, getNextOrdQueTime(curTDate));
		nextTime = min(nextTime, getNextTransTime(curTDate));

		if(nextTime == UINT64_MAX)
			break;

		//再根据时间回放tick数据
		_cur_date = (uint32_t)(nextTime / 1000000000);
		_cur_time = nextTime % 1000000000 / 100000;
		_cur_secs = nextTime % 100000;

		//1、首先回放委托明细
		for (auto& v : _orddtl_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _orddtl_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdDtlData* newData = WTSOrdDtlData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_order_detail(stdCode, newData);
				newData->release();

				itemList._cursor++;
				total_ticks++;
			}
		}

		//2、其次再回放成交明细
		for (auto& v : _trans_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _trans_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSTransData* newData = WTSTransData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_transaction(stdCode, newData);
				newData->release();

				itemList._cursor++;
				total_ticks++;
			}
		}

		//3、第三步再回放tick数据
		for (auto& v : _tick_sub_map)
		{
			//std::string stdCode = v.first;
			const char* stdCode = v.first.c_str();
			auto& tickList = _ticks_cache[stdCode];
			WTSTickStruct& nextTick = tickList._items[tickList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;
			if (lastTime <= nextTime)
			{
				update_price(stdCode, nextTick.price);
				WTSTickData* newTick = WTSTickData::create(nextTick);
				newTick->setCode(stdCode);
				_listener->handle_tick(stdCode, newTick);
				newTick->release();

				tickList._cursor++;
				total_ticks++;
			}
		}
		
		//4、最后回放委托队列
		for (auto& v : _ordque_sub_map)
		{
			const char* stdCode = v.first.c_str();
			auto& itemList = _ordque_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdQueData* newData = WTSOrdQueData::create(nextItem);
				newData->setCode(stdCode);
				_listener->handle_order_queue(stdCode, newData);
				newData->release();

				itemList._cursor++;
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

	//if(_task && endTDate != 0)
	//{
	//	//如果是交易日结束,清理掉分钟线缓存,不然吃内存太多
	//	std::set<std::string> to_clear;
	//	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	//	{
	//		const BarsList& barsList = it->second;
	//		if (barsList._period != KP_DAY)
	//			to_clear.insert(it->first);
	//	}

	//	for(const std::string& key : to_clear)
	//	{
	//		auto it = _bars_cache.find(key);
	//		if (it != _bars_cache.end())
	//		{
	//			_bars_cache.erase(it);
	//			WTSLogger::info("Data cache %s cleared", key.c_str());
	//		}
	//	}
	//}

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = (BarsList&)it->second;
		if (barsList._period != KP_DAY)
		{
			//如果历史数据指标不在尾部, 说明是回测模式, 要继续回放历史数据
			if (barsList._bars.size() > barsList._cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._cursor];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						//if (_task == NULL)
						{
							if (tickSimulated)
							{
								const std::string& ticker = _ticker_keys[barsList._code];
								if (ticker == it->first)
								{
									//开高低收
									WTSTickStruct& curTS = _day_cache[barsList._code];
									strcpy(curTS.code, barsList._code.c_str());
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

									update_price(barsList._code.c_str(), curTS.price);
									WTSTickData* curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList._code.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.high;
									curTS.volume = nextBar.vol;
									curTS.high = max(curTS.price, curTS.high);
									curTS.low = min(curTS.price, curTS.low);
									update_price(barsList._code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList._code.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.low;
									curTS.high = max(curTS.price, curTS.high);
									curTS.low = min(curTS.price, curTS.low);
									update_price(barsList._code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList._code.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.close;
									curTS.high = max(curTS.price, curTS.high);
									curTS.low = min(curTS.price, curTS.low);
									update_price(barsList._code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(barsList._code.c_str(), curTick);
								}
							}

							uint32_t times = barsList._times;
							if (barsList._period == KP_Minute5)
								times *= 5;
							_listener->handle_bar_close(barsList._code.c_str(), "m", times, &nextBar);
						}
					}
					else
					{
						break;
					}

					barsList._cursor++;

					if (barsList._cursor == barsList._bars.size())
						break;
				}
			}
		}
		else
		{
			if (barsList._bars.size() > barsList._cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._cursor];

					if (nextBar.date <= endTDate)
					{
						//if (_task == NULL)
						{
							if (tickSimulated)
							{
								const std::string& ticker = _ticker_keys[barsList._code];
								if (ticker == it->first)
								{
									CodeHelper::CodeInfo cInfo;
									CodeHelper::extractStdCode(barsList._code.c_str(), cInfo);

									std::string realCode = barsList._code;
									if (cInfo.isStock() && cInfo._exright)
									{
										realCode = cInfo._exchg;
										realCode += ".";
										realCode += cInfo._code;
									}

									WTSSessionInfo* sInfo = get_session_info(realCode.c_str(), true);
									uint32_t curTime = sInfo->getCloseTime();
									//开高低收
									WTSTickStruct curTS;
									strcpy(curTS.code, realCode.c_str());
									curTS.action_date = _cur_date;
									curTS.action_time = curTime * 100000;

									curTS.price = nextBar.open;
									curTS.volume = nextBar.vol;
									update_price(barsList._code.c_str(), curTS.price);
									WTSTickData* curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.high;
									curTS.volume = nextBar.vol;
									update_price(barsList._code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.low;
									update_price(barsList._code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.close;
									update_price(barsList._code.c_str(), curTS.price);
									curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
								}
							}

							_listener->handle_bar_close(barsList._code.c_str(), "d", barsList._times, &nextBar);
						}
					}
					else
					{
						break;
					}

					barsList._cursor++;

					if (barsList._cursor >= barsList._bars.size())
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
	bool isStk = CodeHelper::isStdStkCode(stdCode);
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
	if (strcmp(period, "m") == 0)
	{
		if(times % 5 == 0)
		{
			kp = KP_Minute5;
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
			std::string rawKey = StrUtil::printf("%s#%s#%u", stdCode, period, 1);
			if (_bars_cache.find(rawKey) == _bars_cache.end())
			{
				if (_mode == "csv")
				{
					bHasHisData = cacheRawBarsFromCSV(rawKey, stdCode, kp);
				}
				else if (_mode == "db")
				{
					bHasHisData = cacheRawBarsFromDB(rawKey, stdCode, kp);
				}
				else
				{
					bHasHisData = cacheRawBarsFromBin(rawKey, stdCode, kp);
				}
			}
			else
			{
				bHasHisData = true;
			}
		}
		else
		{
			if (_mode == "csv")
			{
				bHasHisData = cacheRawBarsFromCSV(key, stdCode, kp);
			}
			else if (_mode == "db")
			{
				bHasHisData = cacheRawBarsFromDB(key, stdCode, kp);
			}
			else
			{
				bHasHisData = cacheRawBarsFromBin(key, stdCode, kp);
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
	bool isClosed = (sInfo->offsetTime(_cur_time) >= sInfo->getCloseTime(true));
	if (realTimes != 1 && !bHasCache)
	{	
		std::string rawKey = StrUtil::printf("%s#%s#%u", stdCode, period, 1);
		BarsList& rawBars = _bars_cache[rawKey];
		WTSKlineSlice* rawKline = WTSKlineSlice::create(stdCode, kp, realTimes, &rawBars._bars[0], rawBars._bars.size());
		rawKline->setCode(stdCode);

		static WTSDataFactory dataFact;
		WTSKlineData* kData = dataFact.extractKlineData(rawKline, kp, realTimes, sInfo, true);
		rawKline->release();

		if(kData)
		{
			BarsList& barsList = _bars_cache[key];
			barsList._code = stdCode;
			barsList._period = kp;
			barsList._times = realTimes;
			barsList._count = kData->size();
			barsList._bars.swap(kData->getDataRef());
			kData->release();
			WTSLogger::info("%u resampled %s%u back kline of %s ready", barsList._bars.size(), period, times, stdCode);
		}
		else
		{
			WTSLogger::error("Resampling %s%u back kline of %s failed", period, times, stdCode);
			return NULL;
		}
	}

	BarsList& kBlkPair = _bars_cache[key];
	if (kBlkPair._cursor == UINT_MAX)
	{
		//还没有经过初始定位
		WTSBarStruct bar;
		bar.date = _cur_tdate;
		if(kp != KP_DAY)
			bar.time = (_cur_date - 19900000) * 10000 + _cur_time;
		
		auto it = std::lower_bound(kBlkPair._bars.begin(), kBlkPair._bars.end(), bar, [isDay, isClosed](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				if (!isClosed)
					return a.date < b.date;
				else 
					return a.date <= b.date;
			else
				return a.time < b.time;
		});

		uint32_t eIdx = it - kBlkPair._bars.begin();

		if (it != kBlkPair._bars.end())
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

			kBlkPair._cursor = eIdx + 1;
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
			if (kBlkPair._cursor <= kBlkPair._count)
			{
				if(!isClosed)
				{
					while (kBlkPair._bars[kBlkPair._cursor - 1].date < _cur_tdate  && kBlkPair._cursor < kBlkPair._count && kBlkPair._bars[kBlkPair._cursor].date < _cur_tdate)
					{
						kBlkPair._cursor++;
					}
				}
				else
				{
					while (kBlkPair._bars[kBlkPair._cursor - 1].date <= _cur_tdate  && kBlkPair._cursor < kBlkPair._count && kBlkPair._bars[kBlkPair._cursor].date <= _cur_tdate)
					{
						kBlkPair._cursor++;
					}
				}
				

				if (kBlkPair._bars[kBlkPair._cursor - 1].date > _cur_tdate)
					kBlkPair._cursor--;
			}
		}
		else
		{
			if (kBlkPair._cursor <= kBlkPair._count)
			{
				while (kBlkPair._bars[kBlkPair._cursor-1].time < curMin && kBlkPair._cursor < kBlkPair._count)
				{
					kBlkPair._cursor++;
				}

				if (kBlkPair._bars[kBlkPair._cursor - 1].time > curMin)
					kBlkPair._cursor--;
			}
		}
	}


	if (kBlkPair._cursor == 0)
		return NULL;

	uint32_t sIdx = 0;
	if (kBlkPair._cursor > count)
		sIdx = kBlkPair._cursor - count;

	uint32_t realCnt = kBlkPair._cursor - sIdx;
	WTSKlineSlice* kline = WTSKlineSlice::create(stdCode, kp, 1, kBlkPair._bars.data() + sIdx, realCnt);
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
		auto& tickList = it->second;
		if (tickList._date != uDate)
			bNeedCache = true;
		else if (tickList._count == 0)
			return false;
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
		//WTSLogger::error("手续费模板文件%s不存在", filename);
		WTSLogger::error("Fees template file %s not exists", filename);
		return;
	}


	std::string content;
	StdFile::read_file_content(filename, content);
	if (content.empty())
	{
		//WTSLogger::error("手续费模板文件%s为空", filename);
		WTSLogger::error("Fees template file %s is empty", filename);
		return;
	}

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
	{
		//WTSLogger::error("手续费模板文件%s解析失败", filename);
		WTSLogger::error("Parsing fees template file %s failed", filename);
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	if (!jsonToVariant(root, cfg))
	{
		//WTSLogger::error("手续费模板文件%s转换失败", filename);
		WTSLogger::error("Converting fees template file %s failed", filename);
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

	//WTSLogger::info("共加载%u条手续费模板", _fee_map.size());
	WTSLogger::info("%u items of fees template loaded", _fee_map.size());
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
		if (_mode == "csv")
		{
			bHasHisData = cacheRawBarsFromCSV(key, stdCode, KP_Minute1, false);
		}
		else if (_mode == "db")
		{
			bHasHisData = cacheRawBarsFromDB(key, stdCode, KP_Minute1, false);
		}
		else
		{
			bHasHisData = cacheRawBarsFromBin(key, stdCode, KP_Minute1, false);
		}
		
		if (!bHasHisData)
			continue;

		WTSSessionInfo* sInfo = get_session_info(stdCode, true);

		BarsList& kBlkPair = _unbars_cache[key];
		
		//还没有经过初始定位
		WTSBarStruct bar;
		bar.date = _cur_tdate;
		bar.time = (_cur_date - 19900000) * 10000 + _cur_time;

		auto it = std::lower_bound(kBlkPair._bars.begin(), kBlkPair._bars.end(), bar, [](const WTSBarStruct& a, const WTSBarStruct& b) {
			return a.time < b.time;
		});

		uint32_t eIdx = it - kBlkPair._bars.begin();

		if (it != kBlkPair._bars.end())
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

			kBlkPair._cursor = eIdx + 1;
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
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
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

	std::string filename;
	bool bHit = false;
	//先检查有没有HOT、SND的主力次主力的tick文件
	if(!cInfo.isFlat())
	{
		const char* hot_flag = cInfo.isFlat() ? "" : (cInfo.isHot() ? "HOT" : "2ND");
		std::stringstream ss;
		ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << cInfo._product << "_" << hot_flag << ".dsb";
		filename = ss.str();
		if (StdFile::exists(filename.c_str()))
			bHit = true;
	}

	//如果没有找到，则读取分月合约
	if (!bHit)
	{
		std::stringstream ss;
		ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << rawCode << ".dsb";
		filename = ss.str();
	}

	if (!StdFile::exists(filename.c_str()))
		return false;

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);
	if (content.size() < sizeof(HisTickBlock))
	{
		WTSLogger::error("Sizechecking of back tick data file %s failed", filename.c_str());
		return false;
	}

	HisTickBlock* tBlock = (HisTickBlock*)content.c_str();
	HisTickBlockV2* tBlockV2 = NULL;
	if(tBlock->_version == BLOCK_VERSION_CMP)
	{
		//压缩版本,要重新检查文件大小
		tBlockV2 = (HisTickBlockV2*)content.c_str();

		if (content.size() != (sizeof(HisTickBlockV2) + tBlockV2->_size))
		{
			//WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
			WTSLogger::error("Sizechecking of back tick data file %s failed", filename.c_str());
			return false;
		}
	}

	auto& ticksList = _ticks_cache[key];
	uint32_t tickcnt = 0;
	if (tBlockV2 == NULL)
	{
		tickcnt = (content.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
		ticksList._items.resize(tickcnt);
		memcpy(ticksList._items.data(), tBlock->_ticks, sizeof(WTSTickStruct)*tickcnt);
	}
	else
	{
		//需要解压
		std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);
		tickcnt = buf.size() / sizeof(WTSTickStruct);
		ticksList._items.resize(tickcnt);
		memcpy(ticksList._items.data(), buf.data(), buf.size());
	}	
	
	ticksList._cursor = UINT_MAX;
	ticksList._code = stdCode;
	ticksList._date = uDate;
	ticksList._count = tickcnt;

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
		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisTickBlockV2))
		{
			//WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
			WTSLogger::error("Sizechecking of back tick data file %s failed", filename.c_str());
			return false;
		}

		HisTickBlockV2* tBlock = (HisTickBlockV2*)content.data();
		std::string rawData = WTSCmpHelper::uncompress_data(tBlock->_data, (uint32_t)tBlock->_size);
		uint32_t tickcnt = rawData.size() / sizeof(WTSTickStruct);

		auto& ticksList = _ticks_cache[key];
		ticksList._items.resize(tickcnt);
		memcpy(ticksList._items.data(), rawData.data(), rawData.size());
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
			//WTSLogger::error("历史Tick数据文件%s不存在", csvfile.c_str());
			WTSLogger::error("Back tick data file %s not exists", csvfile.c_str());
			return false;
		}

		std::ifstream ifs;
		ifs.open(csvfile.c_str());

		//WTSLogger::info("正在读取数据文件%s...", csvfile.c_str());
		WTSLogger::info("Reading data from %s...", csvfile.c_str());

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
			ticks.volume = strtoul(ay[3].c_str(), NULL, 10);
			tickList._items.emplace_back(ticks);

			if (tickList._items.size() % 1000 == 0)
			{
				//WTSLogger::info("已读取数据%u条", tickList._items.size());
				WTSLogger::info("%u lines of data loaded", tickList._items.size());
			}
		}
		tickList._count = tickList._items.size();
		ifs.close();
		//WTSLogger::info("数据文件%s全部读取完成, 共%u条", csvfile.c_str(), tickList._items.size());
		WTSLogger::info("Data file %s all loaded, totally %u items", csvfile.c_str(), tickList._items.size());

		HisTickBlockV2 tBlock;
		strcpy(tBlock._blk_flag, BLK_FLAG);
		tBlock._type = BT_HIS_Ticks;
		tBlock._version = BLOCK_VERSION_CMP;
		
		std::string cmpData = WTSCmpHelper::compress_data(tickList._items.data(), sizeof(WTSTickStruct)*tickList._count);
		tBlock._size = cmpData.size();

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			bf.write_file(&tBlock, sizeof(HisTickBlockV2));
		}
		bf.write_file(cmpData);
		bf.close_file();
		//WTSLogger::info("数据已转储至%s", filename.c_str());
		WTSLogger::info("Data dumped to file %s", filename.c_str());
	}

	return true;
}

bool HisDataReplayer::cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars/* = true*/)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
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

	std::stringstream ss;
	ss << _base_dir << "his/" << dirname << "/" << cInfo._exchg << "/";
	if (!StdFile::exists(ss.str().c_str()))
		BoostFile::create_directories(ss.str().c_str());

	if(cInfo.isHot() && cInfo.isFuture())
	{
		ss << cInfo._exchg << "." << cInfo._product << "_HOT.dsb";
	}
	else if (cInfo.isSecond() && cInfo.isFuture())
	{
		ss << cInfo._exchg << "." << cInfo._product << "_2ND.dsb";
	}
	else if (cInfo.isExright() && cInfo.isStock())
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
			//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
			WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
			return false;
		}

		HisKlineBlockV2* kBlock = (HisKlineBlockV2*)content.c_str();
		std::string rawData = WTSCmpHelper::uncompress_data(kBlock->_data, (uint32_t)kBlock->_size);
		uint32_t barcnt = rawData.size() / sizeof(WTSBarStruct);

		BarsList& barList = bForBars ? _bars_cache[key] : _unbars_cache[key];
		barList._bars.resize(barcnt);
		memcpy(barList._bars.data(), rawData.data(), rawData.size());
		barList._cursor = UINT_MAX;
		barList._code = stdCode;
		barList._period = period;
		barList._count = barcnt;

		WTSLogger::info("%u items of back %s data of %s directly loaded from dsb file", barcnt, pname.c_str(), stdCode);
	}
	else
	{
		//如果没有格式化的历史数据文件, 则从csv加载
		std::stringstream ss;
		ss << _base_dir << "csv/" << stdCode << "_" << pname << ".csv";
		std::string csvfile = ss.str();

		if (!StdFile::exists(csvfile.c_str()))
		{
			//WTSLogger::error("历史K线数据文件%s不存在", csvfile.c_str());
			WTSLogger::error("Back kbar data file %s not exists", csvfile.c_str());
			return false;
		}

		std::ifstream ifs;
		ifs.open(csvfile.c_str());

		//WTSLogger::info("正在读取数据文件%s...", csvfile.c_str());
		WTSLogger::info("Reading data from %s...", csvfile.c_str());

		char buffer[512];
		bool headerskipped = false;
		BarsList& barList = bForBars ? _bars_cache[key] : _unbars_cache[key];
		barList._code = stdCode;
		barList._period = period;
		while (!ifs.eof())
		{
			ifs.getline(buffer, 512);
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
			WTSBarStruct bs;
			bs.date = strToDate(ay[0].c_str());
			if (period != KP_DAY)
				bs.time = TimeUtils::timeToMinBar(bs.date, strToTime(ay[1].c_str()));
			bs.open = strtod(ay[2].c_str(), NULL);
			bs.high = strtod(ay[3].c_str(), NULL);
			bs.low = strtod(ay[4].c_str(), NULL);
			bs.close = strtod(ay[5].c_str(), NULL);
			bs.vol = strtoul(ay[6].c_str(), NULL, 10);
			if(ay.size() > 7)
				bs.money = strtod(ay[7].c_str(), NULL);
			if (ay.size() > 8)
				bs.hold = strtod(ay[8].c_str(), NULL);
			if (ay.size() > 9)
				bs.add = strtod(ay[9].c_str(), NULL);
			if (ay.size() > 10)
				bs.settle = strtod(ay[10].c_str(), NULL);
			barList._bars.emplace_back(bs);

			if (barList._bars.size() % 1000 == 0)
			{
				//WTSLogger::info("已读取数据%u条", barList._bars.size());
				WTSLogger::info("%u lines of data loaded", barList._bars.size());
			}
		}
		barList._count = barList._bars.size();
		ifs.close();
		//WTSLogger::info("数据文件%s全部读取完成, 共%u条", csvfile.c_str(), barList._bars.size());
		WTSLogger::info("Data file %s all loaded, totally %u items", csvfile.c_str(), barList._bars.size());

		BlockType btype;
		switch (period)
		{
		case KP_Minute1: btype = BT_HIS_Minute1; break;
		case KP_Minute5: btype = BT_HIS_Minute5; break;
		default: btype = BT_HIS_Day; break;
		}

		HisKlineBlockV2 kBlock;
		strcpy(kBlock._blk_flag, BLK_FLAG);
		kBlock._type = btype;
		kBlock._version = BLOCK_VERSION_CMP;

		std::string cmpData = WTSCmpHelper::compress_data(barList._bars.data(), sizeof(WTSBarStruct)*barList._count);
		kBlock._size = cmpData.size();

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			bf.write_file(&kBlock, sizeof(HisKlineBlockV2));
		}
		bf.write_file(cmpData);
		bf.close_file();
		//WTSLogger::info("数据已转储至%s", filename.c_str());
		WTSLogger::info("Data dumped to file %s", filename.c_str());
	}

	return true;
}

bool HisDataReplayer::cacheRawBarsFromDB(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars/* = true*/)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _bd_mgr.calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	std::string tbname, pname;
	switch (period)
	{
	case KP_Minute1:
		tbname = "tb_kline_min1";
		pname = "min1";
		break;
	case KP_Minute5:
		tbname = "tb_kline_min5";
		pname = "min5";
		break;
	default:
		tbname = "tb_kline_day";
		pname = "day";
		break;
	}

	BarsList& barList = bForBars ? _bars_cache[key] : _unbars_cache[key];
	barList._code = stdCode;
	barList._period = period;

	std::vector<std::vector<WTSBarStruct>*> barsSections;
	bool isDay = (period == KP_DAY);

	uint32_t realCnt = 0;
	if (!cInfo.isFlat() && cInfo.isFuture())//如果是读取期货主力连续数据
	{
		const char* flag = cInfo.isHot() ? "HOT" : "2ND";

		//先按照HOT代码进行读取, 如rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastHotTime = 0;
		for (;;)
		{
			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s.%s' ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, cInfo._product, flag);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s.%s' ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._product, flag);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				WTSLogger::error("Loading back kbar data from database failed: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				if (barcnt > 0)
				{
					hotAy = new std::vector<WTSBarStruct>();
					hotAy->resize(barcnt);

					uint32_t idx = 0;
					while (query.fetch_row())
					{
						WTSBarStruct& bs = hotAy->at(idx);
						bs.date		= query.getuint(0);
						bs.time		= query.getuint(1);
						bs.open		= query.getdouble(2);
						bs.high		= query.getdouble(3);
						bs.low		= query.getdouble(4);
						bs.close	= query.getdouble(5);
						bs.settle	= query.getdouble(6);
						bs.vol		= query.getuint(7);
						bs.money	= query.getdouble(8);
						bs.hold		= query.getuint(9);
						bs.add		= query.getdouble(10);
						idx++;
					}

					if (period != KP_DAY)
						lastHotTime = hotAy->at(barcnt - 1).time;
					else
						lastHotTime = hotAy->at(barcnt - 1).date;
				}

				//WTSLogger::info("主力合约%s历史%s数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
				WTSLogger::info("%u items of back %s data of hot contract %s directly loaded", barcnt, pname.c_str(), stdCode);
			}

			break;
		}

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
		for (auto it = secs.rbegin(); it != secs.rend() && left > 0; it++)
		{
			const HotSection& hotSec = *it;
			const char* curCode = hotSec._code.c_str();
			uint32_t rightDt = hotSec._e_date;
			uint32_t leftDt = hotSec._s_date;

			//要先将日期转换为边界时间
			uint32_t stime, etime;
			if (!isDay)
			{
				uint64_t sTime = _bd_mgr.getBoundaryTime(stdPID.c_str(), leftDt, false, true);
				uint64_t eTime = _bd_mgr.getBoundaryTime(stdPID.c_str(), rightDt, false, false);

				stime = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

				if (stime < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					stime = lastHotTime + 1;
				}

				etime = ((uint32_t)(eTime / 10000) - 19900000) * 10000 + (uint32_t)(eTime % 10000);

				if (etime <= lastHotTime)	//右边界时间小于最后一条Hot时间, 说明全部交叉了, 没有再找的必要了
					break;
			}
			else
			{
				stime = leftDt;
				if (stime < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					stime = lastHotTime + 1;
				}

				etime = rightDt;

				if (etime <= lastHotTime)
					break;
			}

			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s "
					"WHERE exchange='%s' AND code='%s' AND `date`>=%u AND `date`<=%u ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, curCode, stime, etime);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s "
					"WHERE exchange='%s' AND code='%s' AND `time`>=%u AND `time`<=%u ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, curCode, stime, etime);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				//WTSLogger::error("历史K线读取失败: %s", query.errormsg());
				WTSLogger::error("Loading back kbar data from database failed: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				if(barcnt)
				{
					auto tempAy = new std::vector<WTSBarStruct>();
					tempAy->resize(barcnt);

					uint32_t idx = 0;
					while (query.fetch_row())
					{
						WTSBarStruct& bs = tempAy->at(idx);
						bs.date		= query.getuint(0);
						bs.time		= query.getuint(1);
						bs.open		= query.getdouble(2);
						bs.high		= query.getdouble(3);
						bs.low		= query.getdouble(4);
						bs.close	= query.getdouble(5);
						bs.settle	= query.getdouble(6);
						bs.vol		= query.getuint(7);
						bs.money	= query.getdouble(8);
						bs.hold		= query.getuint(9);
						bs.add		= query.getdouble(10);
						idx++;
					}

					realCnt += barcnt;

					barsSections.emplace_back(tempAy);
				}

				if (bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if (cInfo.isExright() && cInfo.isStock())//如果是读取股票复权数据
	{
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastQTime = 0;

		do
		{
			char flag = cInfo._exright == 1 ? 'Q' : 'H';
			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s%c' ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, flag);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s%c' ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, flag);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				//WTSLogger::error("历史K线读取失败: %s", query.errormsg());
				WTSLogger::error("Loading back kbar data from database failed: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				if(barcnt > 0)
				{
					hotAy = new std::vector<WTSBarStruct>();
					hotAy->resize(barcnt);

					uint32_t idx = 0;
					while (query.fetch_row())
					{
						WTSBarStruct& bs = hotAy->at(idx);
						bs.date		= query.getuint(0);
						bs.time		= query.getuint(1);
						bs.open		= query.getdouble(2);
						bs.high		= query.getdouble(3);
						bs.low		= query.getdouble(4);
						bs.close	= query.getdouble(5);
						bs.settle	= query.getdouble(6);
						bs.vol		= query.getuint(7);
						bs.money	= query.getdouble(8);
						bs.hold		= query.getuint(9);
						bs.add		= query.getdouble(10);
						idx++;
					}

					if (period != KP_DAY)
						lastQTime = hotAy->at(barcnt - 1).time;
					else
						lastQTime = hotAy->at(barcnt - 1).date;
				}

				//WTSLogger::error("股票%s历史%s复权数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
				WTSLogger::info("%u items of adjusted back %s data of stock %s directly loaded", barcnt, pname.c_str(), stdCode);
			}

		} while (false);

		bool bAllCovered = false;
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
				sBar.date = lastQTime + 1;
			}

			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' AND date>=%u ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, lastQTime + 1);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' AND time>=%u ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, lastQTime + 1);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				//WTSLogger::error("历史K线读取失败: %s", query.errormsg());
				WTSLogger::error("Loading back kbar data from database failed: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				if (barcnt > 0)
				{
					auto tempAy = new std::vector<WTSBarStruct>();
					tempAy->resize(barcnt);

					uint32_t idx = 0;
					while (query.fetch_row())
					{
						WTSBarStruct& bs = tempAy->at(idx);
						bs.date		= query.getuint(0);
						bs.time		= query.getuint(1);
						bs.open		= query.getdouble(2);
						bs.high		= query.getdouble(3);
						bs.low		= query.getdouble(4);
						bs.close	= query.getdouble(5);
						bs.settle	= query.getdouble(6);
						bs.vol		= query.getuint(7);
						bs.money	= query.getdouble(8);
						bs.hold		= query.getuint(9);
						bs.add		= query.getdouble(10);
						idx++;
					}

					realCnt += barcnt;

					auto& ayFactors = getAdjFactors(cInfo._code, cInfo._exchg);
					if (!ayFactors.empty())
					{
						//做复权处理
						int32_t lastIdx = barcnt;
						WTSBarStruct bar;
						WTSBarStruct* firstBar = tempAy->data();

						//根据复权类型确定基础因子
						//如果是前复权，则历史数据会变小，以最后一个复权因子为基础因子
						//如果是后复权，则新数据会变大，基础因子为1
						double baseFactor = 1.0;
						if (cInfo._exright == 1)
							baseFactor = ayFactors.back()._factor;
						else if (cInfo._exright == 2)
							barList._factor = ayFactors.back()._factor;

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
							{
								break;
							}
						}
					}

					barsSections.emplace_back(tempAy);
				}
			}

		} while (false);

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else
	{
		std::string timecond = "";
		if(!bForBars)
		{
			if (isDay)
				timecond = StrUtil::printf("AND `date` >= %u ", _cur_tdate);
			else
				timecond = StrUtil::printf("AND `time` >= %u%04u ", _cur_date-19900000, _cur_time);
		}

		//读取历史的
		char sql[256] = { 0 };
		if (isDay)
			sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' %sORDER BY `date`;",
				tbname.c_str(), cInfo._exchg, cInfo._code, timecond.c_str());
		else
			sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' %sORDER BY `time`;",
				tbname.c_str(), cInfo._exchg, cInfo._code, timecond.c_str());

		MysqlQuery query(*_db_conn);
		if (!query.exec(sql))
		{
			//WTSLogger::error("历史K线读取失败: %s", query.errormsg());
			WTSLogger::error("Loading back kbar data from database failed: %s", query.errormsg());
		}
		else
		{
			uint32_t barcnt = (uint32_t)query.num_rows();
			if(barcnt > 0)
			{
				auto tempAy = new std::vector<WTSBarStruct>();
				tempAy->resize(barcnt);

				uint32_t idx = 0;
				while (query.fetch_row())
				{
					WTSBarStruct& bs = tempAy->at(idx);
					bs.date		= query.getuint(0);
					bs.time		= query.getuint(1);
					bs.open		= query.getdouble(2);
					bs.high		= query.getdouble(3);
					bs.low		= query.getdouble(4);
					bs.close	= query.getdouble(5);
					bs.settle	= query.getdouble(6);
					bs.vol		= query.getuint(7);
					bs.money	= query.getdouble(8);
					bs.hold		= query.getuint(9);
					bs.add		= query.getdouble(10);
					idx++;
				}

				realCnt += barcnt;

				barsSections.emplace_back(tempAy);
			}
		}
	}

	if (realCnt > 0)
	{
		barList._bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barList._bars.data() + curIdx, tempAy->data(), tempAy->size() * sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barsSections.clear();
	}

	WTSLogger::info("%u items of back %s data of contract %s cached", realCnt, pname.c_str(), stdCode);
	return true;
}

bool HisDataReplayer::cacheRawBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars/* = true*/)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
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

	BarsList& barList = bForBars ? _bars_cache[key] : _unbars_cache[key];
	barList._code = stdCode;
	barList._period = period;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;
	if (!cInfo.isFlat() && cInfo.isFuture())//如果是读取期货主力连续数据
	{
		const char* hot_flag = cInfo.isHot() ? "HOT" : "2ND";

		//先按照HOT代码进行读取, 如rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastHotTime = 0;
		for (;;)
		{
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._exchg << "." << cInfo._product << "_" << hot_flag << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
				WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
					break;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					break;

				std::string rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), rawData.data(), rawData.size());
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), kBlock->_bars, sizeof(WTSBarStruct)*barcnt);
			}

			if (period != KP_DAY)
				lastHotTime = hotAy->at(barcnt - 1).time;
			else
				lastHotTime = hotAy->at(barcnt - 1).date;

			//WTSLogger::info("主力%s历史%s数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
			WTSLogger::info("%u items of back %s data of hot contract %s directly loaded", barcnt, pname.c_str(), stdCode);

			break;
		}

		HotSections secs;
		if(cInfo.isHot())
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
		for (auto it = secs.rbegin(); it != secs.rend() && left > 0; it++)
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
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
					sBar.date = lastHotTime + 1;
				}

				eBar.date = rightDt;

				if (eBar.date <= lastHotTime)
					break;
			}

			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			{
				std::string content;
				StdFile::read_file_content(filename.c_str(), content);
				if (content.size() < sizeof(HisKlineBlock))
				{
					//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
					return false;
				}

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				WTSBarStruct* firstBar = NULL;
				uint32_t barcnt = 0;
				std::string rawData;
				if (kBlock->_version == BLOCK_VERSION_CMP)
				{
					if (content.size() < sizeof(HisKlineBlockV2))
					{
						//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
						WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
						break;
					}

					HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
					if (kBlockV2->_size == 0)
						break;

					rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
					barcnt = rawData.size() / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						break;

					firstBar = (WTSBarStruct*)rawData.data();
				}
				else
				{
					barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						continue;

					firstBar = kBlock->_bars;
				}

				WTSBarStruct* pBar = std::lower_bound(firstBar, firstBar + (barcnt - 1), sBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
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

				pBar = std::lower_bound(firstBar + sIdx, firstBar + (barcnt - 1), eBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
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
		}

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if (cInfo.isExright() && cInfo.isStock())//如果是读取股票复权数据
	{
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastQTime = 0;

		do
		{
			//先直接读取复权过的历史数据,路径如/his/day/sse/SH600000Q.dsb
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._code << (cInfo._exright==1?"Q":"H") << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
				WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
					break;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					break;

				std::string rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), rawData.data(), rawData.size());
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), kBlock->_bars, sizeof(WTSBarStruct)*barcnt);
			}

			if (period != KP_DAY)
				lastQTime = hotAy->at(barcnt - 1).time;
			else
				lastQTime = hotAy->at(barcnt - 1).date;

			//WTSLogger::info("%s历史%s复权数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
			WTSLogger::info("%u items of adjusted back %s data of stock %s directly loaded", barcnt, pname.c_str(), stdCode);
			break;
		} while (false);

		bool bAllCovered = false;
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
				sBar.date = lastQTime + 1;
			}

			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			{
				std::string content;
				StdFile::read_file_content(filename.c_str(), content);
				if (content.size() < sizeof(HisKlineBlock))
				{
					//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
					return false;
				}

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				WTSBarStruct* firstBar = NULL;
				uint32_t barcnt = 0;
				std::string rawData;
				if (kBlock->_version == BLOCK_VERSION_CMP)
				{
					if (content.size() < sizeof(HisKlineBlockV2))
					{
						//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
						WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
						break;
					}

					HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
					if (kBlockV2->_size == 0)
						break;

					rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
					barcnt = rawData.size() / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						break;

					firstBar = (WTSBarStruct*)rawData.data();
				}
				else
				{
					barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						continue;

					firstBar = kBlock->_bars;
				}

				WTSBarStruct* pBar = std::lower_bound(firstBar, firstBar + (barcnt - 1), sBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
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

					auto& ayFactors = getAdjFactors(cInfo._code, cInfo._exchg);
					if (!ayFactors.empty())
					{
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
							barList._factor = ayFactors.back()._factor;

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

							if(pBar->date < bar.date)
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

					barsSections.emplace_back(tempAy);
				}
			}
		} while (false);

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else
	{
		//读取历史的
		std::stringstream ss;
		ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._code << ".dsb";
		std::string filename = ss.str();
		if (StdFile::exists(filename.c_str()))
		{
			//如果有格式化的历史数据文件, 则直接读取
			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
				WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
				return false;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			WTSBarStruct* firstBar = NULL;
			uint32_t barcnt = 0;
			std::string rawData;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					//WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					WTSLogger::error("Sizechecking of back kbar data file %s failed", filename.c_str());
					return false;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					return false;

				rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					return false;

				firstBar = (WTSBarStruct*)rawData.data();
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					return false;

				firstBar = kBlock->_bars;
			}

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
		}
	}

	if (realCnt > 0)
	{
		barList._bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barList._bars.data() + curIdx, tempAy->data(), tempAy->size()*sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barList._count = barList._bars.size();
		barsSections.clear();
	}

	WTSLogger::info("%u items of back %s data of %s cached", realCnt, pname.c_str(), stdCode);
	return true;
}