#include "WtDataWriter.h"

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

#pragma comment(lib, "libmysql.lib")


#include <set>

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

#ifdef _WIN32
#include <wtypes.h>
HMODULE	g_dllModule = NULL;

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dllModule = (HMODULE)hModule;
		break;
	}
	return TRUE;
}
#else
#include <dlfcn.h>

char PLATFORM_NAME[] = "UNIX";

std::string	g_moduleName;

__attribute__((constructor))
void on_load(void) {
	Dl_info dl_info;
	dladdr((void *)on_load, &dl_info);
	g_moduleName = dl_info.dli_fname;
}
#endif

std::string getBinDir()
{
	static std::string _bin_dir;
	if (_bin_dir.empty())
	{


#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		_bin_dir = g_moduleName;
#endif

		uint32_t nPos = _bin_dir.find_last_of('/');
		_bin_dir = _bin_dir.substr(0, nPos + 1);
	}

	return _bin_dir;
}



WtDataWriter::WtDataWriter()
	: _terminated(false)
	, _save_tick_log(false)
	, _log_group_size(1000)
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

void WtDataWriter::init_db()
{
	if (!_db_conf._active)
		return;

#ifdef _WIN32
	std::string module = getBinDir() + "libmysql.dll";
	DLLHelper::load_library(module.c_str());
#endif

	_db_conn.reset(new MysqlDb);
	my_bool autoreconnect = true;
	_db_conn->options(MYSQL_OPT_RECONNECT, &autoreconnect);
	_db_conn->options(MYSQL_SET_CHARSET_NAME, "utf8");

	if (_db_conn->connect(_db_conf._dbname, _db_conf._host, _db_conf._user, _db_conf._pass, _db_conf._port, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS))
	{
		if(_sink)
			_sink->outputWriterLog(LL_INFO, "数据库连接成功:%s:%d", _db_conf._host, _db_conf._port);
	}
	else
	{
		if (_sink)
			_sink->outputWriterLog(LL_ERROR, "数据库连接失败[%s:%d]:%s", _db_conf._host, _db_conf._port, _db_conn->errstr());
		_db_conn.reset();
	}
}

bool WtDataWriter::init(WTSVariant* params, IDataWriterSink* sink)
{
	_sink = sink;
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

	WTSVariant* dbConf = params->get("db");
	if(dbConf)
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

	loadCache();

	_proc_chk.reset(new BoostThread(boost::bind(&WtDataWriter::check_loop, this)));
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
}

/*
void DataManager::preloadRtCaches(const char* exchg)
{
	if (!_preload_enable || _preloaded)
		return;

	_sink->outputWriterLog(LL_INFO, "开始预加载实时数据缓存文件……");
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
	_sink->outputWriterLog(LL_INFO, "预加载%个品种的实时数据缓存文件%u个，耗时%s微秒", codecnt, cnt, WTSLogger::fmtInt64(ticker.micro_seconds()));
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
			std::string key = StrUtil::printf("%s.%s", item._tick.exchg, item._tick.code);
			_tick_cache_idx[key] = i;
		}
	}
}

/*
void* DataManager::resizeKBlock(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	RTKlineBlock* kBlock = (RTKlineBlock*)mfPtr->addr();
	if (kBlock->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uSize = sizeof(RTKlineBlock) + sizeof(WTSBarStruct)*nCount;
	std::filebuf fbuf;
	fbuf.open(filename, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	//这里没大搞明白
	fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
	fbuf.sputc(0);
	fbuf.close();

	BoostMappingFile* pNewMf = new BoostMappingFile();
	if (!pNewMf->map(filename))
	{
		delete pNewMf;
		return NULL;
	}

	mfPtr.reset(pNewMf);

	kBlock = (RTKlineBlock*)mfPtr->addr();
	kBlock->_capacity = nCount;

	return mfPtr->addr();
}
*/

template<typename HeaderType, typename T>
void* WtDataWriter::resizeRTBlock(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	//调用该函数之前，应该已经申请了写锁了
	RTBlockHeader* tBlock = (RTBlockHeader*)mfPtr->addr();
	if (tBlock->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uSize = sizeof(HeaderType) + sizeof(T)*nCount;
	try
	{
		std::filebuf fbuf;
		fbuf.open(filename, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
		//这里没大搞明白
		fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
		fbuf.sputc(0);
		fbuf.close();
	}
	catch(std::exception& ex)
	{
		_sink->outputWriterLog(LL_ERROR, "%s[%u]的实时缓存文件扩容出现异常: %s", ex.what());
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

/*
void* DataManager::resizeTCache(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	RTTickCache* tCache = (RTTickCache*)mfPtr->addr();
	if (tCache->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uSize = sizeof(RTTickCache) + sizeof(TickCacheItem)*nCount;
	std::filebuf fbuf;
	fbuf.open(filename, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	//这里没大搞明白
	fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
	fbuf.sputc(0);
	fbuf.close();

	BoostMappingFile* pNewMf = new BoostMappingFile();
	if (!pNewMf->map(filename))
	{
		delete pNewMf;
		return NULL;
	}

	mfPtr.reset(pNewMf);

	tCache = (RTTickCache*)mfPtr->addr();
	tCache->_capacity = nCount;
	return mfPtr->addr();
}
*/

bool WtDataWriter::writeTick(WTSTickData* curTick, bool bNeedSlice /* = true */)
{
	if (curTick == NULL)
		return false;

	curTick->retain();
	pushTask([this, curTick, bNeedSlice](){

		do
		{
			WTSContractInfo* ct = _bd_mgr->getContract(curTick->code(), curTick->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			//先更新缓存
			if (!updateCache(ct, curTick, bNeedSlice))
				break;

			//写到tick缓存
			pipeToTicks(ct, curTick);

			//写到K线缓存
			pipeToKlines(ct, curTick);

			_sink->broadcastTick(curTick);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTick->exchg()]++;
			if (_tcnt_map[curTick->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的tick数据%s条", curTick->exchg(), StrUtil::fmtUInt64(_tcnt_map[curTick->exchg()]).c_str());
			}
		} while (false);

		curTick->release();
	});
	return true;
}

bool WtDataWriter::writeOrderQueue(WTSOrdQueData* curOrdQue)
{
	if (curOrdQue == NULL)
		return false;

	curOrdQue->retain();
	pushTask([this, curOrdQue](){

		do
		{
			WTSContractInfo* ct = _bd_mgr->getContract(curOrdQue->code(), curOrdQue->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdQueBlockPair* pBlockPair = getOrdQueBlock(ct, curOrdQue->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//先检查容量够不够，不够要扩
			RTOrdQueBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdQueBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdQueStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_queues[blk->_size], &curOrdQue->getOrdQueStruct(), sizeof(WTSOrdQueStruct));
			blk->_size += 1;

			//TODO: 要广播的
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdQue->exchg()]++;
			if (_tcnt_map[curOrdQue->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的委托队列数据%s条", curOrdQue->exchg(), StrUtil::fmtUInt64(_tcnt_map[curOrdQue->exchg()]).c_str());
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
		BoostUniqueLock lck(_task_mtx);
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
					BoostUniqueLock lck(_task_mtx);
					_task_cond.wait(_task_mtx);
					continue;
				}

				std::queue<TaskInfo> tempQueue;
				{
					BoostUniqueLock lck(_task_mtx);
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
	if (curOrdDtl == NULL)
		return false;

	curOrdDtl->retain();
	pushTask([this, curOrdDtl](){

		do
		{

			WTSContractInfo* ct = _bd_mgr->getContract(curOrdDtl->code(), curOrdDtl->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdDtlBlockPair* pBlockPair = getOrdDtlBlock(ct, curOrdDtl->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//先检查容量够不够，不够要扩
			RTOrdDtlBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdDtlBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdDtlStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_details[blk->_size], &curOrdDtl->getOrdDtlStruct(), sizeof(WTSOrdDtlStruct));
			blk->_size += 1;

			//TODO: 要广播的
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdDtl->exchg()]++;
			if (_tcnt_map[curOrdDtl->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的逐笔委托数据%s条", curOrdDtl->exchg(), StrUtil::fmtUInt64(_tcnt_map[curOrdDtl->exchg()]).c_str());
			}
		} while (false);

		curOrdDtl->release();
	});
	
	return true;
}

bool WtDataWriter::writeTransaction(WTSTransData* curTrans)
{
	if (curTrans == NULL)
		return false;

	curTrans->retain();
	pushTask([this, curTrans](){

		do
		{

			WTSContractInfo* ct = _bd_mgr->getContract(curTrans->code(), curTrans->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			TransBlockPair* pBlockPair = getTransBlock(ct, curTrans->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//先检查容量够不够，不够要扩
			RTTransBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTTransBlock*)resizeRTBlock<RTDayBlockHeader, WTSTransStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_trans[blk->_size], &curTrans->getTransStruct(), sizeof(WTSTransStruct));
			blk->_size += 1;

			//TODO: 要广播的
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTrans->exchg()]++;
			if (_tcnt_map[curTrans->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的逐笔成交数据%s条", curTrans->exchg(), StrUtil::fmtUInt64(_tcnt_map[curTrans->exchg()]).c_str());
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

	BoostUniqueLock lock(pBlockPair->_mutex);

	//先检查容量够不够，不够要扩
	RTTickBlock* blk = pBlockPair->_block;
	if(blk->_size >= blk->_capacity)
	{
		pBlockPair->_file->sync();
		pBlockPair->_block = (RTTickBlock*)resizeRTBlock<RTDayBlockHeader, WTSTickStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
		blk = pBlockPair->_block;
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
			<< curTick->totalvolumn() << ","
			<< curTick->openinterest() << ","
			<< (uint64_t)curTick->totalturnover() << ","
			<< curTick->volumn() << ","
			<< curTick->additional() << ","
			<< (uint64_t)curTick->turnover() << std::endl;
	}
}

WtDataWriter::OrdQueBlockPair* WtDataWriter::getOrdQueBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	OrdQueBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_ordque_blocks[key];
	if (pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/queue/%s/", _base_dir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdQueBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "orderqueue缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_queues, 0, sizeof(WTSOrdQueStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_OrdQueue;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			do
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdQueStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s股票%u的委托队列缓存文件已修复", ct->getCode(), curDate);
				}

			} while (false);

		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

WtDataWriter::OrdDtlBlockPair* WtDataWriter::getOrdDtlBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	OrdDtlBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_orddtl_blocks[key];
	if (pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/orders/%s/", _base_dir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdDtlBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "orders缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_details, 0, sizeof(WTSOrdDtlStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_OrdDetail;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdDtlStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s股票%u的逐笔成交缓存文件已修复", ct->getCode(), curDate);
				}

				break;
			}

		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

WtDataWriter::TransBlockPair* WtDataWriter::getTransBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	TransBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_trans_blocks[key];
	if (pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/trans/%s/", _base_dir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTransBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "trans缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_trans, 0, sizeof(WTSTransStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_Trnsctn;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSTransStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s股票%u的逐笔成交缓存文件已修复", ct->getCode(), curDate);
				}

				break;
			}

		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

WtDataWriter::TickBlockPair* WtDataWriter::getTickBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	TickBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_ticks_blocks[key];
	if(pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/ticks/%s/", _base_dir.c_str(), ct->getExchg());
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

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * TICK_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if(!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTickBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "tick缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_ticks, 0, sizeof(WTSTickStruct)*pBlock->_block->_capacity);
		}

		if(isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_Ticks;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			for (;;)
			{
				uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTTickBlock)) / sizeof(WTSTickStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s合约%u的tick缓存文件已修复", ct->getCode(), curDate);
				}
				
				break;
			}
			
		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

void WtDataWriter::pipeToKlines(WTSContractInfo* ct, WTSTickData* curTick)
{
	uint32_t uDate = curTick->actiondate();
	WTSSessionInfo* sInfo = _bd_mgr->getSessionByCode(curTick->code(), curTick->exchg());
	uint32_t curTime = curTick->actiontime() / 100000;

	uint32_t minutes = sInfo->timeToMinutes(curTime, false);
	if (minutes == INVALID_UINT32)
		return;

	//当秒数为0，要专门处理，比如091500000，这笔tick要算作0915的
	//如果是小节结束，要算作小节结束那一分钟，因为经常会有超过结束时间的价格进来，如113000500
	//不能同时处理，所以用or	
	if (sInfo->isLastOfSection(curTime))
	{
		minutes--;
	}

	//更新1分钟线
	KBlockPair* pBlockPair = getKlineBlock(ct, KP_Minute1);
	if (pBlockPair && pBlockPair->_block)
	{
		BoostUniqueLock lock(pBlockPair->_mutex);
		RTKlineBlock* blk = pBlockPair->_block;
		if (blk->_size == blk->_capacity)
		{
			pBlockPair->_file->sync();
			pBlockPair->_block = (RTKlineBlock*)resizeRTBlock<RTKlineBlock, WTSBarStruct>(pBlockPair->_file, blk->_capacity + KLINE_SIZE_STEP);
			blk = pBlockPair->_block;
		}

		WTSBarStruct* lastBar = NULL;
		if(blk->_size > 0)
		{
			lastBar = &blk->_bars[blk->_size - 1];
		}

		//拼接1分钟线
		uint32_t barMins = minutes + 1;
		uint32_t barTime = sInfo->minuteToTime(barMins);
		uint32_t barDate = uDate;
		if (barTime == 0)
		{
			barDate = TimeUtils::getNextDate(barDate);
		}
		barTime = TimeUtils::timeToMinBar(barDate, barTime);

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

			newBar->vol = curTick->volumn();
			newBar->money = curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add = curTick->additional();
		}
		else
		{
			newBar = &blk->_bars[blk->_size-1];

			newBar->close = curTick->price();
			newBar->high = max(curTick->price(), newBar->high);
			newBar->low = min(curTick->price(), newBar->low);

			newBar->vol += curTick->volumn();
			newBar->money += curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add += curTick->additional();
		}
	}


	pBlockPair = getKlineBlock(ct, KP_Minute5);
	if (pBlockPair && pBlockPair->_block)
	{
		BoostUniqueLock lock(pBlockPair->_mutex);
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
		uint32_t barTime = sInfo->minuteToTime(barMins);
		uint32_t barDate = uDate;
		if (barTime == 0)
		{
			barDate = TimeUtils::getNextDate(barDate);
		}
		barTime = TimeUtils::timeToMinBar(barDate, barTime);

		bool bNew = fa