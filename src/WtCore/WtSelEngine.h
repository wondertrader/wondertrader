#pragma once
#include "WtEngine.h"
#include "WtExecMgr.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/ISelStraCtx.h"

#include <memory>

NS_WTP_BEGIN

typedef enum tagTaskPeriodType
{
	TPT_None,		//不重复
	TPT_Minute = 4,	//分钟周期
	TPT_Daily = 8,	//每个交易日
	TPT_Weekly,		//每周,遇到节假日的话要顺延
	TPT_Monthly,	//每月,遇到节假日顺延
	TPT_Yearly		//每年,遇到节假日顺延
}TaskPeriodType;

typedef struct _TaskInfo
{
	uint32_t	_id;
	char		_name[16];		//任务名
	char		_trdtpl[16];	//交易日模板
	char		_session[16];	//交易时间模板
	uint32_t	_day;			//日期,根据周期变化,每日为0,每周为0~6,对应周日到周六,每月为1~31,每年为0101~1231
	uint32_t	_time;			//时间,精确到分钟
	bool		_strict_time;	//是否是严格时间,严格时间即只有时间相等才会执行,不是严格时间,则大于等于触发时间都会执行

	uint64_t	_last_exe_time;	//上次执行时间,主要为了防止重复执行

	TaskPeriodType	_period;	//任务周期
} TaskInfo;

typedef std::shared_ptr<TaskInfo> TaskInfoPtr;

typedef std::shared_ptr<ISelStraCtx> SelContextPtr;
class WtSelRtTicker;


class WtSelEngine : public WtEngine, public IExecuterStub
{
public:
	WtSelEngine();
	~WtSelEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//WtEngine接口
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier) override;

	virtual void run() override;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void handle_push_quote(WTSTickData* newTick) override;

	virtual void on_init() override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

	///////////////////////////////////////////////////////////////////////////
	//IExecuterStub 接口
	virtual uint64_t get_real_time() override;
	virtual WTSCommodityInfo* get_comm_info(const char* stdCode) override;
	virtual WTSSessionInfo* get_sess_info(const char* stdCode) override;
	virtual IHotMgr* get_hot_mon() { return _hot_mgr; }
	virtual uint32_t get_trading_day() { return _cur_tdate; }

public:
	//uint32_t	register_task(const char* name, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict = true, const char* trdtpl = "CHINA");
	void			addContext(SelContextPtr ctx, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict = true, const char* trdtpl = "CHINA", const char* sessionID="TRADING");

	SelContextPtr	getContext(uint32_t id);

	inline void addExecuter(ExecCmdPtr& executer)
	{
		_exec_mgr.add_executer(executer);
		executer->setStub(this);
	}

	void	on_minute_end(uint32_t uDate, uint32_t uTime);

	void	handle_pos_change(const char* straName, const char* stdCode, double diffQty);

private:
	wt_hashmap<uint32_t, TaskInfoPtr>	_tasks;

	typedef wt_hashmap<uint32_t, SelContextPtr> ContextMap;
	ContextMap		_ctx_map;

	WtExecuterMgr	_exec_mgr;

	bool	_terminated;

	WtSelRtTicker*	_tm_ticker;
	WTSVariant*		_cfg;
};

NS_WTP_END
