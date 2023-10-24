#pragma once
#include <stdint.h>
#include "../Includes/WTSStruct.h"
#include "../Share/BoostMappingFile.hpp"

NS_WTP_BEGIN
class WTSTickData;
class WTSQueue;
class WTSVariant;
class WTSOrdDtlData;
class WTSOrdQueData;
class WTSTransData;
NS_WTP_END

USING_NS_WTP;

class ShmCaster
{
public:
#pragma pack(push, 8)
	typedef struct _DataItem
	{
		uint32_t	_type;	//数据类型， 0-tick,1-委托队列,2-逐笔委托,3-逐笔成交
		union
		{
			WTSTickStruct	_tick;
			WTSOrdQueStruct _queue;
			WTSOrdDtlStruct	_order;
			WTSTransStruct	_trans;
		};

		_DataItem() { memset(this, 0, sizeof(_DataItem)); }
	} DataItem;

	template <int N = 8*1024>
	struct _DataQueue
	{
		uint64_t	_capacity = N;
		volatile uint64_t	_readable;
		volatile uint64_t	_writable;
		uint32_t	_pid;
		DataItem	_items[N];

		_DataQueue() :_readable(UINT64_MAX), _writable(0), _pid(0) {}
	};

	typedef _DataQueue<8*1024>	CastQueue;

#pragma pack(pop)

public:
	ShmCaster():_queue(NULL){}

	bool	init(WTSVariant* cfg);

	void	broadcast(WTSTickData* curTick);
	void	broadcast(WTSOrdQueData* curOrdQue);
	void	broadcast(WTSOrdDtlData* curOrdDtl);
	void	broadcast(WTSTransData* curTrans);

private:
	std::string		_path;
	typedef std::shared_ptr<BoostMappingFile> MappedFilePtr;
	MappedFilePtr	_mapfile;
	CastQueue*		_queue;
};

