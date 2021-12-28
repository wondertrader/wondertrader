#pragma once
#include <string>
#include <stdint.h>

#include "DataDefineAD.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/IDataReader.h"

#include "../Share/BoostMappingFile.hpp"

NS_WTP_BEGIN

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class WtDataReaderAD : public IDataReader
{
public:
	WtDataReaderAD();
	virtual ~WtDataReaderAD();

private:
	/*
	 *	将历史数据放入缓存
	 */
	bool	cacheBarsFromStorage(const std::string& key, const char* stdCode, WTSKlinePeriod period);

public:
	virtual void init(WTSVariant* cfg, IDataReaderSink* sink, IHisDataLoader* loader = NULL) override;

	virtual void onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0) override;

	virtual WTSTickSlice*	readTickSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;

	virtual WTSKlineSlice*	readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) override;

private:
	std::string		_base_dir;
	IBaseDataMgr*	_base_data_mgr;
	IHotMgr*		_hot_mgr;

	typedef struct _BarsList
	{
		std::string		_exchg;
		std::string		_code;
		WTSKlinePeriod	_period;
		uint32_t		_size;
		uint32_t		_max_req_cnt;	//用户请求的最大数量
		/*	By Wesley @ 2021.12.27
		 *	加一个容量
		 *	第一次分配初始容量
		 *	如果用户请求的数据，超过容量，则重新按照用户请求的真实长度L，按L*N作为新的容量重新分配大小
		 *	当缓存的数据长度_size和容量一致时，再把最后L条数据拷贝到前面去，如此反复循环
		 */
		uint32_t		_capacity;	
		//最后一条是否从缓存里读取的，如果是下次更新的时候要从lmdb更新一次，最后一条再按照原有逻辑处理
		bool			_last_from_cache;

		std::vector<WTSBarStruct>	_bars;

		_BarsList() :_size(0), _capacity(0){}
	} BarsList;

	typedef struct _TicksList
	{
		std::string		_exchg;
		std::string		_code;
		uint32_t		_size;
		uint32_t		_max_req_cnt;	//用户请求的最大数量
		/*	By Wesley @ 2021.12.27
		 *	加一个容量
		 *	第一次分配初始容量
		 *	如果用户请求的数据，超过容量，则重新按照用户请求的真实长度L，按L*N作为新的容量重新分配大小
		 *	当缓存的数据长度_size和容量一致时，再把最后L条数据拷贝到前面去，如此反复循环
		 */
		uint32_t		_capacity;

		std::vector<WTSTickStruct>	_ticks;

		_TicksList() :_size(0), _capacity(0){}
	} TicksList;

	typedef faster_hashmap<std::string, BarsList> BarsCache;
	BarsCache	_bars_cache;

	typedef faster_hashmap<std::string, TicksList> TicksCache;
	TicksCache	_ticks_cache;

	uint64_t	_last_time;	
};

NS_WTP_END
