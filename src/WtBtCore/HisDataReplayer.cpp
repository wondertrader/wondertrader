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

#pragma comment(lib, "libmysql.lib")

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
{
}


HisDataReplayer::~HisDataReplayer()
{
}


bool HisDataReplayer::init(WTSVariant* cfg)
{
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

	loadFees(cfg->getCString("fees"));

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
			init_db();
	}

	bool bAdjLoaded = false;
	if (_db_conn)
		bAdjLoaded = loadStkAdjFactorsFromDB();

	if (!bAdjLoaded && cfg->has("adjfactor"))
		loadStkAdjFactors(cfg->getCString("adjfactor"));

	return true;
}

void HisDataReplayer::init_db()
{
	if (!_db_conf._active)
		return;

	_db_conn.reset(new MysqlDb);
	my_bool autoreconnect = true;
	_db_conn->options(MYSQL_OPT_RECONNECT, &autoreconnect);
	_db_conn->options(MYSQL_SET_CHARSET_NAME, "utf8");

	if (_db_conn->connect(_db_conf._dbname, _db_conf._host, _db_conf._user, _db_conf._pass, _db_conf._port, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS))
	{
		WTSLogger::info("数据库连接成功:%s:%d", _db_conf._host, _db_conf._port);
	}
	else
	{
		WTSLogger::error("数据库连接失败[%s:%d]:%s", _db_conf._host, _db_conf._port, _db_conn->errstr());
		_db_conn.reset();
	}
}

bool HisDataReplayer::loadStkAdjFactorsFromDB()
{
	MysqlQuery query(*_db_conn);
	if (!query.exec("SELECT exchange,code,date,factor FROM tb_adj_factors ORDER BY exchange,code,date DESC;"))
	{
		WTSLogger::error("查询除权因子表出错:%s", query.errormsg());
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

		fctrLst.push_back(adjFact);
		fct_cnt++;
	}

	WTSLogger::info("共加载%u只股票的%u条除权因子数据", stk_cnt, fct_cnt);
	return true;
}

bool HisDataReplayer::loadStkAdjFactors(const char* adjfile)
{
	if (!BoostFile::exists(adjfile))
	{
		WTSLogger::error("除权因子文件%s不存在", adjfile);
		return false;
	}

	std::string content;
	BoostFile::read_file_contents(adjfile, content);

	rj::Document doc;
	doc.Parse(content.c_str());

	if (doc.HasParseError())
	{
		WTSLogger::error("除权因子文件%s解析失败", adjfile);
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

				fctrLst.push_back(adjFact);
				fct_cnt++;
			}
		}
	}

	WTSLogger::info("共加载%u只股票的%u条除权因子数据", stk_cnt, fct_cnt);
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

	WTSLogger::info("定时任务注册成功，周期：%s", period);
}

void HisDataReplayer::run()
{
	_cur_date = (uint32_t)(_begin_time / 10000);
	_cur_time = (uint32_t)(_begin_time % 10000);
	_cur_secs = 0;
	_cur_tdate = _bd_mgr.calcTradingDate(DEFAULT_SESSIONID, _cur_date, _cur_time, true);

	_listener->handle_init();	

	if(_task == NULL)
	{
		//如果没有时间调度任务，则采用主K线回放的模式
		BarsList& barList = _bars_cache[_main_key];
		WTSSessionInfo* sInfo = get_session_info(barList._code.c_str(), true);
		std::string commId = CodeHelper::stdCodeToStdCommID(barList._code.c_str());

		WTSLogger::log_raw(LL_INFO, fmt::format("开始从{}进行数据回放……", _begin_time).c_str());

		for (;;)
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
					WTSLogger::log_raw(LL_INFO, fmt::format("{}超过结束时间{}，回放结束", nextBarTime, _end_time).c_str());
					_listener->handle_replay_done();
					break;
				}

				uint32_t nextDate = (uint32_t)(nextBarTime / 10000);
				uint32_t nextTime = (uint32_t)(nextBarTime % 10000);

				uint32_t nextTDate = _bd_mgr.calcTradingDate(commId.c_str(), nextDate, nextTime, false);
				if (_opened_tdate != nextTDate)
				{
					_listener->handle_session_begin();
					_opened_tdate = nextTDate;
					//WTSLogger::info("交易日%u开始", nextTDate);
					_cur_tdate = nextTDate;
				}
				;
				if (_tick_enabled)
				{
					uint64_t curBarTime = (uint64_t)_cur_date * 10000 + _cur_time;
					replayTicks(curBarTime, nextBarTime);
				}

				_cur_date = nextDate;
				_cur_time = nextTime;
				_cur_secs = 0;

				uint32_t offTime = sInfo->offsetTime(_cur_time);
				bool isEndTDate = (offTime >= sInfo->getCloseTime(true));

				onMinuteEnd(nextDate, nextTime, (isDay || isEndTDate) ? nextTDate : 0);

				if (isEndTDate && _closed_tdate != _cur_tdate)
				{
					_listener->handle_session_end();
					_closed_tdate = _cur_tdate;
					_day_cache.clear();
					//WTSLogger::info("交易日%u结束", _cur_tdate);
				}

				if (barList._cursor >= barList._bars.size())
				{
					WTSLogger::info("全部数据都已回放，回放结束");
					_listener->handle_replay_done();
					break;
				}
			}
			else
			{
				WTSLogger::info("数据尚未初始化，回放直接退出");
				_listener->handle_replay_done();
				break;
			}
		}

		if (_closed_tdate != _cur_tdate)
		{
			_listener->handle_session_end();
		}
	}
	else //if(_task != NULL)
	{
		//时间调度任务不为空，则按照时间调度任务回放
		WTSSessionInfo* sInfo = NULL;
		const char* DEF_SESS = (strlen(_task->_session) == 0) ? DEFAULT_SESSIONID : _task->_session;
		sInfo = _bd_mgr.getSession(DEF_SESS);
		WTSLogger::log_raw(LL_INFO, fmt::format("从{}开始按任务周期回测……", _begin_time).c_str());

		//分钟即任务和日级别任务分开写
		if (_task->_period != TPT_Minute)
		{
			uint32_t endtime = TimeUtils::getNextMinute(_task->_time, -1);
			bool bIsPreDay = endtime > _task->_time;
			if (bIsPreDay)
				_cur_date = TimeUtils::getNextDate(_cur_date, -1);
			for (;;)
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
								//上一个交易日在上个月，且当前日期大于触发日期
								//说明这个月的开始日期在节假日内，顺延到今天
								if ((preTDate % 10000 / 100 < _cur_date % 10000 / 100) && _cur_date % 1000000 > _task->_day)
								{
									fired = true;
								}
								else if (preTDate % 1000000 < _task->_day && _cur_date % 1000000 > _task->_day)
								{
									//上一个交易日在同一个月，且小于触发日期，但是今天大于触发日期，说明正确触发日期到节假日内，顺延到今天
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
									//上一个交易日的星期大于今天的星期，说明换了一周了
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
					//如果当前时间小于任务时间，则直接赋值即可
					//如果当前时间大于任务时间，则至少要等下一天
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
							_listener->handle_session_begin();
						if (_listener)
							_listener->handle_session_end();
					}
				}
				else
				{
					//用前一分钟作为结束时间
					uint32_t curDate = _cur_date;
					uint32_t curTime = endtime;
					bool bEndSession = sInfo->offsetTime(curTime) >= sInfo->getCloseTime(true);
					if (_listener)
						_listener->handle_session_begin();
					onMinuteEnd(curDate, curTime, bEndSession ? _cur_tdate : preTDate);
					if (_listener)
						_listener->handle_session_end();
				}

				_cur_date = TimeUtils::getNextDate(_cur_date);
				_cur_time = endtime;
				_cur_tdate = _bd_mgr.calcTradingDate(DEF_SESS, _cur_date, _cur_time, true);

				uint64_t nextTime = (uint64_t)_cur_date * 10000 + _cur_time;
				if (nextTime > _end_time)
				{
					WTSLogger::info("按任务周期回测结束");
					if (_listener)
					{
						_listener->handle_session_end();
						_listener->handle_replay_done();
					}

					break;
				}
			}
		}
		else
		{
			if (_listener)
				_listener->handle_session_begin();

			for(;;)
			{
				//要考虑到跨日的情况
				uint32_t mins = sInfo->timeToMinutes(_cur_time);
				//如果一开始不能整除，则直接修正一下
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
						_listener->handle_session_end();
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
							//真实时间后移，说明夜盘算作下一天的
							_cur_date = _cur_tdate;
							_cur_tdate = nextTDate;
						}
						else
						{
							//真实时间前移，说明夜盘是上一天的，这种情况就不需要动了
							_cur_tdate = nextTDate;
							_cur_date = _cur_tdate;
						}
					}

					_cur_time = sInfo->minuteToTime(mins);

					if (_listener)
						_listener->handle_session_begin();
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
					WTSLogger::info("按任务周期回测结束");
					if (_listener)
					{
						_listener->handle_session_end();
						_listener->handle_replay_done();
					}
					break;
				}
			}
		}
	}
}

void HisDataReplayer::replayTicks(uint64_t stime, uint64_t etime)
{
	for (;;)
	{
		uint64_t nextTime = UINT64_MAX;
		for (auto v : _tick_sub_map)
		{
			std::string stdCode = v.first;
			if(!checkTicks(stdCode.c_str(), _cur_tdate))
				continue;

			TickList& tickList = _ticks_cache[stdCode];
			if (tickList._cursor == UINT_MAX)
			{
				uint32_t uDate = (uint32_t)(stime / 10000);
				uint32_t uTime = (uint32_t)(stime % 10000);

				WTSTickStruct curTick;
				curTick.action_date = uDate;
				curTick.action_time = uTime * 100000;

				auto tit = std::lower_bound(tickList._ticks.begin(), tickList._ticks.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
					if (a.action_date != b.action_date)
						return a.action_date < b.action_date;
					else
						return a.action_time < b.action_time;
				});

				uint32_t idx = tit - tickList._ticks.begin();
				tickList._cursor = idx + 1;
			}

			if(tickList._cursor >= tickList._count)
				continue;

			const WTSTickStruct& nextTick = tickList._ticks[tickList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;

			nextTime = min(lastTime, nextTime);
		}

		if (nextTime/100000 >= etime)
			break;

		_cur_date = (uint32_t)(nextTime / 1000000000);
		_cur_time = nextTime % 1000000000 / 100000;
		_cur_secs = nextTime % 100000;
		for (auto v : _tick_sub_map)
		{
			std::string stdCode = v.first;
			TickList& tickList = _ticks_cache[stdCode];
			WTSTickStruct& nextTick = tickList._ticks[tickList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;
			if(lastTime <= nextTime)
			{
				WTSTickData* newTick = WTSTickData::create(nextTick);
				newTick->setCode(stdCode.c_str());
				_listener->handle_tick(stdCode.c_str(), newTick);
				newTick->release();

				tickList._cursor++;
			}
		}
	}
}

void HisDataReplayer::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */)
{
	//这里应该触发检查
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;

	//if(_task && endTDate != 0)
	//{
	//	//如果是交易日结束，清理掉分钟线缓存，不然吃内存太多
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
	//			WTSLogger::info("数据缓存%s已被清理", key.c_str());
	//		}
	//	}
	//}

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = it->second;
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
							if (!_tick_enabled)
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
									curTS.volumn = nextBar.vol;

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
									curTS.volumn = nextBar.vol;
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
							if (!_tick_enabled)
							{
								const std::string& ticker = _ticker_keys[barsList._code];
								if (ticker == it->first)
								{
									CodeHelper::CodeInfo cInfo;
									CodeHelper::extractStdCode(barsList._code.c_str(), cInfo);

									std::string realCode = barsList._code;
									if (cInfo._category == CC_Stock && cInfo._exright)
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
									curTS.volumn = nextBar.vol;
									WTSTickData* curTick = WTSTickData::create(curTS);
									_listener->handle_tick(realCode.c_str(), curTick);
									curTick->release();

									curTS.price = nextBar.high;
									curTS.volumn = nextBar.vol;
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
	//不做判断，主要为了防止没有tick数据，而采用第二方案
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
			}
			else if (oldKey.at(0) == period[0] && times < strtoul(oldKey.substr(2, 1).c_str(), NULL, 10))
			{
				_ticker_keys[stdCode] = key;
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
	if (it == _bars_cache.end())
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
	bool isClosed = sInfo->offsetTime(_cur_time) >= sInfo->getCloseTime(true);
	if (realTimes != 1 && it == _bars_cache.end())
	{

		std::string rawKey = StrUtil::printf("%s#%s#%u", stdCode, period, 1);
		BarsList& rawBars = _bars_cache[rawKey];

		WTSKlineSlice* rawKline = WTSKlineSlice::create(stdCode, kp, realTimes, &rawBars._bars[0], rawBars._bars.size());
		rawKline->setCode(stdCode);
		//memcpy(rawKline->getDataRef().data(), rawBars._bars.data(), sizeof(WTSBarStruct)*rawBars._bars.size());

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
					else
					{
						return NULL;
					}
				}
			}
		}
		else
		{

		}

		kBlkPair._cursor = eIdx + 1;
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
	if (!checkTicks(stdCode, _cur_tdate))
		return NULL;

	TickList& tickList = _ticks_cache[stdCode];
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

		auto tit = std::lower_bound(tickList._ticks.begin(), tickList._ticks.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - tickList._ticks.begin();
		tickList._cursor = idx + 1;
	}

	uint32_t eIdx = tickList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSTickSlice* ticks = WTSTickSlice::create(stdCode, tickList._ticks.data() + sIdx, realCnt);
	return ticks;
}

/*
WTSHisTickData* HisDataReplayer::get_ticks(const char* stdCode, uint32_t count, uint64_t etime)
{
	if (!checkTicks(stdCode, _cur_tdate))
		return NULL;

	TickList& tickList = _ticks_cache[stdCode];
	if (tickList._cursor == 0)
		return NULL;

	if (tickList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;
		if(etime != 0)
		{
			uDate = (uint32_t)(etime / 10000);
			uTime = (uint32_t)(etime % 10000 * 100000);
		}

		WTSTickStruct curTick;
		curTick.action_date = uDate;
		curTick.action_time = uTime;

		auto tit = std::lower_bound(tickList._ticks.begin(), tickList._ticks.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - tickList._ticks.begin();
		tickList._cursor = idx + 1;
	}

	uint32_t eIdx = tickList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSHisTickData* ticks = WTSHisTickData::create(stdCode, realCnt);
	memcpy(ticks->getDataRef().data(), tickList._ticks.data() + sIdx, sizeof(WTSTickStruct)*realCnt);
	return ticks;
}
*/

bool HisDataReplayer::checkTicks(const char* stdCode, uint32_t uDate)
{
	bool bNeedCache = false;
	auto it = _ticks_cache.find(stdCode);
	if (it == _ticks_cache.end())
		bNeedCache = true;
	else
	{
		TickList& tickList = it->second;
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
			has