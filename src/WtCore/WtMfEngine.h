#pragma once
#include "WtEngine.h"
#include "IMfStraCtx.h"
#include "WtExecuter.h"

#include <unordered_map>
#include <memory>

NS_OTP_BEGIN

typedef enum tagTaskPeriodType
{
	TPT_None,		//不重复
	TPT_Daily = 8,	//每个交易日
	TPT_Weekly,		//每周，遇到节假日的话要顺延
	TPT_Monthly,	//每月，遇到节假日顺延
	TPT_Yearly		//每年，遇到节假日顺延
}TaskPeriodType;

typedef struct _TaskInfo
{
	uint32_t	_id;
	char		_name[16];		//任务名
	char		_trdtpl[16];	//交易日模板
	uint32_t	_day;			//日期，根据周期变化，每日为0，每周为0~6，对应周日到周六，每月为1~31，每年为0101~1231
	uint32_t	_time;			//时间，精确到分钟
	bool		_strict_time;	//是否是严格时间，严格时间即只有时间相等才会执行，不是严格时间，则大于等于触发时间都会执行

	uint64_t	_last_exe_time;	//上次执行时间，主要为了防止重复执行

	TaskPeriodType	_period;	//任务周期
} TaskInfo;

typedef std::shared_ptr<TaskInfo> TaskInfoPtr;

typedef boost::shared_ptr<IMfStraCtx> MfContextPtr;
class WtMfRtTicker;


class WtMfEngine : public WtEngine
{
public:
	WtMfEngine();
	~WtMfEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//WtEngine接口
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr) override;

	virtual void run(bool bAsync = false) override;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void handle_push_quote(WTSTickData* newTick, bool isHot) override;

	virtual void on_init() override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

public:
	//uint32_t	register_task(const char* name, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict = true, const char* trdtpl = "CHINA");
	void			addContext(MfContextPtr ctx, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict = true, const char* trdtpl = "CHINA");

	MfContextPtr	getContext(uint32_t id);

	inline void addExecuter(WtExecuterPtr& executer)
	{
		_executers.push_back(executer);
		executer->setEngine(this);
	}

	void	on_minute_end(uint32_t uDate, uint32_t uTime);

private:
	std::unordered_map<uint32_t, TaskInfoPtr>	_tasks;

	typedef std::unordered_map<uint32_t, MfContextPtr> ContextMap;
	ContextMap		_ctx_map;

	typedef std::vector<WtExecuterPtr> ExecuterList;
	ExecuterList	_executers;

	bool	_terminated;

	WtMfRtTicker*	_tm_ticker;
	WTSVariant*		_cfg;
};

NS_OTP_END
