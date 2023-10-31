#include "ShmCaster.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/BoostFile.hpp"
#include "../WTSTools/WTSLogger.h"

bool ShmCaster::init(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	if (!cfg->getBoolean("active"))
		return false;

	_path = cfg->getCString("path");

	//每次启动都重置该队列
	{
		BoostFile bf;
		bf.create_or_open_file(_path.c_str());
		bf.truncate_file(sizeof(CastQueue));
		bf.close_file();
	}

	_mapfile.reset(new BoostMappingFile);
	_mapfile->map(_path.c_str());
	_queue = (CastQueue*)_mapfile->addr();
	new(_mapfile->addr()) CastQueue();

#ifdef _MSC_VER
	_queue->_pid = _getpid();
#else
	_queue->_pid = getpid();
#endif

	_inited = true;
	WTSLogger::info("ShmCaste initialized @ {}", _path.c_str());

	return true;
}

void ShmCaster::broadcast(WTSTickData* curTick)
{
	if (curTick == NULL || _queue == NULL || !_inited)
		return;

	/*
	 *	先移动写的下标，然后写入数据
	 *	写完了以后，再移动读的下标
	 */
	uint64_t wIdx = _queue->_writable++;
	uint64_t realIdx = wIdx % _queue->_capacity;
	_queue->_items[realIdx]._type = 0;
	memcpy(&_queue->_items[realIdx]._tick, &curTick->getTickStruct(), sizeof(WTSTickStruct));
	_queue->_readable = wIdx;
}

void ShmCaster::broadcast(WTSOrdQueData* curOrdQue)
{
	if (curOrdQue == NULL || _queue == NULL || !_inited)
		return;

	/*
	 *	先移动写的下标，然后写入数据
	 *	写完了以后，再移动读的下标
	 */
	uint64_t wIdx = _queue->_writable++;
	uint64_t realIdx = wIdx % _queue->_capacity;
	_queue->_items[realIdx]._type = 1;
	memcpy(&_queue->_items[realIdx]._queue, &curOrdQue->getOrdQueStruct(), sizeof(WTSOrdQueStruct));
	_queue->_readable = wIdx;
}

void ShmCaster::broadcast(WTSOrdDtlData* curOrdDtl)
{
	if (curOrdDtl == NULL || _queue == NULL || !_inited)
		return;

	/*
	 *	先移动写的下标，然后写入数据
	 *	写完了以后，再移动读的下标
	 */
	uint64_t wIdx = _queue->_writable++;
	uint64_t realIdx = wIdx % _queue->_capacity;
	_queue->_items[realIdx]._type = 2;
	memcpy(&_queue->_items[realIdx]._order, &curOrdDtl->getOrdDtlStruct(), sizeof(WTSOrdDtlStruct));
	_queue->_readable = wIdx;
}

void ShmCaster::broadcast(WTSTransData* curTrans)
{
	if (curTrans == NULL || _queue == NULL || !_inited)
		return;

	/*
	 *	先移动写的下标，然后写入数据
	 *	写完了以后，再移动读的下标
	 */
	uint64_t wIdx = _queue->_writable++;
	uint64_t realIdx = wIdx % _queue->_capacity;
	_queue->_items[realIdx]._type = 3;
	memcpy(&_queue->_items[realIdx]._trans, &curTrans->getTransStruct(), sizeof(WTSTransStruct));
	_queue->_readable = wIdx;
}