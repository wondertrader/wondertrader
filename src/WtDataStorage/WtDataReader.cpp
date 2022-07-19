#include "WtDataReader.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSDataDef.hpp"

#include "../WTSUtils/WTSCmpHelper.hpp"
#include "../WTSUtils/WTSCfgLoader.h"

#include <rapidjson/document.h>
namespace rj = rapidjson;

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void pipe_reader_log(IDataReaderSink* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	const char* buffer = fmtutil::format(format, args...);

	sink->reader_log(ll, buffer);
}

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

/*
 *	���������
 */
bool proc_block_data(std::string& content, bool isBar, bool bKeepHead /* = true */)
{
	BlockHeader* header = (BlockHeader*)content.data();

	bool bCmped = header->is_compressed();
	bool bOldVer = header->is_old_version();

	//�����û��ѹ����Ҳ�����ϰ汾�ṹ�壬��ֱ�ӷ���
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
			return false;
		}

		//���ļ�ͷ��������ݽ��н�ѹ
		buffer = WTSCmpHelper::uncompress_data(content.data() + BLOCK_HEADERV2_SIZE, (uint32_t)blkV2->_size);
	}
	else
	{
		if (!bOldVer)
		{
			//��������ϰ汾��ֱ�ӷ���
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
		}
	}

	if (bKeepHead)
	{
		content.resize(BLOCK_HEADER_SIZE);
		content.append(buffer);
		header = (BlockHeader*)content.data();
		header->_version = BLOCK_VERSION_RAW_V2;
	}
	else
	{
		content.swap(buffer);
	}

	return true;
}


WtDataReader::WtDataReader()
	: _last_time(0)
	, _base_data_mgr(NULL)
	, _hot_mgr(NULL)
{
}


WtDataReader::~WtDataReader()
{
}

void WtDataReader::init(WTSVariant* cfg, IDataReaderSink* sink, IHisDataLoader* loader /* = NULL */)
{
	IDataReader::init(cfg, sink, loader);

	_base_data_mgr = sink->get_basedata_mgr();
	_hot_mgr = sink->get_hot_mgr();

	if (cfg == NULL)
		return ;

	std::string root_dir = cfg->getCString("path");
	root_dir = StrUtil::standardisePath(root_dir);

	_rt_dir = root_dir + "rt/";

	_his_dir = cfg->getCString("his_path");
	if(!_his_dir.empty())
		_his_dir = StrUtil::standardisePath(_his_dir);
	else
		_his_dir = root_dir + "his/";

	pipe_reader_log(sink, LL_INFO, "WtDataReader initialized, rt dir is {}, hist dir is {}", _rt_dir, _his_dir);
	
	/*
	 *	By Wesley @ 2021.12.20
	 *	�ȴ�extloader���س�Ȩ����
	 *	�������ʧ�ܣ����������˳�Ȩ�����ļ����ټ��س�Ȩ�����ļ�
	 */
	bool bLoaded = loadStkAdjFactorsFromLoader();

	if (!bLoaded && cfg->has("adjfactor"))
		loadStkAdjFactorsFromFile(cfg->getCString("adjfactor"));
	else
		pipe_reader_log(sink, LL_INFO, "No adjusting factor file configured, loading skipped");
}

bool WtDataReader::loadStkAdjFactorsFromLoader()
{
	if (NULL == _loader)
		return false;

	bool ret = _loader->loadAllAdjFactors(&_adj_factors, [](void* obj, const char* stdCode, uint32_t* dates, double* factors, uint32_t count) {
		AdjFactorMap* fact_map = (AdjFactorMap*)obj;
		AdjFactorList& fctrLst = (*fact_map)[stdCode];

		for(uint32_t i = 0; i < count; i++)
		{
			AdjFactor adjFact;
			adjFact._date = dates[i];
			adjFact._factor = factors[i];

			fctrLst.emplace_back(adjFact);
		}

		//һ��Ҫ�ѵ�һ���ӽ�ȥ����Ȼ�����ǰ��Ȩ�Ļ������ܻ�©�������������
		AdjFactor adjFact;
		adjFact._date = 19900101;
		adjFact._factor = 1;
		fctrLst.emplace_back(adjFact);

		std::sort(fctrLst.begin(), fctrLst.end(), [](const AdjFactor& left, const AdjFactor& right) {
			return left._date < right._date;
		});
	});

	if (ret && _sink) pipe_reader_log(_sink,LL_INFO, "Adjusting factors of {} contracts loaded via extended loader", _adj_factors.size());
	return ret;
}

bool WtDataReader::loadStkAdjFactorsFromFile(const char* adjfile)
{
	if(!StdFile::exists(adjfile))
	{
		pipe_reader_log(_sink,LL_ERROR, "Adjusting factors file {} not exists", adjfile);
		return false;
	}

	WTSVariant* doc = WTSCfgLoader::load_from_file(adjfile, true);
	if(doc == NULL)
	{
		pipe_reader_log(_sink, LL_ERROR, "Loading adjusting factors file {} failed", adjfile);
		return false;
	}

	uint32_t stk_cnt = 0;
	uint32_t fct_cnt = 0;
	for (const std::string& exchg : doc->memberNames())
	{
		WTSVariant* itemExchg = doc->get(exchg);
		for(const std::string& code : itemExchg->memberNames())
		{
			WTSVariant* ayFacts = itemExchg->get(code);
			if(!ayFacts->isArray() )
				continue;

			/*
			 *	By Wesley @ 2021.12.21
			 *	�ȼ��code�ĸ�ʽ�ǲ��ǰ���PID����STK.600000
			 *	�������PID����ֱ�Ӹ�ʽ�����������������ǿ��ΪSTK
			 */
			bool bHasPID = (code.find('.') != std::string::npos);

			std::string key;
			if (bHasPID)
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

			//һ��Ҫ�ѵ�һ���ӽ�ȥ����Ȼ�����ǰ��Ȩ�Ļ������ܻ�©�������������
			AdjFactor adjFact;
			adjFact._date = 19900101;
			adjFact._factor = 1;
			fctrLst.emplace_back(adjFact);

			std::sort(fctrLst.begin(), fctrLst.end(), [](const AdjFactor& left, const AdjFactor& right) {
				return left._date < right._date;
			});
		}
	}

	pipe_reader_log(_sink,LL_INFO, "{} adjusting factors of {} tickers loaded", fct_cnt, stk_cnt);
	doc->release();
	return true;
}

WTSTickSlice* WtDataReader::readTickSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	const char* stdPID = commInfo->getFullPid();

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

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID, curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID, 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (commInfo->isFuture())
	{
		const char* ruleTag = cInfo._ruletag;
		if (strlen(ruleTag) > 0)
			curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), endTDate);
		//else if (cInfo.isHot())
		//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
		//else if (cInfo.isSecond())
		//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);
	}

	//�Ƚ�ʱ��Ķ���
	WTSTickStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		TickBlockPair* tPair = getRTTickBlock(cInfo._exchg, curCode.c_str());
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

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
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
		thread_local static char key[64] = { 0 };
		fmtutil::format_to(key, "{}-{}", stdCode, endTDate);

		auto it = _his_tick_map.find(key);
		if(it == _his_tick_map.end())
		{
			std::stringstream ss;
			ss << _his_dir << "ticks/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisTBlockPair& tBlkPair = _his_tick_map[key];
			StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
			if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
			{
				pipe_reader_log(_sink,LL_ERROR, "Sizechecking of his tick data file {} failed", filename);
				tBlkPair._buffer.clear();
				return NULL;
			}

			proc_block_data(tBlkPair._buffer, false, true);			
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

WTSOrdQueSlice* WtDataReader::readOrdQueSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	const char* stdPID = commInfo->getFullPid();

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

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID, curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID, 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (commInfo->isFuture())
	{
		const char* ruleTag = cInfo._ruletag;
		if (strlen(ruleTag) > 0)
			curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), endTDate);
		//else if (cInfo.isHot())
		//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
		//else if (cInfo.isSecond())
		//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);
	}

	//�Ƚ�ʱ��Ķ���
	WTSOrdQueStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		OrdQueBlockPair* tPair = getRTOrdQueBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTOrdQueBlock* rtBlock = tPair->_block;

		WTSOrdQueStruct* pItem = std::lower_bound(rtBlock->_queues, rtBlock->_queues + (rtBlock->_size - 1), eTick, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pItem - rtBlock->_queues;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, rtBlock->_queues + sIdx, cnt);
		return slice;
	}
	else
	{
		thread_local static char key[64] = { 0 };
		fmtutil::format_to(key, "{}-{}", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _his_dir << "queue/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisOrdQueBlockPair& hisBlkPair = _his_ordque_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisOrdQueBlockV2))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷί�ж��������ļ�{}��СУ��ʧ��", filename);
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisOrdQueBlockV2* tBlockV2 = (HisOrdQueBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisOrdQueBlockV2) + tBlockV2->_size))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷί�ж��������ļ�{}��СУ��ʧ��", filename);
				return NULL;
			}

			//��Ҫ��ѹ
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//��ԭ����bufferֻ����һ��ͷ��,��������tick����׷�ӵ�β��
			hisBlkPair._buffer.resize(sizeof(HisOrdQueBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW_V2;

			hisBlkPair._block = (HisOrdQueBlock*)hisBlkPair._buffer.c_str();
		}

		HisOrdQueBlockPair& tBlkPair = _his_ordque_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisOrdQueBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisOrdQueBlock)) / sizeof(WTSOrdQueStruct);
		if (tcnt <= 0)
			return NULL;

		WTSOrdQueStruct* pTick = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_items;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, tBlock->_items + sIdx, cnt);
		return slice;
	}
}

WTSOrdDtlSlice* WtDataReader::readOrdDtlSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	const char* stdPID = commInfo->getFullPid();

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

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID, curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID, 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (commInfo->isFuture())
	{
		const char* ruleTag = cInfo._ruletag;
		if (strlen(ruleTag) > 0)
			curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), endTDate);
		//else if (cInfo.isHot())
		//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
		//else if (cInfo.isSecond())
		//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);
	}

	//�Ƚ�ʱ��Ķ���
	WTSOrdDtlStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		OrdDtlBlockPair* tPair = getRTOrdDtlBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTOrdDtlBlock* rtBlock = tPair->_block;

		WTSOrdDtlStruct* pItem = std::lower_bound(rtBlock->_details, rtBlock->_details + (rtBlock->_size - 1), eTick, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pItem - rtBlock->_details;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, rtBlock->_details + sIdx, cnt);
		return slice;
	}
	else
	{
		thread_local static char key[64] = { 0 };
		fmtutil::format_to(key, "{}-{}", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _his_dir << "orders/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisOrdDtlBlockPair& hisBlkPair = _his_orddtl_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisOrdDtlBlockV2))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷ���ί�������ļ�{}��СУ��ʧ��", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisOrdDtlBlockV2* tBlockV2 = (HisOrdDtlBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisOrdDtlBlockV2) + tBlockV2->_size))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷ���ί�������ļ�{}��СУ��ʧ��", filename.c_str());
				return NULL;
			}

			//��Ҫ��ѹ
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//��ԭ����bufferֻ����һ��ͷ��,��������tick����׷�ӵ�β��
			hisBlkPair._buffer.resize(sizeof(HisOrdDtlBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW_V2;

			hisBlkPair._block = (HisOrdDtlBlock*)hisBlkPair._buffer.c_str();
		}

		HisOrdDtlBlockPair& tBlkPair = _his_orddtl_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisOrdDtlBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisOrdDtlBlock)) / sizeof(WTSOrdDtlStruct);
		if (tcnt <= 0)
			return NULL;

		WTSOrdDtlStruct* pTick = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_items;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, tBlock->_items + sIdx, cnt);
		return slice;
	}
}

WTSTransSlice* WtDataReader::readTransSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	const char* stdPID = commInfo->getFullPid();

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

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID, curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID, 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (commInfo->isFuture())
	{
		const char* ruleTag = cInfo._ruletag;
		if (strlen(ruleTag) > 0)
			curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), endTDate);
		//else if (cInfo.isHot())
		//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
		//else if (cInfo.isSecond())
		//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);
	}

	//�Ƚ�ʱ��Ķ���
	WTSTransStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		TransBlockPair* tPair = getRTTransBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTTransBlock* rtBlock = tPair->_block;

		WTSTransStruct* pItem = std::lower_bound(rtBlock->_trans, rtBlock->_trans + (rtBlock->_size - 1), eTick, [](const WTSTransStruct& a, const WTSTransStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pItem - rtBlock->_trans;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTransSlice* slice = WTSTransSlice::create(stdCode, rtBlock->_trans + sIdx, cnt);
		return slice;
	}
	else
	{
		thread_local static char key[64] = { 0 };
		fmtutil::format_to(key, "{}-{}", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _his_dir << "trans/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisTransBlockPair& hisBlkPair = _his_trans_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisTransBlockV2))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷ��ʳɽ������ļ�{}��СУ��ʧ��", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisTransBlockV2* tBlockV2 = (HisTransBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisTransBlockV2) + tBlockV2->_size))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷ��ʳɽ������ļ�{}��СУ��ʧ��", filename.c_str());
				return NULL;
			}

			//��Ҫ��ѹ
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//��ԭ����bufferֻ����һ��ͷ��,��������tick����׷�ӵ�β��
			hisBlkPair._buffer.resize(sizeof(HisTransBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW_V2;

			hisBlkPair._block = (HisTransBlock*)hisBlkPair._buffer.c_str();
		}

		HisTransBlockPair& tBlkPair = _his_trans_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisTransBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTransBlock)) / sizeof(WTSTransStruct);
		if (tcnt <= 0)
			return NULL;

		WTSTransStruct* pTick = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSTransStruct& a, const WTSTransStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_items;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTransSlice* slice = WTSTransSlice::create(stdCode, tBlock->_items + sIdx, cnt);
		return slice;
	}
}


bool WtDataReader::cacheFinalBarsFromLoader(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	if (NULL == _loader)
		return false;

	CodeHelper::CodeInfo* cInfo = (CodeHelper::CodeInfo*)codeInfo;

	BarsList& barList = _bars_cache[key];
	barList._code = stdCode;
	barList._period = period;
	barList._exchg = cInfo->_exchg;

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "m1"; break;
	case KP_Minute5: pname = "m5"; break;
	case KP_DAY: pname = "d"; break;
	default: pname = ""; break;
	}

	pipe_reader_log(_sink,LL_INFO, "Reading final bars of {} via extended loader...", stdCode);

	bool ret = _loader->loadFinalHisBars(&barList, stdCode, period, [](void* obj, WTSBarStruct* firstBar, uint32_t count) {
		BarsList* bars = (BarsList*)obj;
		bars->_factor = 1.0;
		bars->_bars.resize(count);
		memcpy(bars->_bars.data(), firstBar, sizeof(WTSBarStruct)*count);
	});

	if(ret)
		pipe_reader_log(_sink,LL_INFO, "{} items of back {} data of {} loaded via extended loader", barList._bars.size(), pname.c_str(), stdCode);

	return ret;
}


bool WtDataReader::cacheIntegratedBars(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo* cInfo = (CodeHelper::CodeInfo*)codeInfo;

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _base_data_mgr->calcTradingDate(cInfo->stdCommID(), curDate, curTime, false);

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
	barList._exchg = cInfo->_exchg;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;

	//const char* hot_flag = cInfo->isHot() ? FILE_SUF_HOT : FILE_SUF_2ND;
	const char* ruleTag = cInfo->_ruletag;

	//�Ȱ���HOT������ж�ȡ, ��rb.HOT
	std::vector<WTSBarStruct>* hotAy = NULL;
	uint64_t lastHotTime = 0;

	do
	{
		/*
		 *	By Wesley @ 2021.12.20
		 *	����������Ҫ�ȵ���_loader->loadRawHisBars���ⲿ��������ȡ������Լ���ݵ�
		 *	�����ϲ�����һ��loadFinalHisBars�������ٵ���loadRawHisBars�������ˣ�����ֱ������
		 */

		std::stringstream ss;
		ss << _his_dir << pname << "/" << cInfo->_exchg << "/" << cInfo->_exchg << "." << cInfo->_product << "_" << ruleTag;
		if (cInfo->isExright())
			ss << (cInfo->_exright == 1 ? SUFFIX_QFQ : SUFFIX_HFQ);
		ss << ".dsb";
		std::string filename = ss.str();
		if (!StdFile::exists(filename.c_str()))
			break;

		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisKlineBlock))
		{
			pipe_reader_log(_sink, LL_ERROR, "��ʷK�������ļ�{}��СУ��ʧ��", filename);
			break;
		}
		proc_block_data(content, true, false);

		if (content.empty())
			break;

		uint32_t barcnt = content.size() / sizeof(WTSBarStruct);

		hotAy = new std::vector<WTSBarStruct>();
		hotAy->resize(barcnt);
		memcpy(hotAy->data(), content.data(), content.size());

		if (period != KP_DAY)
			lastHotTime = hotAy->at(barcnt - 1).time;
		else
			lastHotTime = hotAy->at(barcnt - 1).date;

		pipe_reader_log(_sink, LL_INFO, "{} items of back {} data of wrapped contract {} directly loaded", barcnt, pname.c_str(), stdCode);
	} while (false);
	

	HotSections secs;
	if (strlen(ruleTag) > 0)
	{
		if (!_hot_mgr->splitCustomSections(ruleTag, cInfo->stdCommID(), 19900102, endTDate, secs))
			return false;
	}

	if (secs.empty())
		return false;

	//���ݸ�Ȩ����ȷ����������
	//�����ǰ��Ȩ������ʷ���ݻ��С�������һ����Ȩ����Ϊ��������
	//����Ǻ�Ȩ���������ݻ��󣬻�������Ϊ1
	double baseFactor = 1.0;
	if (cInfo->_exright == 1)
		baseFactor = secs.back()._factor;
	else if (cInfo->_exright == 2)
		barList._factor = secs.back()._factor;

	bool bAllCovered = false;
	for (auto it = secs.rbegin(); it != secs.rend(); it++)
	{
		const HotSection& hotSec = *it;
		const char* curCode = hotSec._code.c_str();
		uint32_t rightDt = hotSec._e_date;
		uint32_t leftDt = hotSec._s_date;

		//Ҫ�Ƚ�����ת��Ϊ�߽�ʱ��
		WTSBarStruct sBar, eBar;
		if (period != KP_DAY)
		{
			uint64_t sTime = _base_data_mgr->getBoundaryTime(cInfo->stdCommID(), leftDt, false, true);
			uint64_t eTime = _base_data_mgr->getBoundaryTime(cInfo->stdCommID(), rightDt, false, false);

			sBar.date = leftDt;
			sBar.time = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

			if (sBar.time < lastHotTime)	//����߽�ʱ��С�����������һ��Bar��ʱ��, ˵���Ѿ��н�����, ����Ҫ�ٴ�����
			{
				bAllCovered = true;
				sBar.time = lastHotTime + 1;
			}

			eBar.date = rightDt;
			eBar.time = ((uint32_t)(eTime / 10000) - 19900000) * 10000 + (uint32_t)(eTime % 10000);

			if (eBar.time <= lastHotTime)	//�ұ߽�ʱ��С�����һ��Hotʱ��, ˵��ȫ��������, û�����ҵı�Ҫ��
				break;
		}
		else
		{
			sBar.date = leftDt;
			if (sBar.date < lastHotTime)	//����߽�ʱ��С�����������һ��Bar��ʱ��, ˵���Ѿ��н�����, ����Ҫ�ٴ�����
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
		 *	�ȴ�extloader��ȡ���º�Լ��K������
		 *	���û�ж������ٴ��ļ���ȡ
		 */
		bool bLoaded = false;
		std::string buffer;
		if (NULL != _loader)
		{
			std::string wCode = fmt::format("{}.{}.{}", cInfo->_exchg, cInfo->_product, (char*)curCode + strlen(cInfo->_product));
			bLoaded = _loader->loadRawHisBars(&buffer, wCode.c_str(), period, [](void* obj, WTSBarStruct* bars, uint32_t count) {
				std::string* buff = (std::string*)obj;
				buff->resize(sizeof(WTSBarStruct)*count);
				memcpy((void*)buff->c_str(), bars, sizeof(WTSBarStruct)*count);
			});
		}

		if (!bLoaded)
		{
			std::stringstream ss;
			ss << _his_dir << pname << "/" << cInfo->_exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				pipe_reader_log(_sink, LL_ERROR, "Sizechecking of his dta file {} failed", filename.c_str());
				return false;
			}
			proc_block_data(content, true, false);	
			buffer.swap(content);
		}
		
		if(buffer.empty())
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
		if ((period == KP_DAY && pBar->date < sBar.date) || (period != KP_DAY && pBar->time < sBar.time))	//���ڱ߽�ʱ��
		{
			//���ڱ߽�ʱ��, ˵��û��������, ��Ϊlower_bound�᷵�ش��ڵ���Ŀ��λ�õ�����
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

		if(cInfo->isExright())
		{	
			double factor = hotSec._factor / baseFactor;
			for (uint32_t idx = sIdx; idx <= eIdx; idx++)
			{
				firstBar[idx].open *= factor;
				firstBar[idx].high *= factor;
				firstBar[idx].low *= factor;
				firstBar[idx].close *= factor;
			}
		}		

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

	pipe_reader_log(_sink,LL_INFO, "{} items of back {} data of {} cached", realCnt, pname.c_str(), stdCode);

	return true;
}

bool WtDataReader::cacheAdjustedStkBars(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo* cInfo = (CodeHelper::CodeInfo*)codeInfo;

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _base_data_mgr->calcTradingDate(cInfo->stdCommID(), curDate, curTime, false);

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
	barList._exchg = cInfo->_exchg;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;

	std::vector<WTSBarStruct>* ayAdjusted = NULL;
	uint64_t lastQTime = 0;

	do
	{
		/*
		 *	By Wesley @ 2021.12.20
		 *	����������Ҫ�ȵ���_loader->loadRawHisBars���ⲿ��������ȡ��Ȩ���ݵ�
		 *	�����ϲ�����һ��loadFinalHisBars�������ٵ���loadRawHisBars�������ˣ�����ֱ������
		 */
		char flag = cInfo->_exright == 1 ? SUFFIX_QFQ : SUFFIX_HFQ;
		std::stringstream ss;
		ss << _his_dir << pname << "/" << cInfo->_exchg << "/" << cInfo->_code << flag << ".dsb";
		std::string filename = ss.str();
		if (!StdFile::exists(filename.c_str()))
			break;

		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisKlineBlock))
		{
			pipe_reader_log(_sink,LL_ERROR, "��ʷK�������ļ�{}��СУ��ʧ��", filename.c_str());
			break;
		}

		proc_block_data(content, true, false);

		uint32_t barcnt = content.size() / sizeof(WTSBarStruct);

		ayAdjusted = new std::vector<WTSBarStruct>();
		ayAdjusted->resize(barcnt);
		memcpy(ayAdjusted->data(), content.data(), content.size());

		if (period != KP_DAY)
			lastQTime = ayAdjusted->at(barcnt - 1).time;
		else
			lastQTime = ayAdjusted->at(barcnt - 1).date;

		pipe_reader_log(_sink,LL_INFO, "{} items of adjusted back {} data of stock {} directly loaded", barcnt, pname.c_str(), stdCode);
	} while (false);


	bool bAllCovered = false;
	do
	{
		const char* curCode = cInfo->_code;

		//Ҫ�Ƚ�����ת��Ϊ�߽�ʱ��
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
		 *	�ȴ�extloader��ȡ
		 *	���û�ж������ٴ��ļ���ȡ
		 */
		bool bLoaded = false;
		std::string buffer;
		std::string rawCode = fmt::format("{}.{}.{}", cInfo->_exchg, cInfo->_product, curCode);
		if (NULL != _loader)
		{
			bLoaded = _loader->loadRawHisBars(&buffer, rawCode.c_str(), period, [](void* obj, WTSBarStruct* bars, uint32_t count) {
				std::string* buff = (std::string*)obj;
				buff->resize(sizeof(WTSBarStruct)*count);
				memcpy((void*)buff->c_str(), bars, sizeof(WTSBarStruct)*count);
			});
		}

		bool bOldVer = false;
		if (!bLoaded)
		{
			std::stringstream ss;
			ss << _his_dir << pname << "/" << cInfo->_exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷK�������ļ�{}��СУ��ʧ��", filename.c_str());
				return false;
			}

			proc_block_data(content, true, false);
			buffer.swap(content);
		}

		if(buffer.empty())
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

			std::vector<WTSBarStruct>* ayRaw = new std::vector<WTSBarStruct>();
			ayRaw->resize(curCnt);
			memcpy(ayRaw->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
			realCnt += curCnt;

			auto& ayFactors = getAdjFactors(cInfo->_code, cInfo->_exchg, cInfo->_product);
			if (!ayFactors.empty())
			{
				//����Ȩ����
				int32_t lastIdx = curCnt;
				WTSBarStruct bar;
				firstBar = ayRaw->data();

				//���ݸ�Ȩ����ȷ����������
				//�����ǰ��Ȩ������ʷ���ݻ��С�������һ����Ȩ����Ϊ��������
				//����Ǻ�Ȩ���������ݻ��󣬻�������Ϊ1
				double baseFactor = 1.0;
				if (cInfo->_exright == 1)
					baseFactor = ayFactors.back()._factor;
				else if (cInfo->_exright == 2)
					barList._factor = ayFactors.back()._factor;

				for (auto it = ayFactors.rbegin(); it != ayFactors.rend(); it++)
				{
					const AdjFactor& adjFact = *it;
					bar.date = adjFact._date;

					//��������
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

			barsSections.emplace_back(ayRaw);
		}
	} while (false);

	if (ayAdjusted)
	{
		barsSections.emplace_back(ayAdjusted);
		realCnt += ayAdjusted->size();
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

	pipe_reader_log(_sink,LL_INFO, "{} items of back {} data of {} cached", realCnt, pname.c_str(), stdCode);

	return true;
}

bool WtDataReader::cacheHisBarsFromFile(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo* cInfo = (CodeHelper::CodeInfo*)codeInfo;
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo->_exchg, cInfo->_product);
	const char* stdPID = commInfo->getFullPid();

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _base_data_mgr->calcTradingDate(cInfo->stdCommID(), curDate, curTime, false);

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
	barList._exchg = cInfo->_exchg;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;
	const char* ruleTag = cInfo->_ruletag;
	if (strlen(ruleTag) > 0)
	{
		//����Ƕ�ȡ�ڻ�������������
		return cacheIntegratedBars(cInfo, key, stdCode, period);
	}
	else if(cInfo->isExright() && commInfo->isStock())
	{
		//����Ƕ�ȡ��Ʊ��Ȩ����
		return cacheAdjustedStkBars(cInfo, key, stdCode, period);
	}

	
	//ֱ��ԭʼ����ֱ�Ӽ���

	/*
	 *	By Wesley @ 2021.12.20
	 *	�ȴ�extloader��ȡ
	 *	���û�ж������ٴ��ļ���ȡ
	 */
	bool bLoaded = false;
	std::string buffer;
	if (NULL != _loader)
	{
		bLoaded = _loader->loadRawHisBars(&buffer, stdCode, period, [](void* obj, WTSBarStruct* bars, uint32_t count) {
			std::string* buff = (std::string*)obj;
			buff->resize(sizeof(WTSBarStruct)*count);
			memcpy((void*)buff->c_str(), bars, sizeof(WTSBarStruct)*count);
		});
	}

	if (!bLoaded)
	{
		//��ȡ��ʷ��
		std::stringstream ss;
		ss << _his_dir << pname << "/" << cInfo->_exchg << "/" << cInfo->_code << ".dsb";
		std::string filename = ss.str();
		if (StdFile::exists(filename.c_str()))
		{
			//����и�ʽ������ʷ�����ļ�, ��ֱ�Ӷ�ȡ
			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				pipe_reader_log(_sink,LL_ERROR, "��ʷK�������ļ�{}��СУ��ʧ��", filename.c_str());
				return false;
			}

			proc_block_data(content, true, false);
			buffer.swap(content);
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

	pipe_reader_log(_sink,LL_INFO, "{} items of back {} data of {} cached", realCnt, pname.c_str(), stdCode);
	return true;
}

WTSKlineSlice* WtDataReader::readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	const char* stdPID = cInfo.stdCommID();

	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, period);
	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		/*
		 *	By Wesley @ 2021.12.20
		 *	�ȴ�extloader�������յ�K�����ݣ�����Ǹ�Ȩ��
		 *	�������ʧ�ܣ����ٴ��ļ�����K������
		 */
		bHasHisData = cacheFinalBarsFromLoader(&cInfo, key, stdCode, period);

		if(!bHasHisData)
			bHasHisData = cacheHisBarsFromFile(&cInfo, key, stdCode, period);
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

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID, curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID, 0, 0, false);
	
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

	//�Ƿ���������
	bool bHasToday = (endTDate == curTDate);

	//By Wesley @ 2022.05.28
	//����Ҫ�����Ƿ����ڻ���
	const char* ruleTag = cInfo._ruletag;
	if (strlen(ruleTag) > 0)
	{
		_bars_cache[key]._raw_code = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), curTDate);
		pipe_reader_log(_sink, LL_INFO, "{} contract on {} confirmed: {} -> {}", ruleTag, curTDate, stdCode, _bars_cache[key]._raw_code.c_str());
	}
	else
	{
		_bars_cache[key]._raw_code = cInfo._code;
	}

	/*
	if (commInfo->isFuture())
	{
		const char* ruleTag = cInfo._ruletag;
		if (strlen(ruleTag) > 0)
		{
			_bars_cache[key]._raw_code = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), curTDate);
			pipe_reader_log(_sink, LL_INFO, "{} contract on {} confirmed with rule {}: {} -> {}", ruleTag, curTDate, stdCode, _bars_cache[key]._raw_code.c_str());
		}
		//else if (cInfo.isHot())
		//{
		//	_bars_cache[key]._raw_code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, curTDate);
		//	pipe_reader_log(_sink, LL_INFO, "Hot contract on {}  confirmed: {} -> {}", curTDate, stdCode, _bars_cache[key]._raw_code.c_str());
		//}
		//else if (cInfo.isSecond())
		//{
		//	_bars_cache[key]._raw_code = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, curTDate);
		//	pipe_reader_log(_sink, LL_INFO, "Second contract on {} confirmed: {} -> {}", curTDate, stdCode, _bars_cache[key]._raw_code.c_str());
		//}
		else
		{
			_bars_cache[key]._raw_code = cInfo._code;
		}
	}
	else
	{
		_bars_cache[key]._raw_code = cInfo._code;
	}
	*/

	if (bHasToday)
	{
		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;

		const char* curCode = _bars_cache[key]._raw_code.c_str();

		//��ȡʵʱ��
		RTKlineBlockPair* kPair = getRTKilneBlock(cInfo._exchg, curCode, period);
		if (kPair != NULL)
		{
			//��ȡ���յ�����
			WTSBarStruct* pBar = NULL;
			pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + (kPair->_block->_size - 1), bar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
				if (period == KP_DAY)
					return a.date < b.date;
				else
					return a.time < b.time;
			});

			uint32_t idx = 0;
			if (pBar != NULL)
				idx = pBar - kPair->_block->_bars;
			else
				idx = kPair->_block->_size;

			if ((period == KP_DAY && pBar->date > bar.date) || (period != KP_DAY && pBar->time > bar.time))
			{
				pBar--;
				idx--;
			}

			uint32_t sIdx = 0;
			if (left <= idx + 1)
			{
				sIdx = idx - left + 1;
			}

			uint32_t curCnt = (idx - sIdx + 1);
			left -= (idx - sIdx + 1);

			//By Wesley @ 2022.05.28
			//������ԼҲҪ֧�ָ�Ȩ
			if(cInfo._exright == 2/* && commInfo->isStock()*/)
			{
				//��Ȩ����Ҫ�����µ����ݽ��и�Ȩ��������Ҫ��Ϊ��ʷ����׷�ӵ�β��
				//��Ȼ��Ȩ����Ҫ���и�Ȩ��������ʵʱ���ݵ�λ�ñ��ҲҪ���µ����£���ȻOnMinuteEnd��ӿ��̿�ʼ�طŵ�
				_bars_cache[key]._rt_cursor = idx;

				BarsList& barsList = _bars_cache[key];
				double factor = barsList._factor;
				uint32_t oldSize = barsList._bars.size();
				uint32_t newSize = oldSize + curCnt;
				barsList._bars.resize(newSize);
				memcpy(&barsList._bars[oldSize], &kPair->_block->_bars[sIdx], sizeof(WTSBarStruct)*curCnt);
				for(uint32_t thisIdx = oldSize; thisIdx < newSize; thisIdx++)
				{
					WTSBarStruct* pBar = &barsList._bars[thisIdx];
					pBar->open *= factor;
					pBar->high *= factor;
					pBar->low *= factor;
					pBar->close *= factor;
				}
			}
			else
			{
				_bars_cache[key]._rt_cursor = idx;				
				rtHead = &kPair->_block->_bars[sIdx];
				rtCnt = curCnt;
			}
		}
	}

	if (left > 0 && bHasHisData)
	{
		hisCnt = left;
		//��ʷ���ݣ�ֱ�Ӵӻ������ʷ����β����ȡ
		BarsList& barList = _bars_cache[key];
		hisCnt = min(hisCnt, (uint32_t)barList._bars.size());
		hisHead = &barList._bars[barList._bars.size() - hisCnt];//indexBarFromCache(key, etime, hisCnt, period == KP_DAY);
	}

	pipe_reader_log(_sink, LL_DEBUG, "His {} bars of {} loaded, {} from history, {} from realtime", PERIOD_NAME[period], stdCode, hisCnt, rtCnt);

	if (hisCnt + rtCnt > 0)
	{
		WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, 1, hisHead, hisCnt);
		if (rtCnt > 0)
			slice->appendBlock(rtHead, rtCnt);
		return slice;
	}

	return NULL;
}


WtDataReader::TickBlockPair* WtDataReader::getRTTickBlock(const char* exchg, const char* code)
{
	//std::string key = StrUtil::printf("%s.%s", exchg, code);
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", exchg, code);

	thread_local static char path[256] = { 0 };
	fmtutil::format_to(path, "{}ticks/{}/{}.dmb", _rt_dir.c_str(), exchg, code);

	if (!StdFile::exists(path))
		return NULL;

	TickBlockPair& block = _rt_tick_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTickBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//˵���ļ���С�ѱ�, ��Ҫ����ӳ��
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTickBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::OrdDtlBlockPair* WtDataReader::getRTOrdDtlBlock(const char* exchg, const char* code)
{
	//std::string key = StrUtil::printf("%s.%s", exchg, code);
	//std::string path = StrUtil::printf("%srt/orders/%s/%s.dmb", _base_dir.c_str(), exchg, code);
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", exchg, code);

	thread_local static char path[256] = { 0 };
	fmtutil::format_to(path, "{}orders/{}/{}.dmb", _rt_dir.c_str(), exchg, code);

	if (!StdFile::exists(path))
		return NULL;

	OrdDtlBlockPair& block = _rt_orddtl_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdDtlBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//˵���ļ���С�ѱ�, ��Ҫ����ӳ��
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdDtlBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::OrdQueBlockPair* WtDataReader::getRTOrdQueBlock(const char* exchg, const char* code)
{
	//std::string key = StrUtil::printf("%s.%s", exchg, code);
	//std::string path = StrUtil::printf("%srt/queue/%s/%s.dmb", _base_dir.c_str(), exchg, code);

	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", exchg, code);

	thread_local static char path[256] = { 0 };
	fmtutil::format_to(path, "{}queue/{}/{}.dmb", _rt_dir.c_str(), exchg, code);

	if (!StdFile::exists(path))
		return NULL;

	OrdQueBlockPair& block = _rt_ordque_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdQueBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//˵���ļ���С�ѱ�, ��Ҫ����ӳ��
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdQueBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::TransBlockPair* WtDataReader::getRTTransBlock(const char* exchg, const char* code)
{
	//std::string key = StrUtil::printf("%s.%s", exchg, code);
	//std::string path = StrUtil::printf("%srt/trans/%s/%s.dmb", _base_dir.c_str(), exchg, code);

	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", exchg, code);

	thread_local static char path[256] = { 0 };
	fmtutil::format_to(path, "{}trans/{}/{}.dmb", _rt_dir.c_str(), exchg, code);

	if (!StdFile::exists(path))
		return NULL;

	TransBlockPair& block = _rt_trans_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTransBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//˵���ļ���С�ѱ�, ��Ҫ����ӳ��
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTransBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::RTKlineBlockPair* WtDataReader::getRTKilneBlock(const char* exchg, const char* code, WTSKlinePeriod period)
{
	if (period != KP_Minute1 && period != KP_Minute5)
		return NULL;

	//std::string key = StrUtil::printf("%s.%s", exchg, code);
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}.{}", exchg, code);

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

	//std::string path = StrUtil::printf("%srt/%s/%s/%s.dmb", _base_dir.c_str(), subdir.c_str(), exchg, code);
	thread_local static char path[256] = { 0 };
	fmtutil::format_to(path, "{}{}/{}/{}.dmb", _rt_dir, subdir, exchg, code);

	if (!StdFile::exists(path))
		return NULL;

	RTKlineBlockPair& block = (*cache_map)[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTKlineBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
		pipe_reader_log(_sink, LL_DEBUG, "RT {} block of {}.{} loaded", subdir.c_str(), exchg, code);
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//˵���ļ���С�ѱ�, ��Ҫ����ӳ��
		pipe_reader_log(_sink, LL_DEBUG, "RT {} block of {}.{} expanded to {}, remapping...", subdir.c_str(), exchg, code, block._block->_capacity);

		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path, boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTKlineBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}	

	return &block;
}

void WtDataReader::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */)
{
	//����Ӧ�ô������
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	if (nowTime <= _last_time)
		return;

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = (BarsList&)it->second;
		if (barsList._period != KP_DAY)
		{
			if (!barsList._raw_code.empty())
			{
				RTKlineBlockPair* kBlk = getRTKilneBlock(barsList._exchg.c_str(), barsList._raw_code.c_str(), barsList._period);
				if (kBlk == NULL)
					continue;

				//ȷ����һ�εĶ�ȡ����ʵʱK������
				uint32_t preCnt = 0;
				//���ʵʱK��û�г�ʼ���������Ѷ�ȡ������Ϊ0
				//����Ѿ���ʼ���������Ѷ�ȡ������Ϊ���+1
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
						//������Ǻ�Ȩ����ֱ�ӻص�onbar
						//����Ǻ�Ȩ��������bar��Ȩ�����Ժ���ӵ�cache�У��ٻص�onbar
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

				//����Ѵ����K��������Ϊ0�����޸Ĺ��λ��
				if (preCnt > 0)
					barsList._rt_cursor = preCnt - 1;
			}
		}
		//��һ���߼�û�����ˣ���ʵ���������ǲ���պϵģ�����Ҳ�����ڵ���K�߱պϵ����
		//ʵ���ж�ͨ��ontick������ʵʱ����
		//else if (barsList._period == KP_DAY)
		//{
		//	if (barsList._his_cursor != UINT_MAX && barsList._bars.size() - 1 > barsList._his_cursor)
		//	{
		//		for (;;)
		//		{
		//			WTSBarStruct& nextBar = barsList._bars[barsList._his_cursor + 1];

		//			if (nextBar.date <= endTDate)
		//			{
		//				_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
		//			}
		//			else
		//			{
		//				break;
		//			}

		//			barsList._his_cursor++;

		//			if (barsList._his_cursor == barsList._bars.size() - 1)
		//				break;
		//		}
		//	}
		//}
	}

	if (_sink)
		_sink->on_all_bar_updated(uTime);

	_last_time = nowTime;
}

double WtDataReader::getAdjFactorByDate(const char* stdCode, uint32_t date /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	if (!commInfo->isStock())
		return 1.0;

	AdjFactor factor = { date, 1.0 };

	std::string key = stdCode;
	if (cInfo.isExright())
		key = key.substr(0, key.size() - 1);
	const AdjFactorList& factList = _adj_factors[key];
	if (factList.empty())
		return 1.0;

	auto it = std::lower_bound(factList.begin(), factList.end(), factor, [](const AdjFactor& a, const AdjFactor&b) {
		return a._date < b._date;
	});

	if(it == factList.end())
	{
		//�Ҳ�������˵��Ŀ�����ڴ������һ�������ڣ�ֱ�ӷ������һ����Ȩ����
		return factList.back()._factor;
	}
	else
	{
		//����ҵ��ˣ��������е����ڴ���Ŀ�����ڣ�������һ��
		//�������Ŀ�����ڣ�����������һ��
		if ((*it)._date > date)
			it--;

		return (*it)._factor;
	}
}

const WtDataReader::AdjFactorList& WtDataReader::getAdjFactors(const char* code, const char* exchg, const char* pid)
{
	thread_local static char key[20] = { 0 };
	fmtutil::format_to(key, "{}.{}.{}", exchg, pid, code);

	auto it = _adj_factors.find(key);
	if (it == _adj_factors.end())
	{
		//By Wesley @ 2021.12.21
		//���û�и�Ȩ���ӣ��ʹ�extloader�����һ��
		if (_loader)
		{
			if(_sink) pipe_reader_log(_sink,LL_INFO, "No adjusting factors of {} cached, searching via extented loader...", key);
			_loader->loadAdjFactors(this, key, [](void* obj, const char* stdCode, uint32_t* dates, double* factors, uint32_t count) {
				WtDataReader* self = (WtDataReader*)obj;
				AdjFactorList& fctrLst = self->_adj_factors[stdCode];

				for (uint32_t i = 0; i < count; i++)
				{
					AdjFactor adjFact;
					adjFact._date = dates[i];
					adjFact._factor = factors[i];

					fctrLst.emplace_back(adjFact);
				}

				//һ��Ҫ�ѵ�һ���ӽ�ȥ����Ȼ�����ǰ��Ȩ�Ļ������ܻ�©�������������
				AdjFactor adjFact;
				adjFact._date = 19900101;
				adjFact._factor = 1;
				fctrLst.emplace_back(adjFact);

				std::sort(fctrLst.begin(), fctrLst.end(), [](const AdjFactor& left, const AdjFactor& right) {
					return left._date < right._date;
				});

				pipe_reader_log(self->_sink, LL_INFO, "{} items of adjusting factors of {} loaded via extended loader", count, stdCode);
			});
		}
	}

	return _adj_factors[key];
}