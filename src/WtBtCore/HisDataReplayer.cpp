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
			initDB();
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

	if (!_tick_enabled)
		checkUnbars();

	if(_task == NULL)
	{
		//如果没有时间调度任务，则采用主K线回放的模式

		//如果没有确定主K线，则确定一个周期最短的主K线
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

			WTSLogger::info("主K线自动确定：%s", _main_key.c_str());
		}

		if(!_main_key.empty())
		{
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

					uint64_t curBarTime = (uint64_t)_cur_date * 10000 + _cur_time;
					if (_tick_enabled)
					{//如果开始了tick回放，则直接回放tick数据
						replayHftDatas(curBarTime, nextBarTime);
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

					onMinuteEnd(nextDate, nextTime, (isDay || isEndTDate) ? nextTDate : 0);

					if (isEndTDate && _closed_tdate != _cur_tdate)
					{
						_listener->handle_session_end();
						_closed_tdate = _cur_tdate;
						_day_cache.clear();
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
		else if(_tick_enabled)
		{
			uint32_t edt = (uint32_t)(_end_time / 10000);
			uint32_t etime = (uint32_t)(_end_time % 10000);
			uint64_t end_tdate = _bd_mgr.calcTradingDate(DEFAULT_SESSIONID, edt, etime, true);

			while(_cur_tdate <= end_tdate)
			{
				if(checkAllTicks(_cur_tdate))
				{
					WTSLogger::info("开始回放%u的tick数据...", _cur_tdate);
					_listener->handle_session_begin();
					replayHftDatasByDay(_cur_tdate);
					_listener->handle_session_end();
				}

				_cur_tdate = TimeUtils::getNextDate(_cur_tdate);
			}

			WTSLogger::info("全部数据都已回放，回放结束");
			_listener->handle_replay_done();
		}
		else
		{
			WTSLogger::info("没有订阅主力K线且未开放tick回测，回放直接退出");
			_listener->handle_replay_done();
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

void HisDataReplayer::replayUnbars(uint64_t stime, uint64_t nowTime, uint32_t endTDate /* = 0 */)
{
	//uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	uint32_t uDate = (uint32_t)(stime / 10000);

	for (auto it = _unbars_cache.begin(); it != _unbars_cache.end(); it++)
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
						if(barTime <= stime)
							continue;

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
	for (auto v : _tick_sub_map)
	{
		std::string stdCode = v.first;
		if (!checkTicks(stdCode.c_str(), curTDate))
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

				auto tit = std::lower_bound(itemList._items.begin(), itemList._items.end(), curItem, [](const auto& a, const auto& b) {
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

				WTSTransStruct curItem;
				curItem.action_date = uDate;
				curItem.action_time = uTime * 100000;

				auto tit = std::lower_bound(itemList._items.begin(), itemList._items.end(), curItem, [](const auto& a, const auto& b) {
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

				WTSTransStruct curItem;
				curItem.action_date = uDate;
				curItem.action_time = uTime * 100000;

				auto tit = std::lower_bound(itemList._items.begin(), itemList._items.end(), curItem, [](const auto& a, const auto& b) {
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
		for (auto v : _orddtl_sub_map)
		{
			std::string stdCode = v.first;
			auto& itemList = _orddtl_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdDtlData* newData = WTSOrdDtlData::create(nextItem);
				newData->setCode(stdCode.c_str());
				_listener->handle_order_detail(stdCode.c_str(), newData);
				newData->release();

				itemList._cursor++;
				total_ticks++;
			}
		}

		//2、其次再回放成交明细
		for (auto v : _trans_sub_map)
		{
			std::string stdCode = v.first;
			auto& itemList = _trans_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSTransData* newData = WTSTransData::create(nextItem);
				newData->setCode(stdCode.c_str());
				_listener->handle_transaction(stdCode.c_str(), newData);
				newData->release();

				itemList._cursor++;
				total_ticks++;
			}
		}

		//3、第三步再回放tick数据
		for (auto v : _tick_sub_map)
		{
			std::string stdCode = v.first;
			auto& tickList = _ticks_cache[stdCode];
			WTSTickStruct& nextTick = tickList._items[tickList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;
			if (lastTime <= nextTime)
			{
				WTSTickData* newTick = WTSTickData::create(nextTick);
				newTick->setCode(stdCode.c_str());
				_listener->handle_tick(stdCode.c_str(), newTick);
				newTick->release();

				tickList._cursor++;
				total_ticks++;
			}
		}
		
		//4、最后回放委托队列
		for (auto v : _ordque_sub_map)
		{
			std::string stdCode = v.first;
			auto& itemList = _ordque_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdQueData* newData = WTSOrdQueData::create(nextItem);
				newData->setCode(stdCode.c_str());
				_listener->handle_order_queue(stdCode.c_str(), newData);
				newData->release();

				itemList._cursor++;
				total_ticks++;
			}
		}
	}

	return total_ticks;
}

void HisDataReplayer::replayHftDatas(uint64_t stime, uint64_t etime)
{
	for (;;)
	{
		uint64_t nextTime = min(UINT64_MAX, getNextTickTime(_cur_tdate, stime));
		nextTime = min(nextTime, getNextOrdDtlTime(_cur_tdate, stime));
		nextTime = min(nextTime, getNextOrdQueTime(_cur_tdate, stime));
		nextTime = min(nextTime, getNextTransTime(_cur_tdate, stime));

		if (nextTime/100000 >= etime)
			break;

		_cur_date = (uint32_t)(nextTime / 1000000000);
		_cur_time = nextTime % 1000000000 / 100000;
		_cur_secs = nextTime % 100000;
		
		//1、首先回放委托明细
		for (auto v : _orddtl_sub_map)
		{
			std::string stdCode = v.first;
			auto& itemList = _orddtl_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdDtlData* newData = WTSOrdDtlData::create(nextItem);
				newData->setCode(stdCode.c_str());
				_listener->handle_order_detail(stdCode.c_str(), newData);
				newData->release();

				itemList._cursor++;
			}
		}

		//2、其次再回放成交明细
		for (auto v : _trans_sub_map)
		{
			std::string stdCode = v.first;
			auto& itemList = _trans_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSTransData* newData = WTSTransData::create(nextItem);
				newData->setCode(stdCode.c_str());
				_listener->handle_transaction(stdCode.c_str(), newData);
				newData->release();

				itemList._cursor++;
			}
		}

		//3、第三步再回放tick数据
		for (auto v : _tick_sub_map)
		{
			std::string stdCode = v.first;
			auto& itemList = _ticks_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSTickData* newData = WTSTickData::create(nextItem);
				newData->setCode(stdCode.c_str());
				_listener->handle_tick(stdCode.c_str(), newData);
				newData->release();

				itemList._cursor++;
			}
		}

		//4、最后回放委托队列
		for (auto v : _ordque_sub_map)
		{
			std::string stdCode = v.first;
			auto& itemList = _ordque_cache[stdCode];
			auto& nextItem = itemList._items[itemList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextItem.action_date * 1000000000 + nextItem.action_time;
			if (lastTime <= nextTime)
			{
				WTSOrdQueData* newData = WTSOrdQueData::create(nextItem);
				newData->setCode(stdCode.c_str());
				_listener->handle_order_queue(stdCode.c_str(), newData);
				newData->release();

				itemList._cursor++;
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

		uint32_t idx = tit - tickList._items.begin();
		tickList._cursor = idx + 1;
	}

	uint32_t eIdx = tickList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSTickSlice* ticks = WTSTickSlice::create(stdCode, tickList._items.data() + sIdx, realCnt);
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

bool HisDataReplayer::checkAllTicks(uint32_t uDate)
{
	bool bHasTick = false;
	for (auto v : _tick_sub_map)
	{
		std::string stdCode = v.first;
		bHasTick = bHasTick || checkTicks(stdCode.c_str(), uDate);
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
		WTSLogger::error("手续费模板文件%s不存在", filename);
		return;
	}


	std::string content;
	StdFile::read_file_content(filename, content);
	if (content.empty())
	{
		WTSLogger::error("手续费模板文件%s为空", filename);
		return;
	}

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
	{
		WTSLogger::error("手续费模板文件%s解析失败", filename);
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	if (!jsonToVariant(root, cfg))
	{
		WTSLogger::error("手续费模板文件%s转换失败", filename);
		return;
	}

	auto keys = cfg->memberNames();
	for (const std::string& key : keys)
	{
		WTSVariant* cfgItem = cfg->get(key.c_str());
		FeeItem& fItem = _fee_map[key];
		fItem._by_volumn = cfgItem->getBoolean("byvolumn");
		fItem._open = cfgItem->getDouble("open");
		fItem._close = cfgItem->getDouble("close");
		fItem._close_today = cfgItem->getDouble("closetoday");
	}

	cfg->release();

	WTSLogger::info("共加载%u条手续费模板", _fee_map.size());
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
	if (fItem._by_volumn)
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
	SIDSet& sids = _tick_sub_map[stdCode];
	sids.insert(sid);
}

void HisDataReplayer::sub_order_detail(uint32_t sid, const char* stdCode)
{
	SIDSet& sids = _orddtl_sub_map[stdCode];
	sids.insert(sid);
}

void HisDataReplayer::sub_order_queue(uint32_t sid, const char* stdCode)
{
	SIDSet& sids = _ordque_sub_map[stdCode];
	sids.insert(sid);
}

void HisDataReplayer::sub_transaction(uint32_t sid, const char* stdCode)
{
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
			if (ay[0] == stdCode)
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
			if (ay[0] == stdCode)
			{
				bHasBars = true;
				break;
			}
		}

		if (bHasBars)
			continue;

		//如果订阅了tick，但是没有对应的K线数据，则自动加载1分钟线到内存中
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
	if (ret > 10000 && !bHasSec)
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
	CodeHelper::extractStdFutCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);
	
	std::string rawCode = cInfo._code;
	if (cInfo._hot)
	{
		rawCode = _hot_mgr.getRawCode(cInfo._exchg, cInfo._product, uDate);
	}

	std::stringstream ss;
	ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << rawCode << ".dsb";

	std::string filename = ss.str();
	if (!StdFile::exists(filename.c_str()))
		return false;

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);
	if (content.size() < sizeof(HisTickBlock))
	{
		WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
		return false;
	}

	HisTickBlock* tBlock = (HisTickBlock*)content.c_str();
	HisTickBlockV2* tBlockV2 = NULL;
	if(tBlock->_version == BLOCK_VERSION_CMP)
	{
		//压缩版本，要重新检查文件大小
		tBlockV2 = (HisTickBlockV2*)content.c_str();

		if (content.size() != (sizeof(HisTickBlockV2) + tBlockV2->_size))
		{
			WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
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
			WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
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
			WTSLogger::error("历史Tick数据文件%s不存在", csvfile.c_str());
			return false;
		}

		std::ifstream ifs;
		ifs.open(csvfile.c_str());

		WTSLogger::info("正在读取数据文件%s……", csvfile.c_str());

		char buffer[512];
		bool headerskipped = false;
		auto& tickList = _ticks_cache[key];
		tickList._code = stdCode;
		tickList._date = uDate;
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
			WTSTickStruct ticks;
			ticks.action_date = strToDate(ay[0].c_str());
			ticks.action_time = strToTime(ay[1].c_str(), true) * 1000;
			ticks.price = strtod(ay[2].c_str(), NULL);
			ticks.volumn = strtoul(ay[3].c_str(), NULL, 10);
			tickList._items.push_back(ticks);

			if (tickList._items.size() % 1000 == 0)
			{
				WTSLogger::info("已读取数据%u条", tickList._items.size());
			}
		}
		tickList._count = tickList._items.size();
		ifs.close();
		WTSLogger::info("数据文件%s全部读取完成, 共%u条", csvfile.c_str(), tickList._items.size());

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
		WTSLogger::info("数据已转储至%s", filename.c_str());
	}

	return true;
}

bool HisDataReplayer::cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars/* = true*/)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "m1"; break;
	case KP_Minute5: pname = "m5"; break;
	case KP_DAY: pname = "d"; break;
	default: pname = ""; break;
	}

	std::stringstream ss;
	ss << _base_dir << "bin/";
	if (!StdFile::exists(ss.str().c_str()))
		BoostFile::create_directories(ss.str().c_str());
	ss << stdCode << "_" << pname << ".dsb";
	std::string filename = ss.str();
	if (StdFile::exists(filename.c_str()))
	{
		//如果有格式化的历史数据文件, 则直接读取
		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisKlineBlockV2))
		{
			WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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
	}
	else
	{
		//如果没有格式化的历史数据文件, 则从csv加载
		std::stringstream ss;
		ss << _base_dir << "csv/" << stdCode << "_" << pname << ".csv";
		std::string csvfile = ss.str();

		if (!StdFile::exists(csvfile.c_str()))
		{
			WTSLogger::error("历史K线数据文件%s不存在", csvfile.c_str());
			return false;
		}

		std::ifstream ifs;
		ifs.open(csvfile.c_str());

		WTSLogger::info("正在读取数据文件%s……", csvfile.c_str());

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
			barList._bars.push_back(bs);

			if (barList._bars.size() % 1000 == 0)
			{
				WTSLogger::info("已读取数据%u条", barList._bars.size());
			}
		}
		barList._count = barList._bars.size();
		ifs.close();
		WTSLogger::info("数据文件%s全部读取完成, 共%u条", csvfile.c_str(), barList._bars.size());

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
		WTSLogger::info("数据已转储至%s", filename.c_str());
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
	if (cInfo._hot && cInfo._category == CC_Future)//如果是读取期货主力连续数据
	{
		HotSections secs;
		if (!_hot_mgr.splitHotSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
			return false;

		if (secs.empty())
			return false;

		//先按照HOT代码进行读取, 如rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastHotTime = 0;
		for (;;)
		{
			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s.HOT' ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, cInfo._product);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s.HOT' ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._product);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				WTSLogger::error("历史K线读取失败：%s", query.errormsg());
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
						bs.date = query.getuint(0);
						bs.time = query.getuint(1);
						bs.open = query.getdouble(2);
						bs.high = query.getdouble(3);
						bs.low = query.getdouble(4);
						bs.close = query.getdouble(5);
						bs.settle = query.getdouble(6);
						bs.vol = query.getuint(7);
						bs.money = query.getdouble(8);
						bs.hold = query.getuint(9);
						bs.add = query.getdouble(10);
						idx++;
					}

					if (period != KP_DAY)
						lastHotTime = hotAy->at(barcnt - 1).time;
					else
						lastHotTime = hotAy->at(barcnt - 1).date;
				}

				WTSLogger::info("主力合约%s历史%s数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
			}

			break;
		}

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
				WTSLogger::error("历史K线读取失败：%s", query.errormsg());
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
						bs.date = query.getuint(0);
						bs.time = query.getuint(1);
						bs.open = query.getdouble(2);
						bs.high = query.getdouble(3);
						bs.low = query.getdouble(4);
						bs.close = query.getdouble(5);
						bs.settle = query.getdouble(6);
						bs.vol = query.getuint(7);
						bs.money = query.getdouble(8);
						bs.hold = query.getuint(9);
						bs.add = query.getdouble(10);
						idx++;
					}

					realCnt += barcnt;

					barsSections.push_back(tempAy);
				}

				if (bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.push_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if (cInfo._exright && cInfo._category == CC_Stock)//如果是读取股票复权数据
	{
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastQTime = 0;

		do
		{
			//先直接读取复权过的历史数据，路径如/his/day/sse/SH600000Q.dsb
			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%sQ' ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, cInfo._code);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%sQ' ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._code);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				WTSLogger::error("历史K线读取失败：%s", query.errormsg());
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
						bs.date = query.getuint(0);
						bs.time = query.getuint(1);
						bs.open = query.getdouble(2);
						bs.high = query.getdouble(3);
						bs.low = query.getdouble(4);
						bs.close = query.getdouble(5);
						bs.settle = query.getdouble(6);
						bs.vol = query.getuint(7);
						bs.money = query.getdouble(8);
						bs.hold = query.getuint(9);
						bs.add = query.getdouble(10);
						idx++;
					}

					if (period != KP_DAY)
						lastQTime = hotAy->at(barcnt - 1).time;
					else
						lastQTime = hotAy->at(barcnt - 1).date;
				}

				WTSLogger::error("股票%s历史%s复权数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
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
				WTSLogger::error("历史K线读取失败：%s", query.errormsg());
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
						bs.date = query.getuint(0);
						bs.time = query.getuint(1);
						bs.open = query.getdouble(2);
						bs.high = query.getdouble(3);
						bs.low = query.getdouble(4);
						bs.close = query.getdouble(5);
						bs.settle = query.getdouble(6);
						bs.vol = query.getuint(7);
						bs.money = query.getdouble(8);
						bs.hold = query.getuint(9);
						bs.add = query.getdouble(10);
						idx++;
					}

					realCnt += barcnt;

					auto& ayFactors = getAdjFactors(cInfo._code, cInfo._exchg);
					if (!ayFactors.empty())
					{
						//做前复权处理
						int32_t lastIdx = barcnt;
						WTSBarStruct bar;
						WTSBarStruct* firstBar = tempAy->data();
						for (auto& adjFact : ayFactors)
						{
							bar.date = adjFact._date;
							double factor = adjFact._factor;

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
									pBar->open /= factor;
									pBar->high /= factor;
									pBar->low /= factor;
									pBar->close /= factor;

									pBar++;
									curIdx++;
								}
								lastIdx = endBar - firstBar;
							}

							if (lastIdx == 0)
								break;
						}
					}

					barsSections.push_back(tempAy);
				}
			}

		} while (false);

		if (hotAy)
		{
			barsSections.push_back(hotAy);
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
			WTSLogger::error("历史K线读取失败：%s", query.errormsg());
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
					bs.date = query.getuint(0);
					bs.time = query.getuint(1);
					bs.open = query.getdouble(2);
					bs.high = query.getdouble(3);
					bs.low = query.getdouble(4);
					bs.close = query.getdouble(5);
					bs.settle = query.getdouble(6);
					bs.vol = query.getuint(7);
					bs.money = query.getdouble(8);
					bs.hold = query.getuint(9);
					bs.add = query.getdouble(10);
					idx++;
				}

				realCnt += barcnt;

				barsSections.push_back(tempAy);
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

	WTSLogger::info("合约%s的历史%s数据已缓存%u条", stdCode, pname.c_str(), realCnt);
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
	if (cInfo._hot && cInfo._category == CC_Future)//如果是读取期货主力连续数据
	{
		HotSections secs;
		if (!_hot_mgr.splitHotSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
			return false;

		if (secs.empty())
			return false;

		//先按照HOT代码进行读取, 如rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastHotTime = 0;
		for (;;)
		{
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._exchg << "." << cInfo._product << "_HOT.dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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

			WTSLogger::info("主力%s历史%s数据直接缓存%u条", stdCode, pname.c_str(), barcnt);

			break;
		}

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
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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
						WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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

				barsSections.push_back(tempAy);

				if (bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.push_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if (cInfo._exright && cInfo._category == CC_Stock)//如果是读取股票复权数据
	{
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastQTime = 0;

		do
		{
			//先直接读取复权过的历史数据，路径如/his/day/sse/SH600000Q.dsb
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._code << "Q.dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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

			WTSLogger::info("%s历史%s复权数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
			break;
		} while (false);

		bool bAllCovered = false;
		do
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
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
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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
						WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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
						//做前复权处理

						int32_t lastIdx = curCnt;
						WTSBarStruct bar;
						firstBar = tempAy->data();
						for (auto& adjFact : ayFactors)
						{
							bar.date = adjFact._date;
							double factor = adjFact._factor;

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
									pBar->open /= factor;
									pBar->high /= factor;
									pBar->low /= factor;
									pBar->close /= factor;

									pBar++;
									curIdx++;
								}
								lastIdx = endBar - firstBar;
							}

							if(lastIdx == 0)
								break;
						}
					}

					barsSections.push_back(tempAy);
				}
			}
		} while (false);

		if (hotAy)
		{
			barsSections.push_back(hotAy);
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
				WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
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

				barsSections.push_back(tempAy);
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

	WTSLogger::info("%s的历史%s数据已缓存%u条", stdCode, pname.c_str(), realCnt);
	return true;
}