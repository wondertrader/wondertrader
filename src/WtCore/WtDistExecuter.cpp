#include "WtDistExecuter.h"

#include "../Includes/WTSVariant.hpp"

#include "../Share/decimal.h"
#include "../WTSTools/WTSLogger.h"

USING_NS_OTP;

WtDistExecuter::WtDistExecuter(const char* name)
	: _name(name)
{

}

WtDistExecuter::~WtDistExecuter()
{

}

bool WtDistExecuter::init(WTSVariant* params)
{
	if (params == NULL)
		return false;

	_config = params;
	_config->retain();

	_scale = params->getUInt32("scale");

	return true;
}

void WtDistExecuter::writeLog(const char* fmt, ...)
{
	char szBuf[2048] = { 0 };
	uint32_t length = sprintf(szBuf, "[%s]", _name.c_str());
	strcat(szBuf, fmt);
	va_list args;
	va_start(args, fmt);
	WTSLogger::vlog_dyn("executer", _name.c_str(), LL_INFO, szBuf, args);
	va_end(args);
}

void WtDistExecuter::set_position(const std::unordered_map<std::string, double>& targets)
{
	for (auto it = targets.begin(); it != targets.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		double newVol = it->second;

		newVol *= _scale;
		double oldVol = _target_pos[stdCode];
		_target_pos[stdCode] = newVol;
		if (!decimal::eq(oldVol, newVol))
		{
			writeLog(fmt::format("{}目标仓位更新: {} -> {}", stdCode, oldVol, newVol).c_str());
		}

		//这里广播目标仓位
	}
}

void WtDistExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	targetPos *= _scale;

	double oldVol = _target_pos[stdCode];
	_target_pos[stdCode] = targetPos;

	if (!decimal::eq(oldVol, targetPos))
	{
		writeLog(fmt::format("{}目标仓位更新: {} -> {}", stdCode, oldVol, targetPos).c_str());
	}

	//这里广播目标仓位
}

void WtDistExecuter::on_tick(const char* stdCode, WTSTickData* newTick)
{
	//分布式执行器不需要处理ontick
}
