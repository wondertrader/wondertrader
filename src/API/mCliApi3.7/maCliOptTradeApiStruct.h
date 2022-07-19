//----------------------------------------------------------------------------
// ��Ȩ������������ģ�����ڽ�֤΢�ں˼ܹ�ƽ̨(KMAP)��һ����
//           ��֤�Ƽ��ɷ����޹�˾  ��Ȩ����
//
// �ļ����ƣ�maCliOptTradeApiStruct.h
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
#if !defined(__MA_CLI_OPT_TRADE_API_STRUCT_H__)
#define __MA_CLI_OPT_TRADE_API_STRUCT_H__

#include "maCliTradeApi.h"

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10314001:�û���¼(�ڲ�ʹ��)------------------------------------
struct MATRADEAPI CReqOptUserLoginField
{
  char          szAcctType[2 + 1];          // �˻����� 'U':�ͻ����� 'Z':�ʲ��˻�
  char          szAcctId[32 + 1];           // �˻���ʶ
  char          chUseScope;                 // ʹ�÷�Χ '4':��Ȩ��������
  char          chAuthType;                 // ��֤���� '0':����
  char          szAuthData[256 + 1];        // ��֤����(����)
  char          chEncryptType;              // ���ܷ�ʽ ��0��:��֤KBSSϵͳ���� ��1��:��֤Win�漯�н��׼��� ��2��:��֤Unix�漯�н��׼��� ��3��:�ⲿ�ؼ��� ��4�� : �ⲿ����
  char          szEncryptKey[32 + 1];       // ��������
};

struct MATRADEAPI CRspOptUserLoginField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          chStkex;                    // �����г�
  char          szStkbd[2 + 1];             // ���װ��
  char          szTrdacct[20 + 1];          // ֤ȯ�˻�
  char          szSubacctCode[8+1];         // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[28+1];         // ��Ȩ��Լ�˻�
  int           iIntOrg;                    // �ڲ�����
  char          chTrdacctStatus;            // �˻�״̬
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szAcctType[2 + 1];          // �˻����� 'U':�ͻ����� 'Z':�ʲ��˻�
  char          szAcctId[32 + 1];           // �˻���ʶ
  char          szTrdacctName[32 + 1];      // �˻�����
  char          szSession[256 + 1];         // �Ựƾ֤
  int           iFlashPort;                 // ����ٶ˿ں�
  char          szFlashStkBizes[64 + 1];    // �����ҵ��
  char          chSzFlashMode;              // ����������г�����ģʽ ��0��-������ ��1��-������� ��2��-FPGA����
  char          chShFlashMode;              // ������Ϻ��г�����ģʽ ��0��-������ ��1��-������� ��2��-FPGA����
  char          chFlashEnableFlag;          // �������ñ�־ ��0��-δ���� ��1��-������
};

//-------------------------------10311001:��Ȩ������Ϣ��ѯ------------------------------------
struct MATRADEAPI CReqOptBaseInfoField
{
  char          szStkbd[2 + 1];             // ���װ��
  char          szOptNum[32 + 1];           // ��Լ����
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          chOptType;                  // ��Լ����
  char          chIsRemind;                 // �������ѱ�־ 1-����������Ϣ��ǩ����1-�����أ�����ί��ʱ�������ѣ�
  char          chQueryFlag;                // ��ѯ����
  char          szQryPos[32 + 1];           // ��λ��
  int           iQryNum;                    // ��ѯ����
};

struct MATRADEAPI CRspOptBaseInfoField
{
  char          chStkex;                    // �����г� �ֵ�[STKEX]
  char          szStkbd[2 + 1];             // �˻���ʶ �ֵ�[STKBD]
  char          szOptNum[16 + 1];           // ��Լ����
  char          szOptCode[32 + 1];          // ��Լ����
  char          szOptName[120 + 1];         // ��Լ���
  char          chOptType;                  // ��Լ����
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOptUndlName[16 + 1];      // ���֤ȯ����
  char          chOptUndlCls;               // ���֤ȯ��� �ֵ�[STK_CLS]
  char          chOptExeType;               // ���з�ʽ
  LONGLONG      llOptUnit;                  // ��Լ��λ
  char          szExepcisePrice[11 + 1];    // ��Ȩ��
  int           iStartDate;                 // �׸�������
  int           iEndDate;                   // �������
  int           iExeRciseDate;              // ��Ȩ��Ȩ��
  int           iExpireDate;                // ��Ȩ������
  char          chUpdVersion;               // ��Լ�汾��
  LONGLONG      llLeavesQty;                // δƽ�ֺ�Լ��
  char          szPreClosePx[11 + 1];       // �������̼�
  char          szPreSettPrice[11 + 1];     // ���ս����
  char          szUndlClsPrice[11 + 1];     // ���ȯǰ����
  char          chPriceLmtType;             // �ǵ�����������
  char          szOptUplmtPrice[11 + 1];    // ��ͣ�۸�
  char          szOptLwlmtPrice[11 + 1];    // ��ͣ�۸�
  char          szMarginUnit[21 + 1];       // ��λ��֤��
  char          szMarginRatio1[11 + 1];     // ��֤������������һ
  char          szMarginRatio2[11 + 1];     // ��֤��������������
  LONGLONG      llOptLotSize;               // ������
  LONGLONG      llOptLbuplmtQty;            // �޼��򵥱��걨����
  LONGLONG      llOptLsuplmtQty;            // �޼��������걨����
  LONGLONG      llOptLlwlmtQty;             // �޼۵����걨����
  LONGLONG      llOptMbuplmtQty;            // �м��򵥱��걨����
  LONGLONG      llOptMsuplmtQty;            // �м��������걨����
  LONGLONG      llOptMlwlmtQty;             // �м۵����걨����
  char          chOpenFlag;                 // ���ֱ�־
  char          chSuspendedFlag;            // ͣ�Ʊ�־ 0:���� 1:��ʱͣ�� 2:����ͣ��
  char          chExpireFlag;               // �����ձ�־
  char          chAdjustFlag;               // ������־
  char          chOptStatus;                // ��Լ״̬
  char          chCombFlag;                 // ��Լ��ϱ�־λ 2015/8/3���
  int           iAutoSplitDate;             // ��ϲ��Ե��ڽ���� 2015/8/3���
  int           iUpdDate;                   // ��������
  int           iDeliveryDate;              // ��Ȩ������
  char          szQryPos[32 + 1];           // ��λ��
};

//-------------------------------10312001:������Ȩί���걨------------------------------------
struct MATRADEAPI CReqOptOrderField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� ��λ��Ԫ����ȷ��0.1��
  LONGLONG        llOrderQty;                 // ί������ ��ͨѯ����0
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]���Ϲ���Ȩ���Ϲ���Ȩ����ͨѯ��Ĭ����100
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  int             iOrderBsn;                  // ί������ 
  char            szClientInfo[256 + 1];      // �ն���Ϣ 1��ͨ��mid����ʱ��ֵ����͹̶����F_OP_SITE(����վ��)��ֵ����һ�£� 2��ͨ��������ʽ����ʱ��ֵ����Ϊ�գ������ֵ������͹̶����F_OP_SITE(����վ��)��ֵ����һ��
  char            chSecurityLevel;            // ��ȫ�ֶ� 0:�ް�ȫ 1:У��Ʊ�� 2:У������
  char            szSecurityInfo[256 + 1];    // ��ȫ��Ϣ ��ȫ�ֶ�ΪУ��Ʊ��������дƱ�� ��ȫ�ֶ�Ϊ����������д������������
  char            szOrderIdEx[64 + 1];        // �ⲿ��ͬ��� �����Ϊ�ⲿ���룬��Ȩϵͳ�����κδ���
  char            chEncryptType;              // ���ܷ�ʽ ����ȫ�ֶ�Ϊ2��У������ʱ��Ϊ������Ρ� ���ܷ�ʽ: ��0��:��֤KBSSϵͳ���ܷ�ʽ ��1��:��֤Win�漯�н��׼��ܷ�ʽ ��2��:��֤Unix�漯�н��׼��ܷ�ʽ ��3��:�ⲿ�ؼ��ܷ�ʽ ��4��:�ⲿ���ܷ�ʽ ˵��������θ��ݽ���ȯ��Ҫ����д��Ĭ��ֵΪ0�� �ⲿ�ؼ��ܷ�ʽ��3����ʹ�÷ǽ�֤���ܺ������������ļ���Ϊ���ĺ�Ȼ��ʹ�ý�֤��˾�ṩ��ͨ�ż��ܺ���kbss_comencrypt��������һ�μ��ܡ� �ⲿ���ܷ�ʽ��4����ʹ�÷ǽ�֤���ܺ������������ļ���Ϊ���ĺ���Ϊ��Ρ�
  char            szEncryptKey[32 + 1];       // �������� ����ȫ�ֶ�Ϊ2��У������ʱ��Ϊ������Ρ� �������ӿ�����дΪ�ʲ��˻���CUACCT_CODE����ֵ
  char            szCombNum[16 + 1];          // ��ϱ��� ��ϲ��Ե���ƽ��ʱ��ֵ�����������Ϳ�ֵ
};

struct MATRADEAPI CRspOptOrderField
{
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptCode[32 + 1];          // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szOptUndlName[16 + 1];      // ���֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� 
  int             iStkBizAction;              // ֤ȯҵ����Ϊ 
  char            szOrderIdEx[64 + 1];        // �ⲿ��ͬ��� 
};

//-------------------------------10312002=>10310502:��Ʊ��Ȩί�г���------------------------------------
struct MATRADEAPI CReqOptCancelOrderField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  int             iOrderBsn;                  // ί������ 
  char            szClientInfo[256 + 1];      // �ͻ�����Ϣ 1��ͨ��mid����ʱ��ֵ����͹̶����F_OP_SITE(����վ��)��ֵ����һ�£� 2��ͨ��������ʽ����ʱ��ֵ����Ϊ�գ������ֵ������͹̶����F_OP_SITE(����վ��)��ֵ����һ��
  char            szOrderIdEx[64 + 1];        // �ⲿ��ͬ��� �����Ϊ�ⲿ���룬��Ȩϵͳ�����κδ���
  char            chForceWth;                 // ǿ����־ ��1-��ǿ����1-ǿ��
};

struct MATRADEAPI CRspOptCancelOrderField
{
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻� 
  char            szStkCode[32 + 1];          // ֤ȯ���� 
  char            szStkName[32 + 1];          // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� 
  int             iStkBizAction;              // ֤ȯҵ����Ϊ 
  char            chCancelStatus;             // �ڲ�������־ 1:�ڲ����� ��1:��ͨ����
  char            szOrderIdEx[64 + 1];        // �ⲿ��ͬ��� 
  char            szOrderIdWtd[10 + 1];       // ������ͬ��� 
};

//-------------------------------10312008:������Ȩ���֤ȯ��������------------------------------------
struct MATRADEAPI CReqOptUndlStkLockField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iIntOrg;                    // �ڲ�����
  char          szStkbd[2 + 1];             // ���װ��
  char          szTrdacct[10 + 1];          // ֤ȯ�˻�
  char          szStkCode[8 + 1];           // ֤ȯ����
  LONGLONG      llOrderQty;                 // ί������
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  int           iOrderBsn;                  // ί������
  char          szClientInfo[256 + 1];      // �ն���Ϣ ����Ӳ�̺�\CPU\��������
  char          chSecurityLevel;            // ��ȫ�ֶ� 0:�ް�ȫ 1:У��Ʊ�� 2:У������
  char          szSecurityInfo[256 + 1];    // ��ȫ�ֶ�ΪУ��Ʊ��������дƱ��,��ȫ�ֶ�Ϊ����������д������������
  char          szOrderIdEx[64 + 1];        // �����Ϊ�ⲿ���룬��Ȩϵͳ�����κδ���
  char          chEncryptType;              // ���ܷ�ʽ 2015/8/3���
  char          szEncryptKey[32+1];         // �������� 2015/8/3���
};

struct MATRADEAPI CRspOptUndlStkLockField
{
  int           iOrderBsn;                  // ί������
  char          szOrderId[10 + 1];          // ��ͬ���
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  LONGLONG      llOrderQty;                 // ί������
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szTrdacct[10 + 1];          // ֤ȯ�˻�
  char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[18+1];         // ��Ȩ��Լ�˻� 2015/8/3 
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOptUndlName[16 + 1];      // ���֤ȯ����
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          szOrderIdEx[64 + 1];        // �ⲿ��ͬ���
};

//-------------------------------10312006:���������Ȩ���ɽ�������------------------------------------
struct MATRADEAPI CReqOptMaxTradeQtyField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            szOptNum[16 + 1];           // ��Լ���� ��Ϊ��ƽ�֡���Ȩ����Լ���벻��Ϊ�գ���Ϊ֤ȯ����������֤ȯ���벻��Ϊ�գ���Ϊ����������Ͼ����ͣ���Ϊ��Ȩָ��ϲ��걨������
  char            szStkCode[8 + 1];           // ֤ȯ���� ��Ϊ��ƽ�֡���Ȩ����Լ���벻��Ϊ�գ���Ϊ֤ȯ����������֤ȯ���벻��Ϊ�գ���Ϊ����������Ͼ����ͣ���Ϊ��Ȩָ��ϲ��걨������
  char            szOrderPrice[21 + 1];       // ί�м۸� ��ƽ�ֱ��ͣ���Ȩ��֤ȯ��������������������ϡ���Ȩָ��ϲ��걨�Ǳ���
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]��������Ȩ��֤ȯ��������������������ϡ���Ȩָ��ϲ��걨��100
  char            szCombNum[16 + 1];          // ��ϱ��� ������ϲ���ʱ�������룬��ϲ��Ե���ƽ��ʱ��ֵ
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� ����������ϱ��ͣ���������
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 1.�Ϲ���ͨ�����ת���Ҳֳɷ�һ��Լ������ͣ� 2.�������ʱ�����Ϲ���ͨ�����ת���Ҳ�֮�⣬������ϲ����м����Ⱦͱ������뼸���ɷֺ�Լ���룬���谴�����룻 3.������ʱ����������ϱ���COMB_NUM����ɷֺ�Լ�������롣 4.��Ȩָ��ϲ��걨�ɷ�һ��Լ����ͳɷֶ���Լ�������
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 1.�Ϲ���ͨ�����ת���Ҳֳɷ�һ��Լ������ͣ� 2.�������ʱ�����Ϲ���ͨ�����ת���Ҳ�֮�⣬������ϲ����м����Ⱦͱ������뼸���ɷֺ�Լ���룬���谴�����룻 3.������ʱ����������ϱ���COMB_NUM����ɷֺ�Լ�������롣 4.��Ȩָ��ϲ��걨�ɷ�һ��Լ����ͳɷֶ���Լ�������
  char            szLeg3Num[16 + 1];          // �ɷ�����Լ���� 1.�Ϲ���ͨ�����ת���Ҳֳɷ�һ��Լ������ͣ� 2.�������ʱ�����Ϲ���ͨ�����ת���Ҳ�֮�⣬������ϲ����м����Ⱦͱ������뼸���ɷֺ�Լ���룬���谴�����룻 3.������ʱ����������ϱ���COMB_NUM����ɷֺ�Լ�������롣 4.��Ȩָ��ϲ��걨�ɷ�һ��Լ����ͳɷֶ���Լ�������
  char            szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ���� 1.�Ϲ���ͨ�����ת���Ҳֳɷ�һ��Լ������ͣ� 2.�������ʱ�����Ϲ���ͨ�����ת���Ҳ�֮�⣬������ϲ����м����Ⱦͱ������뼸���ɷֺ�Լ���룬���谴�����룻 3.������ʱ����������ϱ���COMB_NUM����ɷֺ�Լ�������롣 4.��Ȩָ��ϲ��걨�ɷ�һ��Լ����ͳɷֶ���Լ�������
};

struct MATRADEAPI CRspOptMaxTradeQtyField
{
  char            chStkex;                    // �����г� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            szSubAcctcode[8 + 1];       // ֤ȯ�˻��ӱ���
  char            szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szCombNum[16 + 1];          // ��ϱ��� 
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 
  char            szLeg3Num[16 + 1];          // �ɷ�����Լ���� 
  char            szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ���� 
  LONGLONG        llOrderQty;                 // ί������ ����ί������
  LONGLONG        llNoLmtOrderQty;            // ���ί������ ȥ���������Ƶ�����ί������
};

//-------------------------------10313019:�����ʽ��ѯ--------------------------
struct MATRADEAPI CReqOptExpendableFundField
{
  LONGLONG        llCustCode;                 // �ͻ����� �����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iValueFlag;                 // ȡֵ��־ �ֶ����ڱ�ʶ���ɸ���ȡֵ MARKET_VALUE��FUND_VALUE�� STK_VALUE��FUND_LOAN �ĸ��ֶ��Ƿ�ȡֵ 0���������� 1��ȡMARKET_VALUEֵ 2��ȡFUND_VALUEֵ 4��ȡSTK_VALUEֵ 8��ȡFUND_LOANֵ �ɸ���ȡֵ
};

struct MATRADEAPI CRspOptExpendableFundField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  int             iIntOrg;                    // �ڲ����� 
  char            szMarketValue[21 + 1];      // �ʲ���ֵ �ͻ��ʲ��ܶʵʱ�����ĵ�
  char            szFundValue[21 + 1];        // �ʽ��ʲ� �ʽ��ʲ��ܶ�
  char            szStkValue[21 + 1];         // ��ֵ ���ʽ��ʲ��ܶ� = ��ֵ
  char            szFundPrebln[21 + 1];       // �ʽ�������� 
  char            szFundBln[21 + 1];          // �ʽ���� 
  char            szFundAvl[21 + 1];          // �ʽ���ý�� 
  char            szFundFrz[21 + 1];          // �ʽ𶳽��� 
  char            szFundUfz[21 + 1];          // �ʽ�ⶳ��� 
  char            szFundTrdFrz[21 + 1];       // �ʽ��׶����� 
  char            szFundTrdUfz[21 + 1];       // �ʽ��׽ⶳ��� 
  char            szFundTrdOtd[21 + 1];       // �ʽ�����;��� 
  char            szFundTrdBln[21 + 1];       // �ʽ��������� 
  char            chFundStatus;               // �ʽ�״̬ �ֵ�[FUND_STATUS]
  char            szMarginUsed[21 + 1];       // ռ�ñ�֤�� 
  char            szMarginInclRlt[21 + 1];    // ��ռ�ñ�֤��(��δ�ɽ�) ������������ί��δ�ɽ�����ı�֤��(��ǰ����ۼ���)
  char            szFundExeMargin[21 + 1];    // ��Ȩ������֤�� 
  char            szFundExeFrz[21 + 1];       // ��Ȩ�ʽ𶳽��� 
  char            szFundFeeFrz[21 + 1];       // �ʽ���ö����� 
  char            szPaylater[21 + 1];         // �渶�ʽ� 
  char            szPreadvaPay[21 + 1];       // Ԥ�Ƶ��ʽ�� ����ETF��ȨE+1��Ԥ����ʹ��
  char            szExpPenInt[21 + 1];        // Ԥ�Ƹ�ծ��Ϣ 
  char            szFundDraw[21 + 1];         // �ʽ��ȡ��� 
  char            szFundAvlRlt[21 + 1];       // �ʽ�̬���� 
  char            szMarginInclDyn[21 + 1];    // ��̬ռ�ñ�֤��(��δ�ɽ�) ������������ί��δ�ɽ�����ı�֤��(��ʵʱ�۸����)
  char            szDailyInAmt[21 + 1];       // ������� 
  char            szDailyOutAmt[21 + 1];      // ���ճ��� 
  char            szFundRealAvl[21 + 1];      // �ʽ�ʵ�ʿ��� ����֤��Ʊ��Ȩ��̨ϵͳ���ö�̬���ù���ʱ���ʽ�ʵ�ʿ���=min���ʽ���ý��ʽ�̬���ã����������ö�̬����ʱ���ʽ�ʵ�ʿ���=�ʽ���ý��
};

//-------------------------------10313001:���ú�Լ�ʲ���ѯ------------------------------------
struct MATRADEAPI CReqOptExpendableCuField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szTrdacct[10 + 1];          // �����˻�
  char          szOptNum[16 + 1];           // ��Լ����
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          chOptSide;                  // �ֲַ��� L-Ȩ���֣�S-����֣�C-���Ҳ��Գֲ�
  char          chOptCvdFlag;               // ���ұ�־ 0-�Ǳ��Һ�Լ 1-���Һ�Լ
  char          chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char          szQryPos[32 + 1];           // ��λ��
  int           iQryNum;                    // ��ѯ����
};

struct MATRADEAPI CRspOptExpendableCuField
{
  char            szQryPos[32 + 1];           // ��λ��
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptCode[32 + 1];          // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            chOptType;                  // ��Լ���� �ֵ�[OPT_TYPE]
  char            chOptSide;                  // �ֲַ��� 
  char            chOptCvdFlag;               // ���ұ�־ 0-�Ǳ��Һ�Լ 1-���Һ�Լ
  LONGLONG        llOptPrebln;                // ��Լ������� 
  LONGLONG        llOptBln;                   // ��Լ��� 
  LONGLONG        llOptAvl;                   // ��Լ�������� 
  LONGLONG        llOptFrz;                   // ��Լ�������� 
  LONGLONG        llOptUfz;                   // ��Լ�ⶳ���� 
  LONGLONG        llOptTrdFrz;                // ��Լ���׶������� 
  LONGLONG        llOptTrdUfz;                // ��Լ���׽ⶳ���� 
  LONGLONG        llOptTrdOtd;                // ��Լ������;���� 
  LONGLONG        llOptTrdBln;                // ��Լ������������ 
  LONGLONG        llOptClrFrz;                // ��Լ���㶳������ 
  LONGLONG        llOptClrUfz;                // ��Լ����ⶳ���� 
  LONGLONG        llOptClrOtd;                // ��Լ������;���� 
  char            szOptBcost[21 + 1];         // ��Լ����ɱ� 
  char            szOptBcostRlt[21 + 1];      // ��Լ����ɱ���ʵʱ�� 
  char            szOptPlamt[21 + 1];         // ��Լӯ����� 
  char            szOptPlamtRlt[21 + 1];      // ��Լӯ����ʵʱ�� 
  char            szOptMktVal[21 + 1];        // ��Լ��ֵ 
  char            szOptPremium[21 + 1];       // Ȩ���� 
  char            szOptMargin[21 + 1];        // ��֤�� 
  LONGLONG        llOptCvdAsset;              // ���ҹɷ����� 
  char            szOptClsProfit[21 + 1];     // ����ƽ��ӯ�� 
  char            szSumClsProfit[21 + 1];     // �ۼ�ƽ��ӯ�� 
  char            szOptFloatProfit[21 + 1];   // ����ӯ�� ����ӯ��=֤ȯ��ֵ-����ɱ�
  char            szTotalProfit[21 + 1];      // ��ӯ�� 
  LONGLONG        llOptRealPosi;              // ��Լʵ�ʳֲ� 
  LONGLONG        llOptClsUnmatched;          // ��Լƽ�ֹҵ����� ��ƽ��ί��δ�ɽ�����
  LONGLONG        llOptDailyOpenRlt;          // �����ۼƿ������� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szExerciseVal[21 + 1];      // ��Ȩ��ֵ �Ϲ�Ȩ���ֵ���Ȩ��ֵ��MAX((���-��Ȩ��), 0) * ��Լ��λ * ��Լ���� �Ϲ�Ȩ���ֵ���Ȩ��ֵ��MAX((��Ȩ��-��ļ�), 0) * ��Լ��λ * ��Լ����
  LONGLONG        llCombedQty;                // ����Ϻ�Լ���� ������ϵ���Ȩ��Լ�ֲ�����
  char            szCostPrice[22 + 1];        // ��Լ�ɱ��� 
};

//-------------------------------10313002:���ñ��ҹɷݲ�ѯ------------------------------------
struct MATRADEAPI CReqOptExpendableCvdStkField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szTrdacct[10 + 1];          // �����˻�
  char          szOptCode[8 + 1];           // ֤ȯ����
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char          szQryPos[32 + 1];           // ��λ��
  int           iQryNum;                    // ��ѯ����
};

struct MATRADEAPI CRspOptExpendableCvdStkField
{
  char          szQryPos[32 + 1];           // ��λ��
  int           iIntOrg;                    // �ڲ�����
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szTrdacct[10 + 1];          // ֤ȯ�˻�
  char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[18+1];         // ��Ȩ��Լ�˻� 2015/8/5
  char          chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char          szStkCode[8 + 1];           // ֤ȯ����
  char          szStkName[16 + 1];          // ֤ȯ����
  char          chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  LONGLONG      llCvdStkPrebln;             // ���ҹɷ��������
  LONGLONG      llCvdStkBln;                // ���ҹɷ����
  LONGLONG      llCvdStkAvl;                // ���ҹɷݿ���
};

//-------------------------------10313003:������Ȩ����ί�в�ѯ------------------------------------
struct MATRADEAPI CReqOptCurrDayOrderField
{
  LONGLONG      llCustCode;                 // �ͻ����� �����ͬʱΪ��
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szTrdacct[10 + 1];          // �����˻�
  char          szOptNum[32 + 1];           // ��Լ����
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szCombStraCode[16 + 1];     // ��ϲ��Դ���  2015/8/5
  char          szOrderId[10 + 1];          // ��ͬ���
  int           iOrderBsn;                  // ί������
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          chOrderStatus;              // ί��״̬
  char          szOwnerType[3 + 1];         // ������������  
  char          chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char          szQryPos[32 + 1];           // ��λ��
  int           iQryNum;                    // ��ѯ����
};

struct MATRADEAPI CRspOptCurrDayOrderField
{
  char          szQryPos[32 + 1];           // ��λ��
  int           iTrdDate;                   // ��������
  int           iOrderDate;                 // ί������
  char          szOrderTime[25 + 1];        // ί��ʱ��
  int           iOrderBsn;                  // ί������
  char          szOrderId[10 + 1];          // ��ͬ���
  char          chOrderStatus;              // ί��״̬
  char          chOrderValidFlag;           // ί����Ч��־
  int           iIntOrg;                    // �ڲ�����
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          chStkex;                    // �����г� �ֵ�[STKEX]
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szTrdacct[10 + 1];          // ֤ȯ�˻�
  char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻� 2015/8/5
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // ������������
  char          szOptNum[16 + 1];           // ��Լ����
  char          szOptCode[32 + 1];          // ��Լ����
  char          szOptName[120 + 1];         // ��Լ���
  char          szCombNum[16 + 1];          // ��ϱ���            2015/8/5       
  char          szCombStraCode[16 + 1];     // ��ϲ��Դ���        2015/8/5
  char          szLeg1Num[16 + 1];          // �ɷ�һ��Լ����      2015/8/5
  char          szLeg2Num[16 + 1];          // �ɷֶ���Լ����      2015/8/5
  char          szLeg3Num[16 + 1];          // �ɷ�����Լ����      2015/8/5
  char          szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ����      2015/8/5
  char          chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char          szOrderPrice[11 + 1];       // ί�м۸� CPRICE4
  LONGLONG      llOrderQty;                 // ί������
  char          szOrderAmt[21 + 1];         // ί�н��
  char          szOrderFrzAmt[21 + 1];      // ί�ж�����
  char          szOrderUfzAmt[21 + 1];      // ί�нⶳ���
  LONGLONG      llOfferQty;                 // �걨����
  int           iOfferStime;                // �걨ʱ��
  LONGLONG      llWithdrawnQty;             // �ѳ�������
  LONGLONG      llMatchedQty;               // �ѳɽ�����
  char          szMatchedAmt[21 + 1];       // �ѳɽ����
  char          chIsWithdraw;               // ������־
  char          chIsWithdrawn;              // �ѳ�����־
  char          chOptUndlCls;               // ���֤ȯ���
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOptUndlName[16 + 1];      // ���֤ȯ����
  LONGLONG      llUndlFrzQty;               // ���ȯί�ж�������
  LONGLONG      llUndlUfzQty;               // ���ȯί�нⶳ����
  LONGLONG      llUndlWthQty;               // ���ȯ�ѳ�������
  char          szOfferRetMsg[64 + 1];      // �걨������Ϣ
  char          szOrderIdEx[64 + 1];        // �ⲿ��ͬ���
  int           iOrderSn;                   // ί�����
  char          szRawOrderId[10 + 1];       // �ⲿ��ͬ���
  char          szMarginPreFrz[21 + 1];     // Ԥռ�ñ�֤�� ����ί��ʱ��дԤ����ı�֤�����������0��
  char          szMarginFrz[21 + 1];        // ռ�ñ�֤�� �����ɽ�ʱ��дʵ�ʶ���ı�֤�����������0��
  char          szMarginPreUfz[21 + 1];     // Ԥ�ⶳ��֤�� ��ƽί��ʱ��дԤ�ⶳ�ı�֤�����������0��
  char          szMarginUfz[21 + 1];        // �ⶳ��֤�� ��ƽ�ɽ�ʱ��дʵ�ʽⶳ�ı�֤�����������0��
};

//-------------------------------10313004:������Ȩ���ճɽ���ѯ------------------------------------
struct MATRADEAPI CReqOptCurrDayFillField
{
  LONGLONG      llCustCode;                 // �ͻ����� �����ͬʱΪ��
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szTrdacct[10 + 1];          // �����˻�
  char          szOptNum[32 + 1];           // ��Լ����
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szCombStraCode[16 + 1];     // ��ϲ��Դ���    2015/8/5
  char          szOrderId[10 + 1];          // ��ͬ���
  int           iOrderBsn;                  // ί������
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // ������������    2015/8/5
  char          chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char          szQryPos[32 + 1];           // ��λ��
  int           iQryNum;                    // ��ѯ����
};

struct MATRADEAPI CRspOptCurrDayFillField
{
  char          szQryPos[32 + 1];           // ��λ��
  int           iTrdDate;                   // ��������
  char          szMatchedTime[8 + 1];       // �ɽ�ʱ��
  int           iOrderDate;                 // ί������
  int           iOrderSn;                   // ί�����
  int           iOrderBsn;                  // ί������
  char          szOrderId[10 + 1];          // ��ͬ���
  int           iIntOrg;                    // �ڲ�����
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          chStkex;                    // �����г� �ֵ�[STKEX]
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szTrdacct[10 + 1];          // ֤ȯ�˻�
  char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[18+1];         // ��Ȩ��Լ�˻�          2015/8/5
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // ������������
  char          szOptNum[16 + 1];           // ��Լ����
  char          szOptCode[32 + 1];          // ��Լ����
  char          szOptName[120 + 1];         // ��Լ���
  char          szCombNum[16+1];            // ��ϱ���              2015/8/5
  char          szCombStraCode[16+1];       // ��ϲ��Դ���          2015/8/5
  char          szLeg1Num[16+1];            // �ɷ�һ��Լ����        2015/8/5
  char          szLeg2Num[16+1];            // �ɷֶ���Լ����        2015/8/5
  char          szLeg3Num[16+1];            // �ɷ�����Լ����        2015/8/5
  char          szLeg4Num[16+1];            // �ɷ��ĺ�Լ����        2015/8/5
  char          chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char          chOptUndlCls;               // ���֤ȯ���
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOptUndlName[16 + 1];      // ���֤ȯ����
  char          szOrderPrice[11 + 1];       // ί�м۸� CPRICE4
  LONGLONG      llOrderQty;                 // ί������
  char          szOrderAmt[21 + 1];         // ί�н��
  char          szOrderFrzAmt[21 + 1];      // ί�ж�����
  char          chIsWithdraw;               // ������־
  char          chMatchedType;              // �ɽ�����
  char          szMatchedSn[32 + 1];        // �ɽ����
  char          szMatchedPrice[11 + 1];     // �ɽ��۸� CPRICE4
  LONGLONG      llMatchedQty;               // �ѳɽ�����
  char          szMatchedAmt[21 + 1];       // �ѳɽ����
  char          szOrderIdEx[10 + 1];        // �ⲿ��ͬ���
  char          szMarginPreFrz[21 + 1];     // Ԥռ�ñ�֤�� ����ί��ʱ��дԤ����ı�֤�����������0��
  char          szMarginFrz[21 + 1];        // ռ�ñ�֤�� �����ɽ�ʱ��дʵ�ʶ���ı�֤�����������0��
  char          szMarginPreUfz[21 + 1];     // Ԥ�ⶳ��֤�� ��ƽί��ʱ��дԤ�ⶳ�ı�֤�����������0��
  char          szMarginUfz[21 + 1];        // �ⶳ��֤�� ��ƽ�ɽ�ʱ��дʵ�ʽⶳ�ı�֤�����������0��
  char          szMatchedFee[21 + 1];       // �ɽ�����        2015/8/5    
};

//-------------------------------10313005:������Ȩ��Լ�˻���ѯ------------------------------------
struct MATRADEAPI CReqOptAcctField
{
  LONGLONG        llCustCode;                 // �ͻ����� ����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptAcctField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[10 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻� 
  char            chOptTrdacctLvl;            // ��Ȩ�˻����� 
  int             iIntOrg;                    // �ڲ����� 
  int             iTrdacctSn;                 // �˻���� 
  char            szTrdacctExid[10 + 1];      // �����˻� 
  char            chTrdacctStatus;            // �˻�״̬ 
  char            chTregStatus;               // ָ��״̬ �ֵ�[TREG_STATUS]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szIdType[2 + 1];            // ֤������ 
  char            szIdCode[48 + 1];           // ֤������ 
  char            szCustName[32 + 1];         // �ͻ����� 
  //int             iOptMarginSn;               // ��֤�����ģ���� 
  char            szQuotaVal[21 + 1];         // ������ 
  char            szQuotaValUsed[21 + 1];     // ռ�������� 
  char            chOpFlag;                   // ������־ 
  LONGLONG        llDailyEnqryCount;          // ����ѯ�۴��� 
};

//-------------------------------10313006:������Ȩ�ɳ�ί�в�ѯ------------------------------------
struct MATRADEAPI CReqOptCanWithdrawOrderField
{
  LONGLONG      llCustCode;                 // �ͻ����� �����ͬʱΪ��
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szTrdacct[10 + 1];          // �����˻�
  char          szOptNum[32 + 1];           // ��Լ����
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOrderId[10 + 1];          // ��ͬ���
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char          szQryPos[32 + 1];           // ��λ��
  int           iQryNum;                    // ��ѯ����
};

struct MATRADEAPI CRspOptCanWithdrawOrderField
{
  char          szQryPos[32 + 1];           // ��λ��
  int           iTrdDate;                   // ��������
  int           iOrderDate;                 // ί������
  char          szOrderTime[25 + 1];        // ί��ʱ��
  int           iOrderBsn;                  // ί������
  char          szOrderId[10 + 1];          // ��ͬ���
  char          chOrderStatus;              // ί��״̬
  char          chOrderValidFlag;           // ί����Ч��־
  int           iIntOrg;                    // �ڲ�����
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          chStkex;                    // �����г� �ֵ�[STKEX]
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szTrdacct[10 + 1];          // ֤ȯ�˻�
  char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻�           2015/8/5
  int           iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int           iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // ������������
  char          szOptNum[16 + 1];           // ��Լ����
  char          szOptCode[32 + 1];          // ��Լ����
  char          szOptName[120 + 1];         // ��Լ���
  char          chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char          szOrderPrice[11 + 1];       // ί�м۸� CPRICE4
  LONGLONG      llOrderQty;                 // ί������
  char          szOrderAmt[21 + 1];         // ί�н��
  char          szOrderFrzAmt[21 + 1];      // ί�ж�����
  char          szOrderUfzAmt[21 + 1];      // ί�нⶳ���
  LONGLONG      llOfferQty;                 // �걨����
  int           iOfferStime;                // �걨ʱ��
  LONGLONG      llWithdrawnQty;             // �ѳ�������
  LONGLONG      llMatchedQty;               // �ѳɽ�����
  char          szMatchedAmt[21 + 1];       // �ѳɽ����
  char          chIsWithdraw;               // ������־
  char          chIsWithdrawn;              // �ѳ�����־
  char          chOptUndlCls;               // ���֤ȯ���
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOptUndlName[16 + 1];      // ���֤ȯ����
  LONGLONG      llUndlFrzQty;               // ���ȯί�ж�������
  LONGLONG      llUndlUfzQty;               // ���ȯί�нⶳ����
  LONGLONG      llUndlWthQty;               // ���ȯ�ѳ�������
  char          szMarginPreFrz[21 + 1];     // Ԥռ�ñ�֤�� ����ί��ʱ��дԤ����ı�֤�����������0��
  char          szMarginFrz[21 + 1];        // ռ�ñ�֤�� �����ɽ�ʱ��дʵ�ʶ���ı�֤�����������0��
  char          szMarginPreUfz[21 + 1];     // Ԥ�ⶳ��֤�� ��ƽί��ʱ��дԤ�ⶳ�ı�֤�����������0��
  char          szMarginUfz[21 + 1];        // �ⶳ��֤�� ��ƽ�ɽ�ʱ��дʵ�ʽⶳ�ı�֤�����������0��
};

//-------------------------------10102509:�ͻ��ʵ���ƥ���Ʒ��Ϣ��ѯ------------------------------------
struct MATRADEAPI CReqOptQryCustAppropriateMatchInfoField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
};

struct MATRADEAPI CRspOptQryCustAppropriateMatchInfoField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  char          szStkbd[2 + 1];             // ���װ��
  char          szTrdacct[10 + 1];          // ֤ȯ�˻�
  char          szStkBizes[512 + 1];        // ֤ȯҵ��(����)
  char          szStkBizActions[512 + 1];   // ֤ȯҵ����Ϊ(����)
};

//-------------------------------10102510:�ͻ��ʵ�����Ϣ��ѯ------------------------------------
struct MATRADEAPI CReqOptQryCustAppropriateInfoField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
};

struct MATRADEAPI CRspOptQryCustAppropriateInfoField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  char          chInvestorType;             // Ͷ��������
  int           iProfInvestorExpDate;       // רҵͶ����ʧЧ����
  char          chLowestRiskFlag;           // ��ͷ��յȼ��ͻ���־
  char          chRatingLvl;                // �����ȼ�
  int           iRatingDate;                // ��������
  int           iRatingExpDate;             // ����ʧЧ����
  char          szInvestPro[32 + 1];        // Ͷ��Ʒ��
  char          chInvestLmt;                // Ͷ������
  char          chExpectIncome;             // Ԥ������
  char          chIsSign;                   // �Ƿ�ǩ����վ�ʾ��
};

//-------------------------------10102511:�ͻ����վ�ʾ��ǩ��------------------------------------
struct MATRADEAPI CReqOptCustRiskSignField
{
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  char          chIsSign;                   // �Ƿ�ǩ����վ�ʾ��
  char          szRemark[128 + 1];          // ��ע
};

struct MATRADEAPI CRspOptCustRiskSignField
{
};

//-------------------------------00102023:�ɽ��ر�����------------------------------------
// �ɽ���Ϣ
struct MATRADEAPI CRtnOptOrderFillField
{
  int           iMatchedDate;               // �ɽ�����
  char          szMatchedTime[8 + 1];       // �ɽ�ʱ��
  char          chStkex;                    // �����г� �ֵ�[STKEX]
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szMatchedSn[32 + 1];        // �ɽ����
  char          szStkCode[8 + 1];           // ֤ȯ����
  char          szOrderId[10 + 1];          // ��ͬ���
  char          szTrdacct[16 + 1];          // �����˻�
  LONGLONG      llMatchedQty;               // ���γɽ����� �����ɽ�ʱ��Ϊ����
  char          szMatchedPrice[11 + 1];     // ���γɽ��۸�
  char          szMatchedAmt[21 + 1];       // �ɽ����
  char          szRltSettAmt[21 + 1];       // ʵʱ������
  char          szFundAvl[21 + 1];          // �ʽ���ý��ɽ���
  LONGLONG      llStkAvl;                   // ֤ȯ�����������ɽ���
  char          chIsWithdraw;               // ������־ �ֵ�[IS_WITHDRAW]
  int           iOrderBsn;                  // ί������
  int           iCuacctSn;                  // �˻����
  char          chMatchedType;              // �ɽ����� ��0��:�ڲ������ɽ� ��1��:�Ƿ�ί�г����ɽ� ��2��:�������ɽ��������������ɽ�����ί�гɽ���
  int           iStkBiz;                    // ֤ȯҵ��
  int           iStkBizAction;              // ҵ����Ϊ
  char          szOfferRetMsg[64 + 1];      // �걨��Ϣ �������ϵ�ʱ���طϵ�ԭ��
  char          szTotalMatchedAmt[21 + 1];  // �ۼƳɽ����
  char          szOptBcostRltPrice[21 + 1]; // ʵʱ�ɱ���
  char          szRemark1[32 + 1];          // Ԥ���ֶ�1
  char          szRemark2[32 + 1];          // Ԥ���ֶ�2
  char          szRemark3[32 + 1];          // Ԥ���ֶ�3
  char          szRemark4[32 + 1];          // Ԥ���ֶ�4
  char          szRemark5[32 + 1];          // Ԥ���ֶ�5
  char          szRemark6[32 + 1];          // Ԥ���ֶ�6
  char          szRemark7[32 + 1];          // Ԥ���ֶ�7
  char          szRemark8[32 + 1];          // Ԥ���ֶ�8
  char          szRemark9[32 + 1];          // Ԥ���ֶ�9
  char          szRemarkA[32 + 1];          // Ԥ���ֶ�A
  char          szOrderNo[10 + 1];          // ί�б��
};

// ί����Ϣ
struct MATRADEAPI CRtnOptOrderField
{
  char          chStkex;                    // �����г� �ֵ�[STKEX]
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int           iOrderDate;                 // ί������
  char          szStkCode[8 + 1];           // ֤ȯ����
  int           iOrderBsn;                  // ί������
  char          szOrderId[10 + 1];          // ��ͬ���
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iCuacctSn;                  // �˻����
  char          szTrdacct[16 + 1];          // �����˻�
  char          szOrderPrice[11 + 1];       // ί�м۸�
  LONGLONG      llOrderQty;                 // ί������
  char          szOrderAmt[21 + 1];         // ί�н��
  char          szOrderFrzAmt[21 + 1];      // ί�ж�����
  char          chOrderStatus;              // ί��״̬ �ֵ�[ORDER_STATUS] ���γɽ���ί�е�״̬
  int           iStkBiz;                    // ֤ȯҵ��
  int           iStkBizAction;              // ֤ȯҵ����Ϊ
  LONGLONG      llWithdrawnQty;             // �ѳ�������
  LONGLONG      llTotalMatchedQty;          // �ۼƳɽ�����
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
};

// ��Լ��Ϣ
struct MATRADEAPI CRtnOptContractField
{
  char          chStkex;                    // �����г� �ֵ�[STKEX]
  char          szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char          szStkCode[8 + 1];           // ֤ȯ����
  char          szTrdacct[16 + 1];          // �����˻�
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iStkBiz;                    // ֤ȯҵ��
  int           iStkBizAction;              // ֤ȯҵ����Ϊ
  char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻�
  char          szOwnerType[3 + 1];         // ������������
  char          szOptCode[32 + 1];          // ��Լ����
  char          szOptName[120 + 1];         // ��Լ���
  char          chCurrency;                 // ���Ҵ���
  char          chOptUndlCls;               // ���֤ȯ���
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOptUndlName[16 + 1];      // ���֤ȯ����
  char          szMarginPreFrz[21 + 1];     // Ԥռ�ñ�֤��  ����ί��ʱ��дԤ����ı�֤�����������0��
  char          szMarginFrz[21 + 1];        // ռ�ñ�֤��    �����ɽ�ʱ��дʵ�ʶ���ı�֤�����������0��
  char          szMarginPreUfz[21 + 1];     // Ԥ�ͷű�֤��  ��ƽί��ʱ��дԤ�ⶳ�ı�֤�����������0��
  char          szMarginUfz[21 + 1];        // �ͷű�֤��    ��ƽ�ɽ�ʱ��дʵ�ʽⶳ�ı�֤�����������0��
  char          szCombNum[16 + 1];          // ��ϱ���       ������ϡ�������ʱ��д������������
  char          szCombStraCode[16 + 1];     // ��ϲ��Դ���    ������ϡ�������ʱ��д������������
  char          szLeg1Num[16 + 1];          // �ɷ�һ��Լ����  ������ϡ������ϡ���Ȩָ��ϲ��걨ʱ��д������������
  char          szLeg2Num[16 + 1];          // �ɷֶ���Լ����  ������ϡ������ϡ���Ȩָ��ϲ��걨ʱ��д������������
  char          szLeg3Num[16 + 1];          // �ɷ�����Լ����  ������ϡ�������ʱ��д������������
  char          szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ����  ������ϡ�������ʱ��д������������
};

//-------------------------------00102024:ȷ�ϻر�����------------------------------------
struct MATRADEAPI CRtnOptOrderConfirmField
{
  char          chStkex;                    // �����г�
  char          szStkCode[8 + 1];           // ֤ȯ����
  char          szOrderId[10 + 1];          // ��ͬ���
  char          szTrdacct[16 + 1];          // �����˻�
  char          chIsWithdraw;               // ������־
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iOrderBsn;                  // ί������
  int           iCuacctSn;                  // �˻����
  char          szStkbd[2 + 1];             // ���װ��
  char          chOrderStatus;              // ί��״̬
  int           iStkBiz;                    // ֤ȯҵ��
  int           iStkBizAction;              // ҵ����Ϊ
  int           iOrderDate;                 // ί������
  int           iOrderSn;                   // ί�����
  int           iIntOrg;                    // �ڲ�����
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szOrderPrice[21 + 1];       // ί�м۸�
  LONGLONG      llOrderQty;                 // ί������
  char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char          szOptTrdacct[18 + 1];       // ��Ȩ��Լ�˻�
  char          szOptCode[32 + 1];          // ��Լ����
  char          szOptName[120 + 1];         // ��Լ���
  char          chCurrency;                 // ���Ҵ���
  char          chOptUndlCls;               // ���֤ȯ���
  char          szOptUndlCode[8 + 1];       // ���֤ȯ����
  char          szOptUndlName[16 + 1];      // ���֤ȯ����
  char          szCombNum[16 + 1];          // ��ϱ���       ������ϡ�������ʱ��д������������
  char          szCombStraCode[16 + 1];     // ��ϲ��Դ���    ������ϡ�������ʱ��д������������
  char          szLeg1Num[16 + 1];          // �ɷ�һ��Լ����  ������ϡ������ϡ���Ȩָ��ϲ��걨ʱ��д������������
  char          szLeg2Num[16 + 1];          // �ɷֶ���Լ����  ������ϡ������ϡ���Ȩָ��ϲ��걨ʱ��д������������
  char          szLeg3Num[16 + 1];          // �ɷ�����Լ����  ������ϡ�������ʱ��д������������
  char          szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ����  ������ϡ�������ʱ��д������������
  char          szRemark1[32 + 1];          // Ԥ���ֶ�1
  char          szRemark2[32 + 1];          // Ԥ���ֶ�2
  char          szRemark3[32 + 1];          // Ԥ���ֶ�3
  char          szRemark4[32 + 1];          // Ԥ���ֶ�4
  char          szRemark5[32 + 1];          // Ԥ���ֶ�5
  char          szRemark6[32 + 1];          // Ԥ���ֶ�6
  char          szRemark7[32 + 1];          // Ԥ���ֶ�7
  char          szRemark8[32 + 1];          // Ԥ���ֶ�8
  char          szRemark9[32 + 1];          // Ԥ���ֶ�9
  char          szRemarkA[32 + 1];          // Ԥ���ֶ�A
};

//��ϲ����ļ���ѯ    2015/8/5
struct MATRADEAPI CReqOptCombStraField
{
  char szStkbd[2+1];        //���װ��       625
  char szCombStraCode[16+1];//��ϲ��Դ���     8928
  char chQueryFlag;      //��ѯ����       8987
  char szQueryPos[32+1];    //��λ��         8991
  int  iQueryNum;          //��ѯ����       8992
    
};
struct MATRADEAPI CRspOptCombStraField
{
  char szQueryPos[32+1];      //��λ��               8991
  char chStkex;          // �����г�               207
  char szStkbd[2+1];        // ���װ��               625
  char szCombStraCode[16+1];    // ��ϲ��Դ���             8928
  char szCombStraName[32+1];    // ��ϲ�������           8936
  char chSameExpDate;        // �������Ƿ���ͬ           9095
  char chSameUndl;        //����Ƿ���ͬ             9096
  char chAutoSplitDay;      // �Զ���������           326
  int  iLegCnt;          // �ɷֺ�Լ����             9090
  char  chLeg1Type;        // �ɷ�һ��Լ����           8930
  char  chLeg1Side;        //�ɷ�һ�ֲַ���           8931
  int iLeg1ExePrcSeq;          //�ɷ�һ��Ȩ�۸ߵ�˳��         8932
  int iLeg1ExpDateSeq;      // �ɷ�һ��Լ������˳��         8933
  int iLeg1PosiUnit;          // �ɷ�һ���ݳֲ�����         8934
  char chLeg2Type;        // �ɷֶ���Լ����           8935
  char chLeg2Side;        //�ɷֶ��ֲַ���           9080
  int iLeg2ExePrcSeq;         //�ɷֶ���Ȩ�۸ߵ�˳��         9081
  int iLeg2ExpDateSeq;      // �ɷֶ���Լ������˳��         8929
  int iLeg2PosiUnit;         // �ɷֶ����ݳֲ�����         9083
  char chLeg3Type;        // �ɷ�����Լ����           9084
  char chLeg3Side;        //�ɷ����ֲַ���          9085
  int iLeg3ExePrcSeq;         //�ɷ�����Ȩ�۸ߵ�˳��         9086
  int iLeg3ExpDateSeq;      // �ɷ�����Լ������˳��         9087
  int iLeg3PosiUnit;          // �ɷ������ݳֲ�����         9088
  char  chLeg4Type;        // �ɷ��ĺ�Լ����           9089
  char  chLeg4Side;        // �ɷ��ĳֲַ���           9091
  int iLeg4ExePrcSeq;          // �ɷ�����Ȩ�۸ߵ�˳��         9092
  int iLeg4ExpDateSeq;      // �ɷ��ĺ�Լ������˳��         9093
  int iLeg4PosiUnit;          // �ɷ��ĵ��ݳֲ�����         9094
    

};
// ��ϱ�֤��ί��  2015/8/5
struct MATRADEAPI CReqOptCombStraOrderField
{
  LONGLONG  llCustCode  ;    //�ͻ�����          8902
  LONGLONG  llCuacctCode  ;  //�ʲ��˻�          8920
  int    iIntOrg;        //�ڲ�����          8911
  char  szStkbd[2+1];      //���װ��          625
  char  szTrdacct[10+1];    //֤ȯ�˻�          448
  char  szCombNum[16+1];    //��ϱ���          8900
  char  szCombStraCode[16+1];  //��ϲ��Դ���        8928
  LONGLONG  llOrderQty;      //ί������          38
  char  szLeg1Num[16+1]  ;    //�ɷ�һ��Լ����      9090
  char  szLeg2Num[16+1]  ;    //�ɷֶ���Լ����      9091
  char  szLeg3Num[16+1]  ;    //�ɷ�����Լ����      9092
  char  szLeg4Num[16+1];    //�ɷ��ĺ�Լ����      9094
  int    iStkBiz  ;        //֤ȯҵ��          8842
  int    iStkBizAction;      //֤ȯҵ����Ϊ        40
  char  szStkpbu[8+1];      //���׵�Ԫ          8843
  int    iOrderBsn  ;      //ί������          66
  char  szClientInfo[256+1];  //�ն���Ϣ          9039
  char  chSecurityLevel;    //��ȫ�ֶ�          9080
  char  szSecurityInfo[256+1];  //��ȫ��Ϣ          9081
  char  szOrderIdEx[64+1];    //�ⲿ��ͬ���        9093
  char  chEncryptType;      //���ܷ�ʽ          9085
  char    szEncryptKey[32+1];    //��������          9086
  
};
struct MATRADEAPI CRspOptCombStraOrderField
{
  int     iOrderBsn;            //ί������       66
  char  szOrderId[10+1];        //��ͬ���       11
  LONGLONG  llCuacctCode;        //�ʲ��˻�       8920
  LONGLONG  llOrderQty;          //ί������       38
  char  szStkpbu[8+1];          //���׵�Ԫ       8843
  char  szStkbd[2+1];          //���װ��       625
  char  szTrdacct[10+1];        //֤ȯ�˻�       448
  char  szSubAcctcode[8+1];        //֤ȯ�˻��ӱ���    9099
  char  szOptTrdacct[18+1];        //��Ȩ��Լ�˻�     9098
  char  szCombNum[16+1];        //��ϱ���       8900
  char  szCombStraCode[16+1];      //��ϲ��Դ���     8928
  char  szLeg1Num[16+1];        //�ɷ�һ��Լ����    9090
  char  szLeg2Num[16+1];        //�ɷֶ���Լ����    9091
  char  szLeg3Num[16+1];        //�ɷ�����Լ����    9092
  char  szLeg4Num[16+1];        //�ɷ��ĺ�Լ����    9094
  int    iStkBiz;            //֤ȯҵ��       8842
  int    iStkBizAction;          //֤ȯҵ����Ϊ     40
  char  szOrderIdEx[64+1];        //�ⲿ��ͬ���     9093
      

};

//������ϳֲ�  2015/8/5
struct MATRADEAPI CReqOptCombStraPosField
{
  LONGLONG    llCustCode;            //�ͻ�����  CUST_CODE  BIGINT  X  8902
  LONGLONG  llCuacctCode;          //�ʲ��˻�  CUACCT_CODE  BIGINT  X  8920
  char    szStkbd[2+1];          //���װ��  STKBD  CHAR(2)  X  625
  char    szTrdacct[10+1];        //�����˻�  TRDACCT  VARCHAR(10)  X  448
  char    szCombStraCode[16+1];      //��ϲ��Դ���  COMB_STRA_CODE  VARCHAR(16)  X  8928
  char    szLeg1Num[16+1];        //�ɷ�һ��Լ����  LEG1_NUM  VARCHAR(16)  X  9090
  char    szLeg2Num[16+1];        //�ɷֶ���Լ����  LEG2_NUM  VARCHAR(16)  X  9091
  char    szLeg3Num[16+1];        //�ɷ�����Լ����  LEG3_NUM  VARCHAR(16)  X  9092
  char    szLeg4Num[16+1];        //�ɷ��ĺ�Լ����  LEG4_NUM  VARCHAR(16)  X  9094
  char    chQueryFlag;          //��ѯ����  QUERY_FLAG  CHAR(1)  X  9080
  char    szQueryPos[32+1];        //��λ��  QRY_POS  VARCHAR(32)  X  8991
  int      iQueryNum;            //��ѯ����  QRY_NUM  INTEGER  X  8992
  
  
};
struct MATRADEAPI CRspOptCombStraPosField
{
  char    szQueryPos[32+1];      //��λ��           8991
  LONGLONG  llCustCode;          //�ͻ�����        8902
  LONGLONG  llCuacctCode;        //�ʲ��˻�        8920
  int      iIntOrg;          //�ڲ�����        8911
  char    chStkex;          //�����г�        207
  char    szStkbd[2+1];        //���װ��        625
  char    szStkpbu[8+1];        //���׵�Ԫ        8843
  char    szTrdacct[10+1];      //֤ȯ�˻�        448
  char    szSubacctCode[8+1];      //֤ȯ�˻��ӱ���       9099
  char    szOptTrdacct[18+1];      //��Ȩ��Լ�˻�        9098
  char    chCurrency;          //���Ҵ���        15
  char    szCombStraCode[16+1];    //��ϲ��Դ���        8928
  char    szCombStraName[32+1];    //��ϲ�������        8936
  LONGLONG  llCombPrebln;        //��ϳֲ��������    8860
  LONGLONG  llCombBln;          //��ϳֲ����        8861
  LONGLONG  llCombAvl;          //��ϳֲֿ���        8869
  LONGLONG  llCombTrdFrz;        //��ϳֲֽ��׶���    8872
  LONGLONG  llCombTrdUfz;        //��ϳֲֽ��׽ⶳ    8873
  LONGLONG  llCombTrdOtd;        //��ϳֲֽ�����;    8874
  LONGLONG  llCombTrdBln;        //��ϳֲֽ�������    8867
  LONGLONG  llCombClrFrz;        //��ϳֲ����㶳��    8887
  LONGLONG  llCombClrUfz;        //��ϳֲ�����ⶳ    8888
  LONGLONG  llCombClrOtd;        //��ϳֲ�������;    8889
  LONGLONG  llSecuCombMargin;      //���ռ�ñ�֤��       8871
  int      iLegCnt;          //�ɷֺ�Լ����        8875
  char    szLeg1Num[16+1];      //�ɷ�һ��Լ����       9090
  char    chLeg1Type;          //�ɷ�һ��Լ����       8877
  char    chLeg1Side;          //�ɷ�һ�ֲַ���       8878
  LONGLONG  llLeg1Posi;          //�ɷ�һ�ֲ�����       8879
  char    szLeg2Num[16+1];      //�ɷֶ���Լ����       9091
  char    chLeg2Type;          //�ɷֶ���Լ����       9084
  char    chLeg2Side;          //�ɷֶ��ֲַ���       9086
  LONGLONG  llLeg2Posi;          //�ɷֶ��ֲ�����       9087
  char    szLeg3Num[16+1];      //�ɷ�����Լ����       9092
  char    chLeg3Type;          //�ɷ�����Լ����       9093
  char    chLeg3Side;          //�ɷ����ֲַ���       9089
  LONGLONG  llLeg3Posi;          //�ɷ����ֲ�����       9095
  char    szLeg4Num[16+1];      //�ɷ��ĺ�Լ����       9094
  char    chLeg4Type;          //�ɷ��ĺ�Լ����       9096
  char    chLeg4Side;          //�ɷ��ĳֲַ���       9097
  LONGLONG  llLeg4Posi;          //�ɷ��ĳֲ�����       8876
    
    
};

//������ϳֲ���ϸ
struct MATRADEAPI CReqOptCombStraPosDetailField
{
  LONGLONG    llCustCode;             //�ͻ�����  CUST_CODE  BIGINT  X  8902
  LONGLONG  llCuacctCode;           //�ʲ��˻�  CUACCT_CODE  BIGINT  X  8920
  char    szStkbd[2+1];           //���װ��  STKBD  CHAR(2)  X  625
  char    szTrdacct[10+1];         //�����˻�  TRDACCT  VARCHAR(10)  X  448
  char    szCombNum[16+1];         //��ϱ���  COMB_NUM  VARCHAR(16)  X  8900
  char    szCombStraCode[16+1];       //��ϲ��Դ���  COMB_STRA_CODE  VARCHAR(16)  X  8928
  char    szLeg1Num[16+1];         //�ɷ�һ��Լ����  LEG1_NUM  VARCHAR(16)  X  9090
  char    szLeg2Num[16+1];         //�ɷֶ���Լ����  LEG2_NUM  VARCHAR(16)  X  9091
  char    szLeg3Num[16+1];         //�ɷ�����Լ����  LEG3_NUM  VARCHAR(16)  X  9092
  char    szLeg4Num[16+1];         //�ɷ��ĺ�Լ����  LEG4_NUM  VARCHAR(16)  X  9094
  char    chQueryFlag;           //��ѯ����  QUERY_FLAG  CHAR(1)  X  9080
  char    szQueryPos[32+1];         //��λ��  QRY_POS  VARCHAR(32)  X  8991
  int      iQueryNum;             //��ѯ����  QRY_NUM  INTEGER  X  8992
  
};
struct MATRADEAPI CRspOptCombStraPosDetailField
{
  char    szQueryPos[32+1];     //��λ��               8991
  LONGLONG  llCustCode;         //�ͻ�����              8902
  LONGLONG  llCuacctCode;       //�ʲ��˻�              8920
  int      iIntOrg;         //�ڲ�����              8911
  char    chStkex;         //�����г�              207
  char    szStkbd[2+1];       //���װ��              625
  char    szStkpbu[8+1];       //���׵�Ԫ              8843
  char    szTrdacct[10+1];     //֤ȯ�˻�              448
  char    szSubacctCode[8+1];     //֤ȯ�˻��ӱ���           9099
  char    szOptTrdacct[18+1];     //��Ȩ��Լ�˻�          9098
  char    chCurrency;         //���Ҵ���              15
  char    szCombNum[16+1];     //��ϱ���              8900
  char    szCombStraCode[16+1];   //��ϲ��Դ���          8928
  char    szCombStraName[32+1];   //��ϲ�������          8936
  LONGLONG  llCombPrebln;       //��ϳֲ��������          8860
  LONGLONG  llCombBln;         //��ϳֲ����          8861
  LONGLONG  llCombAvl;         //��ϳֲֿ���          8869
  LONGLONG  llCombTrdFrz;       //��ϳֲֽ��׶���          8872
  LONGLONG  llCombTrdUfz;       //��ϳֲֽ��׽ⶳ          8873
  LONGLONG  llCombTrdOtd;       //��ϳֲֽ�����;          8874
  LONGLONG  llCombTrdBln;       //��ϳֲֽ�������          8867
  LONGLONG  llCombClrFrz;       //��ϳֲ����㶳��          8887
  LONGLONG  llCombClrUfz;       //��ϳֲ�����ⶳ          8888
  LONGLONG  llCombClrOtd;       //��ϳֲ�������;          8889
  LONGLONG  llSecuCombMargin;     //���ռ�ñ�֤��           8871
  int      iLegCnt;         //�ɷֺ�Լ����          8875
  char    szLeg1Num[16+1];     //�ɷ�һ��Լ����           9090
  char    chLeg1Type;         //�ɷ�һ��Լ����           8877
  char    chLeg1Side;         //�ɷ�һ�ֲַ���           8878
  LONGLONG  llLeg1Posi;         //�ɷ�һ�ֲ�����           8879
  char    szLeg2Num[16+1];     //�ɷֶ���Լ����           9091
  char    chLeg2Type;         //�ɷֶ���Լ����           9084
  char    chLeg2Side;         //�ɷֶ��ֲַ���           9086
  LONGLONG  llLeg2Posi;         //�ɷֶ��ֲ�����           9087
  char    szLeg3Num[16+1];     //�ɷ�����Լ����           9092
  char    chLeg3Type;         //�ɷ�����Լ����           9093
  char    chLeg3Side;         //�ɷ����ֲַ���           9089
  LONGLONG  llLeg3Posi;         //�ɷ����ֲ�����           9095
  char    szLeg4Num[16+1];     //�ɷ��ĺ�Լ����           9094
  char    chLeg4Type;         //�ɷ��ĺ�Լ����           9096
  char    chLeg4Side;         //�ɷ��ĳֲַ���           9097
  LONGLONG  llLeg4Posi;         //�ɷ��ĳֲ�����           8876
  
};

//�ͻ����㵥��ѯ
struct MATRADEAPI CReqOptSettListField
{
  int     iTrdDate;    //  ��������  TRD_DATE  INTEGER  ��
  LONGLONG llCuacctCode;  //  �ʲ��ʺ�  CUACCT_CODE  BIGINT  ��
  LONGLONG llCustCode;  //  �ͻ�����  CUST_CODE  BIGINT  ��
  char     chCurrency;    //  ���Ҵ���  CURRENCY  CHAR(1)  ��

};
struct MATRADEAPI CRspOptSettListField
{
  int      iTrdDate;        //��������  TRD_DATE  INTEGER
  int      iIntOrg;        //�ڲ�����  INT_ORG  SMALLINT
  LONGLONG llCustCode;        //�ͻ�����  CUST_CODE  BIGINT
  char     szCustName[64+1];      //�ͻ�����  CUST_NAME  VARCHAR(64)
  LONGLONG llCuacctCode;        //�ʲ��˻�  CUACCT_CODE  BIGINT
  char     chCurrency;      //���Ҵ���  CURRENCY  CHAR(1)
  char     szFundPrebln[21+1];  //�ڳ����  FUND_PREBLN  CMONEY
  char     szFundBln[21+1];    //��ĩ���  FUND_BLN  CMONEY
  char     szFundAvl[21+1];    //�����ʽ�  FUND_AVL  CMONEY
  char     szFundExeFrz[21+1];  //��Ȩ�ʽ𶳽���  FUND_EXE_FRZ  CMONEY
  char     szFundExeMargin[21+1];  //��Ȩ����ά�ֱ�֤��  FUND_EXE_MARGIN  CMONEY
  char     szFundUsedMargin[21+1];//ռ�ñ�֤��  FUND_USED_MARGIN  CMONEY
  char     szPaylater[21+1];    //�渶�ʽ�  PAYLATER  CMONEY
  char     szExpectPnint[21+1];  //Ԥ�Ƶ��ʷ�Ϣ  EXPECT_PNINT  CMONEY
  char     szRepayPaylater[21+1];  //�黹����  REPAY_PAYLATER  CMONEY
  char     szRepayPnint[21+1];  //�黹��Ϣ  REPAY_PNINT  CMONEY
  char     szRemitPnint[21+1];  //���ⷣϢ  REMIT_PNINT  CMONEY
  char     szInterstIncome[21+1];  //��Ϣ�鱾  INTERST_INCOME  CMONEY
  char     szInterstIncomeTax[21+1];//��Ϣ˰  INTERST_INCOME_TAX  CMONEY
  char     szInAmt[21+1];      //�������  IN_AMT  CMONEY
  char     szOutAmt[21+1];    //���ܳ���  OUT_AMT  CMONEY
  char     szPremiumPay[21+1];  //Ȩ�����ո�  PREMIUM_PAY  CMONEY
  char     szExecPay[21+1];    //��Ȩ�ո�  EXEC_PAY  CMONEY
  char     szSXFFee[21+1];    //������  SXF_FEE  CMONEY
  char     szQsfFee[21+1];    //�����  QSF_FEE  CMONEY
  char     szJsfFee[21+1];    //���ַ�  JSF_FEE  CMONEY
  char     szJysJsfFee[21+1];    //���������ַ�  JYS_JSF_FEE  CMONEY
  char     szXqghFee[21+1];    //��Ȩ������  XQGH_FEE  CMONEY
  char     szXqjsfFee[21+1];    //��Ȩ�����  XQJSF_FEE  CMONEY
  char     szXqsxfFee[21+1];    //��Ȩ������  XQSXF_FEE  CMONEY
  LONGLONG llOptFloatProfit;      //����ӯ��  OPT_FLOAT_PROFIT  BIGINT
  LONGLONG llQuotaValUsed;      //ռ��������  QUOTA_VAL_USED  BIGINT
  LONGLONG llQuotaVal;        //������  QUOTA_VAL  BIGINT
  char     szRightMktVal[21+1];  //Ȩ������ֵ  RIGHT_MKT_VAL  CMONEY
  char     szDuteMktVal[21+1];  //�������ֵ  DUTE_MKT_VAL  CMONEY
  char     szMarginRiskRate[8+1];  //��֤�������  MARGIN_RISK_RATE  CRATE
  char     szAddMarginAmt[21+1];  //Ӧ׷�ӱ�֤��  ADD_MARGIN_AMT  CMONEY
  char     szAddMarginNotice[2048+1];    //׷��֪ͨ����  ADD_MARGIN_NOTICE  VARCHAR(2048)
  char     chConfirmFlag;      //�ͻ�ȷ�ϱ�־  CONFIRM_FLAG  CHAR(1)
  char     szConfirmTime[25+1];  //�ͻ�ȷ��ʱ��  CONFIRM_TIME  DATETIME
};

//�ͻ����㵥ȷ��
struct MATRADEAPI CReqOptSettListConfirmField
{
  int           iTrdDate;                     // ��������
  LONGLONG     llCuacctCode;                 // �ʲ��ʺ�
  LONGLONG     llCustCode;                   // �ͻ�����
  char          chCurrency;          // ���Ҵ���
};
struct MATRADEAPI CRspOptSettListConfirmField
{
  //none
};

//����ת�� Bank Derivative Transfer
struct MATRADEAPI CReqOptBankDeriTransField
{
  LONGLONG llCuacctCode;         //�ʲ��˻�
  char     chCurrency;         //���Ҵ���
  char     szFundPwd[32+1];       //�ʽ�����
  char     szBankCode[4+1];       //���д���
  char     szBankPwd[32+1];       //��������
  char     chTransType;         //ת������
  char     szTransAmt[21+1];       //ת�˽��
  char     chEncryptType;         //���ܷ�ʽ
  char     szEncryptKey[32+1];     //��������
  char     szOpRemark[128+1];       //������ע
  
};
struct MATRADEAPI CRspOptBankDeriTransField
{
  LONGLONG llSerialNo;        //��ˮ���
};

//���ܽ���������ϢBank Derivative Info
struct MATRADEAPI CReqOptBankDeriInfoField
{
  char          chCurrency;          // ���Ҵ���
  int           iExtOrg;                     // �ⲿ����
  int           iIntOrg;                     // �ڲ�����
  char          szCubsbTrdId[2 + 1];          // ����ҵ���ʶ
};
struct MATRADEAPI CRspOptBankDeriInfoField
{
  int           iExtOrg;                      // �ⲿ����
  int           iIntOrg;                      // �ڲ�����
  char          chCurrency;          // ���Ҵ���
  char          szCubsbTrdId[2 + 1];          // ����ҵ���ʶ
  char          chChkId;            // У�����
  char          chChkFundPwd;          // У���ʽ�����
  char          chChkBankPwd;          // У����������
  char          chChkBankAcct;        // У�������˻�
  char          chSendToBank;          // �������б�־
  char          chSendCuacct;          // �����ʲ��˻�
  char          szChannels[32 + 1];           // ��������
  char          szUserTypes[16 + 1];          // �ͻ�����
  char          szCuacctAttrs[8 + 1];         // �ʲ��˻�����
  char          szWeekDays[64 + 1];           // ����Ч��
  int           iBgnStime;                    // ��ʼʱ��
  int           iEndStime;                    // ����ʱ��
};

//�ͻ�ǩԼ���д���
struct MATRADEAPI CReqOptUserBankField
{
  char          chCurrency;             // ���Ҵ���
  LONGLONG      llCuacctCode;           // �ʲ��ʻ�
};
struct MATRADEAPI CRspOptUserBankField
{
  char          chCurrency;             // ���Ҵ���
  int           iExtOrg;                // �ⲿ����
  char          szExtOrgName[32 + 1];   // �ⲿ��������
};

//���˵���Ȩ������ˮStatement Delivery History
struct MATRADEAPI CReqOptStateDeliHisField
{
  LONGLONG     llCustCode;                   // �ͻ�����
  LONGLONG     llCuacctCode;                 // �ʲ��˻�
  char          szStkbd[2 + 1];               // ���װ��
  char          szIntOrges[2048 + 1];         // ��������
  char          szStkBizs[2048 + 1];          // ֤ȯҵ��
  int           iBgnDate;                     // ��ʼ����
  int           iEndDate;                     // ��ֹ����
  int           iQryPos;                      // ��λ��
  int           iQryNum;                      // ��ѯ����
};
struct MATRADEAPI CRspOptStateDeliHisField
{
  int           iQryPos;                      // ��λ��
  LONGLONG      llSerialNo;                   // ��ˮ���
  int           iSettDate;                    // ��������
  int           iOccurDate;                   // ��������
  char          szOccurTime[32 + 1];          // ����ʱ��
  int           iComeInDate;                  // ��������
  int           iIntOrg;                      // �ڲ�����
  char          szTrdacct[10 + 1];            // �����˻�
  char          szSubacctCode[8 + 1];         // �����˻��ӱ���
  char          szOptTrdacct[18 + 1];         // ��Ȩ��Լ�˻�
  char          chTrdacctExcls;               // �����˻����
  char          chStkex;                      // �����г�
  char          szStkbd[2 + 1];               // ���װ��
  LONGLONG      llCustCode;                   // �ͻ�����
  char          szCustName[32 + 1];           // �ͻ�����
  char          chCustCls;                    // �ͻ����
  char          szCustType[2 + 1];            // �ͻ�����
  LONGLONG      llCuacctCode;                 // �ʲ��˻�
  char          chCurrency;                   // ���Ҵ���
  int           iStkBiz;                      // ֤ȯҵ��
  int           iStkBizAction;                // ֤ȯҵ����Ϊ
  char         szBizAmt[21 + 1];              // ҵ����LONGLONG->char[] 2015/10/21
  char         szFundBln[21 + 1];             // �ʽ�ǰ���LONGLONG->char[] 2015/10/21
  char         szFundAvl[21 + 1];             // �ʽ�ǰ����LONGLONG->char[] 2015/10/21
  char         szFundClrFrz[21 + 1];          // ���ᱣ֤��LONGLONG->char[] 2015/10/21
  char         szFundClrUfz[21 + 1];          // �ⶳ��֤��LONGLONG->char[] 2015/10/21
  LONGLONG     llOptUndlQty;                  // ���ձ������
  char         szOptUndlCode[8 + 1];          // ��Ĵ���
  char         szStampDuty[21+1];             // ӡ��˰LONGLONG->char[] 2015/10/21
  char         szHandleFee[21+1];             // ���ַ�LONGLONG->char[] 2015/10/21
  char         szSecuReguFee[21+1];           // ֤�ܷ�LONGLONG->char[] 2015/10/21
  char         szTrdRegFee[21+1];             // ���׹��LONGLONG->char[] 2015/10/21
  char         szClearingFee[21+1];           // �����LONGLONG->char[] 2015/10/21
  char         szVentureFee[21+1];            // ���ս�LONGLONG->char[] 2015/10/21
  char         szTransFee[21+1];              // ������LONGLONG->char[] 2015/10/21
  char         szExeTrinFee[21+1];            // ��Ȩ������LONGLONG->char[] 2015/10/21
  char         szPaidinCom[21+1];             // ��������LONGLONG->char[] 2015/10/21
  char         szFeeAmt[21+1];                // �ܷ���LONGLONG->char[] 2015/10/21
  int           iBizCode;                     // ҵ�����
  char          szBizName[32 + 1];            // ҵ���������
  char          szRemark[128 + 1];            // ��ע��Ϣ
};

//����ת����ˮBank Derivative Tansaction History
struct MATRADEAPI CReqOptBankDeriTransHisField
{
  LONGLONG     llSerialNo;                   // ��ˮ���
  char          chCurrency;          // ���Ҵ���
  LONGLONG     llCuacctCode;                 // �ʲ��ʺ�
  LONGLONG     llCustCode;                   // �ͻ�����
  int           iExtOrg;                     // �ⲿ����
  int           iIntOrg;                     // �ڲ�����
  char          szCubsbTrdIds[64 + 1];        // ����ҵ���ʶ
  char          szCubsbTrdStatuss[16 + 1];    // ����ҵ��״̬
  char          szCancelStatuss[16 + 1];      // ����״̬
};
struct MATRADEAPI CRspOptBankDeriTransHisField
{
  LONGLONG     llSerialNo;                   // ��ˮ���к�
  int           iSettDate;                    // ��������
  int           iOccurDate;                   // ��������
  char          szOccurTime[32 + 1];          // ����ʱ��
  int           iExtOrg;                     // �ⲿ����
  int           iIntOrg;                     // �ڲ�����
  LONGLONG     llCustCode;                   // �ͻ�����
  char          chUserType;          // �û�����
  char          szCustName[32 + 1];           // �ͻ�����
  LONGLONG     llCuacctCode;                 // �ʲ��˻�
  char          chCuacctAttr;          // �ʲ��˻�����
  char          szBankAcct[32 + 1];           // �����˻�
  char          szBdmfAcct[32 + 1];           // ����˻�
  char          szIdType[2 + 1];              // ֤������
  char          szIdCode[32 + 1];             // ֤������
  char          chCurrency;          // ���Ҵ���
  char          szCubsbTrdId[2 + 1];          // ����ҵ���ʶ
  char          szBizAmt[21 + 1];             // ҵ���� LONGLONG->char[] 2015/10/21
  char          szFundBln[21 + 1];            // �ʽ�ǰ��� LONGLONG->char[] 2015/10/21
  char          szAuthData[64 + 1];           // ��֤����
  char          chCubsbTrdStatus;        // ����ҵ��״̬
  char          chCancelStatus;         // ������־
  char          chOriginalSn;          // ԭ��ˮ���
  LONGLONG     llCuacctLogSn;                // ������ˮ���
  int           iRpcSubsysSn;                // ��ϵͳ
  int           iIntMsgCode;                 // �ڲ�������Ϣ����
  char          szIntMsgText[256 + 1];        // �ڲ�������Ϣ����
  LONGLONG     llIntSerialNo;                // �ڲ�������ˮ���
  int           iIntReqStime;                 // �ڲ���������ʱ��
  int           iIntAnsStime;                 // �ڲ�����Ӧ��ʱ��
  char          szExtMsgCode[256 + 1];        // �ⲿ������Ϣ����
  char          szExtMsgText[256 + 1];        // �ⲿ������Ϣ����
  char          szExtSerialNo[32 + 1];        // �ⲿ������ˮ���
  int           iExtReqStime;                 // �ⲿ��������ʱ��
  int           iExtAnsStime;                 // �ⲿ����Ӧ��ʱ��
  int           iCanceledTimes;              // �ѳ�������
  char          szLstCancelTime[256 + 1];     // ������ʱ��
  LONGLONG     llOpUser;                     // �����û�
  char          chOpRole;            // �����û���ɫ
  char          szOpName[32 + 1];             // �����û�����
  int           iOpOrg;                      // ��������
  char          szOpSite[32 + 1];             // ����վ��
  char          chChannel;          // ��������
  char          szRemark[128 + 1];            // ��ע��Ϣ
};

//�������Bank Balance
struct MATRADEAPI CReqOptBankBalaField
{
  char          szReqSn[32 + 1];              // ������ˮ��
  char          chCurrency;                   // ���Ҵ���
  LONGLONG      llCuacctCode;                 // �ʲ��˻�
  int           iExtOrg;                      // �ⲿ����
  char          szFundAuthData[32 + 1];       // �ʽ���֤����
  char          szBankAuthData[32 + 1];       // ������֤����
  char          chEncryptType;                // ���ܷ�ʽ
  char          szEncryptKey[32 + 1];         // ��������
  char          szBankCustNum[200 + 1];       // ���пͻ���
  char          szOpRemark[128 + 1];          // ������ע
};
struct MATRADEAPI CRspOptBankBalaField
{
  LONGLONG     llAnsSn;                      // ��ˮ���
};

//�����޸�
struct MATRADEAPI CReqOptChangePwdField
{
  LONGLONG llUserCode;           //�ͻ�����  USER_CODE  BIGINT  ��  
  char   chUseScope;           //ʹ�÷�Χ  USE_SCOPE  CHAR(1)  ��  
  char     chAuthType;           //��֤����  AUTH_TYPE  VARCHAR(1)  ��  
  char     szOldAuthData[256+1];       //������  OLD_AUTH_DATA  VARCHAR(256)  ��  
  char   szNewAuthData[256+1];       //������  NEW_AUTH_DATA  VARCHAR(256)  ��  
  char     chEncryptType;           //���ܷ�ʽ  ENCRYPT_TYPE  CHAR(1)  ��  
  char     szEncryptKey[32+1];       //��������  ENCRYPT_KEY  VARCHAR(32)  ��  
  
};
struct MATRADEAPI CRspOptChangePwdField
{
  //None
};

//������Ȩ��ʼ��֤�� 10312013
struct MATRADEAPI CReqOptIniMarginField
{
  LONGLONG llCuacctCode;    //�ʲ��˻�  CUACCT_CODE  BIGINT  ��  8920
  char     chCurrency;    //���Ҵ���  CURRENCY  CHAR(1)  ��  15
  char     szStkbd[2+1];    //���װ��  STKBD  CHAR(2)  ��  625
  char     szTrdacct[10+1];  //֤ȯ�˻�  TRDACCT  VARCHAR(10)  ��  448
  char     szOptNum[16+1];  //��Լ����  OPT_NUM  VARCHAR(16)  ��  9082
  LONGLONG llOrderQty;    //ί������  ORDER_QTY  BIGINT  ��  38

};
struct MATRADEAPI CRspOptIniMarginField
{
  LONGLONG llCustCode;      //�ͻ�����  CUST_CODE  BIGINT  8902
  LONGLONG llCuacctCode;      //�ʲ��˻�  CUACCT_CODE  BIGINT  8920
  char     chCurrency;      //���Ҵ���  CURRENCY  CHAR(1)  15
  char     szStkbd[2+1];      //���װ��  STKBD  CHAR(2)  625
  char     szTrdacct[10+1];    //֤ȯ�˻�  TRDACCT  VARCHAR(10)  448
  char     szSubacctCode[8+1];  //֤ȯ�˻��ӱ���  SUBACCT_CODE  VARCHAR(8)  9099
  char     szOptTrdacct[18+1];  //��Ȩ��Լ�˻�  OPT_TRDACCT  VARCHAR(18)  9098
  char     szOptNum[16+1];    //��Լ����  OPT_NUM  VARCHAR(16)  9082
  LONGLONG llOrderQty;      //ί������  ORDER_QTY  BIGINT  38
  char     szSecuMargin[21+1];  //��ʼ��֤��(ȯ��)  SECU_MARGIN  CMONEY  9086
  char     szStkexMargin[21+1];  //��ʼ��֤��(������)  STKEX_MARGIN  CMONEY  9087

};

//��Ȩָ��Ƿ��
struct MATRADEAPI CReqOptExeDebtField
{
  LONGLONG llCustCode;  //�ͻ�����  CUST_CODE  BIGINT  X  8902
  LONGLONG llCuacctCode;  //�ʲ��˻�  CUACCT_CODE  BIGINT  X  8920

};
struct MATRADEAPI CRspOptExeDebtField
{
  LONGLONG llCustCode;    //�ͻ�����  CUST_CODE  BIGINT  8902
  char     szCustName[32+1];  //�ͻ�����  CUST_NAME  VARCHAR(32)  8932
  LONGLONG llCuacctCode;    //�ʲ��˻�  CUACCT_CODE  BIGINT  8920
  char     chCurrency;        //���Ҵ���  CURRENCY  CHAR(1)  15
  int      iIntOrg;      //�ڲ�����  INT_ORG  SMALLINT  8911
  char     szFundPrebln[21+1];  //�ʽ��������  FUND_PREBLN  CMONEY  8860
  char     szFundBln[21+1];    //�ʽ����  FUND_BLN  CMONEY  8984
  char     szFundAvl[21+1];    //�ʽ���ý��  FUND_AVL  CMONEY  8861
  char   szExeDueAmt[21+1];    //��ȨӦ���ʽ�  EXE_DUE_AMT  CMONEY  8862
  char   szExeLackAmt[21+1];  //��ȨǷ�ʽ��  EXE_LACK_AMT  CMONEY  9086
};

//��Ȩָ��Ƿȯ
 struct MATRADEAPI CReqOptExeStkDebtField
 {
  LONGLONG llCustCode;      //�ͻ�����  CUST_CODE  BIGINT  X  8902
  LONGLONG llCuacctCode;      //�ʲ��˻�  CUACCT_CODE  BIGINT  X  8920
  char     szStkbd[2+1];       //���װ��  STKBD  CHAR(2)  ��  625
  char     szTrdacct[10+1];     //�����˻�  TRDACCT  VARCHAR(10)  ��  448
  char     szOptUndlCode[8+1];   //���֤ȯ����  OPT_UNDL_CODE  VARCHAR(8)  X  9090

 };
 struct MATRADEAPI CRspOptExeStkDebtField
 {
  LONGLONG llCustCode;     //�ͻ�����  CUST_CODE  BIGINT  8902
  char     szCustName[32+1];   //�ͻ�����  CUST_NAME  VARCHAR(32)  8932
  LONGLONG llCuacctCode;     //�ʲ��˻�  CUACCT_CODE  BIGINT  8920
  char     chCurrency;       //���Ҵ���  CURRENCY  CHAR(1)  15
  int      iIntOrg;       //�ڲ�����  INT_ORG  SMALLINT  8911
  char     szStkbd[2+1];     //���װ��  STKBD  CHAR(2)  625
  char     szTrdacct[10+1];   //�����˻�  TRDACCT  VARCHAR(10)  448
  char     szOptUndlCode[8+1]; //���֤ȯ����  OPT_UNDL_CODE  VARCHAR(8)  9090
  char     szOptUndlName[16+1];//���֤ȯ����  OPT_UNDL_NAME  VARCHAR(16)  9083
  LONGLONG llOptUndlAvl;      //���֤ȯ����  OPT_UNDL_AVL  BIGINT  9091
  LONGLONG llExeDueQty;      //��ȨӦ������  EXE_DUE_QTY  BIGINT  8862
  LONGLONG llExeLackQty;     //��ȨǷȯ����  EXE_LACK_QTY  BIGINT  9087

 };
 //��Ȩ�걨
 struct MATRADEAPI CReqOptDeclareExeField
 {
 
 };
 struct MATRADEAPI CRspOptDeclareExeField
 {
 
 };
 //�Զ���Ȩ
 struct MATRADEAPI CReqOptAutoExeField
 {
 
 };
 struct MATRADEAPI CRspOptAutoExeField
 {
 
 };
 //�ͻ����ն�10313011
 struct MATRADEAPI CReqOptRiskLvlField
 {
  LONGLONG llCuacctCode;     //�ʲ��˻�  CUACCT_CODE  BIGINT  ��  8920
  char   chCurrency;     //���Ҵ���  CURRENCY  CHAR(1)  ��  15
  char   szStkbd[2+1];        //���װ��  STKBD  CHAR(2)  X  625
  char   chValueFlag;      //ȡֵ��־  VALUE_FLAG  CHAR(1)  X  9080

 };
 struct MATRADEAPI CRspOptRiskLvlField
 {
  LONGLONG llCustCode;          //�ͻ�����  CUST_CODE  BIGINT  8902
  LONGLONG llCuacctCode;          //�ʲ��˻�  CUACCT_CODE  BIGINT  8920
  char     chCurrency;        //���Ҵ���  CURRENCY  CHAR(1)  15
  int      iIntOrg;          //�ڲ�����  INT_ORG  SMALLINT  8911
  char     szFundBln[21+1];      //�ʽ����  FUND_BLN  CMONEY  8984
  char     szDueAddAmt[21+1];      //Ӧ���ʽ�  DUE_ADD_AMT  CMONEY  9095
  char     szFundAvl[21+1];      //�ʽ���ý��  FUND_AVL  CMONEY  8861
  char     szFundAvlRlt[21+1];    //ʵʱ���ý��  FUND_AVL_RLT  CMONEY  8862
  char     szStkexFundAvl[21+1];    //���ý��(������)  STKEX_FUND_AVL  CMONEY  8863
  char     szStkexFundAvlRlt[21+1];   //ʵʱ���ý��(������)  STKEX_FUND_AVL_RLT  CMONEY  8864
  char     szRiskValue[21+1];      //������  RISK_VALUE  CRATE  9085
  char     szRiskValueRlt[21+1];     //ʵʱ������  RISK_VALUE_RLT  CRATE  9086
  char     szStkexRiskValue[21+1];     //������(������)  STKEX_RISK_VALUE  CRATE  9087
  char     szStkexRiskValueRlt[21+1];   //ʵʱ������(������)  STKEX_RISK_VALUE_RLT  CRATE  9089
  char     szMarginUsed[21+1];    //ռ�ñ�֤��  MARGIN_USED  CMONEY  9041
  char     szMarginUsedRlt[21+1];    //ʵʱռ�ñ�֤��  MARGIN_USED_RLT  CMONEY  9042
  char     szStkexMagUsed[21+1];    //ռ�ñ�֤��(������)  STKEX_MAG_USED  CMONEY  9043
  char     szStkexMagUsedRlt[21+1];   //ʵʱռ�ñ�֤��(������)  STKEX_MAG_USED_RLT  CMONEY  9044
  char     szHedgedMargin[21+1];    //�Գ��֤��  HEDGED_MARGIN  CMONEY  9045
  char     szHedgedMarginRlt[21+1];   //�Գ��ʵʱ��֤��  HEDGED_MARGIN_RLT  CMONEY  9046
  char     szStkexHedgedMag[21+1];   //�Գ��֤��(������)  STKEX_HEDGED_MAG  CMONEY  9047
  char     szStkexHedgedMagRlt[21+1];   //�Գ��ʵʱ��֤��(������)  STKEX_HEDGED_MAG_RLT  CMONEY  9048

 };

 //-------------------------------10312011:�������ɷݲ�ѯ------------------------------------
 struct MATRADEAPI CReqOptCanLockStkField
 {
   LONGLONG      llCustCode;                 // �ͻ�����
   LONGLONG      llCuacctCode;               // �ʲ��˻�
   char          szStkbd[2 + 1];             // ���װ��
   char          szTrdacct[10 + 1];          // ֤ȯ�˻�
 };

 struct MATRADEAPI CRspOptCanLockStkField
 {
   char          chStkex;                    // �����г�
   char          szStkbd[2 + 1];             // ���װ��
   char          szTrdacct[10 + 1];          // ֤ȯ�˻�
   char          szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
   char          szStkCode[8 + 1];           // ֤ȯ����
   LONGLONG      llOrderQty;                 // ��ί������
 };

//-------------------------------10312019:��Ʊ��Ȩ��Ȩָ��ϲ��걨ί��--------------------------
struct MATRADEAPI CReqOptCombExeField
{
  LONGLONG        llCustCode;                 // �ͻ�����
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]��Ĭ����100
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  int             iOrderBsn;                  // ί������ 
  char            szClientInfo[256 + 1];      // �ն���Ϣ ����Ӳ�̺�\CPU\��������
  char            chSecurityLevel;            // ��ȫ�ֶ� 0:�ް�ȫ 1:У��Ʊ�� 2:У������
  char            szSecurityInfo[256 + 1];    // ��ȫ��Ϣ ��ȫ�ֶ�ΪУ��Ʊ��������дƱ�� ��ȫ�ֶ�Ϊ����������д������������
  char            szOrderIdEx[64 + 1];        // �ⲿ��ͬ��� �����Ϊ�ⲿ���룬��Ȩϵͳ�����κδ���
  char            chEncryptType;              // ���ܷ�ʽ ����ȫ�ֶ�Ϊ2��У������ʱ��Ϊ������Ρ� ���ܷ�ʽ: ��0��:��֤KBSSϵͳ���ܷ�ʽ ��1��:��֤Win�漯�н��׼��ܷ�ʽ ��2��:��֤Unix�漯�н��׼��ܷ�ʽ ��3��:�ⲿ�ؼ��ܷ�ʽ ��4��:�ⲿ���ܷ�ʽ ˵��������θ��ݽ���ȯ��Ҫ����д��Ĭ��ֵΪ0�� �ⲿ�ؼ��ܷ�ʽ��3����ʹ�÷ǽ�֤���ܺ������������ļ���Ϊ���ĺ�Ȼ��ʹ�ý�֤��˾�ṩ��ͨ�ż��ܺ���kbss_comencrypt��������һ�μ��ܡ� �ⲿ���ܷ�ʽ��4���� ʹ�÷ǽ�֤���ܺ������������ļ���Ϊ���ĺ���Ϊ��Ρ�
  char            szEncryptKey[32 + 1];       // �������� ����ȫ�ֶ�Ϊ2��У������ʱ��Ϊ������Ρ� �������ӿ�����дΪ�ʲ��˻���CUACCT_CODE����ֵ
};

struct MATRADEAPI CRspOptCombExeField
{
  int             iOrderBsn;                  // ί������
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szOrderIdEx[64 + 1];        // �ⲿ��ͬ��� 
};

//-------------------------------10313036:��Ȩָ��ϲ��걨�ɷݺ�Լ��ѯ--------------------------
struct MATRADEAPI CReqOptQryCombExeLegNumField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ �����г����ͣ��Ϻ��г��ɲ���
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� �ͺϲ��걨������һ����Լ����
  char            chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryCombExeLegNumField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg1Code[32 + 1];         // �ɷ�һ��Լ���� 
  char            szLeg1Name[120 + 1];        // �ɷ�һ��Լ��� 
  char            chLeg1Type;                 // �ɷ�һ��Լ���� �ֵ�[OPT_TYPE]
  char            chLeg1Side;                 // �ɷ�һ�ֲַ��� �ֵ�[OPT_SIDE]
  LONGLONG        llLeg1Unit;                 // �ɷ�һ��Լ��λ 
  int             iLeg1EndDate;               // �ɷ�һ������� 
  int             iLeg1ExerciseDate;          // �ɷ�һ��Ȩ��Ȩ�� 
  int             iLeg1ExpireDate;            // �ɷ�һ��Ȩ������ 
  char            szLeg1ExercisePrice[21 + 1];// �ɷ�һ��Ȩ�� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� �ɷ�һ���ɷֶ�����Ȩָ��ϲ��걨
  char            szLeg2Code[32 + 1];         // �ɷֶ���Լ���� 
  char            szLeg2Name[120 + 1];        // �ɷֶ���Լ��� 
  char            chLeg2Type;                 // �ɷֶ���Լ���� �ֵ�[OPT_TYPE]
  char            chLeg2Side;                 // �ɷֶ��ֲַ��� �ֵ�[OPT_SIDE]
  LONGLONG        llLeg2Unit;                 // �ɷֶ���Լ��λ 
  int             iLeg2EndDate;               // �ɷݶ�������� 
  int             iLeg2ExerciseDate;          // �ɷֶ���Ȩ��Ȩ�� 
  int             iLeg2ExpireDate;            // �ɷֶ���Ȩ������ 
  char            szLeg2ExercisePrice[21 + 1];// �ɷֶ���Ȩ�� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szOptUndlName[16 + 1];      // ���֤ȯ���� 
};

//-------------------------------10313027:��Ʊ��Ȩ����ί�в�ѯ[�Ż�]--------------------------
struct MATRADEAPI CReqOptQryCurrDayOrderExField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char            szQryPos[32 + 1];           // ��λ�� ��ҳ��ѯ�����ֶβ���ֵ�� ��ҳ��ѯ���ϴβ�ѯ��������һ����¼��QRY_POS
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryCurrDayOrderExField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szOrderTime[25 + 1];        // ί��ʱ�� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            chOrderStatus;              // ί��״̬ 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  int             iStkBiz;                    // ֤ȯҵ�� 
  int             iStkBizAction;              // ֤ȯҵ����Ϊ 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            szCombNum[16 + 1];          // ��ϱ��� 
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 
  char            szLeg3Num[16 + 1];          // �ɷ�����Լ���� 
  char            szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  LONGLONG        llWithdrawnQty;             // �ѳ������� 
  LONGLONG        llMatchedQty;               // �ѳɽ����� 
  char            chIsWithdraw;               // ������־ 
  char            chIsWithdrawn;              // �ѳ�����־ 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
};

//-------------------------------10313028:��Ʊ��Ȩ���ճɽ���ѯ[�Ż�]--------------------------
struct MATRADEAPI CReqOptQryCurrDayFillExField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char            szQryPos[32 + 1];           // ��λ�� ��ҳ��ѯ�����ֶβ���ֵ�� ��ҳ��ѯ���ϴβ�ѯ��������һ����¼��QRY_POS
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryCurrDayFillExField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  char            szMatchedTime[8 + 1];       // �ɽ�ʱ�� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  int             iStkBiz;                    // ֤ȯҵ�� 
  int             iStkBizAction;              // ֤ȯҵ����Ϊ 
  char            szOwnerType[3 + 1];         // ������������ 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            szCombNum[16 + 1];          // ��ϱ��� 
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 
  char            szLeg3Num[16 + 1];          // �ɷ�����Լ���� 
  char            szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ���� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            chIsWithdraw;               // ������־ 
  char            chMatchedType;              // �ɽ����� 
  char            szMatchedSn[32 + 1];        // �ɽ���� 
  char            szMatchedPrice[21 + 1];     // �ɽ��۸� 
  char            szMatchedQty[21 + 1];       // �ɽ����� 
  char            szMatchedAmt[21 + 1];       // �ɽ���� 
  char            szMatchedFee[21 + 1];       // �ɽ����� 
};

//-------------------------------10313032:�����ʽ��ѯ[�Ż�]--------------------------
struct MATRADEAPI CReqOptQryExpendableFundExField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
};

struct MATRADEAPI CRspOptQryExpendableFundExField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szFundPrebln[21 + 1];       // �ʽ�������� 
  char            szFundBln[21 + 1];          // �ʽ���� 
  char            szFundAvl[21 + 1];          // �ʽ���ý�� 
  char            szFundTrdBln[21 + 1];       // �ʽ��������� 
  char            chFundStatus;               // �ʽ�״̬ �ֵ�[FUND_STATUS]
};

//-------------------------------10313033:��ȡ�ʽ��ѯ[�Ż�]--------------------------
struct MATRADEAPI CReqOptQryDrawFundExField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
};

struct MATRADEAPI CRspOptQryDrawFundExField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szFundBln[21 + 1];          // �ʽ���� 
  char            szFundAvl[21 + 1];          // �ʽ���ý�� 
  char            szFundDraw[21 + 1];         // �ʽ��ȡ��� 
};

//-------------------------------10313034:���ú�Լ�ʲ���ѯ[�Ż�]--------------------------
struct MATRADEAPI CReqOptQryExpendableCuExField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryExpendableCuExField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            chOptType;                  // ��Լ���� �ֵ�[OPT_TYPE]
  char            chOptSide;                  // �ֲַ��� 
  char            chOptCvdFlag;               // ���ұ�־ 0-�Ǳ��Һ�Լ 1-���Һ�Լ
  LONGLONG        llOptPrebln;                // ��Լ������� 
  LONGLONG        llOptBln;                   // ��Լ��� 
  LONGLONG        llOptAvl;                   // ��Լ�������� 
  LONGLONG        llOptTrdBln;                // ��Լ������������ 
  LONGLONG        llCombedQty;                // ����Ϻ�Լ���� ������ϵ���Ȩ��Լ�ֲ�����
  char            szCostPrice[21 + 1];        // ��Լ�ɱ��� 
};

//-------------------------------10102512:��ʷί�в�ѯ--------------------------
struct MATRADEAPI CReqOptQryHisOrderField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ��������ʲ��˻������ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szStkBizs[2048 + 1];        // ֤ȯҵ�� ���֤ȯҵ���ö��ŷָ�
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  int             iOrderBsn;                  // ί������ 
  int             iBgnDate;                   // ��ʼ���� 
  int             iEndDate;                   // ��ֹ���� 
  LONGLONG        llPageRecnum;               // ��ҳ��ʼ��¼ 
  int             iPageReccnt;                // ��ҳ��¼���� Ĭ��ÿҳ100�м�¼
};

struct MATRADEAPI CRspOptQryHisOrderField
{
  int             iRecNum;                    // ��¼��� 
  int             iTrdDate;                   // �������� 
  int             iOrderDate;                 // ί������ 
  char            szOrderTime[23 + 1];        // ί��ʱ�� 
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            chOrderStatus;              // ί��״̬ 
  char            chOrderValidFlag;           // ί����Ч��־ 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  int             iStkBiz;                    // ֤ȯҵ�� 
  int             iStkBizAction;              // ֤ȯҵ����Ϊ 
  char            szOwnerType[3 + 1];         // ������������ 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptCode[32 + 1];          // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            szCombNum[16 + 1];          // ��ϱ��� 
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 
  char            szLeg3Num[16 + 1];          // �ɷ�����Լ���� 
  char            szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ���� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szOrderUfzAmt[21 + 1];      // ί�нⶳ��� 
  LONGLONG        llOfferQty;                 // �걨���� 
  int             iOfferStime;                // �걨ʱ�� 
  LONGLONG        llWithdrawnQty;             // �ѳ������� 
  LONGLONG        llMatchedQty;               // �ѳɽ����� 
  char            szMatchedAmt[21 + 1];       // �ѳɽ���� 
  char            chIsWithdraw;               // ������־ 
  char            chIsWithdrawn;              // �ѳ�����־ 
  char            chOptUndlCls;               // ���֤ȯ��� �ֵ�[STK_CLS]
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szOptUndlName[16 + 1];      // ���֤ȯ���� 
  LONGLONG        llUndlFrzQty;               // ���ȯί�ж������� 
  LONGLONG        llUndlUfzQty;               // ���ȯί�нⶳ���� 
  LONGLONG        llUndlWthQty;               // ���ȯ�ѳ������� 
  char            szOfferRetMsg[64 + 1];      // �걨������Ϣ 
};

//-------------------------------10102513:��ʷ�ɽ���ѯ--------------------------
struct MATRADEAPI CReqOptQryHisFillField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ��������ʲ��˻������ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szStkBizs[2048 + 1];        // ֤ȯҵ�� ���֤ȯҵ���ö��ŷָ�
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  int             iOrderBsn;                  // ί������ 
  int             iBgnDate;                   // ��ʼ���� 
  int             iEndDate;                   // ��ֹ���� 
  LONGLONG        llPageRecnum;               // ��ҳ��ʼ��¼ 
  int             iPageReccnt;                // ��ҳ��¼���� Ĭ��ÿҳ100�м�¼
};

struct MATRADEAPI CRspOptQryHisFillField
{
  int             iRecNum;                    // ��¼��� 
  int             iTrdDate;                   // �������� 
  char            szMatchedTime[8 + 1];       // �ɽ�ʱ�� 
  int             iOrderDate;                 // ί������ 
  int             iOrderSn;                   // ί����� 
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[32 + 1];          // �ɽ���� 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  int             iStkBiz;                    // ֤ȯҵ�� 
  int             iStkBizAction;              // ֤ȯҵ����Ϊ �������˵��
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            szCombNum[16 + 1];          // ��ϱ��� 
  char            szCombStraCode[16 + 1];     // ��ϲ��Դ��� 
  char            szLeg1Num[16 + 1];          // �ɷ�һ��Լ���� 
  char            szLeg2Num[16 + 1];          // �ɷֶ���Լ���� 
  char            szLeg3Num[16 + 1];          // �ɷ�����Լ���� 
  char            szLeg4Num[16 + 1];          // �ɷ��ĺ�Լ���� 
  char            chCurrency;                 // ���Ҵ��� 
  char            chOptUndlCls;               // ���֤ȯ��� �ֵ�[STK_CLS]
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szOptUndlName[16 + 1];      // ���֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szMatchPrice[21 + 1];       // �ɽ��۸� 
  char            szMatchQty[21 + 1];         // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szOwnerType[3 + 1];         // ������������ 
};

//-------------------------------10102514:�ͻ����㵥(�ļ�)��ѯ--------------------------
struct MATRADEAPI CReqOptQrySettListFileField
{
  int             iTrdDate;                   // �������� �������������Ϊ�Ǳ������ʱ��Ĭ��Ϊǰһ�����ս��㵥����θ�ʽYYYYMMDD������20160203��ʾ2016��2��3�յĽ��㵥
  int             iTrdMonth;                  // �����·� ��θ�ʽYYYYMM������201602��ʾ2016��2�µĽ��㵥
  char            chSettListType;             // ���㵥���� �ֵ�[SETT_LIST_TYPE]�����㵥����Ϊ��D���սᵥʱ��������TRD_DATE�����Ч�������㵥����Ϊ��M���½ᵥʱ�������·�TRD_MONTH�����Ч��
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQrySettListFileField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llTotalLines;               // �ܼ����� 
  int             iTrdDate;                   // �������� 
  int             iTrdMonth;                  // �����·� 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chConfirmFlag;              // �ͻ�ȷ�ϱ�־ ��0��-δȷ�ϣ���1��-��ȷ��
  char            szConfirmTime[23 + 1];      // �ͻ�ȷ��ʱ�� 
  char            szFileFragment[512 + 1];    // �ļ�����Ƭ 
};

//-------------------------------10312020:��Ʊ��Ȩί���걨(�Ż�)--------------------------
struct MATRADEAPI CReqOptOrderExField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻�
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻�
  char            szOptNum[16 + 1];           // ��Լ����
  char            szOrderPrice[21 + 1];       // ί�м۸�
  LONGLONG        llOrderQty;                 // ί������
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ
  int             iOrderBsn;                  // ί������
  char            chEnforceFlag;              // ǿ��ƽ�ֱ�־ 0-��ǿƽ 1-ǿƽ
};

struct MATRADEAPI CRspOptOrderExField
{
  char            szOrderId[10 + 1];          // ��ͬ���
};

//-------------------------------10312022:��Ȩ����˫�߱��ۣ����ڣ�--------------------------
struct MATRADEAPI CReqOptBilateralQuoteField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻�
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻�
  char            szOptNum[16 + 1];           // ��Լ����
  int             iBidStkBiz;                 // ��֤ȯҵ�� 400-���뿪�֣�403-����ƽ��
  char            szBidPrice[21 + 1];         // ��۸�
  LONGLONG        llBidVolume;                // ������
  int             iAskStkBiz;                 // ��֤ȯҵ�� 401-����ƽ�֣�402-��������
  char            szAskPrice[21 + 1];         // ���۸� ��������������һ�ߵ������۸񶼴���0
  LONGLONG        llAskVolume;                // ������
};

struct MATRADEAPI CRspOptBilateralQuoteField
{
  char            szBidOrderId[10 + 1];       // ���ͬ���
  char            szAskOrderId[10 + 1];       // ����ͬ���
  int             iOrderBsn;                  // ί������
  LONGLONG        llCustCode;                 // �ͻ�����
  LONGLONG        llCuacctCode;               // �ʲ��˻�
  char            szStkpbu[8 + 1];            // ���׵�Ԫ
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻�
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ���
  char            szQuoteId[10 + 1];          // ���ۺ�ͬ��� 
};

//-------------------------------10312023:��Ȩ����˫�߱��۳��������ڣ�--------------------------
struct MATRADEAPI CReqOptBilateralQuotationKillField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻�
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻�
  char            szOrderId[10 + 1];          // ��ͬ���
};

struct MATRADEAPI CRspOptBilateralQuotationKillField
{
  int             iOrderBsn;                  // ί������
  char            szBidOrderId[10 + 1];       // ���ͬ���
  char            szRawBidOrderId[10 + 1];    // ԭ���ͬ���
  char            szAskOrderId[10 + 1];       // ����ͬ���
  char            szRawAskOrderId[10 + 1];    // ԭ����ͬ���
  char            szQuoteIdWtd[10 + 1];       // ������ͬ��� 
  char            szQuoteIdRaw[10 + 1];       // ԭ���ۺ�ͬ��� 
};

//-------------------------------10311022:֤ȯ��Ϣ��ѯ����Ȩ��--------------------------
struct MATRADEAPI CReqOptQryStkInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryStkInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[32 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            chStkStatus;                // ֤ȯ״̬ �ֵ�[STK_STATUS]
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkUplmtRatio[21 + 1];    // ��ͣ���� 
  char            szStkLwlmtRatio[21 + 1];    // ��ͣ���� 
  char            szStkUplmtPrice[21 + 1];    // ��ͣ�۸� 
  char            szStkLwlmtPrice[21 + 1];    // ��ͣ�۸� 
  LONGLONG        llStkUplmtQty;              // �������� 
  LONGLONG        llStkLwlmtQty;              // �������� 
  LONGLONG        llStkLotSize;               // ÿ������ 
  char            chStkLotFlag;               // ��λ��־ 0:�ɻ���Ϊ��λ 1:��Ϊ��λ
  LONGLONG        llStkSpread;                // �۲��λ�� 
  char            chStkCalMktval;             // ��ֵ��־ 0:��������ֵ 1:������ֵ
  char            chStkSuspended;             // ͣ�Ʊ�־ 0:���� 1:��ʱͣ�� 2:����ͣ��
  char            szStkIsin[16 + 1];          // ���ʱ��� 
  char            chStkSubCls;                // ֤ȯ����� 
  char            szStkBizes[512 + 1];        // ֤ȯҵ�� 
  char            chStkCustodyMode;           // �й�ģʽ 
  char            szStkUndlCode[8 + 1];       // ���ɴ��� 
  int             iBuyUnit;                   // ������С��λ 
  int             iSaleUnit;                  // ������С��λ 
  char            szBondInt[21 + 1];          // ծȯ��Ϣ 
  char            chStkLevel;                 // ֤ȯ���� 
  int             iStkDeadline;               // ���׽�ֹ���� 
  char            szRemindMessage[128 + 1];   // ������ʾ��Ϣ 
};

//-------------------------------10102515:��Ȩ�ͻ���¼--------------------------
struct MATRADEAPI CReqOptRpcUserLoginField
{
  char            szAcctType[2 + 1];          // �˻����� U:�ͻ����� Z:�ʲ��˻� 05:��A֤ȯ�˻� 15:��A֤ȯ�˻�
  char            szAcctId[32 + 1];           // �˻���ʶ 
  char            chUseScope;                 // ʹ�÷�Χ ��4��:��Ȩ�������� ��5��:��Ȩ�ʽ�����
  char            chAuthType;                 // ��֤���� ��0��--����
  char            szAuthData[256 + 1];        // ��֤���� ����
  char            chEncryptType;              // ���ܷ�ʽ ���ܷ�ʽ: ��0��:��֤KBSSϵͳ���ܷ�ʽ ��1��:��֤Win�漯�н��׼��ܷ�ʽ ��2��:��֤Unix�漯�н��׼��ܷ�ʽ ��3��:�ⲿ�ؼ��ܷ�ʽ ��4��:�ⲿ���ܷ�ʽ ˵��������θ��ݽ���ȯ��Ҫ����д��Ĭ��ֵΪ0�� �ⲿ�ؼ��ܷ�ʽ��3����ʹ�÷ǽ�֤���ܺ������������ļ���Ϊ���ĺ�Ȼ��ʹ�ý�֤��˾�ṩ��ͨ�ż��ܺ���kbss_comencrypt��������һ�μ��ܡ� �ⲿ���ܷ�ʽ��4���� ʹ�÷ǽ�֤���ܺ������������ļ���Ϊ���ĺ���Ϊ��Ρ�
  char            szEncryptKey[32 + 1];       // �������� �������ӿ�����дΪ�˻���ʶ��ACCT_ID����ֵ
};

struct MATRADEAPI CRspOptRpcUserLoginField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chStkex;                    // �����г� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szSubacctCode[8 + 1];       // �����˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            chTrdacctStatus;            // �˻�״̬ 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szAcctType[2 + 1];          // ��½�˻����� U:�ͻ����� Z:�ʲ��˻� 05:��A֤ȯ�˻� 15:��A֤ȯ�˻�
  char            szAcctId[32 + 1];           // ��½�˻���ʶ 
  char            chTrdacctExcls;             // �����˻���� 
  char            szTrdacctName[32 + 1];      // �����˻����� 
  char            szSessionId[256 + 1];       // �Ựƾ֤ 
  int             iSubsysSn;                  // ���׽ڵ��� ��Ȩ�������׽���ţ���֧�ֶ�ڵ㶩��ϵͳ
  char            chPwdMdfFlag;               // �����޸ı�ʶ 0-���޸� 1-�޸� ������ʼ��������״ε�½����1�� ����������״ε�½����1�� �����޸ĺ󣬵�½����0��
  char            szLastLoginTime[23 + 1];    // ����¼ʱ�� 
  char            szLastLoginSite[32 + 1];    // ����¼վ�� 
  char            chLastLoginChnl;            // ����¼���� 
  int             iFlashPort;                 // ����ٶ˿ں�
  char            chFlashFlag;                // �������ñ�־ '0'-δ���� '1'-������
  char            szFlashStkBizes[768 + 1];   // �����ҵ��
};

//-------------------------------10102516:���ٶ����ʽ𻮲�--------------------------
struct MATRADEAPI CReqOptRpcFundingTransferField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ�����\�ʲ��˻�����Ҫ��һ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            chDirect;                   // �������� 0-���ٶ������ 1-���ٶ�������
  char            szTransAmt[21 + 1];         // ������� ������������
  char            szRemark[256 + 1];          // ��ע��Ϣ 
};

struct MATRADEAPI CRspOptRpcFundingTransferField
{
  LONGLONG        llSerialNo;                 // ��ˮ�� 
};

//-------------------------------10102517:���ٶ����ɻ����ʽ��ѯ--------------------------
struct MATRADEAPI CReqOptQryRpcFundingTransferField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ�����\�ʲ��˻�����Ҫ��һ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
};

struct MATRADEAPI CRspOptQryRpcFundingTransferField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szTransAmt[21 + 1];         // �ɻ������ 
};

//-------------------------------10102518:���ٶ����ʽ𻮲���ˮ��ѯ--------------------------
struct MATRADEAPI CReqOptQryRpcFundingTransferFlowField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ�����\�ʲ��˻�����Ҫ��һ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            chDirect;                   // �������� 
  char            chTransStatus;              // ����״̬ 
  char            chCancelStatus;             // ����״̬ 
  LONGLONG        llQryPos;                   // ��λ�� 
  LONGLONG        llQryNum;                   // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryRpcFundingTransferFlowField
{
  LONGLONG        llQryPos;                   // ��λ�� 
  int             iSettDate;                  // �������� 
  LONGLONG        llSerialNo;                 // ��ˮ��� 
  int             iOccurDate;                 // �������� 
  char            szOccurTime[32 + 1];        // ����ʱ�� 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szBizAmt[21 + 1];           // ҵ���� 
  char            chDirect;                   // �������� 
  char            chTransStatus;              // ����״̬ 
  int             iIntSubsysSn;               // �ڲ�������ϵͳ���� 
  int             iIntMsgCode;                // �ڲ�������Ϣ���� 
  char            szIntMsgText[256 + 1];      // �ڲ�������Ϣ���� 
  LONGLONG        llIntSerialNo;              // �ڲ�������ˮ��� 
  int             iIntReqStime;               // �ڲ���������ʱ�� 
  int             iIntAnsStime;               // �ڲ�����Ӧ��ʱ�� 
  int             iExtMsgCode;                // �ⲿ������Ϣ���� 
  char            szExtMsgText[256 + 1];      // �ⲿ������Ϣ���� 
  LONGLONG        llExtSerialNo;              // �ⲿ������ˮ��� 
  int             iExtReqStime;               // �ⲿ��������ʱ�� 
  int             iExtAnsStime;               // �ⲿ����Ӧ��ʱ�� 
  char            chCancelStatus;             // ����״̬ 
  LONGLONG        llOpUser;                   // �����û� 
  char            chOpRole;                   // �����û���ɫ 
  char            szOpName[32 + 1];           // �����û����� 
  char            szRemark[256 + 1];          // ��ע��Ϣ 
};

//-------------------------------10313018:�ٽ����ں�Լ�ʲ���ѯ--------------------------
struct MATRADEAPI CReqOptQryExpendableCuOfCloseToExpireDateField
{
  LONGLONG        llCustCode;                 // �ͻ����� �����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            chOptSide;                  // �ֲַ��� L-Ȩ���֣�S-����֣�C-���Ҳ��Գֲ�
  char            chOptCvdFlag;               // ���ұ�־ 0-�Ǳ��Һ�Լ 1-���Һ�Լ
  int             iDueCloseDays;              // ���뵽������ 
  char            chQueryFlag;                // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryExpendableCuOfCloseToExpireDateField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptCode[32 + 1];          // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            chOptType;                  // ��Լ���� �ֵ�[OPT_TYPE]
  char            chOptSide;                  // �ֲַ��� 
  char            chOptCvdFlag;               // ���ұ�־ 0-�Ǳ��Һ�Լ 1-���Һ�Լ
  LONGLONG        llOptPrebln;                // ��Լ������� 
  LONGLONG        llOptBln;                   // ��Լ��� 
  LONGLONG        llOptAvl;                   // ��Լ�������� 
  LONGLONG        llOptFrz;                   // ��Լ�������� 
  LONGLONG        llOptUfz;                   // ��Լ�ⶳ���� 
  LONGLONG        llOptTrdFrz;                // ��Լ���׶������� 
  LONGLONG        llOptTrdUfz;                // ��Լ���׽ⶳ���� 
  LONGLONG        llOptTrdOtd;                // ��Լ������;���� 
  LONGLONG        llOptTrdBln;                // ��Լ������������ 
  LONGLONG        llOptClrFrz;                // ��Լ���㶳������ 
  LONGLONG        llOptClrUfz;                // ��Լ����ⶳ���� 
  LONGLONG        llOptClrOtd;                // ��Լ������;���� 
  LONGLONG        llOptRealPosi;              // ��Լʵ�ʳֲ� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szOptUndlName[32 + 1];      // ���֤ȯ���� 
  char            chOptUndlCls;               // ���֤ȯ��� �ֵ�[STK_CLS]
  int             iExpireDate;                // ��Ȩ������ 
  int             iExerciseDate;              // ��Ȩ��Ȩ�� 
  int             iRealDueDays;               // ʵ�ʾ��뵽�������� 
};

//-------------------------------10102519:��Ʊ��Ȩ��Ȩָ����ϸ��ѯ--------------------------
struct MATRADEAPI CReqOptQryRpcExeDetailField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ��������ʲ��˻������ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            chOptType;                  // ��Լ���� �ֵ�[OPT_TYPE]
  char            chOptCvdFlag;               // ���ұ�־ 0-�Ǳ��Һ�Լ 1-���Һ�Լ
  char            chExecSide;                 // ��Ȩ���� ��B��-��Ȩ��  ��S��-����Ȩ��
  LONGLONG        llQryPos;                   // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
  char            chCombFlag;                 // �����Ȩ��־ 0:'�������Ȩ';1:'�������Ȩ'
};

struct MATRADEAPI CRspOptQryRpcExeDetailField
{
  LONGLONG        llQryPos;                   // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szUserName[32 + 1];         // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iIntOrg;                    // �ڲ����� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSubacctCode[8 + 1];       // ֤ȯ�˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptName[120 + 1];         // ��Լ��� 
  char            chOptType;                  // ��Լ���� �ֵ�[OPT_TYPE]
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            chOptCvdFlag;               // ���ұ�־ 
  LONGLONG        llOptEffect;                // ָ�ɺ�Լ����(��) 
  LONGLONG        llStkEffect;                // ���֤ȯ�ո����� 
  char            szFundEffect[21 + 1];       // �ո���� ��Ȩ��*ָ������
  char            szExecPrice[21 + 1];        // ��Ȩ�� 
  char            chExecSide;                 // ��Ȩ���� ��B��-��Ȩ��  ��S��-����Ȩ��
  int             iComeInDate;                // �������� 
  char            szFundExeMargin[21 + 1];    // ��Ȩ������֤�� 
  char            szClearingFee[21 + 1];      // ��Ȩ����� 
  char            szCommision[21 + 1];        // ������ 
  char            szFundCombExeUfz[21 + 1];   // �����Ȩ�ⶳ�ʽ� 
  LONGLONG        llCombQty;                  // �����Ȩ���� 
};

//-------------------------------10102520:��Ʊ��Ȩ���շ���֪ͨ��ѯ--------------------------
struct MATRADEAPI CReqOptQryRpcCurrDayRiskNotifyField
{
  char            szRiskIds[512 + 1];         // ���ձ�ʶ(����) 
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ��������ʲ��˻�����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            chReadFlag;                 // �Ķ���־ 0δ�Ķ� 1���Ķ�
  LONGLONG        llQryPos;                   // ��λ�� 1��ʾ��ҳ���ӵڶ�ҳ��ʼ�������ڶ�ҳ������ֵΪ��һҳ�����һ����¼�еġ���λ�����ֶε�ֵ��1��
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryRpcCurrDayRiskNotifyField
{
  LONGLONG        llRecNum;                   // ��¼��� 
  LONGLONG        llNoticeSn;                 // ֪ͨ��� 
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  char            szRiskId[2 + 1];            // ���ձ�ʶ 
  char            szSchemaId[4 + 1];          // Ԥ����� 
  int             iCreateDate;                // �������� 
  int             iCreateTime;                // ����ʱ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSubacctCode[8 + 1];       // �����˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            szUserName[32 + 1];         // �ͻ����� 
  char            szMobile[16 + 1];           // �ƶ��绰 
  char            szEmail[254 + 1];           // �������� 
  char            szTel[16 + 1];              // ��ϵ�绰 
  char            szContent[2048 + 1];        // ֪ͨ���� 
  char            chNoticeChnl;               // ֪ͨ���� ���� �����ʼ� �绰֪ͨ ϵͳ֪ͨ
  char            chSendType;                 // �������� 0���ն��� 1�ֹ����� 2���̴���
  int             iSendDate;                  // �������� 
  int             iSendTime;                  // ����ʱ�� 
  char            chReadFlag;                 // �Ķ���־ 0δ�Ķ� 1���Ķ�
  char            szRespCont[256 + 1];        // �������� 
  int             iRespDate;                  // �������� 
  int             iRespTime;                  // ����ʱ�� 
};

//-------------------------------10102521:��Ʊ��Ȩ��ʷ����֪ͨ��ѯ--------------------------
struct MATRADEAPI CReqOptQryRpcHisRiskNotifyField
{
  char            szRiskIds[512 + 1];         // ���ձ�ʶ(����) 
  int             iBgnDate;                   // ��ʼ���� 
  int             iEndDate;                   // �������� 
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ��������ʲ��˻�����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            chReadFlag;                 // �Ķ���־ 0δ�Ķ� 1���Ķ�
  LONGLONG        llQryPos;                   // ��λ�� 1��ʾ��ҳ���ӵڶ�ҳ��ʼ�������ڶ�ҳ������ֵΪ��һҳ�����һ����¼�еġ���λ�����ֶε�ֵ��1��
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspOptQryRpcHisRiskNotifyField
{
  LONGLONG        llRecNum;                   // ��¼��� 
  LONGLONG        llNoticeSn;                 // ֪ͨ��� 
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  char            szRiskId[2 + 1];            // ���ձ�ʶ 
  char            szSchemaId[4 + 1];          // Ԥ����� 
  int             iCreateDate;                // �������� 
  int             iCreateTime;                // ����ʱ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSubacctCode[8 + 1];       // �����˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            szContent[2048 + 1];        // ֪ͨ���� 
  char            chNoticeChnl;               // ֪ͨ���� ���� �����ʼ� �绰֪ͨ ϵͳ֪ͨ
  char            chSendType;                 // �������� 0���ն��� 1�ֹ����� 2���̴���
  int             iSendDate;                  // �������� 
  int             iSendTime;                  // ����ʱ�� 
  char            chReadFlag;                 // �Ķ���־ 0δ�Ķ� 1���Ķ�
  char            szRespCont[256 + 1];        // �������� 
  int             iRespDate;                  // �������� 
  int             iRespTime;                  // ����ʱ�� 
};

//-------------------------------10102522:��Ʊ��Ȩ���շ���֪ͨ��ѯ�����ϵͳר�ã�--------------------------
struct MATRADEAPI CReqOptQryRpcCurrDayRiskSysNotifyField
{
  LONGLONG        llQryPos;                   // ��λ�� 1��ʾ��ҳ���ӵڶ�ҳ��ʼ�������ڶ�ҳ������ֵΪ��һҳ�����һ����¼�еġ���λ�����ֶε�ֵ��1��
  int             iQryNum;                    // ��ѯ���� 
  char            szNoticeId[512 + 1];        // ���ձ�ʶ(����) �ֵ�[NOTICE_REASON]
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ��������ʲ��˻�����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            chReadFlag;                 // �Ķ���־ 0δ�Ķ� 1���Ķ�
  char            chRespFlag;                 // ������־ 0δ���� 1�ѷ���
};

struct MATRADEAPI CRspOptQryRpcCurrDayRiskSysNotifyField
{
  LONGLONG        llRecNum;                   // ��¼��� 
  LONGLONG        llNoticeSn;                 // ֪ͨ��� 
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  char            szNoticeId[4 + 1];          // ���ձ�ʶ 
  char            szSchemaId[6 + 1];          // Ԥ����� 
  int             iCreateDate;                // �������� 
  char            szCreateTime[32 + 1];       // ����ʱ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSubacctCode[8 + 1];       // �����˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            szUserName[32 + 1];         // �ͻ����� 
  char            szMobile[32 + 1];           // �ƶ��绰 
  char            szEmail[254 + 1];           // �������� 
  char            szTel[32 + 1];              // ��ϵ�绰 
  char            szContent[2048 + 1];        // ֪ͨ���� 
  char            szNoticeChnl[16 + 1];       // ֪ͨ���� ���� �����ʼ� �绰֪ͨ ϵͳ֪ͨ
  char            chSendType;                 // �������� 0���ն��� 1�ֹ����� 2���̴���
  int             iSendDate;                  // �������� 
  char            szSendTime[32 + 1];         // ����ʱ�� 
  char            chRespFlag;                 // ������־ 0δ���� 1�ѷ���
};

//-------------------------------10102523:��Ʊ��Ȩ��ʷ����֪ͨ��ѯ�����ϵͳר�ã�--------------------------
struct MATRADEAPI CReqOptQryRpcHisRiskSysNotifyField
{
  LONGLONG        llQryPos;                   // ��λ�� 1��ʾ��ҳ���ӵڶ�ҳ��ʼ�������ڶ�ҳ������ֵΪ��һҳ�����һ����¼�еġ���λ�����ֶε�ֵ��1��
  int             iQryNum;                    // ��ѯ���� 
  char            szNoticeId[512 + 1];        // ���ձ�ʶ(����) �ֵ�[NOTICE_REASON]
  int             iBgnDate;                   // ��ʼ���� 
  int             iEndDate;                   // �������� 
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ��������ʲ��˻�����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOptUndlCode[8 + 1];       // ���֤ȯ���� 
  char            chReadFlag;                 // �Ķ���־ 0δ�Ķ� 1���Ķ�
  char            chRespFlag;                 // ������־ 0δ���� 1�ѷ���
};

struct MATRADEAPI CRspOptQryRpcHisRiskSysNotifyField
{
  LONGLONG        llRecNum;                   // ��¼��� 
  LONGLONG        llNoticeSn;                 // ֪ͨ��� 
  LONGLONG        llNoticeBsn;                // ֪ͨ���� 
  char            szNoticeId[4 + 1];          // ���ձ�ʶ 
  char            szSchemaId[6 + 1];          // Ԥ����� 
  int             iCreateDate;                // �������� 
  char            szCreateTime[32 + 1];       // ����ʱ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSubacctCode[8 + 1];       // �����˻��ӱ��� 
  char            szOptTrdacct[28 + 1];       // ��Ȩ��Լ�˻� 
  char            szContent[2048 + 1];        // ֪ͨ���� 
  char            szNoticeChnl[16 + 1];       // ֪ͨ���� ���� �����ʼ� �绰֪ͨ ϵͳ֪ͨ
  char            chSendType;                 // �������� 0���ն��� 1�ֹ����� 2���̴���
  int             iSendDate;                  // �������� 
  char            szSendTime[32 + 1];         // ����ʱ�� 
  char            chRespFlag;                 // ������־ 0δ���� 1�ѷ���
};

//////////////////////////////////////////////////////////////////////////
// �����Ϣ����
#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN 256 
#endif

// ��������Ϣ����
#ifndef MAX_MSG_DEBUG_LEN
#define MAX_MSG_DEBUG_LEN 1024 
#endif

// ���ί����ų���
#ifndef MAX_ORDER_ID_LEN
#define MAX_ORDER_ID_LEN 10 
#endif

// ����ͳ���32K
#ifndef MAX_SEND_BUF_LEN
#define MAX_SEND_BUF_LEN (1024 * 32) 
#endif

// �����ճ���32K
#ifndef MAX_RECV_BUF_LEN
#define MAX_RECV_BUF_LEN (1024 * 32) 
#endif

//-------------------------------10312041:��Ȩί���걨(����)-------------------------
struct MATRADEAPI CReqOptOrderFlashField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szOptNum[16 + 1];           // ��Լ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  short           siStkBiz;                   // ֤ȯҵ�� �ֵ�[STK_BIZ]
  short           siStkBizAction;             // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ(δ����)   
  int             iOrderBsn;                  // ί������
  char            chEnforceFlag;              // ǿ��ƽ�ֱ�־
  char            chOrderType;                // ί������
  char            szClientInfo[MAX_MSG_LEN + 1]; // �ն���Ϣ(δ����)
};

struct MATRADEAPI CRspOptOrderFlashField
{
  char szOrderId[MAX_ORDER_ID_LEN + 1];       // ��ͬ���
  int iOrderBsn;
};

//-------------------------------10312042:��Ȩί�г���(����)------------------------------------
struct MATRADEAPI CReqOptCancelOrderFlashField
{
  LONGLONG      llCuacctCode;                 // �ʲ��˻�
  char          szStkbd[2 + 1];               // ���װ��
  char          szOrderId[MAX_ORDER_ID_LEN + 1]; // ��ͬ���
  int           iOrderBsn;                    // ί������(δ����)  
  char          chForceWth;                   // ǿ����־
  char          szRiskId[2 + 1];              // ���ձ�ʶ(ǿ��ƽ��ʱ����)
  char          szClientInfo[MAX_MSG_LEN + 1]; // �ն���Ϣ(δ����)
  short         siStkBiz;                     // ֤ȯҵ��
};

struct MATRADEAPI CRspOptCancelOrderFlashField
{
  char          szStkbd[2 + 1];               // ���װ��  
  LONGLONG      llCuacctCode;                 // �ʲ��˻�
  char          szOrderId[MAX_ORDER_ID_LEN + 1]; // ��ͬ��� ԭί�к�ͬ���
  char          szOrderIdWtd[MAX_ORDER_ID_LEN + 1]; // ί�к�ͬ�� ����ί�к�ͬ���  
  int           iOrderBsn;                    // ί������(δ����)
  char          chCancelStatus;               // �ڲ�������־(δ����)  
  short         siStkBiz;                     // ֤ȯҵ��
};

//-------------------------------10312044:�ɽ��ر�����(���٣�------------------------------------
struct MATRADEAPI CRtnOptOrderFillFlashField
{
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iMatchedDate;               // �ɽ�����
  char          szStkbd[2 + 1];             // ���װ��
  char          szOrderId[10 + 1];          // ��ͬ���
  char          szMatchedSn[32 + 1];        // �ɽ����
  char          szFundAvl[21 + 1];          // �ʽ���ý��ɽ���
  LONGLONG      llOptAvl;                   // ��Լ�����������ɽ���
  char          chOrderStatus;              // ί��״̬
  LONGLONG      llMatchedQty;               // ���γɽ�����
  char          szMatchedPrice[11 + 1];     // ���γɽ��۸�
  char          szOptCode[16 + 1];          // ֤ȯ����
  char          chIsWithdraw;               // ������־
};

//-------------------------------10312045:ȷ�ϻر����ͣ����٣�------------------------------------
struct MATRADEAPI CRtnOptOrderConfirmFlashField
{
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iOrderDate;                 // ί������
  char          szOptCode[16 + 1];          // ��Լ����
  char          szStkbd[2 + 1];             // ���װ��
  char          szOrderId[10 + 1];          // ��ͬ���
  char          chIsWithdraw;               // ������־
  char          chOrderStatus;              // ί��״̬
};

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

END_NAMESPACE_MACLI

#endif  //__MA_CLI_OPT_TRADE_API_STRUCT_H__