/*!
 * \file TraderMAOpt.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#pragma once

#include <string>
#include <queue>
#include <stdint.h>

#include <boost/asio/io_service.hpp>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../API/mCliApi3.7/maCliOptTradeApi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"


 // �û���ɫ
typedef enum
{
	MA_CLIENT = '1',
	MA_TELLER = '2'
} MA_USER_ROLE;

// �����г�
typedef enum
{
	MA_SZSE = '0',
	MA_SSE
} MA_STK_EX;

// ���װ��
typedef enum
{
	MA_SZSE_A = '00',
	MA_SZSE_B = '01',
	MA_THREE_BOARD = '02',
	MA_SZSE_OPT = '05',
	MA_SSE_A = '10',
	MA_SSE_B = '11',
	MA_SSE_OPT = '15'
} MA_STK_BD;

// ֤ȯ���
typedef enum
{
	MA_STOCK = 'A',
	MA_ETF = 'D',
	MA_STK_OPT = 'U'
} MA_STK_CLS;

// ���Ҵ���
typedef enum
{
	MA_RMB = '0',
	MA_HK = '1',
	MA_US = '2'
} MA_CURRENCY;

// ֤ȯҵ��
typedef enum
{
	MA_BuyOpen = 400,  // ��
	MA_SellClose,	   // ��ƽ
	MA_SellOpen,	  // ����
	MA_BuyClose,       // ��ƽ
	MA_OPT_CALL = 406,  // �Ϲ���Ȩ
	MA_OPT_PUT        // �Ϲ���Ȩ
} MA_STK_BIZ;

// ֤ȯҵ��
typedef enum
{
	MA_OPT_LIMITPRICE_GFD = 130,  // ��Ʊ��Ȩ�걨-�޼�GFD
	MA_OPT_LIMITPRICE_FOK = 131,  // ��Ʊ��Ȩ�걨-�޼�FOK
	MA_OPT_ANYPRICE_FOK = 133,  // ��Ʊ��Ȩ�걨-�м�FOK
	MA_OPT_ANYPRICE_IOC = 134,  // ��Ʊ��Ȩ�걨-�м�IOC
	MA_OPT_ANYPRICE_LEFT_LIMIT = 132,  // ��Ʊ��Ȩ�걨-�м�ʣת�޼�GFD
	MA_ORDER = 100,  // ί���걨
	MA_CANCEL_ORDER = 101,  // �����걨
	MA_BESTFIVELEVELPRICE = 121,  // �м������嵵��ʱ�ɽ�ʣ�೷��
	MA_MARKEPRICE_ALLORDRAW = 122,  // �м�ȫ��ɽ�����
	MA_BESTPRICE_THIS_SIDE = 123,  // �м۱������ż۸�
	MA_BESTPRICE_OTHER_SIDE = 124,  // �м۶��ַ����ż۸�
	MA_MARKETPRICE_LEFT_CANCEL = 125  // �мۼ�ʱ�ɽ�ʣ�೷��
} MA_STK_BIZ_ACTION;

typedef enum
{
	MA_GFD = 600,  // ������Ч
	MA_FOK,  // ȫ�ɻ���ȫ��
	MA_IOC  // �����ɽ�������ȡ��
} MATimeConditionType;

// ������־
typedef enum
{
	MA_ORDER_CANCEL = 'T',  // ����
	MA_ORDER_NORMAL = 'F'  // ����
} MA_IS_WITHDRAW;

// ί����Ч��־
typedef enum
{
	MA_ORDER_VALID = '0',  // ��Ч
	MA_ORDER_INVALID = '1'  // ��Ч
} MA_ORDER_VALID_FLAG;

// ί��״̬
typedef enum tagOrderState
{
	MA_Nottouched = '0',  // δ��, д��ί�е�����׶Σ��ñ�ҵ��û�е��걨ʱ��
	MA_Submitting, // �������ñ�־��©��ɨ����м�״̬����ʾ�����ж���
	MA_Submitted, // �ѱ����Ѿ��ɹ�д��ӿڿ�
	MA_Withdraw_NotFilled, // �ѱ��������ñ�ҵ�񱻳�����������δ�ɽ�
	MA_PartFilled_ToWithdraw, // ���ɴ������ñ�ҵ�񲿷ֳɽ���ʣ�ಿ�ֵȴ�����
	MA_PartWithdraw, // ���������ֳ���
	MA_Withdawn, // �ѳ��������Ѿ��ɽ�
	MA_PartFilled_Withdrawn, // �����ѳ������ֳɽ�
	MA_AllFilled, // �ѳɣ��Ѿ��ɽ�
	MA_Canceled, // �ϵ������������Զ�����
	MA_ToSubmit = 'A', // ������д�뱨�̶���δ�ɹ�
	MA_OFFER_CONFIRM = 'B'  // ���̻�ȷ��
}  MAOrderState;

// �ֲַ���
typedef enum
{
	MA_LONG = 'L',   // Ȩ����
	MA_SHORT = 'S',  // �����
	MA_COVERED_CALL = 'C'  // ���Ҳ��Գֲ�
} MA_OPT_SIDE;

// ��Լ����
typedef enum
{
	MA_OPT_TYPE_CALL = 'C',  // �Ϲ�
	MA_OPT_TYPE_PUT = 'P'  // �Ϲ�
} MA_OPT_TYPE;

// ���㵥����
typedef enum
{
	MA_DAY_SETTLEMENT = 'D',  // �ս��㵥
	MA_MONTH_SETTLEMENT = 'M',  // �½��㵥
	MA_TEMP_SETTLEMENT = 'T'  // ��ʱ���㵥
} MA_SETT_LIST_TYPE;

// ����״̬
typedef enum
{
	MA_CANCEL_NORMAL = '0',  // ����
	MA_CANCEL_LOCKED,  // ��������
	MA_CANCELED,  // �ѱ�����
	MA_FZN_CANCEL  // �Ѷ���ȡ��
} MA_CANCEL_STATUS;


USE_NAMESPACE_MACLI

USING_NS_WTP;

class TraderMAOpt : public ITraderApi, public IOptTraderApi, public CCliOptTradeSpi
{
public:
	TraderMAOpt();
	virtual ~TraderMAOpt();

public:
	typedef enum
	{
		WS_NOTLOGIN,		//δ��¼
		WS_LOGINING,		//���ڵ�¼
		WS_LOGINED,			//�ѵ�¼
		WS_LOGINFAILED,		//��¼ʧ��
		WS_CONFIRM_QRYED,
		WS_CONFIRMED,		//��ȷ��
		WS_ALLREADY			//ȫ������
	} WrapperState;

private:
	int doLogin();

	//////////////////////////////////////////////////////////////////////////
	//ITraderApi�ӿ�
public:
	virtual bool init(WTSVariant* params) override;

	virtual void release() override;

	virtual void registerSpi(ITraderSpi *listener) override;

	virtual bool makeEntrustID(char* buffer, int length) override;

	virtual void connect() override;

	virtual void disconnect() override;

	virtual bool isConnected() override;

	virtual int login(const char* user, const char* pass, const char* productInfo) override;

	virtual int logout() override;

	virtual int orderInsert(WTSEntrust* entrust) override;

	virtual int orderAction(WTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;

	virtual int querySettlement(uint32_t uDate) override;


	//////////////////////////////////////////////////////////////////////////
	//IOptTraderApi
	virtual int orderInsertOpt(WTSEntrust* entrust) override;

	virtual int orderActionOpt(WTSEntrustAction* action) override;

	virtual int	queryOrdersOpt(WTSBusinessType bType) override;


	//////////////////////////////////////////////////////////////////////////
	//maCliOpt���׽ӿ�ʵ��
public:
	// �ͻ�����������ɹ�����ͨ�����Ӻ󣬸÷���������
	virtual int OnConnected(void) override;

	// �ͻ�����������ɹ���ͨ�����ӶϿ�ʱ���÷���������
	virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) override;

	// ������Ӧ
	virtual int OnRspHeartBeat(CFirstSetField *p_pFirstSet, CRspHeartBeatField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// �û���¼������Ӧ
	virtual int OnRspUserLogin(CFirstSetField *p_pFirstSet, CRspOptUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// �����ʽ��ѯ��Ӧ
	virtual int OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, CRspOptExpendableFundField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ���ú�Լ�ʲ���ѯ��Ӧ
	virtual int OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, CRspOptExpendableCuField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ������Ȩ��Լ�˻���ѯ��Ӧ
	virtual int OnRspQryAcct(CFirstSetField* p_pFirstSetField, CRspOptAcctField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ������Ȩί���걨��Ӧ
	virtual int OnRspOrder(CFirstSetField *p_pFirstSet, CRspOptOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ������Ȩί�г�����Ӧ
	virtual int OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ȷ�ϻر�
	virtual int OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRspField) override;

	// ��Ȩ������Ϣ��ѯ��Ӧ
	virtual int OnRspQryBaseInfo(CFirstSetField *p_pFirstSet, CRspOptBaseInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex);

	// �ɽ��ر�--ί����Ϣ
	virtual int OnRtnOrder(CRtnOptOrderField* p_pRspField) override;

	// �ɽ��ر�--�ɽ���Ϣ
	virtual int OnRtnOrderFill(CRtnOptOrderFillField* p_pRtnField) override;

	// �ɽ��ر�--��Լ��Ϣ
	virtual int OnRtnContract(CRtnOptContractField* p_pRspField) override;

	//�ͻ����㵥��ѯ 2015/8/6
	virtual int OnRspQrySettList(CFirstSetField* p_pFirstSet, CRspOptSettListField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ������Ȩ����ί�в�ѯ��Ӧ
	virtual int OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, CRspOptCurrDayOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ������Ȩ���ճɽ���ѯ��Ӧ
	virtual int OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, CRspOptCurrDayFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	//������ϳֲ���ϸ��ѯ��Ӧ 2015/8/5
	virtual int OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSetField, CRspOptCombStraPosDetailField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ���������Ȩ���ɽ���������Ӧ
	virtual int OnRspMaxTradeQty(CFirstSetField *p_pFirstSet, CRspOptMaxTradeQtyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	/////////////////// �Զ���ӿ�
public:
	// �ʽ��˻���ѯ
	int RspQryTradingAccount(void);

protected:
	/*
	*	��������Ϣ
	*/
	bool IsErrorRspInfo(CFirstSetField *pRspInfo);


	WTSOrderInfo*	makeOrderInfo(CRspOptOrderField* orderField);
	WTSOrderInfo*	makeOrderInfo(CRtnOptOrderField* orderField);
	WTSOrderInfo*   makeOrderInfo(CRspOptCurrDayOrderField* daiOrderField);
	//WTSOrderInfo*   makeOrderInfo(CRspOptCancelOrderField* cancelOrderField);
	WTSEntrust*		makeEntrust(CRspOptOrderField  *entrustField);
	WTSError*		makeError(CFirstSetField* rspInfo);
	WTSTradeInfo*	makeTradeRecord(CRtnOptOrderFillField *tradeField);
	WTSTradeInfo*   makeTradeRecord(CRspOptCurrDayFillField *dayTradeField);
	WTSEntrust*		makeEntrust(CRtnOptOrderFillField  *entrustField);

	void			generateEntrustID(char* buffer, long long ll_cust_code, long long ll_cuacct_code, long order_bsn);
	bool			extractEntrustID(const char* entrustid, long long &ll_cust_code, long long &ll_cuacct_code, long &order_bsn);

	uint32_t		genRequestID();

protected:
	int64_t			m_llCustCode;  // �ͻ�����
	int64_t			m_llCuacctCode;  // �ʽ𻧴���
	int				m_iInitOrg;  // ��������
	std::string		m_strChannel;  // ��������

	std::string		m_strHost;  // IP��ַ
	int				m_iPort;  // �˿�

	std::string		m_strUser;
	std::string		m_strPass;
	std::string		m_strTrdAcct;  // �����˻�

	std::string		m_strAuthData;
	std::string		m_strEncryptKey;

	std::string		m_strUseScope;
	std::string		m_strAuthType;
	std::string		m_strEncryptType;
	std::string		m_strAcctType;

	std::string		m_strStkBD;  // ���װ��
	std::string		m_strStkPBU;  // ���׵�Ԫ

	std::string		m_strOptNum;  // ��Լ����
	std::string		m_strOrderId;  // ��ͬ���

	ITraderSpi*		m_bscSink;
	IOptTraderSpi*	m_optSink;

	uint32_t					m_lDate;
	std::atomic<uint32_t>		m_orderRef;		//��������

	WrapperState				m_wrapperState;

	CCliOptTradeApi*			m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*				m_mapPosition;
	WTSArray*					m_ayTrades;
	WTSArray*					m_ayOrders;

	IBaseDataMgr*				m_bdMgr;

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	std::string		m_strModule;
	DllHandle		m_hInstMA;

	WTSAccountInfo*         m_accountInfo;

	//ί�е���ǻ�����
	WtKVCache		m_eidCache;
	//������ǻ�����
	WtKVCache		m_oidCache;

	// �޶���ѯ��Χ
	bool b_inQryAcct;
};
