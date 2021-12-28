#include "WtDataWriterAD.h"
#include "LMDBKeys.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/BoostFile.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/IniHelper.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Includes/IBaseDataMgr.h"

#include "../WTSTools/WTSCmpHelper.hpp"

#include <set>

extern "C"
{
	EXPORT_FLAG IDataWriter* createWriter()
	{
		IDataWriter* ret = new WtDataWriterAD();
		return ret;
	}

	EXPORT_FLAG void deleteWriter(IDataWriter* &writer)
	{
		if (writer != NULL)
		{
			delete writer;
			writer = NULL;
		}
	}
};

static const uint32_t CACHE_SIZE_STEP = 400;


WtDataWriterAD::WtDataWriterAD()
	: _terminated(false)
	, _log_group_size(1000)
	, _disable_day(false)
	, _disable_min1(false)
	, _disable_min5(false)
	, _disable_tick(false)
	, _tick_cache_block(nullptr)
	, _m1_cache_block(nullptr)
	, _m5_cache_block(nullptr)
{
}


WtDataWriterAD::~WtDataWriterAD()
{
}

bool WtDataWriterAD::init(WTSVariant* params, IDataWriterSink* sink)
{
	IDataWriter::init(params, sink);

	_bd_mgr = sink->getBDMgr();

	_base_dir = StrUtil::standardisePath(params->getCString("path"));
	if (!BoostFile::exists(_base_dir.c_str()))
		BoostFile::create_directories(_base_dir.c_str());

	_cache_file_tick = "cache_tick.dmb";
	_cache_file_m1 = "cache_m1.dmb";
	_cache_file_m5 = "cache_m5.dmb";

	_log_group_size = params->getUInt32("groupsize");

	_disable_tick = params->getBoolean("disabletick");
	_disable_min1 = params->getBoolean("disablemin1");
	_disable_min5 = params->getBoolean("disablemin5");
	_disable_day = params->getBoolean("disableday");

	loadCache();

	return true;
}

void WtDataWriterAD::release()
{
	_terminated = true;
	if (_task_thrd)
	{
		_task_cond.notify_all();
		_task_thrd->join();
	}
}

void WtDataWriterAD::loadCache()
{
	if (_tick_cache_file == NULL)
	{
		bool bNew = false;
		std::string filename = _base_dir + _cache_file_tick;
		if (!BoostFile::exists(filename.c_str()))
		{
			uint64_t uSize = sizeof(RTTickCache) + sizeof(TickCacheItem) * CACHE_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();
			bNew = true;
		}

		_tick_cache_file.reset(new BoostMappingFile);
		_tick_cache_file->map(filename.c_str());
		_tick_cache_block = (RTTickCache*)_tick_cache_file->addr();

		_tick_cache_block->_size = min(_tick_cache_block->_size, _tick_cache_block->_capacity);

		if (bNew)
		{
			memset(_tick_cache_block, 0, _tick_cache_file->size());

			_tick_cache_block->_capacity = CACHE_SIZE_STEP;
			_tick_cache_block->_type = BT_RT_Cache;
			_tick_cache_block->_size = 0;
			_tick_cache_block->_version = 1;
			strcpy(_tick_cache_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			for (uint32_t i = 0; i < _tick_cache_block->_size; i++)
			{
				const TickCacheItem& item = _tick_cache_block->_items[i];
				std::string key = StrUtil::printf("%s.%s", item._tick.exchg, item._tick.code);
				_tick_cache_idx[key] = i;
			}
		}
	}

	if (_m1_cache_file == NULL)
	{
		bool bNew = false;
		std::string filename = _base_dir + _cache_file_m1;
		if (!BoostFile::exists(filename.c_str()))
		{
			uint64_t uSize = sizeof(RTBarCache) + sizeof(BarCacheItem) * CACHE_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();
			bNew = true;
		}

		_m1_cache_file.reset(new BoostMappingFile);
		_m1_cache_file->map(filename.c_str());
		_m1_cache_block = (RTBarCache*)_m1_cache_file->addr();

		_m1_cache_block->_size = min(_m1_cache_block->_size, _m1_cache_block->_capacity);

		if (bNew)
		{
			memset(_m1_cache_block, 0, _m1_cache_file->size());

			_m1_cache_block->_capacity = CACHE_SIZE_STEP;
			_m1_cache_block->_type = BT_RT_Cache;
			_m1_cache_block->_size = 0;
			_m1_cache_block->_version = 1;
			strcpy(_m1_cache_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			for (uint32_t i = 0; i < _m1_cache_block->_size; i++)
			{
				const BarCacheItem& item = _m1_cache_block->_items[i];
				std::string key = StrUtil::printf("%s.%s", item._exchg, item._code);
				_m1_cache_idx[key] = i;
			}
		}
	}

	if (_m5_cache_file == NULL)
	{
		bool bNew = false;
		std::string filename = _base_dir + _cache_file_m5;
		if (!BoostFile::exists(filename.c_str()))
		{
			uint64_t uSize = sizeof(RTBarCache) + sizeof(BarCacheItem) * CACHE_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();
			bNew = true;
		}

		_m5_cache_file.reset(new BoostMappingFile);
		_m5_cache_file->map(filename.c_str());
		_m5_cache_block = (RTBarCache*)_m5_cache_file->addr();

		_m5_cache_block->_size = min(_m5_cache_block->_size, _m5_cache_block->_capacity);

		if (bNew)
		{
			memset(_m1_cache_block, 0, _m5_cache_file->size());

			_m5_cache_block->_capacity = CACHE_SIZE_STEP;
			_m5_cache_block->_type = BT_RT_Cache;
			_m5_cache_block->_size = 0;
			_m5_cache_block->_version = 1;
			strcpy(_m5_cache_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			for (uint32_t i = 0; i < _m5_cache_block->_size; i++)
			{
				const BarCacheItem& item = _m5_cache_block->_items[i];
				std::string key = StrUtil::printf("%s.%s", item._exchg, item._code);
				_m5_cache_idx[key] = i;
			}
		}
	}
}

template<typename HeaderType, typename T>
void* WtDataWriterAD::resizeRTBlock(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	//调用该函数之前,应该已经申请了写锁了
	RTBlockHeader* tBlock = (RTBlockHeader*)mfPtr->addr();
	if (tBlock->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uOldSize = sizeof(HeaderType) + sizeof(T)*tBlock->_capacity;
	uint64_t uNewSize = sizeof(HeaderType) + sizeof(T)*nCount;
	std::string data;
	data.resize((std::size_t)(uNewSize - uOldSize), 0);
	try
	{
		BoostFile f;
		f.open_existing_file(filename);
		f.seek_to_end();
		f.write_file(data.c_str(), data.size());
		f.close_file();
	}
	catch(std::exception& ex)
	{
		_sink->outputWriterLog(LL_ERROR, "Exception occured while expanding RT cache file of %s[%u]: %s", filename, uNewSize, ex.what());
		return mfPtr->addr();
	}

	BoostMappingFile* pNewMf = new BoostMappingFile();
	if (!pNewMf->map(filename))
	{
		delete pNewMf;
		return NULL;
	}

	mfPtr.reset(pNewMf);

	tBlock = (RTBlockHeader*)mfPtr->addr();
	tBlock->_capacity = nCount;
	return mfPtr->addr();
}

bool WtDataWriterAD::writeTick(WTSTickData* curTick, bool bNeedSlice /* = true */)
{
	if (curTick == NULL)
		return false;

	curTick->retain();
	pushTask([this, curTick, bNeedSlice](){

		do
		{
			WTSContractInfo* ct = _bd_mgr->getContract(curTick->code(), curTick->exchg());
			if(ct == NULL)
				break;

			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			//先更新缓存
			if (!updateTickCache(ct, curTick, bNeedSlice))
				break;

			//写到tick缓存
			if(!_disable_tick)
				pipeToTicks(ct, curTick);

			//写到K线缓存
			updateBarCache(ct, curTick);

			_sink->broadcastTick(curTick);

			static faster_hashmap<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTick->exchg()]++;
			if (_tcnt_map[curTick->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "%s ticks received from exchange %s", StrUtil::fmtUInt64(_tcnt_map[curTick->exchg()]).c_str(), curTick->exchg());
			}
		} while (false);

		curTick->release();
	});
	return true;
}

void WtDataWriterAD::pushTask(TaskInfo task)
{
	if(_async_task)
	{
		StdUniqueLock lck(_task_mtx);
		_tasks.push(task);
		_task_cond.notify_all();
	}
	else
	{
		task();
		return;
	}

	if(_task_thrd == NULL)
	{
		_task_thrd.reset(new StdThread([this](){
			while (!_terminated)
			{
				if(_tasks.empty())
				{
					StdUniqueLock lck(_task_mtx);
					_task_cond.wait(_task_mtx);
					continue;
				}

				std::queue<TaskInfo> tempQueue;
				{
					StdUniqueLock lck(_task_mtx);
					tempQueue.swap(_tasks);
				}

				while(!tempQueue.empty())
				{
					TaskInfo& curTask = tempQueue.front();
					curTask();
					tempQueue.pop();
				}
			}
		}));
	}
}

void WtDataWriterAD::pipeToTicks(WTSContractInfo* ct, WTSTickData* curTick)
{
	//直接落地
	const char* key = ct->getFullCode();

	WtLMDBPtr db = get_t_db(ct->getExchg(), ct->getCode());
	if (db)
	{
		LMDBHftKey key(ct->getExchg(), ct->getCode(), curTick->actiondate(), curTick->actiontime());
		WtLMDBQuery query(*db);
		if (!query.put((void*)&key, sizeof(key), &curTick->getTickStruct(), sizeof(WTSTickStruct)))
		{
			_sink->outputWriterLog(LL_ERROR, "pipe tick of %s to db failed", ct->getFullCode());
		}
	}

	for(auto& item : _dumpers)
	{
		const char* id = item.first.c_str();
		IHisDataDumper* dumper = item.second;
		if (dumper == NULL)
			continue;

		bool bSucc = dumper->dumpHisTicks(key, curTick->tradingdate(), &curTick->getTickStruct(), 1);
		if (!bSucc)
		{
			_sink->outputWriterLog(LL_ERROR, "pipe tick data of %s via extended dumper %s failed", ct->getFullCode(), id);
		}
	}
}

void WtDataWriterAD::pipeToDayBars(WTSContractInfo* ct, const WTSTickStruct& ts)
{
	//直接落地
	const char* key = ct->getFullCode();

	WTSBarStruct bar;
	bar.date = ts.trading_date;
	bar.time = 0;
	bar.open = ts.open;
	bar.close = ts.price;
	bar.high = ts.high;
	bar.low = ts.low;
	bar.settle = ts.settle_price;
	bar.vol = ts.total_volume;
	bar.hold = ts.open_interest;
	bar.money = ts.total_turnover;
	bar.add = ts.open_interest - ts.pre_interest;

	WtLMDBPtr db = get_k_db(ct->getExchg(), KP_DAY);
	if (db)
	{
		LMDBBarKey key(ct->getExchg(), ct->getCode(), bar.date);
		WtLMDBQuery query(*db);
		if (!query.put((void*)&key, sizeof(key), (void*)&bar, sizeof(bar)))
		{
			_sink->outputWriterLog(LL_ERROR, "pipe day bar @ %u of %s to db failed", bar.date, ct->getFullCode());
		}
	}

	for (auto& item : _dumpers)
	{
		const char* id = item.first.c_str();
		IHisDataDumper* dumper = item.second;
		if (dumper == NULL)
			continue;

		bool bSucc = dumper->dumpHisBars(key, "d1", &bar, 1);
		if (!bSucc)
		{
			_sink->outputWriterLog(LL_ERROR, "pipe day bar @%u of %s via extended dumper %s failed", bar.date, ct->getFullCode(), id);
		}
	}
}

void WtDataWriterAD::pipeToM1Bars(WTSContractInfo* ct, const WTSBarStruct& bar)
{
	//直接落地
	const char* key = ct->getFullCode();

	WtLMDBPtr db = get_k_db(ct->getExchg(), KP_Minute1);
	if(db)
	{
		LMDBBarKey key(ct->getExchg(), ct->getCode(), bar.time);
		WtLMDBQuery query(*db);
		if(!query.put((void*)&key, sizeof(key), (void*)&bar, sizeof(bar)))
		{
			_sink->outputWriterLog(LL_ERROR, "pipe m1 bar @ %u of %s to db failed", bar.time, ct->getFullCode());
		}
	}

	for (auto& item : _dumpers)
	{
		const char* id = item.first.c_str();
		IHisDataDumper* dumper = item.second;
		if (dumper == NULL)
			continue;

		bool bSucc = dumper->dumpHisBars(key, "m1", (WTSBarStruct*)&bar, 1);
		if (!bSucc)
		{
			_sink->outputWriterLog(LL_ERROR, "pipe m1 bar @%u of %s via extended dumper %s failed", bar.time, ct->getFullCode(), id);
		}
	}
}

void WtDataWriterAD::pipeToM5Bars(WTSContractInfo* ct, const WTSBarStruct& bar)
{
	//直接落地
	const char* key = ct->getFullCode();

	WtLMDBPtr db = get_k_db(ct->getExchg(), KP_Minute5);
	if (db)
	{
		LMDBBarKey key(ct->getExchg(), ct->getCode(), bar.time);
		WtLMDBQuery query(*db);
		if (!query.put((void*)&key, sizeof(key), (void*)&bar, sizeof(bar)))
		{
			_sink->outputWriterLog(LL_ERROR, "pipe m5 bar @ %u of %s to db failed", bar.time, ct->getFullCode());
		}
	}

	for (auto& item : _dumpers)
	{
		const char* id = item.first.c_str();
		IHisDataDumper* dumper = item.second;
		if (dumper == NULL)
			continue;

		bool bSucc = dumper->dumpHisBars(key, "m5", (WTSBarStruct*)&bar, 1);
		if (!bSucc)
		{
			_sink->outputWriterLog(LL_ERROR, "pipe m5 bar of %s via extended dumper %s failed", ct->getFullCode(), id);
		}
	}
}

void WtDataWriterAD::updateBarCache(WTSContractInfo* ct, WTSTickData* curTick)
{
	uint32_t uDate = curTick->actiondate();
	WTSSessionInfo* sInfo = _bd_mgr->getSessionByCode(curTick->code(), curTick->exchg());
	uint32_t curTime = curTick->actiontime() / 100000;

	uint32_t minutes = sInfo->timeToMinutes(curTime, false);
	if (minutes == INVALID_UINT32)
		return;

	//当秒数为0,要专门处理,比如091500000,这笔tick要算作0915的
	//如果是小节结束,要算作小节结束那一分钟,因为经常会有超过结束时间的价格进来,如113000500
	//不能同时处理,所以用or	
	if (sInfo->isLastOfSection(curTime))
	{
		minutes--;
	}

	std::string key = StrUtil::printf("%s.%s", curTick->exchg(), curTick->code());

	//更新1分钟线
	if (!_disable_min1 && _m1_cache_block)
	{
		StdUniqueLock lock(_mtx_m1_cache);
		uint32_t idx = 0;
		bool bNewCode = false;
		if (_m1_cache_idx.find(key) == _m1_cache_idx.end())
		{
			idx = _m1_cache_block->_size;
			_m1_cache_idx[key] = _m1_cache_block->_size;
			_m1_cache_block->_size += 1;
			if (_m1_cache_block->_size >= _m1_cache_block->_capacity)
			{
				_m1_cache_block = (RTBarCache*)resizeRTBlock<RTBarCache, BarCacheItem>(_m1_cache_file, _m1_cache_block->_capacity + CACHE_SIZE_STEP);
				_sink->outputWriterLog(LL_INFO, "m1 cache resized to %u items", _m1_cache_block->_capacity);
			}
			bNewCode = true;
		}
		else
		{
			idx = _m1_cache_idx[key];
		}

		BarCacheItem& item = (BarCacheItem&)_m1_cache_block->_items[idx];
		if(bNewCode)
		{
			strcpy(item._exchg, curTick->exchg());
			strcpy(item._code, curTick->code());
		}
		WTSBarStruct* lastBar = &item._bar;


		//拼接1分钟线
		uint32_t barMins = minutes + 1;
		uint32_t barTime = sInfo->minuteToTime(barMins);
		uint32_t barDate = uDate;
		if (barTime == 0)
		{
			barDate = TimeUtils::getNextDate(barDate);
		}
		barTime = TimeUtils::timeToMinBar(barDate, barTime);

		bool bNewBar = false;
		if (lastBar == NULL || barTime > lastBar->time)
		{
			bNewBar = true;
		}

		WTSBarStruct* newBar = lastBar;
		if (bNewBar)
		{
			//这里要将lastBar往外写
			//如果是新的合约，说明还没数据，不往外写
			if (!bNewCode)
			{
				pipeToM1Bars(ct, *lastBar);

				uint32_t lastMins = sInfo->timeToMinutes(lastBar->time % 10000, false);
				if(lastMins > barMins)
				{
					//如果上一条K线的分钟数大于当前K线的分钟数
					//说明交易日换了
					//需要保存日线了
				}
			}

			newBar->date = curTick->tradingdate();
			newBar->time = barTime;
			newBar->open = curTick->price();
			newBar->high = curTick->price();
			newBar->low = curTick->price();
			newBar->close = curTick->price();

			newBar->vol = curTick->volume();
			newBar->money = curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add = curTick->additional();
		}
		else
		{
			newBar->close = curTick->price();
			newBar->high = max(curTick->price(), newBar->high);
			newBar->low = min(curTick->price(), newBar->low);

			newBar->vol += curTick->volume();
			newBar->money += curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add += curTick->additional();
		}
	}

	//更新5分钟线
	if (!_disable_min5 && _m5_cache_block)
	{
		StdUniqueLock lock(_mtx_m5_cache);
		uint32_t idx = 0;
		bool bNewCode = false;
		if (_m5_cache_idx.find(key) == _m5_cache_idx.end())
		{
			idx = _m5_cache_block->_size;
			_m5_cache_idx[key] = _m5_cache_block->_size;
			_m5_cache_block->_size += 1;
			if (_m5_cache_block->_size >= _m5_cache_block->_capacity)
			{
				_m5_cache_block = (RTBarCache*)resizeRTBlock<RTBarCache, BarCacheItem>(_m5_cache_file, _m5_cache_block->_capacity + CACHE_SIZE_STEP);
				_sink->outputWriterLog(LL_INFO, "m1 cache resized to %u items", _m5_cache_block->_capacity);
			}
			bNewCode = true;
		}
		else
		{
			idx = _m5_cache_idx[key];
		}

		BarCacheItem& item = (BarCacheItem&)_m5_cache_block->_items[idx];
		if (bNewCode)
		{
			strcpy(item._exchg, curTick->exchg());
			strcpy(item._code, curTick->code());
		}
		WTSBarStruct* lastBar = &item._bar;

		//拼接5分钟线
		uint32_t barMins = (minutes / 5) * 5 + 5;
		uint32_t barTime = sInfo->minuteToTime(barMins);
		uint32_t barDate = uDate;
		if (barTime == 0)
		{
			barDate = TimeUtils::getNextDate(barDate);
		}
		barTime = TimeUtils::timeToMinBar(barDate, barTime);

		bool bNewBar = false;
		if (lastBar == NULL || barTime > lastBar->time)
		{
			bNewBar = true;
		}

		WTSBarStruct* newBar = lastBar;
		if (bNewBar)
		{
			//这里要将lastBar往外写
			//如果是新的合约，说明还没数据，不往外写
			if(!bNewCode)
				pipeToM5Bars(ct, *lastBar);

			newBar->date = curTick->tradingdate();
			newBar->time = barTime;
			newBar->open = curTick->price();
			newBar->high = curTick->price();
			newBar->low = curTick->price();
			newBar->close = curTick->price();

			newBar->vol = curTick->volume();
			newBar->money = curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add = curTick->additional();
		}
		else
		{
			newBar->close = curTick->price();
			newBar->high = max(curTick->price(), newBar->high);
			newBar->low = min(curTick->price(), newBar->low);

			newBar->vol += curTick->volume();
			newBar->money += curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add += curTick->additional();
		}
	}
}

WTSTickData* WtDataWriterAD::getCurTick(const char* code, const char* exchg/* = ""*/)
{
	if (strlen(code) == 0)
		return NULL;

	WTSContractInfo* ct = _bd_mgr->getContract(code, exchg);
	if (ct == NULL)
		return NULL;

	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	StdUniqueLock lock(_mtx_tick_cache);
	auto it = _tick_cache_idx.find(key);
	if (it == _tick_cache_idx.end())
		return NULL;

	uint32_t idx = it->second;
	TickCacheItem& item = _tick_cache_block->_items[idx];
	return WTSTickData::create(item._tick);
}

bool WtDataWriterAD::updateTickCache(WTSContractInfo* ct, WTSTickData* curTick, bool bNeedSlice /* = true */)
{
	if (curTick == NULL || _tick_cache_block == NULL)
	{
		_sink->outputWriterLog(LL_ERROR, "Tick cache data not initialized");
		return false;
	}

	StdUniqueLock lock(_mtx_tick_cache);
	std::string key = StrUtil::printf("%s.%s", curTick->exchg(), curTick->code());
	uint32_t idx = 0;
	if (_tick_cache_idx.find(key) == _tick_cache_idx.end())
	{
		idx = _tick_cache_block->_size;
		_tick_cache_idx[key] = _tick_cache_block->_size;
		_tick_cache_block->_size += 1;
		if(_tick_cache_block->_size >= _tick_cache_block->_capacity)
		{
			_tick_cache_block = (RTTickCache*)resizeRTBlock<RTTickCache, TickCacheItem>(_tick_cache_file, _tick_cache_block->_capacity + CACHE_SIZE_STEP);
			_sink->outputWriterLog(LL_INFO, "Tick Cache resized to %u items", _tick_cache_block->_capacity);
		}
	}
	else
	{
		idx = _tick_cache_idx[key];
	}


	TickCacheItem& item = _tick_cache_block->_items[idx];
	if (curTick->tradingdate() < item._date)
	{
		_sink->outputWriterLog(LL_INFO, "Tradingday[%u] of %s is less than cached tradingday[%u]", curTick->tradingdate(), curTick->code(), item._date);
		return false;
	}

	WTSTickStruct& newTick = curTick->getTickStruct();

	if (curTick->tradingdate() > item._date)
	{
		//新数据交易日大于老数据,则认为是新一天的数据
		//把老的tick转储为日线
		pipeToDayBars(ct, item._tick);

		item._date = curTick->tradingdate();
		memcpy(&item._tick, &newTick, sizeof(WTSTickStruct));
		if (bNeedSlice)
		{
			item._tick.volume = item._tick.total_volume;
			item._tick.turn_over = item._tick.total_turnover;
			item._tick.diff_interest = item._tick.open_interest - item._tick.pre_interest;

			newTick.volume = newTick.total_volume;
			newTick.turn_over = newTick.total_turnover;
			newTick.diff_interest = newTick.open_interest - newTick.pre_interest;
		}

		//	newTick.trading_date, curTick->exchg(), curTick->code(), curTick->volume(),
		//	curTick->turnover(), curTick->openinterest(), curTick->additional());
		_sink->outputWriterLog(LL_INFO, "First tick of new tradingday %u,%s.%s,%f,%u,%f,%u,%d", 
			newTick.trading_date, curTick->exchg(), curTick->code(), curTick->price(), curTick->volume(),
			curTick->turnover(), curTick->openinterest(), curTick->additional());
	}
	else
	{
		//如果缓存里的数据日期大于最新行情的日期
		//或者缓存里的时间大于等于最新行情的时间,数据就不需要处理
		WTSSessionInfo* sInfo = _bd_mgr->getSessionByCode(curTick->code(), curTick->exchg());
		uint32_t tdate = sInfo->getOffsetDate(curTick->actiondate(), curTick->actiontime() / 100000);
		if (tdate > curTick->tradingdate())
		{
			_sink->outputWriterLog(LL_ERROR, "Last tick of %s.%s with time %u.%u has an exception, abandoned", curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime());
			return false;
		}
		else if (curTick->totalvolume() < item._tick.total_volume)
		{
			_sink->outputWriterLog(LL_ERROR, "Last tick of %s.%s with time %u.%u, volume %u is less than cached volume %u, abandoned", 
				curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime(), curTick->totalvolume(), item._tick.total_volume);
			return false;
		}

		//时间戳相同,但是成交量大于等于原来的,这种情况一般是郑商所,这里的处理方式就是时间戳+200毫秒
		//By Wesley @ 2021.12.21
		//今天发现居然一秒出现了4笔，实在是有点无语
		//只能把500毫秒的变化量改成200，并且改成发生时间小于等于上一笔的判断
		if(newTick.action_date == item._tick.action_date && newTick.action_time <= item._tick.action_time && newTick.total_volume >= item._tick.total_volume)
		{
			newTick.action_time += 200;
		}

		//这里就要看需不需要预处理了
		if(!bNeedSlice)
		{
			memcpy(&item._tick, &newTick, sizeof(WTSTickStruct));
		}
		else
		{
			newTick.volume = newTick.total_volume - item._tick.total_volume;
			newTick.turn_over = newTick.total_turnover - item._tick.total_turnover;
			newTick.diff_interest = newTick.open_interest - item._tick.open_interest;

			memcpy(&item._tick, &newTick, sizeof(WTSTickStruct));
		}
	}

	return true;
}

WtDataWriterAD::WtLMDBPtr WtDataWriterAD::get_k_db(const char* exchg, WTSKlinePeriod period)
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

	WtLMDBPtr dbPtr = std::move(std::make_shared<WtLMDB>(new WtLMDB(false)));
	std::string path = StrUtil::printf("%s/%s/%s/", _base_dir.c_str(), subdir.c_str(), exchg);
	if(!dbPtr->open(path.c_str()))
	{
		if (_sink) _sink->outputWriterLog(LL_ERROR, "Opening %s db at %s failed: %s", subdir.c_str(), path.c_str(), dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}

	(*the_map)[exchg] = dbPtr;
	return std::move(dbPtr);
}

WtDataWriterAD::WtLMDBPtr WtDataWriterAD::get_t_db(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);
	auto it = _tick_dbs.find(key);
	if (it != _tick_dbs.end())
		return std::move(it->second);

	WtLMDBPtr dbPtr = std::move(std::make_shared<WtLMDB>(new WtLMDB(false)));
	std::string path = StrUtil::printf("%s/ticks/%s/%s", _base_dir.c_str(), exchg, code);
	if (!dbPtr->open(path.c_str()))
	{
		if (_sink) _sink->outputWriterLog(LL_ERROR, "Opening tick db at %s failed: %s", path.c_str(), dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}

	_tick_dbs[exchg] = dbPtr;
	return std::move(dbPtr);
}