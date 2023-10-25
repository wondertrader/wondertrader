#pragma once
#include "IDataCaster.h"
#include <stdint.h>
#include "../Includes/WTSStruct.h"
#include "../Share/BoostMappingFile.hpp"

NS_WTP_BEGIN
class WTSVariant;
NS_WTP_END

USING_NS_WTP;

class ShmCaster : public IDataCaster
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
	ShmCaster():_queue(NULL), _inited(false){}

	bool	init(WTSVariant* cfg);

	virtual void	broadcast(WTSTickData* curTick) override;
	virtual void	broadcast(WTSOrdQueData* curOrdQue) override;
	virtual void	broadcast(WTSOrdDtlData* curOrdDtl) override;
	virtual void	broadcast(WTSTransData* curTrans) override;

private:
	std::string		_path;
	typedef std::shared_ptr<BoostMappingFile> MappedFilePtr;
	MappedFilePtr	_mapfile;
	CastQueue*		_queue;
	bool			_inited;
};

