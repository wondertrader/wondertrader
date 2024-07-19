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

class WTSError : public WTSPoolObject<WTSError>
{
public:
	WTSError() :m_errCode(WEC_NONE), m_strMsg("") {}
	virtual ~WTSError(){}

public:
	static WTSError* create(WTSErroCode ec, const char* errmsg) noexcept
	{
		WTSError* pRet = WTSError::allocate();
		pRet->m_errCode = ec;
		strncpy(pRet->m_strMsg, errmsg, 255);

		return pRet;
	}

	constexpr const char*		getMessage() const noexcept{return m_strMsg;}
	constexpr WTSErroCode		getErrorCode() const noexcept{return m_errCode;}

protected:
	WTSErroCode		m_errCode = WEC_NONE;
	char			m_strMsg[256] = { 0 };
};


NS_WTP_END