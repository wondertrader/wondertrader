/*!
 * \file ParserUT.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <map>
#include <string>

#if _WIN32
#ifndef WINAPI
#define WINAPI __stdcall
#endif
#ifndef BOOL
typedef int BOOL;
#endif
#endif

#include "../Share/IParserApi.h"
#include "./UTApi/XYZQ_UT_API.h"


NS_OTP_BEGIN
class WTSTickData;
NS_OTP_END

USING_NS_OTP;

class ParserUT : public IParserApi, public xyzq_ut_spi
{
public:
	ParserUT();
	virtual ~ParserUT();

public:
	enum LoginStatus
	{
		LS_NOTLOGIN,
		LS_LOGINING,
		LS_LOGINED
	};

//IQuoteParser 接口
public:
	virtual bool init(WTSParams* config) override;

	virtual void release() override;

	virtual bool connect() override;

	virtual bool disconnect() override;

	virtual bool isConnected() override;

	virtual void subscribe(const CodeSet &vecSymbols) override;
	virtual void unsubscribe(const CodeSet &vecSymbols) override;

	virtual void registerListener(IParserApiListener* listener) override;

//////////////////////////////////////////////////////////////////////////
//xyzq_ut_spi 接口
public:
	//错误信息响应
	virtual void rsp_error(error_msg *arg) override;
	//状态响应
	virtual void rsp_status_changed(int status) override;
	//单个品种行情推送
	virtual void rsp_market_data(ut_data *arg) override;
	//多个品种行情推送
	virtual void rsp_market_data_ex(ut_data_list *arg) override;
	//单个品种逐笔成交推送
	virtual void rsp_transactions(ut_data *arg) override;
	//单个品种逐笔委托推送
	virtual void rsp_orders(ut_data *arg) override;
	//单个品种委托队列推送
	virtual void rsp_order_list(ut_order_queue_list *arg) override;
	//查询基础信息响应
	virtual void rsp_base_info(rsp_base_info_s *arg) override;

private:
	/*
	 *	发送登录请求
	 */
	void DoLogin();
	/*
	 *	订阅品种行情
	 */
	void SubscribeMDs(bool isAll = false);
	/*
	 *	检查错误信息
	 */
	bool IsErrorRspInfo(error_msg *pRspInfo);

	WTSTickData*	makeTickData(ut_data* pMktData);

private:
	LoginStatus			m_loginState;
	xyzq_ut_api*		m_pUserAPI;

	std::string			m_strFrontAddr;
	std::string			m_strUserNo;
	std::string			m_strUserID;
	std::string			m_strPassword;

	bool				m_bHasOrder;
	bool				m_bHasOrdQue;
	bool				m_bHasTrans;
	bool				m_bLogined;

	CodeSet				m_filterSubs;
	CodeSet				m_ignoreCodes;

	IParserApiListener*	m_parserSink;
	IBaseDataMgr*		m_pBaseDataMgr;
};

