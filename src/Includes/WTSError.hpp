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
	static WTSError* create(WTSErroCode ec, const char* errmsg)
	{
		WTSError* pRet = new WTSError;
		pRet->m_errCode = ec;
		pRet->m_strMsg = errmsg;

		return pRet;
	}

	const char*		getMessage() const{return m_strMsg.c_str();}
	WTSErroCode		getErrorCode() const{return m_errCode;}

protected:
	WTSErroCode		m_errCode;
	std::string		m_strMsg;
};


NS_WTP_END