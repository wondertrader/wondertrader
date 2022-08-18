/*!
 * \file WtExecuter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#pragma once
#include "ITrdNotifySink.h"
#include "IExecCommand.h"
#include "WtExecuterFactory.h"
#include "../Includes/ExecuteDefs.h"
#include "../Share/threadpool.hpp"
#include "../Share/SpinMutex.hpp"

NS_WTP_BEGIN
class WTSVariant;
class IDataManager;
class TraderAdapter;
class IHotMgr;

//本地执行器
class WtLocalExecuter : public ExecuteContext,
	public ITrdNotifySink, public IExecCommand
{
public:
	WtLocalExecuter(WtExecuterFactory* factory, const char* name, IDataManager* dataMgr);
	virtual ~WtLocalExecuter();

public:
	/*
	 *	初始化执行器
	 *	传入初始化参数
	 */
	bool init(WTSVariant* params);

	void setTrader(TraderAdapter* adapter);

private:
	ExecuteUnitPtr	getUnit(const char* code, bool bAutoCreate = true);

public:
	//////////////////////////////////////////////////////////////////////////
	//ExecuteContext
	virtual WTSTickSlice* getTicks(const char* code, uint32_t count, uint64_t etime = 0) override;

	virtual WTSTickData*	grabLastTick(const char* code) override;

	virtual double		getPosition(const char* stdCode, bool validOnly = true, int32_t flag = 3) override;
	virtual OrderMap*	getOrders(const char* code) override;
	virtual double		getUndoneQty(const char* code) override;

	virtual OrderIDs	buy(const char* code, double price, double qty, bool bForceClose = false) override;
	virtual OrderIDs	sell(const char* code, double price, double qty, bool bForceClose = false) override;
	virtual bool		cancel(uint32_t localid) override;
	virtual OrderIDs	cancel(const char* code, bool isBuy, double qty) override;
	virtual void		writeLog(const char* message) override;

	virtual WTSCommodityInfo*	getCommodityInfo(const char* stdCode) override;
	virtual WTSSessionInfo*		getSessionInfo(const char* stdCode) override;

	virtual uint64_t	getCurTime() override;

public:
	virtual bool			getMarketValue(double& market_value) override;

private:
	bool amountToPos(const char* stdCode, double amount, double& pos);
	bool ratioToPos(const char* stdCode, double ratio, double& pos);
	inline void checkTarget();

public:
	/*
	 *	设置目标仓位
	 */
	virtual void set_position(const faster_hashmap<LongKey, double>& targets) override;


	/*
	 *	合约仓位变动
	 */
	virtual void on_position_changed(const char* stdCode, double targetPos) override;

	/*
	 *	合约金额变动
	 */
	virtual void on_amount_changed(const char* stdCode, double targetAmount) override;

	/*
	 *	合约比例变动
	 */
	virtual void on_ratio_changed(const char* stdCode, double targetRatio) override;

	/*
	 *	实时行情回调
	 */
	virtual void on_tick(const char* stdCode, WTSTickData* newTick) override;

	/*
	 *	成交回报
	 */
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	/*
	 *	订单回报
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled = false) override;

	/*
	 *
	 */
	virtual void on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday) override;

	/*
	 *
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	/*
	 *	交易通道就绪
	 */
	virtual void on_channel_ready() override;

	/*
	 *	交易通道丢失
	 */
	virtual void on_channel_lost() override;

	/*
	 *	资金回报
	 */
	virtual void on_account(const char* currency, double prebalance, double balance, double dynbalance,
		double avaliable, double closeprofit, double dynprofit, double margin, double fee, double deposit, double withdraw) override;

private:
	ExecuteUnitMap		_unit_map;
	TraderAdapter*		_trader;
	WtExecuterFactory*	_factory;
	IDataManager*		_data_mgr;
	WTSVariant*			_config;

	double				_scale;				//放大倍数
	bool				_auto_clear;		//是否自动清理上一期的主力合约头寸
	bool				_strict_sync;		//是否严格同步目标仓位
	bool				_channel_ready;
	double				_fix_capital;		// 使用给定的固定资本计算比例持仓
	bool				_use_fix_capital;
	SpinMutex			_mtx_units;

	typedef struct _CodeGroup
	{
		char	_name[32] = { 0 };
		faster_hashmap<LongKey, double>	_items;
	} CodeGroup;
	typedef std::shared_ptr<CodeGroup> CodeGroupPtr;
	typedef faster_hashmap<LongKey, CodeGroupPtr>	CodeGroups;
	CodeGroups				_groups;			//合约组合（组合名称到组合的映射）
	CodeGroups				_code_to_groups;	//合约代码到组合的映射

	faster_hashset<LongKey>	_clear_includes;	//自动清理包含品种
	faster_hashset<LongKey>	_clear_excludes;	//自动清理排除品种

	faster_hashset<LongKey> _channel_holds;		//通道持仓

	faster_hashmap<LongKey, double> _target_pos;
	faster_hashmap<LongKey, double> _target_amount;
	faster_hashmap<LongKey, double> _target_ratio;
	double							_avaliable;
	typedef std::shared_ptr<boost::threadpool::pool> ThreadPoolPtr;
	ThreadPoolPtr		_pool;
};

typedef std::shared_ptr<IExecCommand> ExecCmdPtr;

NS_WTP_END
