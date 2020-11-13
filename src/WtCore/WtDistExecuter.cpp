#include "WtDistExecuter.h"

USING_NS_OTP;

void WtDistExecuter::set_position(const std::unordered_map<std::string, double>& targets)
{
	
}

void WtDistExecuter::on_position_changed(const char* stdCode, double targetPos)
{

}

void WtDistExecuter::on_tick(const char* stdCode, WTSTickData* newTick)
{
	//分布式执行器不需要处理ontick
}
