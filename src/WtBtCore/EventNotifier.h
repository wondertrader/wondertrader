/*!
 * \file EventCaster.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief UDP广播对象定义
 */
#pragma once

#include <queue>

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSObject.hpp"
#include "../Share/StdUtils.hpp"

typedef unsigned long(*FuncCreateMQServer)(const char*);
typedef void(*FuncDestroyMQServer)(unsigned long);
typedef void(*FundPublishMessage)(unsigned long, const char*, const char*, unsigned long);

typedef void(*FuncLogCallback)(unsigned long, const char*, bool);
typedef void(*FuncRegCallbacks)(FuncLogCallback);


NS_OTP_BEGIN
class WTSVariant;

class EventNotifier
{
public:
	EventNotifier();
	~EventNotifier();

public:
	bool	init(WTSVariant* cfg);
	void	notifyLog(const char* tag, const char* message);
	void	notifyEvent(const char* evtType);

private:
	std::string		m_strURL;
	uint32_t		_mq_sid;
	FuncCreateMQServer	_creator;
	FuncDestroyMQServer	_remover;
	FundPublishMessage	_publisher;
	FuncRegCallbacks	_register;
};

NS_OTP_END