#include "WtRdmDtReader.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSSessionInfo.hpp"

#include "../WTSUtils/WTSCmpHelper.hpp"
#include "../WTSUtils/WTSCfgLoader.h"

#include <rapidjson/document.h>
namespace rj = rapidjson;

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void pipe_rdmreader_log(IRdmDtReaderSink* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	fmtutil::format_to(buffer, format, args...);

	sink->reader_log(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG IRdmDtReader* createRdmDtReader()
	{
		IRdmDtReader* ret = new WtRdmDtReader();
		return ret;
	}

	EXPORT_FLAG void deleteRdmDtReader(IRdmDtReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};

/*
 *	���������
 */
extern bool proc_block_data(std::string& content, bool isBar, bool bKeepHead = true);

WtRdmDtReader::WtRdmDtReader()
	: _base_data_mgr(NULL)
	, _hot_mgr(NULL)
	, _stopped(false)
{
}


WtRdmDtReader::~WtRdmDtReader()
{
	_stopped = true;
	if (_thrd_check)
		_thrd_check->join();
}

void WtRdmDtReader::init(WTSVariant* cfg, IRdmDtReaderSink* sink)
{
	_sink = sink;

	_base_data_mgr = _sink->get_basedata_mgr();
	_hot_mgr = _sink->get_hot_mgr();

	if (cfg == NULL)
		return ;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);

	bool bAdjLoaded = false;
	
	if (!bAdjLoaded && cfg->has("adjfactor"))
		loadStkAdjFactorsFromFile(cfg->getCString("adjfactor"));

	_thrd_check.reset(new StdThread([this]() {
		while(!_stopped)
		{
			std::this_thread::sleep_for(std::chrono::seconds(5));
			uint64_t now = TimeUtils::getLocalTimeNow();

			for(auto& m : _rt_tick_map)
			{
				//���5����֮��û�з��ʣ����ͷŵ�
				TickBlockPair& tPair = (TickBlockPair&)m.second;
				if(now > tPair._last_time + 300000 && tPair._block != NULL)
				{	
					StdUniqueLock lock(*tPair._mtx);
					tPair._block = NULL;
					tPair._file.reset();
				}
			}

			for (auto& m : _rt_ordque_map)
			{
				//���5����֮��û�з��ʣ����ͷŵ�
				OrdQueBlockPair& tPair = (OrdQueBlockPair&)m.second;
				if (now > tPair._last_time + 300000 && tPair._block != NULL)
				{
					StdUniqueLock lock(*tPair._mtx);
					tPair._block = NULL;
					tPair._file.reset();
				}
			}

			for (auto& m : _rt_orddtl_map)
			{
				//���5����֮��û�з��ʣ����ͷŵ�
				OrdDtlBlockPair& tPair = (OrdDtlBlockPair&)m.second;
				if (now > tPair._last_time + 300000 && tPair._block != NULL)
				{
					StdUniqueLock lock(*tPair._mtx);
					tPair._block = NULL;
					tPair._file.reset();
				}
			}

			for (auto& m : _rt_trans_map)
			{
				//���5����֮��û�з��ʣ����ͷŵ�
				TransBlockPair& tPair = (TransBlockPair&)m.second;
				if (now > tPair._last_time + 300000 && tPair._block != NULL)
				{
					StdUniqueLock lock(*tPair._mtx);
					tPair._block = NULL;
					tPair._file.reset();
				}
			}

			for (auto& m : _rt_min1_map)
			{
				//���5����֮��û�з��ʣ����ͷŵ�
				RTKlineBlockPair& tPair = (RTKlineBlockPair&)m.second;
				if (now > tPair._last_time + 300000 && tPair._block != NULL)
				{
					StdUniqueLock lock(*tPair._mtx);
					tPair._block = NULL;
					tPair._file.reset();
				}
			}

			for (auto& m : _rt_min5_map)
			{
				//���5����֮��û�з��ʣ����ͷŵ�
				RTKlineBlockPair& tPair = (RTKlineBlockPair&)m.second;
				if (now > tPair._last_time + 300000 && tPair._block != NULL)
				{
					StdUniqueLock lock(*tPair._mtx);
					tPair._block = NULL;
					tPair._file.reset();
				}
			}
		}
	}));
}


bool WtRdmDtReader::loadStkAdjFactorsFromFile(const char* adjfile)
{
	if (!StdFile::exists(adjfile))
	{
		pipe_rdmreader_log(_sink, LL_ERROR, "Adjusting factors file {} not exists", adjfile);
		return false;
	}

	WTSVariant* doc = WTSCfgLoader::load_from_file(adjfile, true);
	if (doc == NULL)
	{
		pipe_rdmreader_log(_sink, LL_ERROR, "Loading adjusting factors file {} failed", adjfile);
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

	pipe_rdmreader_log(_sink, LL_INFO, "{} adjusting factors of {} tickers loaded", fct_cnt, stk_cnt);
	doc->release();
	return true;
}

WTSTickSlice* WtRdmDtReader::readTickSliceByDate(const char* stdCode, uint32_t uDate )
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);
	bool isToday = (uDate == curTDate);

	//����ĳ�С�ڵ��ڣ���Ҫ����̺��ȡ�����
	//����Ѿ�����������ҵ��ʵʱ���ݾͶ�������
	if (uDate <= curTDate)
	{
		std::string curCode = cInfo._code;
		std::string hotCode;
		if (commInfo->isFuture())
		{
			const char* ruleTag = cInfo._ruletag;
			if(strlen(ruleTag) > 0)
			{
				curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), uDate);
				pipe_rdmreader_log(_sink, LL_INFO, "{} contract on {} confirmed with rule {}: {} -> {}", ruleTag, uDate, stdCode, curCode.c_str());
				hotCode = cInfo._product;
				hotCode += "_";
				hotCode += ruleTag;
			}
			//else if (cInfo.isHot())
			//{
			//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, uDate);
			//	pipe_rdmreader_log(_sink, LL_INFO, "Hot contract on {} confirmed: {} -> {}", uDate, stdCode, curCode.c_str());
			//	hotCode = cInfo._product;
			//	hotCode += "_HOT";
			//}
			//else if (cInfo.isSecond())
			//{
			//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, uDate);
			//	pipe_rdmreader_log(_sink, LL_INFO, "Second contract on {} confirmed: {} -> {}", uDate, stdCode, curCode.c_str());
			//	hotCode = cInfo._product;
			//	hotCode += "_2ND";
			//}
		}

		std::string key = fmt::format("{}-{}", stdCode, uDate);

		auto it = _his_tick_map.find(key);
		bool bHasHisTick = (it != _his_tick_map.end());
		if (!bHasHisTick)
		{
			for (;;)
			{
				std::string filename;
				bool bHitHot = false;
				if (!hotCode.empty())
				{
					std::stringstream ss;
					ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << hotCode << ".dsb";
					filename = ss.str();
					if (StdFile::exists(filename.c_str()))
					{
						bHitHot = true;
					}
				}

				if (!bHitHot)
				{
					std::stringstream ss;
					ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << curCode << ".dsb";
					filename = ss.str();
					if (!StdFile::exists(filename.c_str()))
					{
						break;
					}
				}

				HisTBlockPair& tBlkPair = _his_tick_map[key];
				StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
				if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
				{
					pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of tick data file {} failed", filename.c_str());
					tBlkPair._buffer.clear();
					break;
				}

				proc_block_data(tBlkPair._buffer, false, true);
				tBlkPair._block = (HisTickBlock*)tBlkPair._buffer.c_str();
				bHasHisTick = true;
				break;
			}
		}

		while (bHasHisTick)
		{
			HisTBlockPair& tBlkPair = _his_tick_map[key];
			if (tBlkPair._block == NULL)
				break;

			HisTickBlock* tBlock = tBlkPair._block;

			uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
			if (tcnt <= 0)
				break;

			WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks, tcnt);
			return slice;

			break;
		}
	}
	
	while(isToday)
	{
		std::string curCode = cInfo._code;
		if(commInfo->isFuture())
		{
			const char* ruleTag = cInfo._ruletag;
			if (strlen(ruleTag) > 0)
				curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), curTDate);
			//else if (cInfo.isHot())
			//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, curTDate);
			//else if (cInfo.isSecond())
			//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, curTDate);
		}
		

		TickBlockPair* tPair = getRTTickBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL || tPair->_block->_size == 0)
			break;

		StdUniqueLock lock(*tPair->_mtx);
		RTTickBlock* tBlock = tPair->_block;
		
		WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks, tBlock->_size);
		return slice;
	}

	return NULL;
}

WTSTickSlice* WtRdmDtReader::readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	WTSSessionInfo* sInfo = _base_data_mgr->getSession(_base_data_mgr->getCommodity(cInfo._exchg, cInfo._code)->getSession());

	uint32_t rDate, rTime, rSecs;
	//20190807124533900
	rDate = (uint32_t)(etime / 1000000000);
	rTime = (uint32_t)(etime % 1000000000) / 100000;
	rSecs = (uint32_t)(etime % 100000);

	uint32_t lDate, lTime, lSecs;
	//20190807124533900
	lDate = (uint32_t)(stime / 1000000000);
	lTime = (uint32_t)(stime % 1000000000) / 100000;
	lSecs = (uint32_t)(stime % 100000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t beginTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), lDate, lTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

	bool hasToday = (endTDate >= curTDate);

	WTSTickSlice* slice = WTSTickSlice::create(stdCode, NULL, 0);

	WTSTickStruct sTick;
	sTick.action_date = lDate;
	sTick.action_time = lTime * 100000 + lSecs;
	
	uint32_t nowTDate = beginTDate;
	while(nowTDate < curTDate)
	{
		std::string curCode = cInfo._code;
		std::string hotCode;
		if(commInfo->isFuture())
		{
			const char* ruleTag = cInfo._ruletag;
			if (strlen(ruleTag) > 0)
			{
				curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), nowTDate);

				pipe_rdmreader_log(_sink, LL_INFO, "{} contract on {} confirmed: {} -> {}", ruleTag, curTDate, stdCode, curCode.c_str());
				hotCode = cInfo._product;
				hotCode += "_";
				hotCode += ruleTag;
			}
			//else if (cInfo.isHot() && commInfo->isFuture())
			//{
			//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, nowTDate);
			//	pipe_rdmreader_log(_sink, LL_INFO, "Hot contract on {} confirmed: {} -> {}", curTDate, stdCode, curCode.c_str());
			//	hotCode = cInfo._product;
			//	hotCode += "_HOT";
			//}
			//else if (cInfo.isSecond() && commInfo->isFuture())
			//{
			//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, nowTDate);
			//	pipe_rdmreader_log(_sink, LL_INFO, "Second contract on {} confirmed: {} -> {}", curTDate, stdCode, curCode.c_str());
			//	hotCode = cInfo._product;
			//	hotCode += "_2ND";
			//}
		}
		

		std::string key = fmt::format("{}-{}", stdCode, nowTDate);

		auto it = _his_tick_map.find(key);
		bool bHasHisTick = (it != _his_tick_map.end());
		if(!bHasHisTick)
		{
			for(;;)
			{
				std::string filename;
				bool bHitHot = false;
				if (!hotCode.empty())
				{
					std::stringstream ss;
					ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << nowTDate << "/" << hotCode << ".dsb";
					filename = ss.str();
					if (StdFile::exists(filename.c_str()))
					{
						bHitHot = true;
					}
				}

				if (!bHitHot)
				{
					std::stringstream ss;
					ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << nowTDate << "/" << curCode << ".dsb";
					filename = ss.str();
					if (!StdFile::exists(filename.c_str()))
					{
						break;
					}
				}

				HisTBlockPair& tBlkPair = _his_tick_map[key];
				StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
				if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
				{
					pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of tick data file {} failed", filename.c_str());
					tBlkPair._buffer.clear();
					break;
				}

				proc_block_data(tBlkPair._buffer, false, true);
				tBlkPair._block = (HisTickBlock*)tBlkPair._buffer.c_str();
				bHasHisTick = true;
				break;
			}
		}
		
		while(bHasHisTick)
		{
			//�Ƚ�ʱ��Ķ���
			WTSTickStruct eTick;
			if(nowTDate == endTDate)
			{
				eTick.action_date = rDate;
				eTick.action_time = rTime * 100000 + rSecs;
			}
			else
			{
				eTick.action_date = nowTDate;
				eTick.action_time = sInfo->getCloseTime() * 100000 + 59999;
			}

			HisTBlockPair& tBlkPair = _his_tick_map[key];
			if (tBlkPair._block == NULL)
				break;

			HisTickBlock* tBlock = tBlkPair._block;

			uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
			if (tcnt <= 0)
				break;

			WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tcnt - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t eIdx = pTick - tBlock->_ticks;
			if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
			{
				pTick--;
				eIdx--;
			}

			if (beginTDate != nowTDate)
			{
				//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
				//WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks, eIdx + 1);
				//ayTicks->append(slice, false);
				slice->appendBlock(tBlock->_ticks, eIdx + 1);
			}
			else
			{
				//�����������ͬ���������ʼ��λ��
				pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + eIdx, sTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
					if (a.action_date != b.action_date)
						return a.action_date < b.action_date;
					else
						return a.action_time < b.action_time;
				});

				std::size_t sIdx = pTick - tBlock->_ticks;
				//WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, eIdx - sIdx + 1);
				//ayTicks->append(slice, false);
				slice->appendBlock(tBlock->_ticks + sIdx, eIdx - sIdx + 1);
			}

			break;
		}
		
		nowTDate = TimeUtils::getNextDate(nowTDate);
	}

	while(hasToday)
	{
		std::string curCode = cInfo._code;
		if (commInfo->isFuture())
		{
			const char* ruleTag = cInfo._ruletag;
			if (strlen(ruleTag) > 0)
				curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), curTDate);
			//else if (cInfo.isHot())
			//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, curTDate);
			//else if (cInfo.isSecond())
			//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, curTDate);
		}

		TickBlockPair* tPair = getRTTickBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL || tPair->_block->_size == 0)
			break;

		StdUniqueLock lock(*tPair->_mtx);
		RTTickBlock* tBlock = tPair->_block;
		WTSTickStruct eTick;
		if (curTDate == endTDate)
		{
			eTick.action_date = rDate;
			eTick.action_time = rTime * 100000 + rSecs;
		}
		else
		{
			eTick.action_date = curTDate;
			eTick.action_time = sInfo->getCloseTime() * 100000 + 59999;
		}

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tBlock->_size - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		std::size_t eIdx = pTick - tBlock->_ticks;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pTick->action_date > eTick.action_date || pTick->action_time > eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		if (beginTDate != curTDate)
		{
			//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
			//WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks, eIdx + 1);
			//ayTicks->append(slice, false);
			slice->appendBlock(tBlock->_ticks, eIdx + 1);
		}
		else
		{
			//�����������ͬ���������ʼ��λ��
			pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + eIdx, sTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pTick - tBlock->_ticks;
			//WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, eIdx - sIdx + 1);
			//ayTicks->append(slice, false);
			slice->appendBlock(tBlock->_ticks + sIdx, eIdx - sIdx + 1);
		}
		break;
	}

	return slice;
}

WTSOrdQueSlice* WtRdmDtReader::readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	uint32_t rDate, rTime, rSecs;
	//20190807124533900
	rDate = (uint32_t)(etime / 1000000000);
	rTime = (uint32_t)(etime % 1000000000) / 100000;
	rSecs = (uint32_t)(etime % 100000);

	uint32_t lDate, lTime, lSecs;
	//20190807124533900
	lDate = (uint32_t)(stime / 1000000000);
	lTime = (uint32_t)(stime % 1000000000) / 100000;
	lSecs = (uint32_t)(stime % 100000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t beginTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), lDate, lTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

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
	eTick.action_date = rDate;
	eTick.action_time = rTime * 100000 + rSecs;

	WTSOrdQueStruct sTick;
	sTick.action_date = lDate;
	sTick.action_time = lTime * 100000 + lSecs;

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

		std::size_t eIdx = pItem - rtBlock->_queues;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		if (beginTDate != endTDate)
		{
			//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
			WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, rtBlock->_queues, eIdx + 1);
			return slice;
		}
		else
		{
			//�����������ͬ���������ʼ��λ��
			pItem = std::lower_bound(rtBlock->_queues, rtBlock->_queues + eIdx, sTick, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pItem - rtBlock->_queues;
			WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, rtBlock->_queues + sIdx, eIdx - sIdx + 1);
			return slice;
		}
	}
	else
	{
		std::string key = fmt::format("{}-{}", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/queue/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisOrdQueBlockPair& hisBlkPair = _his_ordque_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisOrdQueBlockV2))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of orderqueue data file {} failed", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisOrdQueBlockV2* tBlockV2 = (HisOrdQueBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisOrdQueBlockV2) + tBlockV2->_size))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of orderqueue data file {} failed", filename.c_str());
				return NULL;
			}

			//��Ҫ��ѹ
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//��ԭ����bufferֻ����һ��ͷ��,��������tick����׷�ӵ�β��
			hisBlkPair._buffer.resize(sizeof(HisOrdQueBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW;

			hisBlkPair._block = (HisOrdQueBlock*)hisBlkPair._buffer.c_str();
		}

		HisOrdQueBlockPair& tBlkPair = _his_ordque_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisOrdQueBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisOrdQueBlock)) / sizeof(WTSOrdQueStruct);
		if (tcnt <= 0)
			return NULL;

		WTSOrdQueStruct* pItem = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		std::size_t eIdx = pItem - tBlock->_items;
		if (pItem->action_date > eTick.action_date || pItem->action_time >= eTick.action_time)
		{
			pItem--;
			eIdx--;
		}


		if (beginTDate != endTDate)
		{
			//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
			WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, tBlock->_items, eIdx + 1);
			return slice;
		}
		else
		{
			//�����������ͬ���������ʼ��λ��
			pItem = std::lower_bound(tBlock->_items, tBlock->_items + eIdx, sTick, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pItem - tBlock->_items;
			WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, tBlock->_items + sIdx, eIdx - sIdx + 1);
			return slice;
		}
	}
}

WTSOrdDtlSlice* WtRdmDtReader::readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	uint32_t rDate, rTime, rSecs;
	//20190807124533900
	rDate = (uint32_t)(etime / 1000000000);
	rTime = (uint32_t)(etime % 1000000000) / 100000;
	rSecs = (uint32_t)(etime % 100000);

	uint32_t lDate, lTime, lSecs;
	//20190807124533900
	lDate = (uint32_t)(stime / 1000000000);
	lTime = (uint32_t)(stime % 1000000000) / 100000;
	lSecs = (uint32_t)(stime % 100000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t beginTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), lDate, lTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

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
	eTick.action_date = rDate;
	eTick.action_time = rTime * 100000 + rSecs;

	WTSOrdDtlStruct sTick;
	sTick.action_date = lDate;
	sTick.action_time = lTime * 100000 + lSecs;

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

		std::size_t eIdx = pItem - rtBlock->_details;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		if (beginTDate != endTDate)
		{
			//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
			WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, rtBlock->_details, eIdx + 1);
			return slice;
		}
		else
		{
			//�����������ͬ���������ʼ��λ��
			pItem = std::lower_bound(rtBlock->_details, rtBlock->_details + eIdx, sTick, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pItem - rtBlock->_details;
			WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, rtBlock->_details + sIdx, eIdx - sIdx + 1);
			return slice;
		}
	}
	else
	{
		std::string key = fmt::format("{}-{}", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/orders/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisOrdDtlBlockPair& hisBlkPair = _his_orddtl_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisOrdDtlBlockV2))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of orderdetail data file {} failed", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisOrdDtlBlockV2* tBlockV2 = (HisOrdDtlBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisOrdDtlBlockV2) + tBlockV2->_size))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of orderdetail data file {} failed", filename.c_str());
				return NULL;
			}

			//��Ҫ��ѹ
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//��ԭ����bufferֻ����һ��ͷ��,��������tick����׷�ӵ�β��
			hisBlkPair._buffer.resize(sizeof(HisOrdDtlBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW;

			hisBlkPair._block = (HisOrdDtlBlock*)hisBlkPair._buffer.c_str();
		}

		HisOrdDtlBlockPair& tBlkPair = _his_orddtl_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisOrdDtlBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisOrdDtlBlock)) / sizeof(WTSOrdDtlStruct);
		if (tcnt <= 0)
			return NULL;

		WTSOrdDtlStruct* pItem = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		std::size_t eIdx = pItem - tBlock->_items;
		if (pItem->action_date > eTick.action_date || pItem->action_time >= eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		if (beginTDate != endTDate)
		{
			//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
			WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, tBlock->_items, eIdx + 1);
			return slice;
		}
		else
		{
			//�����������ͬ���������ʼ��λ��
			pItem = std::lower_bound(tBlock->_items, tBlock->_items + eIdx, sTick, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pItem - tBlock->_items;
			WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, tBlock->_items + sIdx, eIdx - sIdx + 1);
			return slice;
		}
	}
}

WTSTransSlice* WtRdmDtReader::readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	uint32_t rDate, rTime, rSecs;
	//20190807124533900
	rDate = (uint32_t)(etime / 1000000000);
	rTime = (uint32_t)(etime % 1000000000) / 100000;
	rSecs = (uint32_t)(etime % 100000);

	uint32_t lDate, lTime, lSecs;
	//20190807124533900
	lDate = (uint32_t)(stime / 1000000000);
	lTime = (uint32_t)(stime % 1000000000) / 100000;
	lSecs = (uint32_t)(stime % 100000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t beginTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), lDate, lTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

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
	eTick.action_date = rDate;
	eTick.action_time = rTime * 100000 + rSecs;

	WTSTransStruct sTick;
	sTick.action_date = lDate;
	sTick.action_time = lTime * 100000 + lSecs;

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

		std::size_t eIdx = pItem - rtBlock->_trans;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		if (beginTDate != endTDate)
		{
			//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
			WTSTransSlice* slice = WTSTransSlice::create(stdCode, rtBlock->_trans, eIdx + 1);
			return slice;
		}
		else
		{
			//�����������ͬ���������ʼ��λ��
			pItem = std::lower_bound(rtBlock->_trans, rtBlock->_trans + eIdx, sTick, [](const WTSTransStruct& a, const WTSTransStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pItem - rtBlock->_trans;
			WTSTransSlice* slice = WTSTransSlice::create(stdCode, rtBlock->_trans + sIdx, eIdx - sIdx + 1);
			return slice;
		}
	}
	else
	{
		std::string key = fmt::format("{}-{}", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/trans/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisTransBlockPair& hisBlkPair = _his_trans_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisTransBlockV2))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of transaction data file {} failed", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisTransBlockV2* tBlockV2 = (HisTransBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisTransBlockV2) + tBlockV2->_size))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of transaction data file {} failed", filename.c_str());
				return NULL;
			}

			//��Ҫ��ѹ
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//��ԭ����bufferֻ����һ��ͷ��,��������tick����׷�ӵ�β��
			hisBlkPair._buffer.resize(sizeof(HisTransBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW;

			hisBlkPair._block = (HisTransBlock*)hisBlkPair._buffer.c_str();
		}

		HisTransBlockPair& tBlkPair = _his_trans_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisTransBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTransBlock)) / sizeof(WTSTransStruct);
		if (tcnt <= 0)
			return NULL;

		WTSTransStruct* pItem = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSTransStruct& a, const WTSTransStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		std::size_t eIdx = pItem - tBlock->_items;
		if (pItem->action_date > eTick.action_date || pItem->action_time >= eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		if (beginTDate != endTDate)
		{
			//�����ʼ�Ľ����պ͵�ǰ�Ľ����ղ�һ�£��򷵻�ȫ����tick����
			WTSTransSlice* slice = WTSTransSlice::create(stdCode, tBlock->_items, eIdx + 1);
			return slice;
		}
		else
		{
			//�����������ͬ���������ʼ��λ��
			pItem = std::lower_bound(tBlock->_items, tBlock->_items + eIdx, sTick, [](const WTSTransStruct& a, const WTSTransStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t sIdx = pItem - tBlock->_items;
			WTSTransSlice* slice = WTSTransSlice::create(stdCode, tBlock->_items + sIdx, eIdx - sIdx + 1);
			return slice;
		}
	}
}

bool WtRdmDtReader::cacheHisBarsFromFile(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo* cInfo = (CodeHelper::CodeInfo*)codeInfo;
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo->_exchg, cInfo->_product);
	const char* stdPID = cInfo->stdCommID();

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID, curDate, curTime, false);

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
	if (strlen(ruleTag) > 0)//����Ƕ�ȡ�ڻ�������������
	{
		//�Ȱ���HOT������ж�ȡ, ��rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint64_t lastHotTime = 0;
		for (;;)
		{
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo->_exchg << "/" << cInfo->_exchg << "." << cInfo->_product << "_" << ruleTag;
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
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of his kline data file {} failed", filename.c_str());
				break;
			}

			proc_block_data(content, true, false);
			uint32_t barcnt = content.size() / sizeof(WTSBarStruct);

			hotAy = new std::vector<WTSBarStruct>();
			hotAy->resize(barcnt);
			memcpy(hotAy->data(), content.data(), content.size());

			if (period != KP_DAY)
				lastHotTime = hotAy->at(barcnt - 1).time;
			else
				lastHotTime = hotAy->at(barcnt - 1).date;

			pipe_rdmreader_log(_sink, LL_INFO, "{} items of back {} data of hot contract {} directly loaded", barcnt, pname.c_str(), stdCode);
			break;
		}

		HotSections secs;
		if (strlen(ruleTag))
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
				uint64_t sTime = _base_data_mgr->getBoundaryTime(stdPID, leftDt, false, true);
				uint64_t eTime = _base_data_mgr->getBoundaryTime(stdPID, rightDt, false, false);

				sBar.date = leftDt;
				sBar.time = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

				if(sBar.time < lastHotTime)	//����߽�ʱ��С�����������һ��Bar��ʱ��, ˵���Ѿ��н�����, ����Ҫ�ٴ�����
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

			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo->_exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			{
				std::string content;
				StdFile::read_file_content(filename.c_str(), content);
				if (content.size() < sizeof(HisKlineBlock))
				{
					pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of his kline data file {} failed", filename.c_str());
					return false;
				}
				
				proc_block_data(content, true, false);

				if(content.empty())
					break;

				uint32_t barcnt = content.size() / sizeof(WTSBarStruct);
				WTSBarStruct* firstBar = (WTSBarStruct*)content.data();

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

				std::size_t sIdx = pBar - firstBar;
				if ((period == KP_DAY && pBar->date < sBar.date) || (period != KP_DAY && pBar->time < sBar.time))	//���ڱ߽�ʱ��
				{
					//���ڱ߽�ʱ��, ˵��û��������, ��Ϊlower_bound�᷵�ش��ڵ���Ŀ��λ�õ�����
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

				std::size_t eIdx = pBar - firstBar;
				if ((period == KP_DAY && pBar->date > eBar.date) || (period != KP_DAY && pBar->time > eBar.time))
				{
					pBar--;
					eIdx--;
				}

				if (eIdx < sIdx)
					continue;

				uint32_t curCnt = eIdx - sIdx + 1;

				if (cInfo->isExright())
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

				if(bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if(cInfo->isExright() && commInfo->isStock())//����Ƕ�ȡ��Ʊ��Ȩ����
	{
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint64_t lastQTime = 0;
		
		do
		{
			//��ֱ�Ӷ�ȡ��Ȩ������ʷ����,·����/his/day/sse/SH600000Q.dsb
			char flag = cInfo->_exright == 1 ? SUFFIX_QFQ : SUFFIX_HFQ;
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo->_exchg << "/" << cInfo->_code << flag << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of his kline data file {} failed", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			std::string buffer;
			bool bOldVer = kBlock->is_old_version();
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of his kline data file {} failed", filename.c_str());
					break;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					break;

				buffer = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
			}
			else
			{
				content.erase(0, BLOCK_HEADER_SIZE);
				buffer.swap(content);
			}

			if(buffer.empty())
				break;

			if(bOldVer)
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

			barcnt = buffer.size() / sizeof(WTSBarStruct);

			hotAy = new std::vector<WTSBarStruct>();
			hotAy->resize(barcnt);
			memcpy(hotAy->data(), buffer.data(), buffer.size());

			if (period != KP_DAY)
				lastQTime = hotAy->at(barcnt - 1).time;
			else
				lastQTime = hotAy->at(barcnt - 1).date;

			pipe_rdmreader_log(_sink, LL_INFO, "{} history exrighted {} data of {} directly cached", barcnt, pname.c_str(), stdCode);
			break;
		} while (false);

		bool bAllCovered = false;
		do
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
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

			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo->_exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			{
				std::string content;
				StdFile::read_file_content(filename.c_str(), content);
				if (content.size() < sizeof(HisKlineBlock))
				{
					pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of his kline data file {} failed", filename.c_str());
					return false;
				}

				proc_block_data(content, true, false);
				if(content.empty())
					break;

				uint32_t barcnt = content.size() / sizeof(WTSBarStruct);
				WTSBarStruct* firstBar = (WTSBarStruct*)content.data();

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
					std::size_t sIdx = pBar - firstBar;
					uint32_t curCnt = barcnt - sIdx;
					std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
					tempAy->resize(curCnt);
					memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
					realCnt += curCnt;

					auto& ayFactors = getAdjFactors(cInfo->_code, cInfo->_exchg, cInfo->_product);
					if(!ayFactors.empty())
					{
						double baseFactor = 1.0;
						if (cInfo->_exright == 1)
							baseFactor = ayFactors.back()._factor;
						else if (cInfo->_exright == 2)
							barList._factor = ayFactors.back()._factor;

						//��ǰ��Ȩ����
						std::size_t lastIdx = curCnt;
						WTSBarStruct bar;
						firstBar = tempAy->data();
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
								std::size_t curIdx = pBar - firstBar;
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
		//��ȡ��ʷ��
		std::stringstream ss;
		ss << _base_dir << "his/" << pname << "/" << cInfo->_exchg << "/" << cInfo->_code << ".dsb";
		std::string filename = ss.str();
		if (StdFile::exists(filename.c_str()))
		{
			//����и�ʽ������ʷ�����ļ�, ��ֱ�Ӷ�ȡ
			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of his kline data file {} failed", filename.c_str());
				return false;
			}

			proc_block_data(content, true, false);

			if (content.empty())
				return false;

			uint32_t barcnt = content.size() / sizeof(WTSBarStruct);
			WTSBarStruct* firstBar = (WTSBarStruct*)content.data();

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
		barsSections.clear();
	}

	pipe_rdmreader_log(_sink, LL_INFO, "{} history {} data of {} cached", realCnt, pname.c_str(), stdCode);
	return true;
}

WTSBarStruct* WtRdmDtReader::indexBarFromCacheByRange(const std::string& key, uint64_t stime, uint64_t etime, uint32_t& count, bool isDay /* = false */)
{
	uint32_t rDate, rTime, lDate, lTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);
	lDate = (uint32_t)(stime / 10000);
	lTime = (uint32_t)(stime % 10000);

	BarsList& barsList = _bars_cache[key];
	if (barsList._bars.empty())
		return NULL;
	
	std::size_t eIdx,sIdx;
	{
		//�����δ��ʼ��, ��Ҫ���¶�λ
		uint64_t nowTime = (uint64_t)rDate * 10000 + rTime;

		WTSBarStruct eBar;
		eBar.date = rDate;
		eBar.time = (rDate - 19900000) * 10000 + rTime;

		WTSBarStruct sBar;
		sBar.date = lDate;
		sBar.time = (lDate - 19900000) * 10000 + lTime;

		auto eit = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});


		if (eit == barsList._bars.end())
			eIdx = barsList._bars.size() - 1;
		else
		{
			if ((isDay && eit->date > eBar.date) || (!isDay && eit->time > eBar.time))
			{
				eit--;
			}

			eIdx = eit - barsList._bars.begin();
		}

		auto sit = std::lower_bound(barsList._bars.begin(), eit, sBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});
		sIdx = sit - barsList._bars.begin();
	}

	uint32_t curCnt = eIdx - sIdx + 1;
	count = curCnt;
	return &barsList._bars[sIdx];
}

WTSBarStruct* WtRdmDtReader::indexBarFromCacheByCount(const std::string& key, uint64_t etime, uint32_t& count, bool isDay /* = false */)
{
	uint32_t rDate, rTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);

	BarsList& barsList = _bars_cache[key];
	if (barsList._bars.empty())
		return NULL;

	std::size_t eIdx, sIdx;
	WTSBarStruct eBar;
	eBar.date = rDate;
	eBar.time = (rDate - 19900000) * 10000 + rTime;

	auto eit = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});


	if (eit == barsList._bars.end())
		eIdx = barsList._bars.size() - 1;
	else
	{
		if ((isDay && eit->date > eBar.date) || (!isDay && eit->time > eBar.time))
		{
			eit--;
		}

		eIdx = eit - barsList._bars.begin();
	}

	uint32_t curCnt = min((uint32_t)eIdx + 1, count);
	sIdx = eIdx + 1 - curCnt;
	count = curCnt;
	return &barsList._bars[sIdx];
}

uint32_t WtRdmDtReader::readBarsFromCacheByRange(const std::string& key, uint64_t stime, uint64_t etime, std::vector<WTSBarStruct>& ayBars, bool isDay /* = false */)
{
	uint32_t rDate, rTime, lDate, lTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);
	lDate = (uint32_t)(stime / 10000);
	lTime = (uint32_t)(stime % 10000);

	BarsList& barsList = _bars_cache[key];
	std::size_t eIdx,sIdx;
	{
		WTSBarStruct eBar;
		eBar.date = rDate;
		eBar.time = (rDate - 19900000) * 10000 + rTime;

		WTSBarStruct sBar;
		sBar.date = lDate;
		sBar.time = (lDate - 19900000) * 10000 + lTime;

		auto eit = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});
		

		if(eit == barsList._bars.end())
			eIdx = barsList._bars.size() - 1;
		else
		{
			if ((isDay && eit->date > eBar.date) || (!isDay && eit->time > eBar.time))
			{
				if (eit == barsList._bars.begin())
					return 0;
				
				eit--;
			}

			eIdx = eit - barsList._bars.begin();
		}

		auto sit = std::lower_bound(barsList._bars.begin(), eit, sBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});
		sIdx = sit - barsList._bars.begin();
	}

	uint32_t curCnt = eIdx - sIdx + 1;
	if(curCnt > 0)
	{
		ayBars.resize(curCnt);
		memcpy(ayBars.data(), &barsList._bars[sIdx], sizeof(WTSBarStruct)*curCnt);
	}
	return curCnt;
}

WTSKlineSlice* WtRdmDtReader::readKlineSliceByRange(const char* stdCode, WTSKlinePeriod period, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	std::string key = fmt::format("{}#{}", stdCode, period);
	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		bHasHisData = cacheHisBarsFromFile(&cInfo, key, stdCode, period);
	}
	else
	{
		bHasHisData = true;
	}

	if (etime == 0)
		etime = 203012312359;

	uint32_t rDate, rTime, lDate, lTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);
	lDate = (uint32_t)(stime / 10000);
	lTime = (uint32_t)(stime % 10000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);
	
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

	bool isDay = period == KP_DAY;

	//�Ƿ���������
	bool bHasToday = (endTDate >= curTDate);
	std::string raw_code = cInfo._code;

	const char* ruleTag = cInfo._ruletag;
	if (strlen(ruleTag) > 0)
	{
		raw_code = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), curTDate);

		pipe_rdmreader_log(_sink, LL_INFO, "{} contract on {} confirmed: {} -> {}", ruleTag, curTDate, stdCode, raw_code);
	}
	else
	{
		raw_code = cInfo._code;
	}

	WTSBarStruct eBar;
	eBar.date = rDate;
	eBar.time = (rDate - 19900000) * 10000 + rTime;

	WTSBarStruct sBar;
	sBar.date = lDate;
	sBar.time = (lDate - 19900000) * 10000 + lTime;

	bool bNeedHisData = true;

	if (bHasToday)
	{
		//��ȡʵʱ��

		const char* curCode = raw_code.c_str();

		if(cInfo._exright != 2)
		{
			RTKlineBlockPair* kPair = getRTKilneBlock(cInfo._exchg, curCode, period);
			if (kPair != NULL)
			{
				StdUniqueLock lock(*kPair->_mtx);
				//��ȡ���յ�����
				WTSBarStruct* pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + (kPair->_block->_size - 1), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
					if (isDay)
						return a.date < b.date;
					else
						return a.time < b.time;
				});
				std::size_t idx = pBar - kPair->_block->_bars;
				if ((isDay && pBar->date > eBar.date) || (!isDay && pBar->time > eBar.time))
				{
					pBar--;
					idx--;
				}

				pBar = &kPair->_block->_bars[0];
				//�����һ��ʵʱK�ߵ�ʱ����ڿ�ʼ���ڣ���ʵʱK��Ҫȫ��������ȥ
				if ((isDay && pBar->date > sBar.date) || (!isDay && pBar->time > sBar.time))
				{
					rtHead = &kPair->_block->_bars[0];
					rtCnt = idx + 1;
				}
				else
				{
					pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + idx, sBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
						if (isDay)
							return a.date < b.date;
						else
							return a.time < b.time;
					});

					std::size_t sIdx = pBar - kPair->_block->_bars;
					rtHead = pBar;
					rtCnt = idx - sIdx + 1;
					bNeedHisData = false;
				}
			}
		}
		else
		{
			RTKlineBlockPair* kPair = getRTKilneBlock(cInfo._exchg, curCode, period);
			if (kPair != NULL)
			{
				//����Ǻ�Ȩ��ʵʱ��������Ҫ��������ģ��������ﴦ���ܸ���
				BarsList& barsList = _bars_cache[key];

				//1���ȼ�黺�����ж���ʵʱ����
				std::size_t oldSize = barsList._rt_bars.size();
				std::size_t newSize = kPair->_block->_size;

				//2���ٿ���ԭʼʵʱ�����ж��٣������������Ҫ�������
				if (newSize > oldSize)
				{
					barsList._rt_bars.resize(newSize);
					auto idx = oldSize;
					if (oldSize != 0)
						idx--;

					//��Ϊÿ�ο��������һ��K�߶��п�����δ�պϵģ�������Ҫ�����һ��K�߸���
					memcpy(&barsList._rt_bars[idx], &kPair->_block->_bars[idx], sizeof(WTSBarStruct)*(newSize - oldSize + 1));

					//�������Ȩ����
					double factor = barsList._factor;
					for (; idx < newSize; idx++)
					{
						WTSBarStruct* pBar = &barsList._rt_bars[idx];
						pBar->open *= factor;
						pBar->high *= factor;
						pBar->low *= factor;
						pBar->close *= factor;
					}
				}

				//�����һ����λ
				auto it = std::lower_bound(barsList._rt_bars.begin(), barsList._rt_bars.end(), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
					if (isDay)
						return a.date < b.date;
					else
						return a.time < b.time;
				});
				std::size_t idx = it - barsList._rt_bars.begin();
				WTSBarStruct* pBar = &barsList._rt_bars[idx];
				if ((isDay && pBar->date > eBar.date) || (!isDay && pBar->time > eBar.time))
				{
					pBar--;
					idx--;
				}

				pBar = &barsList._rt_bars[0];
				//�����һ��ʵʱK�ߵ�ʱ����ڿ�ʼ���ڣ���ʵʱK��Ҫȫ��������ȥ
				if ((isDay && pBar->date > sBar.date) || (!isDay && pBar->time > sBar.time))
				{
					rtHead = &barsList._rt_bars[0];
					rtCnt = idx + 1;
				}
				else
				{
					it = std::lower_bound(barsList._rt_bars.begin(), barsList._rt_bars.begin() + idx, sBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
						if (isDay)
							return a.date < b.date;
						else
							return a.time < b.time;
					});

					std::size_t sIdx = it - barsList._rt_bars.begin();
					rtHead = &barsList._rt_bars[sIdx];
					rtCnt = idx - sIdx + 1;
					bNeedHisData = false;
				}
			}
		}	
		
	}

	if (bNeedHisData)
	{
		hisHead = indexBarFromCacheByRange(key, stime, etime, hisCnt, period == KP_DAY);
	}

	if (hisCnt + rtCnt > 0)
	{
		WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, 1, hisHead, hisCnt);
		if (rtCnt > 0)
			slice->appendBlock(rtHead, rtCnt);
		return slice;
	}

	return NULL;
}


WtRdmDtReader::TickBlockPair* WtRdmDtReader::getRTTickBlock(const char* exchg, const char* code)
{
	std::string key = fmt::format("{}.{}", exchg, code);

	std::string path = fmt::format("{}rt/ticks/{}/{}.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	TickBlockPair& block = _rt_tick_map[key];
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
		//˵���ļ���С�ѱ�, ��Ҫ����ӳ��
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTickBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	block._last_time = TimeUtils::getLocalTimeNow();
	return &block;
}

WtRdmDtReader::OrdDtlBlockPair* WtRdmDtReader::getRTOrdDtlBlock(const char* exchg, const char* code)
{
	std::string key = fmt::format("{}.{}", exchg, code);

	std::string path = fmt::format("{}rt/orders/{}/{}.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	OrdDtlBlockPair& block = _rt_orddtl_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
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

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdDtlBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	block._last_time = TimeUtils::getLocalTimeNow();
	return &block;
}

WtRdmDtReader::OrdQueBlockPair* WtRdmDtReader::getRTOrdQueBlock(const char* exchg, const char* code)
{
	std::string key = fmt::format("{}.{}", exchg, code);

	std::string path = fmt::format("{}rt/queue/{}/{}.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	OrdQueBlockPair& block = _rt_ordque_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
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

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdQueBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	block._last_time = TimeUtils::getLocalTimeNow();
	return &block;
}

WtRdmDtReader::TransBlockPair* WtRdmDtReader::getRTTransBlock(const char* exchg, const char* code)
{
	std::string key = fmt::format("{}.{}", exchg, code);

	std::string path = fmt::format("{}rt/trans/{}/{}.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	TransBlockPair& block = _rt_trans_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
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

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTransBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	block._last_time = TimeUtils::getLocalTimeNow();
	return &block;
}

WtRdmDtReader::RTKlineBlockPair* WtRdmDtReader::getRTKilneBlock(const char* exchg, const char* code, WTSKlinePeriod period)
{
	if (period != KP_Minute1 && period != KP_Minute5)
		return NULL;

	std::string key = fmt::format("{}.{}", exchg, code);

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

	std::string path = fmt::format("{}rt/{}/{}/{}.dmb", _base_dir.c_str(), subdir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	RTKlineBlockPair& block = (*cache_map)[key];
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
		//˵���ļ���С�ѱ�, ��Ҫ����ӳ��
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTKlineBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	block._last_time = TimeUtils::getLocalTimeNow();
	return &block;
}

WTSKlineSlice* WtRdmDtReader::readKlineSliceByCount(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	std::string key = fmt::format("{}#{}", stdCode, period);
	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		bHasHisData = cacheHisBarsFromFile(&cInfo, key, stdCode, period);
	}
	else
	{
		bHasHisData = true;
	}

	if (etime == 0)
		etime = 203012312359;

	uint32_t rDate, rTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

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

	bool isDay = period == KP_DAY;

	//�Ƿ���������
	bool bHasToday = (endTDate >= curTDate);
	std::string raw_code = cInfo._code;

	const char* ruleTag = cInfo._ruletag;
	if (strlen(ruleTag) > 0)
	{
		raw_code = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), curTDate);
		pipe_rdmreader_log(_sink, LL_INFO, "{} contract on {} confirmed: {} -> {}", ruleTag, curTDate, stdCode, raw_code.c_str());
	}
	else
	{
		raw_code = cInfo._code;
	}

	WTSBarStruct eBar;
	eBar.date = rDate;
	eBar.time = (rDate - 19900000) * 10000 + rTime;


	bool bNeedHisData = true;

	if (bHasToday)
	{
		const char* curCode = raw_code.c_str();
		if(cInfo._exright != 2)
		{
			//��ȡʵʱ��
			RTKlineBlockPair* kPair = getRTKilneBlock(cInfo._exchg, curCode, period);
			if (kPair != NULL)
			{
				StdUniqueLock lock(*kPair->_mtx);
				//��ȡ���յ�����
				WTSBarStruct* pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + (kPair->_block->_size - 1), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
					if (isDay)
						return a.date < b.date;
					else
						return a.time < b.time;
				});
				std::size_t idx = pBar - kPair->_block->_bars;
				if ((isDay && pBar->date > eBar.date) || (!isDay && pBar->time > eBar.time))
				{
					pBar--;
					idx--;
				}

				//�����һ��ʵʱK�ߵ�ʱ����ڿ�ʼ���ڣ���ʵʱK��Ҫȫ��������ȥ
				rtCnt = min((uint32_t)idx + 1, count);
				std::size_t sIdx = idx + 1 - rtCnt;
				rtHead = kPair->_block->_bars + sIdx;
				bNeedHisData = (rtCnt < count);
			}
		}
		else
		{
			RTKlineBlockPair* kPair = getRTKilneBlock(cInfo._exchg, curCode, period);
			if (kPair != NULL)
			{
				//����Ǻ�Ȩ��ʵʱ��������Ҫ��������ģ��������ﴦ���ܸ���
				BarsList& barsList = _bars_cache[key];

				//1���ȼ�黺�����ж���ʵʱ����
				std::size_t oldSize = barsList._rt_bars.size();
				std::size_t newSize = kPair->_block->_size;

				//2���ٿ���ԭʼʵʱ�����ж��٣������������Ҫ�������
				if(newSize > oldSize)
				{
					barsList._rt_bars.resize(newSize);
					auto idx = oldSize;
					if (oldSize != 0)
						idx--;

					//��Ϊÿ�ο��������һ��K�߶��п�����δ�պϵģ�������Ҫ�����һ��K�߸���
					memcpy(&barsList._rt_bars[idx], &kPair->_block->_bars[idx], sizeof(WTSBarStruct)*(newSize - oldSize + 1));

					//�������Ȩ����
					double factor = barsList._factor;
					for(; idx < newSize; idx++)
					{
						WTSBarStruct* pBar = &barsList._rt_bars[idx];
						pBar->open *= factor;
						pBar->high *= factor;
						pBar->low *= factor;
						pBar->close *= factor;
					}
				}

				//�����һ����λ
				auto it = std::lower_bound(barsList._rt_bars.begin(), barsList._rt_bars.end(), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
					if (isDay)
						return a.date < b.date;
					else
						return a.time < b.time;
				});
				std::size_t idx = it - barsList._rt_bars.begin();
				WTSBarStruct* pBar = &barsList._rt_bars[idx];
				if ((isDay && pBar->date > eBar.date) || (!isDay && pBar->time > eBar.time))
				{
					pBar--;
					idx--;
				}

				//�����һ��ʵʱK�ߵ�ʱ����ڿ�ʼ���ڣ���ʵʱK��Ҫȫ��������ȥ
				rtCnt = min((uint32_t)idx + 1, count);
				std::size_t sIdx = idx + 1 - rtCnt;
				rtHead = &barsList._rt_bars[sIdx];
				bNeedHisData = (rtCnt < count);
			}
		}
	}
	

	if (bNeedHisData)
	{
		hisCnt = count - rtCnt;
		hisHead = indexBarFromCacheByCount(key, etime, hisCnt, period == KP_DAY);
	}

	if (hisCnt + rtCnt > 0)
	{
		WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, 1, hisHead, hisCnt);
		if (rtCnt > 0)
			slice->appendBlock(rtHead, rtCnt);
		return slice;
	}

	return NULL;
}

WTSTickSlice* WtRdmDtReader::readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = fmt::format("{}.{}", cInfo._exchg, cInfo._product);

	WTSSessionInfo* sInfo = _base_data_mgr->getSession(_base_data_mgr->getCommodity(cInfo._exchg, cInfo._code)->getSession());

	uint32_t rDate, rTime, rSecs;
	//20190807124533900
	rDate = (uint32_t)(etime / 1000000000);
	rTime = (uint32_t)(etime % 1000000000) / 100000;
	rSecs = (uint32_t)(etime % 100000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

	bool hasToday = (endTDate >= curTDate);

	WTSTickSlice* slice = WTSTickSlice::create(stdCode);

	uint32_t left = count;
	while (hasToday)
	{
		std::string curCode = cInfo._code;
		if(commInfo->isFuture())
		{
			const char* ruleTag = cInfo._ruletag;
			if (strlen(ruleTag) > 0)
			{
				curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), curTDate);

				pipe_rdmreader_log(_sink, LL_INFO, "{} contract on {} confirmed: {} -> {}", ruleTag, curTDate, stdCode, curCode.c_str());
			}
			//else if (cInfo.isHot())
			//{
			//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, curTDate);
			//	pipe_rdmreader_log(_sink, LL_INFO, "Hot contract on {} confirmed: {} -> {}", curTDate, stdCode, curCode.c_str());
			//}
			//else if (cInfo.isSecond())
			//{
			//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, curTDate);
			//	pipe_rdmreader_log(_sink, LL_INFO, "Second contract on {} confirmed: {} -> {}", curTDate, stdCode, curCode.c_str());
			//}
		}		

		TickBlockPair* tPair = getRTTickBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL || tPair->_block->_size == 0)
			break;

		StdUniqueLock lock(*tPair->_mtx);
		RTTickBlock* tBlock = tPair->_block;
		WTSTickStruct eTick;
		if (curTDate == endTDate)
		{
			eTick.action_date = rDate;
			eTick.action_time = rTime * 100000 + rSecs;
		}
		else
		{
			eTick.action_date = curTDate;
			eTick.action_time = sInfo->getCloseTime() * 100000 + 59999;
		}

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tBlock->_size - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		std::size_t eIdx = pTick - tBlock->_ticks;

		//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
		if (pTick->action_date > eTick.action_date || pTick->action_time > eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t thisCnt = min((uint32_t)eIdx + 1, left);
		uint32_t sIdx = eIdx + 1 - thisCnt;
		slice->insertBlock(0, tBlock->_ticks + sIdx, thisCnt);
		left -= thisCnt;
		break;
	}

	uint32_t nowTDate = min(endTDate, curTDate);
	if (nowTDate == curTDate)
		nowTDate = TimeUtils::getNextDate(nowTDate, -1);
	uint32_t missingCnt = 0;
	while (left > 0)
	{
		if(missingCnt >= 30)
			break;

		std::string curCode = cInfo._code;
		std::string hotCode;
		if(commInfo->isFuture())
		{
			const char* ruleTag = cInfo._ruletag;
			if (strlen(ruleTag) > 0)
			{
				curCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), nowTDate);

				hotCode = cInfo._product;
				hotCode += "_";
				hotCode += ruleTag;
				pipe_rdmreader_log(_sink, LL_INFO, "{} contract on {} confirmed: {} -> {}", ruleTag, curTDate, stdCode, curCode.c_str());
			}
			//else if (cInfo.isHot())
			//{
			//	curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, nowTDate);
			//	hotCode = cInfo._product;
			//	hotCode += "_HOT";
			//	pipe_rdmreader_log(_sink, LL_INFO, "Hot contract on {} confirmed: {} -> {}", curTDate, stdCode, curCode.c_str());
			//}
			//else if (cInfo.isSecond())
			//{
			//	curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, nowTDate);
			//	hotCode = cInfo._product;
			//	hotCode += "_2ND";
			//	pipe_rdmreader_log(_sink, LL_INFO, "Second contract on {} confirmed: {} -> {}", curTDate, stdCode, curCode.c_str());
			//}
		}
		

		std::string key = fmt::format("{}-{}", stdCode, nowTDate);

		auto it = _his_tick_map.find(key);
		bool bHasHisTick = (it != _his_tick_map.end());
		if (!bHasHisTick)
		{
			for (;;)
			{
				std::string filename;
				bool bHitHot = false;
				if(!hotCode.empty())
				{
					std::stringstream ss;
					ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << nowTDate << "/" << hotCode << ".dsb";
					filename = ss.str();
					if (StdFile::exists(filename.c_str()))
					{
						bHitHot = true;
					}
				}

				if(!bHitHot)
				{
					std::stringstream ss;
					ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << nowTDate << "/" << curCode << ".dsb";
					filename = ss.str();
					if (!StdFile::exists(filename.c_str()))
					{
						missingCnt++;
						break;
					}
				}

				missingCnt = 0;

				HisTBlockPair& tBlkPair = _his_tick_map[key];
				StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
				if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
				{
					pipe_rdmreader_log(_sink, LL_ERROR, "Sizechecking of his tick data file {} failed", filename.c_str());
					tBlkPair._buffer.clear();
					break;
				}

				proc_block_data(tBlkPair._buffer, false, true);				
				tBlkPair._block = (HisTickBlock*)tBlkPair._buffer.c_str();
				bHasHisTick = true;
				break;
			}
		}

		while (bHasHisTick)
		{
			//�Ƚ�ʱ��Ķ���
			WTSTickStruct eTick;
			if (nowTDate == endTDate)
			{
				eTick.action_date = rDate;
				eTick.action_time = rTime * 100000 + rSecs;
			}
			else
			{
				eTick.action_date = nowTDate;
				eTick.action_time = sInfo->getCloseTime() * 100000 + 59999;
			}

			HisTBlockPair& tBlkPair = _his_tick_map[key];
			if (tBlkPair._block == NULL)
				break;

			HisTickBlock* tBlock = tBlkPair._block;

			uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
			if (tcnt <= 0)
				break;

			WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tcnt - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
				if (a.action_date != b.action_date)
					return a.action_date < b.action_date;
				else
					return a.action_time < b.action_time;
			});

			std::size_t eIdx = pTick - tBlock->_ticks;
			if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
			{
				pTick--;
				eIdx--;
			}

			uint32_t thisCnt = min((uint32_t)eIdx + 1, left);
			uint32_t sIdx = eIdx + 1 - thisCnt;
			slice->insertBlock(0, tBlock->_ticks + sIdx, thisCnt);
			left -= thisCnt;
			break;
		}

		nowTDate = TimeUtils::getNextDate(nowTDate, -1);
	}

	return slice;
}
