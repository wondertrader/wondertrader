/*!
 * \file ISessionMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 交易时间模板管理器接口定义
 */
#pragma once

#include "WTSMarcos.h"

NS_WTP_BEGIN
class WTSSessionInfo;

/*
 *	时间模板管理器接口
 */
class ISessionMgr
{
public:
	/*
	 *	获取合约所属的时间模板对象指针
	 *	@code	合约代码
	 *	@exchg	交易所代码
	 *
	 *	返回值	时间模板指针,不存在则为NULL
	 */
	virtual WTSSessionInfo* getSession(const char* code, const char* exchg = "")	= 0;
};
NS_WTP_END