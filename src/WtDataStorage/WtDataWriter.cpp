#include "WtDataWriter.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/BoostFile.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/IniHelper.hpp"

#include "../Includes/IBaseDataMgr.h"
#include "../WTSUtils/WTSCmpHelper.hpp"

#include <set>
#include <algorithm>

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void pipe_writer_log(IDataWriterSink* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	const char* buffer = fmtutil::format(format, args...);

	sink->outputLog(ll, buffer);
}

/*
 *	���������
 */
extern bool proc_block_data(std::string& content, bool isBar, bool bKeepHead = true);

extern "C"
{
	EXPORT_FLAG IDataWriter* createWriter()
	{
		IDataWriter* ret = new WtDataWriter();
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

static const uint32_t CACHE_SIZE_STEP = 200;
static const uint32_t TICK_SIZE_STEP = 2500;
static const uint32_t KLINE_SIZE_STEP = 200;

const char CMD_CLEAR_CACHE[] = "CMD_CLEAR_CACHE";
const char MARKER_FILE[] = "marker.ini";


WtDataWriter::WtDataWriter()
	: _terminated(false)
	, _save_tick_log(false)
	, _log_group_size(1000)
	, _disable_day(false)
	, _disable_min1(false)
	, _disable_min5(false)
	, _disable_orddtl(false)
	, _disable_ordque(false)
	, _disable_trans(false)
	, _disable_tick(false)
	, _disable_his(false)
{
}


WtDataWriter::~WtDataWriter()
{
}

bool WtDataWriter::isSessionProceeded(const char* sid)
{
	auto it = _proc_date.find(sid);
	if (it == _proc_date.end())
		return false;

	return (it->second >= TimeUtils::getCurDate());
}


bool WtDataWriter::init(WTSVariant* params, IDataWriterSink* sink)
{
	IDataWriter::init(params, sink);

	_bd_mgr = sink->getBDMgr();
	_save_tick_log = params->getBoolean("savelog");

	_base_dir = StrUtil::standardisePath(params->getCString("path"));
	if (!BoostFile::exists(_base_dir.c_str()))
		BoostFile::create_directories(_base_dir.c_str());
	_cache_file = params->getCString("cache");
	if (_cache_file.empty())
		_cache_file = "cache.dmb";

	_async_proc = params->getBoolean("async");
	_log_group_size = params->getUInt32("groupsize");

	//������ʷ����
	_disable_his = params->getBoolean("disablehis");

	_disable_tick = params->getBoolean("disabletick");
	_disable_min1 = params->getBoolean("disablemin1");
	_disable_min5 = params->getBoolean("disablemin5");
	_disable_day = params->getBoolean("disableday");

	_disable_trans = params->getBoolean("disabletrans");
	_disable_ordque = params->getBoolean("disableordque");
	_disable_orddtl = params->getBoolean("disableorddtl");

	{
		std::string filename = _base_dir + MARKER_FILE;
		IniHelper iniHelper;
		iniHelper.load(filename.c_str());
		StringVector ayKeys, ayVals;
		iniHelper.readSecKeyValArray("markers", ayKeys, ayVals);
		for (uint32_t idx = 0; idx < ayKeys.size(); idx++)
		{
			_proc_date[ayKeys[idx].c_str()] = strtoul(ayVals[idx].c_str(), 0, 10);
		}
	}

	loadCache();

	_proc_chk.reset(new StdThread(boost::bind(&WtDataWriter::check_loop, this)));

	pipe_writer_log(sink, LL_INFO, "WtDataWriter initialized, root dir: {}, save_csv_tick: {}, async_mode: {}, log_group_size: {}, disable_history: {}, "
		"disable_tick: {}, disable_min1: {}, disable_min5: {}, disable_day: {}, disable_trans: {}, disable_ordque: {}, disable_orders: {}", 
		_base_dir, _save_tick_log, _async_proc, _log_group_size, _disable_his, _disable_tick, 
		_disable_min1, _disable_min5, _disable_day, _disable_trans, _disable_ordque, _disable_orddtl);
	return true;
}

void WtDataWriter::release()
{
	_terminated = true;
	if (_proc_thrd)
	{
		_proc_cond.notify_all();
		_proc_thrd->join();
	}

	for(auto& v : _rt_ticks_blocks)
	{
		delete v.second;
	}

	for (auto& v : _rt_trans_blocks)
	{
		delete v.second;
	}

	for (auto& v : _rt_orddtl_blocks)
	{
		delete v.second;
	}

	for (auto& v : _rt_ordque_blocks)
	{
		delete v.second;
	}

	for (auto& v : _rt_min1_blocks)
	{
		delete v.second;
	}

	for (auto& v : _rt_min5_blocks)
	{
		delete v.second;
	}
}

/*
void DataManager::preloadRtCaches(const char* exchg)
{
	if (!_preload_enable || _preloaded)
		return;

	pipe_writer_log(_sink, LL_INFO, "��ʼԤ����ʵʱ���ݻ����ļ�...");
	TimeUtils::Ticker ticker;
	uint32_t cnt = 0;
	uint32_t codecnt = 0;
	WTSArray* ayCts = _bd_mgr->getContracts(exchg);
	if (ayCts != NULL && ayCts->size() > 0)
	{
		for (auto it = ayCts->begin(); it != ayCts->end(); it++)
		{
			WTSContractInfo* ct = (WTSContractInfo*)(*it);
			if (ct == NULL)
				continue;
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);
			if(commInfo == NULL)
				continue;

			bool isStk = (commInfo->getCategoty() == CC_Stock);
			codecnt++;
			
			releaseBlock(getTickBlock(ct->getCode(), TimeUtils::getCurDate(), true));
			releaseBlock(getKlineBlock(ct->getCode(), KP_Minute1, true));
			releaseBlock(getKlineBlock(ct->getCode(), KP_Minute5, true));
			cnt += 3;
			if (isStk && strcmp(commInfo->getProduct(), "STK") == 0)
			{
				releaseBlock(getOrdQueBlock(ct->getCode(), TimeUtils::getCurDate(), true));
				releaseBlock(getTransBlock(ct->getCode(), TimeUtils::getCurDate(), true));
				cnt += 2;
				if (strcmp(ct->getExchg(), "SZSE") == 0)
				{
					releaseBlock(getOrdDtlBlock(ct->getCode(), TimeUtils::getCurDate(), true));
					cnt++;
				}
			}
		}
	}

	if (ayCts != NULL)
		ayCts->release();
	pipe_writer_log(_sink, LL_INFO, "Ԥ����%��Ʒ�ֵ�ʵʱ���ݻ����ļ�{}��,��ʱ{}΢��", codecnt, cnt, WTSLogger::fmtInt64(ticker.micro_seconds()));
	_preloaded = true;
}
*/

void WtDataWriter::loadCache()
{
	if (_tick_cache_file != NULL)
		return;

	bool bNew = false;
	std::string filename = _base_dir + _cache_file;
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

	if(bNew)
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
			const TickCacheItem& item = _tick_cache_block->_ticks[i];
			std::string key = fmt::format("{}.{}", item._tick.exchg, item._tick.code);
			_tick_cache_idx[key] = i;
		}
	}
}

template<typename HeaderType, typename T>
void* WtDataWriter::resizeRTBlock(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	//���øú���֮ǰ,Ӧ���Ѿ�������д����
	RTBlockHeader* tBlock = (RTBlockHeader*)mfPtr->addr();
	if (tBlock->_capacity >= nCount)
		return mfPtr->addr();

	std::string filename = mfPtr->filename();
	uint64_t uOldSize = sizeof(HeaderType) + sizeof(T)*tBlock->_capacity;
	uint64_t uNewSize = sizeof(HeaderType) + sizeof(T)*nCount;
	std::string data;
	data.resize((std::size_t)(uNewSize - uOldSize), 0);
	try
	{
		BoostFile f;
		f.open_existing_file(filename.c_str());
		f.seek_to_end();
		f.write_file(data.c_str(), data.size());
		f.close_file();
	}
	catch(std::exception& ex)
	{
		pipe_writer_log(_sink, LL_ERROR, "Exception occured while expanding RT cache file {} to {}: {}", filename, uNewSize, ex.what());
		return NULL;
	}


	mfPtr.reset();
	BoostMappingFile* pNewMf = new BoostMappingFile();
	try
	{
		if (!pNewMf->map(filename.c_str()))
		{
			delete pNewMf;
			return NULL;
		}
	}
	catch (std::exception& ex)
	{
		pipe_writer_log(_sink, LL_ERROR, "Exception occured while mapping RT cache file {}: {}", filename, ex.what());
		return NULL;
	}	

	mfPtr.reset(pNewMf);

	tBlock = (RTBlockHeader*)mfPtr->addr();
	tBlock->_capacity = nCount;
	return mfPtr->addr();
}

bool WtDataWriter::writeTick(WTSTickData* curTick, uint32_t procFlag)
{
	if (curTick == NULL)
		return false;

	curTick->retain();
	pushTask([this, curTick, procFlag](){

		do
		{
			WTSContractInfo* ct = curTick->getContractInfo();
			if(ct == NULL)
				break;

			WTSCommodityInfo* commInfo = ct->getCommInfo();

			//�ٸ���״̬����
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			//�ȸ��»���
			if (!updateCache(ct, curTick, procFlag))
				break;

			//д��tick����
			if(!_disable_tick)
				pipeToTicks(ct, curTick);

			//д��K�߻���
			pipeToKlines(ct, curTick);

			_sink->broadcastTick(curTick);

			static faster_hashmap<ShortKey, uint64_t> _tcnt_map;
			_tcnt_map[curTick->exchg()]++;
			if (_tcnt_map[curTick->exchg()] % _log_group_size == 0)
			{
				pipe_writer_log(_sink, LL_INFO, "{} ticks received from exchange {}", _tcnt_map[curTick->exchg()], curTick->exchg());
			}
		} while (false);

		curTick->release();
	});
	return true;
}

bool WtDataWriter::writeOrderQueue(WTSOrdQueData* curOrdQue)
{
	if (curOrdQue == NULL || _disable_ordque)
		return false;

	curOrdQue->retain();
	pushTask([this, curOrdQue](){

		do
		{
			WTSContractInfo* ct = _bd_mgr->getContract(curOrdQue->code(), curOrdQue->exchg());
			if (ct == NULL)
				break;

			WTSCommodityInfo* commInfo = ct->getCommInfo();

			//�ٸ���״̬����
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdQueBlockPair* pBlockPair = getOrdQueBlock(ct, curOrdQue->tradingdate());
			if (pBlockPair == NULL)
				break;

			SpinLock lock(pBlockPair->_mutex);

			//�ȼ������������,����Ҫ��
			RTOrdQueBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdQueBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdQueStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_queues[blk->_size], &curOrdQue->getOrdQueStruct(), sizeof(WTSOrdQueStruct));
			blk->_size += 1;

			//TODO: Ҫ�㲥��
			//g_udpCaster.broadcast(curTrans);

			static faster_hashmap<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdQue->exchg()]++;
			if (_tcnt_map[curOrdQue->exchg()] % _log_group_size == 0)
			{
				pipe_writer_log(_sink, LL_INFO, "{} orderques received from exchange {}", _tcnt_map[curOrdQue->exchg()], curOrdQue->exchg());
			}
		} while (false);
		curOrdQue->release();
	});
	return true;
}

void WtDataWriter::pushTask(TaskInfo task)
{
	if(_async_proc)
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

bool WtDataWriter::writeOrderDetail(WTSOrdDtlData* curOrdDtl)
{
	if (curOrdDtl == NULL || _disable_orddtl)
		return false;

	curOrdDtl->retain();
	pushTask([this, curOrdDtl](){

		do
		{

			WTSContractInfo* ct = _bd_mgr->getContract(curOrdDtl->code(), curOrdDtl->exchg());
			if (ct == NULL)
				break;

			WTSCommodityInfo* commInfo = ct->getCommInfo();

			//�ٸ���״̬����
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdDtlBlockPair* pBlockPair = getOrdDtlBlock(ct, curOrdDtl->tradingdate());
			if (pBlockPair == NULL)
				break;

			SpinLock lock(pBlockPair->_mutex);

			//�ȼ������������,����Ҫ��
			RTOrdDtlBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdDtlBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdDtlStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_details[blk->_size], &curOrdDtl->getOrdDtlStruct(), sizeof(WTSOrdDtlStruct));
			blk->_size += 1;

			//TODO: Ҫ�㲥��
			//g_udpCaster.broadcast(curTrans);

			static faster_hashmap<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdDtl->exchg()]++;
			if (_tcnt_map[curOrdDtl->exchg()] % _log_group_size == 0)
			{
				pipe_writer_log(_sink, LL_INFO, "{} orderdetails received from exchange {}", _tcnt_map[curOrdDtl->exchg()], curOrdDtl->exchg());
			}
		} while (false);

		curOrdDtl->release();
	});
	
	return true;
}

bool WtDataWriter::writeTransaction(WTSTransData* curTrans)
{
	if (curTrans == NULL || _disable_trans)
		return false;

	curTrans->retain();
	pushTask([this, curTrans](){

		do
		{

			WTSContractInfo* ct = _bd_mgr->getContract(curTrans->code(), curTrans->exchg());
			if (ct == NULL)
				break;

			WTSCommodityInfo* commInfo = ct->getCommInfo();

			//�ٸ���״̬����
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			TransBlockPair* pBlockPair = getTransBlock(ct, curTrans->tradingdate());
			if (pBlockPair == NULL)
				break;

			SpinLock lock(pBlockPair->_mutex);

			//�ȼ������������,����Ҫ��
			RTTransBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTTransBlock*)resizeRTBlock<RTDayBlockHeader, WTSTransStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_trans[blk->_size], &curTrans->getTransStruct(), sizeof(WTSTransStruct));
			blk->_size += 1;

			//TODO: Ҫ�㲥��
			//g_udpCaster.broadcast(curTrans);

			static faster_hashmap<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTrans->exchg()]++;
			if (_tcnt_map[curTrans->exchg()] % _log_group_size == 0)
			{
				pipe_writer_log(_sink, LL_INFO, "{} transactions received from exchange {}", _tcnt_map[curTrans->exchg()], curTrans->exchg());
			}
		} while (false);

		curTrans->release();
	});
	return true;
}

void WtDataWriter::pipeToTicks(WTSContractInfo* ct, WTSTickData* curTick)
{
	TickBlockPair* pBlockPair = getTickBlock(ct, curTick->tradingdate());
	if (pBlockPair == NULL)
		return;

	SpinLock lock(pBlockPair->_mutex);

	//�ȼ������������,����Ҫ��
	RTTickBlock* blk = pBlockPair->_block;
	if(blk && blk->_size >= blk->_capacity)
	{
		pBlockPair->_file->sync();
		pBlockPair->_block = (RTTickBlock*)resizeRTBlock<RTDayBlockHeader, WTSTickStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
		blk = pBlockPair->_block;
		if(blk) pipe_writer_log(_sink, LL_DEBUG, "RT tick block of {} resized to {}", ct->getFullCode(), blk->_capacity);
	}
	
	if (blk == NULL)
	{
		pipe_writer_log(_sink, LL_DEBUG, "RT tick block of {} is not valid", ct->getFullCode());
		return;
	}

	memcpy(&blk->_ticks[blk->_size], &curTick->getTickStruct(), sizeof(WTSTickStruct));
	blk->_size += 1;

	if(_save_tick_log && pBlockPair->_fstream)
	{
		*(pBlockPair->_fstream) << curTick->code() << ","
			<< curTick->tradingdate() << ","
			<< curTick->actiondate() << ","
			<< curTick->actiontime() << ","
			<< TimeUtils::getLocalTime(false) << ","
			<< curTick->price() << ","
			<< curTick->totalvolume() << ","
			<< curTick->openinterest() << ","
			<< (uint64_t)curTick->totalturnover() << ","
			<< curTick->volume() << ","
			<< curTick->additional() << ","
			<< (uint64_t)curTick->turnover() << std::endl;
	}
}

WtDataWriter::OrdQueBlockPair* WtDataWriter::getOrdQueBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	OrdQueBlockPair* pBlock = NULL;
	const char* key = ct->getFullCode();
	pBlock = _rt_ordque_blocks[key];
	if(pBlock == NULL)
	{
		pBlock = new OrdQueBlockPair();
		_rt_ordque_blocks[key] = pBlock;
	}

	if (pBlock->_block == NULL)
	{
		std::string path = fmt::format("{}rt/queue/{}/", _base_dir.c_str(), ct->getExchg());
		if (bAutoCreate)
			BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			pipe_writer_log(_sink, LL_INFO, "Data file {} not exists, initializing...", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			pipe_writer_log(_sink, LL_INFO, "Mapping file {} failed", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdQueBlock*)pBlock->_file->addr();

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			pipe_writer_log(_sink, LL_INFO, "date[{}] of orderqueue cache block[{}] is different from current date[{}], reinitializing...", pBlock->_block->_date, path.c_str(), curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_queues, 0, sizeof(WTSOrdQueStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW_V2;
			pBlock->_block->_type = BT_RT_OrdQueue;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//��黺���ļ��Ƿ�������,Ҫ�Զ��ָ�
			do
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdQueStruct));
					//�ļ���С��ƥ��,һ������Ϊcapacity����,����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					pipe_writer_log(_sink, LL_WARN, "Oderqueue cache file of {} on date {} repaired", ct->getCode(), curDate);
				}

			} while (false);

		}
	}

	pBlock->_lasttime = TimeUtils::getLocalTimeNow() / 1000;
	return pBlock;
}

WtDataWriter::OrdDtlBlockPair* WtDataWriter::getOrdDtlBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	OrdDtlBlockPair* pBlock = NULL;
	const char* key = ct->getFullCode();
	pBlock = _rt_orddtl_blocks[key];
	if (pBlock == NULL)
	{
		pBlock = new OrdDtlBlockPair();
		_rt_orddtl_blocks[key] = pBlock;
	}

	if (pBlock->_block == NULL)
	{
		std::string path = fmt::format("{}rt/orders/{}/", _base_dir.c_str(), ct->getExchg());
		if(bAutoCreate)
			BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			pipe_writer_log(_sink, LL_INFO, "Data file {} not exists, initializing...", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			pipe_writer_log(_sink, LL_INFO, "Mapping file {} failed", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdDtlBlock*)pBlock->_file->addr();

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			pipe_writer_log(_sink, LL_INFO, "date[{}] of orderdetail cache block[{}] is different from current date[{}], reinitializing...", pBlock->_block->_date, path.c_str(), curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_details, 0, sizeof(WTSOrdDtlStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW_V2;
			pBlock->_block->_type = BT_RT_OrdDetail;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//��黺���ļ��Ƿ�������,Ҫ�Զ��ָ�
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdDtlStruct));
					//�ļ���С��ƥ��,һ������Ϊcapacity����,����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					pipe_writer_log(_sink, LL_WARN, "Orderdetail cache file of {} on date {} repaired", ct->getCode(), curDate);
				}

				break;
			}

		}
	}

	pBlock->_lasttime = TimeUtils::getLocalTimeNow() / 1000;
	return pBlock;
}

WtDataWriter::TransBlockPair* WtDataWriter::getTransBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	TransBlockPair* pBlock = NULL;
	const char* key = ct->getFullCode();
	pBlock = _rt_trans_blocks[key];
	if (pBlock == NULL)
	{
		pBlock = new TransBlockPair();
		_rt_trans_blocks[key] = pBlock;
	}

	if (pBlock->_block == NULL)
	{
		std::string path = fmt::format("{}rt/trans/{}/", _base_dir.c_str(), ct->getExchg());
		if (bAutoCreate)
			BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			pipe_writer_log(_sink, LL_INFO, "Data file {} not exists, initializing...", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			pipe_writer_log(_sink, LL_INFO, "Mapping file {} failed", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTransBlock*)pBlock->_file->addr();

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			pipe_writer_log(_sink, LL_INFO, "date[{}] of transaction cache block[{}] is different from current date[{}], reinitializing...", pBlock->_block->_date, path.c_str(), curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_trans, 0, sizeof(WTSTransStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW_V2;
			pBlock->_block->_type = BT_RT_Trnsctn;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//��黺���ļ��Ƿ�������,Ҫ�Զ��ָ�
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSTransStruct));
					//�ļ���С��ƥ��,һ������Ϊcapacity����,����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					pipe_writer_log(_sink, LL_WARN, "Transaction cache file of {} on date {} repaired", ct->getCode(), curDate);
				}

				break;
			}

		}
	}

	pBlock->_lasttime = TimeUtils::getLocalTimeNow() / 1000;
	return pBlock;
}

WtDataWriter::TickBlockPair* WtDataWriter::getTickBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	TickBlockPair* pBlock = NULL;
	const char* key = ct->getFullCode();
	pBlock = _rt_ticks_blocks[key];
	if (pBlock == NULL)
	{
		pBlock = new TickBlockPair();
		_rt_ticks_blocks[key] = pBlock;
	}

	if(pBlock->_block == NULL)
	{
		std::string path = fmt::format("{}rt/ticks/{}/", _base_dir.c_str(), ct->getExchg());
		if (bAutoCreate)
			BoostFile::create_directories(path.c_str());

		if(_save_tick_log)
		{
			std::stringstream fname;
			fname << path << ct->getCode() << "." << curDate << ".csv";
			pBlock->_fstream.reset(new std::ofstream());
			pBlock->_fstream->open(fname.str().c_str(), std::ios_base::app);
		}

		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			pipe_writer_log(_sink, LL_INFO, "Data file {} not exists, initializing...", path.c_str());

			uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * TICK_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		pBlock->_file.reset(new BoostMappingFile);
		if(!pBlock->_file->map(path.c_str()))
		{
			pipe_writer_log(_sink, LL_ERROR, "Mapping file {} failed", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTickBlock*)pBlock->_file->addr();

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			pipe_writer_log(_sink, LL_INFO, "date[{}] of tick cache block[{}] is different from current date[{}], reinitializing...", pBlock->_block->_date, path.c_str(), curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_ticks, 0, sizeof(WTSTickStruct)*pBlock->_block->_capacity);
		}

		if(isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW_V2;
			pBlock->_block->_type = BT_RT_Ticks;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//��黺���ļ��Ƿ�������,Ҫ�Զ��ָ�
			do
			{
				uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * pBlock->_block->_capacity;
				uint64_t realSz = pBlock->_file->size();
				if (realSz != uSize)
				{
					uint32_t realCap = (uint32_t)((realSz - sizeof(RTTickBlock)) / sizeof(WTSTickStruct));
					uint32_t markedCap = pBlock->_block->_capacity;
					pipe_writer_log(_sink, LL_WARN, "Tick cache file of {} on {} repaired, real capiacity:{}, marked capacity:{}",
						ct->getCode(), curDate, realCap, markedCap);

					//�ļ���С��ƥ��,һ������Ϊcapacity����,����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = realCap;
					pBlock->_block->_size = min(realCap,markedCap);
				}
				
			} while (false);
			
		}
	}

	pBlock->_lasttime = TimeUtils::getLocalTimeNow() / 1000;
	return pBlock;
}

void WtDataWriter::pipeToKlines(WTSContractInfo* ct, WTSTickData* curTick)
{
	uint32_t uDate = curTick->actiondate();
	WTSSessionInfo* sInfo = ct->getCommInfo()->getSessionInfo();
	uint32_t curTime = curTick->actiontime() / 100000;

	uint32_t minutes = sInfo->timeToMinutes(curTime, false);
	if (minutes == INVALID_UINT32)
		return;

	//������Ϊ0,Ҫר�Ŵ���,����091500000,���tickҪ����0915��
	//�����С�ڽ���,Ҫ����С�ڽ�����һ����,��Ϊ�������г�������ʱ��ļ۸����,��113000500
	//����ͬʱ����,������or	
	if (sInfo->isLastOfSection(curTime))
	{
		minutes--;
	}

	//����1������
	if (!_disable_min1)
	{
		KBlockPair* pBlockPair = getKlineBlock(ct, KP_Minute1);
		if (pBlockPair && pBlockPair->_block)
		{
			SpinLock lock(pBlockPair->_mutex);
			RTKlineBlock* blk = pBlockPair->_block;
			if (blk->_size == blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTKlineBlock*)resizeRTBlock<RTKlineBlock, WTSBarStruct>(pBlockPair->_file, blk->_capacity + KLINE_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			WTSBarStruct* lastBar = NULL;
			if (blk->_size > 0)
			{
				lastBar = &blk->_bars[blk->_size - 1];
			}

			//ƴ��1������
			uint32_t barMins = minutes + 1;
			uint64_t barTime = sInfo->minuteToTime(barMins);
			uint32_t barDate = uDate;
			if (barTime == 0)
			{
				barDate = TimeUtils::getNextDate(barDate);
			}
			barTime = TimeUtils::timeToMinBar(barDate, (uint32_t)barTime);

			bool bNew = false;
			if (lastBar == NULL || barTime > lastBar->time)
			{
				bNew = true;
			}

			WTSBarStruct* newBar = NULL;
			if (bNew)
			{
				newBar = &blk->_bars[blk->_size];
				blk->_size += 1;

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
				newBar = &blk->_bars[blk->_size - 1];

				newBar->close = curTick->price();
				newBar->high = std::max(curTick->price(), newBar->high);
				newBar->low = std::min(curTick->price(), newBar->low);

				newBar->vol += curTick->volume();
				newBar->money += curTick->turnover();
				newBar->hold = curTick->openinterest();
				newBar->add += curTick->additional();
			}
		}
	}

	//����5������
	if (!_disable_min5)
	{
		KBlockPair* pBlockPair = getKlineBlock(ct, KP_Minute5);
		if (pBlockPair && pBlockPair->_block)
		{
			SpinLock lock(pBlockPair->_mutex);
			RTKlineBlock* blk = pBlockPair->_block;
			if (blk->_size == blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTKlineBlock*)resizeRTBlock<RTKlineBlock, WTSBarStruct>(pBlockPair->_file, blk->_capacity + KLINE_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			WTSBarStruct* lastBar = NULL;
			if (blk->_size > 0)
			{
				lastBar = &blk->_bars[blk->_size - 1];
			}

			uint32_t barMins = (minutes / 5) * 5 + 5;
			uint64_t barTime = sInfo->minuteToTime(barMins);
			uint32_t barDate = uDate;
			if (barTime == 0)
			{
				barDate = TimeUtils::getNextDate(barDate);
			}
			barTime = TimeUtils::timeToMinBar(barDate, (uint32_t)barTime);

			bool bNew = false;
			if (lastBar == NULL || barTime > lastBar->time)
			{
				bNew = true;
			}

			WTSBarStruct* newBar = NULL;
			if (bNew)
			{
				newBar = &blk->_bars[blk->_size];
				blk->_size += 1;

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
				newBar = &blk->_bars[blk->_size - 1];

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
}

template<typename T>
void WtDataWriter::releaseBlock(T* block)
{
	if (block == NULL || block->_file == NULL)
		return;

	SpinLock lock(block->_mutex);
	block->_block = NULL;
	block->_file.reset();
	block->_lasttime = 0;
}

WtDataWriter::KBlockPair* WtDataWriter::getKlineBlock(WTSContractInfo* ct, WTSKlinePeriod period, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	KBlockPair* pBlock = NULL;
	const char* key = ct->getFullCode();

	KBlockFilesMap* cache_map = NULL;
	std::string subdir = "";
	BlockType bType;
	switch(period)
	{
	case KP_Minute1: 
		cache_map = &_rt_min1_blocks; 
		subdir = "min1";
		bType = BT_RT_Minute1;
		break;
	case KP_Minute5: 
		cache_map = &_rt_min5_blocks;
		subdir = "min5";
		bType = BT_RT_Minute5;
		break;
	default: break;
	}

	if (cache_map == NULL)
		return NULL;

	pBlock = (*cache_map)[key];
	if (pBlock == NULL)
	{
		pBlock = new KBlockPair();
		(*cache_map)[key] = pBlock;
	}

	if (pBlock->_block == NULL)
	{
		//std::string path = StrUtil::printf("%srt/%s/%s/", _base_dir.c_str(), subdir.c_str(), ct->getExchg());
		thread_local static char path[256] = { 0 };
		char * s = fmt::format_to(path, "{}rt/{}/{}/", _base_dir, subdir, ct->getExchg());
		s[0] = '\0';
		if (bAutoCreate)
			BoostFile::create_directories(path);

		wt_strcpy(s, ct->getCode());
		s += strlen(ct->getCode());
		wt_strcpy(s, ".dmb");
		s += 4;
		s[0] = '\0';

		bool isNew = false;
		if (!BoostFile::exists(path))
		{
			if (!bAutoCreate)
				return NULL;

			pipe_writer_log(_sink, LL_INFO, "Data file {} not exists, initializing...", path);

			uint64_t uSize = sizeof(RTKlineBlock) + sizeof(WTSBarStruct) * KLINE_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(path);
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		pBlock->_file.reset(new BoostMappingFile);
		if(pBlock->_file->map(path))
		{
			pBlock->_block = (RTKlineBlock*)pBlock->_file->addr();
		}
		else
		{
			pipe_writer_log(_sink, LL_ERROR, "Mapping file {} failed", path);
			pBlock->_file.reset();
			return NULL;
		}

		//if(pBlock->_block->_date != uDate)
		//{
		//	pBlock->_block->_size = 0;
		//	pBlock->_block->_date = uDate;

		//	memset(&pBlock->_block->_bars, 0, sizeof(WTSBarStruct)*pBlock->_block->_capacity);
		//}

		if (isNew)
		{
			//memset(pBlock->_block, 0, pBlock->_file->size());
			pBlock->_block->_capacity = KLINE_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW_V2;
			pBlock->_block->_type = bType;
			pBlock->_block->_date = TimeUtils::getCurDate();
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
	}

	pBlock->_lasttime = TimeUtils::getLocalTimeNow() / 1000;
	return pBlock;
}

WTSTickData* WtDataWriter::getCurTick(const char* code, const char* exchg/* = ""*/)
{
	if (strlen(code) == 0)
		return NULL;

	WTSContractInfo* ct = _bd_mgr->getContract(code, exchg);
	if (ct == NULL)
		return NULL;

	const char* key = ct->getFullCode();
	SpinLock lock(_lck_tick_cache);
	auto it = _tick_cache_idx.find(key);
	if (it == _tick_cache_idx.end())
		return NULL;

	uint32_t idx = it->second;
	TickCacheItem& item = _tick_cache_block->_ticks[idx];
	return WTSTickData::create(item._tick);
}

bool WtDataWriter::updateCache(WTSContractInfo* ct, WTSTickData* curTick, uint32_t procFlag)
{
	if (curTick == NULL || _tick_cache_block == NULL)
	{
		pipe_writer_log(_sink, LL_ERROR, "Tick cache data not initialized");
		return false;
	}

	SpinLock lock(_lck_tick_cache);
	const char* key = ct->getFullCode();
	uint32_t idx = 0;
	auto it = _tick_cache_idx.find(key);
	if (it == _tick_cache_idx.end())
	{
		idx = _tick_cache_block->_size;
		_tick_cache_idx[key] = _tick_cache_block->_size;
		_tick_cache_block->_size += 1;
		if(_tick_cache_block->_size >= _tick_cache_block->_capacity)
		{
			_tick_cache_block = (RTTickCache*)resizeRTBlock<RTTickCache, TickCacheItem>(_tick_cache_file, _tick_cache_block->_capacity + CACHE_SIZE_STEP);
			pipe_writer_log(_sink, LL_INFO, "Tick Cache resized to {} items", _tick_cache_block->_capacity);
		}
	}
	else
	{
		idx = it->second;
	}


	TickCacheItem& item = _tick_cache_block->_ticks[idx];
	if (curTick->tradingdate() < item._date)
	{
		pipe_writer_log(_sink, LL_INFO, "Tradingday[{}] of {} is less than cached tradingday[{}]", curTick->tradingdate(), curTick->code(), item._date);
		return false;
	}

	WTSTickStruct& newTick = curTick->getTickStruct();

	if (curTick->tradingdate() > item._date)
	{
		//�����ݽ����մ���������,����Ϊ����һ�������
		item._date = curTick->tradingdate();
		memcpy(&item._tick, &newTick, sizeof(WTSTickStruct));
		if (procFlag==1)
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
		pipe_writer_log(_sink, LL_INFO, "First tick of new tradingday {},{}.{},{},{},{},{},{}", 
			newTick.trading_date, curTick->exchg(), curTick->code(), curTick->price(), curTick->volume(),
			curTick->turnover(), curTick->openinterest(), curTick->additional());
	}
	else
	{
		//�����������������ڴ����������������
		//���߻������ʱ����ڵ������������ʱ��,���ݾͲ���Ҫ����
		WTSSessionInfo* sInfo = ct->getCommInfo()->getSessionInfo();
		uint32_t tdate = sInfo->getOffsetDate(curTick->actiondate(), curTick->actiontime() / 100000);
		if (tdate > curTick->tradingdate())
		{
			pipe_writer_log(_sink, LL_WARN, "Last tick of {}.{} with time {}.{} has an exception, abandoned", curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime());
			return false;
		}
		else if (curTick->totalvolume() < item._tick.total_volume)
		{
			pipe_writer_log(_sink, LL_WARN, "Last tick of {}.{} with time {}.{}, volume {} is less than cached volume {}, abandoned",
				curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime(), curTick->totalvolume(), item._tick.total_volume);
			return false;
		}

		//ʱ�����ͬ,���ǳɽ������ڵ���ԭ����,�������һ����֣����,����Ĵ���ʽ����ʱ���+200����
		//By Wesley @ 2021.12.21
		//���췢�־�Ȼһ�������4�ʣ�ʵ�����е�����
		//ֻ�ܰ�500����ı仯���ĳ�200�����Ҹĳɷ���ʱ��С�ڵ�����һ�ʵ��ж�
		if(newTick.action_date == item._tick.action_date && newTick.action_time <= item._tick.action_time && newTick.total_volume >= item._tick.total_volume)
		{
			newTick.action_time += 200;
		}

		//�����Ҫ���費��ҪԤ������
		if(procFlag == 0)
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

		//pipe_writer_log(_sink, LL_DEBUG, "Tick cache data updated {}[{}.{}]", newTick.code, newTick.action_date, newTick.action_time);
	}

	return true;
}

void WtDataWriter::transHisData(const char* sid)
{
	StdUniqueLock lock(_proc_mtx);
	if (strcmp(sid, CMD_CLEAR_CACHE) != 0)
	{
		CodeSet* pCommSet = _sink->getSessionComms(sid);
		if (pCommSet == NULL)
			return;

		for (auto it = pCommSet->begin(); it != pCommSet->end(); it++)
		{
			const char* key = (*it).c_str();

			const StringVector& ay = StrUtil::split(key, ".");
			const char* exchg = ay[0].c_str();
			const char* pid = ay[1].c_str();

			WTSCommodityInfo* pCommInfo = _bd_mgr->getCommodity(exchg, pid);
			if (pCommInfo == NULL)
				continue;

			const CodeSet& codes = pCommInfo->getCodes();
			for (auto code : codes)
			{
				WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg);
				if(ct)
					_proc_que.push(ct->getFullCode());
			}
		}

		_proc_que.push(StrUtil::printf("MARK.%s", sid));
	}
	else
	{
		_proc_que.push(sid);
	}

	if (_proc_thrd == NULL)
	{
		_proc_thrd.reset(new StdThread(boost::bind(&WtDataWriter::proc_loop, this)));
	}
	else
	{
		_proc_cond.notify_all();
	}
}

void WtDataWriter::check_loop()
{
	uint32_t expire_secs = 600;
	while(!_terminated)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));
		/*
		 *	By Wesley @ 2022.04.18
		 *	���������ҵ�߳��Ѿ���������ֱ���˳�����߳�
		 */
		if(_proc_thrd != NULL)
			break;

		uint64_t now = TimeUtils::getLocalTimeNow() / 1000;
		for (auto it = _rt_ticks_blocks.begin(); it != _rt_ticks_blocks.end(); it++)
		{
			const char* key = it->first.c_str();
			TickBlockPair* tBlk = (TickBlockPair*)it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				pipe_writer_log(_sink, LL_INFO, "tick cache of {} mapping expired, automatically closed", key);
				releaseBlock<TickBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_trans_blocks.begin(); it != _rt_trans_blocks.end(); it++)
		{
			const char* key = it->first.c_str();
			TransBlockPair* tBlk = (TransBlockPair*)it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				pipe_writer_log(_sink, LL_INFO, "trans cache o {} mapping expired, automatically closed", key);
				releaseBlock<TransBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_orddtl_blocks.begin(); it != _rt_orddtl_blocks.end(); it++)
		{
			const char* key = it->first.c_str();
			OrdDtlBlockPair* tBlk = (OrdDtlBlockPair*)it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				pipe_writer_log(_sink, LL_INFO, "order cache of {} mapping expired, automatically closed", key);
				releaseBlock<OrdDtlBlockPair>(tBlk);
			}
		}

		for (auto& v : _rt_ordque_blocks)
		{
			const char* key = v.first.c_str();
			OrdQueBlockPair* tBlk = (OrdQueBlockPair*)v.second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				pipe_writer_log(_sink, LL_INFO, "queue cache of {} mapping expired, automatically closed", key);
				releaseBlock<OrdQueBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_min1_blocks.begin(); it != _rt_min1_blocks.end(); it++)
		{
			const char* key = it->first.c_str();
			KBlockPair* kBlk = (KBlockPair*)it->second;
			if (kBlk->_lasttime != 0 && (now - kBlk->_lasttime > expire_secs))
			{
				pipe_writer_log(_sink, LL_INFO, "min1 cache of {} mapping expired, automatically closed", key);
				releaseBlock<KBlockPair>(kBlk);
			}
		}

		for (auto it = _rt_min5_blocks.begin(); it != _rt_min5_blocks.end(); it++)
		{
			const char* key = it->first.c_str();
			KBlockPair* kBlk = (KBlockPair*)it->second;
			if (kBlk->_lasttime != 0 && (now - kBlk->_lasttime > expire_secs))
			{
				pipe_writer_log(_sink, LL_INFO, "min5 cache of {} mapping expired, automatically closed", key);
				releaseBlock<KBlockPair>(kBlk);
			}
		}
	}
}

uint32_t WtDataWriter::dump_bars_via_dumper(WTSContractInfo* ct)
{
	if (ct == NULL || _dumpers.empty())
		return 0;

	std::string key = ct->getFullCode();

	uint32_t count = 0;

	//�ӻ����ж�ȡ����tick,���µ���ʷ����
	auto it = _tick_cache_idx.find(key);
	if (it != _tick_cache_idx.end())
	{
		uint32_t idx = it->second;

		const TickCacheItem& tci = _tick_cache_block->_ticks[idx];
		const WTSTickStruct& ts = tci._tick;

		WTSBarStruct bsDay;
		bsDay.open = ts.open;
		bsDay.high = ts.high;
		bsDay.low = ts.low;
		bsDay.close = ts.price;
		bsDay.settle = ts.settle_price;
		bsDay.vol = ts.total_volume;
		bsDay.money = ts.total_turnover;
		bsDay.hold = ts.open_interest;
		bsDay.add = ts.diff_interest;

		for(auto& item : _dumpers)
		{
			const char* id = item.first.c_str();
			IHisDataDumper* dumper = item.second;
			if(dumper == NULL)
				continue;

			bool bSucc = dumper->dumpHisBars(key.c_str(), "d1", &bsDay, 1);
			if (!bSucc)
			{
				pipe_writer_log(_sink, LL_ERROR, "Closing Task of day bar of {} failed via extended dumper {}", ct->getFullCode(), id);
			}
		}

		count++;

	}

	//ת��ʵʱ1������
	KBlockPair* kBlkPair = getKlineBlock(ct, KP_Minute1, false);
	if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
	{
		uint32_t size = kBlkPair->_block->_size;
		pipe_writer_log(_sink, LL_INFO, "Transfering min1 bars of {}...", ct->getFullCode());
		SpinLock lock(kBlkPair->_mutex);

		for (auto& item : _dumpers)
		{
			const char* id = item.first.c_str();
			IHisDataDumper* dumper = item.second;
			if (dumper == NULL)
				continue;

			bool bSucc = dumper->dumpHisBars(key.c_str(), "m1", kBlkPair->_block->_bars, size);
			if (!bSucc)
			{
				pipe_writer_log(_sink, LL_ERROR, "Closing Task of m1 bar of {} failed via extended dumper {}", ct->getFullCode(), id);
			}
		}

		count++;
		kBlkPair->_block->_size = 0;
	}

	if (kBlkPair)
		releaseBlock(kBlkPair);

	//���Ĳ�,ת��ʵʱ5������
	kBlkPair = getKlineBlock(ct, KP_Minute5, false);
	if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
	{
		uint32_t size = kBlkPair->_block->_size;
		pipe_writer_log(_sink, LL_INFO, "Transfering min5 bars of {}...", ct->getFullCode());
		SpinLock lock(kBlkPair->_mutex);

		for (auto& item : _dumpers)
		{
			const char* id = item.first.c_str();
			IHisDataDumper* dumper = item.second;
			if (dumper == NULL)
				continue;

			bool bSucc = dumper->dumpHisBars(key.c_str(), "m5", kBlkPair->_block->_bars, size);
			if (!bSucc)
			{
				pipe_writer_log(_sink, LL_ERROR, "Closing Task of m5 bar of {} failed via extended dumper {}", ct->getFullCode(), id);
			}
		}

		count++;
		kBlkPair->_block->_size = 0;
	}

	if (kBlkPair)
		releaseBlock(kBlkPair);

	return count;
}

bool WtDataWriter::proc_block_data(const char* tag, std::string& content, bool isBar, bool bKeepHead /* = true */)
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

			pipe_writer_log(_sink, LL_INFO, "{} bars of {} transferd to new version...", barcnt, tag);
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
			pipe_writer_log(_sink, LL_INFO, "{} ticks of {} transferd to new version...", tick_cnt, tag);
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

bool WtDataWriter::dump_day_data(WTSContractInfo* ct, WTSBarStruct* newBar)
{
	std::stringstream ss;
	ss << _base_dir << "his/day/" << ct->getExchg() << "/";
	std::string path = ss.str();
	BoostFile::create_directories(ss.str().c_str());
	std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), ct->getCode());

	bool bNew = false;
	if (!BoostFile::exists(filename.c_str()))
		bNew = true;

	BoostFile f;
	if (f.create_or_open_file(filename.c_str()))
	{
		bool bNeedWrite = true;
		if (bNew)
		{
			BlockHeader header;
			strcpy(header._blk_flag, BLK_FLAG);
			header._type = BT_HIS_Day;
			header._version = BLOCK_VERSION_RAW_V2;

			f.write_file(&header, sizeof(header));

			f.write_file(newBar, sizeof(WTSBarStruct));
		}
		else
		{
			//���߱���Ҫ���һ��
			std::string content;
			BoostFile::read_file_contents(filename.c_str(), content);
			HisKlineBlock* kBlock = (HisKlineBlock*)content.data();
			//����ϵ��ļ��Ѿ���ѹ���汾,�����������ݴ�С����100��,�����ѹ��
			bool bCompressed = kBlock->is_compressed();

			//��ͳһ��ѹ����
			proc_block_data(filename.c_str(), content, true, false);
			
			uint32_t barcnt = content.size() / sizeof(WTSBarStruct);
			//��ʼ�Ƚ�K��ʱ���ǩ,��ҪΪ�˷�ֹ�����ظ�д
			if (barcnt != 0)
			{
				WTSBarStruct& oldBS = ((WTSBarStruct*)content.data())[barcnt - 1];

				if (oldBS.date == newBar->date && memcmp(&oldBS, newBar, sizeof(WTSBarStruct)) != 0)
				{
					//������ͬ�����ݲ�ͬ,�������µ��滻���һ��
					oldBS = *newBar;
				}
				else if (oldBS.date < newBar->date)	//�ϵ�K������С���µ�,��ֱ��׷�ӵ�����
				{
					content.append((char*)newBar, sizeof(WTSBarStruct));
					barcnt++;
				}
			}

			//����ϵ��ļ��Ѿ���ѹ���汾,�����������ݴ�С����100��,�����ѹ��
			bool bNeedCompress = bCompressed || (barcnt > 100);
			if (bNeedCompress)
			{
				std::string cmpData = WTSCmpHelper::compress_data(content.data(), content.size());
				BlockHeaderV2 header;
				strcpy(header._blk_flag, BLK_FLAG);
				header._type = BT_HIS_Day;
				header._version = BLOCK_VERSION_CMP_V2;
				header._size = cmpData.size();

				f.truncate_file(0);
				f.seek_to_begin();
				f.write_file(&header, sizeof(header));

				f.write_file(cmpData.data(), cmpData.size());
			}
			else
			{
				BlockHeader header;
				strcpy(header._blk_flag, BLK_FLAG);
				header._type = BT_HIS_Day;
				header._version = BLOCK_VERSION_RAW_V2;

				f.truncate_file(0);
				f.seek_to_begin();
				f.write_file(&header, sizeof(header));
				f.write_file(content.data(), content.size());
			}
		}

		f.close_file();

		return true;
	}
	else
	{
		pipe_writer_log(_sink, LL_ERROR, "ClosingTask of day bar failed: openning history data file {} failed", filename.c_str());
		return false;
	}
}

uint32_t WtDataWriter::dump_bars_to_file(WTSContractInfo* ct)
{
	if (ct == NULL)
		return 0;

	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());

	uint32_t count = 0;

	//�ӻ����ж�ȡ����tick,���µ���ʷ����
	if (!_disable_day)
	{
		auto it = _tick_cache_idx.find(key);
		if (it != _tick_cache_idx.end())
		{
			uint32_t idx = it->second;

			const TickCacheItem& tci = _tick_cache_block->_ticks[idx];
			const WTSTickStruct& ts = tci._tick;

			WTSBarStruct bs;
			bs.date = ts.trading_date;
			bs.time = 0;
			bs.open = ts.open;
			bs.close = ts.price;
			bs.high = ts.high;
			bs.low = ts.low;
			bs.settle = ts.settle_price;
			bs.vol = ts.total_volume;
			bs.hold = ts.open_interest;
			bs.money = ts.total_turnover;
			bs.add = ts.open_interest - ts.pre_interest;

			dump_day_data(ct, &bs);
		}
	}

	//ת��ʵʱ1������
	if (!_disable_min1)
	{
		KBlockPair* kBlkPair = getKlineBlock(ct, KP_Minute1, false);
		if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
		{
			uint32_t size = kBlkPair->_block->_size;
			pipe_writer_log(_sink, LL_INFO, "Transfering min1 bars of {}...", ct->getFullCode());
			SpinLock lock(kBlkPair->_mutex);

			std::stringstream ss;
			ss << _base_dir << "his/min1/" << ct->getExchg() << "/";
			BoostFile::create_directories(ss.str().c_str());
			std::string path = ss.str();
			BoostFile::create_directories(ss.str().c_str());
			std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), ct->getCode());

			bool bNew = false;
			if (!BoostFile::exists(filename.c_str()))
				bNew = true;

			pipe_writer_log(_sink, LL_INFO, "Openning data storage faile: {}", filename.c_str());

			BoostFile f;
			if (f.create_or_open_file(filename.c_str()))
			{
				std::string buffer;
				bool bOldVer = false;
				if (!bNew)
				{
					std::string content;
					BoostFile::read_file_contents(filename.c_str(), content);
					proc_block_data(filename.c_str(), content, true, false);
					buffer.swap(content);
				}

				//׷���µ�����
				buffer.append((const char*)kBlkPair->_block->_bars, sizeof(WTSBarStruct)*size);

				std::string cmpData = WTSCmpHelper::compress_data(buffer.data(), buffer.size());

				f.truncate_file(0);
				f.seek_to_begin(0);

				BlockHeaderV2 header;
				strcpy(header._blk_flag, BLK_FLAG);
				header._type = BT_HIS_Minute1;
				header._version = BLOCK_VERSION_CMP_V2;
				header._size = cmpData.size();
				f.write_file(&header, sizeof(header));
				f.write_file(cmpData);
				count += size;

				//��󽫻������
				//memset(kBlkPair->_block->_bars, 0, sizeof(WTSBarStruct)*kBlkPair->_block->_size);
				kBlkPair->_block->_size = 0;
			}
			else
			{
				pipe_writer_log(_sink, LL_ERROR, "ClosingTask of min1 bar failed: openning history data file {} failed", filename.c_str());
			}
		}

		if (kBlkPair)
			releaseBlock(kBlkPair);
	}

	//���Ĳ�,ת��ʵʱ5������
	if (!_disable_min5)
	{
		KBlockPair* kBlkPair = getKlineBlock(ct, KP_Minute5, false);
		if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
		{
			uint32_t size = kBlkPair->_block->_size;
			pipe_writer_log(_sink, LL_INFO, "Transfering min5 bar of {}...", ct->getFullCode());
			SpinLock lock(kBlkPair->_mutex);

			std::stringstream ss;
			ss << _base_dir << "his/min5/" << ct->getExchg() << "/";
			BoostFile::create_directories(ss.str().c_str());
			std::string path = ss.str();
			BoostFile::create_directories(ss.str().c_str());
			std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), ct->getCode());

			bool bNew = false;
			if (!BoostFile::exists(filename.c_str()))
				bNew = true;

			pipe_writer_log(_sink, LL_INFO, "Openning data storage file: {}", filename.c_str());

			BoostFile f;
			if (f.create_or_open_file(filename.c_str()))
			{
				std::string buffer;
				bool bOldVer = false;
				if (!bNew)
				{
					std::string content;
					BoostFile::read_file_contents(filename.c_str(), content);
					proc_block_data(filename.c_str(), content, true, false);
					buffer.swap(content);
				}

				buffer.append((const char*)kBlkPair->_block->_bars, sizeof(WTSBarStruct)*size);

				std::string cmpData = WTSCmpHelper::compress_data(buffer.data(), buffer.size());

				f.truncate_file(0);
				f.seek_to_begin(0);

				BlockHeaderV2 header;
				strcpy(header._blk_flag, BLK_FLAG);
				header._type = BT_HIS_Minute5;
				header._version = BLOCK_VERSION_CMP_V2;
				header._size = cmpData.size();
				f.write_file(&header, sizeof(header));
				f.write_file(cmpData);
				count += size;

				//��󽫻������
				kBlkPair->_block->_size = 0;
			}
			else
			{
				pipe_writer_log(_sink, LL_ERROR, "ClosingTask of min5 bar failed: openning history data file {} failed", filename.c_str());
			}
		}

		if (kBlkPair)
			releaseBlock(kBlkPair);
	}

	return count;
}

void WtDataWriter::proc_loop()
{
	while (!_terminated)
	{
		if(_proc_que.empty())
		{
			StdUniqueLock lock(_proc_mtx);
			_proc_cond.wait(_proc_mtx);
			continue;
		}

		std::string fullcode;
		try
		{
			StdUniqueLock lock(_proc_mtx);
			fullcode = _proc_que.front().c_str();
			_proc_que.pop();
		}
		catch(std::exception& e)
		{
			pipe_writer_log(_sink, LL_ERROR, e.what());
			continue;
		}

		if (fullcode.compare(CMD_CLEAR_CACHE) == 0)
		{
			//������
			SpinLock lock(_lck_tick_cache);

			std::set<std::string> setCodes;
			std::stringstream ss_snapshot;
			ss_snapshot << "date,exchg,code,open,high,low,close,settle,volume,turnover,openinterest,upperlimit,lowerlimit,preclose,presettle,preinterest" << std::endl << std::fixed;
			for (auto it = _tick_cache_idx.begin(); it != _tick_cache_idx.end(); it++)
			{
				const char* key = it->first.c_str();
				const StringVector& ay = StrUtil::split(key, ".");
				WTSContractInfo* ct = _bd_mgr->getContract(ay[1].c_str(), ay[0].c_str());
				if (ct != NULL)
				{
					setCodes.insert(key);

					uint32_t idx = it->second;

					const TickCacheItem& tci = _tick_cache_block->_ticks[idx];
					const WTSTickStruct& ts = tci._tick;
					ss_snapshot << ts.trading_date << ","
						<< ts.exchg << ","
						<< ts.code << ","
						<< ts.open << ","
						<< ts.high << ","
						<< ts.low << ","
						<< ts.price << ","
						<< ts.settle_price << ","
						<< ts.total_volume << ","
						<< ts.total_turnover << ","
						<< ts.open_interest << ","
						<< ts.upper_limit << ","
						<< ts.lower_limit << ","
						<< ts.pre_close << ","
						<< ts.pre_settle << ","
						<< ts.pre_interest << std::endl;
				}
				else
				{
					pipe_writer_log(_sink, LL_WARN, "{}[{}] expired, cache will be cleared", ay[1].c_str(), ay[0].c_str());

					//ɾ���Ѿ����ڴ����ʵʱtick�ļ�
					std::string path = StrUtil::printf("%srt/ticks/%s/%s.dmb", _base_dir.c_str(), ay[0].c_str(), ay[1].c_str());
					BoostFile::delete_file(path.c_str());
				}
			}

			//���������������ͬ,˵���д��������,���ų���
			if(setCodes.size() != _tick_cache_idx.size())
			{
				uint32_t diff = _tick_cache_idx.size() - setCodes.size();

				uint32_t scale = setCodes.size() / CACHE_SIZE_STEP;
				if (setCodes.size() % CACHE_SIZE_STEP != 0)
					scale++;

				uint32_t size = sizeof(RTTickCache) + sizeof(TickCacheItem)*scale*CACHE_SIZE_STEP;
				std::string buffer;
				buffer.resize(size, 0);

				RTTickCache* newCache = (RTTickCache*)buffer.data();
				newCache->_capacity = scale*CACHE_SIZE_STEP;
				newCache->_type = BT_RT_Cache;
				newCache->_size = setCodes.size();
				newCache->_version = BLOCK_VERSION_RAW_V2;
				strcpy(newCache->_blk_flag, BLK_FLAG);

				faster_hashmap<LongKey, uint32_t> newIdxMap;

				uint32_t newIdx = 0;
				for (const std::string& key : setCodes)
				{
					uint32_t oldIdx = _tick_cache_idx[key];
					newIdxMap[key] = newIdx;

					memcpy(&newCache->_ticks[newIdx], &_tick_cache_block->_ticks[oldIdx], sizeof(TickCacheItem));

					newIdx++;
				}

				//�����滻
				_tick_cache_idx = newIdxMap;
				_tick_cache_file->close();
				_tick_cache_block = NULL;

				std::string filename = _base_dir + _cache_file;
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					f.write_file(buffer.data(), buffer.size());
					f.close_file();
				}

				_tick_cache_file->map(filename.c_str());
				_tick_cache_block = (RTTickCache*)_tick_cache_file->addr();
				
				pipe_writer_log(_sink, LL_INFO, "{} expired cache cleared totally", diff);
			}

			//�����յ����߿�����ص�һ�������ļ�
			{

				std::stringstream ss;
				ss << _base_dir << "his/snapshot/";
				BoostFile::create_directories(ss.str().c_str());
				ss << TimeUtils::getCurDate() << ".csv";
				std::string path = ss.str();

				const std::string& content = ss_snapshot.str();
				BoostFile f;
				f.create_new_file(path.c_str());
				f.write_file(content.data());
				f.close_file();
			}

			int try_count = 0;
			do
			{
				if(try_count >= 5)
				{
					pipe_writer_log(_sink, LL_ERROR, "Too many trys to clear rt cache files��skip");
					break;
				}

				try_count++;
				try
				{
					std::string path = StrUtil::printf("%srt/min1/", _base_dir.c_str());
					boost::filesystem::remove_all(boost::filesystem::path(path));
					path = StrUtil::printf("%srt/min5/", _base_dir.c_str());
					boost::filesystem::remove_all(boost::filesystem::path(path));
					path = StrUtil::printf("%srt/ticks/", _base_dir.c_str());
					boost::filesystem::remove_all(boost::filesystem::path(path));
					path = StrUtil::printf("%srt/orders/", _base_dir.c_str());
					boost::filesystem::remove_all(boost::filesystem::path(path));
					path = StrUtil::printf("%srt/queue/", _base_dir.c_str());
					boost::filesystem::remove_all(boost::filesystem::path(path));
					path = StrUtil::printf("%srt/trans/", _base_dir.c_str());
					boost::filesystem::remove_all(boost::filesystem::path(path));
					break;
				}
				catch (...)
				{
					pipe_writer_log(_sink, LL_ERROR, "Error occured while clearing rt cache files��retry in 300s");
					std::this_thread::sleep_for(std::chrono::seconds(300));
					continue;
				}
			} while (true);

			continue;
		}
		else if (StrUtil::startsWith(fullcode.c_str(), "MARK.", false))
		{
			//���ָ����MARK.��ͷ,˵���Ǳ��ָ��,Ҫдһ�����
			std::string filename = _base_dir + MARKER_FILE;
			std::string sid = fullcode.substr(5);
			uint32_t curDate = TimeUtils::getCurDate();
			IniHelper iniHelper;
			iniHelper.load(filename.c_str());
			iniHelper.writeInt("markers", sid.c_str(), curDate);
			iniHelper.save();
			pipe_writer_log(_sink, LL_INFO, "ClosingTask mark of Trading session [{}] updated: {}", sid.c_str(), curDate);
		}

		auto pos = fullcode.find(".");
		std::string exchg = fullcode.substr(0, pos);
		std::string code = fullcode.substr(pos + 1);
		WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
		if (ct == NULL)
			continue;

		//�����ʷ���ݱ����ã����ٽ���������ҵ
		if (!_disable_his)
		{
			uint32_t count = 0;

			uint32_t uDate = _sink->getTradingDate(ct->getFullCode());
			//ת��ʵʱtick����
			if (!_disable_tick)
			{
				TickBlockPair *tBlkPair = getTickBlock(ct, uDate, false);
				if (tBlkPair != NULL)
				{
					if (tBlkPair->_fstream)
						tBlkPair->_fstream.reset();

					if (tBlkPair->_block->_size > 0)
					{
						pipe_writer_log(_sink, LL_INFO, "Transfering tick data of {}...", fullcode.c_str());
						SpinLock lock(tBlkPair->_mutex);

						for (auto& item : _dumpers)
						{
							const char* id = item.first.c_str();
							IHisDataDumper* dumper = item.second;
							bool bSucc = dumper->dumpHisTicks(fullcode.c_str(), tBlkPair->_block->_date, tBlkPair->_block->_ticks, tBlkPair->_block->_size);
							if (!bSucc)
							{
								pipe_writer_log(_sink, LL_ERROR, "ClosingTask of tick of {} on {} via extended dumper {} failed", fullcode.c_str(), tBlkPair->_block->_date, id);
							}
						}

						{//////////////////////////////////////////////////////////////////////////
							//dump tick data to dsb file
							std::stringstream ss;
							ss << _base_dir << "his/ticks/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
							std::string path = ss.str();
							pipe_writer_log(_sink, LL_INFO, path.c_str());
							BoostFile::create_directories(ss.str().c_str());
							std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

							bool bNew = false;
							if (!BoostFile::exists(filename.c_str()))
								bNew = true;

							pipe_writer_log(_sink, LL_INFO, "Openning data storage file: {}", filename.c_str());
							BoostFile f;
							if (f.create_new_file(filename.c_str()))
							{
								//��ѹ������
								std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_ticks, sizeof(WTSTickStruct)*tBlkPair->_block->_size);

								BlockHeaderV2 header;
								strcpy(header._blk_flag, BLK_FLAG);
								header._type = BT_HIS_Ticks;
								header._version = BLOCK_VERSION_CMP_V2;
								header._size = cmp_data.size();
								f.write_file(&header, sizeof(header));

								f.write_file(cmp_data.c_str(), cmp_data.size());
								f.close_file();

								count += tBlkPair->_block->_size;

								//��󽫻������
								//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
								tBlkPair->_block->_size = 0;
							}
							else
							{
								pipe_writer_log(_sink, LL_ERROR, "ClosingTask of tick failed: openning history data file {} failed", filename.c_str());
							}
						}
					}
				}

				if (tBlkPair)
					releaseBlock<TickBlockPair>(tBlkPair);
			}

			//ת��ʵʱtrans����
			if (!_disable_trans)
			{
				TransBlockPair *tBlkPair = getTransBlock(ct, uDate, false);
				if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
				{
					pipe_writer_log(_sink, LL_INFO, "Transfering transaction data of {}...", fullcode.c_str());
					SpinLock lock(tBlkPair->_mutex);

					for (auto& item : _dumpers)
					{
						const char* id = item.first.c_str();
						IHisDataDumper* dumper = item.second;
						bool bSucc = dumper->dumpHisTrans(fullcode.c_str(), tBlkPair->_block->_date, tBlkPair->_block->_trans, tBlkPair->_block->_size);
						if (!bSucc)
						{
							pipe_writer_log(_sink, LL_ERROR, "ClosingTask of transaction of {} on {} via extended dumper {} failed", fullcode.c_str(), tBlkPair->_block->_date, id);
						}
					}

					{
						std::stringstream ss;
						ss << _base_dir << "his/trans/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
						std::string path = ss.str();
						pipe_writer_log(_sink, LL_INFO, path.c_str());
						BoostFile::create_directories(ss.str().c_str());
						std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

						bool bNew = false;
						if (!BoostFile::exists(filename.c_str()))
							bNew = true;

						pipe_writer_log(_sink, LL_INFO, "Openning data storage file: {}", filename.c_str());
						BoostFile f;
						if (f.create_new_file(filename.c_str()))
						{
							//��ѹ������
							std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_trans, sizeof(WTSTransStruct)*tBlkPair->_block->_size);

							BlockHeaderV2 header;
							strcpy(header._blk_flag, BLK_FLAG);
							header._type = BT_HIS_Trnsctn;
							header._version = BLOCK_VERSION_CMP_V2;
							header._size = cmp_data.size();
							f.write_file(&header, sizeof(header));

							f.write_file(cmp_data.c_str(), cmp_data.size());
							f.close_file();

							count += tBlkPair->_block->_size;

							//��󽫻������
							//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
							tBlkPair->_block->_size = 0;
						}
						else
						{
							pipe_writer_log(_sink, LL_ERROR, "ClosingTask of transaction failed: openning history data file {} failed", filename.c_str());
						}
					}
				}

				if (tBlkPair)
					releaseBlock<TransBlockPair>(tBlkPair);
			}

			//ת��ʵʱorder����
			if (!_disable_orddtl)
			{
				OrdDtlBlockPair *tBlkPair = getOrdDtlBlock(ct, uDate, false);
				if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
				{
					pipe_writer_log(_sink, LL_INFO, "Transfering order detail data of {}...", fullcode.c_str());
					SpinLock lock(tBlkPair->_mutex);

					for (auto& item : _dumpers)
					{
						const char* id = item.first.c_str();
						IHisDataDumper* dumper = item.second;
						bool bSucc = dumper->dumpHisOrdDtl(fullcode.c_str(), tBlkPair->_block->_date, tBlkPair->_block->_details, tBlkPair->_block->_size);
						if (!bSucc)
						{
							pipe_writer_log(_sink, LL_ERROR, "ClosingTask of order details of {} on {} via extended dumper {} failed", fullcode.c_str(), tBlkPair->_block->_date, id);
						}
					}

					{
						std::stringstream ss;
						ss << _base_dir << "his/orders/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
						std::string path = ss.str();
						pipe_writer_log(_sink, LL_INFO, path.c_str());
						BoostFile::create_directories(ss.str().c_str());
						std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

						bool bNew = false;
						if (!BoostFile::exists(filename.c_str()))
							bNew = true;

						pipe_writer_log(_sink, LL_INFO, "Openning data storage file: {}", filename.c_str());
						BoostFile f;
						if (f.create_new_file(filename.c_str()))
						{
							//��ѹ������
							std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_details, sizeof(WTSOrdDtlStruct)*tBlkPair->_block->_size);

							BlockHeaderV2 header;
							strcpy(header._blk_flag, BLK_FLAG);
							header._type = BT_HIS_OrdDetail;
							header._version = BLOCK_VERSION_CMP_V2;
							header._size = cmp_data.size();
							f.write_file(&header, sizeof(header));

							f.write_file(cmp_data.c_str(), cmp_data.size());
							f.close_file();

							count += tBlkPair->_block->_size;

							//��󽫻������
							//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
							tBlkPair->_block->_size = 0;
						}
						else
						{
							pipe_writer_log(_sink, LL_ERROR, "ClosingTask of order detail failed: openning history data file {} failed", filename.c_str());
						}
					}
				}

				if (tBlkPair)
					releaseBlock<OrdDtlBlockPair>(tBlkPair);
			}

			//ת��ʵʱqueue����
			if (!_disable_ordque)
			{
				OrdQueBlockPair *tBlkPair = getOrdQueBlock(ct, uDate, false);
				if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
				{
					pipe_writer_log(_sink, LL_INFO, "Transfering order queue data of {}...", fullcode.c_str());
					SpinLock lock(tBlkPair->_mutex);

					for (auto& item : _dumpers)
					{
						const char* id = item.first.c_str();
						IHisDataDumper* dumper = item.second;
						bool bSucc = dumper->dumpHisOrdQue(fullcode.c_str(), tBlkPair->_block->_date, tBlkPair->_block->_queues, tBlkPair->_block->_size);
						if (!bSucc)
						{
							pipe_writer_log(_sink, LL_ERROR, "ClosingTask of order queues of {} on {} via extended dumper {} failed", fullcode.c_str(), tBlkPair->_block->_date, id);
						}
					}

					{
						std::stringstream ss;
						ss << _base_dir << "his/queue/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
						std::string path = ss.str();
						pipe_writer_log(_sink, LL_INFO, path.c_str());
						BoostFile::create_directories(ss.str().c_str());
						std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

						bool bNew = false;
						if (!BoostFile::exists(filename.c_str()))
							bNew = true;

						pipe_writer_log(_sink, LL_INFO, "Openning data storage file: {}", filename.c_str());
						BoostFile f;
						if (f.create_new_file(filename.c_str()))
						{
							//��ѹ������
							std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_queues, sizeof(WTSOrdQueStruct)*tBlkPair->_block->_size);

							BlockHeaderV2 header;
							strcpy(header._blk_flag, BLK_FLAG);
							header._type = BT_HIS_OrdQueue;
							header._version = BLOCK_VERSION_CMP_V2;
							header._size = cmp_data.size();
							f.write_file(&header, sizeof(header));

							f.write_file(cmp_data.c_str(), cmp_data.size());
							f.close_file();

							count += tBlkPair->_block->_size;

							//��󽫻������
							//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
							tBlkPair->_block->_size = 0;
						}
						else
						{
							pipe_writer_log(_sink, LL_ERROR, "ClosingTask of order queue failed: openning history data file {} failed", filename.c_str());
						}
					}
				}

				if (tBlkPair)
					releaseBlock<OrdQueBlockPair>(tBlkPair);
			}

			//ת����ʷK��
			dump_bars_via_dumper(ct);

			count += dump_bars_to_file(ct);

			pipe_writer_log(_sink, LL_INFO, "ClosingTask of {}[{}] done, {} datas processed totally", ct->getCode(), ct->getExchg(), count);
		}
		else
		{
			pipe_writer_log(_sink, LL_INFO, "ClosingTask of {}[{}] skipped due to history data disabled", ct->getCode(), ct->getExchg());
		}
	}
}