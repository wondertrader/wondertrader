/*!
 * \file TraderXTP.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include <boost/asio/io_service.hpp>

#include "../API/XTP2.2.32.2/xtp_trader_api.h"

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

USING_NS_WTP;

class TraderXTP : public XTP::API::TraderSpi, public ITraderApi
{
public:
	TraderXTP();
	virtual ~TraderXTP();

	typedef enum
	{
		TS_NOTLOGIN,		//δ��¼
		TS_LOGINING,		//���ڵ�¼
		TS_LOGINED,			//�ѵ�¼
		TS_LOGINFAILED,		//��¼ʧ��
		TS_ALLREADY			//ȫ������
	} TraderState;

public:
	//////////////////////////////////////////////////////////////////////////
	//XTP::API::TraderSpi �ӿ�
	virtual void OnDisconnected(uint64_t session_id, int reason) override;

	virtual void OnError(XTPRI *error_info) override;

	virtual void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) override;

	virtual void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) override;

	virtual void OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) override;

	virtual void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	virtual void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	virtual void OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	virtual void OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderApi �ӿ�
	virtual bool init(WTSVariant *params) override;

	virtual void release() override;

	virtual void registerSpi(ITraderSpi *listener) override;

	virtual void connect() override;

	virtual void disconnect() override;

	virtual bool isConnected() override;

	virtual bool makeEntrustID(char* buffer, int length) override;

	virtual int login(const char* user, const char* pass, const char* productInfo) override;

	virtual int logout() override;

	virtual int orderInsert(WTSEntrust* eutrust) override;

	virtual int orderAction(WTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;

private:
	void		reconnect();
	inline uint32_t			genRequestID();
	void					doLogin();

	inline WTSOrderInfo*	makeOrderInfo(XTPQueryOrderRsp* orderField);
	inline WTSEntrust*		makeEntrust(XTPOrderInfo *entrustField);
	inline WTSTradeInfo*	makeTradeInfo(XTPQueryTradeRsp *tradeField);

	bool					extractEntrustID(const char* entrustid, uint32_t &orderRef);
	inline std::string		genEntrustID(uint32_t orderRef);

private:
	XTP::API::TraderApi*	_api;
	ITraderSpi*		_sink;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*			_positions;
	WTSArray*				_trades;
	WTSArray*				_orders;

	IBaseDataMgr*			_bd_mgr;

	std::string		_user;
	std::string		_pass;
	std::string		_flowdir;

	std::string		_acckey;

	std::string		_host;
	int				_port;
	int				_client;

	bool			_quick;
	bool			_inited;

	TraderState		_state;

	uint64_t		_sessionid;
	uint32_t		_tradingday;
	std::atomic<uint32_t>		_reqid;
	std::atomic<uint32_t>		_ordref;		//��������

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	DllHandle		m_hInstXTP;
	typedef XTP::API::TraderApi* (*XTPCreator)(uint8_t, const char*, XTP_LOG_LEVEL);
	XTPCreator		m_funcCreator;

	IniHelper		_ini;
};

