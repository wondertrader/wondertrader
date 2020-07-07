#include "WtDataReader.h"

#include "../Share/WTSVariant.hpp"
#include "../Share/BoostFile.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"

#include "../Share/WTSContractInfo.hpp"
#include "../Share/IBaseDataMgr.h"
#include "../Share/IHotMgr.h"
#include "../Share/WTSDataDef.hpp"

#include "../WTSTools/WTSCmpHelper.hpp"

#include <rapidjson/document.h>
namespace rj = rapidjson;

extern "C"
{
	EXPORT_FLAG IDataReader* createDataReader()
	{
		IDataReader* ret = new WtDataReader();
		return ret;
	}

	EXPORT_FLAG void deleteDataReader(IDataReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};


WtDataReader::WtDataReader()
	: _last_time(0)
	, _base_data_mgr(NULL)
	, _hot_mgr(NULL)
{
}


WtDataReader::~WtDataReader()
{
}


//bool WtDataStorage::init(WTSVariant* cfg, WtEngine* engine, IBaseDataMgr* bdMgr, IHotMgr* hotMgr)
void WtDataReader::init(WTSVariant* cfg, IDataReaderSink* sink)
{
	_sink = sink;
	_base_data_mgr = sink->get_basedata_mgr();
	_hot_mgr = sink->get_hot_mgr();

	if (cfg == NULL)
		return ;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);

	if(cfg->has("adjfactor"))
		loadStkAdjFactors(cfg->getCString("adjfactor"));
}


bool WtDataReader::loadStkAdjFactors(const char* adjfile)
{
	if(!BoostFile::exists(adjfile))
	{
		if (_sink) _sink->reader_log(LL_ERROR, "除权因子文件%s不存在", adjfile);
		return false;
	}

	std::string content;
	BoostFile::read_file_contents(adjfile, content);

	rj::Document doc;
	doc.Parse(content.c_str());

	if(doc.HasParseError())
	{
		if (_sink) _sink->reader_log(LL_ERROR, "除权因子文件%s解析失败", adjfile);
		return false;
	}

	uint32_t stk_cnt = 0;
	uint32_t fct_cnt = 0;
	for (auto& mExchg : doc.GetObject())
	{
		const char* exchg = mExchg.name.GetString();
		const rj::Value& itemExchg = mExchg.value;
		for(auto& mCode : itemExchg.GetObject())
		{
			const char* code = mCode.name.GetString();
			const rj::Value& ayFacts = mCode.value;
			if(!ayFacts.IsArray() )
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

	if (_sink) _sink->reader_log(LL_INFO, "共加载%u只股票的%u条除权因子数据", stk_cnt, fct_cnt);
	return true;
}

WTSTickSlice* WtDataReader::readTickSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	std::string exchg, code, pid;
	bool isHot = false;
	CodeHelper::extractStdCode(stdCode, exchg, code, pid, isHot);
	std::string stdPID = StrUtil::printf("%s.%s", exchg.c_str(), pid.c_str());


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

	std::string curCode = code;
	if (isHot)
		curCode = _hot_mgr->getRawCode(exchg.c_str(), pid.c_str(), endTDate);

	std::vector<WTSTickStruct>	ayTicks;

	//比较时间的对象
	WTSTickStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		TBlockPair* tPair = getRTTBlock(exchg.c_str(), curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTTickBlock* tBlock = tPair->_block;

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tBlock->_size - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_ticks;

		//如果光标定位的tick时间比目标时间打, 则全部回退一个
		if (pTick->action_date > eTick.action_date || pTick->action_time>eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, cnt);
		return slice;
	}
	else
	{
		std::string key = StrUtil::printf("%s-%d", stdCode, endTDate);

		auto it = _his_tick_map.find(key);
		if(it == _his_tick_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/ticks/" << exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisTBlockPair& tBlkPair = _his_tick_map[key];
			StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
			if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
				tBlkPair._buffer.clear();
				return NULL;
			}

			HisTickBlock* tBlock = (HisTickBlock*)tBlkPair._buffer.c_str();
			if (tBlock->_version == BLOCK_VERSION_CMP)
			{
				//压缩版本，要重新检查文件大小
				HisTickBlockV2* tBlockV2 = (HisTickBlockV2*)tBlkPair._buffer.c_str();

				if (tBlkPair._buffer.size() != (sizeof(HisTickBlockV2) + tBlockV2->_size))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
					return NULL;
				}

				//需要解压
				std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

				//将原来的buffer只保留一个头部，并将所有tick数据追加到尾部
				tBlkPair._buffer.resize(sizeof(HisTickBlock));
				tBlkPair._buffer.append(buf);
				tBlockV2->_version = BLOCK_VERSION_RAW;
			}
			
			tBlkPair._block = (HisTickBlock*)tBlkPair._buffer.c_str();
		}
		
		HisTBlockPair& tBlkPair = _his_tick_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisTickBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
		if (tcnt <= 0)
			return NULL;

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tcnt - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_ticks;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, cnt);
		return slice;
	}
}

WTSHisTickData* WtDataReader::readTicks(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */, bool bOnlyValid /* = false */)
{
	std::string exchg, code, pid;
	bool isHot = false;
	CodeHelper::extractStdCode(stdCode, exchg, code, pid, isHot);
	std::string stdPID = StrUtil::printf("%s.%s", exchg.c_str(), pid.c_str());

	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		curSecs = _sink->get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime*100000 + curSecs;
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
	
	std::string curCode = code;
	if (isHot)
		curCode = _hot_mgr->getRawCode(exchg.c_str(), pid.c_str(), endTDate);

	std::vector<WTSTickStruct>	ayTicks;

	//比较时间的对象
	WTSTickStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if(isToday)
	{
		TBlockPair* tPair = getRTTBlock(exchg.c_str(), curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTTickBlock* tBlock = tPair->_block;

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tBlock->_size - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_ticks;

		//如果光标定位的tick时间比目标时间打, 则全部回退一个
		if (pTick->action_date > eTick.action_date || pTick->action_time>eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		//如果只要有效tick（即成交量大于0的数据)
		if (bOnlyValid)
		{
			uint32_t left = count;
			for (;;)
			{
				if (tBlock->_ticks[eIdx].volumn != 0)
				{
					ayTicks.push_back(tBlock->_ticks[eIdx]);
					left--;
				}

				if (eIdx == 0)
					break;

				eIdx--;

				if (left == 0)
					break;
			}

			std::reverse(ayTicks.begin(), ayTicks.end());
		}
		else
		{
			//如果不是限制, 则可以整块数据拷贝
			uint32_t cnt = min(eIdx + 1, count);
			uint32_t sIdx = eIdx + 1 - cnt;
			ayTicks.resize(cnt);
			memcpy(ayTicks.data(), tBlock->_ticks + sIdx, sizeof(WTSTickStruct)*cnt);
		}
	}
	else
	{
		std::string key = StrUtil::printf("%s-%d", stdCode, endTDate);

		auto it = _his_tick_map.find(key);
		if (it == _his_tick_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/ticks/" << exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisTBlockPair& tBlkPair = _his_tick_map[key];
			StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
			if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
				tBlkPair._buffer.clear();
				return NULL;
			}

			HisTickBlock* tBlock = (HisTickBlock*)tBlkPair._buffer.c_str();
			if (tBlock->_version == BLOCK_VERSION_CMP)
			{
				//压缩版本，要重新检查文件大小
				HisTickBlockV2* tBlockV2 = (HisTickBlockV2*)tBlkPair._buffer.c_str();

				if (tBlkPair._buffer.size() != (sizeof(HisTickBlockV2) + tBlockV2->_size))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
					return NULL;
				}

				//需要解压
				std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

				//将原来的buffer只保留一个头部，并将所有tick数据追加到尾部
				tBlkPair._buffer.resize(sizeof(HisTickBlock));
				tBlkPair._buffer.append(buf);
				tBlockV2->_version = BLOCK_VERSION_RAW;
			}

			tBlkPair._block = (HisTickBlock*)tBlkPair._buffer.c_str();
		}

		HisTBlockPair& tBlkPair = _his_tick_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisTickBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
		if (tcnt <= 0)
			return NULL;

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tcnt - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_ticks;
		if (pTick->action_date > eTick.action_date || pTick->action_time>=eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		if(bOnlyValid)
		{
			uint32_t left = count;
			for (;;)
			{
				if (tBlock->_ticks[eIdx].volumn != 0)
				{
					ayTicks.push_back(tBlock->_ticks[eIdx]);
					left--;
				}

				if(eIdx == 0)
					break;

				eIdx--;

				if(left == 0)
					break;
			}

			std::reverse(ayTicks.begin(), ayTicks.end());
		}
		else
		{
			uint32_t cnt = min(eIdx + 1, count);
			uint32_t sIdx = eIdx + 1 - cnt;
			ayTicks.resize(cnt);
			memcpy(ayTicks.data(), tBlock->_ticks + sIdx, sizeof(WTSTickStruct)*cnt);
		}
	}

	if (ayTicks.empty())
		return NULL;

	WTSHisTickData* ticks = WTSHisTickData::create(stdCode, ayTicks, bOnlyValid);
	return ticks;
}

bool WtDataReader::cacheHisBars(const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "min1"; break;
	case KP_Minute5: pname = "min5"; break;
	default: pname = "day"; break;
	}

	BarsList& barList = _bars_cache[key];
	barList._code = stdCode;
	barList._period = period;
	barList._exchg = cInfo._exchg;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;
	if (cInfo._hot && cInfo._category == CC_Future)//如果是读取期货主力连续数据
	{
		HotSections secs;
		if (!_hot_mgr->splitHotSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
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
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if(kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
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

			if (_sink) _sink->reader_log(LL_INFO, "主力合约%s历史%s数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
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
				uint64_t sTime = _base_data_mgr->getBoundaryTime(stdPID.c_str(), leftDt, false, true);
				uint64_t eTime = _base_data_mgr->getBoundaryTime(stdPID.c_str(), rightDt, false, false);

				sBar.date = leftDt;
				sBar.time = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

				if(sBar.time < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
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
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
					return false;
				}

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				WTSBarStruct* firstBar = NULL;
				uint32_t barcnt = 0;
				std::string rawData;
				if(kBlock->_version == BLOCK_VERSION_CMP)
				{
					if (content.size() < sizeof(HisKlineBlockV2))
					{
						if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
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

				if(bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.push_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if(cInfo._exright && cInfo._category == CC_Stock)//如果是读取股票复权数据
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
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
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

			if (_sink) _sink->reader_log(LL_INFO, "股票%s历史%s复权数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
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
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
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
						if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
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

				if(pBar != NULL)
				{
					uint32_t sIdx = pBar - firstBar;
					uint32_t curCnt = barcnt - sIdx;
					std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
					tempAy->resize(curCnt);
					memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
					realCnt += curCnt;

					auto& ayFactors = getAdjFactors(cInfo._code, cInfo._exchg);
					if(!ayFactors.empty())
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
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
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
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
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
		barsSections.clear();
	}

	if (_sink) _sink->reader_log(LL_INFO, "合约%s的历史%s数据已缓存%u条", stdCode, pname.c_str(), realCnt);
	return true;
}

WTSBarStruct* WtDataReader::indexBarFromCache(const std::string& key, uint64_t etime, uint32_t& count, bool isDay /* = false */)
{
	uint32_t curDate, curTime;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
	}
	else
	{
		curDate = (uint32_t)(etime / 10000);
		curTime = (uint32_t)(etime % 10000);
	}

	BarsList& barsList = _bars_cache[key];
	if (barsList._his_cursor == UINT_MAX)
	{
		//光标尚未初始化, 需要重新定位
		uint64_t nowTime = (uint64_t)curDate * 10000 + curTime;
		if (_last_time < nowTime)
		{
			_last_time = nowTime;
		}

		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;
		auto it = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), bar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});


		if (it == barsList._bars.end())
			barsList._his_cursor = barsList._bars.size() - 1;
		else
		{
			if ((isDay && it->date > bar.date) || (!isDay && it->time > bar.time))
			{
				it--;
			}

			barsList._his_cursor = it - barsList._bars.begin();
		}
	}

	uint32_t sIdx = 0;
	if (count <= barsList._his_cursor + 1)
	{
		sIdx = barsList._his_cursor - count + 1;
	}

	uint32_t curCnt = barsList._his_cursor - sIdx + 1;
	//if (curCnt > 0)
	//{
	//	ayBars.resize(curCnt);
	//	memcpy(ayBars.data(), &barsList._bars[sIdx], sizeof(WTSBarStruct)*curCnt);
	//}
	count = curCnt;
	return &barsList._bars[sIdx];
}

uint32_t WtDataReader::readBarsFromCache(const std::string& key, uint64_t etime, uint32_t count, std::vector<WTSBarStruct>& ayBars, bool isDay /* = false */)
{
	uint32_t curDate, curTime;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
	}
	else
	{
		curDate = (uint32_t)(etime / 10000);
		curTime = (uint32_t)(etime % 10000);
	}

	BarsList& barsList = _bars_cache[key];
	if (barsList._his_cursor == UINT_MAX)
	{
		//光标尚未初始化, 需要重新定位
		uint64_t nowTime = (uint64_t)curDate * 10000 + curTime;
		if (_last_time < nowTime)
		{
			_last_time = nowTime;
		}

		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;
		auto it = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), bar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});
		

		if(it == barsList._bars.end())
			barsList._his_cursor = barsList._bars.size() - 1;
		else
		{
			if ((isDay && it->date > bar.date) || (!isDay && it->time > bar.time))
			{
				if (it == barsList._bars.begin())
					return 0;
				
				it--;
			}

			barsList._his_cursor = it - barsList._bars.begin();
		}
	}

	uint32_t sIdx = 0;
	if (count <= barsList._his_cursor + 1)
	{
		sIdx = barsList._his_cursor - count + 1;
	}

	uint32_t curCnt = barsList._his_cursor - sIdx + 1;
	if(curCnt > 0)
	{
		ayBars.resize(curCnt);
		memcpy(ayBars.data(), &barsList._bars[sIdx], sizeof(WTSBarStruct)*curCnt);
	}
	return curCnt;
}

WTSKlineSlice* WtDataReader::readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	std::string key = StrUtil::printf("%s#%u", stdCode, period);
	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		bHasHisData = cacheHisBars(key, stdCode, period);
	}
	else
	{
		bHasHisData = true;
	}

	uint32_t curDate, curTime;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		etime = (uint64_t)curDate * 10000 + curTime;
	}
	else
	{
		curDate = (uint32_t)(etime / 10000);
		curTime = (uint32_t)(etime % 10000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);


	//WTSKlineSlice* kData = WTSKlineSlice::create(stdCode, period, times, NULL, 0, NULL, 0);
	
	WTSBarStruct* hisHead = NULL;
	WTSBarStruct* rtHead = NULL;
	uint32_t hisCnt = 0;
	uint32_t rtCnt = 0;

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "min1"; break;
	case KP_Minute5: pname = "min5"; break;
	default: pname = "day"; break;
	}

	uint32_t left = count;

	//是否包含当天的
	bool bHasToday = (endTDate == curTDate);

	if (cInfo._hot && cInfo._category == CC_Future)
	{
		_bars_cache[key]._raw_code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, curTDate);
		if (_sink) _sink->reader_log(LL_INFO, "主力合约映射确认: %s -> %s", stdCode, _bars_cache[key]._raw_code.c_str());
	}
	else
	{
		_bars_cache[key]._raw_code = cInfo._code;
	}

	if (bHasToday)
	{
		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;

		const char* curCode = _bars_cache[key]._raw_code.c_str();

		//读取实时的
		RTKBlockPair* kPair = getRTKBlock(cInfo._exchg, curCode, period);
		if (kPair != NULL)
		{
			//读取当日的数据
			WTSBarStruct* pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + (kPair->_block->_size - 1), bar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
				if (period == KP_DAY)
					return a.date < b.date;
				else
					return a.time < b.time;
			});
			uint32_t idx = pBar - kPair->_block->_bars;
			if ((period == KP_DAY && pBar->date > bar.date) || (period != KP_DAY && pBar->time > bar.time))
			{
				pBar--;
				idx--;
			}

			_bars_cache[key]._rt_cursor = idx;

			uint32_t sIdx = 0;
			if (left <= idx + 1)
			{
				sIdx = idx - left + 1;
			}

			uint32_t curCnt = (idx - sIdx + 1);
			left -= (idx - sIdx + 1);
			rtHead = &kPair->_block->_bars[sIdx];
			rtCnt = curCnt;

			//std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
			//tempAy->resize(curCnt);
			//memcpy(tempAy->data(), &kPair->_block->_bars[sIdx], sizeof(WTSBarStruct)*curCnt);
			//realCnt += curCnt;

			//barsSections.push_back(tempAy);

		}
	}

	if (left > 0 && bHasHisData)
	{
		hisCnt = left;
		hisHead = indexBarFromCache(key, etime, hisCnt, period == KP_DAY);
	}

	if (hisCnt + rtCnt > 0)
	{
		WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, 1, hisHead, hisCnt, rtHead, rtCnt);
		return slice;
	}

	return NULL;
}

/*
WTSKlineData* WtDataReader::readBars(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	std::string key = StrUtil::printf("%s#%u", stdCode, period);
	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		bHasHisData = cacheHisBars(key, stdCode, period);
	}
	else
	{
		bHasHisData = true;
	}

	uint32_t curDate, curTime;
	if(etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		etime = (uint64_t)curDate * 10000 + curTime;
	}
	else
	{
		curDate = (uint32_t)(etime / 10000);
		curTime = (uint32_t)(etime % 10000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);


	WTSKlineData* kData = NULL;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "min1"; break;
	case KP_Minute5: pname = "min5"; break;
	default: pname = "day"; break;
	}

	uint32_t realCnt = 0;
	uint32_t left = count;
	//是否包含当天的
	bool bHasToday = (endTDate == curTDate);

	if (cInfo._hot && cInfo._category == CC_Future)
	{
		_bars_cache[key]._raw_code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, curTDate);
		if (_sink) _sink->reader_log(LL_INFO, "主力合约映射确认: %s -> %s", stdCode, _bars_cache[key]._raw_code.c_str());
	}
	else
	{
		_bars_cache[key]._raw_code = cInfo._code;
	}

	if (bHasToday)
	{
		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;

		const char* curCode = _bars_cache[key]._raw_code.c_str();

		//读取实时的
		RTKBlockPair* kPair = getRTKBlock(cInfo._exchg, curCode, period);
		if (kPair != NULL)
		{
			//读取当日的数据
			WTSBarStruct* pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + (kPair->_block->_size - 1), bar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
				if (period == KP_DAY)
					return a.date < b.date;
				else
					return a.time < b.time;
			});
			uint32_t idx = pBar - kPair->_block->_bars;
			if ((period == KP_DAY && pBar->date > bar.date) || (period != KP_DAY && pBar->time > bar.time))
			{
				pBar--;
				idx--;
			}

			_bars_cache[key]._rt_cursor = idx;

			uint32_t sIdx = 0;
			if (left <= idx + 1)
			{
				sIdx = idx - left + 1;
			}

			uint32_t curCnt = (idx - sIdx + 1);
			left -= (idx - sIdx + 1);

			std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
			tempAy->resize(curCnt);
			memcpy(tempAy->data(), &kPair->_block->_bars[sIdx], sizeof(WTSBarStruct)*curCnt);
			realCnt += curCnt;

			barsSections.push_back(tempAy);
		}
	}


	//读取历史的
	if (left > 0 && bHasHisData)
	{
		std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
		uint32_t curCnt = readBarsFromCache(key, etime, left, *tempAy, period == KP_DAY);
		realCnt += curCnt;
		barsSections.push_back(tempAy);
	}

	//if(isHot)
	//{
	//	_bars_cache[key]._raw_code = _hot_mgr->getRawCode(exchg.c_str(), pid.c_str(), curTDate);
	//	if (_sink) _sink->reader_log(LL_INFO, "主力合约映射确认: %s -> %s", stdCode, _bars_cache[key]._raw_code.c_str());		

	//	

	//	if (left > 0 && bHasHisData)
	//	{
	//		std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
	//		uint32_t curCnt = readBarsFromCache(key, etime, left, *tempAy, period == KP_DAY);
	//		realCnt += curCnt;
	//		barsSections.push_back(tempAy);
	//	}
	//}
	//else
	//{
	//	_bars_cache[key]._raw_code = code;

	//	if (bHasToday)
	//	{
	//		WTSBarStruct bar;
	//		bar.date = curDate;
	//		bar.time = (curDate - 19900000) * 10000 + curTime;

	//		//读取实时的
	//		RTKBlockPair* kPair = getRTKBlock(exchg.c_str(), code.c_str(), period);
	//		if (kPair != NULL)
	//		{
	//			//读取当日的数据
	//			WTSBarStruct* pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + (kPair->_block->_size - 1), bar, [](const WTSBarStruct& a, const WTSBarStruct& b){
	//				if (a.date != b.date)
	//					return a.date < b.date;
	//				else
	//					return a.time < b.time;
	//			});
	//			uint32_t idx = pBar - kPair->_block->_bars;
	//			_bars_cache[key]._rt_cursor = idx;

	//			uint32_t sIdx = 0;
	//			if (count <= idx + 1)
	//			{
	//				sIdx = idx - count + 1;
	//			}

	//			uint32_t curCnt = (idx - sIdx + 1);
	//			left -= curCnt;

	//			std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
	//			tempAy->resize(curCnt);
	//			memcpy(tempAy->data(), &kPair->_block->_bars[sIdx], sizeof(WTSBarStruct)*curCnt);
	//			realCnt += curCnt;

	//			barsSections.push_back(tempAy);
	//		}
	//	}

	//	//读取历史的
	//	if(left > 0 && bHasHisData)
	//	{
	//		std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
	//		uint32_t curCnt = readBarsFromCache(key, etime, left, *tempAy, period == KP_DAY);
	//		realCnt += curCnt;
	//		barsSections.push_back(tempAy);
	//	}
	//}

	if (realCnt > 0)
	{
		kData = WTSKlineData::create(stdCode, realCnt);
		kData->setPeriod(period);
		kData->setClosed(!bHasToday);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(kData->getDataRef().data() + curIdx, tempAy->data(), tempAy->size()*sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barsSections.clear();
	}

	return kData;
}
*/

WtDataReader::TBlockPair* WtDataReader::getRTTBlock(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);

	std::string path = StrUtil::printf("%srt/ticks/%s/%s.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	TBlockPair& block = _rt_tick_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTickBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//说明文件大小已变, 需要重新映射
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTickBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::RTKBlockPair* WtDataReader::getRTKBlock(const char* exchg, const char* code, WTSKlinePeriod period)
{
	if (period != KP_Minute1 && period != KP_Minute5)
		return NULL;

	std::string key = StrUtil::printf("%s.%s", exchg, code);

	RTKBlockFilesMap* cache_map = NULL;
	std::string subdir = "";
	BlockType bType;
	switch (period)
	{
	case KP_Minute1:
		cache_map = &_rt_min1_map;
		subdir = "min1";
		bType = BT_RT_Minute1;
		break;
	case KP_Minute5:
		cache_map = &_rt_min5_map;
		subdir = "min5";
		bType = BT_RT_Minute5;
		break;
	default: break;
	}

	std::string path = StrUtil::printf("%srt/%s/%s/%s.dmb", _base_dir.c_str(), subdir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	RTKBlockPair& block = (*cache_map)[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTKlineBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//说明文件大小已变, 需要重新映射
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTKlineBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

void WtDataReader::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */)
{
	//这里应该触发检查
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	if (nowTime <= _last_time)
		return;

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = it->second;
		if (barsList._period != KP_DAY)
		{
			//如果历史数据指标不在尾部, 说明是回测模式, 要继续回放历史数据
			if (barsList._bars.size() - 1 > barsList._his_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._his_cursor + 1];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
					}
					else
					{
						break;
					}

					barsList._his_cursor++;

					if (barsList._his_cursor == barsList._bars.size() - 1)
						break;
				}
			}
			else if (!barsList._raw_code.empty())
			{
				RTKBlockPair* kBlk = getRTKBlock(barsList._exchg.c_str(), barsList._raw_code.c_str(), barsList._period);
				if (kBlk == NULL)
					continue;

				uint32_t preCnt = 0;
				if (barsList._rt_cursor == UINT_MAX)
					preCnt = 0;
				else
					preCnt = barsList._rt_cursor + 1;

				for (;;)
				{
					if (kBlk->_block->_size <= preCnt)
						break;

					WTSBarStruct& nextBar = kBlk->_block->_bars[preCnt];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
					}
					else
					{
						break;
					}

					preCnt++;
				}

				if (preCnt > 0)
					barsList._rt_cursor = preCnt - 1;
			}
		}
		else
		{
			if (barsList._bars.size() - 1 > barsList._his_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._his_cursor + 1];

					if (nextBar.date <= endTDate)
					{
						_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
					}
					else
					{
						break;
					}

					barsList._his_cursor++;

					if (barsList._his_cursor == barsList._bars.size() - 1)
						break;
				}
			}
		}
	}

	if (_sink)
		_sink->on_all_bar_updated(uTime);

	_last_time = nowTime;
}