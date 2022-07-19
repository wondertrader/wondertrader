//----------------------------------------------------------------------------
// ��Ȩ������������ģ�����ڽ�֤΢�ں˼ܹ�ƽ̨(KMAP)��һ����
//           ��֤�Ƽ��ɷ����޹�˾  ��Ȩ����
//
// �ļ����ƣ�maCliOptTradeApi.h
// ģ�����ƣ���Ʊ��Ȩ����API C++ Class
// ģ��������
// �������ߣ������
// �������ڣ�2012-12-23
// ģ��汾��001.000.000
//----------------------------------------------------------------------------
// �޸�����      �汾          ����            ��ע
//----------------------------------------------------------------------------
// 2012-12-23    1.0          �����          ����
//----------------------------------------------------------------------------
#if !defined(__MA_CLI_OPT_TRADE_API_H__)
#define __MA_CLI_OPT_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliOptTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliOptTradeSpi : virtual public CCliTradeSpi
{
public:
  // �û���¼������Ӧ
  virtual int OnRspUserLogin(CFirstSetField* p_pFirstSetField, CRspOptUserLoginField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��Ȩ������Ϣ��ѯ��Ӧ
  virtual int OnRspQryBaseInfo(CFirstSetField* p_pFirstSetField, CRspOptBaseInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩί���걨��Ӧ
  virtual int OnRspOrder(CFirstSetField* p_pFirstSetField, CRspOptOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ������Ȩί�г�����Ӧ
  virtual int OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩί���걨��Ӧ(����)
  virtual int OnRspOrderFlash(CFirstSetField* p_pFirstSetField, CRspOptOrderFlashField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩί�г�����Ӧ(����)
  virtual int OnRspCancelOrderFlash(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderFlashField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩ���֤ȯ����������Ӧ
  virtual int OnRspUndlStkLock(CFirstSetField* p_pFirstSetField, CRspOptUndlStkLockField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ���������Ȩ���ɽ���������Ӧ
  virtual int OnRspMaxTradeQty(CFirstSetField* p_pFirstSetField, CRspOptMaxTradeQtyField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // �����ʽ��ѯ��Ӧ
  virtual int OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, CRspOptExpendableFundField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ���ú�Լ�ʲ���ѯ��Ӧ
  virtual int OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, CRspOptExpendableCuField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ���ñ��ҹɷݲ�ѯ��Ӧ
  virtual int OnRspQryExpendableCvdStk(CFirstSetField* p_pFirstSetField, CRspOptExpendableCvdStkField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩ����ί�в�ѯ��Ӧ
  virtual int OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, CRspOptCurrDayOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩ���ճɽ���ѯ��Ӧ
  virtual int OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, CRspOptCurrDayFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩ��Լ�˻���ѯ��Ӧ
  virtual int OnRspQryAcct(CFirstSetField* p_pFirstSetField, CRspOptAcctField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ������Ȩ�ɳ�ί�в�ѯ��Ӧ
  virtual int OnRspQryCanWithdrawOrder(CFirstSetField* p_pFirstSetField, CRspOptCanWithdrawOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ȷ�ϻر�
  virtual int OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRspField) {return 0;}

  // �ɽ��ر�--ί����Ϣ
  virtual int OnRtnOrder(CRtnOptOrderField* p_pRspField) {return 0;}

  // �ɽ��ر�--�ɽ���Ϣ
  virtual int OnRtnOrderFill(CRtnOptOrderFillField* p_pRspField) {return 0;}

  // �ɽ��ر�--��Լ��Ϣ
  virtual int OnRtnContract(CRtnOptContractField* p_pRspField) {return 0;}

  // ȷ�ϻر�(����)
  virtual int OnRtnOrderConfirmFlash(CRtnOptOrderConfirmFlashField* p_pRspField) {return 0;}

  // �ɽ��ر�(����)
  virtual int OnRtnOrderFillFlash(CRtnOptOrderFillFlashField* p_pRspField) {return 0;}

  //////////////////////////////////////////////////////////////////////////////
  
  //��ϲ����ļ���ѯ��Ӧ 2015/8/3
  virtual int OnRspQryCombStra(CFirstSetField* p_pFirstSetField,CRspOptCombStraField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //��ϱ�֤��ί����Ӧ 2015/8/5
  virtual int OnRspCombStraOrder(CFirstSetField* p_pFirstSetField,CRspOptCombStraOrderField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //������ϳֲֲ�ѯ��Ӧ 2015/8/5
  virtual int OnRspQryCombStraPos(CFirstSetField* p_pFirstSetField,CRspOptCombStraPosField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //������ϳֲ���ϸ��ѯ��Ӧ 2015/8/5
  virtual int OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSetField,CRspOptCombStraPosDetailField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //������Ȩ��ʼ��֤�� 2015/8/10
  virtual int OnRspIniMargin(CFirstSetField* p_pFirstSetField,CRspOptIniMarginField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //��Ȩָ��Ƿ��     2015/8/10
  virtual int OnRspQryExeDebt(CFirstSetField* p_pFirstSetField,CRspOptExeDebtField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //��Ȩָ��Ƿȯ       2015/8/10
  virtual int OnRspQryExeStkDebt(CFirstSetField* p_pFirstSetField,CRspOptExeStkDebtField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //�ͻ����ն�         2015/8/10
  virtual int OnRspQryRiskLvl(CFirstSetField* p_pFirstSetField,CRspOptRiskLvlField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //�ͻ����㵥��ѯ 2015/8/6
  virtual int OnRspQrySettList(CFirstSetField* p_pFirstSetField,CRspOptSettListField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //�ͻ����㵥ȷ�� 2015/8/6
  virtual int OnRspSettListConfirm(CFirstSetField* p_pFirstSetField,CRspOptSettListConfirmField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //����ת����Ӧ 2015/8/3
  virtual int OnRspBankDeriTrans(CFirstSetField* p_pFirstSetField,CRspOptBankDeriTransField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}
  
  //��ѯ���ܽ���������Ϣ��Ӧ 2015/8/7
  virtual int OnRspBankDeriInfo(CFirstSetField* p_pFirstSetField,CRspOptBankDeriInfoField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //��ѯ�ͻ�ǩԼ���д�����Ӧ 2015/8/7
  virtual int OnRspUserBank(CFirstSetField* p_pFirstSetField,CRspOptUserBankField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //���˵���Ȩ������ˮ��ѯ��Ӧ 2015/8/7
  virtual int OnRspStateDeliHis(CFirstSetField* p_pFirstSetField,CRspOptStateDeliHisField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //����ת����ˮ��ѯ��Ӧ 2015/8/7
  virtual int OnRspBankDeriTransHis(CFirstSetField* p_pFirstSetField,CRspOptBankDeriTransHisField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //��������ѯ��Ӧ 2015/8/7
  virtual int OnRspBankBala(CFirstSetField* p_pFirstSetField,CRspOptBankBalaField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //�����޸���Ӧ 2015/8/3
  virtual int OnRspChangePwd(CFirstSetField* p_pFirstSetField,CRspOptChangePwdField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  // �������ɷݲ�ѯ
  virtual int OnRspQryCanLockStk(CFirstSetField* p_pFirstSetField, CRspOptCanLockStkField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // �ͻ��ʵ���ƥ���Ʒ��Ϣ��ѯ
  virtual int OnRspQryCustAppropriateMatchInfo(CFirstSetField* p_pFirstSetField, CRspOptQryCustAppropriateMatchInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // �ͻ��ʵ�����Ϣ��ѯ
  virtual int OnRspQryCustAppropriateInfo(CFirstSetField* p_pFirstSetField, CRspOptQryCustAppropriateInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // �ͻ����վ�ʾ��ǩ��
  virtual int OnRspCustRiskSign(CFirstSetField* p_pFirstSetField, CRspOptCustRiskSignField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ��Ʊ��Ȩ��Ȩָ��ϲ��걨ί����Ӧ
  virtual int OnRspCombExe(CFirstSetField *p_pFirstSetField, CRspOptCombExeField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��Ȩָ��ϲ��걨�ɷݺ�Լ��ѯ��Ӧ
  virtual int OnRspQryCombExeLegNum(CFirstSetField *p_pFirstSetField, CRspOptQryCombExeLegNumField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��Ʊ��Ȩ����ί�в�ѯ[�Ż�]��Ӧ
  virtual int OnRspQryCurrDayOrderEx(CFirstSetField *p_pFirstSetField, CRspOptQryCurrDayOrderExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��Ʊ��Ȩ���ճɽ���ѯ[�Ż�]��Ӧ
  virtual int OnRspQryCurrDayFillEx(CFirstSetField *p_pFirstSetField, CRspOptQryCurrDayFillExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // �����ʽ��ѯ[�Ż�]��Ӧ
  virtual int OnRspQryExpendableFundEx(CFirstSetField *p_pFirstSetField, CRspOptQryExpendableFundExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��ȡ�ʽ��ѯ[�Ż�]��Ӧ
  virtual int OnRspQryDrawFundEx(CFirstSetField *p_pFirstSetField, CRspOptQryDrawFundExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ���ú�Լ�ʲ���ѯ[�Ż�]��Ӧ
  virtual int OnRspQryExpendableCuEx(CFirstSetField *p_pFirstSetField, CRspOptQryExpendableCuExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��ʷί�в�ѯ(RPC)��Ӧ
  virtual int OnRspQryHisOrder(CFirstSetField* p_pFirstSetField, CRspOptQryHisOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��ʷ�ɽ���ѯ(RPC)��Ӧ
  virtual int OnRspQryHisFill(CFirstSetField* p_pFirstSetField, CRspOptQryHisFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // �ͻ����㵥(�ļ�)��ѯ��Ӧ
  virtual int OnRspQrySettListFile(CFirstSetField* p_pFirstSetField, CRspOptQrySettListFileField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ������Ȩί���걨(�Ż�)��Ӧ
  virtual int OnRspOrderEx(CFirstSetField* p_pFirstSetField, CRspOptOrderExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��Ȩ����˫�߱��ۣ����ڣ���Ӧ
  virtual int OnRspBilateralQuote(CFirstSetField* p_pFirstSetField, CRspOptBilateralQuoteField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��Ȩ����˫�߱��۳��������ڣ���Ӧ
  virtual int OnRspBilateralQuotationKill(CFirstSetField* p_pFirstSetField, CRspOptBilateralQuotationKillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ֤ȯ��Ϣ��ѯ����Ȩ����Ӧ
  virtual int OnRspQryStkInfo(CFirstSetField* p_pFirstSetField, CRspOptQryStkInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ��Ȩ�ͻ���¼(��)��Ӧ
  virtual int OnRspRpcUserLogin(CFirstSetField* p_pFirstSetField, CRspOptRpcUserLoginField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ���ٶ����ʽ𻮲���Ӧ
  virtual int OnRspRpcFundingTransfer(CFirstSetField* p_pFirstSetField, CRspOptRpcFundingTransferField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ���ٶ����ɻ����ʽ��ѯ��Ӧ
  virtual int OnRspQryRpcFundingTransfer(CFirstSetField* p_pFirstSetField, CRspOptQryRpcFundingTransferField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ���ٶ����ʽ𻮲���ˮ��ѯ��Ӧ
  virtual int OnRspQryRpcFundingTransferFlow(CFirstSetField* p_pFirstSetField, CRspOptQryRpcFundingTransferFlowField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // �ٽ����ں�Լ�ʲ���ѯ��Ӧ
  virtual int OnRspQryExpendableCuOfCloseToExpireDate(CFirstSetField *p_pFirstSetField, CRspOptQryExpendableCuOfCloseToExpireDateField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ��Ʊ��Ȩ��Ȩָ����ϸ��ѯ��Ӧ
  virtual int OnRspQryRpcExeDetail(CFirstSetField *p_pFirstSetField, CRspOptQryRpcExeDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ��Ʊ��Ȩ���շ���֪ͨ��ѯ��Ӧ
  virtual int OnRspQryRpcCurrDayRiskNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcCurrDayRiskNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ��Ʊ��Ȩ��ʷ����֪ͨ��ѯ��Ӧ
  virtual int OnRspQryRpcHisRiskNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcHisRiskNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ��Ʊ��Ȩ���շ���֪ͨ��ѯ�����ϵͳר�ã���Ӧ
  virtual int OnRspQryRpcCurrDayRiskSysNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcCurrDayRiskSysNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // ��Ʊ��Ȩ��ʷ����֪ͨ��ѯ�����ϵͳר�ã���Ӧ
  virtual int OnRspQryRpcHisRiskSysNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcHisRiskSysNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliOptTradeApi : virtual public CCliTradeApi
{
public:
  // Ĭ�Ϲ��캯��
  CCliOptTradeApi(void);

  // ��������
  virtual ~CCliOptTradeApi(void);

  // �û���¼����
  virtual int ReqUserLogin(CReqOptUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // ��Ȩ������Ϣ��ѯ
  virtual int ReqQryBaseInfo(CReqOptBaseInfoField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩί���걨
  virtual int ReqOrder(CReqOptOrderField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩί�г���
  virtual int ReqCancelOrder(CReqOptCancelOrderField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩί���걨(����)
  virtual int ReqOrderFlash(CReqOptOrderFlashField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩί�г���(����)
  virtual int ReqCancelOrderFlash(CReqOptCancelOrderFlashField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩ���֤ȯ��������
  virtual int ReqUndlStkLock(CReqOptUndlStkLockField *p_pReqField, LONGLONG p_llRequestId);

  // ���������Ȩ���ɽ�������
  virtual int ReqMaxTradeQty(CReqOptMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);

  // �����ʽ��ѯ
  virtual int ReqQryExpendableFund(CReqOptExpendableFundField *p_pReqField, LONGLONG p_llRequestId);

  // ���ú�Լ�ʲ���ѯ
  virtual int ReqQryExpendableCu(CReqOptExpendableCuField *p_pReqField, LONGLONG p_llRequestId);

  // ���ñ��ҹɷݲ�ѯ
  virtual int ReqQryExpendableCvdStk(CReqOptExpendableCvdStkField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩ����ί�в�ѯ
  virtual int ReqQryCurrDayOrder(CReqOptCurrDayOrderField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩ���ճɽ���ѯ
  virtual int ReqQryCurrDayFill(CReqOptCurrDayFillField *p_pReqField, LONGLONG p_llRequestId);
 
  // ������Ȩ��Լ�˻���ѯ
  virtual int ReqQryAcct(CReqOptAcctField *p_pReqField, LONGLONG p_llRequestId);

  // ������Ȩ�ɳ�ί�в�ѯ
  virtual int ReqQryCanWithdrawOrder(CReqOptCanWithdrawOrderField *p_pReqField, LONGLONG p_llRequestId);

  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  //��ϲ����ļ���ѯ 2015/8/3
  virtual int ReqQryCombStra(CReqOptCombStraField * p_pReqField,LONGLONG p_llRequestId);

  //��ϱ�֤��ί�� 2015/8/5 
  virtual int ReqCombStraOrder(CReqOptCombStraOrderField* p_pReqField,LONGLONG p_llRequestId);
  
  //������ϳֲֲ�ѯ 2015/8/5 
  virtual int ReqQryCombStraPos(CReqOptCombStraPosField* p_pReqField,LONGLONG p_llRequestId);
 
  //������ϳֲ���ϸ��ѯ 2015/8/5 
  virtual int ReqQryCombStraPosDetail(CReqOptCombStraPosDetailField* p_pReqField,LONGLONG p_llRequestId);
  
  //������Ȩ��ʼ��֤�� 2015/8/10
  virtual int ReqIniMargin(CReqOptIniMarginField* p_pReqField,LONGLONG p_llRequestId);

  //��Ȩָ��Ƿ��     2015/8/10
  virtual int ReqQryExeDebt(CReqOptExeDebtField* p_pReqField,LONGLONG p_llRequestId);

  //��Ȩָ��Ƿȯ       2015/8/10
  virtual int ReqQryExeStkDebt(CReqOptExeStkDebtField* p_pReqField,LONGLONG p_llRequestId);

  //�ͻ����ն�         2015/8/10
  virtual int ReqQryRiskLvl(CReqOptRiskLvlField* p_pReqField,LONGLONG p_llRequestId);
  
  //�ͻ����㵥��ѯ 2015/8/6
  virtual int ReqQrySettList(CReqOptSettListField* p_pReqField,LONGLONG p_llRequestId);

  //�ͻ����㵥ȷ�� 2015/8/6
  virtual int ReqSettListConfirm(CReqOptSettListConfirmField* p_pReqField,LONGLONG p_llRequestId);

  //����ת�� 2015/8/3
  virtual int ReqBankDeriTrans(CReqOptBankDeriTransField* p_pReqField,LONGLONG p_llRequestId);
  
  //��ѯ���ܽ���������Ϣ 2015/8/7
  virtual int ReqBankDeriInfo(CReqOptBankDeriInfoField* p_pReqField,LONGLONG p_llRequestId);

  //��ѯ�ͻ�ǩԼ���д��� 2015/8/7
  virtual int ReqUserBank(CReqOptUserBankField* p_pReqField,LONGLONG p_llRequestId);

  //���˵���Ȩ������ˮ��ѯ 2015/8/7
  virtual int ReqStateDeliHis(CReqOptStateDeliHisField* p_pReqField,LONGLONG p_llRequestId);

  //����ת����ˮ��ѯ 2015/8/7
  virtual int ReqBankDeriTransHis(CReqOptBankDeriTransHisField* p_pReqField,LONGLONG p_llRequestId);

  //��������ѯ 2015/8/7
  virtual int ReqBankBala(CReqOptBankBalaField* p_pReqField,LONGLONG p_llRequestId);

  //�����޸� 2015/8/3
  virtual int ReqChangePwd(CReqOptChangePwdField* p_pReqField,LONGLONG p_llRequestId);

  // �������ɷݲ�ѯ
  virtual int ReqQryCanLockStk(CReqOptCanLockStkField* p_pReqField, LONGLONG p_llRequestId);

  // �ͻ��ʵ���ƥ���Ʒ��Ϣ��ѯ
  virtual int ReqQryCustAppropriateMatchInfo(CReqOptQryCustAppropriateMatchInfoField* p_pReqField, LONGLONG p_llRequestId);

  // �ͻ��ʵ�����Ϣ��ѯ
  virtual int ReqQryCustAppropriateInfo(CReqOptQryCustAppropriateInfoField* p_pReqField, LONGLONG p_llRequestId);

  // �ͻ����վ�ʾ��ǩ��
  virtual int ReqCustRiskSign(CReqOptCustRiskSignField* p_pReqField, LONGLONG p_llRequestId);

  // ��Ʊ��Ȩ��Ȩָ��ϲ��걨ί������
  virtual int ReqCombExe(CReqOptCombExeField *p_pReqField, LONGLONG p_llRequestId);

  // ��Ȩָ��ϲ��걨�ɷݺ�Լ��ѯ����
  virtual int ReqQryCombExeLegNum(CReqOptQryCombExeLegNumField *p_pReqField, LONGLONG p_llRequestId);

  // ��Ʊ��Ȩ����ί�в�ѯ[�Ż�]����
  virtual int ReqQryCurrDayOrderEx(CReqOptQryCurrDayOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // ��Ʊ��Ȩ���ճɽ���ѯ[�Ż�]����
  virtual int ReqQryCurrDayFillEx(CReqOptQryCurrDayFillExField *p_pReqField, LONGLONG p_llRequestId);

  // �����ʽ��ѯ[�Ż�]����
  virtual int ReqQryExpendableFundEx(CReqOptQryExpendableFundExField *p_pReqField, LONGLONG p_llRequestId);

  // ��ȡ�ʽ��ѯ[�Ż�]����
  virtual int ReqQryDrawFundEx(CReqOptQryDrawFundExField *p_pReqField, LONGLONG p_llRequestId);

  // ���ú�Լ�ʲ���ѯ[�Ż�]����
  virtual int ReqQryExpendableCuEx(CReqOptQryExpendableCuExField *p_pReqField, LONGLONG p_llRequestId);

  // ��ʷί�в�ѯ(RPC)����
  virtual int ReqQryHisOrder(CReqOptQryHisOrderField* p_pRspField, LONGLONG p_llRequestId);

  // ��ʷ�ɽ���ѯ(RPC)����
  virtual int ReqQryHisFill(CReqOptQryHisFillField* p_pRspField, LONGLONG p_llRequestId);

  // �ͻ����㵥(�ļ�)��ѯ����
  virtual int ReqQrySettListFile(CReqOptQrySettListFileField* p_pRspField, LONGLONG p_llRequestId);

  // ������Ȩί���걨(�Ż�)
  virtual int ReqOrderEx(CReqOptOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // ��Ȩ����˫�߱��ۣ����ڣ�
  virtual int ReqBilateralQuote(CReqOptBilateralQuoteField *p_pReqField, LONGLONG p_llRequestId);

  // ��Ȩ����˫�߱��۳��������ڣ�
  virtual int ReqBilateralQuotationKill(CReqOptBilateralQuotationKillField *p_pReqField, LONGLONG p_llRequestId);

  // ֤ȯ��Ϣ��ѯ����Ȩ��
  virtual int ReqQryStkInfo(CReqOptQryStkInfoField *p_pReqField, LONGLONG p_llRequestId);

  // ��Ȩ�ͻ���¼(��)
  virtual int ReqRpcUserLogin(CReqOptRpcUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // ���ٶ����ʽ𻮲�����
  virtual int ReqRpcFundingTransfer(CReqOptRpcFundingTransferField *p_pReqField, LONGLONG p_llRequestId);

  // ���ٶ����ɻ����ʽ��ѯ����
  virtual int ReqQryRpcFundingTransfer(CReqOptQryRpcFundingTransferField *p_pReqField, LONGLONG p_llRequestId);
  
  // ���ٶ����ʽ𻮲���ˮ��ѯ����
  virtual int ReqQryRpcFundingTransferFlow(CReqOptQryRpcFundingTransferFlowField *p_pReqField, LONGLONG p_llRequestId);
  
  // �ٽ����ں�Լ�ʲ���ѯ����
  virtual int ReqQryExpendableCuOfCloseToExpireDate(CReqOptQryExpendableCuOfCloseToExpireDateField *p_pReqField, LONGLONG p_llRequestId);
  
  // ��Ʊ��Ȩ��Ȩָ����ϸ��ѯ����
  virtual int ReqQryRpcExeDetail(CReqOptQryRpcExeDetailField *p_pReqField, LONGLONG p_llRequestId);
  
  // ��Ʊ��Ȩ���շ���֪ͨ��ѯ����
  virtual int ReqQryRpcCurrDayRiskNotify(CReqOptQryRpcCurrDayRiskNotifyField *p_pReqField, LONGLONG p_llRequestId);
  
  // ��Ʊ��Ȩ��ʷ����֪ͨ��ѯ����
  virtual int ReqQryRpcHisRiskNotify(CReqOptQryRpcHisRiskNotifyField *p_pReqField, LONGLONG p_llRequestId);
  
  // ��Ʊ��Ȩ���շ���֪ͨ��ѯ�����ϵͳר�ã�����
  virtual int ReqQryRpcCurrDayRiskSysNotify(CReqOptQryRpcCurrDayRiskSysNotifyField *p_pReqField, LONGLONG p_llRequestId);
  
  // ��Ʊ��Ȩ��ʷ����֪ͨ��ѯ�����ϵͳר�ã�����
  virtual int ReqQryRpcHisRiskSysNotify(CReqOptQryRpcHisRiskSysNotifyField *p_pReqField, LONGLONG p_llRequestId);

public:
  // �첽�ص�����
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // �����ص�����
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // �û���¼������Ӧ
  void OnRspUserLogin(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��Ȩ������Ϣ��ѯ��Ӧ
  void OnRspQryBaseInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩί���걨��Ӧ
  void OnRspOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩί�г�����Ӧ
  void OnRspCancelOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩ���֤ȯ����������Ӧ
  void OnRspUndlStkLock(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ���������Ȩ���ɽ���������Ӧ
  void OnRspMaxTradeQty(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // �����ʽ��ѯ��Ӧ
  void OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ���ú�Լ�ʲ���ѯ��Ӧ
  void OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ���ñ��ҹɷݲ�ѯ��Ӧ
  void OnRspQryExpendableCvdStk(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩ����ί�в�ѯ��Ӧ
  void OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩ���ճɽ���ѯ��Ӧ
  void OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩ��Լ�˻���ѯ��Ӧ
  void OnRspQryAcct(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩ�ɳ�ί�в�ѯ��Ӧ
  void OnRspQryCanWithdrawOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  ////////////////////////////////////////////////////////////////////////////////////////
  //��ϲ����ļ���ѯ��Ӧ  2015/8/5
  void OnRspQryCombStra(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //��ϱ�֤��ί����Ӧ  2015/8/3
  void OnRspCombStraOrder(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //������ϳֲֲ�ѯ��Ӧ  2015/8/5
  void OnRspQryCombStraPos(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //������ϳֲ���ϸ��ѯ��Ӧ  2015/8/5
  void OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum); 
  
  //������Ȩ��ʼ��֤�� 2015/8/10
  void OnRspIniMargin(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //��Ȩָ��Ƿ�ʲ�ѯ     2015/8/10
  void OnRspQryExeDebt(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //��Ȩָ��Ƿȯ��ѯ      2015/8/10
  void OnRspQryExeStkDebt(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //�ͻ����նȲ�ѯ         2015/8/10
  void OnRspQryRiskLvl(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //�ͻ����㵥��ѯ 2015/8/6
  void OnRspQrySettList(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum); 
  
  //�ͻ����㵥ȷ�� 2015/8/6
  void OnRspSettListConfirm(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum); 
   
  //����ת����Ӧ 2015/8/3
  void OnRspBankDeriTrans(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //��ѯ���ܽ���������Ϣ��Ӧ 2015/8/7
  void OnRspBankDeriInfo(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //��ѯ�ͻ�ǩԼ���д�����Ӧ 2015/8/7
  void OnRspUserBank(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //���˵���Ȩ������ˮ��ѯ��Ӧ 2015/8/7
  void OnRspStateDeliHis(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //����ת����ˮ��ѯ��Ӧ 2015/8/7
  void OnRspBankDeriTransHis(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //��������ѯ��Ӧ 2015/8/7
  void OnRspBankBala(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //�����޸���Ӧ 2015/8/3
  void OnRspChangePwd(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  // �������ɷݲ�ѯ��Ӧ
  void OnRspQryCanLockStk(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // �ͻ��ʵ���ƥ���Ʒ��Ϣ��ѯ��Ӧ
  void OnRspQryCustAppropriateMatchInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // �ͻ��ʵ�����Ϣ��ѯ��Ӧ
  void OnRspQryCustAppropriateInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // �ͻ����վ�ʾ��ǩ����Ӧ
  void OnRspCustRiskSign(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��Ʊ��Ȩ��Ȩָ��ϲ��걨ί����Ӧ
  void OnRspCombExe(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // ��Ȩָ��ϲ��걨�ɷݺ�Լ��ѯ��Ӧ
  void OnRspQryCombExeLegNum(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��Ʊ��Ȩ����ί�в�ѯ[�Ż�]��Ӧ
  void OnRspQryCurrDayOrderEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��Ʊ��Ȩ���ճɽ���ѯ[�Ż�]��Ӧ
  void OnRspQryCurrDayFillEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // �����ʽ��ѯ[�Ż�]��Ӧ
  void OnRspQryExpendableFundEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��ȡ�ʽ��ѯ[�Ż�]��Ӧ
  void OnRspQryDrawFundEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ���ú�Լ�ʲ���ѯ[�Ż�]��Ӧ
  void OnRspQryExpendableCuEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��ʷί�в�ѯ(RPC)��Ӧ
  void OnRspQryHisOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��ʷ�ɽ���ѯ(RPC)��Ӧ
  void OnRspQryHisFill(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // �ͻ����㵥(�ļ�)��ѯ��Ӧ
  void OnRspQrySettListFile(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ������Ȩί���걨(�Ż�)��Ӧ
  void OnRspOrderEx(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��Ȩ����˫�߱��ۣ����ڣ���Ӧ
  void OnRspBilateralQuote(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��Ȩ����˫�߱��۳��������ڣ���Ӧ
  void OnRspBilateralQuotationKill(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ֤ȯ��Ϣ��ѯ����Ȩ����Ӧ
  void OnRspQryStkInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ��Ȩ�ͻ���¼(��)��Ӧ
  void OnRspRpcUserLogin(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ���ٶ����ʽ𻮲���Ӧ
  void OnRspRpcFundingTransfer(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ���ٶ����ɻ����ʽ��ѯ��Ӧ
  void OnRspQryRpcFundingTransfer(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ���ٶ����ʽ𻮲���ˮ��ѯ��Ӧ
  void OnRspQryRpcFundingTransferFlow(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // �ٽ����ں�Լ�ʲ���ѯ��Ӧ
  void OnRspQryExpendableCuOfCloseToExpireDate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // ��Ʊ��Ȩ��Ȩָ����ϸ��ѯ��Ӧ
  void OnRspQryRpcExeDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // ��Ʊ��Ȩ���շ���֪ͨ��ѯ��Ӧ
  void OnRspQryRpcCurrDayRiskNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // ��Ʊ��Ȩ��ʷ����֪ͨ��ѯ��Ӧ
  void OnRspQryRpcHisRiskNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // ��Ʊ��Ȩ���շ���֪ͨ��ѯ�����ϵͳר�ã���Ӧ
  void OnRspQryRpcCurrDayRiskSysNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // ��Ʊ��Ȩ��ʷ����֪ͨ��ѯ�����ϵͳר�ã���Ӧ
  void OnRspQryRpcHisRiskSysNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_OPT_TRADE_API_H__