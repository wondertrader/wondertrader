#pragma once
#include "WtEngine.h"
#include "WtExecMgr.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/ISelStraCtx.h"

#include <memory>

NS_WTP_BEGIN

typedef enum tagTaskPeriodType
{
	TPT_None,		//���ظ�
	TPT_Minute = 4,	//��������
	TPT_Daily = 8,	//ÿ��������
	TPT_Weekly,		//ÿ��,�����ڼ��յĻ�Ҫ˳��
	TPT_Monthly,	//ÿ��,�����ڼ���˳��
	TPT_Yearly		//ÿ��,�����ڼ���˳��
}TaskPeriodType;

typedef struct _TaskInfo
{
	uint32_t	_id;
	char		_name[16];		//������
	char		_trdtpl[16];	//������ģ��
	char		_session[16];	//����ʱ��ģ��
	uint32_t	_day;			//����,�������ڱ仯,ÿ��Ϊ0,ÿ��Ϊ0~6,��Ӧ���յ�����,ÿ��Ϊ1~31,ÿ��Ϊ0101~1231
	uint32_t	_time;			//ʱ��,��ȷ������
	bool		_strict_time;	//�Ƿ����ϸ�ʱ��,�ϸ�ʱ�伴ֻ��ʱ����ȲŻ�ִ��,�����ϸ�ʱ��,����ڵ��ڴ���ʱ�䶼��ִ��

	uint64_t	_last_exe_time;	//�ϴ�ִ��ʱ��,��ҪΪ�˷�ֹ�ظ�ִ��

	TaskPeriodType	_period;	//��������
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
	//WtEngine�ӿ�
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier) override;

	virtual void run(bool bAsync = false) override;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void handle_push_quote(WTSTickData* newTick, uint32_t hotFlag) override;

	virtual void on_init() override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

	///////////////////////////////////////////////////////////////////////////
	//IExecuterStub �ӿ�
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
	faster_hashmap<uint32_t, TaskInfoPtr>	_tasks;

	typedef faster_hashmap<uint32_t, SelContextPtr> ContextMap;
	ContextMap		_ctx_map;

	WtExecuterMgr	_exec_mgr;

	bool	_terminated;

	WtSelRtTicker*	_tm_ticker;
	WTSVariant*		_cfg;
};

NS_WTP_END
