#pragma once
#include "../WtCore/IExecCommand.h"

USING_NS_OTP;

class ExpExecuter : public IExecCommand
{
public:
	ExpExecuter(const char* name):IExecCommand(name){}

	void	init();


	virtual void set_position(const faster_hashmap<std::string, double>& targets) override;


	virtual void on_position_changed(const char* stdCode, double targetPos) override;

};

