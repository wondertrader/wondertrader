#include "WtRdmDtReaderAD.h"
#include "LMDBKeys.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSDataDef.hpp"

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void pipe_rdmreader_log(IRdmDtReaderSink* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	memset(buffer, 0, 512);
	fmt::format_to(buffer, format, args...);

	sink->reader_log(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG IRdmDtReader* createRdmDtReader()
	{
		IRdmDtReader* ret = new WtRdmDtReaderAD();
		return ret;
	}

	EXPORT_FLAG void deleteRdmDtReader(IRdmDtReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};


WtRdmDtReaderAD::WtRdmDtReaderAD()
	: _base_data_mgr(NULL)
	, _hot_mgr(NULL)
{
}


WtRdmDtReaderAD::~WtRdmDtReaderAD()
{
}

void WtRdmDtReaderAD::init(WTSVariant* cfg, IRdmDtReaderSink* sink)
{
	IRdmDtReader::init(cfg, sink);

	_base_data_mgr = sink->get_basedata_mgr();
	_hot_mgr = sink->get_hot_mgr();

	if (cfg == NULL)
		return;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);

	pipe_rdmreader_log(sink, LL_INFO, "WtRdmDtReaderAD initialized, root data folder is {}", _base_dir);
}

WTSTickSlice* WtRdmDtReaderAD::readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	//TODO: 以后再来实现吧
	return NULL;
}

WTSTickSlice* WtRdmDtReaderAD::readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	WTSSessionInfo* sInfo = commInfo->getSessionInfo();
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t rDate, rTime, rSecs;
	//20190807124533900
	rDate = (uint32_t)(etime / 1000000000);
	rTime = sInfo->offsetTime((uint32_t)(etime % 1000000000) / 100000, false);
	rSecs = (uint32_t)(etime % 100000);

	uint32_t lDate, lTime, lSecs;
	//20190807124533900
	lDate = (uint32_t)(stime / 1000000000);
	lTime = sInfo->offsetTime((uint32_t)(stime % 1000000000) / 100000, true);
	lSecs = (uint32_t)(stime % 100000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t beginTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), lDate, lTime, false);

	std::string key = stdCode;

	//先检查缓存
	TicksList& tickList = _ticks_cache[key];

	bool isEmpty = tickList._ticks.empty();
	bool bNeedOlder = stime < tickList._first_tick_time;
	bool bNeedNewer = etime > tickList._last_tick_time;

	//这里要改成从lmdb读取
	WtLMDBPtr db = get_t_db(cInfo._exchg, cInfo._code);
	if (db == NULL)
		return NULL;

	if (isEmpty)
	{
		//按照区间加载即可
		WtLMDBQuery query(*db);
		LMDBHftKey lKey(cInfo._exchg, cInfo._code, beginTDate, lTime * 100000 + lSecs);
		LMDBHftKey rKey(cInfo._exchg, cInfo._code, endTDate, rTime * 100000 + rSecs);
		int cnt = query.get_range(std::string((const char*)&lKey, sizeof(lKey)),
			std::string((const char*)&rKey, sizeof(rKey)), [this, &tickList](const ValueArray& ayKeys, const ValueArray& ayVals) {
			tickList._ticks.resize(ayVals.size());
			std::size_t idx = 0;
			for (const std::string& item : ayVals)
			{
				memcpy(&tickList._ticks[idx], item.data(), item.size());
				idx++;
			}
		});

		if (cnt > 0)
		{
			WTSTickStruct& curTs = tickList._ticks.front();
			tickList._first_tick_time = (uint64_t)curTs.trading_date * 1000000000 + sInfo->offsetTime(curTs.action_time / 100000, true) + curTs.action_time % 100000;

			curTs = tickList._ticks.back();
			tickList._last_tick_time = (uint64_t)curTs.trading_date * 1000000000 + sInfo->offsetTime(curTs.action_time / 100000, false) + curTs.action_time % 100000;

			pipe_rdmreader_log(_sink, LL_DEBUG, "{} ticks between [{},{}] of {} loaded to cache", cnt, tickList._first_tick_time, tickList._last_tick_time, stdCode);
		}
	}
	else
	{
		if (bNeedOlder)
		{
			//读取更早的数据
			WtLMDBQuery query(*db);
			LMDBHftKey rKey(cInfo._exchg, cInfo._code, (uint32_t)(tickList._first_tick_time / 1000000000), (uint32_t)(tickList._first_tick_time % 1000000000));
			LMDBHftKey lKey(cInfo._exchg, cInfo._code, beginTDate, lTime * 100000 + lSecs);
			int cnt = query.get_range(std::string((const char*)&lKey, sizeof(lKey)), std::string((const char*)&rKey, sizeof(rKey)),
				[this, &tickList](const ValueArray& ayKeys, const ValueArray& ayVals) {
				std::vector<WTSTickStruct> ayTicks;
				ayTicks.resize(ayVals.size() + tickList._ticks.size());
				std::size_t idx = 0;
				for (const std::string& item : ayVals)
				{
					WTSTickStruct* curTick = (WTSTickStruct*)item.data();
					memcpy(&ayTicks[idx], item.data(), item.size());
					idx++;
				}

				//将原来的数据拷贝到后面，再做一个swap即可
				memcpy(&ayTicks[idx], tickList._ticks.data(), sizeof(WTSTickStruct)*tickList._ticks.size());
				tickList._ticks.swap(ayTicks);
			});

			if(cnt > 0)
			{
				const WTSTickStruct& curTs = tickList._ticks.front();
				tickList._first_tick_time = (uint64_t)curTs.trading_date * 1000000000 + sInfo->offsetTime(curTs.action_time / 100000, false) + curTs.action_time % 100000;

				pipe_rdmreader_log(_sink, LL_DEBUG, "{} prev ticks of {} loaded to cache", cnt, stdCode);
			}
		}

		if (bNeedNewer)
		{
			//读取更新的数据
			WtLMDBQuery query(*db);
			LMDBHftKey lKey(cInfo._exchg, cInfo._code, (uint32_t)(tickList._last_tick_time / 1000000000), (uint32_t)(tickList._last_tick_time % 1000000000));
			LMDBHftKey rKey(cInfo._exchg, cInfo._code, endTDate, rTime * 100000 + rSecs);
			int cnt = query.get_range(std::string((const char*)&lKey, sizeof(lKey)), std::string((const char*)&rKey, sizeof(rKey)),
				[this, &tickList](const ValueArray& ayKeys, const ValueArray& ayVals) {
				for (const std::string& item : ayVals)
				{
					WTSTickStruct* curTick = (WTSTickStruct*)item.data();
					tickList._ticks.emplace_back(*curTick);
				}
			});

			if (cnt > 0)
			{
				const WTSTickStruct& curTs = tickList._ticks.back();
				tickList._last_tick_time = (uint64_t)curTs.trading_date * 1000000000 + sInfo->offsetTime(curTs.action_time / 100000, true) + curTs.action_time % 100000;

				pipe_rdmreader_log(_sink, LL_DEBUG, "{} newer ticks of {} loaded to cache", cnt, stdCode);
			}
		}
	}

	//全部读取完成以后，再生成切片
	{
		//比较时间的对象
		WTSTickStruct sTick, eTick;
		sTick.action_date = lDate;
		sTick.action_time = (uint32_t)(stime % 1000000000);
		eTick.action_date = rDate;
		eTick.action_time = (uint32_t)(etime % 1000000000);

		std::size_t cnt = 0;

		WTSTickStruct* pTick = std::lower_bound(&tickList._ticks[0], &tickList._ticks[0] + (tickList._ticks.size() - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		std::size_t eIdx = pTick - &tickList._ticks[0];
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		pTick = &tickList._ticks[0];
		//如果第一条实时K线的时间大于开始日期，则实时K线要全部包含进去
		if (pTick->action_date > sTick.action_date || (pTick->action_date == sTick.action_date && pTick->action_time > sTick.action_time))
		{
			cnt = eIdx + 1;
		}
		else
		{
			pTick = std::lower_bound(&tickList._ticks[0], &tickList._ticks[0] + (tickList._ticks.size() - 1), sTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pTick - &tickList._ticks[0];
			cnt = eIdx - sIdx + 1;
		}

		WTSTickSlice* slice = WTSTickSlice::create(stdCode, pTick, cnt);
		return slice;
	}
}

WTSKlineSlice* WtRdmDtReaderAD::readKlineSliceByCount(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime /* = 0 */)
{
	//TODO: 以后再来实现吧
	return NULL;
}

WTSTickSlice* WtRdmDtReaderAD::readTickSliceByDate(const char* stdCode, uint32_t uDate )
{
	//TODO: 以后再来实现吧
	return NULL;
}

WTSKlineSlice* WtRdmDtReaderAD::readKlineSliceByRange(const char* stdCode, WTSKlinePeriod period, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t rDate, rTime, lDate, lTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);
	lDate = (uint32_t)(stime / 10000);
	lTime = (uint32_t)(stime % 10000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t beginTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), lDate, lTime, false);

	bool isDay = period == KP_DAY;
	//转换成K线时间
	etime = isDay ? endTDate : (etime - 19000000);

	//暂时不考虑HOT之类的，只针对7×24小时品种做一个实现
	std::string key = StrUtil::printf("%s#%u", stdCode, period);
	BarsList& barsList = _bars_cache[key];

	bool bNeedNewer = (etime > barsList._last_bar_time);

	//全部重载
	WtLMDBPtr db = get_k_db(cInfo._exchg, period);
	if (db == NULL)
		return NULL;

	if (barsList._bars.empty())
	{
		pipe_rdmreader_log(_sink, LL_DEBUG, "Reading back {} bars of {}.{}...", PERIOD_NAME[period], cInfo._exchg, cInfo._code);
		WtLMDBQuery query(*db);
		LMDBBarKey rKey(cInfo._exchg, cInfo._code, 0xffffffff);
		LMDBBarKey lKey(cInfo._exchg, cInfo._code, 0);
		int cnt = query.get_range(std::string((const char*)&lKey, sizeof(lKey)), std::string((const char*)&rKey, sizeof(rKey)),
			[this, &barsList, &lKey](const ValueArray& ayKeys, const ValueArray& ayVals) {
			if (ayVals.empty())
				return;

			std::size_t cnt = ayVals.size();
			barsList._bars.resize(cnt);
			std::size_t idx = 0;
			for (const std::string& item : ayVals)
			{
				memcpy(&barsList._bars[idx], item.data(), item.size());
				idx++;
			}
		});
	}
	else if(bNeedNewer)
	{
		//加载更新的数据
		pipe_rdmreader_log(_sink, LL_DEBUG, "Reading back {} bars of {}.{}...", PERIOD_NAME[period], cInfo._exchg, cInfo._code);
		WtLMDBQuery query(*db);
		LMDBBarKey rKey(cInfo._exchg, cInfo._code, 0xffffffff);
		LMDBBarKey lKey(cInfo._exchg, cInfo._code, (uint32_t)barsList._last_bar_time);
		int cnt = query.get_range(std::string((const char*)&lKey, sizeof(lKey)), std::string((const char*)&rKey, sizeof(rKey)),
			[this, &barsList, &lKey](const ValueArray& ayKeys, const ValueArray& ayVals) {
			if (ayVals.empty())
				return;

			std::size_t cnt = ayVals.size();
			barsList._bars.resize(cnt);
			std::size_t idx = 0;
			for (const std::string& item : ayVals)
			{
				memcpy(&barsList._bars[idx], item.data(), item.size());
				idx++;
			}
		});
	}

	//
	{
		WTSBarStruct eBar;
		eBar.date = rDate;
		eBar.time = (rDate - 19900000) * 10000 + rTime;

		WTSBarStruct sBar;
		sBar.date = lDate;
		sBar.time = (lDate - 19900000) * 10000 + lTime;

		WTSBarStruct* pHead = NULL;
		std::size_t cnt = 0;

		WTSBarStruct* pBar = std::lower_bound(&barsList._bars[0], &barsList._bars[0] + (barsList._bars.size() - 1), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});

		std::size_t idx = pBar - &barsList._bars[0];
		if ((isDay && pBar->date > eBar.date) || (!isDay && pBar->time > eBar.time))
		{
			pBar--;
			idx--;
		}

		pBar = &barsList._bars[0];
		//如果第一条实时K线的时间大于开始日期，则实时K线要全部包含进去
		if ((isDay && pBar->date > sBar.date) || (!isDay && pBar->time > sBar.time))
		{
			pHead = pBar;
			cnt = idx + 1;
		}
		else
		{
			pBar = std::lower_bound(&barsList._bars[0], &barsList._bars[0] + (barsList._bars.size() - 1), sBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
				if (isDay)
					return a.date < b.date;
				else
					return a.time < b.time;
			});

			std::size_t sIdx = pBar - &barsList._bars[0];
			pHead = pBar;
			cnt = idx - sIdx + 1;
		}

		WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, 1, pHead, cnt);
		return slice;
	}
}

WtRdmDtReaderAD::WtLMDBPtr WtRdmDtReaderAD::get_k_db(const char* exchg, WTSKlinePeriod period)
{
	WtLMDBMap* the_map = NULL;
	std::string subdir;
	if (period == KP_Minute1)
	{
		the_map = &_exchg_m1_dbs;
		subdir = "min1";
	}
	else if (period == KP_Minute5)
	{
		the_map = &_exchg_m5_dbs;
		subdir = "min5";
	}
	else if (period == KP_DAY)
	{
		the_map = &_exchg_d1_dbs;
		subdir = "day";
	}
	else
		return std::move(WtLMDBPtr());

	auto it = the_map->find(exchg);
	if (it != the_map->end())
		return std::move(it->second);

	WtLMDBPtr dbPtr(new WtLMDB(true));
	std::string path = StrUtil::printf("%s%s/%s/", _base_dir.c_str(), subdir.c_str(), exchg);
	boost::filesystem::create_directories(path);
	if (!dbPtr->open(path.c_str()))
	{
		pipe_rdmreader_log(_sink, LL_ERROR, "Opening {} db if {} failed: {}", subdir, exchg, dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}
	else
	{
		pipe_rdmreader_log(_sink, LL_DEBUG, "{} db of {} opened", subdir, exchg);
	}

	(*the_map)[exchg] = dbPtr;
	return std::move(dbPtr);
}

WtRdmDtReaderAD::WtLMDBPtr WtRdmDtReaderAD::get_t_db(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);
	auto it = _tick_dbs.find(key);
	if (it != _tick_dbs.end())
		return std::move(it->second);

	WtLMDBPtr dbPtr(new WtLMDB(true));
	std::string path = StrUtil::printf("%sticks/%s/%s", _base_dir.c_str(), exchg, code);
	boost::filesystem::create_directories(path);
	if (!dbPtr->open(path.c_str()))
	{
		pipe_rdmreader_log(_sink, LL_ERROR, "Opening tick db of {}.{} failed: {}", exchg, code, dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}
	else
	{
		pipe_rdmreader_log(_sink, LL_DEBUG, "Tick db of {}.{} opened", exchg, code);
	}

	_tick_dbs[exchg] = dbPtr;
	return std::move(dbPtr);
}