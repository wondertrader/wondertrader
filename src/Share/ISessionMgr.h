#pragma once

#include "WTSMarcos.h"

NS_OTP_BEGIN
class WTSSessionInfo;

class ISessionMgr
{
public:
	virtual WTSSessionInfo* getSession(const char* code, const char* exchg = "")	= 0;
};
NS_OTP_END