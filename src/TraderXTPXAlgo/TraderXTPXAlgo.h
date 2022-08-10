#pragma once

#include <stdint.h>
#include <boost/asio/io_service.hpp>

#include "../API/XTPXAlgo/include/xtp_trader_api.h"

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"


USING_NS_WTP;

class TraderXTPXAlgo : public XTP::API::TraderSpi, public ITraderApi
{
public:
	TraderXTPXAlgo();
	virtual ~TraderXTPXAlgo();

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
	///���ͻ��˵�ĳ�������뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á�
	///@param reason ����ԭ��������������Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �û���������logout���µĶ��ߣ����ᴥ���˺�����api�����Զ������������߷���ʱ�����û�����ѡ����������������ڴ˺����е���Login���µ�¼��������session_id����ʱ�û��յ������ݸ�����֮ǰ��������
	virtual void OnDisconnected(uint64_t session_id, int reason) override;

	///����Ӧ��
	///@param error_info ����������Ӧ��������ʱ�ľ���Ĵ������ʹ�����Ϣ,��error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark �˺���ֻ���ڷ�������������ʱ�Ż���ã�һ�������û�����
	virtual void OnError(XTPRI *error_info) override;

	///���ͻ�����AlgoBusͨ�����ӶϿ�ʱ���÷��������á�
	///@param reason ����ԭ��������������Ӧ
	///@remark �벻Ҫ�������̣߳������Ӱ��algo�ĵ�¼����Algo֮������ӣ����ߺ���Զ��������û���������������
	virtual void OnAlgoDisconnected(int reason) override;

	///���ͻ�����AlgoBus���ߺ���������ʱ���÷��������ã����ڶ��������ɹ���ᱻ���á�
	virtual void OnAlgoConnected() override;

	///algoҵ���в�ѯ�����б����Ӧ
	///@param strategy_info ���Ծ�����Ϣ
	///@param strategy_param �˲����а����Ĳ��������error_info.error_idΪ0ʱ��������
	///@param error_info ��ѯ��ѯ�����б�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryStrategy(XTPStrategyInfoStruct* strategy_info, char* strategy_param, XTPRI *error_info, int32_t request_id, bool is_last, uint64_t session_id) override;

	///algoҵ���в�������ʱ����״̬֪ͨ
	///@param strategy_state �û��������������״̬֪ͨ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnStrategyStateReport(XTPStrategyStateReportStruct* strategy_state, uint64_t session_id) override;

	///algoҵ�����û������㷨ͨ������Ϣ��Ӧ
	///@param user �û���
	///@param error_info �����㷨ͨ����������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д��󣬼��㷨ͨ���ɹ�
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �㷨ͨ�������ɹ��󣬲��ܶ��û��������ԵȲ�����һ���û�ֻ��ӵ��һ���㷨ͨ�������֮ǰ�Ѿ��������������ظ�����
	virtual void OnALGOUserEstablishChannel(char* user, XTPRI* error_info, uint64_t session_id) override;

	///algoҵ���б��Ͳ��Ե�����Ӧ
	///@param strategy_info �û����͵Ĳ��Ե��ľ�����Ϣ
	///@param error_info ���Ͳ��Ե���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnInsertAlgoOrder(XTPStrategyInfoStruct* strategy_info, XTPRI *error_info, uint64_t session_id) override;

	///algoҵ���г������Ե�����Ӧ
	///@param strategy_info �û������Ĳ��Ե��ľ�����Ϣ
	///@param error_info �������Ե���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnCancelAlgoOrder(XTPStrategyInfoStruct* strategy_info, XTPRI *error_info, uint64_t session_id) override;

	///����֪ͨ
	///@param order_info ������Ӧ������Ϣ���û�����ͨ��order_info.order_xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ�����order_info.qty_left�ֶ��ڶ���Ϊδ�ɽ������ɡ�ȫ�ɡ��ϵ�״̬ʱ����ʾ�˶�����û�гɽ����������ڲ�����ȫ��״̬ʱ����ʾ�˶���������������order_info.order_cancel_xtp_idΪ������Ӧ�ĳ���ID����Ϊ0ʱ��ʾ�˵������ɹ�
	///@param error_info �������ܾ����߷�������ʱ�������ʹ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ÿ�ζ���״̬����ʱ�����ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߣ��ڶ���δ�ɽ���ȫ���ɽ���ȫ�����������ֳ������Ѿܾ���Щ״̬ʱ������Ӧ�����ڲ��ֳɽ�����������ɶ����ĳɽ��ر�������ȷ�ϡ����е�¼�˴��û��Ŀͻ��˶����յ����û��Ķ�����Ӧ
	virtual void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) override;

	///�ɽ�֪ͨ
	///@param trade_info �ɽ��ر��ľ�����Ϣ���û�����ͨ��trade_info.order_xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ����������Ͻ�����exec_id����Ψһ��ʶһ�ʳɽ���������2�ʳɽ��ر�ӵ����ͬ��exec_id���������Ϊ�˱ʽ����Գɽ��ˡ����������exec_id��Ψһ�ģ���ʱ�޴��жϻ��ơ�report_index+market�ֶο������Ψһ��ʶ��ʾ�ɽ��ر���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �����гɽ�������ʱ�򣬻ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ����е�¼�˴��û��Ŀͻ��˶����յ����û��ĳɽ��ر�����ض���Ϊ����״̬����Ҫ�û�ͨ���ɽ��ر��ĳɽ�������ȷ����OnOrderEvent()�������Ͳ���״̬��
	virtual void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) override;

	///�����ѯ������Ӧ-�ɰ汾�ӿ�
	///@param order_info ��ѯ����һ������
	///@param error_info ��ѯ����ʱ��������ʱ�����صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ��˶�Ӧ����������������ѯʹ�ã�������������ʱ����������û���·ӵ�£�����api����
	//virtual void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	///�����ѯ�ɽ���Ӧ
	///@param trade_info ��ѯ����һ���ɽ��ر�
	///@param error_info ��ѯ�ɽ��ر���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ��˶�Ӧ����������������ѯʹ�ã�������������ʱ����������û���·ӵ�£�����api����
	virtual void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	///�����ѯͶ���ֲ߳���Ӧ
	///@param position ��ѯ����һֻ��Ʊ�ĳֲ����
	///@param error_info ��ѯ�˻��ֲַ�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �����û����ܳ��ж����Ʊ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	///�����ѯ�ʽ��˻���Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param asset ��ѯ�����ʽ��˻����
	///@param error_info ��ѯ�ʽ��˻���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
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
	inline WTSEntrust*		makeEntrust(XTPStrategyInfoStruct *stra_info);
	inline WTSTradeInfo*	makeTradeInfo(XTPQueryTradeRsp *tradeField);
	inline WTSOrderInfo*	makeOrderInfo(XTPStrategyInfoStruct *strategyState);  // �㷨����Ϣ

	inline bool	extractEntrustID(const char* entrustid, uint32_t &orderRef);
	inline void	genEntrustID(char* buffer, uint32_t orderRef);

private:
	XTP::API::TraderApi*	_api;
	ITraderSpi*		_sink;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*			_positions;
	WTSArray*				_trades;
	WTSArray*				_orders;

	IBaseDataMgr*			_bd_mgr;

	std::string		_user;  // ���׷������˺�
	std::string		_pass;  // ���׷���������
	std::string		_flowdir;  // ��־·��
	std::string		_flowdir_lnx;  // linux��־·��

	std::string		_acct_key;  // account_key

	std::string		_acct_algo;  // �㷨�������˻�
	std::string		_password_algo;  // �㷨����������

	std::string		_host_algo;  // Algo�㷨������IP
	int				_port_algo;
	int				_client;  // ͬһ�˻��࿪
	std::string		_host_oms;  // ������������IP
	int				_port_oms;

	// algo�㷨�������
	int				_algo_client;  // �㷨��Ӧ��client
	int				_algo_type;  // �㷨����
	std::string		_algo_params;  // ����
	int				_resume_type;

	std::string		_log_dir;  // ��־·��

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

	//ί�е���ǻ�����
	WtKVCache		m_eidCache;
	//������ǻ�����
	WtKVCache		m_oidCache;
};

