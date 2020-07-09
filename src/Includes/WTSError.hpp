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

NS_OTP_BEGIN

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

class WTSNotify : public WTSObject
{
protected:
	WTSNotify(){}
	virtual ~WTSNotify(){}

public:
	static WTSNotify* create(const char* content, const char* title = "", uint64_t utime=0, bool isNotice = false)
	{
		WTSNotify* pRet = new WTSNotify();
		pRet->_content = content;
		pRet->_title = title;
		if(utime == 0)
			pRet->_time = ::time(NULL);
		else
			pRet->_time = utime;
		pRet->_isnotice = isNotice;

		return pRet;
	}

	const char* content() const{return _content.c_str();}
	const char* title() const{return _title.c_str();}
	uint64_t	time() const{return _time;}
	bool		isNotice() const{return _isnotice;}

private:
	std::string _content;
	std::string _title;
	uint64_t	_time;
	bool		_isnotice;
};

NS_OTP_END