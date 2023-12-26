/*!
 * \file WTSError.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt错误对象定义
 */
#pragma once
#include "WTSObject.hpp"
#include "WTSTypes.h"

NS_WTP_BEGIN

class WTSError : public WTSObject
{
protected:
	WTSError():m_errCode(WEC_NONE),m_strMsg(""){}
	virtual ~WTSError(){}

public:
	static WTSError* create(WTSErroCode ec, const char* errmsg) noexcept
	{
		WTSError* pRet = new WTSError;
		pRet->m_errCode = ec;
		wt_strcpy(pRet->m_strMsg, errmsg);

		return pRet;
	}

	constexpr const char*		getMessage() const noexcept{return m_strMsg;}
	constexpr WTSErroCode		getErrorCode() const noexcept{return m_errCode;}

protected:
	WTSErroCode		m_errCode;
	char			m_strMsg[128];
};


NS_WTP_END