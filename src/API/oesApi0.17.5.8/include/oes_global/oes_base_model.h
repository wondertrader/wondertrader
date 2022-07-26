/*
 * Copyright 2020 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    oes_base_model.h
 *
 * 订单执行系统的基础领域模型(数据结构)定义
 *
 * @version 0.11.1      2016/11/17
 * @version 0.12        2016/11/30
 *          - 增加默认佣金模板配置; 为佣金模板配置项增加通配设置
 *          - 增加所有者(OwnerType)配置, 并替换原股东账户中的 optAcctLevel 字段
 *          - 重命名字段 etfId ==> fundId
 *          - 成交回报信息中增加累计成交金额、累计交易费用字段
 *          - 委托确认信息中增加累计成交金额、累计交易费用、冻结金额、冻结费用字段
 *          - 在登录报文中增加协议版本号信息, 并在登录时校验协议版本号的兼容性
 * @version 0.12.1      2016/12/19
 *          - 修改委托请求中ordType的数据字典 eOesOrdTypeShT eOesOrdTypeSzT
 *          - 委托请求、委托确认、成交回报中的userInfo联合体中加入i64、i32类型字段
 * @version 0.12.3      2017/01/10
 *          - 股票持仓基础信息结构体与期权持仓基础信息接结构体拆分
 * @version 0.12.3.5    2017/02/20
 *          - 迁移报撤比相关的宏定义
 *          - 删除卖清仓阈值的宏定义
 *          - 委托确认、成交回报中增加延迟统计字段
 *          - 现货产品基础信息中增加“证券子类型”字段, 并且重命名“买入单位”、“卖出单位”字段
 *          - ETF产品基础信息中增加“证券类型”、“证券子类型”字段
 *          - ETF成份股基础信息中增加“证券子类型”字段
 *          - 期权产品基础信息中增加“证券子类型”字段
 * @version 0.12.6.2    2017/03/16
 *          - ‘出入金委托基础信息’ 中增加 用户私有信息(userInfo) 字段
 *          - 增加 ‘出入金委托回报基础信息’
 * @version 0.12.8      2017/04/17
 *          - 调整 ‘委托确认信息(OesOrdCnfmT)’ 中部分字段的位置
 *          - 新增 ‘委托确认信息(OesOrdCnfmT)’ 中增加深圳多交易网关特性相关OES内部字段
 *            __tgwSetIdx、__tgwIdx、__declaredTgwIdx
 *          - 新增 ‘成交确认信息(OesTrdCnfmT)’ 中增加深圳多交易网关特性相关OES内部字段
 *            __tgwSetIdx
 * @version 0.12.8.1    2017/04/24
 *          - 重命名 ‘委托确认信息(OesOrdCnfmT)’、‘成交确认信息(OesTrdCnfmT)’ 中
 *            __tgwSetIdx => __tgwGroupIdx
 * @version 0.12.8.2    2017/05/16
 *          - 新增 ‘证券发行基础信息’
 *          - 新增 ‘产品基础信息(OesStockBaseInfoT)’ 中新增字段
 *              - 新增 是否需要适当性管理(isQualificationRequired)
 *              - 新增 是否支持当日回转交易(isDayTrading)
 *              - 新增 每百元面值债券应计利息(bondInterest), 精确到元后8位
 *          - 删除 ‘产品基础信息(OesStockBaseInfoT)’ 中删除字段
 *              - 删除 卖出资金是否可用(cashRealBack)
 *              - 删除 买入股是否可卖出(hldnRealBack)
 *          - 新增 ‘股东账户基础信息(OesInvAcctBaseInfoT)’ 中新增字段 新股认购限额(subscriptionQuota)
 *          - 新增 ‘委托确认信息(OesOrdCnfmT)’ 中新增字段
 *              - 新增 冻结利息(frzInterest)
 *              - 新增 已发生利息(cumInterest)
 *          - 新增 ‘成交确认信息(OesTrdCnfmT)’ 中新增字段
 *              - 新增 已发生利息(cumInterest)
 * @version 0.15.1      2017/06/26
 *          - ‘现货产品基础信息(OesStockBaseInfoT)’ 中
 *              - 调整字段 isQualificationRequired => qualificationClass,
 *                  取值请参考 eOesQualificationClassT
 *              - 新增字段 产品风险等级(securityRiskLevel), 取值请参考 eOesSecurityRiskLevelT
 *              - 新增字段 逆回购期限(repoExpirationDays)
 *              - 新增字段 占款天数(cashHoldDays) 字段
 *          - ‘客户资金基础信息(OesCashAssetBaseInfoT)’ 中
 *              - 字段重命名 期初余额(originalBal => beginningBal)
 *              - 新增字段 期初可用余额(beginningAvailableBal)
 *              - 新增字段 期初可取余额(beginningDrawableBal)
 *              - 新增字段 当前冲正金额(红冲蓝补的资金净额, reversalAmt)
 *          - ‘客户基础信息(OesCustBaseInfoT)’ 中
 *              - 新增字段 风险等级(riskLevel)
 *              - 新增字段 原始风险等级(originRiskLevel)
 *          - ‘证券账户基础信息(OesInvAcctBaseInfoT)’ 中
 *              - 调整字段 '(券账户权限限制, acctLimit)'
 *                  - 类型 uint32 => uint64
 *                  - 重命名 acctLimit => Limits
 *              - 调整字段 '(股东权限/客户权限, acctRight)'
 *                  - 类型 uint32 => uint64
 *                  - 重命名 acctRight => permissions
 *              - 新增字段 适当性管理分类(qualificationClass)
 *          - ‘股票持仓基础信息(OesStkHoldingBaseInfoT)’ 中
 *              - 新增字段 手动冻结持仓(管理员冻结持仓净额, manualFrzHld)
 * @version 0.15.2      2017/07/18
 *          - 新增 OES配号失败原因(eOesLotRejReasonT)
 *          - 新增 新股配号、中签记录信息结构体定义(OesLotWinningBaseInfoT)
 * @version 0.15.2.1    2017/07/31
 *          - 新增 出入金业务拒绝的回报结构定义(OesFundTrsfRejectReportT)
 *          - 出入金委托回报的结构体(OesFundTrsfReportT) 中
 *              - 新增字段 错误信息(rejReasonInfo)
 * @version 0.15.2.2    2017/08/07
 *          - 出入金拒绝回报消息中调整‘错误信息’字段命名 rejReasonInfo => errorInfo
 *          - 出入金执行回报消息中调整‘错误信息’字段命名 rejReasonInfo => errorInfo
 * @version 0.15.3      2017/08/14
 *          - 新股配号、中签记录信息(OesLotWinningBaseInfoT) 中
 *              - 删除 客户代码(custId) 字段
 *              - 新增 证券名称(securityName) 字段
 *          - 证券发行基础信息(OesIssueBaseInfoT) 中
 *              - 新增 发行起始日(startDate) 字段
 *              - 新增 发行结束日(endDate) 字段
 *          - 客户基础信息(OesCustBaseInfoT) 中
 *              - 新增 机构标志(institutionFlag) 字段
 *              - 新增 投资者分类(investorClass) 字段
 *          - 证券账户基础信息(OesInvAcctBaseInfoT) 中
 *              - 删除 适当性管理分类(qualificationClass) 字段
 * @version 0.15.4      2017/09/04
 *          - '委托确认信息 (OesOrdCnfmT)' 中增加字段:
 *              - 客户端编号 (clientId)
 *              - 客户端环境号 (clEnvId)
 *              - 原始订单的客户委托流水号 (origClSeqNo)
 *              - 原始订单的客户端环境号 (origClEnvId)
 *              - 交易所订单编号 (exchOrdId)
 *          - '成交回报信息 (OesTrdCnfmT)' 中增加字段:
 *              - 客户端编号 (clientId)
 *              - 客户端环境号 (clEnvId)
 *          - '股票持仓基础信息 (OesStkHoldingBaseInfoT)' 中增加字段:
 *              - 持仓成本 (costAmount)
 *          - 整理以下结构体定义, 调整了字段顺序, 并删除了个别字段
 *              - 现货产品基础信息 (OesStockBaseInfoT)
 *              - 期权产品基础信息 (OesOptionBaseInfoT)
 *              - 证券账户基础信息 (OesInvAcctBaseInfoT)
 *          - 重命名 '出入金拒绝 (OesFundTrsfRejectReportT)' => OesFundTrsfRejectT
 * @version 0.15.4.1    2017/09/19
 *          - 为委托拒绝、撤单拒绝、出入金拒绝分别定义独立的回报消息结构体, 并各自增加委托日期和委托时间字段
 *              - 委托拒绝 (OesOrdRejectT)
 *              - 撤单拒绝 (OesOrdCancelRejectT)
 *              - 出入金拒绝 (OesFundTrsfRejectT)
 *          - '出入金基础信息 (OesFundTrsfBaseInfoT)'中
 *              - 新增 是否仅调拨 (isAllotOnly) 字段
 *          - '出入金委托回报的结构体(OesFundTrsfReportT)' 中
 *              - 新增 是否仅调拨 (isAllotOnly) 字段
 *              - 新增 资金调拨流水号 (allotSerialNo) 字段
 *          - '股票持仓基础信息 (OesStkHoldingBaseInfoT)'中
 *              - 删除 持仓成本 (costAmount) 字段
 *              - 新增 日初总持仓成本 (originalCostAmt) 字段
 *              - 新增 日中累计买入金额 (totalBuyAmt) 字段
 *              - 新增 日中累计卖出金额 (totalSellAmt) 字段
 *              - 新增 日中累计买入费用 (totalBuyFee) 字段
 *              - 新增 日中累计卖出费用 (totalSellFee) 字段
 *          - '股票持仓查询结果 (OesStkHoldingItemT)' 中
 *              - 删除 持仓成本 (costAmount) 字段
 *              - 新增 日初总持仓成本 (originalCostAmt) 字段
 *              - 新增 日中累计买入金额 (totalBuyAmt) 字段
 *              - 新增 日中累计卖出金额 (totalSellAmt) 字段
 *              - 新增 日中累计买入费用 (totalBuyFee) 字段
 *              - 新增 日中累计卖出费用 (totalSellFee) 字段
 *          - '出入金回报和查询结果 (OesFundTrsfReportT/OesFundTransferSerialItemT)' 中
 *              - 新增 主柜错误码 (counterErrCode) 字段
 *                发生主柜错误时记录具体的主柜错误码, 而错误原因(rejReason)字段将统一设置为: OESERR_COUNTER_ERR
 *          - '委托回报和查询结果 (OesOrdCnfmT/OesOrdItemT)' 中
 *              - 新增 交易所错误码 (exchErrCode) 字段
 *                发生交易所错误时记录具体的交易所错误码, 而错误原因(ordRejReason)字段将统一设置为: OESERR_EXCHANGE_ERR
 *          - 调整固定费用和佣金的费率单位, 将精度从百万分之一修改为千万分之一
 *              - OES_FEE_RATE_UNIT 从 1000000 修改为 10000000
 *          - 默认开启用于统计延时的打点信息, 并将打点信息的时间戳类型改为 timeval32 (STimeval32T) 类型,
 *            同时将回报推送的打点信息, 从消息头迁移到消息体中 (__pushingTime)
 * @version 0.15.4.2    2017/10/16
 *          - 为委托拒绝、撤单拒绝回报消息结构增加错误原因(ordRejReason)字段
 * @version 0.15.4.3    2017/10/27
 *          - '委托拒绝 (OesOrdRejectT)' 结构体中增加字段:
 *              - 客户端编号 (clientId)
 *              - 客户端环境号 (clEnvId)
 *          - '撤单拒绝(OesOrdCancelRejectT)' 结构体中新增字段:
 *              - 客户端编号 (clientId)
 *              - 客户端环境号 (clEnvId)
 *          - '出入金拒绝 (OesFundTrsfRejectT)' 结构体中增加字段:
 *              - 客户端编号 (clientId)
 *              - 客户端环境号 (clEnvId)
 * @version 0.15.5      2017/11/03
 *          - 删除结构体 '撤单拒绝 (OesOrdCancelRejectT)', 并将内容整合到 '委托拒绝 (OesOrdRejectT)' 结构体中
 *          - '委托拒绝 (OesOrdRejectT)' 结构体中增加字段:
 *              - 原始订单的客户委托流水号 (origClSeqNo)
 *              - 原始订单的客户端环境号 (origClEnvId)
 *          - '股票持仓基础信息 (OesStkHoldingBaseInfoT)' 结构体中增加字段:
 *              - 证券类型 (securityType)
 *              - 证券子类型 (subSecurityType)
 *          - '委托请求 (OesOrdReqT)' 和 '撤单请求(OesOrdCancelReqT)' 结构体中增加用于统计延时的时间戳
 *            字段 __ordReqOrigSendTime, 该字段由API在发送时自动填充, 并在委托回报中携带回客户端
 *          - 委托请求/撤单请求中增加用于统计延时的时间戳字段 __ordReqOrigSendTime,
 *            该字段由API在发送时自动填充, 并在委托回报中携带回客户端
 * @version 0.15.5.2    2018/01/07
 *          - 修复 'ETF证券基础信息 (OesEtfBaseInfoT)' 中 '前一交易 (preTradingDay)' 字段的拼写错误
 * @version 0.15.5.6    2018/04/01
 *          - 新增 市场状态信息结构定义(OesMarketStateInfoT)
 * @version 0.15.5.14   2018/08/01
 *          - 删除委托信息中已不再使用的字段 __tgwIdx、__declaredTgwIdx (协议保持兼容)
 *          - 重命名字段 __tgwGroupIdx => __tgwGrpNo (协议保持兼容)
 *          - 出入金回报及查询结构体(OesFundTrsfReportT, OesFundTransferSerialItemT)中新增
 *            内部字段 '是否已转账到主柜(__hasCounterTransfered)'
 *          - 调整委托确认结构体中的内部字段定义, 调整了__declareFlag字段的位置, 并增加了__recNum字段
 * @version 0.15.5.16   2018/08/31
 *          - 成交基础信息(OesTrdBaseInfoT) 中新增 证券子类别(subSecurityType) 字段
 *          - 客户资金基础信息(OesCashAssetBaseInfoT) 中新增 是否禁止出入金(isFundTrsfDisabled) 字段
 *          - 证券账户基础信息(OesInvAcctBaseInfoT) 中新增 是否禁止交易(isTradeDisabled) 字段
 *          - 出入金请求(OesFundTrsfReqT) 中 '是否仅调拨(isAllotOnly)'字段含义调整为出入金转账类型
 *            取值在兼容原来基础上增加类型 '主柜和银行之间转账', 参见 'eOesFundTrsfTypeT' 定义
 *          - 出入金执行回报及查询结构(OesFundTrsfReportT / OesFundTrsfRejectT / OesFundTransferSerialItemT)
 *            中 '是否仅调拨(isAllotOnly)' 字段取值也同步调整
 * @version 0.15.5.16_u3 2018/09/28
 *          - 调整成交基础信息(OesTrdBaseInfoT)和成交回报信息(OesTrdCnfmT)的结构体字段
 *              - 调整 v0.15.5.16 版本新增的 '证券子类别(subSecurityType)' 字段的位置
 *              - 增加 '原始委托数量(origOrdQty)' 和 '原始委托价格(origOrdPrice)' 字段
 * @version 0.15.7.4_u4 2018/11/26
 *          - 调整成交基础信息(OesTrdBaseInfoT)和成交回报信息(OesTrdCnfmT)的结构体字段
 *              - 增加 __isTrsfInCashAvailable 字段, 以标识ETF赎回得到的替代资金是否当日可用
 * @version 0.15.7.6    2018/11/03
 *          - 在以下结构体中增加 '产品类型 (productType)' 字段
 *              - 证券信息 (OesStockBaseInfoT/OesStockItemT)
 *              - 证券发行信息 (OesIssueBaseInfoT/OesIssueItemT)
 *              - 股票持仓信息 (OesStkHoldingBaseInfoT/OesStkHoldingItemT)
 *              - 委托回报 (OesOrdCnfmT/OesOrdItemT)
 *              - 成交回报 (OesTrdCnfmT/OesTrdItemT)
 * @version 0.15.7.6_RC2 2018/11/05
 *          - 调整交易所订单编号(exchOrdId)的字段长度, 从char[20]调整为char[17]
 *          - 再次调整委托确认结构体中 __declareFlag 字段的位置
 * @version 0.15.8      2018/09/27
 *          - 委托回报和成交回报中新增供OES内部使用的 '交易网关平台分区号(__tgwPartitionNo)' 字段 (协议保持兼容)
 * @version 0.15.9      2019/03/12
 *          - 为了支持科创板, 扩展以下数据结构 (兼容之前版本的API)
 *              - 证券账户基础信息 (OesInvAcctBaseInfoT) 中增加如下字段:
 *                  - 科创板权益 (kcSubscriptionQuota)
 *              - 现货产品基础信息 (OesStockBaseInfoT) 中增加如下字段:
 *                  - 限价买数量上限 (lmtBuyMaxQty)
 *                  - 限价买数量下限 (lmtBuyMinQty)
 *                  - 限价卖数量上限 (lmtSellMaxQty)
 *                  - 限价卖数量下限 (lmtSellMinQty)
 *                  - 市价买数量上限 (mktBuyMaxQty)
 *                  - 市价买数量下限 (mktBuyMinQty)
 *                  - 市价卖数量上限 (mktSellMaxQty)
 *                  - 市价卖数量下限 (mktSellMinQty)
 *          - 重构涨跌停价格、价格档位字段命名, 为这些字段增加新的别名 (兼容之前版本的API)
 *                  - ceilPrice => upperLimitPrice
 *                  - floorPrice => lowerLimitPrice
 *                  - priceUnit => priceTick
 * @version 0.15.9_u4   2019/11/07
 *          - 证券发行基础信息(OesIssueBaseInfoT) 中新增字段:
 *              - 停牌标识(suspFlag) 字段
 *              - 发行方式(issueType) 字段
 * @version 0.15.9.2    2019/09/29
 *          - 委托回报中增加内部字段 '重复回报标志 (__repeatFlag)'
 *          - 委托回报中增加字段 '所有者类型 (ownerType)'
 * @version 0.15.9.4    2019/12/24
 *          - 调整现货产品基础信息(OesStockBaseInfoT)的结构体定义
 *              - 增加 '总股本(outstandingShare)' 和 '流通股数量(publicFloatShare)' 字段
 *          - 调整证券账户基础信息(OesInvAcctBaseInfoT)的结构体定义
 *              - 增加 '个股持仓比例阀值(stkPositionLimitRatio)' 字段
 * @version 0.15.10.1   2020/01/17
 *          - '股票持仓基础信息 (OesStkHoldingBaseInfoT)'中
 *              - 删除 当前已锁定持仓 (lockHld) 字段
 *              - 删除 当前锁定冻结持仓 (lockFrzHld) 字段
 *              - 删除 当前解锁定冻结持仓数量 (unlockFrzHld) 字段
 *              - 删除 当前备兑冻结的现货持仓数量 (coveredFrzHld) 字段
 *              - 删除 当前已备兑使用的现货持仓数量 (coveredHld) 字段
 *              - 新增 日初可用持仓 (originalAvlHld) 字段
 *              - 新增 日初锁定持仓 (originalLockHld) 字段
 *              - 新增 日中累计锁定持仓 (totalLockHld) 字段
 *              - 新增 日中累计解锁持仓 (totalUnlockHld) 字段
 *              - 新增 当日最大可减持额度 (maxReduceQuota) 字段
 * @version 0.15.10.6   2020/04/19
 *          - 将延迟统计相关的时间戳字段升级为纳秒级时间戳 (内部使用的字段, 协议保持兼容, STimeval32T => STimespec32T)
 * @version 0.15.11     2020/05/29
 *          - 'ETF成份证券现金替代标志 (eOesEtfSubFlagT)'中
 *              - 新增 港市退补现金替代(OES_ETF_SUBFLAG_HK_REFUND_SUB) 定义
 *              - 新增 港市必须现金替代(OES_ETF_SUBFLAG_HK_MUST_SUB) 定义
 *          - 'ETF成份证券基础信息(OesEtfComponentBaseInfoT)' 中:
 *              - 新增 ETF基金市场代码 (fundMktId) 字段
 *              - 新增 是否是作为申赎对价的成份证券 (isTrdComponent) 字段
 *              - 新增 赎回折价比例 (discountRatio) 字段
 *              - 重构 申购溢价比例、赎回替代金额 字段命名, 为这些字段增加新的别名 (兼容之前版本的API)
 *                  - premiumRate => premiumRatio
 *                  - redemptionCashSub => redemptionSubCash
 *          - 为支持创业板注册制改革, '现货产品基础信息 (OesStockBaseInfoT)' 中:
 *              - 新增 证券状态 (securityStatus)
 *              - 新增 证券属性 (securityAttribute) 保留字段
 *              - 新增 是否注册制 (isRegistration)
 *              - 新增 是否为融资标的 (isCrdMarginTradeUnderlying) 字段
 *              - 新增 是否为融券标的 (isCrdShortSellUnderlying) 字段
 *              - 新增 是否为融资融券担保品 (isCrdCollateral) 字段
 *              - 新增 是否尚未盈利 (isNoProfit) 字段
 *              - 新增 是否存在投票权差异 (isWeightedVotingRights) 字段
 *              - 新增 是否具有协议控制框架 (isVie) 字段
 *              - 新增 限价买入单位 (lmtBuyQtyUnit)
 *              - 新增 限价卖出单位 (lmtSellQtyUnit)
 *              - 新增 市价买入单位 (mktBuyQtyUnit)
 *              - 新增 市价卖出单位 (mktSellQtyUnit)
 *              - 重构 面值 (parPrice) 字段, 增加新的别名 (兼容之前版本的API)
 *                  - parPrice => parValue
 *              - 新增 连续交易时段的有效竞价范围限制类型 (auctionLimitType)
 *              - 新增 连续交易时段的有效竞价范围基准价类型 (auctionReferPriceType)
 *              - 新增 连续交易时段的有效竞价范围涨跌幅度 (auctionUpDownRange)
 *              - 新增 上市日期 (listDate) 字段
 *              - 新增 到期日期 (maturityDate) 字段
 *              - 新增 基础证券代码 (underlyingSecurityId) 字段
 *          - 为支持创业板注册制改革, '证券发行基础信息(OesIssueBaseInfoT)' 中:
 *              - 新增 是否注册制 (isRegistration) 字段
 *              - 新增 证券属性 (securityAttribute) 保留字段
 *              - 新增 是否尚未盈利 (isNoProfit) 字段
 *              - 新增 是否存在投票权差异 (isWeightedVotingRights) 字段
 *              - 新增 是否具有协议控制框架 (isVie) 字段
 *              - 新增 配股股权登记日 (alotRecordDay) 字段
 *              - 新增 配股股权除权日 (alotExRightsDay) 字段
 * @version 0.15.11.1   2020/06/08
 *          - 从现货版本中删除已经过时的期权相关枚举类型和结构体定义, 包括:
 *              - 已过时的期权合约结构体 (OesOptionItemT)
 *              - 已过时的期权持仓结构体 (OesOptHoldingItemT)
 * @version 0.15.11.12  2020/09/30
 *          - '现货产品基础信息 (OesStockBaseInfoT)' 中启用 证券属性 (securityAttribute) 字段
 *          - '证券发行基础信息(OesIssueBaseInfoT)' 中启用 证券属性 (securityAttribute) 字段
 * @version 0.16        2019/11/20
 *          - '股票持仓基础信息 (OesStkHoldingBaseInfoT)'中
 *              - 删除 当前已锁定持仓 (lockHld) 字段
 *              - 删除 当前锁定冻结持仓 (lockFrzHld) 字段
 *              - 删除 当前解锁定冻结持仓数量 (unlockFrzHld) 字段
 *              - 删除 当前备兑冻结的现货持仓数量 (coveredFrzHld) 字段
 *              - 删除 当前已备兑使用的现货持仓数量 (coveredHld) 字段
 *              - 新增 日初可用持仓 (originalAvlHld) 字段
 *              - 新增 日初锁定持仓 (originalLockHld) 字段
 *              - 新增 日中累计锁定持仓 (totalLockHld) 字段
 *              - 新增 日中累计解锁持仓 (totalUnlockHld) 字段
 *          - 新增 通知消息结构定义(OesNotifyBaseInfoT)
 *          - '委托回报 (OesOrdCnfmT/OesOrdItemT)' 中
 *              - 新增 委托当前冻结的保证金 (frzMargin) 字段
 *              - 新增 委托累计已使用的保证金 (cumMargin) 字段
 *          - '成交回报 (OesTrdCnfmT/OesTrdItemT)' 中
 *              - 新增 债券利息 (trdInterest) 字段
 *              - 新增 交易费用 (trdFee) 字段
 *              - 新增 占用/释放的保证金 (trdMargin) 字段
 *              - 新增 累计占用/释放的保证金 (cumMargin) 字段
 *          - 新增 客户资金回报结构体定义(OesCashAssetReportT)
 *          - 新增 股票持仓回报结构体定义(OesStkHoldingReportT)
 *          - 资金回报 (OesCashAssetReportT) 中增加限购额度相关字段
 *              - 上海市场可用限购额度 (sseAvailablePurchaseLimit)
 *              - 深圳市场可用限购额度 (szseAvailablePurchaseLimit)
 * @version 0.17        2020/09/16
 *          - '现货产品基础信息 (OesStockBaseInfoT)' 中:
 *              - 新增 融资融券可充抵保证金证券的交易状态 (isCrdCollateralTradable) 字段
 *              - 新增 是否启用公允价格 (securityAttribute) 字段
 *              - 新增 是否为高流通性证券 (isHighLiquidity) 字段
 *              - 新增 可充抵保证金折算率 (collateralRatio) 字段
 *              - 新增 融资买入保证金比例 (marginBuyRatio) 字段
 *              - 新增 融券卖出保证金比例 (shortSellRatio) 字段
 *              - 新增 公允价格 (fairPrice) 字段
 *          - '委托拒绝 (OesOrdRejectT)' 中
 *              - 新增 业务类型 (businessType) 字段
 *          - '委托回报和委托信息 (OesOrdCnfmT/OesOrdItemT)' 中
 *              - 新增 业务类型 (businessType) 字段
 *          - '成交回报和成交信息 (OesTrdCnfmT/OesTrdItemT)' 中
 *              - 新增 业务类型 (businessType) 字段
 *          - '股票持仓回报和股票持仓信息 (OesStkHoldingReportT/OesStkHoldingItemT)' 中:
 *              - 新增 信用持仓标识 (isCreditHolding) 字段
 *              - 新增 直接还券可用持仓 (repayStockDirectAvlHld) 字段
 *              - 新增 直接还券冻结持仓 (collateralRepayDirectQty) 字段
 *              - 新增 专项证券头寸可用数量 (specialSecurityPositionAvailableQty) 字段
 *          - '资金变动回报和资金信息 (OesCashAssetReportT/OesCashAssetItemT)' 中
 *              - 新增 信用总资产 (totalAssetValue) 字段
 *              - 新增 信用总负债 (totalDebtValue) 字段
 *              - 新增 日中归还债务的累计金额 (totalRepaidAmt) 字段
 *              - 新增 日中为归还债务而在途冻结的金额 (repayFrzAmt) 字段
 *              - 新增 买担保品可用余额 (buyCollateralAvailableBal) 字段
 *              - 新增 买券还券可用余额 (repayStockAvailableBal) 字段
 *              - 新增 保证金可用余额 (marginAvailableBal) 字段
 *              - 新增 融资可用额度 (marginBuyAvailableQuota) 字段
 *              - 新增 融券可用额度 (shortSellAvailableQuota) 字段
 *              - 新增 专项资金头寸可用余额 (specialCashPositionAvailableBal) 字段
 *          - '委托回报 (OesOrdCnfmT/OesOrdItemT)' 中
 *              - 新增 归还模式 (仅适用于卖券还款委托) (repayMode) 字段
 * @version 0.17.0.8    2021/04/20
 *          - '委托回报 (OesOrdCnfmT/OesOrdItemT)' 中
 *              - 新增 强制标志 (mandatoryFlag) 字段
 *          - '成交回报 (OesTrdCnfmT/OesTrdItemT)' 中
 *              - 新增 强制标志 (mandatoryFlag) 字段
 *              - 新增 所有者类型 (ownerType) 字段
 * @version 0.17.1      2021/06/21
 *          - '资金回报 (OesCashAssetReportT)'、'资金信息查询 (OesCashAssetItemT)' 中
 *              - 删除 '买融资标的/现金还款可用余额 (buyMarginUnderlyingAvailableBal)' 字段 (可使用 '当前可用余额 (currentAvailableBal)' 字段)
 *              - 增加融资买入可用授信额度、资金头寸可用余额、证券持仓总市值等融资融券负债统计相关字段
 *              - 重命名 专项资金头寸可用余额 (cashPositionAvailableBal) => specialCashPositionAvailableBal
 *          - '持仓回报 (OesStkHoldingReportT)'、'持仓信息查询 (OesStkHoldingItemT)' 中
 *              - 调整 总持仓数量 (sumHld) 字段取值含义 => 包含担保品转出冻结和直接还券冻结在内的总持仓
 *              - 增加证券头寸可用数量、融资负债金额等融资融券负债统计相关字段
 *              - 重命名 专项证券头寸可用数量 (securityPositionAvailableQty) => specialSecurityPositionAvailableQty
 *              - 重命名 直接还券冻结的持仓数量 (repayStockDirectFrzHld) => collateralRepayDirectQty
 * @version 0.17.1.1    2021/06/23
 *          - 调整 '资金回报 (OesCashAssetReportT)'、'资金信息查询 (OesCashAssetItemT)' 结构体定义
 *              - 融资融券业务扩展字段改为使用统一的 '客户信用资产信息 (OesCrdCreditAssetBaseInfoT)' 结构体
 *          - 调整 '持仓回报 (OesStkHoldingReportT)'、'持仓信息查询 (OesStkHoldingItemT)' 结构体定义
 *              - 融资融券业务扩展字段改为使用统一的 '客户单证券融资融券负债统计信息 (OesCrdSecurityDebtStatsBaseInfoT)' 结构体
 * @version 0.17.1.3    2021/07/16
 *          - 调整 '通知消息回报 (OesNotifyInfoReportT)' 结构体定义
 *              - 新增 业务类型 (businessType) 字段
 *              - 新增 证券代码 (securityId) 字段
 *              - 新增 市场代码 (mktId) 字段
 *              - 新增 通知内容长度 (contentLen) 字段
 *              - 调整 通知内容 (content) 字段的长度
 * @version 0.17.2      2021/08/02
 *          - 调整 '出入金请求 (OesFundTrsfReqT)' 结构体定义
 *              - 重命名 出入金转账类型 (isAllotOnly => fundTrsfType), 之前的名称保持兼容
 * @version 0.17.3.1    2021/08/26
 *          - '现货产品基础信息 (OesStockBaseInfoT)' 中:
 *              - 新增 计价方式 (pricingMethod) 字段
 * @version 0.17.4      2021/09/14
 *          - '证券账户基础信息 (OesInvAcctBaseInfoT)' 结构字段变更:
 *              - 删除 个股持仓比例阀值(stkPositionLimitRatio) 字段
 *
 * @since   2015/07/30
 */


#ifndef _OES_BASE_MODEL_H
#define _OES_BASE_MODEL_H


#include    <oes_global/oes_base_constants.h>
#include    <oes_global/oes_base_model_credit.h>
#include    <oes_global/oes_base_model_option.h>
#include    <sutil/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ===================================================================
 * 委托信息的结构体定义
 * =================================================================== */

/**
 * 委托信息的基础内容定义
 */
#define __OES_ORD_BASE_INFO_PKT                                         \
        /** 客户委托流水号 (由客户端维护的递增流水, 用于识别重复的委托申报) */ \
        int32           clSeqNo; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8           mktId; \
        /** 订单类型 @see eOesOrdTypeShT eOesOrdTypeSzT */ \
        uint8           ordType; \
        /** 买卖类型 @see eOesBuySellTypeT */ \
        uint8           bsType; \
        /** 按64位对齐的填充域 */ \
        uint8           __ORD_BASE_INFO_filler; \
        \
        /** 证券账户 (可以为空, 为空则自动填充) */ \
        char            invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 证券代码 (撤单时被撤委托的证券代码可不填) */ \
        char            securityId[OES_SECURITY_ID_MAX_LEN]; \
        \
        /** 委托数量 */ \
        int32           ordQty; \
        /** 委托价格, 单位精确到元后四位, 即1元 = 10000 */ \
        int32           ordPrice; \
        /** 原始订单(待撤销的订单)的客户订单编号 (仅撤单时需要填充) */ \
        int64           origClOrdId; \
        \
        /** 用户私有信息 (由客户端自定义填充, 并在回报数据中原样返回) */ \
        union { \
            uint64      u64;                /**< uint64 类型的用户私有信息 */ \
            int64       i64;                /**< int64 类型的用户私有信息 */ \
            uint32      u32[2];             /**< uint32[2] 类型的用户私有信息 */ \
            int32       i32[2];             /**< int32[2] 类型的用户私有信息 */ \
            char        c8[8];              /**< char[8] 类型的用户私有信息 */ \
        } userInfo


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ORD_BASE_INFO_PKT                                 \
        0, 0, 0, 0, 0, \
        {0}, {0}, \
        0, 0, 0, \
        {0}
/* -------------------------           */


/**
 * 撤单请求的基础内容定义
 */
#define __OES_ORD_CANCEL_BASE_INFO_PKT                                  \
        /** 客户委托流水号 (由客户端维护的递增流水, 用于识别重复的委托申报, 必填) */ \
        int32           clSeqNo; \
        /** 市场代码 (必填) @see eOesMarketIdT */ \
        uint8           mktId; \
        /** 按64位对齐的填充域 */ \
        uint8           __ORD_CANCEL_BASE_INFO_filler1[3]; \
        \
        /** 证券账户 (选填, 若不为空则校验待撤订单是否匹配) */ \
        char            invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 证券代码 (选填, 若不为空则校验待撤订单是否匹配) */ \
        char            securityId[OES_SECURITY_ID_MAX_LEN]; \
        \
        /** 原始订单(待撤销的订单)的客户委托流水号 (若使用 origClOrdId, 则不必填充该字段) */ \
        int32           origClSeqNo; \
        /** 原始订单(待撤销的订单)的客户端环境号 (小于等于0, 则使用当前会话的 clEnvId) */ \
        int8            origClEnvId; \
        /** 按64位对齐的填充域 */ \
        uint8           __ORD_CANCEL_BASE_INFO_filler2[3]; \
        /** 原始订单(待撤销的订单)的客户订单编号 (若使用 origClSeqNo, 则不必填充该字段) */ \
        int64           origClOrdId; \
        \
        /** 用户私有信息 (由客户端自定义填充, 并在回报数据中原样返回) */ \
        union { \
            uint64      u64;                /**< uint64 类型的用户私有信息 */ \
            int64       i64;                /**< int64 类型的用户私有信息 */ \
            uint32      u32[2];             /**< uint32[2] 类型的用户私有信息 */ \
            int32       i32[2];             /**< int32[2] 类型的用户私有信息 */ \
            char        c8[8];              /**< char[8] 类型的用户私有信息 */ \
        } userInfo


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ORD_CANCEL_BASE_INFO_PKT                          \
        0, 0, {0}, \
        {0}, {0}, \
        0, 0, {0}, 0, \
        {0}
/* -------------------------           */


/**
 * 附加到请求消息末尾的, 用于延迟测量的打点信息字段 (是否启用取决于构建配置)
 */
#define __OES_ORD_REQ_LATENCY_FIELDS_IMPL                               \
        /** 委托请求的客户端原始发送时间 (OES内部使用, 由API在发送时自动填充) */ \
        STimespec32T    __ordReqOrigSendTime


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS_IMPL                       \
        {0, 0}
/* -------------------------           */


/**
 * 附加到回报消息末尾的, 用于延迟测量的打点信息字段 (是否启用取决于构建配置)
 */
#define __OES_ORD_CNFM_LATENCY_FIELDS_IMPL                              \
        /** 委托请求的初始接收时间 */ \
        STimespec32T    __ordReqOrigRecvTime; \
        /** 委托请求的入队时间 */ \
        STimespec32T    __ordReqCollectedTime; \
        /** 委托请求的实际处理开始时间 */ \
        STimespec32T    __ordReqActualDealTime; \
        /** 委托请求的处理完成时间 */ \
        STimespec32T    __ordReqProcessedTime; \
        \
        /** 委托确认的开始采集时间 */ \
        STimespec32T    __ordCnfmOrigRecvTime; \
        /** 委托确认的采集完成时间 */ \
        STimespec32T    __ordCnfmCollectedTime; \
        /** 委托确认的实际处理开始时间 */ \
        STimespec32T    __ordCnfmActualDealTime; \
        /** 委托确认的处理完成时间 */ \
        STimespec32T    __ordCnfmProcessedTime; \
        \
        /** 初始报盘时间 */ \
        STimespec32T    __ordDeclareTime; \
        /** 报盘完成时间 */ \
        STimespec32T    __ordDeclareDoneTime; \
        \
        /** 消息推送时间 (写入推送缓存以后, 实际网络发送之前) */ \
        STimespec32T    __pushingTime


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ORD_CNFM_LATENCY_FIELDS_IMPL                      \
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, \
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, \
        {0, 0}, {0, 0}, \
        {0, 0}
/* -------------------------           */


#if defined (_OES_EXPORT_LATENCY_STATS) \
        || defined (_OES_ENABLE_ORIG_SEND_TIMESTAMP)
/** 委托请求中的时间戳字段定义 (用于记录打点信息以统计延迟) */
#   define  __OES_ORD_REQ_LATENCY_FIELDS                                \
            ; \
            __OES_ORD_REQ_LATENCY_FIELDS_IMPL


/* 委托请求中的时间戳字段的初始化值定义 */
#   define  __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS                        \
            , \
            __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS_IMPL


#else
#   define  __OES_ORD_REQ_LATENCY_FIELDS
#   define  __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS


#endif  /* _OES_EXPORT_LATENCY_STATS */


#if defined (_OES_EXPORT_LATENCY_STATS)
/** 委托回报中的时间戳字段定义 (用于记录打点信息以统计延迟) */
#   define  __OES_ORD_CNFM_LATENCY_FIELDS                               \
            ; \
            __OES_ORD_CNFM_LATENCY_FIELDS_IMPL


/* 委托回报中的时间戳字段的初始化值定义 */
#   define  __NULLOBJ_OES_ORD_CNFM_LATENCY_FIELDS                       \
            , \
            __NULLOBJ_OES_ORD_CNFM_LATENCY_FIELDS_IMPL


#else
#   define  __OES_ORD_CNFM_LATENCY_FIELDS
#   define  __NULLOBJ_OES_ORD_CNFM_LATENCY_FIELDS


#endif  /* _OES_EXPORT_LATENCY_STATS */
/* -------------------------           */


/**
 * 委托确认基础信息的内容定义
 */
#define __OES_ORD_CNFM_BASE_INFO_PKT                                    \
        __OES_ORD_BASE_INFO_PKT \
        __OES_ORD_REQ_LATENCY_FIELDS; \
        \
        /** 客户订单编号 (在OES内具有唯一性的内部委托编号) */ \
        int64           clOrdId; \
        /** 客户端编号 */ \
        int16           clientId; \
        /** 客户端环境号 */ \
        int8            clEnvId; \
        /** 原始订单(待撤销的订单)的客户端环境号 (仅适用于撤单委托) */ \
        int8            origClEnvId; \
        /** 原始订单(待撤销的订单)的客户委托流水号 (仅适用于撤单委托) */ \
        int32           origClSeqNo; \
        \
        /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */ \
        int32           ordDate; \
        /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */ \
        int32           ordTime; \
        /** 委托确认时间 (格式为 HHMMSSsss, 形如 141206000) */ \
        int32           ordCnfmTime; \
        \
        /** 订单当前状态 @see eOesOrdStatusT */ \
        uint8           ordStatus; \
        /** 委托确认状态 (交易所返回的回报状态, 仅供参考)  @see eOesOrdStatusT */ \
        uint8           ordCnfmSts; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8           securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8           subSecurityType; \
        \
        /** 平台号 (OES内部使用) @see eOesPlatformIdT */ \
        uint8           __platformId; \
        /** 交易网关组序号 (OES内部使用) */ \
        uint8           __tgwGrpNo; \
        /** 交易网关平台分区号 (OES内部使用) */ \
        uint8           __tgwPartitionNo; \
        /** 产品类型 @see eOesProductTypeT */ \
        uint8           productType; \
        /** 交易所订单编号 (深交所的订单编号是16位的非数字字符串) */ \
        char            exchOrdId[OES_EXCH_ORDER_ID_MAX_LEN]; \
        /** 已报盘标志 (OES内部使用) */ \
        uint8           __declareFlag; \
        /** 重复回报标志 (OES内部使用) */ \
        uint8           __repeatFlag; \
        /** 所有者类型 @see eOesOwnerTypeT */ \
        uint8           ownerType; \
        \
        /** 委托当前冻结的交易金额 */ \
        int64           frzAmt; \
        /** 委托当前冻结的利息 */ \
        int64           frzInterest; \
        /** 委托当前冻结的交易费用 */ \
        int64           frzFee; \
        /** 委托累计已发生的交易金额 */ \
        int64           cumAmt; \
        /** 委托累计已发生的利息 */ \
        int64           cumInterest; \
        /** 委托累计已发生的交易费用 */ \
        int64           cumFee; \
        \
        /** 累计执行数量 (累计成交数量) */ \
        int32           cumQty; \
        /** 已撤单数量 */ \
        int32           canceledQty; \
        \
        /** 订单/撤单拒绝原因 */ \
        int32           ordRejReason; \
        /** 交易所错误码 */ \
        int32           exchErrCode; \
        /** PBU代码 (席位号) */ \
        int32           pbuId; \
        /** 营业部代码 */ \
        int32           branchId; \
        /** 回报记录号 (OES内部使用) */ \
        int32           __rowNum; \
        /** OIW委托编号 (OES内部使用) */ \
        uint32          __recNum \
        \
        __OES_ORD_CNFM_LATENCY_FIELDS


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ORD_CNFM_BASE_INFO_PKT                            \
        __NULLOBJ_OES_ORD_BASE_INFO_PKT \
        __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS, \
        0, 0, 0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, {0}, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, \
        0, 0, \
        0, 0, 0, 0, 0, 0 \
        __NULLOBJ_OES_ORD_CNFM_LATENCY_FIELDS
/* -------------------------           */


/**
 * 委托确认扩展信息的内容定义
 */
#define __OES_ORD_CNFM_EXT_INFO_PKT                                     \
        /** 委托当前冻结的保证金 */ \
        int64           frzMargin; \
        /** 委托累计已使用的保证金 */ \
        int64           cumMargin; \
        /** 业务类型 @see eOesBusinessTypeT */ \
        uint8           businessType; \
        /** 强制标志 @see eOesOrdMandatoryFlagT */ \
        uint8           mandatoryFlag; \
        /** 归还模式 (仅适用于卖券还款委托) @see eOesCrdDebtRepayModeT */ \
        uint8           repayMode; \
        /** 按64位对齐的填充域 */ \
        uint8           __ORD_CNFM_EXT_filler[5]; \
        /** 预留的备用字段 */ \
        char            __ORD_CNFM_EXT_reserve[16]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ORD_CNFM_EXT_INFO_PKT                             \
        0, 0, 0, 0, 0, {0}, {0}
/* -------------------------           */


/**
 * 委托请求的结构体定义
 */
typedef struct _OesOrdReq {
    __OES_ORD_BASE_INFO_PKT
    __OES_ORD_REQ_LATENCY_FIELDS;
} OesOrdReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_ORD_REQ                                             \
        __NULLOBJ_OES_ORD_BASE_INFO_PKT \
        __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS
/* -------------------------           */


/**
 * 撤单请求的结构体定义
 */
typedef struct _OesOrdCancelReq {
    __OES_ORD_CANCEL_BASE_INFO_PKT
    __OES_ORD_REQ_LATENCY_FIELDS;
} OesOrdCancelReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_ORD_CANCEL_REQ                                      \
        __NULLOBJ_OES_ORD_CANCEL_BASE_INFO_PKT \
        __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS
/* -------------------------           */


/**
 * 委托拒绝(OES业务拒绝)的结构体定义
 */
typedef struct _OesOrdReject {
    __OES_ORD_BASE_INFO_PKT
    __OES_ORD_REQ_LATENCY_FIELDS;

    /** 原始订单(待撤销的订单)的客户委托流水号 (仅适用于撤单请求) */
    int32               origClSeqNo;
    /** 原始订单(待撤销的订单)的客户端环境号 (仅适用于撤单请求) */
    int8                origClEnvId;
    /** 客户端环境号 */
    int8                clEnvId;
    /** 客户端编号 */
    int16               clientId;

    /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */
    int32               ordDate;
    /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               ordTime;

    /** 订单拒绝原因 */
    int32               ordRejReason;
    /** 业务类型 @see eOesBusinessTypeT */ \
    uint8               businessType; \
    /** 按64位对齐的填充域 */ \
    uint8               __filler[3];
} OesOrdRejectT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_ORD_REJECT                                          \
        __NULLOBJ_OES_ORD_BASE_INFO_PKT \
        __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS, \
        0, 0, 0, 0, \
        0, 0, \
        0, 0, {0}
/* -------------------------           */


/**
 * 委托确认的结构体定义
 */
typedef struct _OesOrdCnfm {
    __OES_ORD_CNFM_BASE_INFO_PKT;
    __OES_ORD_CNFM_EXT_INFO_PKT;
} OesOrdCnfmT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_ORD_CNFM                                            \
        __NULLOBJ_OES_ORD_CNFM_BASE_INFO_PKT, \
        __NULLOBJ_OES_ORD_CNFM_EXT_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 融资融券负债归还请求的结构体定义
 * =================================================================== */

/**
 * 融资融券负债归还请求 (除直接还款以外的直接还券、买券还券、卖券还款等负债归还请求)
 */
typedef struct _OesCrdRepayReq {
    /** 委托请求信息 */
    OesOrdReqT          ordReq;

    /** 归还模式 (仅适用于归还融资负债) @see eOesCrdAssignableRepayModeT */
    uint8               repayMode;
    /** 按64位对齐的填充域 */
    uint8               __filler[7];

    /** 指定归还的合约编号 */
    char                debtId[OES_CREDIT_DEBT_ID_MAX_LEN];
} OesCrdRepayReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_REPAY_REQ                                       \
        {NULLOBJ_OES_ORD_REQ}, \
        0, {0}, \
        {0}
/* -------------------------           */


/* ===================================================================
 * 融资融券直接还款请求的结构体定义
 * =================================================================== */

/**
 * 融资融券直接还款请求的基础内容定义
 */
#define __OES_CRD_CASH_REPAY_REQ_BASE_PKT                               \
        /** 客户委托流水号 (由客户端维护的递增流水) */ \
        int32           clSeqNo; \
        /** 归还模式 @see eOesCrdAssignableRepayModeT */ \
        uint8           repayMode; \
        /** 归还指令类型 @see eOesCrdDebtJournalTypeT */ \
        uint8           repayJournalType; \
        /** 按64位对齐的填充域 */ \
        uint8           __CRD_CASH_REPAY_REQ_BASE_filler[2]; \
        /** 归还金额 (单位精确到元后四位, 即1元=10000. @note 实际还款金额会向下取整到分) */ \
        int64           repayAmt; \
        \
        /** 资金账户代码 */ \
        char            cashAcctId[OES_CASH_ACCT_ID_MAX_LEN]; \
        /** 指定归还的合约编号 */ \
        char            debtId[OES_CREDIT_DEBT_ID_MAX_LEN]; \
        \
        /** 用户私有信息 (由客户端自定义填充, 并在回报数据中原样返回) */ \
        union { \
            uint64      u64;                /**< uint64 类型的用户私有信息 */ \
            int64       i64;                /**< int64 类型的用户私有信息 */ \
            uint32      u32[2];             /**< uint32[2] 类型的用户私有信息 */ \
            int32       i32[2];             /**< int32[2] 类型的用户私有信息 */ \
            char        c8[8];              /**< char[8] 类型的用户私有信息 */ \
        } userInfo


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_CRD_CASH_REPAY_REQ_BASE_PKT                       \
        0, 0, 0, {0}, 0, \
        {0}, {0}, \
        {0}
/* -------------------------           */


/**
 * 融资融券直接还款请求定义
 */
typedef struct _OesCrdCashRepayReq {
    __OES_CRD_CASH_REPAY_REQ_BASE_PKT
    __OES_ORD_REQ_LATENCY_FIELDS;
} OesCrdCashRepayReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_CASH_REPAY_REQ                                  \
        __NULLOBJ_OES_CRD_CASH_REPAY_REQ_BASE_PKT \
        __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS
/* -------------------------           */


/**
 * 融资融券直接还款请求执行状态回报的结构体定义
 */
typedef struct _OesCrdCashRepayReport {
    __OES_CRD_CASH_REPAY_REQ_BASE_PKT
    __OES_ORD_REQ_LATENCY_FIELDS;

    /** 证券账户 (仅适用于管理端现金了结/场外了结融券负债委托回报) */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 证券代码 (仅适用于管理端现金了结/场外了结融券负债委托回报) */
    char                securityId[OES_SECURITY_ID_MAX_LEN];
    /** 市场代码 @see eOesMarketIdT */
    uint8               mktId;
    /** 按64位对齐的填充域 */
    uint8               __filler1[7];
    /** 委托价格 (公允价格, 仅适用于管理端现金了结/场外了结融券负债委托回报; 单位精确到元后四位, 即1元=10000) */
    int32               ordPrice;
    /** 归还数量 (仅适用于管理端现金了结/场外了结融券负债委托回报) */
    int32               ordQty;

    /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */
    int32               ordDate;
    /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               ordTime;

    /** 客户订单编号 (在OES内具有唯一性的内部委托编号, 只有有效的委托才会生成, 被拒绝的委托该字段为0) */
    int64               clOrdId;
    /** 客户端编号 */
    int16               clientId;
    /** 客户端环境号 */
    int8                clEnvId;
    /** 委托强制标志 */
    uint8               mandatoryFlag;

    /** 订单当前状态 @see eOesOrdStatusT */
    uint8               ordStatus;
    /** 所有者类型 (被拒绝的委托该字段为0) @see eOesOwnerTypeT */
    uint8               ownerType;
    /** 按64位对齐的填充域 */
    uint8               __filler2[2];
    /** 订单拒绝原因 */
    int32               ordRejReason;

    /** 实际归还数量 (仅适用于管理端现金了结/场外了结融券负债委托回报) */
    int32               repaidQty;
    /** 实际归还金额 (单位精确到元后四位, 即1元=10000) */
    int64               repaidAmt;
    /** 实际归还费用 (单位精确到元后四位, 即1元=10000) */
    int64               repaidFee;
    /** 实际归还利息 (单位精确到元后四位, 即1元=10000) */
    int64               repaidInterest;

    /** 营业部编号 (被拒绝的委托该字段为0) */
    int32               branchId;
    /** 按64位对齐的填充域 */
    int32               __filler3;
} OesCrdCashRepayReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_CASH_REPAY_REPORT                               \
        __NULLOBJ_OES_CRD_CASH_REPAY_REQ_BASE_PKT \
        __NULLOBJ_OES_ORD_REQ_LATENCY_FIELDS, \
        {0}, {0}, 0, {0}, 0, 0, \
        0, 0, \
        0, 0, 0, 0, \
        0, 0, {0}, 0, \
        0, 0, 0, 0, \
        0, 0
/* -------------------------           */


/* ===================================================================
 * 成交基础信息的结构体定义
 * =================================================================== */

/**
 * 成交基础信息的内容定义
 */
#define __OES_TRD_BASE_INFO_PKT                                         \
        /** 交易所成交编号 (以下的6个字段是成交信息的联合索引字段) */ \
        int64           exchTrdNum; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8           mktId; \
        /** 买卖类型 (取值范围: 买/卖, 申购/赎回(仅深圳)) @see eOesBuySellTypeT */ \
        uint8           trdSide; \
        /** 平台号 (OES内部使用) @see eOesPlatformIdT */ \
        uint8           __platformId; \
        /** 成交类型 (OES内部使用) @see eOesTrdCnfmTypeT */ \
        uint8           __trdCnfmType; \
        /** ETF成交回报顺序号 (OES内部使用), 为区分ETF成交记录而设置 (以订单为单位) */ \
        uint32          __etfTrdCnfmSeq; \
        \
        /** 股东账户代码 */ \
        char            invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 证券代码 */ \
        char            securityId[OES_SECURITY_ID_MAX_LEN]; \
        \
        /** 成交日期 (格式为 YYYYMMDD, 形如 20160830) */ \
        int32           trdDate; \
        /** 成交时间 (格式为 HHMMSSsss, 形如 141205000) */ \
        int32           trdTime; \
        /** 成交数量 */ \
        int32           trdQty; \
        /** 成交价格 (单位精确到元后四位, 即: 1元=10000) */ \
        int32           trdPrice; \
        /** 成交金额 (单位精确到元后四位, 即: 1元=10000) */ \
        int64           trdAmt; \
        \
        /** 客户订单编号 */ \
        int64           clOrdId; \
        /** 累计执行数量 */ \
        int32           cumQty; \
        /** 回报记录号 (OES内部使用) */ \
        int32           __rowNum; \
        \
        /** 交易网关组序号 (OES内部使用) */ \
        uint8           __tgwGrpNo; \
        /** ETF赎回得到的替代资金是否当日可用 (OES内部使用) */ \
        uint8           __isTrsfInCashAvailable; \
        /** 交易网关平台分区号 (OES内部使用) */ \
        uint8           __tgwPartitionNo; \
        /** 产品类型 @see eOesProductTypeT */ \
        uint8           productType; \
        /** 原始委托数量 */ \
        int32           origOrdQty; \
        \
        /** PBU代码 (席位号) */ \
        int32           pbuId; \
        /** 营业部代码 */ \
        int32           branchId


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_TRD_BASE_INFO_PKT                                 \
        0, 0, 0, 0, 0, 0, \
        {0}, {0}, \
        0, 0, 0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, 0, 0, \
        0, 0
/* -------------------------           */


/**
 * 附加的用于延迟测量的打点信息字段, 是否启用取决于构建配置
 */
#define __OES_TRD_BASE_LATENCY_FIELDS                                   \
        /** 成交确认的开始采集时间 */ \
        STimespec32T    __trdCnfmOrigRecvTime; \
        /** 成交确认的采集完成时间 */ \
        STimespec32T    __trdCnfmCollectedTime; \
        /** 成交确认的实际处理开始时间 (POC测试时会被复用于存储委托请求的原始发送时间) */ \
        STimespec32T    __trdCnfmActualDealTime; \
        /** 成交确认的处理完成时间 */ \
        STimespec32T    __trdCnfmProcessedTime; \
        \
        /** 消息推送时间 (写入推送缓存以后, 实际网络发送之前) */ \
        STimespec32T    __pushingTime


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_TRD_BASE_LATENCY_FIELDS                           \
        {0, 0}, {0, 0}, {0, 0}, {0, 0}, \
        {0, 0}
/* -------------------------           */


#if defined (_OES_EXPORT_LATENCY_STATS)
/** 成交回报中的时间戳字段定义 (用于记录打点信息以统计延迟) */
#   define  __OES_TRD_CNFM_LATENCY_FIELDS                               \
            ; \
            __OES_TRD_BASE_LATENCY_FIELDS


/* 成交回报中的时间戳字段的初始化值定义 */
#   define  __NULLOBJ_OES_TRD_CNFM_LATENCY_FIELDS                       \
            , \
            __NULLOBJ_OES_TRD_BASE_LATENCY_FIELDS


#else
#   define  __OES_TRD_CNFM_LATENCY_FIELDS
#   define  __NULLOBJ_OES_TRD_CNFM_LATENCY_FIELDS


#endif  /* _OES_EXPORT_LATENCY_STATS */
/* -------------------------           */


/**
 * 成交回报信息的内容定义
 */
#define __OES_TRD_CNFM_BASE_INFO_PKT                                    \
        __OES_TRD_BASE_INFO_PKT; \
        \
        /** 客户委托流水号 */ \
        int32           clSeqNo; \
        /** 客户端编号 */ \
        int16           clientId; \
        /** 客户端环境号 */ \
        int8            clEnvId; \
        /** 证券子类别 (为保持兼容而位置凌乱, 后续会做调整) @see eOesSubSecurityTypeT */ \
        uint8           subSecurityType; \
        \
        /** 订单当前状态 @see eOesOrdStatusT */ \
        uint8           ordStatus; \
        /** 订单类型 @see eOesOrdTypeShT eOesOrdTypeSzT */ \
        uint8           ordType; \
        /** 买卖类型 @see eOesBuySellTypeT */ \
        uint8           ordBuySellType; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8           securityType; \
        /** 原始委托价格, 单位精确到元后四位, 即1元 = 10000 */ \
        int32           origOrdPrice; \
        \
        /** 累计成交金额 */ \
        int64           cumAmt; \
        /** 累计成交利息 */ \
        int64           cumInterest; \
        /** 累计交易费用 */ \
        int64           cumFee; \
        \
        /** 用户私有信息 (由客户端自定义填充, 并在回报数据中原样返回) */ \
        union { \
            uint64      u64;                /**< uint64 类型的用户私有信息 */ \
            int64       i64;                /**< int64 类型的用户私有信息 */ \
            uint32      u32[2];             /**< uint32[2] 类型的用户私有信息 */ \
            int32       i32[2];             /**< int32[2] 类型的用户私有信息 */ \
            char        c8[8];              /**< char[8] 类型的用户私有信息 */ \
        } userInfo \
        \
        __OES_TRD_CNFM_LATENCY_FIELDS


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_TRD_CNFM_BASE_INFO_PKT                            \
        __NULLOBJ_OES_TRD_BASE_INFO_PKT, \
        0, 0, 0, 0, \
        0, 0, 0, 0, 0, \
        0, 0, 0, \
        {0} \
        __NULLOBJ_OES_TRD_CNFM_LATENCY_FIELDS
/* -------------------------           */


/**
 * 成交回报信息的内容定义
 */
#define __OES_TRD_CNFM_EXT_INFO_PKT                                     \
        /** 债券利息 */ \
        int64           trdInterest; \
        /** 交易费用 */ \
        int64           trdFee; \
        /** 占用/释放的保证金 */ \
        int64           trdMargin; \
        /** 累计占用/释放的保证金 */ \
        int64           cumMargin; \
        /** 业务类型 @see eOesBusinessTypeT */ \
        uint8           businessType; \
        /** 强制标志 @see eOesOrdMandatoryFlagT */ \
        uint8           mandatoryFlag; \
        /** 所有者类型 @see eOesOwnerTypeT */ \
        uint8           ownerType; \
        /** 按64位对齐的填充域 */ \
        uint8           __TRD_CNFM_EXT_filler[5]; \
        /** 预留的备用字段 */ \
        char            __TRD_CNFM_EXT_reserve[16]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_TRD_CNFM_EXT_INFO_PKT                             \
        0, 0, 0, 0, 0, 0, 0, {0}, {0}
/* -------------------------           */


/**
 * 成交基础信息的结构体定义
 */
typedef struct _OesTrdBaseInfo {
    __OES_TRD_BASE_INFO_PKT;
} OesTrdBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_TRD_BASE_INFO                                       \
        __NULLOBJ_OES_TRD_BASE_INFO_PKT
/* -------------------------           */


/**
 * 成交回报结构体定义
 */
typedef struct _OesTrdCnfm {
    __OES_TRD_CNFM_BASE_INFO_PKT;
    __OES_TRD_CNFM_EXT_INFO_PKT;
} OesTrdCnfmT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_TRD_CNFM                                            \
        __NULLOBJ_OES_TRD_CNFM_BASE_INFO_PKT, \
        __NULLOBJ_OES_TRD_CNFM_EXT_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 新股配号、中签记录信息 (OesLotWinningBaseInfo) 定义
 * =================================================================== */

/**
 * 新股配号、中签记录信息的内容定义
 */
#define __OES_LOT_WINNING_BASE_INFO_PKT                                 \
        /** 证券账户 */ \
        char            invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 配号代码/中签代码 */ \
        char            securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8           mktId; \
        /** 记录类型 @see eOesLotTypeT */ \
        uint8           lotType; \
        /**
         * 失败原因, 当且仅当 lotType 为 OES_LOT_TYPE_FAILED 时此字段有效
         * @see eOesLotRejReasonT
         */ \
        uint8           rejReason; \
        /** 按64位对齐的填充域 */ \
        int8            __LOT_WINNING_BASE_INFO_filler; \
        /** 配号日期/中签日期 (格式为 YYYYMMDD, 形如 20160830) */ \
        int32           lotDate; \
        \
        /** 证券名称 (UTF-8 编码) */ \
        char            securityName[OES_SECURITY_NAME_MAX_LEN]; \
        \
        /** 起始配号号码 (当为中签记录时此字段固定为0) */ \
        int64           assignNum; \
        /** 配号成功数量/中签股数 */ \
        int32           lotQty; \
        /** 最终发行价, 单位精确到元后四位, 即1元 = 10000。当为配号记录时此字段值固定为0 */ \
        int32           lotPrice; \
        /** 中签金额, 单位精确到元后四位, 即1元 = 10000。当为配号记录时此字段值固定为0 */ \
        int64           lotAmt


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_LOT_WINNING_BASE_INFO_PKT                         \
        {0}, {0}, \
        0, 0, 0, 0, 0, \
        {0}, \
        0, 0, 0, 0
/* -------------------------           */


/**
 * 新股配号、中签记录信息定义
 */
typedef struct _OesLotWinningBaseInfo {
    __OES_LOT_WINNING_BASE_INFO_PKT;
} OesLotWinningBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_LOT_WINNING_BASE_INFO                               \
        __NULLOBJ_OES_LOT_WINNING_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 出入金信息的结构体定义
 * =================================================================== */

/**
 * 出入金委托基础信息的内容定义
 */
#define __OES_FUND_TRSF_BASE_INFO_PKT                                   \
        /** 客户委托流水号 (由客户端维护的递增流水) */ \
        int32           clSeqNo; \
        /** 划转方向 @see eOesFundTrsfDirectT */ \
        uint8           direct; \
        union { \
            /** 出入金转账类型 @see eOesFundTrsfTypeT */ \
            uint8       fundTrsfType; \
            /** 出入金转账类型 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            uint8       isAllotOnly; \
        }; \
        /** 按64位对齐的填充域 */ \
        uint8           __FUND_TRSF_BASE_filler[2]; \
        \
        /** 资金账户代码 (可以为空, 为空则自动填充) */ \
        char            cashAcctId[OES_CASH_ACCT_ID_MAX_LEN]; \
        /** 交易密码 (沪深OES之间内部资金划拨时无需填写该字段, 其它场景该字段必填) */ \
        char            trdPasswd[OES_PWD_MAX_LEN]; \
        /**
         * 转账密码
         * - 转账方向为转入(银行转证券)时, 此密码为银行密码
         * - 转账方向为转出(证券转银行)时, 此密码为主柜资金密码
         * - OES和主柜之间划拨资金时, 此密码为主柜资金密码
         * - 沪深OES之间内部资金划拨时, 无需填写该字段
         */ \
        char            trsfPasswd[OES_PWD_MAX_LEN]; \
        \
        /**
         * 发生金额 (单位精确到元后四位, 即1元=10000)
         * @note 注意事项:
         * - 无论入金还是出金, 发生金额的取值都应为正数
         * - 精度将被自动向下舍入到分, 例如: 金额 1.9999 将被自动转换为 1.9900
         */ \
        int64           occurAmt; \
        \
        /** 用户私有信息 (由客户端自定义填充, 并在回报数据中原样返回) */ \
        union { \
            uint64      u64;                /**< uint64 类型的用户私有信息 */ \
            int64       i64;                /**< int64 类型的用户私有信息 */ \
            uint32      u32[2];             /**< uint32[2] 类型的用户私有信息 */ \
            int32       i32[2];             /**< int32[2] 类型的用户私有信息 */ \
            char        c8[8];              /**< char[8] 类型的用户私有信息 */ \
        } userInfo


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_FUND_TRSF_BASE_INFO_PKT                           \
        0, 0, {0}, {0}, \
        {0}, {0}, {0}, \
        0, {0}
/* -------------------------           */


/**
 * 出入金委托的基础信息结构体定义
 */
typedef struct _OesFundTrsfBaseInfo {
    __OES_FUND_TRSF_BASE_INFO_PKT;
} OesFundTrsfBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_FUND_TRSF_BASE_INFO                                 \
        __NULLOBJ_OES_FUND_TRSF_BASE_INFO_PKT
/* -------------------------           */


/**
 * 出入金请求定义
 */
typedef struct _OesFundTrsfReq {
    __OES_FUND_TRSF_BASE_INFO_PKT;
} OesFundTrsfReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_FUND_TRSF_REQ                                       \
        __NULLOBJ_OES_FUND_TRSF_BASE_INFO_PKT
/* -------------------------           */


/**
 * 出入金拒绝的回报结构定义 (因风控检查未通过而被OES拒绝)
 */
typedef struct _OesFundTrsfReject {
    __OES_FUND_TRSF_BASE_INFO_PKT;

    /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */
    int32               ordDate;
    /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               ordTime;

    /** 客户端编号 */
    int16               clientId;
    /** 客户端环境号 */
    int8                clEnvId;
    /** 64位对齐的填充域 */
    int8                __filler;
    /** 错误码 */
    int32               rejReason;

    /** 错误信息 */
    char                errorInfo[OES_MAX_ERROR_INFO_LEN];
} OesFundTrsfRejectT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_FUND_TRSF_REJECT                                    \
        __NULLOBJ_OES_FUND_TRSF_BASE_INFO_PKT, \
        0, 0, \
        0, 0, 0, 0, {0}
/* -------------------------           */


/**
 * 出入金委托执行状态回报的结构体定义
 */
typedef struct _OesFundTrsfReport {
    /** 客户委托流水号 (由客户端维护的递增流水) */
    int32               clSeqNo;
    /** 客户端编号 */
    int16               clientId;
    /** 客户端环境号 */
    int8                clEnvId;
    /** 划转方向 @see eOesFundTrsfDirectT */
    uint8               direct;
    /** 资金账户代码 */
    char                cashAcctId[OES_CASH_ACCT_ID_MAX_LEN];

    /**
     * 发生金额 (单位精确到元后四位, 即1元=10000)
     * @note 注意事项:
     * - 无论入金还是出金, 发生金额的取值都应为正数
     * - 精度将被自动向下舍入到分, 例如: 金额 1.9999 将被自动转换为 1.9900
     */
    int64               occurAmt;
    /** 用户私有信息 (由客户端自定义填充, 并在回报数据中原样返回) */
    union {
        uint64          u64;                /**< uint64 类型的用户私有信息 */
        int64           i64;                /**< int64 类型的用户私有信息 */
        uint32          u32[2];             /**< uint32[2] 类型的用户私有信息 */
        int32           i32[2];             /**< int32[2] 类型的用户私有信息 */
        char            c8[8];              /**< char[8] 类型的用户私有信息 */
    } userInfo;

    /** OES出入金委托编号 (在OES内具有唯一性的内部出入金委托编号) */
    int32               fundTrsfId;
    /** 柜台出入金委托编号 */
    int32               counterEntrustNo;

    /** 出入金委托日期 (格式为 YYYYMMDD, 形如 20160830) */
    int32               operDate;
    /** 出入金委托时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               operTime;
    /** 上报柜台时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               dclrTime;
    /** 柜台执行结果采集时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               doneTime;

    union { \
        /** 出入金转账类型 @see eOesFundTrsfTypeT */ \
        uint8           fundTrsfType; \
        /** 出入金转账类型 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
        uint8           isAllotOnly; \
    }; \
    /** 出入金委托执行状态 @see eOesFundTrsfStatusT */
    uint8               trsfStatus;
    /** 是否有转账到主柜 */
    uint8               __hasCounterTransfered;
    /** 指令来源 @see eOesFundTrsfSourceTypeT */
    uint8               fundTrsfSourceType;

    /** 错误原因 */
    int32               rejReason;
    /** 主柜错误码 */
    int32               counterErrCode;
    /** 按64位对齐的填充域 */
    uint32              __filler;
    /** 资金调拨流水号 */
    char                allotSerialNo[OES_MAX_ALLOT_SERIALNO_LEN];
    /** 错误信息 */
    char                errorInfo[OES_MAX_ERROR_INFO_LEN];
} OesFundTrsfReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_FUND_TRSF_REPORT                                    \
        0, 0, 0, 0, {0}, \
        0, {0}, \
        0, 0, \
        0, 0, 0, 0, \
        {0}, 0, 0, 0, \
        0, 0, 0, {0}, {0}
/* -------------------------           */


/* ===================================================================
 * 客户基础信息的结构体定义
 * =================================================================== */

/**
 * 客户基础信息的内容定义
 */
#define __OES_CUST_BASE_INFO_PKT                                        \
        /** 客户代码 */ \
        char                custId[OES_CUST_ID_MAX_LEN]; \
        \
        /** 客户类型 @see eOesCustTypeT */ \
        uint8               custType; \
        /** 客户状态 (0:正常, 非0:不正常) */ \
        uint8               status; \
        /** OES风险等级 @see eOesSecurityRiskLevelT */ \
        uint8               riskLevel; \
        /** 客户原始风险等级 */ \
        uint8               originRiskLevel; \
        /** 机构标志 (0:个人投资者, 1:机构投资者) */ \
        uint8               institutionFlag; \
        /** 投资者分类 @see eOesInvestorClassT */ \
        uint8               investorClass; \
        /** 期权账户结算单确认标志 (0:未确认, 1:已确认) */ \
        uint8               optSettlementCnfmFlag; \
        /** 按64位对齐的填充域 */ \
        uint8               __CUST_BASE_filler1; \
        \
        /** 营业部代码 */ \
        int32               branchId; \
        /** 按64位对齐的填充域 */ \
        uint32              __CUST_BASE_filler2


#define __NULLOBJ_OES_CUST_BASE_INFO_PKT                                \
        {0}, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0
/* -------------------------           */


/**
 * 客户基础信息的结构体定义
 */
typedef struct _OesCustBaseInfo {
    __OES_CUST_BASE_INFO_PKT;
} OesCustBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CUST_BASE_INFO                                      \
        __NULLOBJ_OES_CUST_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 证券账户基础信息的结构体定义
 * =================================================================== */

/**
 * 证券账户基础信息的内容定义
 */
#define __OES_INV_ACCT_BASE_INFO_PKT                                    \
        /** 股东账户代码 */ \
        char                invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 市场 @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** 账户类型 @see eOesAcctTypeT */ \
        uint8               acctType; \
        /** 账户状态, 同步于主柜或者通过MON手动设置 @see eOesAcctStatusT */ \
        uint8               status; \
        /** 股东账户的所有者类型 @see eOesOwnerTypeT */ \
        uint8               ownerType; \
        /** 投资者期权等级 @see eOesOptInvLevelT */ \
        uint8               optInvLevel; \
        /** 是否禁止交易 (仅供API查询使用) */ \
        uint8               isTradeDisabled; \
        /** 按64位对齐的填充域 */ \
        uint8               __INV_ACCT_BASE_filler[2]; \
        \
        /** 证券账户权限限制 @see OES_LIMIT_xxx */ \
        uint64              limits; \
        /** 股东权限/客户权限 @see eOesTradingPermissionT */ \
        uint64              permissions; \
        \
        /** 席位号 */ \
        int32               pbuId; \
        /** 个股持仓比例阀值 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
        int32               stkPositionLimitRatio; \
        /** 主板权益 (新股认购限额) */ \
        int32               subscriptionQuota; \
        /** 科创板权益 (新股认购限额) */ \
        int32               kcSubscriptionQuota; \
        \
        /** 预留的备用字段 */ \
        char                __INV_ACCT_BASE_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_INV_ACCT_BASE_INFO_PKT                            \
        {0}, 0, \
        0, 0, 0, 0, 0, {0}, \
        0, 0, \
        0, 0, 0, 0, \
        {0}
/* -------------------------           */


/**
 * 证券账户基础信息的结构体定义
 */
typedef struct _OesInvAcctBaseInfo {
    __OES_INV_ACCT_BASE_INFO_PKT;
} OesInvAcctBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_INV_ACCT_BASE_INFO                                  \
        __NULLOBJ_OES_INV_ACCT_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 现货产品信息(证券信息)的结构体定义
 * =================================================================== */

/**
 * 竞价交易的限价参数(涨停价/跌停价)定义
 */
typedef struct _OesPriceLimit {
    union {
        /** 涨停价 (单位精确到元后四位, 即1元 = 10000) */
        int32           upperLimitPrice;
        /** 涨停价 @deprecated 已废弃, 为了兼容旧版本而保留 */
        int32           ceilPrice;
    };

    union {
        /** 跌停价 (单位精确到元后四位, 即1元 = 10000) */
        int32           lowerLimitPrice;
        /** 跌停价 @deprecated 已废弃, 为了兼容旧版本而保留 */
        int32           floorPrice;
    };
} OesPriceLimitT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_PRICE_LIMIT                                         \
        {0}, {0}
/* -------------------------           */


/**
 * 现货产品基础信息的内容定义
 */
#define __OES_STOCK_BASE_INFO_PKT                                       \
        /** 证券代码 */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** 产品类型 @see eOesProductTypeT */ \
        uint8               productType; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 证券级别 @see eOesSecurityLevelT */ \
        uint8               securityLevel; \
        /** 证券风险等级 @see eOesSecurityRiskLevelT */ \
        uint8               securityRiskLevel; \
        /** 币种 @see eOesCurrTypeT */ \
        uint8               currType; \
        /** 投资者适当性管理分类 @see eOesQualificationClassT */ \
        uint8               qualificationClass; \
        \
        /** 证券状态 @see eOesSecurityStatusT */ \
        uint32              securityStatus; \
        /** 证券属性 @see eOesSecurityAttributeT */ \
        uint32              securityAttribute; \
        /** 禁止交易标识 (0:正常交易, 非0:禁止交易) @see eOesSecuritySuspFlagT */ \
        uint8               suspFlag; \
        /** 临时停牌标识 (0:未停牌, 1:已停牌) */ \
        uint8               temporarySuspFlag; \
        /** 是否支持当日回转交易 (0:不支持, 1:支持) */ \
        uint8               isDayTrading; \
        \
        /** 是否注册制 (0:核准制, 1:注册制) */ \
        uint8               isRegistration; \
        /** 是否为融资融券可充抵保证金证券 (0:不可充抵保证金, 1:可充抵保证金) */ \
        uint8               isCrdCollateral; \
        /** 是否为融资标的 (0:不是融资标的, 1:是融资标的) */ \
        uint8               isCrdMarginTradeUnderlying; \
        /** 是否为融券标的 (0:不是融券标的, 1:是融券标的) */ \
        uint8               isCrdShortSellUnderlying; \
        /** 是否尚未盈利 (0:已盈利, 1:未盈利 (仅适用于科创板和创业板产品)) */ \
        uint8               isNoProfit; \
        /** 是否存在投票权差异 (0:无差异, 1:存在差异 (仅适用于科创板和创业板产品)) */ \
        uint8               isWeightedVotingRights; \
        /** 是否具有协议控制框架 (0:没有, 1:有 (仅适用于创业板产品)) */ \
        uint8               isVie; \
        /** 是否为高流通性证券 (目前仅适用于融资融券业务, 融券卖出所得可买高流通性证券) */ \
        uint8               isHighLiquidity; \
        /** 融资融券可充抵保证金证券的交易状态 (0:不可交易, 1:可交易) */ \
        uint8               isCrdCollateralTradable; \
        \
        /** 计价方式 (仅适用于债券 @see eOesPricingMethodT) */ \
        uint8               pricingMethod; \
        /** 按64位对齐的填充域 */ \
        uint8               __STOCK_BASE_filler[3]; \
        \
        /** 限价参数表 (涨/跌停价格, 数组下标为当前时段标志 @see eOesTrdSessTypeT) */ \
        OesPriceLimitT      priceLimit[__OES_TRD_SESS_TYPE_MAX]; \
        union { \
            /** 最小报价单位 (单位精确到元后四位, 即1元 = 10000) */ \
            int32           priceTick; \
            /** 最小报价单位 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           priceUnit; \
        }; \
        /** 前收盘价, 单位精确到元后四位, 即1元 = 10000 */ \
        int32               prevClose; \
        \
        union { \
            /** 单笔限价买委托数量上限 */ \
            int32           lmtBuyMaxQty; \
            /** 单笔限价买委托数量上限 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           buyOrdMaxQty; \
        }; \
        union { \
            /** 单笔限价买委托数量下限 */ \
            int32           lmtBuyMinQty; \
            /** 单笔限价买委托数量下限 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           buyOrdMinQty; \
        }; \
        union { \
            /** 单笔限价买入单位 */ \
            int32           lmtBuyQtyUnit; \
            /** 单笔限价买入单位 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           buyQtyUnit; \
        }; \
        /** 单笔市价买委托数量上限 */ \
        int32               mktBuyMaxQty; \
        /** 单笔市价买委托数量下限 */ \
        int32               mktBuyMinQty; \
        /** 单笔市价买入单位 */ \
        int32               mktBuyQtyUnit; \
        \
        union { \
            /** 单笔限价卖委托数量上限 */ \
            int32           lmtSellMaxQty; \
            /** 单笔限价卖委托数量上限 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           sellOrdMaxQty; \
        }; \
        union { \
            /** 单笔限价卖委托数量下限 */ \
            int32           lmtSellMinQty; \
            /** 单笔限价卖委托数量下限 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           sellOrdMinQty; \
        }; \
        union { \
            /** 单笔限价卖出单位 */ \
            int32           lmtSellQtyUnit; \
            /** 单笔限价卖出单位 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           sellQtyUnit; \
        }; \
        /** 单笔市价卖委托数量上限 */ \
        int32               mktSellMaxQty; \
        /** 单笔市价卖委托数量下限 */ \
        int32               mktSellMinQty; \
        /** 单笔市价卖出单位 */ \
        int32               mktSellQtyUnit; \
        \
        /** 债券的每张应计利息, 单位精确到元后八位, 即应计利息1元 = 100000000 */ \
        int64               bondInterest; \
        union { \
            /** 面值, 单位精确到元后四位, 即1元 = 10000 */ \
            int64           parValue; \
            /** 面值, 单位精确到元后四位, 即1元 = 10000 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int64           parPrice; \
        }; \
        /** 逆回购期限 */ \
        int32               repoExpirationDays; \
        /** 占款天数 */ \
        int32               cashHoldDays; \
        \
        /** 连续交易时段的有效竞价范围限制类型 @see eOesAuctionLimitTypeT */ \
        uint8               auctionLimitType; \
        /** 连续交易时段的有效竞价范围基准价类型 @see eOesAuctionReferPriceTypeT */ \
        uint8               auctionReferPriceType; \
        /** 按64位对齐的填充域 */ \
        uint8               __STOCK_BASE_filler1[2]; \
        /** 连续交易时段的有效竞价范围涨跌幅度 (百分比或绝对价格, 取决于'有效竞价范围限制类型') */ \
        int32               auctionUpDownRange; \
        \
        /** 上市日期 */ \
        int32               listDate; \
        /** 到期日期 (仅适用于债券等有发行期限的产品) */ \
        int32               maturityDate; \
        /** 总股本 (即: 总发行数量, 上证无该字段, 未额外维护时取值为0) */ \
        int64               outstandingShare; \
        /** 流通股数量 */ \
        int64               publicFloatShare; \
        \
        /** 基础证券代码 (标的产品代码) */ \
        char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN]; \
        /** ETF基金申赎代码 */ \
        char                fundId[OES_SECURITY_ID_MAX_LEN]; \
        /** 证券名称 (UTF-8 编码) */ \
        char                securityName[OES_SECURITY_NAME_MAX_LEN]; \
        /** 预留的备用字段 */ \
        char                __STOCK_BASE_reserve1[80]; \
        \
        union { \
            /** 融资融券业务专用字段 */ \
            struct { \
                /** 可充抵保证金折算率 (单位:万分比) */ \
                int32       collateralRatio; \
                /** 融资买入保证金比例 (单位:万分比) */ \
                int32       marginBuyRatio; \
                /** 融券卖出保证金比例 (单位:万分比) */ \
                int32       shortSellRatio; \
                /** 公允价格, 大于0代表启用 (价格单位精确到元后四位, 即: 1元=10000) */ \
                int32       fairPrice; \
            } creditExt; \
        }; \
        /** 预留的备用字段 */ \
        char                __STOCK_BASE_reserve2[48]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_STOCK_BASE_INFO_PKT                               \
        {0}, 0, \
        0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, {0}, \
        {{NULLOBJ_OES_PRICE_LIMIT}}, \
        {0}, 0, \
        {0}, {0}, {0}, 0, 0, 0, \
        {0}, {0}, {0}, 0, 0, 0, \
        0, {0}, 0, 0, \
        0, 0, {0}, 0, \
        0, 0, 0, 0, \
        {0}, {0}, {0}, {0}, \
        {{0, 0, 0, 0}}, \
        {0}
/* -------------------------           */


/**
 * 现货产品基础信息的结构体定义
 */
typedef struct _OesStockBaseInfo {
    __OES_STOCK_BASE_INFO_PKT;
} OesStockBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_STOCK_BASE_INFO                                     \
        __NULLOBJ_OES_STOCK_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 证券发行信息的结构体定义
 * =================================================================== */

/**
 * 证券发行基础信息的内容定义
 */
#define __OES_ISSUE_BASE_INFO_PKT                                       \
        /** 证券发行代码 */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 产品类型 @see eOesProductTypeT */ \
        uint8               productType; \
        /** 发行方式 @see eOesSecurityIssueTypeT */ \
        uint8               issueType; \
        /** 是否允许撤单 */ \
        uint8               isCancelAble; \
        /** 是否允许重复认购 */ \
        uint8               isReApplyAble; \
        /** 禁止交易标识 (0:正常交易, 非0:禁止交易) @see eOesSecuritySuspFlagT */ \
        uint8               suspFlag; \
        \
        /** 证券属性 @see eOesSecurityAttributeT */ \
        uint32              securityAttribute; \
        /** 是否注册制 (0 核准制, 1 注册制) */ \
        uint8               isRegistration; \
        /** 是否尚未盈利 (0 已盈利, 1 未盈利 (仅适用于创业板产品)) */ \
        uint8               isNoProfit; \
        /** 是否存在投票权差异 (0 无差异, 1 存在差异 (仅适用于创业板产品)) */ \
        uint8               isWeightedVotingRights; \
        /** 是否具有协议控制框架 (0 没有, 1 有 (仅适用于创业板产品)) */ \
        uint8               isVie; \
        /** 按64位对齐的填充域 */ \
        uint8               __ISSUE_BASE_filler[8]; \
        \
        /** 发行起始日 */ \
        int32               startDate; \
        /** 发行结束日 */ \
        int32               endDate; \
        \
        /** 发行价格 */ \
        int32               issuePrice; \
        union { \
            /** 申购价格上限 (单位精确到元后四位, 即1元 = 10000) */ \
            int32           upperLimitPrice; \
            /** 申购价格上限 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           ceilPrice; \
        }; \
        union { \
            /** 申购价格下限 (单位精确到元后四位, 即1元 = 10000) */ \
            int32           lowerLimitPrice; \
            /** 申购价格下限 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           floorPrice; \
        }; \
        \
        /** 委托最大份数 */ \
        int32               ordMaxQty; \
        /** 委托最小份数 */ \
        int32               ordMinQty; \
        /** 委托份数单位 */ \
        int32               qtyUnit; \
        \
        /** 总发行量 */ \
        int64               issueQty; \
        /** 配股股权登记日(仅上海市场有效) */ \
        int32               alotRecordDay; \
        /** 配股股权除权日(仅上海市场有效) */ \
        int32               alotExRightsDay; \
        \
        /** 基础证券代码 (正股代码) */ \
        char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 证券名称 (UTF-8 编码) */ \
        char                securityName[OES_SECURITY_NAME_MAX_LEN]; \
        /** 预留的备用字段 */ \
        char                __ISSUE_BASE_reserve1[56]; \
        \
        /** 预留的备用字段 */ \
        char                __ISSUE_BASE_reserve2[64]


#define __NULLOBJ_OES_ISSUE_BASE_INFO_PKT                               \
        {0}, 0, \
        0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, {0}, \
        0, 0, \
        0, {0}, {0}, \
        0, 0, 0, \
        0, 0, 0, \
        {0}, {0}, {0}, \
        {0}
/* -------------------------           */


/**
 * 证券发行基础信息的结构体定义
 */
typedef struct _OesIssueBaseInfo {
    __OES_ISSUE_BASE_INFO_PKT;
} OesIssueBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_ISSUE_BASE_INFO                                     \
        __NULLOBJ_OES_ISSUE_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * ETF申赎产品基础信息的结构体定义
 * =================================================================== */

/**
 * ETF申赎产品基础信息定义
 */
#define __OES_ETF_BASE_INFO_PKT                                         \
        /** ETF基金申赎代码 */ \
        char                fundId[OES_SECURITY_ID_MAX_LEN]; \
        /** ETF基金买卖代码 */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** ETF基金市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 是否需要发布IOPV  1: 是; 0: 否 */ \
        uint8               isPublishIOPV; \
        \
        /** 交易所/基金公司的允许申购标志  1: 是; 0: 否 */ \
        uint8               isCreationAble; \
        /** 交易所/基金公司的允许赎回标志  1: 是; 0: 否 */ \
        uint8               isRedemptionAble; \
        /** 券商管理端的禁止交易标志  1: 是; 0: 否  */ \
        uint8               isDisabled; \
        /** 按64位对齐的填充域 */ \
        uint8               __ETF_BASE_filler; \
        \
        /** 成份证券数目 */ \
        int32               componentCnt; \
        /** 每个篮子 (最小申购、赎回单位) 对应的ETF份数, 即申购赎回单位 */ \
        int32               creRdmUnit; \
        /** 最大现金替代比例, 单位精确到十万分之一, 即替代比例50% = 50000 */ \
        int32               maxCashRatio; \
        /** 前一日基金的单位净值, 单位精确到元后四位, 即1元 = 10000 */ \
        int32               nav; \
        \
        /** 前一日最小申赎单位净值, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               navPerCU; \
        /** 红利金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               dividendPerCU; \
        \
        /** 当前交易日, 格式YYYYMMDD */ \
        int32               tradingDay; \
        /** 前一交易日, 格式YYYYMMDD */ \
        int32               preTradingDay; \
        /** 每个篮子的预估现金差额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               estiCashCmpoent; \
        /** 前一日现金差额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               cashCmpoent; \
        /** 单个账户当日累计申购总额限制 */ \
        int64               creationLimit; \
        /** 单个账户当日累计赎回总额限制 */ \
        int64               redemLimit; \
        /** 单个账户当日净申购总额限制 */ \
        int64               netCreationLimit; \
        /** 单个账户当日净赎回总额限制 */ \
        int64               netRedemLimit


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ETF_BASE_INFO_PKT                                 \
        {0}, {0}, 0, \
        0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0
/* -------------------------           */


/**
 * ETF申赎产品基础信息的结构体定义
 */
typedef struct _OesEtfBaseInfo {
    __OES_ETF_BASE_INFO_PKT;
} OesEtfBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_ETF_BASE_INFO                                       \
        __NULLOBJ_OES_ETF_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * ETF成份证券基础信息的结构体定义
 * =================================================================== */

/**
 * ETF成份证券基础信息定义
 */
#define __OES_ETF_COMPONENT_BASE_INFO_PKT                               \
        /** ETF基金申赎代码 */ \
        char                fundId[OES_SECURITY_ID_MAX_LEN]; \
        /** 成份证券代码 */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        \
        /** 成份证券市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        /** ETF基金市场代码 @see eOesMarketIdT */ \
        uint8               fundMktId; \
        /** 现金替代标识 @see eOesEtfSubFlagT */ \
        uint8               subFlag; \
        /** 成份证券的证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 成份证券的证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /**
         * 是否是作为申赎对价的成份证券
         * @note 注意事项:
         *  - 非申赎对价的成份证券信息仅供参考, 申赎时不能对该类成份证券进行股份计算
         *    或现金替代处理。
         *  - 例如: 深交所跨市场ETF中的沪市成份证券信息就属于非申赎对价的成份证券信息,
         *    对深交所跨市场ETF进行申赎时应使用 159900 虚拟成份券进行沪市成份证券份额
         *    的现金替代处理
         */ \
        uint8               isTrdComponent; \
        /** 按64位对齐的填充域 */ \
        uint8               __ETF_COMPONENT_BASE_filler[2]; \
        \
        /** 前收盘价格, 单位精确到元后四位, 即1元 = 10000 */ \
        int32               prevClose; \
        /** 成份证券数量 */ \
        int32               qty; \
        \
        union { \
            /** 申购溢价比例, 单位精确到十万分之一, 即溢价比例10% = 10000 */ \
            int32           premiumRatio; \
            /** 申购溢价比例 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int32           premiumRate; \
        }; \
        /** 赎回折价比例, 单位精确到十万分之一, 即折价比例10% = 10000 */ \
        int32               discountRatio; \
        \
        /** 申购替代金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               creationSubCash; \
        union { \
            /** 赎回替代金额, 单位精确到元后四位, 即1元 = 10000 */ \
            int64           redemptionSubCash; \
            /** 赎回替代金额 @deprecated 已废弃, 为了兼容旧版本而保留 */ \
            int64           redemptionCashSub; \
        }


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_ETF_COMPONENT_BASE_INFO_PKT                       \
        {0}, {0}, \
        0, 0, 0, 0, 0, 0, {0}, \
        0, 0, \
        {0}, 0, \
        0, {0}
/* -------------------------           */


/**
 * ETF成份证券基础信息的结构体定义
 */
typedef struct _OesEtfComponentBaseInfo {
    __OES_ETF_COMPONENT_BASE_INFO_PKT;
} OesEtfComponentBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_ETF_COMPONENT_BASE_INFO                             \
        __NULLOBJ_OES_ETF_COMPONENT_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 客户资金信息结构体定义
 * =================================================================== */

/**
 * 客户资金信息内容定义
 */
#define __OES_CASH_ASSET_BASE_INFO_PKT                                  \
        /** 资金账户代码 */ \
        char                cashAcctId[OES_CASH_ACCT_ID_MAX_LEN]; \
        /** 客户代码 */ \
        char                custId[OES_CUST_ID_MAX_LEN]; \
        \
        /** 币种 @see eOesCurrTypeT */ \
        uint8               currType; \
        /** 资金帐户类别(冗余自资金账户) @see eOesAcctTypeT */ \
        uint8               cashType; \
        /** 资金帐户状态(冗余自资金账户) @see eOesAcctStatusT */ \
        uint8               cashAcctStatus; \
        /** 是否禁止出入金 (仅供API查询使用) */ \
        uint8               isFundTrsfDisabled; \
        /** 按64位对齐的填充域 */ \
        uint8               __CASH_ASSET_BASE_filler[4]; \
        \
        /** 期初余额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               beginningBal; \
        /** 期初可用余额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               beginningAvailableBal; \
        /** 期初可取余额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               beginningDrawableBal; \
        \
        /** 不可用资金余额(既不可交易又不可提取), 单位精确到元后四位, 即1元 = 10000 */ \
        int64               disableBal; \
        /** 当前冲正金额(红冲蓝补的资金净额), 取值可以为负数(表示资金调出), 单位精确到元后四位, 即1元 = 10000 */ \
        int64               reversalAmt; \
        /** 手动冻结资金, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               manualFrzAmt; \
        \
        /** 日中累计存入资金金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               totalDepositAmt; \
        /** 日中累计提取资金金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               totalWithdrawAmt; \
        /** 当前提取冻结资金金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               withdrawFrzAmt; \
        \
        /** 日中累计 卖/赎回 获得的可用资金金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               totalSellAmt; \
        /** 日中累计 买/申购/逆回购 使用资金金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               totalBuyAmt; \
        /** 当前交易冻结金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               buyFrzAmt; \
        \
        /** 日中累计交易费用金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               totalFeeAmt; \
        /** 当前冻结交易费用金额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               feeFrzAmt; \
        \
        /**
         * 维持保证金金额, 单位精确到元后四位, 即1元 = 10000
         * 对于普通资金账户, 此字段为ETF申赎时的预估现金差额
         * 对于信用资金账户, 此字段固定为0
         * 对于衍生品资金账户, 此字段为当前维持的开仓保证金
         */ \
        int64               marginAmt; \
        /**
         * 在途冻结保证金金额, 单位精确到元后四位, 即1元 = 10000
         * 对于普通资金账户, 此字段为ETF申赎在途时冻结的预估现金差额
         * 对于信用资金账户, 此字段固定为0
         * 对于衍生品资金账户, 此字段为尚未成交的开仓保证金
         */ \
        int64               marginFrzAmt


#define __NULLOBJ_OES_CASH_ASSET_BASE_INFO_PKT                          \
        {0}, {0}, \
        0, 0, 0, 0, {0}, \
        0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, \
        0, 0, \
        0, 0
/* -------------------------           */


/**
 * 客户资金回报信息的内容定义
 */
#define __OES_CASH_ASSET_RPT_INFO_PKT                                   \
        /**
         * 当前余额 (总现金资产), 包括当前可用余额和在途冻结资金在內的汇总值, 单位精确到元后四位, 即1元 = 10000
         * - @note 可用余额请参考 '当前可用余额 (currentAvailableBal)' 字段
         */ \
        int64               currentTotalBal; \
        /**
         * 当前可用余额, 单位精确到元后四位, 即1元 = 10000
         * - 对于信用资金账户, 该字段表示现金还款/买融资标的可用余额
         */ \
        int64               currentAvailableBal; \
        /** 当前可取余额, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               currentDrawableBal; \
        \
        /** 日中沪深结点内部划拨的累计净发生资金 (正数代表累计净划入, 负数代表累计净划出), 单位精确到元后四位, 即1元 = 10000 */ \
        int64               totalInternalAllotAmt; \
        /** 日中沪深结点内部划拨的在途资金 (正数代表在途划入, 负数代表在途划出), 单位精确到元后四位, 即1元 = 10000 */ \
        int64               internalAllotUncomeAmt; \
        /** 预留的备用字段 */ \
        char                __CASH_ASSET_RPT_reserve[16]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_CASH_ASSET_RPT_INFO_PKT                           \
        0, 0, 0, \
        0, 0, {0}
/* -------------------------           */


/**
 * 客户资金基础信息结构体定义
 */
typedef struct _OesCashAssetBaseInfo {
    __OES_CASH_ASSET_BASE_INFO_PKT;
} OesCashAssetBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CASH_ASSET_BASE_INFO                                \
        __NULLOBJ_OES_CASH_ASSET_BASE_INFO_PKT
/* -------------------------           */


/**
 * 客户资金回报结构体定义
 *
 * @note 可用余额等信息参考如下字段:
 * - 总现金资产请参考 '当前余额 (currentTotalBal)' 字段
 * - 可用余额请参考 '当前可用余额 (currentAvailableBal)' 字段
 * - 可取余额请参考 '当前可取余额 (currentDrawableBal)' 字段
 * - 信用系统现金还款/买融资标的可用余额请参考 '当前可用余额 (currentAvailableBal)' 字段
 * - 信用系统买担保品可用余额请参考 '买担保品可用余额 (creditExt.buyCollateralAvailableBal)' 字段
 * - 信用系统买券还券可用余额请参考 '买券还券可用余额 (creditExt.repayStockAvailableBal)' 字段
 */
typedef struct _OesCashAssetReport {
    __OES_CASH_ASSET_BASE_INFO_PKT;
    __OES_CASH_ASSET_RPT_INFO_PKT;

    /*
     * 仅适用于融资融券业务和期权业务的扩展字段
     * @note 现货业务的资金回报中不会携带以下扩展字段, 不要读写和操作这些扩展字段
     */
    union {
        /** 融资融券业务专用字段 (即: 客户信用资产信息; @note 非两融业务不要使用这些字段) */
        OesCrdCreditAssetBaseInfoT
                        creditExt;

        /** 期权业务专用字段 (@note 非期权业务不要使用这些字段) */
        struct {
            /** 日初实际占用保证金, 单位精确到元后四位, 即1元 = 10000 */
            int64       initialMargin;
            /** 行权累计待交收冻结资金, 单位精确到元后四位, 即1元 = 10000 */
            int64       totalExerciseFrzAmt;
            /** 待追加保证金, 单位精确到元后四位, 即1元 = 10000 */
            int64       pendingSupplMargin;

            /** 上海市场可用限购/套保额度 */
            int64       sseAvailablePurchaseLimit;
            /** 深圳市场可用限购/套保额度 */
            int64       szseAvailablePurchaseLimit;

            /** 未对冲实时价格保证金, 单位精确到元后四位, 即1元 = 10000 */
            int64       totalMarketMargin;
            /** 已对冲实时价格保证金, 单位精确到元后四位, 即1元 = 10000 */
            int64       totalNetMargin;
        } optionExt;

        /** 预留的备用字段 (@note 不要直接使用该字段) */
        char            __CASH_ASSET_EXT_reserve[512];
    };
} OesCashAssetReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CASH_ASSET_REPORT                                   \
        __NULLOBJ_OES_CASH_ASSET_BASE_INFO_PKT, \
        __NULLOBJ_OES_CASH_ASSET_RPT_INFO_PKT, \
        {{NULLOBJ_OES_CRD_CREDIT_ASSET_BASE_INFO}}
/* -------------------------           */


/* ===================================================================
 * 股票持仓信息的结构体定义
 * =================================================================== */

/**
 * 股票持仓基础信息的内容定义
 */
#define __OES_STK_HOLDING_BASE_INFO_PKT                                 \
        /** 账户代码 */ \
        char                invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 证券代码 */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 产品类型 @see eOesProductTypeT */ \
        uint8               productType; \
        /** 信用持仓标识 (0:不是信用持仓, 1:是信用持仓) */ \
        uint8               isCreditHolding; \
        /** 按64位对齐的填充域 */ \
        uint8               __HOLD_BASE_filler[3]; \
        \
        /** 日初持仓 */ \
        int64               originalHld; \
        /** 日初总持仓成本 (日初持仓成本价=日初总持仓成本/日初持仓) */ \
        int64               originalCostAmt; \
        \
        /** 日中累计买入持仓 */ \
        int64               totalBuyHld; \
        /** 日中累计卖出持仓 */ \
        int64               totalSellHld; \
        /** 当前卖出冻结持仓 */ \
        int64               sellFrzHld; \
        /** 手动冻结持仓 */ \
        int64               manualFrzHld; \
        \
        /** 日中累计买入金额 */ \
        int64               totalBuyAmt; \
        /** 日中累计卖出金额 */ \
        int64               totalSellAmt; \
        /** 日中累计买入费用 */ \
        int64               totalBuyFee; \
        /** 日中累计卖出费用 */ \
        int64               totalSellFee; \
        \
        /**
         * 日中累计转入持仓
         * - 对于现货持仓:
         *   - 表示赎回ETF获得的成份证券持仓数量, 或申购ETF获得的ETF基金持仓数量
         * - 对于信用持仓:
         *   - 表示担保品划入的在途持仓数量
         */ \
        int64               totalTrsfInHld; \
        /**
         * 日中累计转出持仓
         * - 对于现货持仓:
         *   - 表示申购ETF已使用的成份证券持仓数量, 或赎回ETF已使用的ETF基金持仓数量
         * - 对于信用持仓:
         *   - 固定为0
         */ \
        int64               totalTrsfOutHld; \
        /**
         * 当前转出冻结持仓
         * - 对于现货持仓:
         *   - 包括申购ETF在途冻结的成份证券持仓数量, 或赎回ETF在途冻结的ETF基金持仓数量
         *   - 包括融资融券业务提交担保品在途冻结的持仓数量
         * - 对于信用持仓:
         *   - 表示融资融券业务返还担保品在途冻结的持仓数量
         */ \
        int64               trsfOutFrzHld; \
        \
        /** 日初锁定持仓 (日初备兑占用的持仓数量, OES内部处理时需注意对初始值进行处理) */ \
        int64               originalLockHld; \
        /** 日中累计锁定持仓 */ \
        int64               totalLockHld; \
        /** 日中累计解锁持仓 */ \
        int64               totalUnlockHld; \
        \
        /** 日初可用持仓 */ \
        int64               originalAvlHld; \
        /**
         * 当日最大可减持额度
         * - 小于0, 不进行减持额度控制
         * - 大于或等于0, 最大可减持额度
         */ \
        int64               maxReduceQuota


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_STK_HOLDING_BASE_INFO_PKT                         \
        {0}, {0}, \
        0, 0, 0, 0, 0, {0}, \
        0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, \
        0, 0
/* -------------------------           */


/**
 * 股票持仓回报信息的内容定义
 */
#define __OES_STK_HOLDING_RPT_INFO_PKT                                  \
        /** 当前可卖持仓数量 */ \
        int64               sellAvlHld; \
        /**
         * 当前可转换和信用系统可划出持仓数量
         * - 对于ETF申赎业务:
         *   - 对于成份证券持仓, 表示申购时可以使用的成份证券数量 (现货系统)
         *   - 对于ETF基金持仓, 表示赎回时可以使用的ETF基金数量 (现货系统)
         * - 对于融资融券业务:
         *   - 表示信用系统可划出的担保品持仓数量 (信用系统)
         */ \
        int64               trsfOutAvlHld; \
        /**
         * 当前可锁定和现货系统可划出持仓数量
         * - 对于期权业务:
         *   - 表示现货系统可锁定的标的证券持仓数量 (现货系统)
         * - 对于融资融券业务:
         *   - 表示现货系统可划出的担保品持仓数量 (现货系统)
         *   - 对于信用系统, 该字段固定为0
         */ \
        int64               lockAvlHld; \
        /** 按64位对齐的填充域 (为兼容旧版本而保留) */ \
        int64               __STK_HOLDING_RPT_filler; \
        /**
         * 总持仓数量 (日初持仓数量+累计买入数量-累计卖出数量)
         * - 对于现货系统:
         *   - 包含在途卖出冻结的持仓数量
         *   - 包含在途ETF申购冻结的成份证券持仓数量
         *   - 不包含在途买入数量
         * - 对于信用系统:
         *   - 包含在途卖出冻结的持仓数量
         *   - 包含担保品转出冻结的持仓数量
         *   - 包含直接还券冻结的持仓数量
         *   - 不包含在途买入数量
         *   - 不包含在途担保品转入持仓数量
         * - @note 可卖持仓等相关字段:
         *   - 可卖持仓请参考 '当前可卖持仓数量 (sellAvlHld)' 字段
         *   - 信用系统直接还券可用持仓请参考 '直接还券可用持仓数量 (repayStockDirectAvlHld)' 字段
         *   - 信用系统可划出持仓请参考 '当前可转换和信用系统可划出持仓数量 (trsfOutAvlHld)' 字段
         */ \
        int64               sumHld; \
        /** 持仓成本价 */ \
        int64               costPrice; \
        /** 预留的备用字段 */ \
        char                __STK_HOLDING_RPT_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_STK_HOLDING_RPT_INFO_PKT                          \
        0, 0, 0, 0, 0, 0, {0}
/* -------------------------           */


/**
 * 股票持仓基础信息的结构体定义
 */
typedef struct _OesStkHoldingBaseInfo {
    __OES_STK_HOLDING_BASE_INFO_PKT;
} OesStkHoldingBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_STK_HOLDING_BASE_INFO                               \
        __NULLOBJ_OES_STK_HOLDING_BASE_INFO_PKT
/* -------------------------           */


/**
 * 股票持仓回报结构体定义
 *
 * @note 可卖持仓等信息参考如下字段:
 * - 总持仓请参考 '总持仓数量 (sumHld)' 字段
 * - 可卖持仓请参考 '当前可卖持仓数量 (sellAvlHld)' 字段
 * - ETF申赎可使用的成份证券数量（申购）和ETF基金数量（赎回）请参考 '当前可转换和信用系统可划出持仓数量 (trsfOutAvlHld)' 字段
 * - 现货系统可锁定（期权业务）持仓数量和现货系统可划出（两融业务）持仓数量请参考 '当前可锁定和现货系统可划出持仓数量 (lockAvlHld)' 字段
 * - 信用系统直接还券可用持仓请参考 '直接还券可用持仓数量 (creditExt.repayStockDirectAvlHld)' 字段
 * - 信用系统可划出持仓请参考 '当前可转换和信用系统可划出持仓数量 (trsfOutAvlHld)' 字段
 */
typedef struct _OesStkHoldingReport {
    __OES_STK_HOLDING_BASE_INFO_PKT;
    __OES_STK_HOLDING_RPT_INFO_PKT;

    /*
     * 仅适用于融资融券业务的扩展字段
     * @note 现货业务的持仓回报中不会携带以下扩展字段, 不要读写和操作这些扩展字段
     */
    union {
        /** 融资融券业务专用字段 (即: 客户单证券融资融券负债统计信息; @note 非两融业务不要使用这些字段) */
        OesCrdSecurityDebtStatsBaseInfoT
                        creditExt;

        /** 预留的备用字段 */
        char            __STK_HOLDING_EXT_reserve[432];
    };
} OesStkHoldingReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_STK_HOLDING_REPORT                                  \
        __NULLOBJ_OES_STK_HOLDING_BASE_INFO_PKT, \
        __NULLOBJ_OES_STK_HOLDING_RPT_INFO_PKT, \
        {{NULLOBJ_OES_CRD_SECURITY_DEBT_STATS_BASE_INFO}}
/* -------------------------           */


/* ===================================================================
 * 市场状态信息的结构体定义
 * =================================================================== */

/**
 * 市场状态信息的结构体定义
 */
typedef struct _OesMarketStateInfo {
    uint8               exchId;             /**< 交易所代码 @see eOesExchangeIdT */
    uint8               platformId;         /**< 交易平台类型 @see eOesPlatformIdT */
    uint8               mktId;              /**< 市场代码 @see eOesMarketIdT */
    uint8               mktState;           /**< 市场状态 @see eOesMarketStateT */
    uint8               __filler[4];        /**< 按64位对齐的填充域 */
} OesMarketStateInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_MARKET_STATE_INFO                                   \
        0, 0, 0, 0, {0}
/* -------------------------           */


/* ===================================================================
 * 通知消息的结构体定义
 * =================================================================== */

/**
 * 通知消息的内容定义
 */
#define __OES_NOTIFY_BASE_INFO_PKT                                      \
        /** 通知消息序号 */ \
        int32               notifySeqNo; \
        /** 通知消息来源 @see eOesNotifySourceT */ \
        uint8               notifySource; \
        /** 通知消息类型 @see eOesNotifyTypeT */ \
        uint8               notifyType; \
        /** 通知消息等级 @see eOesNotifyLevelT */ \
        uint8               notifyLevel; \
        /** 通知范围 @see eOesNotifyScopeT */ \
        uint8               notifyScope; \
        \
        /** 通知发出时间 (格式为 HHMMSSsss, 形如 141205000) */ \
        int32               tranTime; \
        /** 业务类型 @see eOesBusinessTypeT */ \
        uint8               businessType; \
        /** 按64位对齐的填充域 */ \
        uint8               __NOTIFY_INFO_filler1[3]; \
        \
        /** 客户代码 (仅当消息通知范围为指定客户时有效) */ \
        char                custId[OES_CUST_ID_MAX_LEN]; \
        /** 证券代码 (仅当通知消息与特定证券相关时有效) */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 (仅用于修饰证券代码) @see eOesMarketIdT */ \
        uint8               mktId; \
        /** 按64位对齐的填充域 */ \
        uint8               __NOTIFY_INFO_filler2[3]; \
        \
        /** 通知内容长度 (不包含'\0'结束符的有效字符长度) */ \
        int32               contentLen; \
        /** 通知内容 */ \
        char                content[OES_NOTIFY_CONTENT_MAX_LEN]


#define __NULLOBJ_OES_NOTIFY_BASE_INFO_PKT                              \
        0, 0, 0, 0, 0, \
        0, 0, {0}, \
        {0}, {0}, 0, {0}, \
        0, {0}
/* -------------------------           */


/**
 * 通知消息的结构体定义
 */
typedef struct _OesNotifyBaseInfo {
    __OES_NOTIFY_BASE_INFO_PKT;
} OesNotifyBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_NOTIFY_BASE_INFO                                    \
        __NULLOBJ_OES_NOTIFY_BASE_INFO_PKT
/* -------------------------           */


/**
 * 通知消息回报结构体定义
 */
typedef struct _OesNotifyInfoReport {
    __OES_NOTIFY_BASE_INFO_PKT;
} OesNotifyInfoReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_NOTIFY_INFO_REPORT                                  \
        __NULLOBJ_OES_NOTIFY_BASE_INFO_PKT
/* -------------------------           */


#ifdef __cplusplus
}
#endif

#endif  /* _OES_BASE_MODEL_H */
