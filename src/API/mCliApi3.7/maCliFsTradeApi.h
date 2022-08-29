#if !defined(__MA_CLI_FS_TRADE_API_H__)
#define __MA_CLI_FS_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliFsTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliFsTradeSpi : virtual public CCliTradeSpi
{
public:
  // 确认回报
  virtual int OnRtnOrderConfirm(CRtnFsOrderConfirmField *p_pRtnOrderConfirmField) {return 0;}
  // 成交回报
  virtual int OnRtnOrderFill(CRtnFsOrderFillField *p_pRtnOrderFillField) {return 0;}
  // 机构信息查询响应
  virtual int OnRspQry10308011(CFirstSetField *p_pFirstSetField, CRspFsQry10308011Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券信息查询响应
  virtual int OnRspQry10308013(CFirstSetField *p_pFirstSetField, CRspFsQry10308013Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 字典信息查询响应
  virtual int OnRspQry10308012(CFirstSetField *p_pFirstSetField, CRspFsQry10308012Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 系统状态查询响应
  virtual int OnRspQry10308014(CFirstSetField *p_pFirstSetField, CRspFsQry10308014Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证转账响应
  virtual int OnRsp10510605(CFirstSetField *p_pFirstSetField, CRspFs10510605Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 券商发起查银行余额响应
  virtual int OnRspQry10510606(CFirstSetField *p_pFirstSetField, CRspFsQry10510606Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询银证业务流水响应
  virtual int OnRspQry10510608(CFirstSetField *p_pFirstSetField, CRspFsQry10510608Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户内现金划转响应
  virtual int OnRsp10510620(CFirstSetField *p_pFirstSetField, CRspFs10510620Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 账户资金归集响应
  virtual int OnRspQry10510621(CFirstSetField *p_pFirstSetField, CRspFsQry10510621Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询转帐银行帐户响应
  virtual int OnRspQry10510601(CFirstSetField *p_pFirstSetField, CRspFsQry10510601Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 财富账户资金划拨响应
  virtual int OnRspQry10510468(CFirstSetField *p_pFirstSetField, CRspFsQry10510468Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 财富账户划拨流水查询响应
  virtual int OnRspQry10510473(CFirstSetField *p_pFirstSetField, CRspFsQry10510473Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户登录响应
  virtual int OnRspQry10301105(CFirstSetField *p_pFirstSetField, CRspFsQry10301105Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资产账户查询响应
  virtual int OnRspQry10308112(CFirstSetField *p_pFirstSetField, CRspFsQry10308112Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 财富账户查询响应
  virtual int OnRspQry10308113(CFirstSetField *p_pFirstSetField, CRspFsQry10308113Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股东查询响应
  virtual int OnRspQry10308001(CFirstSetField *p_pFirstSetField, CRspFsQry10308001Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股东交易协议查询响应
  virtual int OnRspQry10308010(CFirstSetField *p_pFirstSetField, CRspFsQry10308010Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 适当性匹配检查响应
  virtual int OnRsp10308611(CFirstSetField *p_pFirstSetField, CRspFs10308611Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户适当性查询响应
  virtual int OnRspQry10308068(CFirstSetField *p_pFirstSetField, CRspFsQry10308068Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 产品适当性查询响应
  virtual int OnRspQry10308069(CFirstSetField *p_pFirstSetField, CRspFsQry10308069Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金查询响应
  virtual int OnRspQry10308048(CFirstSetField *p_pFirstSetField, CRspFsQry10308048Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 总资产查询响应
  virtual int OnRspQry10308002(CFirstSetField *p_pFirstSetField, CRspFsQry10308002Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份资产查询响应
  virtual int OnRspQry10308003(CFirstSetField *p_pFirstSetField, CRspFsQry10308003Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托查询响应
  virtual int OnRspQry10308004(CFirstSetField *p_pFirstSetField, CRspFsQry10308004Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交查询响应
  virtual int OnRspQry10308006(CFirstSetField *p_pFirstSetField, CRspFsQry10308006Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交统计响应
  virtual int OnRspQry10308053(CFirstSetField *p_pFirstSetField, CRspFsQry10308053Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 最大可交易数量计算响应
  virtual int OnRspQry10308056(CFirstSetField *p_pFirstSetField, CRspFsQry10308056Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券交易买卖委托申报响应
  virtual int OnRsp10308601(CFirstSetField *p_pFirstSetField, CRspFs10308601Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券交易委托撤单响应
  virtual int OnRsp10308607(CFirstSetField *p_pFirstSetField, CRspFs10308607Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可撤单委托查询响应
  virtual int OnRspQry10308054(CFirstSetField *p_pFirstSetField, CRspFsQry10308054Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券交易批量撤单响应
  virtual int OnRsp10308606(CFirstSetField *p_pFirstSetField, CRspFs10308606Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 重置成本响应
  virtual int OnRsp10308602(CFirstSetField *p_pFirstSetField, CRspFs10308602Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 国债预发行持仓明细查询响应
  virtual int OnRspQry10308015(CFirstSetField *p_pFirstSetField, CRspFsQry10308015Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 国债预发行客户平仓明细查询响应
  virtual int OnRspQry10308016(CFirstSetField *p_pFirstSetField, CRspFsQry10308016Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 国债预发行产品信息查询响应
  virtual int OnRspQry10308017(CFirstSetField *p_pFirstSetField, CRspFsQry10308017Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股配号查询响应
  virtual int OnRspQry10418004(CFirstSetField *p_pFirstSetField, CRspFsQry10418004Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股中签查询响应
  virtual int OnRspQry10418005(CFirstSetField *p_pFirstSetField, CRspFsQry10418005Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股申购市值额度查询响应
  virtual int OnRspQry10418006(CFirstSetField *p_pFirstSetField, CRspFsQry10418006Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股信息查询响应
  virtual int OnRspQry10418007(CFirstSetField *p_pFirstSetField, CRspFsQry10418007Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购品种信息查询响应
  virtual int OnRspQry10308019(CFirstSetField *p_pFirstSetField, CRspFsQry10308019Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购质押物折算率信息查询响应
  virtual int OnRspQry10308020(CFirstSetField *p_pFirstSetField, CRspFsQry10308020Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购质押物查询响应
  virtual int OnRspQry10308021(CFirstSetField *p_pFirstSetField, CRspFsQry10308021Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购合约展期响应
  virtual int OnRsp10308608(CFirstSetField *p_pFirstSetField, CRspFs10308608Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购质押资金查询响应
  virtual int OnRspQry10308022(CFirstSetField *p_pFirstSetField, CRspFsQry10308022Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购预约流水查询响应
  virtual int OnRspQry10308023(CFirstSetField *p_pFirstSetField, CRspFsQry10308023Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购预约响应
  virtual int OnRsp10308610(CFirstSetField *p_pFirstSetField, CRspFs10308610Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可提前购回合约查询响应
  virtual int OnRspQry10308024(CFirstSetField *p_pFirstSetField, CRspFsQry10308024Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可展期取消合约查询响应
  virtual int OnRspQry10308025(CFirstSetField *p_pFirstSetField, CRspFsQry10308025Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可调整自动展期合约查询响应
  virtual int OnRspQry10308026(CFirstSetField *p_pFirstSetField, CRspFsQry10308026Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可取消提前终止预约合约查询响应
  virtual int OnRspQry10308027(CFirstSetField *p_pFirstSetField, CRspFsQry10308027Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可取消初始交易预约合约查询响应
  virtual int OnRspQry10308028(CFirstSetField *p_pFirstSetField, CRspFsQry10308028Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可展期合约查询响应
  virtual int OnRspQry10308029(CFirstSetField *p_pFirstSetField, CRspFsQry10308029Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可提前终止预约合约查询响应
  virtual int OnRspQry10308074(CFirstSetField *p_pFirstSetField, CRspFsQry10308074Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权直接还款响应
  virtual int OnRsp10308615(CFirstSetField *p_pFirstSetField, CRspFs10308615Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权合约查询响应
  virtual int OnRspQry10308030(CFirstSetField *p_pFirstSetField, CRspFsQry10308030Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权可还负债查询响应
  virtual int OnRspQry10308085(CFirstSetField *p_pFirstSetField, CRspFsQry10308085Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权偿还明细查询响应
  virtual int OnRspQry10308087(CFirstSetField *p_pFirstSetField, CRspFsQry10308087Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票股东大会信息查询响应
  virtual int OnRspQry10308031(CFirstSetField *p_pFirstSetField, CRspFsQry10308031Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票投票结果查询响应
  virtual int OnRspQry10308032(CFirstSetField *p_pFirstSetField, CRspFsQry10308032Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票议案信息查询响应
  virtual int OnRspQry10308033(CFirstSetField *p_pFirstSetField, CRspFsQry10308033Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票产品信息查询响应
  virtual int OnRspQry10308059(CFirstSetField *p_pFirstSetField, CRspFsQry10308059Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通价差信息查询响应
  virtual int OnRspQry10308051(CFirstSetField *p_pFirstSetField, CRspFsQry10308051Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通汇率信息查询响应
  virtual int OnRspQry10308050(CFirstSetField *p_pFirstSetField, CRspFsQry10308050Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通市场状态信息查询响应
  virtual int OnRspQry10308061(CFirstSetField *p_pFirstSetField, CRspFsQry10308061Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通标的证券信息查询响应
  virtual int OnRspQry10308034(CFirstSetField *p_pFirstSetField, CRspFsQry10308034Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交易日历查询响应
  virtual int OnRspQry10308035(CFirstSetField *p_pFirstSetField, CRspFsQry10308035Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通资金查询响应
  virtual int OnRspQry10308047(CFirstSetField *p_pFirstSetField, CRspFsQry10308047Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 大宗交易行情信息查询响应
  virtual int OnRspQry10308036(CFirstSetField *p_pFirstSetField, CRspFsQry10308036Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 盘后行情查询响应
  virtual int OnRspQry10308108(CFirstSetField *p_pFirstSetField, CRspFsQry10308108Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 受限投资者可交易证券查询响应
  virtual int OnRspQry10308037(CFirstSetField *p_pFirstSetField, CRspFsQry10308037Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转分层信息查询响应
  virtual int OnRspQry10308067(CFirstSetField *p_pFirstSetField, CRspFsQry10308067Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转询价确认信息查询响应
  virtual int OnRspQry10308109(CFirstSetField *p_pFirstSetField, CRspFsQry10308109Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转发行业务信息查询响应
  virtual int OnRspQry10308110(CFirstSetField *p_pFirstSetField, CRspFsQry10308110Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押合约总表查询响应
  virtual int OnRspQry10308038(CFirstSetField *p_pFirstSetField, CRspFsQry10308038Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押合约明细查询响应
  virtual int OnRspQry10308039(CFirstSetField *p_pFirstSetField, CRspFsQry10308039Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押初始交易参考金额查询响应
  virtual int OnRspQry10308062(CFirstSetField *p_pFirstSetField, CRspFsQry10308062Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押股份额度信息查询响应
  virtual int OnRspQry10308063(CFirstSetField *p_pFirstSetField, CRspFsQry10308063Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押标的证券查询响应
  virtual int OnRspQry10308076(CFirstSetField *p_pFirstSetField, CRspFsQry10308076Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 出借人股票质押额度查询响应
  virtual int OnRspQry10308077(CFirstSetField *p_pFirstSetField, CRspFsQry10308077Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳债券质押式协议回购合约查询响应
  virtual int OnRspQry10308040(CFirstSetField *p_pFirstSetField, CRspFsQry10308040Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券质押式协议回购标的证券查询响应
  virtual int OnRspQry10308041(CFirstSetField *p_pFirstSetField, CRspFsQry10308041Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购行情查询申报响应
  virtual int OnRsp10308613(CFirstSetField *p_pFirstSetField, CRspFs10308613Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购非公开报价行情查询响应
  virtual int OnRspQry10308072(CFirstSetField *p_pFirstSetField, CRspFsQry10308072Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购公开报价行情查询响应
  virtual int OnRspQry10308073(CFirstSetField *p_pFirstSetField, CRspFsQry10308073Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购未结算协议查询响应
  virtual int OnRspQry10308075(CFirstSetField *p_pFirstSetField, CRspFsQry10308075Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购合约查询响应
  virtual int OnRspQry10308078(CFirstSetField *p_pFirstSetField, CRspFsQry10308078Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳债券协议回购购回金额查询响应
  virtual int OnRspQry10308086(CFirstSetField *p_pFirstSetField, CRspFsQry10308086Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券质押股份明细查询响应
  virtual int OnRspQry10308042(CFirstSetField *p_pFirstSetField, CRspFsQry10308042Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券质押式回购合约查询响应
  virtual int OnRspQry10308043(CFirstSetField *p_pFirstSetField, CRspFsQry10308043Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请撤单响应
  virtual int OnRsp10308604(CFirstSetField *p_pFirstSetField, CRspFs10308604Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF篮子委托响应
  virtual int OnRsp10308605(CFirstSetField *p_pFirstSetField, CRspFs10308605Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请查询响应
  virtual int OnRspQry10308057(CFirstSetField *p_pFirstSetField, CRspFsQry10308057Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF成分股持仓查询响应
  virtual int OnRspQry10308064(CFirstSetField *p_pFirstSetField, CRspFsQry10308064Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF信息查询响应
  virtual int OnRspQry10308052(CFirstSetField *p_pFirstSetField, CRspFsQry10308052Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海LOF基金分拆合并信息查询响应
  virtual int OnRspQry10308044(CFirstSetField *p_pFirstSetField, CRspFsQry10308044Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳LOF基金分拆合并信息查询响应
  virtual int OnRspQry10308045(CFirstSetField *p_pFirstSetField, CRspFsQry10308045Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股权人员信息查询响应
  virtual int OnRspQry10308046(CFirstSetField *p_pFirstSetField, CRspFsQry10308046Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 所得税策略信息维护响应
  virtual int OnRspQry10308065(CFirstSetField *p_pFirstSetField, CRspFsQry10308065Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 要约收购转换关系表查询响应
  virtual int OnRspQry10308058(CFirstSetField *p_pFirstSetField, CRspFsQry10308058Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海减持股份额度查询响应
  virtual int OnRspQry10308070(CFirstSetField *p_pFirstSetField, CRspFsQry10308070Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳减持股份额度查询响应
  virtual int OnRspQry10308071(CFirstSetField *p_pFirstSetField, CRspFsQry10308071Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 权证信息查询响应
  virtual int OnRspQry10308084(CFirstSetField *p_pFirstSetField, CRspFsQry10308084Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 三方回购篮子信息查询响应
  virtual int OnRspQry10308079(CFirstSetField *p_pFirstSetField, CRspFsQry10308079Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 三方回购质押表查询响应
  virtual int OnRspQry10308080(CFirstSetField *p_pFirstSetField, CRspFsQry10308080Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳三方回购转发成交信息表查询响应
  virtual int OnRspQry10308081(CFirstSetField *p_pFirstSetField, CRspFsQry10308081Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳三方回购合约总表查询响应
  virtual int OnRspQry10308082(CFirstSetField *p_pFirstSetField, CRspFsQry10308082Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳三方回购合约明细查询响应
  virtual int OnRspQry10308083(CFirstSetField *p_pFirstSetField, CRspFsQry10308083Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H股全流通股东账户信息查询响应
  virtual int OnRspQry10308104(CFirstSetField *p_pFirstSetField, CRspFsQry10308104Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H股境外证券代码对应关系查询响应
  virtual int OnRspQry10308105(CFirstSetField *p_pFirstSetField, CRspFsQry10308105Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 询价交易非公开报价行情查询响应
  virtual int OnRspQry10308106(CFirstSetField *p_pFirstSetField, CRspFsQry10308106Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史成交查询响应
  virtual int OnRspQry10418001(CFirstSetField *p_pFirstSetField, CRspFsQry10418001Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史委托查询响应
  virtual int OnRspQry10418002(CFirstSetField *p_pFirstSetField, CRspFsQry10418002Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史交割单查询响应
  virtual int OnRspQry10418003(CFirstSetField *p_pFirstSetField, CRspFsQry10418003Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliFsTradeApi : virtual public CCliTradeApi
{
public:
  // 默认构造函数
  CCliFsTradeApi(void);

  // 析构函数
  virtual ~CCliFsTradeApi(void);

  // 机构信息查询请求
  virtual int ReqQry10308011(CReqFsQry10308011Field *p_pReqField, LONGLONG p_llRequestId);
  // 证券信息查询请求
  virtual int ReqQry10308013(CReqFsQry10308013Field *p_pReqField, LONGLONG p_llRequestId);
  // 字典信息查询请求
  virtual int ReqQry10308012(CReqFsQry10308012Field *p_pReqField, LONGLONG p_llRequestId);
  // 系统状态查询请求
  virtual int ReqQry10308014(CReqFsQry10308014Field *p_pReqField, LONGLONG p_llRequestId);
  // 银证转账请求
  virtual int Req10510605(CReqFs10510605Field *p_pReqField, LONGLONG p_llRequestId);
  // 券商发起查银行余额请求
  virtual int ReqQry10510606(CReqFsQry10510606Field *p_pReqField, LONGLONG p_llRequestId);
  // 查询银证业务流水请求
  virtual int ReqQry10510608(CReqFsQry10510608Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户内现金划转请求
  virtual int Req10510620(CReqFs10510620Field *p_pReqField, LONGLONG p_llRequestId);
  // 账户资金归集请求
  virtual int ReqQry10510621(CReqFsQry10510621Field *p_pReqField, LONGLONG p_llRequestId);
  // 查询转帐银行帐户请求
  virtual int ReqQry10510601(CReqFsQry10510601Field *p_pReqField, LONGLONG p_llRequestId);
  // 财富账户资金划拨请求
  virtual int ReqQry10510468(CReqFsQry10510468Field *p_pReqField, LONGLONG p_llRequestId);
  // 财富账户划拨流水查询请求
  virtual int ReqQry10510473(CReqFsQry10510473Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户登录请求
  virtual int ReqQry10301105(CReqFsQry10301105Field *p_pReqField, LONGLONG p_llRequestId);
  // 资产账户查询请求
  virtual int ReqQry10308112(CReqFsQry10308112Field *p_pReqField, LONGLONG p_llRequestId);
  // 财富账户查询请求
  virtual int ReqQry10308113(CReqFsQry10308113Field *p_pReqField, LONGLONG p_llRequestId);
  // 股东查询请求
  virtual int ReqQry10308001(CReqFsQry10308001Field *p_pReqField, LONGLONG p_llRequestId);
  // 股东交易协议查询请求
  virtual int ReqQry10308010(CReqFsQry10308010Field *p_pReqField, LONGLONG p_llRequestId);
  // 适当性匹配检查请求
  virtual int Req10308611(CReqFs10308611Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户适当性查询请求
  virtual int ReqQry10308068(CReqFsQry10308068Field *p_pReqField, LONGLONG p_llRequestId);
  // 产品适当性查询请求
  virtual int ReqQry10308069(CReqFsQry10308069Field *p_pReqField, LONGLONG p_llRequestId);
  // 资金查询请求
  virtual int ReqQry10308048(CReqFsQry10308048Field *p_pReqField, LONGLONG p_llRequestId);
  // 总资产查询请求
  virtual int ReqQry10308002(CReqFsQry10308002Field *p_pReqField, LONGLONG p_llRequestId);
  // 股份资产查询请求
  virtual int ReqQry10308003(CReqFsQry10308003Field *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托查询请求
  virtual int ReqQry10308004(CReqFsQry10308004Field *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交查询请求
  virtual int ReqQry10308006(CReqFsQry10308006Field *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交统计请求
  virtual int ReqQry10308053(CReqFsQry10308053Field *p_pReqField, LONGLONG p_llRequestId);
  // 最大可交易数量计算请求
  virtual int ReqQry10308056(CReqFsQry10308056Field *p_pReqField, LONGLONG p_llRequestId);
  // 证券交易买卖委托申报请求
  virtual int Req10308601(CReqFs10308601Field *p_pReqField, LONGLONG p_llRequestId);
  // 证券交易委托撤单请求
  virtual int Req10308607(CReqFs10308607Field *p_pReqField, LONGLONG p_llRequestId);
  // 可撤单委托查询请求
  virtual int ReqQry10308054(CReqFsQry10308054Field *p_pReqField, LONGLONG p_llRequestId);
  // 证券交易批量撤单请求
  virtual int Req10308606(CReqFs10308606Field *p_pReqField, LONGLONG p_llRequestId);
  // 重置成本请求
  virtual int Req10308602(CReqFs10308602Field *p_pReqField, LONGLONG p_llRequestId);
  // 国债预发行持仓明细查询请求
  virtual int ReqQry10308015(CReqFsQry10308015Field *p_pReqField, LONGLONG p_llRequestId);
  // 国债预发行客户平仓明细查询请求
  virtual int ReqQry10308016(CReqFsQry10308016Field *p_pReqField, LONGLONG p_llRequestId);
  // 国债预发行产品信息查询请求
  virtual int ReqQry10308017(CReqFsQry10308017Field *p_pReqField, LONGLONG p_llRequestId);
  // 新股配号查询请求
  virtual int ReqQry10418004(CReqFsQry10418004Field *p_pReqField, LONGLONG p_llRequestId);
  // 新股中签查询请求
  virtual int ReqQry10418005(CReqFsQry10418005Field *p_pReqField, LONGLONG p_llRequestId);
  // 新股申购市值额度查询请求
  virtual int ReqQry10418006(CReqFsQry10418006Field *p_pReqField, LONGLONG p_llRequestId);
  // 新股信息查询请求
  virtual int ReqQry10418007(CReqFsQry10418007Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购品种信息查询请求
  virtual int ReqQry10308019(CReqFsQry10308019Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购质押物折算率信息查询请求
  virtual int ReqQry10308020(CReqFsQry10308020Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购质押物查询请求
  virtual int ReqQry10308021(CReqFsQry10308021Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购合约展期请求
  virtual int Req10308608(CReqFs10308608Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购质押资金查询请求
  virtual int ReqQry10308022(CReqFsQry10308022Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购预约流水查询请求
  virtual int ReqQry10308023(CReqFsQry10308023Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购预约请求
  virtual int Req10308610(CReqFs10308610Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可提前购回合约查询请求
  virtual int ReqQry10308024(CReqFsQry10308024Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可展期取消合约查询请求
  virtual int ReqQry10308025(CReqFsQry10308025Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可调整自动展期合约查询请求
  virtual int ReqQry10308026(CReqFsQry10308026Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可取消提前终止预约合约查询请求
  virtual int ReqQry10308027(CReqFsQry10308027Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可取消初始交易预约合约查询请求
  virtual int ReqQry10308028(CReqFsQry10308028Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可展期合约查询请求
  virtual int ReqQry10308029(CReqFsQry10308029Field *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可提前终止预约合约查询请求
  virtual int ReqQry10308074(CReqFsQry10308074Field *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权直接还款请求
  virtual int Req10308615(CReqFs10308615Field *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权合约查询请求
  virtual int ReqQry10308030(CReqFsQry10308030Field *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权可还负债查询请求
  virtual int ReqQry10308085(CReqFsQry10308085Field *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权偿还明细查询请求
  virtual int ReqQry10308087(CReqFsQry10308087Field *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票股东大会信息查询请求
  virtual int ReqQry10308031(CReqFsQry10308031Field *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票投票结果查询请求
  virtual int ReqQry10308032(CReqFsQry10308032Field *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票议案信息查询请求
  virtual int ReqQry10308033(CReqFsQry10308033Field *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票产品信息查询请求
  virtual int ReqQry10308059(CReqFsQry10308059Field *p_pReqField, LONGLONG p_llRequestId);
  // 港股通价差信息查询请求
  virtual int ReqQry10308051(CReqFsQry10308051Field *p_pReqField, LONGLONG p_llRequestId);
  // 港股通汇率信息查询请求
  virtual int ReqQry10308050(CReqFsQry10308050Field *p_pReqField, LONGLONG p_llRequestId);
  // 港股通市场状态信息查询请求
  virtual int ReqQry10308061(CReqFsQry10308061Field *p_pReqField, LONGLONG p_llRequestId);
  // 港股通标的证券信息查询请求
  virtual int ReqQry10308034(CReqFsQry10308034Field *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交易日历查询请求
  virtual int ReqQry10308035(CReqFsQry10308035Field *p_pReqField, LONGLONG p_llRequestId);
  // 港股通资金查询请求
  virtual int ReqQry10308047(CReqFsQry10308047Field *p_pReqField, LONGLONG p_llRequestId);
  // 大宗交易行情信息查询请求
  virtual int ReqQry10308036(CReqFsQry10308036Field *p_pReqField, LONGLONG p_llRequestId);
  // 盘后行情查询请求
  virtual int ReqQry10308108(CReqFsQry10308108Field *p_pReqField, LONGLONG p_llRequestId);
  // 受限投资者可交易证券查询请求
  virtual int ReqQry10308037(CReqFsQry10308037Field *p_pReqField, LONGLONG p_llRequestId);
  // 股转分层信息查询请求
  virtual int ReqQry10308067(CReqFsQry10308067Field *p_pReqField, LONGLONG p_llRequestId);
  // 股转询价确认信息查询请求
  virtual int ReqQry10308109(CReqFsQry10308109Field *p_pReqField, LONGLONG p_llRequestId);
  // 股转发行业务信息查询请求
  virtual int ReqQry10308110(CReqFsQry10308110Field *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押合约总表查询请求
  virtual int ReqQry10308038(CReqFsQry10308038Field *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押合约明细查询请求
  virtual int ReqQry10308039(CReqFsQry10308039Field *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押初始交易参考金额查询请求
  virtual int ReqQry10308062(CReqFsQry10308062Field *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押股份额度信息查询请求
  virtual int ReqQry10308063(CReqFsQry10308063Field *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押标的证券查询请求
  virtual int ReqQry10308076(CReqFsQry10308076Field *p_pReqField, LONGLONG p_llRequestId);
  // 出借人股票质押额度查询请求
  virtual int ReqQry10308077(CReqFsQry10308077Field *p_pReqField, LONGLONG p_llRequestId);
  // 深圳债券质押式协议回购合约查询请求
  virtual int ReqQry10308040(CReqFsQry10308040Field *p_pReqField, LONGLONG p_llRequestId);
  // 债券质押式协议回购标的证券查询请求
  virtual int ReqQry10308041(CReqFsQry10308041Field *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购行情查询申报请求
  virtual int Req10308613(CReqFs10308613Field *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购非公开报价行情查询请求
  virtual int ReqQry10308072(CReqFsQry10308072Field *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购公开报价行情查询请求
  virtual int ReqQry10308073(CReqFsQry10308073Field *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购未结算协议查询请求
  virtual int ReqQry10308075(CReqFsQry10308075Field *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购合约查询请求
  virtual int ReqQry10308078(CReqFsQry10308078Field *p_pReqField, LONGLONG p_llRequestId);
  // 深圳债券协议回购购回金额查询请求
  virtual int ReqQry10308086(CReqFsQry10308086Field *p_pReqField, LONGLONG p_llRequestId);
  // 债券质押股份明细查询请求
  virtual int ReqQry10308042(CReqFsQry10308042Field *p_pReqField, LONGLONG p_llRequestId);
  // 债券质押式回购合约查询请求
  virtual int ReqQry10308043(CReqFsQry10308043Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请撤单请求
  virtual int Req10308604(CReqFs10308604Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF篮子委托请求
  virtual int Req10308605(CReqFs10308605Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请查询请求
  virtual int ReqQry10308057(CReqFsQry10308057Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF成分股持仓查询请求
  virtual int ReqQry10308064(CReqFsQry10308064Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF信息查询请求
  virtual int ReqQry10308052(CReqFsQry10308052Field *p_pReqField, LONGLONG p_llRequestId);
  // 上海LOF基金分拆合并信息查询请求
  virtual int ReqQry10308044(CReqFsQry10308044Field *p_pReqField, LONGLONG p_llRequestId);
  // 深圳LOF基金分拆合并信息查询请求
  virtual int ReqQry10308045(CReqFsQry10308045Field *p_pReqField, LONGLONG p_llRequestId);
  // 股权人员信息查询请求
  virtual int ReqQry10308046(CReqFsQry10308046Field *p_pReqField, LONGLONG p_llRequestId);
  // 所得税策略信息维护请求
  virtual int ReqQry10308065(CReqFsQry10308065Field *p_pReqField, LONGLONG p_llRequestId);
  // 要约收购转换关系表查询请求
  virtual int ReqQry10308058(CReqFsQry10308058Field *p_pReqField, LONGLONG p_llRequestId);
  // 上海减持股份额度查询请求
  virtual int ReqQry10308070(CReqFsQry10308070Field *p_pReqField, LONGLONG p_llRequestId);
  // 深圳减持股份额度查询请求
  virtual int ReqQry10308071(CReqFsQry10308071Field *p_pReqField, LONGLONG p_llRequestId);
  // 权证信息查询请求
  virtual int ReqQry10308084(CReqFsQry10308084Field *p_pReqField, LONGLONG p_llRequestId);
  // 三方回购篮子信息查询请求
  virtual int ReqQry10308079(CReqFsQry10308079Field *p_pReqField, LONGLONG p_llRequestId);
  // 三方回购质押表查询请求
  virtual int ReqQry10308080(CReqFsQry10308080Field *p_pReqField, LONGLONG p_llRequestId);
  // 深圳三方回购转发成交信息表查询请求
  virtual int ReqQry10308081(CReqFsQry10308081Field *p_pReqField, LONGLONG p_llRequestId);
  // 深圳三方回购合约总表查询请求
  virtual int ReqQry10308082(CReqFsQry10308082Field *p_pReqField, LONGLONG p_llRequestId);
  // 深圳三方回购合约明细查询请求
  virtual int ReqQry10308083(CReqFsQry10308083Field *p_pReqField, LONGLONG p_llRequestId);
  // H股全流通股东账户信息查询请求
  virtual int ReqQry10308104(CReqFsQry10308104Field *p_pReqField, LONGLONG p_llRequestId);
  // H股境外证券代码对应关系查询请求
  virtual int ReqQry10308105(CReqFsQry10308105Field *p_pReqField, LONGLONG p_llRequestId);
  // 询价交易非公开报价行情查询请求
  virtual int ReqQry10308106(CReqFsQry10308106Field *p_pReqField, LONGLONG p_llRequestId);
  // 历史成交查询请求
  virtual int ReqQry10418001(CReqFsQry10418001Field *p_pReqField, LONGLONG p_llRequestId);
  // 历史委托查询请求
  virtual int ReqQry10418002(CReqFsQry10418002Field *p_pReqField, LONGLONG p_llRequestId);
  // 历史交割单查询请求
  virtual int ReqQry10418003(CReqFsQry10418003Field *p_pReqField, LONGLONG p_llRequestId);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // 机构信息查询响应
  void OnRspQry10308011(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券信息查询响应
  void OnRspQry10308013(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 字典信息查询响应
  void OnRspQry10308012(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 系统状态查询响应
  void OnRspQry10308014(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证转账响应
  void OnRsp10510605(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 券商发起查银行余额响应
  void OnRspQry10510606(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询银证业务流水响应
  void OnRspQry10510608(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户内现金划转响应
  void OnRsp10510620(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 账户资金归集响应
  void OnRspQry10510621(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询转帐银行帐户响应
  void OnRspQry10510601(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 财富账户资金划拨响应
  void OnRspQry10510468(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 财富账户划拨流水查询响应
  void OnRspQry10510473(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户登录响应
  void OnRspQry10301105(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资产账户查询响应
  void OnRspQry10308112(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 财富账户查询响应
  void OnRspQry10308113(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股东查询响应
  void OnRspQry10308001(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股东交易协议查询响应
  void OnRspQry10308010(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 适当性匹配检查响应
  void OnRsp10308611(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户适当性查询响应
  void OnRspQry10308068(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 产品适当性查询响应
  void OnRspQry10308069(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金查询响应
  void OnRspQry10308048(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 总资产查询响应
  void OnRspQry10308002(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份资产查询响应
  void OnRspQry10308003(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托查询响应
  void OnRspQry10308004(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交查询响应
  void OnRspQry10308006(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交统计响应
  void OnRspQry10308053(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 最大可交易数量计算响应
  void OnRspQry10308056(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券交易买卖委托申报响应
  void OnRsp10308601(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券交易委托撤单响应
  void OnRsp10308607(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可撤单委托查询响应
  void OnRspQry10308054(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券交易批量撤单响应
  void OnRsp10308606(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 重置成本响应
  void OnRsp10308602(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 国债预发行持仓明细查询响应
  void OnRspQry10308015(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 国债预发行客户平仓明细查询响应
  void OnRspQry10308016(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 国债预发行产品信息查询响应
  void OnRspQry10308017(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股配号查询响应
  void OnRspQry10418004(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股中签查询响应
  void OnRspQry10418005(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股申购市值额度查询响应
  void OnRspQry10418006(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股信息查询响应
  void OnRspQry10418007(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购品种信息查询响应
  void OnRspQry10308019(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购质押物折算率信息查询响应
  void OnRspQry10308020(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购质押物查询响应
  void OnRspQry10308021(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购合约展期响应
  void OnRsp10308608(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购质押资金查询响应
  void OnRspQry10308022(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购预约流水查询响应
  void OnRspQry10308023(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购预约响应
  void OnRsp10308610(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可提前购回合约查询响应
  void OnRspQry10308024(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可展期取消合约查询响应
  void OnRspQry10308025(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可调整自动展期合约查询响应
  void OnRspQry10308026(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可取消提前终止预约合约查询响应
  void OnRspQry10308027(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可取消初始交易预约合约查询响应
  void OnRspQry10308028(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可展期合约查询响应
  void OnRspQry10308029(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可提前终止预约合约查询响应
  void OnRspQry10308074(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权直接还款响应
  void OnRsp10308615(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权合约查询响应
  void OnRspQry10308030(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权可还负债查询响应
  void OnRspQry10308085(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权偿还明细查询响应
  void OnRspQry10308087(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票股东大会信息查询响应
  void OnRspQry10308031(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票投票结果查询响应
  void OnRspQry10308032(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票议案信息查询响应
  void OnRspQry10308033(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票产品信息查询响应
  void OnRspQry10308059(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通价差信息查询响应
  void OnRspQry10308051(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通汇率信息查询响应
  void OnRspQry10308050(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通市场状态信息查询响应
  void OnRspQry10308061(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通标的证券信息查询响应
  void OnRspQry10308034(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交易日历查询响应
  void OnRspQry10308035(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通资金查询响应
  void OnRspQry10308047(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 大宗交易行情信息查询响应
  void OnRspQry10308036(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 盘后行情查询响应
  void OnRspQry10308108(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 受限投资者可交易证券查询响应
  void OnRspQry10308037(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转分层信息查询响应
  void OnRspQry10308067(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转询价确认信息查询响应
  void OnRspQry10308109(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转发行业务信息查询响应
  void OnRspQry10308110(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押合约总表查询响应
  void OnRspQry10308038(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押合约明细查询响应
  void OnRspQry10308039(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押初始交易参考金额查询响应
  void OnRspQry10308062(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押股份额度信息查询响应
  void OnRspQry10308063(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押标的证券查询响应
  void OnRspQry10308076(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 出借人股票质押额度查询响应
  void OnRspQry10308077(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳债券质押式协议回购合约查询响应
  void OnRspQry10308040(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券质押式协议回购标的证券查询响应
  void OnRspQry10308041(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购行情查询申报响应
  void OnRsp10308613(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购非公开报价行情查询响应
  void OnRspQry10308072(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购公开报价行情查询响应
  void OnRspQry10308073(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购未结算协议查询响应
  void OnRspQry10308075(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购合约查询响应
  void OnRspQry10308078(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳债券协议回购购回金额查询响应
  void OnRspQry10308086(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券质押股份明细查询响应
  void OnRspQry10308042(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券质押式回购合约查询响应
  void OnRspQry10308043(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请撤单响应
  void OnRsp10308604(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF篮子委托响应
  void OnRsp10308605(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请查询响应
  void OnRspQry10308057(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF成分股持仓查询响应
  void OnRspQry10308064(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF信息查询响应
  void OnRspQry10308052(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海LOF基金分拆合并信息查询响应
  void OnRspQry10308044(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳LOF基金分拆合并信息查询响应
  void OnRspQry10308045(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股权人员信息查询响应
  void OnRspQry10308046(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 所得税策略信息维护响应
  void OnRspQry10308065(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 要约收购转换关系表查询响应
  void OnRspQry10308058(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海减持股份额度查询响应
  void OnRspQry10308070(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳减持股份额度查询响应
  void OnRspQry10308071(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 权证信息查询响应
  void OnRspQry10308084(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 三方回购篮子信息查询响应
  void OnRspQry10308079(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 三方回购质押表查询响应
  void OnRspQry10308080(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳三方回购转发成交信息表查询响应
  void OnRspQry10308081(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳三方回购合约总表查询响应
  void OnRspQry10308082(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳三方回购合约明细查询响应
  void OnRspQry10308083(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H股全流通股东账户信息查询响应
  void OnRspQry10308104(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H股境外证券代码对应关系查询响应
  void OnRspQry10308105(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 询价交易非公开报价行情查询响应
  void OnRspQry10308106(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史成交查询响应
  void OnRspQry10418001(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史委托查询响应
  void OnRspQry10418002(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史交割单查询响应
  void OnRspQry10418003(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
private:
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_FS_TRADE_API_H__