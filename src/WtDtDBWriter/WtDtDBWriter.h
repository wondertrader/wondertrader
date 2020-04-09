#pragma once
#include "DataDefine.h"

#include "../Share/IDataWriter.h"
#include "../Share/BoostDefine.h"
#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"

#include <queue>
#include <unordered_map>

typedef boost::shared_ptr<BoostMappingFile> BoostMFPtr;

USING_NS_OTP;

class WtDtDBWriter : public IDataWriter
{
public:
	WtDtDBWriter();
	~WtDtDBWriter();	

private:
	void  proc_loop();

public:
	virtual bool init(WTSVariant* params, IDataWriterSink* sink) override;
	virtual void release() override;

	virtual bool writeTick(WTSTickData* curTick, bool bNeedProc = true) override;

	virtual bool writeOrderQueue(WTSOrdQueData* curOrdQue) override;

	virtual bool writeOrderDetail(WTSOrdDtlData* curOrdDetail) override;

	virtual bool writeTransaction(WTSTransData* curTrans) override;

	virtual void transHisData(const char* sid) override;
	
	virtual bool isSessionProceeded(const char* sid) override;

	virtual WTSTickData* getCurTick(const char* code) override;

private:
	IDataWriterSink*	_sink;
	IBaseDataMgr*		_bd_mgr;

	BoostUniqueMutex	_mtx_tick_cache;
	std::unordered_map<std::string, uint32_t> _tick_cache_idx;
	BoostMFPtr		_tick_cache_file;
	RTTickCache*	_tick_cache_block;

	typedef std::function<void()> TaskInfo;
	std::queue<TaskInfo>	_tasks;
	StdThreadPtr			_task_thrd;
	BoostUniqueMutex		_task_mtx;
	BoostCondition			_task_cond;

	std::string		_base_dir;
	std::string		_cache_file;
	uint32_t		_log_group_size;
	bool			_async_proc;

	BoostCondition	 _proc_cond;
	BoostUniqueMutex _proc_mtx;
	std::queue<std::string> _proc_que;
	BoostThreadPtr	_proc_thrd;
	bool			_terminated;

	bool			_save_tick_log;
	
	std::map<std::string, uint32_t> _proc_date;

private:
	void loadCache();

	bool updateCache(WTSTickData* curTick, bool bNeedProc = true);

	void pipeToTicks(WTSTickData* curTick);

	void pipeToKlines(WTSTickData* curTick);

	void pushTask(TaskInfo task);
};

