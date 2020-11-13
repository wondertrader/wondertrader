#pragma once
#include "IExecCommand.h"

NS_OTP_BEGIN
class WtDistExecuter : public IExecCommand
{

public:
	//////////////////////////////////////////////////////////////////////////
	//IExecCommand
	virtual void set_position(const std::unordered_map<std::string, double>& targets) override;


	virtual void on_position_changed(const char* stdCode, double targetPos) override;


	virtual void on_tick(const char* stdCode, WTSTickData* newTick) override;

};
NS_OTP_END

