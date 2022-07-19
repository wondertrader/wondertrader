/*!
 * \file ITraderApi.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief ����ͨ���Խӽӿڶ����ļ�
 */
#pragma once
#include <set>
#include <map>
#include <stdint.h>
#include <functional>
#include "WTSTypes.h"

NS_WTP_BEGIN
class WTSVariant;
class WTSEntrust;
class WTSOrderInfo;
class WTSTradeInfo;
class WTSEntrustAction;
class WTSAccountInfo;
class WTSPositionItem;
class WTSContractInfo;
class WTSError;
class WTSTickData;
class WTSNotify;
class WTSArray;
class IBaseDataMgr;

typedef std::function<void()>	CommonExecuter;

#pragma region "Stock Trading API definations"
/*
 *	��Ʊ���׽ӿڻص�
 *	Added By Wesley @ 2020/05/06
 */
class IStkTraderSpi
{

};

/*
 *	��Ʊ���׽ӿ�
 *	Added By Wesley @ 2020/05/06
 *	Ԥ�������Ժ�ʹ��,�Ȱѽӿڵ��໥��ܴ����
 *	��Ҫ�ṩ������ȯ�ȹ�Ʊ���нӿ�
 */
class IStkTraderApi
{
};
#pragma endregion

#pragma region "Option Trading API definations"
/*
 *	��Ȩ���׽ӿڻص�
 *	Added By Wesley @ 2020/05/06
 */
class IOptTraderSpi
{
public:
	virtual void onRspEntrustOpt(WTSEntrust* entrust, WTSError *err) {}
	virtual void onRspOrdersOpt(const WTSArray* ayOrders) {}
	virtual void onPushOrderOpt(WTSOrderInfo* orderInfo) {}
};

/*
 *	��Ȩ���׽ӿ�
 *	Added By Wesley @ 2020/05/06
 *	Ԥ�������Ժ�ʹ��,�Ȱѽӿڵ��໥��ܴ����
 *	��Ҫ�ṩ���ۡ���Ȩ����Ȩ���нӿ�
 */
class IOptTraderApi
{
public:
	/*
	 *	�µ��ӿ�
	 *	entrust �µ��ľ������ݽṹ
	 */
	virtual int orderInsertOpt(WTSEntrust* eutrust) { return -1; }

	/*
	 *	���������ӿ�
	 *	action	�����ľ������ݽṹ
	 */
	virtual int orderActionOpt(WTSEntrustAction* action) { return -1; }

	/*
	 *	��ѯ��Ȩ����
	 */
	virtual int	queryOrdersOpt(WTSBusinessType bType) { return -1; }
};
#pragma endregion


//ί�лص��ӿ�
class ITraderSpi
{
public:
	/*
	 *	��ȡ�������ݹ�����
	 */
	virtual IBaseDataMgr*	getBaseDataMgr() = 0;

	/*
	 *	�����׽ӿڵ���־
	 */
	virtual void handleTraderLog(WTSLogLevel ll, const char* message){}

	/*
	 *	��ȡ��Ʊ���׽ӿ�Spi
	 */
	virtual IStkTraderSpi* getStkSpi(){ return NULL; }

	/*
	 *	��ȡ��Ȩ���׽ӿ�Spi
	 */
	virtual IOptTraderSpi* getOptSpi(){ return NULL; }

public:
	/*
	 *	�����׽ӿ��¼�
	 */
	virtual void handleEvent(WTSTraderEvent e, int32_t ec) = 0;

	/*
	 *	��¼�ر�
	 */
	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate) = 0;

	/*
	 *	ע���ر�
	 */
	virtual void onLogout(){}

	/*
	 *	ί�лر�
	 */
	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err){}

	/*
	 * �ʽ��ѯ�ر�
	 */
	virtual void onRspAccount(WTSArray* ayAccounts) {}

	/*
	 *	�ֲֲ�ѯ�ر�
	 */
	virtual void onRspPosition(const WTSArray* ayPositions){}

	/*
	 *	������ѯ�ر�
	 */
	virtual void onRspOrders(const WTSArray* ayOrders){}

	/*
	 *	�ɽ���ѯ�ر�
	 */
	virtual void onRspTrades(const WTSArray* ayTrades){}

	/*
	 *	���㵥��ѯ�ر�
	 */
	virtual void onRspSettlementInfo(uint32_t uDate, const char* content){}

	/*
	 *	�����ر�����
	 */
	virtual void onPushOrder(WTSOrderInfo* orderInfo){}

	/*
	 *	�ɽ��ر�����
	 */
	virtual void onPushTrade(WTSTradeInfo* tradeRecord){}

	/*
	 *	���׽ӿڴ���ر�
	 */
	virtual void onTraderError(WTSError* err){}

	/*
	 *	��Լ״̬����
	 */
	virtual void onPushInstrumentStatus(const char* exchg, const char* code, WTSTradeStatus state) {}
};

//�µ��ӿڹ���ӿ�
class ITraderApi
{
public:
	virtual ~ITraderApi(){}

	virtual IStkTraderApi* getStkTrader() { return NULL; }
	virtual IOptTraderApi* getOptTrader() { return NULL; }

public:
	/*
	 *	��ʼ������������
	 */
	virtual bool init(WTSVariant *params) { return false; }

	/*
	 *	�ͷŽ���������
	 */
	virtual void release(){}

	/*
	 *	ע��ص��ӿ�
	 */
	virtual void registerSpi(ITraderSpi *listener) {}


	//////////////////////////////////////////////////////////////////////////
	//ҵ���߼��ӿ�

	/*
	 *	���ӷ�����
	 */
	virtual void connect() {}

	/*
	 *	�Ͽ�����
	 */
	virtual void disconnect() {}

	virtual bool isConnected() { return false; }

	/*
	 *	����ί�е���
	 */
	virtual bool makeEntrustID(char* buffer, int length){ return false; }

	/*
	 *	��¼�ӿ�
	 */
	virtual int login(const char* user, const char* pass, const char* productInfo) { return -1; }

	/*
	 *	ע���ӿ�
	 */
	virtual int logout() { return -1; }

	/*
	 *	�µ��ӿ�
	 *	entrust �µ��ľ������ݽṹ
	 */
	virtual int orderInsert(WTSEntrust* eutrust) { return -1; }

	/*
	 *	���������ӿ�
	 *	action	�����ľ������ݽṹ
	 */
	virtual int orderAction(WTSEntrustAction* action) { return -1; }

	/*
	 *	��ѯ�˻���Ϣ
	 */
	virtual int queryAccount() { return -1; }

	/*
	 *	��ѯ�ֲ���Ϣ
	 */
	virtual int queryPositions() { return -1; }

	/*
	 *	��ѯ���ж���
	 */
	virtual int queryOrders() { return -1; }

	/*
	 *	��ѯ�ɽ���ϸ
	 */
	virtual int	queryTrades() { return -1; }

	/*
	 *	��ѯ���㵥
	 */
	virtual int querySettlement(uint32_t uDate){ return 0; }

};

NS_WTP_END

//��ȡIDataMgr�ĺ���ָ������
typedef wtp::ITraderApi* (*FuncCreateTrader)();
typedef void(*FuncDeleteTrader)(wtp::ITraderApi* &trader);
