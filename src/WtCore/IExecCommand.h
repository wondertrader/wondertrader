#pragma once
#include "../Includes/WTSMarcos.h"
#include <stdint.h>
#include <unordered_map>

NS_OTP_BEGIN
class WTSCommodityInfo;
class WTSSessionInfo;
class IHotMgr;
class WTSTickData;

class IExecuterStub
{
public:
	virtual uint64_t get_real_time() = 0;
	virtual WTSCommodityInfo* get_comm_info(const char* stdCode) = 0;
	virtual WTSSessionInfo* get_sess_info(const char* stdCode) = 0;
	virtual IHotMgr* get_hot_mon() = 0;
	virtual uint32_t get_trading_day() = 0;
};

class IExecCommand
{
public:
	IExecCommand() :_stub(NULL) {}
	/*
	 *	设置目标仓位
	 */
	virtual void set_position(const std::unordered_map<std::string, double>& targets) {}

	/*
	 *	合约仓位变动
	 */
	virtual void on_position_changed(const char* stdCode, double targetPos) {}

	/*
	 *	实时行情回调
	 */
	virtual void on_tick(const char* stdCode, WTSTickData* newTick) {}

	void setStub(IExecuterStub* stub) { _stub = stub; }

protected:
	IExecuterStub* _stub;
};
NS_OTP_END