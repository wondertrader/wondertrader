#pragma once
#include "IExecCommand.h"
#include <unordered_map>

NS_OTP_BEGIN
class WTSVariant;

class WtDistExecuter : public IExecCommand
{
public:
	WtDistExecuter(const char* name);
	virtual ~WtDistExecuter();

public:
	/*
	 *	初始化执行器
	 *	传入初始化参数
	 */
	bool init(WTSVariant* params);


	const char* name() const { return _name.c_str(); }

public:
	//////////////////////////////////////////////////////////////////////////
	//IExecCommand
	virtual void set_position(const std::unordered_map<std::string, double>& targets) override;


	virtual void on_position_changed(const char* stdCode, double targetPos) override;


	virtual void on_tick(const char* stdCode, WTSTickData* newTick) override;

private:
	void		writeLog(const char* fmt, ...);

private:
	WTSVariant*			_config;
	std::string			_name;

	uint32_t			_scale;

	std::unordered_map<std::string, double> _target_pos;
};
NS_OTP_END

