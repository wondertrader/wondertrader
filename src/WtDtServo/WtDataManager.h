/*!
 * \file WtDataManager.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <vector>
#include <stdint.h>

#include "../Includes/IDataManager.h"
#include "../Includes/IRdmDtReader.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/WTSCollection.hpp"


class WtDtRunner;

NS_WTP_BEGIN
class WTSVariant;
class WTSTickData;
class WTSKlineSlice;
class WTSKlineData;
class WTSTickSlice;
class IBaseDataMgr;
class IHotMgr;
class WTSSessionInfo;
struct WTSBarStruct;

class WtDataManager : public IRdmDtReaderSink
{
public:
	WtDataManager();
	~WtDataManager();

private:
	bool	initStore(WTSVariant* cfg);

	WTSSessionInfo* get_session_info(const char* sid, bool isCode = false);

//////////////////////////////////////////////////////////////////////////
//IRdmDtReaderSink
public:
	/*
	 *	@brief	��ȡ�������ݹ���ӿ�ָ��
	 */
	virtual IBaseDataMgr*	get_basedata_mgr() override { return _bd_mgr; }

	/*
	 *	@brief	��ȡ�����л��������ӿ�ָ��
	 */
	virtual IHotMgr*		get_hot_mgr() override { return _hot_mgr; }

	/*
	 *	@brief	������ݶ�ȡģ�����־
	 */
	virtual void			reader_log(WTSLogLevel ll, const char* message) override;

public:
	bool	init(WTSVariant* cfg, WtDtRunner* runner);

	WTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSTransSlice* get_transaction_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);

	WTSTickSlice* get_tick_slice_by_date(const char* stdCode, uint32_t uDate = 0);
	WTSKlineSlice* get_skline_slice_by_date(const char* stdCode, uint32_t secs, uint32_t uDate = 0);
	WTSKlineSlice* get_kline_slice_by_date(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t uDate = 0);

	WTSTickSlice* get_tick_slices_by_range(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSKlineSlice* get_kline_slice_by_range(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint64_t stime, uint64_t etime = 0);

	WTSTickSlice* get_tick_slice_by_count(const char* stdCode, uint32_t count, uint64_t etime = 0);
	WTSKlineSlice* get_kline_slice_by_count(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0);

private:
	IRdmDtReader*			_reader;
	FuncDeleteRdmDtReader	_remover;

	IBaseDataMgr*	_bd_mgr;
	IHotMgr*		_hot_mgr;
	WtDtRunner*		_runner;

	//K�߻���
	typedef struct _BarCache
	{
		WTSKlineData*	_bars;
		uint64_t		_last_bartime;
		WTSKlinePeriod	_period;
		uint32_t		_times;

		_BarCache():_last_bartime(0),_period(KP_DAY),_times(1),_bars(NULL){}
	} BarCache;
	typedef faster_hashmap<std::string, BarCache>	BarCacheMap;
	BarCacheMap	_bars_cache;
};

NS_WTP_END