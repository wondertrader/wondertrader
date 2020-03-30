#pragma once
#include "WTSMarcos.h"
#include "WTSTypes.h"

NS_OTP_BEGIN
class ILogHandler
{
public:
	virtual void handleLogAppend(WTSLogLevel ll, const char* msg)	= 0;
};
NS_OTP_END