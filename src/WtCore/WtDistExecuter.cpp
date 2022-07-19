#include "WtDistExecuter.h"

#include "../Includes/WTSVariant.hpp"

#include "../Share/decimal.h"
#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;

WtDistExecuter::WtDistExecuter(const char* name)
	: IExecCommand(name)
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

void WtDistExecuter::set_position(const faster_hashmap<LongKey, double>& targets)
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
			WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "[{}]{}Ŀ���λ����: {} -> {}", _name.c_str(), stdCode, oldVol, newVol);
		}

		//����㲥Ŀ���λ
	}
}

void WtDistExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	targetPos *= _scale;

	double oldVol = _target_pos[stdCode];
	_target_pos[stdCode] = targetPos;

	if (!decimal::eq(oldVol, targetPos))
	{
		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "[{}]{}Ŀ���λ����: {} -> {}", _name.c_str(), stdCode, oldVol, targetPos);
	}

	//����㲥Ŀ���λ
}

void WtDistExecuter::on_tick(const char* stdCode, WTSTickData* newTick)
{
	//�ֲ�ʽִ��������Ҫ����ontick
}
