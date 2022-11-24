/*!
 * \file TraderATP.h
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

#define TRADE_API_USE_STATIC
#include "../API/AtpTraderApiXYZQ3.1.10.6/include/atp_trade_api.h"

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"

USING_NS_WTP;

class TraderATP : public ATPTradeHandler, public ITraderApi
{
public:
	TraderATP();
	virtual ~TraderATP();

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
	// ����ص�
	virtual void OnLogin(const std::string& reason) override;

	// �ǳ��ص�
	virtual void OnLogout(const std::string& reason) override;

	// ����ʧ��
	virtual void OnConnectFailure(const std::string &reason) override;

	// ���ӳ�ʱ
	virtual void OnConnectTimeOut(const std::string &reason) override;

	// ������ʱ
	virtual void OnHeartbeatTimeout(const std::string &reason) override;

	// ���ӹر�
	virtual void OnClosed(const std::string &reason) override;

	// ���ӽ����ص�
	virtual void OnEndOfConnection(const std::string& reason) override;

	// �ͻ��ŵ���ص�
	virtual void OnRspCustLoginResp(const ATPRspCustLoginRespOtherMsg &cust_login_resp) override;

	// �ͻ��ŵǳ��ص�
	virtual void OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg &cust_logout_resp) override;

	// �����´��ڲ���Ӧ
	virtual void OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg& order_status_ack) override;

	// �����´ｻ����ȷ��
	virtual void OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg& order_status_ack) override;

	// �ɽ��ر�
	virtual void OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg& cash_auction_trade_er) override;

	// �����´��ڲ��ܾ�
	virtual void OnRspBizRejection(const ATPRspBizRejectionOtherMsg& biz_rejection) override;

	/**
	 * @brief �ʽ��ѯ���
	 * @param[in] ATPRspFundQueryResultMsg
	 * @type query
	 */
	virtual void OnRspFundQueryResult(const ATPRspFundQueryResultMsg &msg) override;

	/**
	 * @brief �ɷݲ�ѯ���
	 * @param[in] ATPRspShareQueryResultMsg
	 * @type query
	 */
	virtual void OnRspShareQueryResult(const ATPRspShareQueryResultMsg &msg) override;

	/**
	 * @brief �ɽ���ѯ���
	 * @param[in] ATPRspTradeOrderQueryResultMsg
	 * @type query
	 */
	virtual void OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg &msg) override;

	/**
	 * @brief ������ѯ���
	 * @param[in] ATPRspOrderQueryResultMsg
	 * @type query
	 */
	virtual void OnRspOrderQueryResult(const ATPRspOrderQueryResultMsg &msg) override;

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
	inline uint32_t	genRequestID();
	void		doLogin(const char* productid);

	inline WTSOrderInfo*	makeOrderInfo(const APIOrderUnit* orderField);
	inline WTSEntrust*		makeEntrust(const ATPRspOrderStatusAckMsg *entrustField);
	inline WTSTradeInfo*	makeTradeInfo(const APITradeOrderUnit *tradeField);
	inline WTSOrderInfo*	makeOrderInfo(const ATPRspOrderStatusAckMsg *order_status_ack);
	inline WTSTradeInfo*	makeTradeRecord(const ATPRspCashAuctionTradeERMsg *cash_auction_trade_er);

	inline bool	extractEntrustID(const char* entrustid, uint32_t &orderRef);
	inline void	genEntrustID(char* buffer, uint32_t orderRef);

private:
	ATPTradeAPI*	_api;
	ITraderSpi*		_sink;
	bool			_close_flag;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*			_positions;
	WTSArray*				_trades;
	WTSArray*				_orders;

	IBaseDataMgr*			_bd_mgr;

	std::string		_user;  // AGW��¼�˺�
	std::string		_pass;  // AGW��¼����
	std::string		_node_id;  // �ڵ���Ϣ
	int				_loginmode;  // �˻��ĵ�¼ģʽ

	std::string		_sh_acctid;  // �Ϻ�֤ȯ�˻�
	std::string		_sz_acctid;  // ����֤ȯ�˻�
	std::string		_fund_accountid;  // �ʽ��˻�
	std::string		_accpasswd;  // ��������
	std::string		_branchid;   //Ӫҵ��ID ������¼ģʽΪ2-�ʽ��˻���¼ģʽʱ�����ֶ��Ƿ��������ѯȯ�̣�  

	std::string		_front;  // ��������
	std::string		_front2;

	TraderState		_state;

	uint64_t		_sessionid;
	int64_t		_sh_total_num;  // �ܵĶ�����
	int64_t		_sz_total_num;  // �����ܵĶ�����
	
	uint32_t		_tradingday;
	std::atomic<uint32_t>		_reqid;
	std::atomic<uint32_t>		_ordref;		//��������

	std::string		_cust_id;  // �ͻ���

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	DllHandle		m_hInstATP;

	//ί�е���ǻ�����
	WtKVCache		m_eidCache;
	//������ǻ�����
	WtKVCache		m_oidCache;

	std::map<int32_t, int32_t> report_sync;

	WTSArray* ayOrders;  // ������ѯ���
	WTSArray* ayTrades;  // �ɽ���ѯ���
};

