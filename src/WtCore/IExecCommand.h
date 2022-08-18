#pragma once
#include "../Includes/FasterDefs.h"
#include <stdint.h>

NS_WTP_BEGIN
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
	IExecCommand(const char* name) :_stub(NULL), _name(name){}
	/*
	 *	����Ŀ���λ
	 */
	virtual void set_position(const faster_hashmap<LongKey, double>& targets) {}

	/*
	 *	��Լ��λ�䶯
	 */
	virtual void on_position_changed(const char* stdCode, double targetPos) {}

	/*
 *	��Լ���䶯
 */
	virtual void on_amount_changed(const char* stdCode, double targetAmount) {}

	/*
	 *	��Լ�����䶯
	 */
	virtual void on_ratio_changed(const char* stdCode, double targetRatio) {}

	/*
	 *	ʵʱ����ص�
	 */
	virtual void on_tick(const char* stdCode, WTSTickData* newTick) {}


	inline void setStub(IExecuterStub* stub) { _stub = stub; }

	inline const char* name() const { return _name.c_str(); }

	inline void setName(const char* name) { _name = name; }

protected:
	IExecuterStub*	_stub;
	std::string		_name;
};
NS_WTP_END