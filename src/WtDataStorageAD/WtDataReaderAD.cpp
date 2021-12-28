#include "WtDataReaderAD.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/ModuleHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSDataDef.hpp"

#include "../WTSTools/WTSCmpHelper.hpp"

#include <rapidjson/document.h>
namespace rj = rapidjson;

extern "C"
{
	EXPORT_FLAG IDataReader* createDataReader()
	{
		IDataReader* ret = new WtDataReaderAD();
		return ret;
	}

	EXPORT_FLAG void deleteDataReader(IDataReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};


WtDataReaderAD::WtDataReaderAD()
	: _last_time(0)
	, _base_data_mgr(NULL)
	, _hot_mgr(NULL)
{
}


WtDataReaderAD::~WtDataReaderAD()
{
}

void WtDataReaderAD::init(WTSVariant* cfg, IDataReaderSink* sink, IHisDataLoader* loader /* = NULL */)
{
	IDataReader::init(cfg, sink, loader);

	_base_data_mgr = sink->get_basedata_mgr();
	_hot_mgr = sink->get_hot_mgr();

	if (cfg == NULL)
		return ;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);
}

WTSTickSlice* WtDataReaderAD::readTickSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		curSecs = _sink->get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime * 100000 + curSecs;
	}
	else
	{
		//20190807124533900
		curDate = (uint32_t)(etime / 1000000000);
		curTime = (uint32_t)(etime % 1000000000) / 100000;
		curSecs = (uint32_t)(etime % 100000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (cInfo.isHot() && commInfo->isFuture())
		curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
	else if (cInfo.isSecond() && commInfo->isFuture())
		curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);

	std::vector<WTSTickStruct>	ayTicks;

	//比较时间的对象
	WTSTickStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	//这里要改成从lmdb读取
	//if (isToday)
	//{
	//	TickBlockPair* tPair = getRTTickBlock(cInfo._exchg, curCode.c_str());
	//	if (tPair == NULL)
	//		return NULL;

	//	RTTickBlock* tBlock = tPair->_block;

	//	WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tBlock->_size - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
	//		if (a.action_date != b.action_date)
	//			return a.action_date < b.action_date;
	//		else
	//			return a.action_time < b.action_time;
	//	});

	//	uint32_t eIdx = pTick - tBlock->_ticks;

	//	//如果光标定位的tick时间比目标时间打, 则全部回退一个
	//	if (pTick->action_date > eTick.action_date || pTick->action_time>eTick.action_time)
	//	{
	//		pTick--;
	//		eIdx--;
	//	}

	//	uint32_t cnt = min(eIdx + 1, count);
	//	uint32_t sIdx = eIdx + 1 - cnt;
	//	WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, cnt);
	//	return slice;
	//}
	//else
	//{
	//	std::string key = StrUtil::printf("%s-%d", stdCode, endTDate);

	//	auto it = _his_tick_map.find(key);
	//	if(it == _his_tick_map.end())
	//	{
	//		std::stringstream ss;
	//		ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
	//		std::string filename = ss.str();
	//		if (!StdFile::exists(filename.c_str()))
	//			return NULL;

	//		HisTBlockPair& tBlkPair = _his_tick_map[key];
	//		StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
	//		if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
	//		{
	//			if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
	//			tBlkPair._buffer.clear();
	//			return NULL;
	//		}

	//		HisTickBlock* tBlock = (HisTickBlock*)tBlkPair._buffer.c_str();
	//		if (tBlock->_version == BLOCK_VERSION_CMP)
	//		{
	//			//压缩版本,要重新检查文件大小
	//			HisTickBlockV2* tBlockV2 = (HisTickBlockV2*)tBlkPair._buffer.c_str();

	//			if (tBlkPair._buffer.size() != (sizeof(HisTickBlockV2) + tBlockV2->_size))
	//			{
	//				if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
	//				return NULL;
	//			}

	//			//需要解压
	//			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

	//			//将原来的buffer只保留一个头部,并将所有tick数据追加到尾部
	//			tBlkPair._buffer.resize(sizeof(HisTickBlock));
	//			tBlkPair._buffer.append(buf);
	//			tBlockV2->_version = BLOCK_VERSION_RAW;
	//		}
	//		
	//		tBlkPair._block = (HisTickBlock*)tBlkPair._buffer.c_str();
	//	}
	//	
	//	HisTBlockPair& tBlkPair = _his_tick_map[key];
	//	if (tBlkPair._block == NULL)
	//		return NULL;

	//	HisTickBlock* tBlock = tBlkPair._block;

	//	uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
	//	if (tcnt <= 0)
	//		return NULL;

	//	WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tcnt - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
	//		if (a.action_date != b.action_date)
	//			return a.action_date < b.action_date;
	//		else
	//			return a.action_time < b.action_time;
	//	});

	//	uint32_t eIdx = pTick - tBlock->_ticks;
	//	if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
	//	{
	//		pTick--;
	//		eIdx--;
	//	}

	//	uint32_t cnt = min(eIdx + 1, count);
	//	uint32_t sIdx = eIdx + 1 - cnt;
	//	WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, cnt);
	//	return slice;
	//}

	return NULL;
}

bool WtDataReaderAD::cacheBarsFromStorage(const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	return true;
}

WTSKlineSlice* WtDataReaderAD::readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	std::string key = StrUtil::printf("%s#%u", stdCode, period);
	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		bHasHisData = cacheBarsFromStorage(key, stdCode, period);
	}
	else
	{
		bHasHisData = true;
	}


	return NULL;
}

void WtDataReaderAD::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */)
{
	//这里应该触发检查
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	if (nowTime <= _last_time)
		return;

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = (BarsList&)it->second;
		if (barsList._period != KP_DAY)
		{
			/* 这里要改成读取lmdb和缓存来进行判断
			RTKlineBlockPair* kBlk = getRTKilneBlock(barsList._exchg.c_str(), barsList._raw_code.c_str(), barsList._period);
			if (kBlk == NULL)
				continue;

			uint32_t preCnt = 0;
			if (barsList._size == UINT_MAX)
				preCnt = 0;
			else
				preCnt = barsList._size + 1;

			for (;;)
			{
				if (kBlk->_block->_size <= preCnt)
					break;

				WTSBarStruct& nextBar = kBlk->_block->_bars[preCnt];

				uint64_t barTime = 199000000000 + nextBar.time;
				if (barTime <= nowTime)
				{
					//如果不是后复权，则直接回调onbar
					//如果是后复权，则将最新bar复权处理以后，添加到cache中，再回调onbar
					if(barsList._factor == DBL_MAX)
					{
						_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
					}
					else
					{
						WTSBarStruct cpBar = nextBar;
						cpBar.open *= barsList._factor;
						cpBar.high *= barsList._factor;
						cpBar.low *= barsList._factor;
						cpBar.close *= barsList._factor;

						barsList._bars.emplace_back(cpBar);

						_sink->on_bar(barsList._code.c_str(), barsList._period, &barsList._bars[barsList._bars.size()-1]);
					}
				}
				else
				{
					break;
				}

				preCnt++;
			}

			if (preCnt > 0)
				barsList._size = preCnt - 1;
			*/
		}
	}

	if (_sink)
		_sink->on_all_bar_updated(uTime);

	_last_time = nowTime;
}
