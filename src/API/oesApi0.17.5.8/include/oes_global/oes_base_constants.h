/*
 * Copyright 2021 the original author or authors.
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
 * @file    oes_base_constants.h
 *
 * 订单执行系统的基础领域模型常量定义
 *
 * @version 0.11.1      2016/11/17
 * @version 0.12.1      2016/12/19
 *          - 修改委托请求中ordType的数据字典 eOesOrdTypeShT eOesOrdTypeSzT
 * @version 0.12.3.5    2017/02/20
 *          - 新增债券、基金的证券类型, 调整ETF证券类型宏定义的取值
 *          - 新增证券子类型定义
 * @version 0.12.6.2    2017/03/16
 *          - 调整 ‘买卖类型’ 中期权标的锁定(OES_BS_TYPE_UF)、期权标的解锁(OES_BS_TYPE_UU)、
 *              指定登记(OES_BS_TYPE_SSE_DESIGNATION)、指定撤消(OES_BS_TYPE_SSE_RECALL_DESIGNATION)、
 *              托管注册(OES_BS_TYPE_SZSE_DESIGNATION)、托管撤消(OES_BS_TYPE_SZSE_CANCEL_DESIGNATION)
 *              的宏值
 *          - 新增 逆回购(OES_BS_TYPE_CS => OES_BS_TYPE_REVERSE_REPO) 买卖类型
 *          - 调整 ‘费用计算模式’ 的宏定义 eOesCalFeeModeT => eOesCalcFeeModeT
 *              - 调整 OES_CAL_FEE_MODE_AMOUNT => OES_CALC_FEE_MODE_AMOUNT
 *              - 调整 OES_CAL_FEE_MODE_QTY => OES_CALC_FEE_MODE_QTY
 *              - 新增 OES_CALC_FEE_MODE_ORD (按笔数计算费用)
 *          - 重命名 ‘出入金方向’ 的宏定义 eOesCashDirectT => eOesFundTrsfDirectT
 *              - 调整 OES_CASH_DIRECT_IN => OES_FUND_TRSF_DIRECT_IN
 *              - 调整 OES_CASH_DIRECT_OUT => OES_FUND_TRSF_DIRECT_OUT
 *          - 重命名 ‘出入金委托状态’ 的宏定义
 *              - 调整 OES_CASH_TRSF_STS_RECV => OES_FUND_TRSF_STS_RECV
 *              - 调整 OES_CASH_TRSF_STS_DCLR => OES_FUND_TRSF_STS_DCLR
 *              - 调整 OES_CASH_TRSF_STS_DONE => OES_FUND_TRSF_STS_DONE
 *              - 调整 OES_CASH_TRSF_STS_INVALID_OES => OES_FUND_TRSF_STS_INVALID_OES
 *              - 调整 OES_CASH_TRSF_STS_INVALID_COUNTER => OES_FUND_TRSF_STS_INVALID_COUNTER
 *          - 调整 ‘产品级别’(eOesSecurityLevelT) 中的枚举值
 * @version 0.12.8.2    2017/05/16
 *          - 新增 ‘买卖类型’ 新股认购(OES_BS_TYPE_SUBSCRIPTION)
 *          - 重命名 如下‘买卖类型’ 定义, 原始买卖类型将被废弃
 *              - 买入 OES_BS_TYPE_B => OES_BS_TYPE_BUY
 *              - 卖出 OES_BS_TYPE_S => OES_BS_TYPE_SELL
 *              - 申购 OES_BS_TYPE_KB => OES_BS_TYPE_CREATION
 *              - 赎回 OES_BS_TYPE_KS => OES_BS_TYPE_REDEMPTION
 *              - 融资买入 OES_BS_TYPE_CB => OES_BS_TYPE_CREDIT_BUY
 *              - 融券卖出, 质押式逆回购 OES_BS_TYPE_CS => OES_BS_TYPE_CREDIT_SELL
 *              - 期权买入开仓 OES_BS_TYPE_BO => OES_BS_TYPE_BUY_OPEN
 *              - 期权买入平仓 OES_BS_TYPE_BC => OES_BS_TYPE_BUY_CLOSE
 *              - 期权卖出开仓 OES_BS_TYPE_SO => OES_BS_TYPE_SELL_OPEN
 *              - 期权卖出平仓 OES_BS_TYPE_SC => OES_BS_TYPE_SELL_CLOSE
 *              - 期权备兑开仓 OES_BS_TYPE_CO => OES_BS_TYPE_COVERED_OPEN
 *              - 期权备兑平仓 OES_BS_TYPE_CC => OES_BS_TYPE_COVERED_CLOSE
 *              - 期权行权 OES_BS_TYPE_TE => OES_BS_TYPE_OPTION_EXERCISE
 *              - 期权标的锁定 OES_BS_TYPE_UF => OES_BS_TYPE_UNDERLYING_FREEZE
 *              - 期权标的解锁 OES_BS_TYPE_UU => OES_BS_TYPE_UNDERLYING_UNFREEZE
 * @version 0.15.1      2017/06/26
 *          - 添加交易权限的枚举值定义 eOesTradingPermissionT, eOesTradingLimitT
 *          - 交易权限的元数据定义 OesTradingPermissionEntryT
 *          - 重命名枚举类型 eOesExchangeTypeT => eOesExchangeIdT
 * @version 0.15.2      2017/07/18
 *          - 新增 OES中签、配号记录类型(eOesLotTypeT)
 *          - 出入金委托状态(eOesFundTrsfStatusT) 中新增如下状态
 *              - 指令已报到主柜前待回滚(OES_FUND_TRSF_STS_DCLR_ROLLBACK)
 *              - 指令已报到主柜后待回滚(OES_FUND_TRSF_STS_DONE_ROLLBACK)
 *              - 出入金指令完成, 等待事务结束(OES_FUND_TRSF_STS_WAIT_DONE)
 * @version 0.15.2.1    2017/07/31
 *          - 重命名 ‘出入金委托状态’ 的宏定义
 *              - 调整 OES_FUND_TRSF_STS_RECV => OES_FUND_TRSF_STS_UNDECLARED
 *              - 调整 OES_FUND_TRSF_STS_DCLR => OES_FUND_TRSF_STS_DECLARED
 *              - 调整 OES_FUND_TRSF_STS_DCLR_ROLLBACK => OES_FUND_TRSF_STS_UNDECLARED_ROLLBACK
 *              - 调整 OES_FUND_TRSF_STS_DONE_ROLLBACK => OES_FUND_TRSF_STS_DECLARED_ROLLBACK
 * @version 0.15.2.2    2017/08/07
 *          - 新增 出入金执行的挂起状态(OES_FUND_TRSF_STS_SUSPENDED)
 * @version 0.15.3      2017/08/14
 *          - 新增 投资者分类(eOesInvestorClassT) 枚举定义
 * @version 0.15.4      2017/09/04
 *          - 调整枚举类型 '出入金委托状态 (eOesFundTrsfStatusT)' 的取值
 * @version 0.15.4.1    2017/09/19
 *          - 调整固定费用和佣金的费率单位, 将精度从百万分之一修改为千万分之一
 *              - OES_FEE_RATE_UNIT 从 1000000 修改为 10000000
 * @version 0.15.5      2017/11/03
 *          - 调整委托状态 eOesOrdStatusT:
 *              - 删除 'OES_ORD_STATUS_DECLARING (正报)' 状态
 *              - 重命名 OES_ORD_STATUS_NORMAL => OES_ORD_STATUS_NEW (新订单)
 *          - 调整产品风险等级 eOesSecurityRiskLevelT 中 OES_RISK_LEVEL_VERY_LOW 字段含义:
 *              - 重定义 "极低风险" => "最低风险"
 *          - 调整客户状态/证券帐户/资金账户状态 eOesAcctStatusT:
 *              - 删除 OES_ACCT_STATUS_CLOSE    (销户) 等
 *              - 增加 OES_ACCT_STATUS_DISABLED (非正常)
 *          - 删除 eOesSourceTypeT 定义
 * @version 0.15.5.2    2018/01/07
 *          - 新增 ‘出入金金额单位’ 的宏定义(OES_FUND_TRSF_UNIT)
 * @version 0.15.5.6    2018/04/01
 *          - 新增 交易平台类型定义(eOesPlatformIdT)
 * @version 0.15.5.12   2018/06/29
 *          - 新增 存托凭证对应的证券子类型 (OES_SUB_SECURITY_TYPE_STOCK_CDR)
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中新增
 *              - 存托凭证交易权限 (OES_PERMIS_CDR)
 *              - 创新企业股票交易权限 (OES_PERMIS_INNOVATION)
 * @version 0.15.5.14   2018/08/01
 *          - 证券子类别枚举(eOesSubSecurityTypeT) 中新增 存托凭证(OES_SUB_SECURITY_TYPE_STOCK_CDR) 子类别定义
 *          - 账户权限枚举(eOesTradingPermissionT) 中:
 *              - 新增 存托凭证交易权限(OES_PERMIS_CDR) 定义
 *              - 新增 创新企业股票交易权限(OES_PERMIS_INNOVATION) 定义
 *          - 新增 客户端状态定义(eOesClientStatusT) 枚举类型, 仅内部使用
 * @version 0.15.5.16   2018/08/31
 *          - 新增 ‘出入金转账类型’ 定义(eOesFundTrsfTypeT)
 * @version 0.15.6.13   2018/07/16
 *          - 新增 OES执行类型定义 (eOesExecTypeT)
 * @version 0.15.7.6    2018/11/03
 *          - 新增买卖类型
 *              - 配股认购 (OES_BS_TYPE_ALLOTMENT)
 *          - 新增 '产品类型 (eOesProductTypeT)' 定义, 作为产品和持仓的高层类别定义
 *          - 新增证券子类型
 *              - 沪伦通CDR本地交易业务产品(OES_SUB_SECURITY_TYPE_STOCK_HLTCDR)
 * @version 0.15.9      2019/03/12
 *          - 新增证券子类型
 *              - 科创板股票 (OES_SUB_SECURITY_TYPE_STOCK_KSH)
 *              - 科创板存托凭证 (OES_SUB_SECURITY_TYPE_STOCK_KCDR)
 *          - 调整上证委托类型 (eOesOrdTypeShT)
 *              - 增加 '对手方最优价格申报 (OES_ORD_TYPE_SH_MTL_BEST)' 类型 (仅适用于科创板)
 *              - 增加 '本方最优价格申报 (OES_ORD_TYPE_SH_MTL_SAMEPATY_BEST)' 类型 (仅适用于科创板)
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中新增
 *              - 科创板交易权限 (OES_PERMIS_KSH)
 * @version 0.15.11     2020/05/29
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中宏定义调整
 *              - 删除 '股票质押式回购 (OES_PERMIS_STOCK_PLEDGE_REPURCHASE)'
 *              - 删除 '质押回购 (OES_PERMIS_PLEDGE_REPURCHASE)'
 *              - 增加 '债券ETF申赎 (OES_PERMIS_BOND_ETF)'
 *              - 增加 '黄金ETF申赎 (OES_PERMIS_GOLD_ETF)'
 *              - 调整 '货币基金申赎 (OES_PERMIS_CURRENCY_ETF)' 权限描述为 '货币ETF申赎'
 *              - 重命名 '创业板交易 (OES_PERMIS_GEM)' -> '核准制创业板交易 (OES_PERMIS_GEM_UNREGISTRATION)'
 *              - 增加 '注册制创业板交易 (OES_PERMIS_GEM_REGISTRATION)'
 * @version 0.15.11.1   2020/06/08
 *          - 证券子类别枚举(eOesSubSecurityTypeT) 中新增:
 *              - 创业板存托凭证 (OES_SUB_SECURITY_TYPE_STOCK_GEMCDR)
 *              - 可交换债券 (OES_SUB_SECURITY_TYPE_BOND_EXG)
 *              - 商品期货ETF (OES_SUB_SECURITY_TYPE_ETF_COMMODITY_FUTURES)
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中新增:
 *              - 商品期货ETF申赎权限 (OES_PERMIS_COMMODITY_FUTURES_ETF)
 *          - 调整买卖类型 (eOesBuySellTypeT) 定义:
 *              - 重命名 质押式逆回购 (OES_BS_TYPE_CREDIT_SELL => OES_BS_TYPE_REVERSE_REPO)
 *              - 废弃 OES_BS_TYPE_CREDIT_SELL (融资买入或质押式逆回购)
 *              - 废弃 OES_BS_TYPE_CREDIT_BUY (融资买入)
 * @version 0.15.11.3   2020/06/29
 *          - 证券类别枚举(eOesSecurityTypeT) 中新增 管理类(OES_SECURITY_TYPE_MGR) 类别定义
 *          - 证券子类别枚举(eOesSubSecurityTypeT) 中新增
 *              - 指定登记(OES_SUB_SECURITY_TYPE_MGR_SSE_DESIGNATION) 子类别定义
 *              - 指定撤消(OES_SUB_SECURITY_TYPE_MGR_SSE_RECALL_DESIGNATION) 子类别定义
 *              - 托管注册(OES_SUB_SECURITY_TYPE_MGR_SZSE_DESIGNATION) 子类别定义
 *              - 托管撤消(OES_SUB_SECURITY_TYPE_MGR_SZSE_CANCEL_DESIGNATION) 子类别定义
 * @version 0.15.11.6   2020/07/21
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中新增:
 *              - 创业板存托凭证交易权限 (OES_PERMIS_GEMCDR)
 *              - 创业板创新企业股票交易权限 (OES_PERMIS_GEM_INNOVATION)
 * @version 0.15.11.12  2020/09/30
 *          - 新增 证券属性定义(eOesSecurityAttributeT) 枚举类型
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中新增:
 *              - 可转换公司债券交易权限 (OES_PERMIS_CONVERTIBLE_BOND)
 *              - 基础设施基金交易权限 (OES_PERMIS_INFRASTRUCTURE_FUND)
 * @version 0.15.11.15  2020/11/20
 *          - 证券子类别枚举(eOesSubSecurityTypeT) 中:
 *              - 新增 基础设施基金(OES_SUB_SECURITY_TYPE_FUND_REITS)
 *          - 股东账户交易权限枚举(eOesTradingPermissionT) 中:
 *              - 字段重命名 基础设施基金交易权限(OES_PERMIS_INFRASTRUCTURE_FUND => OES_PERMIS_REITS)
 *          - 证券属性枚举(eOesSecurityAttributeT) 中:
 *              - 删除 基础设施基金(OES_SECURITY_ATTR_INFRASTRUCTURE_FUND)
 * @version 0.15.11.16  2021/02/26
 *          - 证券属性枚举(eOesSecurityAttributeT) 中:
 *              - 新增 科创板标记(OES_SECURITY_ATTR_KSH)
 *              - 废弃 科创板ETF/科创板LOF(OES_SECURITY_ATTR_KSH_FUND)
 * @version 0.15.12     2021/03/19
 *          - 市场类型枚举(eOesMarketIdT) 中:
 *              - 新增 港股(OES_MKT_EXT_HK) 定义, 仅用于跨沪深港ETF的成分股查询
 * @version 0.15.12.1   2021/04/19
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中:
 *              - 删除 融资行权(OES_PERMIS_XXX4)
 * @version 0.16        2019/11/20
 *          - 证券类别枚举(eOesSecurityTypeT) 中新增 管理类(OES_SECURITY_TYPE_MGR) 类别定义
 *          - 证券子类别枚举(eOesSubSecurityTypeT) 中新增
 *              - 指定登记(OES_SUB_SECURITY_TYPE_MGR_SSE_DESIGNATION) 子类别定义
 *              - 指定撤消(OES_SUB_SECURITY_TYPE_MGR_SSE_RECALL_DESIGNATION) 子类别定义
 *              - 托管注册(OES_SUB_SECURITY_TYPE_MGR_SZSE_DESIGNATION) 子类别定义
 *              - 托管撤消(OES_SUB_SECURITY_TYPE_MGR_SZSE_CANCEL_DESIGNATION) 子类别定义
 *          - 调整委托状态 eOesOrdStatusT:
 *              - 重命名 OES_ORD_STATUS_UNDEFINE => OES_ORD_STATUS_PENDING (待处理 (仅内部使用))
 *          - 交易限制的枚举(eOesTradingLimitT) 中新增
 *              - 禁止买入开仓 (OES_LIMIT_BUY_OPEN)
 *              - 禁止卖出平仓 (OES_LIMIT_SELL_CLOSE)
 *              - 禁止卖出开仓 (OES_LIMIT_SELL_OPEN)
 *              - 禁止买入平仓 (OES_LIMIT_BUY_CLOSE)
 *              - 禁止备兑开仓 (OES_LIMIT_COVERED_OPEN)
 *              - 禁止备兑平仓 (OES_LIMIT_COVERED_CLOSE)
 *              - 禁止标的锁定 (OES_LIMIT_UNDERLYING_FREEZE)
 *              - 禁止标的解锁 (OES_LIMIT_UNDERLYING_UNFREEZE)
 *              - 禁止期权行权 (OES_LIMIT_OPTION_EXERCISE)
 *          - 新增 通知消息结构定义(OesNotifyBaseInfoT)
 *          - 新增 通知消息的枚举值定义(eOesNotifySourceT, eOesNotifyTypeT, eOesNotifyLevelT, eOesNotifyScopeT)
 * @version 0.16.0.1    2019/12/12
 *          - 新增上证期权业务委托类型枚举定义 (eOesOrdTypeShOptT)
 * @version 0.16.1      2020/06/05
 *          - 调整买卖类型 (eOesBuySellTypeT) 定义:
 *              - 重命名 质押式逆回购 (OES_BS_TYPE_CREDIT_SELL => OES_BS_TYPE_REVERSE_REPO)
 *              - 废弃 OES_BS_TYPE_CREDIT_SELL (融资买入或质押式逆回购)
 *              - 废弃 OES_BS_TYPE_CREDIT_BUY (融资买入)
 * @version 0.17        2020/09/16
 *          - 买卖类型(eOesBuySellTypeT) 中宏定义调整
 *              - 删除 融资买入 (OES_BS_TYPE_CREDIT_BUY)
 *              - 重命名 质押式逆回购 (OES_BS_TYPE_CREDIT_SELL => OES_BS_TYPE_REVERSE_REPO)
 *              - 新增 信用担保品买入 (OES_BS_TYPE_COLLATERAL_BUY)
 *              - 新增 信用担保品卖出 (OES_BS_TYPE_COLLATERAL_SELL)
 *              - 新增 信用担保品转入 (OES_BS_TYPE_COLLATERAL_TRANSFER_IN)
 *              - 新增 信用担保品转出 (OES_BS_TYPE_COLLATERAL_TRANSFER_OUT)
 *              - 新增 信用融资买入 (OES_BS_TYPE_MARGIN_BUY)
 *              - 新增 信用卖券还款 (OES_BS_TYPE_REPAY_MARGIN_BY_SELL)
 *              - 新增 信用融券卖出 (OES_BS_TYPE_SHORT_SELL)
 *              - 新增 信用买券还券 (OES_BS_TYPE_REPAY_STOCK_BY_BUY)
 *              - 新增 信用直接还券 (OES_BS_TYPE_REPAY_STOCK_DIRECT)
 *          - 删除交易限制枚举类型(eOesTradingLimitT), 改为使用宏定义, 并新增如下交易限制
 *              - 删除 禁止回购融资 (OES_LIMIT_REPO)
 *              - 新增 禁止担保品划入 (OES_LIMIT_COLLATERAL_TRANSFER_IN)
 *              - 新增 禁止担保品划出 (OES_LIMIT_COLLATERAL_TRANSFER_OUT)
 *              - 新增 禁止融资买入 (OES_LIMIT_MARGIN_BUY)
 *              - 新增 禁止卖券还款 (OES_LIMIT_REPAY_MARGIN_BY_SELL)
 *              - 新增 禁止融券卖出 (OES_LIMIT_SHORT_SELL)
 *              - 新增 禁止买券还券 (OES_LIMIT_REPAY_STOCK_BY_BUY)
 *          - 费用类型(eOesFeeTypeT) 中宏定义调整
 *              - 新增 两融业务息费 (OES_FEE_TYPE_BROKER_CREDIT_INTEREST)
 *          - 交易业务范围(eOesBusinessTypeT) 中宏定义调整
 *              - 新增 信用业务 (OES_BUSINESS_TYPE_CREDIT)
 *          - 通知消息类型(eOesNotifyTypeT) 中宏定义调整
 *              - 新增 融资融券担保品信息更新 (OES_NOTIFY_TYPE_CRD_COLLATERAL_INFO_UPDATE)
 *              - 新增 融资融券标的信息更新 (OES_NOTIFY_TYPE_CRD_UNDERLYING_INFO_UPDATE)
 * @version 0.17.1      2021/06/21
 *          - 融资融券负债流水类型枚举(eOesCrdDebtJournalTypeT) 中:
 *              - 新增 合约展期(OES_CRD_DEBT_JOURNAL_TYPE_CONTRACT_POST_PONE)
 * @version 0.17.1.1    2021/06/22
 *          - 提取单独的基础常量定义文件
 * @version 0.17.1.2    2021/07/12
 *          - 通知消息类型(eOesNotifyTypeT) 中宏定义调整
 *              - 新增 融资融券资金头寸信息更新 (OES_NOTIFY_TYPE_CRD_CASH_POSITION_UPDATE)
 *              - 新增 融资融券证券头寸信息更新 (OES_NOTIFY_TYPE_CRD_SECURITY_POSITION_UPDATE)
 * @version 0.17.1.3    2021/07/16
 *          - 通知消息类型(eOesNotifyTypeT) 中宏定义调整
 *              - 新增 融资融券维持担保比例更新 (OES_NOTIFY_TYPE_CRD_MAINTENANCE_RATIO_UPDATE)
 *              - 新增 融资融券授信额度更新 (OES_NOTIFY_TYPE_CRD_LINE_OF_CERDIT_UPDATE)
 * @version 0.17.2      2021/08/02
 *          - 出入金转账类型(eOesFundTrsfTypeT) 中宏定义调整
 *              - 新增 沪深OES之间的内部资金划拨 (OES_FUND_TRSF_TYPE_OES_TO_OES)
 * @version 0.17.3.1    2021/08/26
 *          - 新增 计价方式(eOesPricingMethodT) 枚举类型定义
 *          - 证券禁止交易标识(eOesSecuritySuspFlagT) 中宏定义调整
 *              - 新增 因闭市而禁止交易 (OES_SUSPFLAG_MARKET_CLOSE)
 *          - 交易平台类型(eOesPlatformIdT) 中宏定义调整
 *              - 新增 国际市场互联平台 (暂未对接) (OES_PLATFORM_INTERNATIONAL_MARKET)
 *              - 新增 新债券交易平台 (OES_PLATFORM_BOND_TRADING)
 * @version 0.17.4.3    2021/11/05
 *          - 证券交易限制 中常量定义调整
 *              - 新增 禁止市价委托 (OES_LIMIT_MARKET_ORDER)
 *              - 新增 禁止交易上交所新债券平台 (OES_LIMIT_SSE_BOND_PLATFORM)
 * @version 0.17.4.7    2022/04/02
 *          - 更新证券交易限制的部分描述:
 *              - 禁止交易上交所新债券平台 => 禁止交易沪市债券
 *              - 禁止交易可转债 => 禁止买入可转债
 * @version 0.17.4.8    2022/05/13
 *          - 股东账户交易权限枚举(eOesTradingPermissionT)中新增:
 *              - 公司债/企业债交易权限 (OES_PERMIS_CORPORATE_BOND)
 * @version 0.17.5      2021/09/30
 *          - 投资者分类(eOesInvestorClassT) 中:
 *              - 新增 专业投资者(OES_INVESTOR_CLASS_PROFESSIONAL)
 *
 * @since   0.17.1.1    2021/06/22
 */


#ifndef _OES_BASE_CONSTANTS_H
#define _OES_BASE_CONSTANTS_H


#include    <sutil/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ===================================================================
 * 预编译宏处理
 * =================================================================== */

/* 默认开启用于统计延时的打点信息 */
#if ! defined (_OES_DISABLE_LATENCY_STATS)
#   ifndef  _OES_EXPORT_LATENCY_STATS
#       define  _OES_EXPORT_LATENCY_STATS   1
#   endif

#   ifndef  _OES_ENABLE_LATENCY_STATS
#       define  _OES_ENABLE_LATENCY_STATS   1
#   endif
#endif
/* -------------------------           */


/* ===================================================================
 * 常量定义
 * =================================================================== */

/** 客户端名称最大长度 */
#define OES_CLIENT_NAME_MAX_LEN             (32)
/** 客户端说明最大长度 */
#define OES_CLIENT_DESC_MAX_LEN             (32)
/** 客户端标签最大长度 */
#define OES_CLIENT_TAG_MAX_LEN              (32)
/** 密码最大长度 */
#define OES_PWD_MAX_LEN                     (40)
/** 协议版本号的最大长度 */
#define OES_VER_ID_MAX_LEN                  (32)
/** 发送方/接收方代码字符串的最大长度 */
#define OES_MAX_COMP_ID_LEN                 (32)
/** 系统支持的最大客户端环境号数量 */
#define OES_MAX_CLIENT_ENVID_COUNT          (128)
/** 批量委托的每批次最大委托数量 @deprecated 已过时 */
#define OES_MAX_BATCH_ORDERS_COUNT          (500)
/** 批量委托接口建议的每批次最大委托数量 (@note 超过该数量将自动拆分为多次提交, 可能会因为超出流量限制而导致部分委托被服务器端拒绝) */
#define OES_MAX_BATCH_ORDERS_SINGLE_COMMIT  (300)
/** 批量委托接口允许同时下单的最大委托数量上限 */
#define OES_MAX_BATCH_ORDERS_UPPER_LIMIT    (10000)

/** 客户代码最大长度 */
#define OES_CUST_ID_MAX_LEN                 (16)
/** 客户代码真实长度 */
#define OES_CUST_ID_REAL_LEN                (12)
/** 客户名称最大长度 */
#define OES_CUST_NAME_MAX_LEN               (64)
/** 客户长名称最大长度 */
#define OES_CUST_LONG_NAME_MAX_LEN          (128)

/** 资金账户代码最大长度 */
#define OES_CASH_ACCT_ID_MAX_LEN            (16)
/** 资金账户代码的实际长度 */
#define OES_CASH_ACCT_ID_REAL_LEN           (12)

/** 股东账户代码最大长度 */
#define OES_INV_ACCT_ID_MAX_LEN             (16)
/** 股东账户代码实际长度 */
#define OES_INV_ACCT_ID_REAL_LEN            (10)

/** 营业部代码最大长度 */
#define OES_BRANCH_ID_MAX_LEN               (8)
/** 营业部代码实际长度 */
#define OES_BRANCH_ID_REAL_LEN              (5)

/** 银行代码最大长度 */
#define OES_BANK_NO_MAX_LEN                 (8)
/** 银行代码实际使用长度 */
#define OES_BANK_NO_REAL_LEN                (4)

/** PBU域长度 */
#define OES_PBU_MAX_LEN                     (8)
/** PBU实际长度 */
#define OES_PBU_REAL_LEN                    (6)

/** 系统支持的最大现货证券产品信息数量 */
#define OES_SECURITY_MAX_COUNT              (50000)
/** 证券代码的最大长度 */
#define OES_SECURITY_ID_MAX_LEN             (16)
/** 实际的股票产品代码长度 */
#define OES_STOCK_ID_REAL_LEN               (6)

/** 证券名称长度 */
#define OES_SECURITY_NAME_MAX_LEN           (24)
/** 证券名称实际长度 */
#define OES_SECURITY_NAME_REAL_LEN          (20)
/** 证券长名称长度 */
#define OES_SECURITY_LONG_NAME_MAX_LEN      (80)
/** 证券英文名称长度 */
#define OES_SECURITY_ENGLISH_NAME_MAX_LEN   (48)
/** 证券ISIN代码长度 */
#define OES_SECURITY_ISIN_CODE_MAX_LEN      (16)

/** 交易所订单编号的最大长度 */
#define OES_EXCH_ORDER_ID_MAX_LEN           (17)
/** 交易所订单编号的实际长度 (上证) */
#define OES_EXCH_ORDER_ID_SSE_LEN           (8)
/** 交易所订单编号的实际长度 (深证) */
#define OES_EXCH_ORDER_ID_SZSE_LEN          (16)

/** 点分十进制的IPv4, 字符串的最大长度 */
#define OES_MAX_IP_LEN                      (16)
/** MAC地址字符串的最大长度 */
#define OES_MAX_MAC_LEN                     (20)
/** MAC地址字符串的最大长度(按64位对齐的长度) */
#define OES_MAX_MAC_ALGIN_LEN               (24)
/** 设备序列号字符串的最大长度 */
#define OES_MAX_DRIVER_ID_LEN               (21)
/** 设备序列号字符串的最大长度(按64位对齐的长度) */
#define OES_MAX_DRIVER_ID_ALGIN_LEN         (24)

/** 测试请求标识符的最大长度 */
#define OES_MAX_TEST_REQ_ID_LEN             (32)
/** 发送时间字段(YYYYMMDD-HH:mm:SS.sss (*C21))的最大长度 */
#define OES_MAX_SENDING_TIME_LEN            (22)
/** 发送时间字段(YYYYMMDD-HH:mm:SS.sss (*C21))的实际有效数据长度 */
#define OES_REAL_SENDING_TIME_LEN           (21)

/** 错误描述信息长度 */
#define OES_MAX_ERROR_INFO_LEN              (64)
/** 消息通知内容的最大长度 */
#define OES_NOTIFY_CONTENT_MAX_LEN          (296)
/** 主柜调拨流水号信息长度 */
#define OES_MAX_ALLOT_SERIALNO_LEN          (64)

/** 资金的转换单位 */
#define OES_CASH_UNIT                       (10000)
/** 出入金的金额单位 */
#define OES_FUND_TRSF_UNIT                  (100)
/** 费用 (佣金/固定费用) 的费率单位 */
#define OES_FEE_RATE_UNIT                   (10000000)
/** ETF使用的资金百分比单位 */
#define OES_ETF_CASH_RATIO_UNIT             (100000)
/** 债券每张应计利息的转换单位 */
#define OES_BOND_INTEREST_UNIT              (100000000)
/** 个股持仓比例阀值百分比单位 @deprecated 已废弃, 为了兼容旧版本而保留 */
#define OES_STK_POSITION_LIMIT_UNIT         (1000000)

/** 基础比例单位 (万分比) */
#define OES_BASIS_POINT_RATIO_UNIT          (10000)
/** 百分比比例单位 (百分比) */
#define OES_PERCENTAGE_RATIO_UNIT           (100)
/** 千分比比例单位 (千分比) */
#define OES_PERMILLAGE_RATIO_UNIT           (1000)

/** 产品有效竞价范围涨跌幅度转换单位 */
#define OES_AUCTION_UP_DOWN_RATE_UNIT       (100)
/** 最大买卖价格, 委托价格不能等于或超过此价格 */
#define OES_MAX_BS_PRICE                    (10000 * OES_CASH_UNIT)

/** 券商名称最大长度 */
#define OES_BROKER_NAME_MAX_LEN             (128)
/** 券商保证金账户最大长度 */
#define OES_BROKER_MARGIN_ACCT_MAX_LEN      (32)
/** 券商联系电话最大长度 */
#define OES_BROKER_PHONE_MAX_LEN            (32)
/** 券商网址最大长度 */
#define OES_BROKER_WEBSITE_MAX_LEN          (256)

/** 周边应用废弃版本数目的最大个数 */
#define OES_APPL_DISCARD_VERSION_MAX_COUNT  (5)
/** 周边应用升级协议名称的最大长度 */
#define OES_APPL_UPGRADE_PROTOCOL_MAX_LEN   (32)
/* -------------------------           */


/* ===================================================================
 * 常量定义 (期权业务相关)
 * =================================================================== */

/** 系统支持的最大期权合约产品信息数量 */
#define OES_OPTION_MAX_COUNT                (10000)
/** 实际的期权产品代码长度 */
#define OES_OPTION_ID_REAL_LEN              (8)
/** 期权合约状态信息长度 */
#define OES_SECURITY_STATUS_FLAG_MAX_LEN    (8)
/** 期权合约交易代码长度 */
#define OES_CONTRACT_EXCH_ID_MAX_LEN        (24)
/** 期权合约交易代码实际长度 */
#define OES_CONTRACT_EXCH_ID_REAL_LEN       (19)

/** 期权保证金计算比例百分比单位 */
#define OES_MARGIN_RATIO_UNIT               (10000)
/** 期权监控线计算比例百分比单位 */
#define OES_LINE_RATIO_UNIT                 (10000)

/** 期权业务保证金比例最大值 */
#define OES_OPTION_MARGIN_MAX_RATIO         (99999999)
/* -------------------------           */


/* ===================================================================
 * 常量定义 (融资融券业务相关)
 * =================================================================== */

/** 融资融券合同编号最大长度 */
#define OES_CREDIT_COMPACT_ID_MAX_LEN       (32)
/** 融资融券合约编号最大长度 */
#define OES_CREDIT_DEBT_ID_MAX_LEN          (32)
/** 融资融券合约编号实际长度 */
#define OES_CREDIT_DEBT_ID_REAL_LEN         (23)

/** 融资融券保证金比例/可充抵保证金折算率单位 (万分比) */
#define OES_CREDIT_MARGIN_RATIO_UNIT        (10000)
/** 融资融券利率费率比例单位 (万分比) */
#define OES_CREDIT_INTEREST_RATIO_UNIT      (10000)
/** 融资融券维持担保比例/集中度比例单位 (千分比) */
#define OES_CREDIT_MAINTENANCE_RATIO_UNIT   (1000)
/** 融资融券业务计息天数 */
#define OES_CREDIT_INTEREST_CALC_DAYS       (360)

/** 融资融券业务维持担保比例最大值 */
#define OES_CREDIT_MAINTENANCE_MAX_RATIO    (99999999)
/* -------------------------           */


/* ===================================================================
 * 常量定义 (交易限制)
 * =================================================================== */

/** 禁止买入 */
#define OES_LIMIT_BUY                       (1ULL << 1)
/** 禁止卖出 */
#define OES_LIMIT_SELL                      (1ULL << 2)
/** 禁止撤销指定 */
#define OES_LIMIT_RECALL_DESIGNATION        (1ULL << 3)
/** 禁止转托管 */
#define OES_LIMIT_DESIGNATION               (1ULL << 4)
/** 禁止回购融资 */
#define OES_LIMIT_REPO                      (1ULL << 5)
/** 禁止质押式逆回购 */
#define OES_LIMIT_REVERSE_REPO              (1ULL << 6)
/** 禁止新股认购 (新股/可转债/可交换债认购) */
#define OES_LIMIT_SUBSCRIPTION              (1ULL << 7)
/** 禁止买入可转债 */
#define OES_LIMIT_CONVERTIBLE_BOND          (1ULL << 8)
/** 禁止市价委托 (自动根据市价权限映射) */
#define OES_LIMIT_MARKET_ORDER              (1ULL << 9)

/** 禁止买入开仓 */
#define OES_LIMIT_BUY_OPEN                  (1ULL << 10)
/** 禁止卖出平仓 */
#define OES_LIMIT_SELL_CLOSE                (1ULL << 11)
/** 禁止卖出开仓 */
#define OES_LIMIT_SELL_OPEN                 (1ULL << 12)
/** 禁止买入平仓 */
#define OES_LIMIT_BUY_CLOSE                 (1ULL << 13)
/** 禁止备兑开仓 */
#define OES_LIMIT_COVERED_OPEN              (1ULL << 14)
/** 禁止备兑平仓 */
#define OES_LIMIT_COVERED_CLOSE             (1ULL << 15)
/** 禁止标的锁定 */
#define OES_LIMIT_UNDERLYING_FREEZE         (1ULL << 16)
/** 禁止标的解锁 */
#define OES_LIMIT_UNDERLYING_UNFREEZE       (1ULL << 17)
/** 禁止期权行权 */
#define OES_LIMIT_OPTION_EXERCISE           (1ULL << 18)

/** 禁止入金 */
#define OES_LIMIT_DEPOSIT                   (1ULL << 21)
/** 禁止出金 */
#define OES_LIMIT_WITHDRAW                  (1ULL << 22)
/** 禁止交易沪市债券 (指定交易当日不支持债券交易) */
#define OES_LIMIT_SSE_BOND_PLATFORM         (1ULL << 23)
/** 禁止ETF申购 */
#define OES_LIMIT_ETF_CREATION              (1ULL << 24)
/** 禁止ETF赎回 */
#define OES_LIMIT_ETF_REDEMPTION            (1ULL << 25)

/** 禁止担保品划入 */
#define OES_LIMIT_COLLATERAL_TRANSFER_IN    (1ULL << 31)
/** 禁止担保品划出 */
#define OES_LIMIT_COLLATERAL_TRANSFER_OUT   (1ULL << 32)
/** 禁止融资买入 */
#define OES_LIMIT_MARGIN_BUY                (1ULL << 33)
/** 禁止卖券还款 */
#define OES_LIMIT_REPAY_MARGIN_BY_SELL      (1ULL << 34)
/** 禁止直接还款 */
#define OES_LIMIT_REPAY_MARGIN_DIRECT       (1ULL << 35)
/** 禁止融券卖出 */
#define OES_LIMIT_SHORT_SELL                (1ULL << 36)
/** 禁止买券还券 */
#define OES_LIMIT_REPAY_STOCK_BY_BUY        (1ULL << 37)
/** 禁止直接还券 */
#define OES_LIMIT_REPAY_STOCK_DIRECT        (1ULL << 38)
/* -------------------------           */


/*
 * 交易限制集合
 */
/** 全部限制 */
#define __OES_LIMIT_ALL                     0xFFFFFFFFFFFFFFFF

/** 现货开仓相关的交易限制集合 */
#define __OES_LIMIT_OPEN_POSITION_STK       OES_LIMIT_BUY               \
                                            | OES_LIMIT_REPO \
                                            | OES_LIMIT_REVERSE_REPO

/** 现货平仓相关的交易限制集合 */
#define __OES_LIMIT_CLOSE_POSITION_STK      OES_LIMIT_SELL

/** 期权开仓相关的交易限制集合 */
#define __OES_LIMIT_OPEN_POSITION_OPT       OES_LIMIT_BUY_OPEN          \
                                            | OES_LIMIT_SELL_OPEN \
                                            | OES_LIMIT_COVERED_OPEN \
                                            | OES_LIMIT_UNDERLYING_FREEZE

/** 期权平仓相关的交易限制集合 */
#define __OES_LIMIT_CLOSE_POSITION_OPT      OES_LIMIT_SELL_CLOSE        \
                                            | OES_LIMIT_BUY_CLOSE \
                                            | OES_LIMIT_COVERED_CLOSE \
                                            | OES_LIMIT_UNDERLYING_UNFREEZE

/** 信用开仓相关的交易限制集合 */
#define __OES_LIMIT_OPEN_POSITION_CRD       OES_LIMIT_BUY               \
                                            | OES_LIMIT_COLLATERAL_TRANSFER_OUT \
                                            | OES_LIMIT_MARGIN_BUY \
                                            | OES_LIMIT_SHORT_SELL \
                                            | OES_LIMIT_REPAY_STOCK_BY_BUY

/** 信用平仓相关的交易限制集合 */
#define __OES_LIMIT_CLOSE_POSITION_CRD      OES_LIMIT_SELL              \
                                            | OES_LIMIT_COLLATERAL_TRANSFER_IN \
                                            | OES_LIMIT_REPAY_MARGIN_BY_SELL

/** 信用担保品划转交易限制集合 */
#define __OES_LIMIT_COLLATERAL_TRANSFER_CRD OES_LIMIT_COLLATERAL_TRANSFER_IN \
                                            | OES_LIMIT_COLLATERAL_TRANSFER_OUT

/** 开仓相关的所有交易限制集合 */
#define __OES_LIMIT_OPEN_POSITION_ALL       __OES_LIMIT_OPEN_POSITION_STK \
                                            | __OES_LIMIT_OPEN_POSITION_OPT \
                                            | __OES_LIMIT_OPEN_POSITION_CRD

/** 平仓相关的所有交易限制集合 */
#define __OES_LIMIT_CLOSE_POSITION_ALL      __OES_LIMIT_CLOSE_POSITION_STK \
                                            | __OES_LIMIT_CLOSE_POSITION_OPT \
                                            | __OES_LIMIT_CLOSE_POSITION_CRD
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (1. 市场相关)
 * =================================================================== */

/**
 * 交易所代码定义
 */
typedef enum _eOesExchangeId {
    OES_EXCH_UNDEFINE                       = 0,        /**< 未定义的交易所代码 */
    OES_EXCH_SSE                            = 1,        /**< 上海证券交易所 */
    OES_EXCH_SZSE                           = 2,        /**< 深圳证券交易所 */
    __MAX_OES_EXCH,

    /** 上海证券交易所 @deprecated 已过时, 请使用 OES_EXCH_SSE */
    OES_EXCHANGE_TYPE_SSE                   = OES_EXCH_SSE,
    /** 深圳证券交易所 @deprecated 已过时, 请使用 OES_EXCH_SZSE */
    OES_EXCHANGE_TYPE_SZSE                  = OES_EXCH_SZSE,

    __OES_EXCH_ID_MAX_ALIGNED4              = 4,        /**< 交易所代码最大值 (按4字节对齐的大小) */
    __OES_EXCH_ID_MAX_ALIGNED8              = 8         /**< 交易所代码最大值 (按8字节对齐的大小) */
} eOesExchangeIdT;


/**
 * 市场类型定义
 */
typedef enum _eOesMarketId {
    OES_MKT_UNDEFINE                        = 0,        /**< 未定义的市场类型 */
    OES_MKT_SH_ASHARE                       = 1,        /**< 上海A股 */
    OES_MKT_SZ_ASHARE                       = 2,        /**< 深圳A股 */
    OES_MKT_SH_OPTION                       = 3,        /**< 上海期权 */
    OES_MKT_SZ_OPTION                       = 4,        /**< 深圳期权 */
    __OES_MKT_ID_MAX,                                   /**< 市场类型最大值 */
    __OES_MKT_ID_MAX_ALIGNED8               = 8,        /**< 市场类型最大值 (按8字节对齐的大小) */

    /*
     * 扩展的外部市场定义 (仅用于查询)
     */
    OES_MKT_EXT_HK                          = 11,       /**< 港股, 仅用于跨沪深港ETF的成分股查询 */
    __OES_MKT_EXT_MAX,                                  /**< 扩展市场类型的最大值 */

    /** 未定义的市场类型 @deprecated 已过时, 请使用 OES_MKT_UNDEFINE  */
    OES_MKT_ID_UNDEFINE                     = OES_MKT_UNDEFINE,
    /** 上海A股 @deprecated 已过时, 请使用 OES_MKT_SH_ASHARE */
    OES_MKT_ID_SH_A                         = OES_MKT_SH_ASHARE,
    /** 深圳A股 @deprecated 已过时, 请使用 OES_MKT_SZ_ASHARE */
    OES_MKT_ID_SZ_A                         = OES_MKT_SZ_ASHARE,
    /** 上海期权 @deprecated 已过时, 请使用 OES_MKT_SH_OPTION */
    OES_MKT_ID_SH_OPT                       = OES_MKT_SH_OPTION
} eOesMarketIdT;


/**
 * 交易平台类型定义
 */
typedef enum _eOesPlatformId {
    OES_PLATFORM_UNDEFINE                   = 0,        /**< 未定义的交易平台类型 */
    OES_PLATFORM_CASH_AUCTION               = 1,        /**< 现货集中竞价交易平台 */
    OES_PLATFORM_FINANCIAL_SERVICES         = 2,        /**< 综合金融服务平台 */
    OES_PLATFORM_NON_TRADE                  = 3,        /**< 非交易处理平台 */
    OES_PLATFORM_DERIVATIVE_AUCTION         = 4,        /**< 衍生品集中竞价交易平台 */
    OES_PLATFORM_INTERNATIONAL_MARKET       = 5,        /**< 国际市场互联平台 (暂未对接) */
    OES_PLATFORM_BOND_TRADING               = 6,        /**< 新债券交易平台 */
    __OES_PLATFORM_ID_MAX,                              /**< 平台号的最大值 */
    __OES_PLATFORM_ID_MAX_ALIGNED8          = 8         /**< 平台号的最大值 (按8字节对齐的大小) */
} eOesPlatformIdT;


/**
 * 市场状态定义
 */
typedef enum _eOesMarketState {
    OES_MKT_STATE_UNDEFINE                  = 0,        /**< 未定义的市场状态 */
    OES_MKT_STATE_PRE_OPEN                  = 1,        /**< 未开放 (PreOpen) */
    OES_MKT_STATE_OPEN_UP_COMING            = 2,        /**< 即将开放 (OpenUpComing) */
    OES_MKT_STATE_OPEN                      = 3,        /**< 开放 (Open) */
    OES_MKT_STATE_HALT                      = 4,        /**< 暂停开放 (Halt) */
    OES_MKT_STATE_CLOSE                     = 5,        /**< 关闭 (Close) */
    __OES_MKT_STATE_MAX                                 /**< 市场状态最大值 */
} eOesMarketStateT;


/**
 * OES 竞价时段定义
 */
typedef enum _eOesTrdSessType {
    OES_TRD_SESS_TYPE_O                     = 0,        /**< 开盘集合竞价时段 */
    OES_TRD_SESS_TYPE_T                     = 1,        /**< 连续竞价时段 */
    OES_TRD_SESS_TYPE_C                     = 2,        /**< 收盘集合竞价 */
    __OES_TRD_SESS_TYPE_MAX                             /**< 时段类型最大值 (时段类型数量) */
} eOesTrdSessTypeT;
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (2. 产品相关)
 * =================================================================== */

/**
 * 产品类型 (high-level category)
 */
typedef enum _eOesProductType {
    OES_PRODUCT_TYPE_UNDEFINE               = 0,        /**< 未定义的产品类型 */
    OES_PRODUCT_TYPE_EQUITY                 = 1,        /**< 普通股票/存托凭证/债券/基金/科创板 */
    OES_PRODUCT_TYPE_BOND_STD               = 2,        /**< 逆回购标准券 */
    OES_PRODUCT_TYPE_IPO                    = 3,        /**< 新股认购 */
    OES_PRODUCT_TYPE_ALLOTMENT              = 4,        /**< 配股认购 */
    OES_PRODUCT_TYPE_OPTION                 = 5,        /**< 期权 */

    __OES_PRODUCT_TYPE_MAX                              /**< 产品类型最大值 */
} eOesProductTypeT;


/**
 * 证券类别
 */
typedef enum _eOesSecurityType {
    OES_SECURITY_TYPE_UNDEFINE              = 0,        /**< 未定义的证券类型 */
    OES_SECURITY_TYPE_STOCK                 = 1,        /**< 股票 */
    OES_SECURITY_TYPE_BOND                  = 2,        /**< 债券 */
    OES_SECURITY_TYPE_ETF                   = 3,        /**< ETF */
    OES_SECURITY_TYPE_FUND                  = 4,        /**< 基金 */
    OES_SECURITY_TYPE_OPTION                = 5,        /**< 期权 */
    OES_SECURITY_TYPE_MGR                   = 9,        /**< 管理类 */
    __OES_SECURITY_TYPE_MAX,                            /**< 证券类型最大值 */

    __OES_SECURITY_TYPE_NOT_SUPPORT         = 100,      /**< 不支持的证券类别 */
    __OES_SECURITY_TYPE_VIRTUAL             = 101       /**< 虚拟证券的证券类别 */
} eOesSecurityTypeT;


/**
 * 证券子类别
 */
typedef enum _eOesSubSecurityType {
    OES_SUB_SECURITY_TYPE_UNDEFINE          = 0,        /**< 未定义的证券子类型 */

    __OES_SUB_SECURITY_TYPE_STOCK_MIN       = 10,       /**< 股票类证券子类型最小值 */
    OES_SUB_SECURITY_TYPE_STOCK_ASH         = 11,       /**< A股股票, A Share */
    OES_SUB_SECURITY_TYPE_STOCK_SME         = 12,       /**< 中小板股票, Small & Medium Enterprise (SME) Board */
    OES_SUB_SECURITY_TYPE_STOCK_GEM         = 13,       /**< 创业板股票, Growth Enterprise Market (GEM) */
    OES_SUB_SECURITY_TYPE_STOCK_KSH         = 14,       /**< 科创板股票 */
    OES_SUB_SECURITY_TYPE_STOCK_KCDR        = 15,       /**< 科创板存托凭证 */
    OES_SUB_SECURITY_TYPE_STOCK_CDR         = 16,       /**< 存托凭证, Chinese Depository Receipt (CDR) */
    OES_SUB_SECURITY_TYPE_STOCK_HLTCDR      = 17,       /**< 沪伦通CDR本地交易业务产品 */
    OES_SUB_SECURITY_TYPE_STOCK_GEMCDR      = 18,       /**< 创业板存托凭证 */
    __OES_SUB_SECURITY_TYPE_STOCK_MAX,                  /**< 股票类证券子类型最大值 */

    __OES_SUB_SECURITY_TYPE_BOND_MIN        = 20,       /**< 债券类证券子类型最小值 */
    OES_SUB_SECURITY_TYPE_BOND_GBF          = 21,       /**< 国债 (国债/地方债/政策性金融债/上交所政府支持债) */
    OES_SUB_SECURITY_TYPE_BOND_CBF          = 22,       /**< 企业债 */
    OES_SUB_SECURITY_TYPE_BOND_CPF          = 23,       /**< 公司债 */
    OES_SUB_SECURITY_TYPE_BOND_CCF          = 24,       /**< 可转换债券 */
    OES_SUB_SECURITY_TYPE_BOND_FBF          = 25,       /**< 金融机构发行债券 (仅适用于深交所, 上交所已废弃) */
    OES_SUB_SECURITY_TYPE_BOND_PRP          = 26,       /**< 通用质押式回购 */
    OES_SUB_SECURITY_TYPE_BOND_STD          = 27,       /**< 债券标准券 */
    OES_SUB_SECURITY_TYPE_BOND_EXG          = 28,       /**< 可交换债券 */
    __OES_SUB_SECURITY_TYPE_BOND_MAX,                   /**< 债券类证券子类型最大值 */

    __OES_SUB_SECURITY_TYPE_ETF_MIN         = 30,       /**< ETF类证券子类型最小值 */
    OES_SUB_SECURITY_TYPE_ETF_SINGLE_MKT    = 31,       /**< 单市场股票ETF */
    OES_SUB_SECURITY_TYPE_ETF_CROSS_MKT     = 32,       /**< 跨市场股票ETF */
    OES_SUB_SECURITY_TYPE_ETF_BOND          = 33,       /**< 实物债券ETF */
    OES_SUB_SECURITY_TYPE_ETF_CURRENCY      = 34,       /**< 货币ETF */
    OES_SUB_SECURITY_TYPE_ETF_CROSS_BORDER  = 35,       /**< 跨境ETF */
    OES_SUB_SECURITY_TYPE_ETF_GOLD          = 36,       /**< 黄金ETF */
    OES_SUB_SECURITY_TYPE_ETF_COMMODITY_FUTURES
                                            = 37,       /**< 商品期货ETF */
    __OES_SUB_SECURITY_TYPE_ETF_MAX,                    /**< ETF类证券子类型最大值 */

    __OES_SUB_SECURITY_TYPE_FUND_MIN        = 40,       /**< 基金类证券子类型最小值 */
    OES_SUB_SECURITY_TYPE_FUND_LOF          = 41,       /**< LOF基金 */
    OES_SUB_SECURITY_TYPE_FUND_CEF          = 42,       /**< 封闭式基金, Close-end Fund */
    OES_SUB_SECURITY_TYPE_FUND_OEF          = 43,       /**< 开放式基金, Open-end Fund */
    OES_SUB_SECURITY_TYPE_FUND_GRADED       = 44,       /**< 分级子基金 */
    OES_SUB_SECURITY_TYPE_FUND_REITS        = 45,       /**< 基础设施基金 */
    __OES_SUB_SECURITY_TYPE_FUND_MAX,                   /**< 基金类证券子类型最大值 */

    __OES_SUB_SECURITY_TYPE_OPTION_MIN      = 50,       /**< 期权类证券子类型最小值 */
    OES_SUB_SECURITY_TYPE_OPTION_ETF        = 51,       /**< ETF期权 */
    OES_SUB_SECURITY_TYPE_OPTION_STOCK      = 52,       /**< 个股期权 */
    __OES_SUB_SECURITY_TYPE_OPTION_MAX,                 /**< 期权类证券子类型最大值 */

    __OES_SUB_SECURITY_TYPE_MGR_MIN         = 90,       /**< 管理类证券子类型最小值 */
    OES_SUB_SECURITY_TYPE_MGR_SSE_DESIGNATION
                                            = 91,       /**< 指定登记 */
    OES_SUB_SECURITY_TYPE_MGR_SSE_RECALL_DESIGNATION
                                            = 92,       /**< 指定撤消 */
    OES_SUB_SECURITY_TYPE_MGR_SZSE_DESIGNATION
                                            = 93,       /**< 托管注册 */
    OES_SUB_SECURITY_TYPE_MGR_SZSE_CANCEL_DESIGNATION
                                            = 94,       /**< 托管撤消 */
    OES_SUB_SECURITY_TYPE_MGR_OPT_EXERCISE_TRANSFER
                                            = 95,       /**< 期权转处置 */
    OES_SUB_SECURITY_TYPE_MGR_CRD_COLLATERAL_TRANSFER
                                            = 96,       /**< 信用担保证券划转 */
    __OES_SUB_SECURITY_TYPE_MGR_MAX ,                   /**< 管理类证券子类型最大值 */

    __OES_SUB_SECURITY_TYPE_MAX             = __OES_SUB_SECURITY_TYPE_MGR_MAX
} eOesSubSecurityTypeT;


/**
 * 证券级别
 */
typedef enum _eOesSecurityLevel {
    OES_SECURITY_LEVEL_UNDEFINE             = 0,
    OES_SECURITY_LEVEL_N                    = 1,        /**< 正常证券 */
    OES_SECURITY_LEVEL_XST                  = 2,        /**< *ST股 */
    OES_SECURITY_LEVEL_ST                   = 3,        /**< ST股 */
    OES_SECURITY_LEVEL_P                    = 4,        /**< 退市整理证券 */
    OES_SECURITY_LEVEL_T                    = 5,        /**< 退市转让证券 */
    OES_SECURITY_LEVEL_U                    = 6,        /**< 优先股 */
    OES_SECURITY_LEVEL_B                    = 7,        /**< B级基金 */
    __OES_SECURITY_LEVEL_MAX
} eOesSecurityLevelT;


/**
 * 证券风险等级
 */
typedef enum _eOesSecurityRiskLevel {
    OES_RISK_LEVEL_VERY_LOW                 = 0,        /**< 最低风险 */
    OES_RISK_LEVEL_LOW                      = 1,        /**< 低风险 */
    OES_RISK_LEVEL_MEDIUM_LOW               = 2,        /**< 中低风险 */
    OES_RISK_LEVEL_MEDIUM                   = 3,        /**< 中风险 */
    OES_RISK_LEVEL_MEDIUM_HIGH              = 4,        /**< 中高风险 */
    OES_RISK_LEVEL_HIGH                     = 5,        /**< 高风险 */
    OES_RISK_LEVEL_VERY_HIGH                = 6,        /**< 极高风险 */
    __OES_RISK_LEVEL_MAX
} eOesSecurityRiskLevelT;


/**
 * 证券禁止交易标识
 */
typedef enum _eOesSecuritySuspFlag {
    OES_SUSPFLAG_NONE                       = 0x0,      /**< 无禁止交易标识 */
    OES_SUSPFLAG_EXCHANGE                   = 0x1,      /**< 因证券连续停牌而禁止交易 */
    OES_SUSPFLAG_BROKER                     = 0x2,      /**< 因券商设置而禁止交易 */
    OES_SUSPFLAG_MARKET_CLOSE               = 0x4,      /**< 因闭市而禁止交易 */
    __OES_SUSPFLAG_OTHER
} eOesSecuritySuspFlagT;


/**
 * 证券状态的枚举值定义
 */
typedef enum _eOesSecurityStatus {
    OES_SECURITY_STATUS_NONE                = 0,        /**< 无特殊状态 */
    OES_SECURITY_STATUS_FIRST_LISTING       = (1 << 0), /**< 上市首日 */
    OES_SECURITY_STATUS_RESUME_FIRST_LISTING
                                            = (1 << 1), /**< 恢复上市首日 */
    OES_SECURITY_STATUS_NEW_LISTING         = (1 << 2), /**< 上市初期 */
    OES_SECURITY_STATUS_EXCLUDE_RIGHT       = (1 << 3), /**< 除权 */
    OES_SECURITY_STATUS_EXCLUDE_DIVIDEN     = (1 << 4), /**< 除息 */
    OES_SECURITY_STATUS_SUSPEND             = (1 << 5), /**< 证券连续停牌 */
    OES_SECURITY_STATUS_SPECIAL_TREATMENT   = (1 << 6), /**< ST股 */
    OES_SECURITY_STATUS_X_SPECIAL_TREATMENT = (1 << 7), /**< *ST股 */
    OES_SECURITY_STATUS_DELIST_PERIOD       = (1 << 8), /**< 退市整理期 */
    OES_SECURITY_STATUS_DELIST_TRANSFER     = (1 << 9)  /**< 退市转让期 */
} eOesSecurityStatusT;


/**
 * 证券属性的枚举值定义
 */
typedef enum _eOesSecurityAttribute {
    OES_SECURITY_ATTR_NONE                  = 0,        /**< 无特殊属性 */
    OES_SECURITY_ATTR_INNOVATION            = (1 << 0), /**< 创新企业 */
    OES_SECURITY_ATTR_KSH                   = (1 << 1), /**< 科创板标记 */

    /** 科创板ETF/科创板LOF @deprecated 已过时, 请使用 OES_SECURITY_ATTR_KSH */
    OES_SECURITY_ATTR_KSH_FUND              = OES_SECURITY_ATTR_KSH
} eOesSecurityAttributeT;


/**
 * 有效竞价范围限制类型
 */
typedef enum _eOesAuctionLimitType {
    OES_AUCTION_LIMIT_TYPE_NONE             = 0,        /**< 无竞价范围限制 */
    OES_AUCTION_LIMIT_TYPE_RATE             = 1,        /**< 按幅度限制 (百分比) */
    OES_AUCTION_LIMIT_TYPE_ABSOLUTE         = 2         /**< 按价格限制 (绝对值) */
} eOesAuctionLimitTypeT;


/**
 * 有效竞价范围基准价类型
 */
typedef enum _eOesAuctionReferPriceType {
    OES_AUCTION_REFER_PRICE_TYPE_LAST       = 1,        /**< 最近价 */
    OES_AUCTION_REFER_PRICE_TYPE_BEST       = 2         /**< 对手方最优价 */
} eOesAuctionReferPriceTypeT;


/**
 * OES中签、配号记录类型
 */
typedef enum _eOesLotType {
    OES_LOT_TYPE_UNDEFINE                   = 0,        /**< 未定义的中签、配号记录类型 */
    OES_LOT_TYPE_FAILED                     = 1,        /**< 配号失败记录 */
    OES_LOT_TYPE_ASSIGNMENT                 = 2,        /**< 配号成功记录 */
    OES_LOT_TYPE_LOTTERY                    = 3,        /**< 中签记录 */
    __OES_LOT_TYPE_MAX                                  /**< 中签、配号记录类型最大值 */
} eOesLotTypeT;


/**
 * OES配号失败原因
 */
typedef enum _eOesLotRejReason {
    OES_LOT_REJ_REASON_DUPLICATE            = 1,        /**< 配号失败-重复申购 */
    OES_LOT_REJ_REASON_INVALID_DUPLICATE    = 2,        /**< 配号失败-违规重复 */
    OES_LOT_REJ_REASON_OFFLINE_FIRST        = 3,        /**< 配号失败-网下在先 */
    OES_LOT_REJ_REASON_BAD_RECORD           = 4,        /**< 配号失败-不良记录 */
    OES_LOT_REJ_REASON_UNKNOW               = 5         /**< 配号失败-未知原因 */
} eOesLotRejReasonT;


/**
 * 产品发行方式
 */
typedef enum _eOesSecurityIssueType {
    OES_ISSUE_TYPE_UNDEFINE                 = 0,        /**< 未定义的发行方式 */
    OES_ISSUE_TYPE_MKT_QUOTA                = 1,        /**< 按市值限额申购 (检查认购限额, 不预冻结资金) */
    OES_ISSUE_TYPE_CASH                     = 2,        /**< 增发资金申购 (不检查认购限额, 预冻结资金) */
    OES_ISSUE_TYPE_CREDIT                   = 3         /**< 信用申购 (不检查认购限额, 不预冻结资金) */
} eOesSecurityIssueTypeT;


/**
 * 计价方式
 */
typedef enum _eOesPricingMethod {
    OES_PRICING_METHOD_UNDEFINE             = 0,        /**< 未定义的计价方式 */
    OES_PRICING_METHOD_CLEAN                = 1,        /**< 净价 */
    OES_PRICING_METHOD_DIRTY                = 2         /**< 全价 */
} eOesPricingMethodT;
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (3. 订单相关)
 * =================================================================== */

/**
 * 订单执行状态定义
 */
typedef enum _eOesOrdStatus {
    OES_ORD_STATUS_PENDING                  = 0,        /**< 待处理 (仅内部使用) */
    OES_ORD_STATUS_NEW                      = 1,        /**< 新订单 (风控通过) */

    OES_ORD_STATUS_DECLARED                 = 2,        /**< 已确认 */
    OES_ORD_STATUS_PARTIALLY_FILLED         = 3,        /**< 部分成交 */

    __OES_ORD_STATUS_FINAL_MIN              = 4,        /**< 订单终结状态判断标志 */
    OES_ORD_STATUS_CANCEL_DONE              = 5,        /**< 撤单指令已执行 (适用于撤单请求, 并做为撤单请求的终结状态) */
    OES_ORD_STATUS_PARTIALLY_CANCELED       = 6,        /**< 部分撤单 (部分成交, 剩余撤单) */
    OES_ORD_STATUS_CANCELED                 = 7,        /**< 已撤单 */
    OES_ORD_STATUS_FILLED                   = 8,        /**< 已成交 (全部成交) */
    __OES_ORD_STATUS_VALID_MAX,

    __OES_ORD_STATUS_INVALID_MIN            = 10,       /**< 废单判断标志 (委托状态大于该值的全部为废单) */
    OES_ORD_STATUS_INVALID_OES              = 11,       /**< OES内部废单 */
    OES_ORD_STATUS_INVALID_EXCHANGE         = 12,       /**< 交易所后台废单 */
    OES_ORD_STATUS_INVALID_TGW_REJECT       = 13,       /**< 交易所前台废单 (因订单不合法而被交易网关拒绝) */
    OES_ORD_STATUS_INVALID_TGW_COMM         = 14,       /**< 交易所通信故障 (仅适用于上交所) */
    OES_ORD_STATUS_INVALID_TGW_TRY_AGAIN    = 18,       /**< 因平台尚未开放(非交易时段)而被交易网关拒绝
                                                             (@note 前端需要关注该状态, 可以根据需要尝试重新发送委托请求) */
    __OES_ORD_STATUS_INVALID_MAX,

    /*
     * 以下订单状态定义已废弃, 只是为了兼容之前的版本而暂时保留
     */
    OES_ORD_STATUS_NORMAL                   = OES_ORD_STATUS_NEW,
    OES_ORD_STATUS_DECLARING                = OES_ORD_STATUS_NEW,
    __OES_ORD_STATUS_INVALID_OES            = OES_ORD_STATUS_INVALID_OES,

    /* 上证后台判断该订单为废单 @deprecated 已废弃 */
    OES_ORD_STATUS_INVALID_SH_F             = OES_ORD_STATUS_INVALID_EXCHANGE,
    /* 上证前台判断该订单为废单 @deprecated 已废弃 */
    OES_ORD_STATUS_INVALID_SH_E             = OES_ORD_STATUS_INVALID_TGW_REJECT,
    /* 通信故障(仅适用于上交所) @deprecated 已废弃 */
    OES_ORD_STATUS_INVALID_SH_COMM          = OES_ORD_STATUS_INVALID_TGW_COMM,
    /* 深证前台废单 @deprecated 已废弃 (深交所不会返回该错误, 之前版本的取值为: 15) */
    OES_ORD_STATUS_INVALID_SZ_F             = OES_ORD_STATUS_INVALID_TGW_REJECT,
    /* 深证后台废单 @deprecated 已废弃 (@note 取值发生变化, 之前版本的取值为: 16) */
    OES_ORD_STATUS_INVALID_SZ_E             = OES_ORD_STATUS_INVALID_EXCHANGE,
    /* 深证业务拒绝 @deprecated 已废弃 (@note 取值发生变化, 之前版本的取值为: 17) */
    OES_ORD_STATUS_INVALID_SZ_REJECT        = OES_ORD_STATUS_INVALID_TGW_REJECT,
    /* 深证平台未开放(需尝试重报) @deprecated 已废弃 */
    OES_ORD_STATUS_INVALID_SZ_TRY_AGAIN     = OES_ORD_STATUS_INVALID_TGW_TRY_AGAIN

} eOesOrdStatusT;


/**
 * 委托类型
 *
 * 部分缩写解释如下:
 *  - LMT (Limit)           : 限价
 *  - MTL (Market To Limit) : 剩余转限价(市价)
 *  - FAK (Fill and Kill)   : 剩余转撤销(市价)
 *  - FOK (Fill or Kill)    : 全部成交或全部撤销(市价/限价)
 *
 * 上海A股支持类型:
 *      1. OES_ORD_TYPE_LMT
 *      2. OES_ORD_TYPE_MTL_BEST_5
 *      3. OES_ORD_TYPE_FAK_BEST_5
 *      4. OES_ORD_TYPE_MTL_BEST (仅适用于科创板)
 *      5. OES_ORD_TYPE_MTL_SAMEPARTY_BEST (仅适用于科创板)
 *
 * 上海期权支持市价类型:
 *      1. OES_ORD_TYPE_LMT
 *      2. OES_ORD_TYPE_LMT_FOK
 *      3. OES_ORD_TYPE_MTL
 *      4. OES_ORD_TYPE_FAK
 *      5. OES_ORD_TYPE_FOK
 *
 * 深圳A股支持市价类型:
 *      1. OES_ORD_TYPE_LMT
 *      2. OES_ORD_TYPE_MTL_BEST
 *      3. OES_ORD_TYPE_MTL_SAMEPARTY_BEST
 *      4. OES_ORD_TYPE_FAK_BEST_5
 *      5. OES_ORD_TYPE_FAK
 *      6. OES_ORD_TYPE_FOK
 *
 * 深圳期权支持市价类型:
 *      1. OES_ORD_TYPE_LMT
 *      2. OES_ORD_TYPE_LMT_FOK
 *      3. OES_ORD_TYPE_MTL_BEST
 *      4. OES_ORD_TYPE_MTL_SAMEPARTY_BEST
 *      5. OES_ORD_TYPE_FAK_BEST_5
 *      6. OES_ORD_TYPE_FAK
 *      7. OES_ORD_TYPE_FOK
 */
typedef enum _eOesOrdType {
    OES_ORD_TYPE_LMT                        = 0,        /**< 限价委托 */
    OES_ORD_TYPE_LMT_FOK                    = 1,        /**< 限价全部成交或全部撤销委托 */
    __OES_ORD_TYPE_LMT_MAX,

    OES_ORD_TYPE_MTL_BEST_5                 = 10,       /**< 最优五档即时成交剩余转限价委托 */
    OES_ORD_TYPE_MTL_BEST                   = 11,       /**< 对手方最优价格委托 */
    OES_ORD_TYPE_MTL_SAMEPARTY_BEST         = 12,       /**< 本方最优价格委托 */
    OES_ORD_TYPE_MTL                        = 13,       /**< 市价剩余转限价委托 */
    __OES_ORD_TYPE_MTL_MAX,

    OES_ORD_TYPE_FAK_BEST_5                 = 20,       /**< 最优五档即时成交剩余撤销委托 */
    OES_ORD_TYPE_FAK                        = 21,       /**< 即时成交剩余撤销委托 */
    __OES_ORD_TYPE_FAK_MAX,

    OES_ORD_TYPE_FOK                        = 30,       /**< 市价全部成交或全部撤销委托 */
    __OES_ORD_TYPE_FOK_MAX,
    __OES_ORD_TYPE_MAX,
    __OES_ORD_TYPE_MAX_ALIGNED              = 32        /**< 委托类型最大值 (按8字节对齐的大小) */
} eOesOrdTypeT;


/**
 * 上证委托类型
 *
 * 部分缩写解释如下:
 *  - LMT (Limit)           : 限价
 *  - MTL (Market To Limit) : 剩余转限价(市价)
 *  - FAK (Fill and Kill)   : 剩余转撤销(市价)
 *  - FOK (Fill or Kill)    : 全部成交或全部撤销(市价/限价)
 */
typedef enum _eOesOrdTypeSh {
    /** 限价, 0 */
    OES_ORD_TYPE_SH_LMT                     = OES_ORD_TYPE_LMT,

    /** 最优五档即时成交剩余转限价委托, 10 */
    OES_ORD_TYPE_SH_MTL_BEST_5              = OES_ORD_TYPE_MTL_BEST_5,
    /** 对手方最优价格委托(仅适用于科创板), 11 */
    OES_ORD_TYPE_SH_MTL_BEST                = OES_ORD_TYPE_MTL_BEST,
    /** 本方最优价格委托(仅适用于科创板), 12 */
    OES_ORD_TYPE_SH_MTL_SAMEPARTY_BEST      = OES_ORD_TYPE_MTL_SAMEPARTY_BEST,
    /** 最优五档即时成交剩余撤销委托, 20 */
    OES_ORD_TYPE_SH_FAK_BEST_5              = OES_ORD_TYPE_FAK_BEST_5
} eOesOrdTypeShT;


/**
 * 上证期权业务委托类型
 *
 * 部分缩写解释如下:
 *  - LMT (Limit)           : 限价
 *  - MTL (Market To Limit) : 剩余转限价(市价)
 *  - FAK (Fill and Kill)   : 剩余转撤销(市价)
 *  - FOK (Fill or Kill)    : 全部成交或全部撤销(市价/限价)
 */
typedef enum _eOesOrdTypeShOpt {
    /** 限价, 0 */
    OES_ORD_TYPE_SHOPT_LMT                  = OES_ORD_TYPE_LMT,
    /** 限价全部成交或全部撤销委托, 1 */
    OES_ORD_TYPE_SHOPT_LMT_FOK              = OES_ORD_TYPE_LMT_FOK,

    /** 市价剩余转限价委托, 13 */
    OES_ORD_TYPE_SHOPT_MTL                  = OES_ORD_TYPE_MTL,
    /** 即时成交剩余撤销委托, 21 */
    OES_ORD_TYPE_SHOPT_FAK                  = OES_ORD_TYPE_FAK,
    /** 市价全部成交或全部撤销委托, 30 */
    OES_ORD_TYPE_SHOPT_FOK                  = OES_ORD_TYPE_FOK,

    /*
     * 以下委托类型已废弃, 只是为了兼容之前的版本而暂时保留
     */
    OES_ORD_TYPE_SH_LMT_FOK                 = OES_ORD_TYPE_SHOPT_LMT_FOK,
    OES_ORD_TYPE_SH_FOK                     = OES_ORD_TYPE_SHOPT_FOK
} eOesOrdTypeShOptT;


/**
 * 深证委托类型
 *
 * 部分缩写解释如下:
 *  - LMT (Limit)           : 限价
 *  - MTL (Market To Limit) : 剩余转限价(市价)
 *  - FAK (Fill and Kill)   : 剩余转撤销(市价)
 *  - FOK (Fill or Kill)    : 全部成交或全部撤销(市价/限价)
 */
typedef enum _eOesOrdTypeSz {
    /** 限价, 0 */
    OES_ORD_TYPE_SZ_LMT                     = OES_ORD_TYPE_LMT,
    /** 限价全部成交或全部撤销委托(仅适用于期权), 1 */
    OES_ORD_TYPE_SZ_LMT_FOK                 = OES_ORD_TYPE_LMT_FOK,

    /** 对手方最优价格委托, 11 */
    OES_ORD_TYPE_SZ_MTL_BEST                = OES_ORD_TYPE_MTL_BEST,
    /** 本方最优价格委托, 12 */
    OES_ORD_TYPE_SZ_MTL_SAMEPARTY_BEST      = OES_ORD_TYPE_MTL_SAMEPARTY_BEST,
    /** 最优五档即时成交剩余撤销委托, 20 */
    OES_ORD_TYPE_SZ_FAK_BEST_5              = OES_ORD_TYPE_FAK_BEST_5,
    /** 即时成交剩余撤销委托, 21 */
    OES_ORD_TYPE_SZ_FAK                     = OES_ORD_TYPE_FAK,
    /** 市价全部成交或全部撤销委托, 30 */
    OES_ORD_TYPE_SZ_FOK                     = OES_ORD_TYPE_FOK
} eOesOrdTypeSzT;


/**
 * 买卖类型
 */
typedef enum _eOesBuySellType {
    OES_BS_TYPE_UNDEFINE                    = 0,        /**< 未定义的买卖类型 */

    OES_BS_TYPE_BUY                         = 1,        /**< 普通买入/信用担保品买入 */
    OES_BS_TYPE_SELL                        = 2,        /**< 普通卖出/信用担保品卖出 */
    OES_BS_TYPE_CREATION                    = 3,        /**< 申购 */
    OES_BS_TYPE_REDEMPTION                  = 4,        /**< 赎回 */
    OES_BS_TYPE_REVERSE_REPO                = 6,        /**< 质押式逆回购 */
    OES_BS_TYPE_SUBSCRIPTION                = 7,        /**< 新股/可转债/可交换债认购 */
    OES_BS_TYPE_ALLOTMENT                   = 8,        /**< 配股/配债认购 */
    __OES_BS_TYPE_MAX_SPOT,                             /**< 现货交易的买卖类型最大值 */
    /* -------------------------           */

    __OES_BS_TYPE_MIN_OPTION                = 10,       /**< 期权交易的买卖类型最小值 */
    OES_BS_TYPE_BUY_OPEN                    = 11,       /**< 期权买入开仓 */
    OES_BS_TYPE_SELL_CLOSE                  = 12,       /**< 期权卖出平仓 */
    OES_BS_TYPE_SELL_OPEN                   = 13,       /**< 期权卖出开仓 */
    OES_BS_TYPE_BUY_CLOSE                   = 14,       /**< 期权买入平仓 */
    OES_BS_TYPE_COVERED_OPEN                = 15,       /**< 期权备兑开仓 */
    OES_BS_TYPE_COVERED_CLOSE               = 16,       /**< 期权备兑平仓 */
    OES_BS_TYPE_OPTION_EXERCISE             = 17,       /**< 期权行权 */
    OES_BS_TYPE_UNDERLYING_FREEZE           = 18,       /**< 期权标的锁定 */
    OES_BS_TYPE_UNDERLYING_UNFREEZE         = 19,       /**< 期权标的解锁 */
    __OES_BS_TYPE_MAX_OPTION,                           /**< 期权交易的买卖类型最大值 */
    /* -------------------------           */

    OES_BS_TYPE_CANCEL                      = 30,       /**< 撤单 */
    /* -------------------------           */

    __OES_BS_TYPE_MIN_CREDIT                = 30,       /**< 信用交易特有的买卖类型最小值 */
    /** 信用担保品买入 */
    OES_BS_TYPE_COLLATERAL_BUY              = OES_BS_TYPE_BUY,
    /** 信用担保品卖出 */
    OES_BS_TYPE_COLLATERAL_SELL             = OES_BS_TYPE_SELL,
    OES_BS_TYPE_COLLATERAL_TRANSFER_IN      = 31,       /**< 信用担保品转入 */
    OES_BS_TYPE_COLLATERAL_TRANSFER_OUT     = 32,       /**< 信用担保品转出 */

    OES_BS_TYPE_MARGIN_BUY                  = 33,       /**< 信用融资买入 */
    OES_BS_TYPE_REPAY_MARGIN_BY_SELL        = 34,       /**< 信用卖券还款 */

    OES_BS_TYPE_SHORT_SELL                  = 35,       /**< 信用融券卖出 */
    OES_BS_TYPE_REPAY_STOCK_BY_BUY          = 36,       /**< 信用买券还券 */
    OES_BS_TYPE_REPAY_STOCK_DIRECT          = 37,       /**< 信用直接还券 */

    __OES_BS_TYPE_MAX_CREDIT,                           /**< 信用交易特有的买卖类型最大值 */
    __OES_BS_TYPE_MAX_TRADING,                          /**< 对外开放的交易类业务的买卖类型最大值 */
    __OES_BS_TYPE_MAX_TRADING_ALIGNED8      = 40,       /**< 对外开放的交易类业务的买卖类型最大值 (按8字节对齐的大小) */
    /* -------------------------           */

    /* 仅用于兼容之前版本的质押式逆回购, 不可用于‘信用融券卖出’交易 */
    OES_BS_TYPE_CREDIT_SELL                 = OES_BS_TYPE_REVERSE_REPO,
    OES_BS_TYPE_B                           = OES_BS_TYPE_BUY,
    OES_BS_TYPE_S                           = OES_BS_TYPE_SELL,
    OES_BS_TYPE_KB                          = OES_BS_TYPE_CREATION,
    OES_BS_TYPE_KS                          = OES_BS_TYPE_REDEMPTION,
    OES_BS_TYPE_CS                          = OES_BS_TYPE_REVERSE_REPO,

    OES_BS_TYPE_BO                          = OES_BS_TYPE_BUY_OPEN,
    OES_BS_TYPE_BC                          = OES_BS_TYPE_BUY_CLOSE,
    OES_BS_TYPE_SO                          = OES_BS_TYPE_SELL_OPEN,
    OES_BS_TYPE_SC                          = OES_BS_TYPE_SELL_CLOSE,
    OES_BS_TYPE_CO                          = OES_BS_TYPE_COVERED_OPEN,
    OES_BS_TYPE_CC                          = OES_BS_TYPE_COVERED_CLOSE,
    OES_BS_TYPE_TE                          = OES_BS_TYPE_OPTION_EXERCISE,
    OES_BS_TYPE_UF                          = OES_BS_TYPE_UNDERLYING_FREEZE,
    OES_BS_TYPE_UU                          = OES_BS_TYPE_UNDERLYING_UNFREEZE

} eOesBuySellTypeT;


/**
 * 订单的买卖方向 (内部使用)
 */
typedef enum _eOesOrdDir {
    OES_ORD_DIR_BUY                         = 0,        /**< 买 */
    OES_ORD_DIR_SELL                        = 1,        /**< 卖 */
    __OES_ORD_DIR_MAX                                   /**< 买卖方向最大值 */
} eOesOrdDirT;


/**
 * 委托强制标志
 */
typedef enum _eOesOrdMandatoryFlag {
    OES_ORD_MANDATORY_FLAG_NONE             = 0,        /**< 无强制标志 */
    OES_ORD_MANDATORY_FLAG_DELEGATE         = 1,        /**< 代客下单标志 */

    __OES_ORD_MANDATORY_FLAG_MEMBER_MIN     = 10,       /**< 会员管理委托的最小值 */
    OES_ORD_MANDATORY_FLAG_LIQUDATION       = 11,       /**< 强制平仓标志 */
    OES_ORD_MANDATORY_FLAG_MANAGEMENT       = 12,       /**< 管理指令标志 */
    __OES_ORD_MANDATORY_FLAG_MAX                        /**< 委托强制标志的最大值 */
} eOesOrdMandatoryFlagT;


/**
 * 成交回报记录的成交类型
 *
 * 上证接口规范 (IS103_ETFInterface_CV14_20130123) 中规定如下:
 * - 二级市场记录表示一笔申购/赎回交易连续记录的开始,对一笔申购/赎回交易而言,有且只有一条;
 * - 一级市场记录不再表示对应申购/赎回交易连续记录的结束,对一笔申购/赎回交易而言,有且只有一条。
 *
 * 上证期权接口规格说明书 (IS113_DTPInterface_CV1.1_20161017) 中描述如下:
 * - 执行报告中的会员内部编号(clOrdId)以QP1开头，表示为交易所保证金强制平仓
 * - 执行报告中的会员内部编号(clOrdId)以CV1开头，表示为交易所备兑强制平仓
 */
typedef enum _eOesTrdCnfmType {
    OES_TRDCNFM_TYPE_NORMAL                 = 0,        /**< 普通成交记录 */

    OES_TRDCNFM_TYPE_ETF_FIRST              = 1,        /**< ETF 二级市场记录 */
    OES_TRDCNFM_TYPE_ETF_CMPOENT            = 2,        /**< ETF 成份股记录 */
    OES_TRDCNFM_TYPE_ETF_CASH               = 3,        /**< ETF 资金记录 */
    OES_TRDCNFM_TYPE_ETF_LAST               = 4,        /**< ETF 一级市场记录 */
    __OES_TRDCNFM_TYPE_ETF_MAX,

    OES_TRDCNFM_TYPE_OPT_QP1                = 11,       /**< OPT 交易所保证金强制平仓 */
    OES_TRDCNFM_TYPE_OPT_CV1                = 12,       /**< OPT 交易所备兑强制平仓 */
    __OES_TRDCNFM_TYPE_OPT_MAX,

    __OES_TRDCNFM_TYPE_MAX                  = __OES_TRDCNFM_TYPE_OPT_MAX,

    /*
     * 以下成交类型定义即将被废弃
     */
    OES_ETF_TRDCNFM_TYPE_NONE               = OES_TRDCNFM_TYPE_NORMAL,
    OES_ETF_TRDCNFM_TYPE_ETF_FIRST          = OES_TRDCNFM_TYPE_ETF_FIRST,
    OES_ETF_TRDCNFM_TYPE_CMPOENT            = OES_TRDCNFM_TYPE_ETF_CMPOENT,
    OES_ETF_TRDCNFM_TYPE_CASH               = OES_TRDCNFM_TYPE_ETF_CASH,
    OES_ETF_TRDCNFM_TYPE_ETF_LAST           = OES_TRDCNFM_TYPE_ETF_LAST,
    __OES_ETF_TRDCNFM_TYPE_MAX              = __OES_TRDCNFM_TYPE_ETF_MAX
} eOesTrdCnfmTypeT;


/**
 * ETF成份证券现金替代标志
 */
typedef enum _eOesEtfSubFlag {
    OES_ETF_SUBFLAG_FORBID_SUB              = 0,        /**< 禁止现金替代 (必须有证券) */
    OES_ETF_SUBFLAG_ALLOW_SUB               = 1,        /**< 可以进行现金替代(先用证券, 如证券不足可用现金替代) */
    OES_ETF_SUBFLAG_MUST_SUB                = 2,        /**< 必须用现金替代 */
    OES_ETF_SUBFLAG_SZ_REFUND_SUB           = 3,        /**< 该证券为深市证券, 退补现金替代 */
    OES_ETF_SUBFLAG_SZ_MUST_SUB             = 4,        /**< 该证券为深市证券, 必须现金替代 */
    OES_ETF_SUBFLAG_OTHER_REFUND_SUB        = 5,        /**< 非沪深市场成份证券退补现金替代 */
    OES_ETF_SUBFLAG_OTHER_MUST_SUB          = 6,        /**< 非沪深市场成份证券必须现金替代 */
    OES_ETF_SUBFLAG_HK_REFUND_SUB           = 7,        /**< 港市退补现金替代 (仅适用于跨沪深港ETF产品) */
    OES_ETF_SUBFLAG_HK_MUST_SUB             = 8         /**< 港市必须现金替代 (仅适用于跨沪深港ETF产品) */
} eOesEtfSubFlagT;


/**
 * OES执行类型
 */
typedef enum _eOesExecType {
    OES_EXECTYPE_UNDEFINE                   = 0,        /**< 未定义的执行类型 */
    OES_EXECTYPE_INSERT                     = 1,        /**< 已接收 (OES已接收) */
    OES_EXECTYPE_CONFIRMED                  = 2,        /**< 已确认 (交易所已确认/出入金主柜台已确认) */
    OES_EXECTYPE_CANCELLED                  = 3,        /**< 已撤单 (原始委托的撤单完成回报) */
    OES_EXECTYPE_AUTO_CANCELLED             = 4,        /**< 自动撤单 (市价委托发生自动撤单后的委托回报) */
    OES_EXECTYPE_REJECT                     = 5,        /**< 拒绝 (OES拒绝/交易所废单/出入金主柜台拒绝) */
    OES_EXECTYPE_TRADE                      = 6,        /**< 成交 (成交回报) */
    OES_EXECTYPE_REPAY                      = 7,        /**< 归还 (融资融券业务的合约归还回报) */
    __OES_EXECTYPE_MAX                                  /**< 执行类型最大值 */
} eOesExecTypeT;
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (4. 资金/费用/出入金相关)
 * =================================================================== */

/**
 * 货币类型
 */
typedef enum _eOesCurrType {
    OES_CURR_TYPE_RMB                       = 0,        /**< 人民币 */
    OES_CURR_TYPE_HKD                       = 1,        /**< 港币 */
    OES_CURR_TYPE_USD                       = 2,        /**< 美元 */
    __OES_CURR_TYPE_MAX                                 /**< 货币种类最大值 */
} eOesCurrTypeT;


/**
 * 费用类型标识符
 */
typedef enum _eOesFeeType {
    __OES_FEE_TYPE_UNDEFINE                 = 0,        /**< 未定义的费用类型 */

    OES_FEE_TYPE_EXCHANGE_STAMP             = 0x1,      /**< 交易所固定费用-印花税 */
    OES_FEE_TYPE_EXCHANGE_TRANSFER          = 0x2,      /**< 交易所固定费用-过户费 */
    OES_FEE_TYPE_EXCHANGE_SETTLEMENT        = 0x3,      /**< 交易所固定费用-结算费 */
    OES_FEE_TYPE_EXCHANGE_TRADE_RULE        = 0x4,      /**< 交易所固定费用-交易规费 */
    OES_FEE_TYPE_EXCHANGE_EXCHANGE          = 0x5,      /**< 交易所固定费用-经手费 */
    OES_FEE_TYPE_EXCHANGE_ADMINFER          = 0x6,      /**< 交易所固定费用-证管费 */
    OES_FEE_TYPE_EXCHANGE_OTHER             = 0x7,      /**< 交易所固定费用-其他费 */
    __OES_FEE_TYPE_EXCHANGE_MAX,                        /**< 交易所固定费用最大值 */

    OES_FEE_TYPE_BROKER_BACK_END            = 0x11,     /**< 券商佣金-后台费用 */

    OES_FEE_TYPE_BROKER_CREDIT_INTEREST     = 0x21      /**< 券商息费-融资融券息费 */
} eOesFeeTypeT;


/**
 * 费用 (佣金/固定费用) 计算模式
 */
typedef enum _eOesCalcFeeMode {
    OES_CALC_FEE_MODE_AMOUNT                = 0,        /**< 按金额 */
    OES_CALC_FEE_MODE_QTY                   = 1,        /**< 按份额 */
    OES_CALC_FEE_MODE_ORD                   = 2         /**< 按笔数 */
} eOesCalcFeeModeT;


/**
 * 出入金方向定义
 */
typedef enum _eOesFundTrsfDirect {
    OES_FUND_TRSF_DIRECT_IN                 = 0,        /**< 转入OES (入金) */
    OES_FUND_TRSF_DIRECT_OUT                = 1         /**< 转出OES (出金) */
} eOesFundTrsfDirectT;


/**
 * 出入金转账类型定义
 */
typedef enum _eOesFundTrsfType {
    OES_FUND_TRSF_TYPE_OES_BANK             = 0,        /**< OES和银行之间转账 */
    OES_FUND_TRSF_TYPE_OES_COUNTER          = 1,        /**< OES和主柜之间划拨资金 */
    OES_FUND_TRSF_TYPE_COUNTER_BANK         = 2,        /**< 主柜和银行之间转账 */
    OES_FUND_TRSF_TYPE_OES_TO_OES           = 3,        /**< 沪深OES之间的内部资金划拨 */
    __OES_FUND_TRSF_TYPE_MAX                            /**< 出入金转账类型最大值 */
} eOesFundTrsfTypeT;


/**
 * 出入金委托状态
 */
typedef enum _eOesFundTrsfStatus {
    OES_FUND_TRSF_STS_UNDECLARED            = 0,        /**< 尚未上报 */
    OES_FUND_TRSF_STS_DECLARED              = 1,        /**< 已上报 */
    OES_FUND_TRSF_STS_WAIT_DONE             = 2,        /**< 主柜处理完成, 等待事务结束 */
    OES_FUND_TRSF_STS_DONE                  = 3,        /**< 出入金处理完成 */

    __OES_FUND_TRSF_STS_ROLLBACK_MIN        = 5,        /**< 废单判断标志 */
    OES_FUND_TRSF_STS_UNDECLARED_ROLLBACK   = 6,        /**< 待回滚(未上报前) */
    OES_FUND_TRSF_STS_DECLARED_ROLLBACK     = 7,        /**< 待回滚(已上报后) */

    __OES_FUND_TRSF_STS_INVALID_MIN         = 10,       /**< 废单判断标志 */
    OES_FUND_TRSF_STS_INVALID_OES           = 11,       /**< OES内部判断为废单 */
    OES_FUND_TRSF_STS_INVALID_COUNTER       = 12,       /**< 主柜判断为废单 */
    OES_FUND_TRSF_STS_SUSPENDED             = 13        /**< 挂起状态 (主柜的出入金执行状态未知, 待人工干预处理) */
} eOesFundTrsfStatusT;


/**
 * 出入金指令来源分类
 */
typedef enum _eOesFundTrsfSourceType {
    OES_FUND_TRSF_SOURCE_UNDEFINE           = 0,        /**< 未定义 */
    OES_FUND_TRSF_SOURCE_CUST               = 1,        /**< 客户发起 */
    OES_FUND_TRSF_SOURCE_TIMER              = 2,        /**< 系统内部定时任务发起 */
    OES_FUND_TRSF_SOURCE_COLO_PEER          = 3,        /**< 两地交易的对端结点发起 */
    __OES_FUND_TRSF_SOURCE_MAX
} eOesFundTrsfSourceTypeT;
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (5. 投资人/账户/权限相关)
 * =================================================================== */

/**
 * 业务类型定义
 */
typedef enum _eOesBusinessType {
    OES_BUSINESS_TYPE_UNDEFINE              = 0x0,      /**< 未定义的业务范围 */
    OES_BUSINESS_TYPE_STOCK                 = 0x01,     /**< 现货业务 */
    OES_BUSINESS_TYPE_OPTION                = 0x02,     /**< 期权业务 */
    OES_BUSINESS_TYPE_CREDIT                = 0x04,     /**< 信用业务 */

    _OES_BUSINESS_TYPE_MAX,                             /**< 业务范围最大值 (单一业务) */
    _OES_BUSINESS_TYPE_MAX_ALIGNED8         = 0x08,     /**< 业务范围最大值 (单一业务, 按8字节对齐的大小) */

    OES_BUSINESS_TYPE_ALL                   = 0xFF      /**< 所有业务 */
} eOesBusinessTypeT;


/**
 * 交易业务范围
 * @deprecated 已废弃, 改为使用 eOesBusinessTypeT
 */
typedef enum _eOesBusinessScope {
    /** 未定义的业务范围 @deprecated 已废弃 */
    OES_BIZ_SCOPE_UNDEFINE                  = OES_BUSINESS_TYPE_UNDEFINE,
    /** 现货业务 @deprecated 已废弃 */
    OES_BIZ_SCOPE_STOCK                     = OES_BUSINESS_TYPE_STOCK,
    /** 期权业务 @deprecated 已废弃 */
    OES_BIZ_SCOPE_OPTION                    = OES_BUSINESS_TYPE_OPTION,

    /** 所有业务 @deprecated 已废弃 */
    OES_BIZ_SCOPE_ALL                       = OES_BUSINESS_TYPE_ALL
} eOesBusinessScopeT;


/**
 * 账户类别定义
 * 资金账户类别与证券账户类别定义相同
 */
typedef enum _eOesAcctType {
    OES_ACCT_TYPE_NORMAL                    = 0,        /**< 普通账户 */
    OES_ACCT_TYPE_CREDIT                    = 1,        /**< 信用账户 */
    OES_ACCT_TYPE_OPTION                    = 2,        /**< 衍生品账户 */
    __OES_ACCT_TYPE_MAX,                                /**< 账户类别最大值 */
    __OES_ACCT_TYPE_MAX_ALIGNED4            = 4,        /**< 账户类别最大值 (按4字节对齐的大小) */
    __OES_ACCT_TYPE_MAX_ALIGNED8            = 8         /**< 账户类别最大值 (按8字节对齐的大小) */
} eOesAcctTypeT;


/**
 * 资金类型定义
 * @deprecated 已废弃, 改为使用 eOesAcctTypeT
 */
typedef enum _eOesCashType {
    /** 普通账户资金/现货资金 */
    OES_CASH_TYPE_SPOT                      = OES_ACCT_TYPE_NORMAL,
    /** 信用账户资金/信用资金 */
    OES_CASH_TYPE_CREDIT                    = OES_ACCT_TYPE_CREDIT,
    /** 衍生品账户资金/期权保证金 */
    OES_CASH_TYPE_OPTION                    = OES_ACCT_TYPE_OPTION,
    /** 资金类型最大值 */
    __OES_CASH_TYPE_MAX                     = __OES_ACCT_TYPE_MAX,
    /** 资金类型最大值 (按4字节对齐的大小) */
    __OES_CASH_TYPE_MAX_ALIGNED4            = __OES_ACCT_TYPE_MAX_ALIGNED4,
    /** 资金类型最大值 (按8字节对齐的大小) */
    __OES_CASH_TYPE_MAX_ALIGNED8            = __OES_ACCT_TYPE_MAX_ALIGNED8,

    /** 兼容性定义, 即将废弃 */
    OES_CASH_TYPE_CRE                       = OES_ACCT_TYPE_CREDIT,
    OES_CASH_TYPE_OPT                       = OES_ACCT_TYPE_OPTION,
} eOesCashTypeT;


/**
 * 客户状态/证券帐户/资金账户状态
 */
typedef enum _eOesAcctStatus {
    OES_ACCT_STATUS_NORMAL                  = 0,        /**< 正常 */
    OES_ACCT_STATUS_DISABLED                = 1,        /**< 非正常 */
    OES_ACCT_STATUS_LOCKED                  = 2         /**< 已锁定 */
} eOesAcctStatusT;


/**
 * 交易权限的枚举值定义
 */
typedef enum _eOesTradingPermission {
    OES_PERMIS_MARKET_ORDER                 = (1ULL << 1),      /**< 市价委托 */
    OES_PERMIS_STRUCTURED_FUND              = (1ULL << 2),      /**< 分级基金适当性 */
    OES_PERMIS_BOND_QUALIFIED_INVESTOR      = (1ULL << 3),      /**< 债券合格投资者 */

    OES_PERMIS_DELISTING                    = (1ULL << 5),      /**< 退市整理股票 */
    OES_PERMIS_RISK_WARNING                 = (1ULL << 6),      /**< 风险警示股票 */

    OES_PERMIS_SINGLE_MARKET_ETF            = (1ULL << 7),      /**< 单市场ETF申赎 */
    OES_PERMIS_CROSS_BORDER_ETF             = (1ULL << 8),      /**< 跨境ETF申赎 */
    OES_PERMIS_CROSS_MARKET_ETF             = (1ULL << 9),      /**< 跨市场ETF申赎 */
    OES_PERMIS_CURRENCY_ETF                 = (1ULL << 10),     /**< 货币ETF申赎 */

    OES_PERMIS_GEMCDR                       = (1ULL << 11),     /**< 创业板存托凭证 */
    OES_PERMIS_GEM_REGISTRATION             = (1ULL << 12),     /**< 注册制创业板交易 */
    OES_PERMIS_GEM_UNREGISTRATION           = (1ULL << 13),     /**< 核准制创业板交易 */
    OES_PERMIS_SH_HK_STOCK_CONNECT          = (1ULL << 14),     /**< 沪港通 */
    OES_PERMIS_SZ_HK_STOCK_CONNECT          = (1ULL << 15),     /**< 深港通 */

    OES_PERMIS_HLTCDR                       = (1ULL << 16),     /**< 沪伦通存托凭证 */
    OES_PERMIS_CDR                          = (1ULL << 17),     /**< 存托凭证 */
    OES_PERMIS_INNOVATION                   = (1ULL << 18),     /**< 创新企业股票 */
    OES_PERMIS_KSH                          = (1ULL << 19),     /**< 科创板交易 */

    OES_PERMIS_BOND_ETF                     = (1ULL << 20),     /**< 债券ETF申赎 */
    OES_PERMIS_GOLD_ETF                     = (1ULL << 21),     /**< 黄金ETF申赎 */
    OES_PERMIS_COMMODITY_FUTURES_ETF        = (1ULL << 22),     /**< 商品期货ETF申赎 */
    OES_PERMIS_GEM_INNOVATION               = (1ULL << 23),     /**< 创业板创新企业股票 */

    OES_PERMIS_CONVERTIBLE_BOND             = (1ULL << 24),     /**< 可转换公司债券 */
    OES_PERMIS_REITS                        = (1ULL << 25),     /**< 基础设施基金 */
    OES_PERMIS_CORPORATE_BOND               = (1ULL << 26),     /**< 公司债/企业债 */


    __OES_PERMIS_ALL                        = 0xFFFFFFFFFFFFFFFF,
                                                                /**< 全部权限 */

    /*
     * 以下定义已废弃, 只是为了兼容之前的版本而暂时保留
     */
    OES_PERMIS_GEM                          = OES_PERMIS_GEM_UNREGISTRATION

} eOesTradingPermissionT;


/**
 * 投资者适当性管理分类
 */
typedef enum _eOesQualificationClass {
    OES_QUALIFICATION_PUBLIC_INVESTOR       = 0,        /**< 公众投资者 */
    OES_QUALIFICATION_QUALIFIED_INVESTOR    = 1,        /**< 合格投资者(个人投资者) */
    OES_QUALIFICATION_QUALIFIED_INSTITUTIONAL
                                            = 2,        /**< 合格投资者(机构投资者) */
    __OES_QUALIFICATION_MAX                             /**< 投资者适当性管理分类的最大值 */
} eOesQualificationClassT;


/**
 * 投资者分类
 *
 * A类专业投资者: 满足《证券期货投资者适当性管理办法》第八条 (一)、 (二)、 (三) 点,
 *      比如证券公司、期货公司、基金管理公司、商业银行、保险公司、发行的理财产品等
 * B类专业投资者: 满足《证券期货投资者适当性管理办法》第八条 (四)、 (五) 点,
 *      可以是法人或者其他组织、自然人, 满足一定的净资产和金融资产的要求, 具有相关的投资经验
 * C类专业投资者: 满足《证券期货投资者适当性管理办法》第十一条 (一)、 (二) 点,
 *      由普通投资者主动申请转化而来, 满足一定的净资产和金融资产的要求, 具有相关的投资经验
 */
typedef enum _eOesInvestorClass {
    OES_INVESTOR_CLASS_NORMAL               = 0,        /**< 普通投资者 */
    OES_INVESTOR_CLASS_PROFESSIONAL_A       = 1,        /**< A类专业投资者 */
    OES_INVESTOR_CLASS_PROFESSIONAL_B       = 2,        /**< B类专业投资者 */
    OES_INVESTOR_CLASS_PROFESSIONAL_C       = 3,        /**< C类专业投资者 */
    OES_INVESTOR_CLASS_PROFESSIONAL         = 4,        /**< 专业投资者 */
    __OES_INVESTOR_CLASS_MAX                            /**< 投资者分类的最大值 */
} eOesInvestorClassT;


/**
 * 客户类型定义
 */
typedef enum _eOesCustType {
    OES_CUST_TYPE_PERSONAL                  = 0,        /**< 个人 */
    OES_CUST_TYPE_INSTITUTION               = 1,        /**< 机构 */
    OES_CUST_TYPE_PROPRIETARY               = 2,        /**< 自营 */
    OES_CUST_TYPE_PRODUCT                   = 3,        /**< 产品 */
    OES_CUST_TYPE_MKT_MAKER                 = 4,        /**< 做市商 */
    OES_CUST_TYPE_OTHERS                    = 5,        /**< 其他 */
    __OES_CUST_TYPE_MAX                                 /**< 客户类型的最大值 */
} eOesCustTypeT;


/**
 * 所有者类型 (内部使用)
 */
typedef enum _eOesOwnerType {
    OES_OWNER_TYPE_UNDEFINE                 = 0,        /**< 未定义 */
    OES_OWNER_TYPE_PERSONAL                 = 1,        /**< 个人投资者 */
    OES_OWNER_TYPE_EXCHANGE                 = 101,      /**< 交易所 */
    OES_OWNER_TYPE_MEMBER                   = 102,      /**< 会员 */
    OES_OWNER_TYPE_INSTITUTION              = 103,      /**< 机构投资者 */
    OES_OWNER_TYPE_PROPRIETARY              = 104,      /**< 自营 */
    OES_OWNER_TYPE_MKT_MAKER                = 105,      /**< 做市商 */
    OES_OWNER_TYPE_SETTLEMENT               = 106,      /**< 结算机构 */
    __OES_OWNER_TYPE_MAX                                /**< 所有者类型的最大值 */
} eOesOwnerTypeT;


/**
 * 客户端类型定义 (内部使用)
 */
typedef enum _eOesClientType {
    OES_CLIENT_TYPE_UNDEFINED               = 0,        /**< 客户端类型-未定义 */
    OES_CLIENT_TYPE_INVESTOR                = 1,        /**< 普通投资人 */
    OES_CLIENT_TYPE_VIRTUAL                 = 2         /**< 虚拟账户 (仅开通行情, 不可交易) */
} eOesClientTypeT;


/**
 * 客户端状态定义 (内部使用)
 */
typedef enum _eOesClientStatus {
    OES_CLIENT_STATUS_UNACTIVATED           = 0,        /**< 未激活 (不加载) */
    OES_CLIENT_STATUS_ACTIVATED             = 1,        /**< 已激活 (正常加载) */
    OES_CLIENT_STATUS_PAUSE                 = 2,        /**< 已暂停 (正常加载, 不可交易) */
    OES_CLIENT_STATUS_SUSPENDED             = 3,        /**< 已挂起 (不加载) */
    OES_CLIENT_STATUS_CANCELLED             = 4         /**< 已注销 (不加载) */
} eOesClientStatusT;
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (6. 通知消息相关)
 * =================================================================== */

/**
 * 通知消息来源分类
 */
typedef enum _eOesNotifySource {
    OES_NOTIFY_SOURCE_UNDEFINE              = 0,        /**< 未定义 */
    OES_NOTIFY_SOURCE_OES                   = 1,        /**< OES 交易系统发起 */
    OES_NOTIFY_SOURCE_MON                   = 2,        /**< MON 监控管理端发起 */
    OES_NOTIFY_SOURCE_BROKER                = 3,        /**< BROKER 期权经营机构发起 */
    OES_NOTIFY_SOURCE_EXCHANGE              = 4,        /**< EXCHANGE 交易所发起 */
    OES_NOTIFY_SOURCE_CSDC                  = 5,        /**< CSDC 中国结算发起 */
    __OES_NOTIFY_SOURCE_MAX                             /**< 通知消息来源分类最大值 */
} eOesNotifySourceT;


/**
 * 通知消息类型
 */
typedef enum _eOesNotifyType {
    OES_NOTIFY_TYPE_UNDEFINE                = 0,        /**< 未定义 */

    /*
     * 从MON管理端下发的通知消息
     */
    OES_NOTIFY_TYPE_CONTRACT_EXPIRE         = 1,        /**< 合约即将到期 */
    OES_NOTIFY_TYPE_CONTRACT_ADJUSTED       = 2,        /**< 合约近期有调整 */
    OES_NOTIFY_TYPE_UNDERLYING_DR_PROXIMITY = 3,        /**< 合约标的即将除权除息 */
    OES_NOTIFY_TYPE_EXERCISE_DATE_PROXIMITY = 4,        /**< 合约临近行权日 */
    OES_NOTIFY_TYPE_EXERCISED_POSSIBILITY   = 5,        /**< 合约可能被行权 */
    OES_NOTIFY_TYPE_EXERCISE_ASSIGNED       = 6,        /**< 合约被指派行权 */
    OES_NOTIFY_TYPE_COVERED_NOT_ENOUGH      = 7,        /**< 备兑证券标的不足 */
    OES_NOTIFY_TYPE_DELIVERY_NOT_ENOUGH     = 8,        /**< 交收证券不足 */
    OES_NOTIFY_TYPE_MARGIN_CALL             = 9,        /**< 追加保证金 */
    OES_NOTIFY_TYPE_FORCED_CLOSE            = 10,       /**< 强制平仓 */
    /* -------------------------           */

    /*
     * 由OES主动触发的通知消息
     */
    OES_NOTIFY_TYPE_CRD_COLLATERAL_INFO_UPDATE
                                            = 61,       /**< 融资融券担保品信息更新 */
    OES_NOTIFY_TYPE_CRD_UNDERLYING_INFO_UPDATE
                                            = 62,       /**< 融资融券标的信息更新 */
    OES_NOTIFY_TYPE_CRD_CASH_POSITION_UPDATE
                                            = 63,       /**< 融资融券资金头寸信息更新 */
    OES_NOTIFY_TYPE_CRD_SECURITY_POSITION_UPDATE
                                            = 64,       /**< 融资融券证券头寸信息更新 */
    OES_NOTIFY_TYPE_CRD_MAINTENANCE_RATIO_UPDATE
                                            = 65,       /**< 融资融券维持担保比例更新 */
    OES_NOTIFY_TYPE_CRD_LINE_OF_CERDIT_UPDATE
                                            = 66,       /**< 融资融券授信额度更新 */
    /* -------------------------           */

    OES_NOTIFY_TYPE_OTHERS                  = 100,      /**< 其它 */
    __OES_NOTIFY_TYPE_MAX                               /**< 通知消息类型最大值 */
} eOesNotifyTypeT;


/**
 * 通知消息等级
 */
typedef enum _eOesNotifyLevel {
    OES_NOTIFY_LEVEL_UNDEFINE               = 0,        /**< 未定义 */
    OES_NOTIFY_LEVEL_LOW                    = 1,        /**< 较低 */
    OES_NOTIFY_LEVEL_GENERAL                = 2,        /**< 一般 */
    OES_NOTIFY_LEVEL_IMPORTANT              = 3,        /**< 重要 */
    OES_NOTIFY_LEVEL_URGENT                 = 4,        /**< 紧急 */
    __OES_NOTIFY_LEVEL_MAX                              /**< 通知消息等级最大值 */
} eOesNotifyLevelT;


/**
 * 消息通知范围
 */
typedef enum _eOesNotifyScope {
    OES_NOTIFY_SCOPE_UNDEFINE               = 0,        /**< 未定义 */
    OES_NOTIFY_SCOPE_CUST                   = 1,        /**< 通知指定客户 */
    OES_NOTIFY_SCOPE_ALL                    = 2,        /**< 通知所有投资者 */
    __OES_NOTIFY_SCOPE_MAX                              /**< 通知消息范围最大值 */
} eOesNotifyScopeT;
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (7. 期权业务相关)
 * =================================================================== */

/**
 * 期权合约类型 (认购/认沽)
 */
typedef enum _eOesOptContractType {
    OES_OPT_CONTRACT_TYPE_UNDEFINE          = 0,        /**< 未定义 */
    OES_OPT_CONTRACT_TYPE_CALL              = 1,        /**< 认购 */
    OES_OPT_CONTRACT_TYPE_PUT               = 2,        /**< 认沽 */
    __OES_OPT_CONTRACT_TYPE_MAX                         /**< 合约类型最大值 */
} eOesOptContractTypeT;


/**
 * 限制开仓标志
 */
typedef enum _eOesOptLimitOpenFlag {
    OES_OPT_LIMIT_OPEN_FLAG_NORMAL          = 0,        /**< 可以开仓 */
    OES_OPT_LIMIT_OPEN_FLAG_LIMITED         = 1         /**< 限制卖出开仓(不包括备兑开仓)和买入开仓 */
} eOesOptLimitOpenFlagT;


/**
 * 期权行权方式 (欧式/美式)
 */
typedef enum _eOesOptExerciseType {
    OES_OPT_EXERCISE_TYPE_E                 = 0,        /**< 欧式 */
    OES_OPT_EXERCISE_TYPE_A                 = 1,        /**< 美式 */
    OES_OPT_EXERCISE_TYPE_B                 = 2,        /**< 百慕大式 */
    __OES_OPT_EXERCISE_TYPE_MAX                         /**< 行权方式最大值 */
} eOesOptExerciseTypeT;


/**
 * 期权交割方式 (证券结算/现金结算, 适用于深交所)
 */
typedef enum _eOesOptDeliveryType {
    OES_OPT_DELIVERY_TYPE_UNDEFINE          = 0,        /**< 未定义 */
    OES_OPT_DELIVERY_TYPE_SECURITY          = 1,        /**< 证券结算 */
    OES_OPT_DELIVERY_TYPE_CASH              = 2,        /**< 现金结算 */
    __OES_OPT_DELIVERY_TYPE_MAX                         /**< 交割方式最大值 */
} eOesOptDeliveryTypeT;


/**
 * 期权持仓类型
 */
typedef enum _eOesOptPositionType {
    OES_OPT_POSITION_TYPE_UNDEFINE          = 0,        /**< 未定义 */
    OES_OPT_POSITION_TYPE_LONG              = 1,        /**< 权利方 */
    OES_OPT_POSITION_TYPE_SHORT             = 2,        /**< 义务方 */
    OES_OPT_POSITION_TYPE_COVERED           = 3,        /**< 备兑方 */
    __OES_OPT_POSITION_TYPE_MAX                         /**< 期权持仓类型最大值 */
} eOesOptPositionTypeT;


/**
 * 投资者期权等级
 */
typedef enum _eOesOptInvLevel {
    OES_OPT_INV_LEVEL_UNDEFINE              = 0,        /**< 未定义 (机构投资者) */
    OES_OPT_INV_LEVEL_1                     = 1,        /**< 个人投资者-一级交易权限 */
    OES_OPT_INV_LEVEL_2                     = 2,        /**< 个人投资者-二级交易权限 */
    OES_OPT_INV_LEVEL_3                     = 3,        /**< 个人投资者-三级交易权限 */
    __OES_OPT_INV_LEVEL_MAX                             /**< 期权投资人级别最大值 */
} eOesOptInvLevelT;


/**
 * 仓位影响 (平仓标识)
 */
typedef enum _eOesPositionEffect {
    OES_POSITION_EFFECT_UNDEFINE            = 0,        /**< 未定义 */
    OES_POSITION_EFFECT_OPEN                = 1,        /**< 开仓 */
    OES_POSITION_EFFECT_CLOSE               = 2,        /**< 平仓 */
    __OES_POSITION_EFFECT_MAX                           /**< 仓位影响最大值 */
} eOesPositionEffectT;
/* -------------------------           */


/* ===================================================================
 * 枚举类型定义 (8. 融资融券业务相关)
 * =================================================================== */

/**
 * 融资融券负债类型
 */
typedef enum _eOesCrdDebtType {
    OES_CRD_DEBT_TYPE_UNDEFINE              = 0,        /**< 未定义的负债类型 */
    OES_CRD_DEBT_TYPE_MARGIN_BUY            = 1,        /**< 融资负债 */
    OES_CRD_DEBT_TYPE_SHORT_SELL            = 2,        /**< 融券负债 */
    OES_CRD_DEBT_TYPE_OTHER_DEBT            = 3,        /**< 其它负债 */
    __OES_CRD_DEBT_TYPE_MAX
} eOesCrdDebtTypeT;


/**
 * 头寸性质
 */
typedef enum _eOesCrdCashGroupProperty {
    OES_CRD_CASH_GROUP_PROP_UNDEFINE        = 0,        /**< 未定义的头寸性质 */
    OES_CRD_CASH_GROUP_PROP_PUBLIC          = 1,        /**< 公共头寸 */
    OES_CRD_CASH_GROUP_PROP_SPECIAL         = 2,        /**< 专项头寸 */
    __OES_CRD_CASH_GROUP_PROP_MAX
} eOesCrdCashGroupPropertyT;


/**
 * 融资融券负债状态
 */
typedef enum _eOesCrdDebtStatus {
    OES_CRD_DEBT_STATUS_UNDEFINE            = 0,        /**< 未定义的负债状态 */
    OES_CRD_DEBT_STATUS_NOT_TRADE           = 1,        /**< 合约尚未成交 */
    OES_CRD_DEBT_STATUS_NOT_REPAID          = 2,        /**< 未归还 */
    OES_CRD_DEBT_STATUS_PARTIALLY_REPAID    = 3,        /**< 部分归还 */
    OES_CRD_DEBT_STATUS_EXPIRED             = 4,        /**< 到期未了结 */

    OES_CRD_DEBT_STATUS_REPAID              = 5,        /**< 客户自行了结 */
    OES_CRD_DEBT_STATUS_MANNUAL_REPAID      = 6,        /**< 手工了结 */
    OES_CRD_DEBT_STATUS_NOT_DEBT            = 7,        /**< 未形成负债 */
    __OES_CRD_DEBT_STATUS_MAX
} eOesCrdDebtStatusT;


/**
 * 融资融券负债展期状态
 */
typedef enum _eOesCrdDebtPostponeStatus {
    OES_CRD_DEBT_POSTPONE_STATUS_UNDEFINE   = 0,        /**< 未定义的展期状态 */
    OES_CRD_DEBT_POSTPONE_STATUS_APPLICABLE = 1,        /**< 可申请 */
    OES_CRD_DEBT_POSTPONE_STATUS_APPLIED    = 2,        /**< 已申请 */
    OES_CRD_DEBT_POSTPONE_STATUS_APPROVED   = 3,        /**< 审批通过 */
    OES_CRD_DEBT_POSTPONE_STATUS_UNAPPROVED = 4,        /**< 审批不通过 */
    OES_CRD_DEBT_POSTPONE_STATUS_UNAPPLICABLE
                                            = 5,        /**< 不可申请 */
    __OES_CRD_DEBT_POSTPONE_STATUS_MAX
} eOesCrdDebtPostponeStatusT;


/**
 * 融资融券合同约定的负债归还模式
 */
typedef enum _eOesCrdDebtRepayMode {
    OES_CRD_DEBT_REPAY_MODE_UNDEFINE        = 0,        /**< 未定义的负债归还模式 */
    OES_CRD_DEBT_REPAY_MODE_MATCHING_PRINCIPAL
                                            = 1,        /**< 按比例归还 (利随本清) */
    OES_CRD_DEBT_REPAY_MODE_INTEREST_FIRST  = 2,        /**< 优先归还息费 (先息后本) */
    OES_CRD_DEBT_REPAY_MODE_PRINCIPAL_FIRST = 3,        /**< 优先归还本金 (先本后息) */
    __OES_CRD_DEBT_REPAY_MODE_MAX_COMPACT,              /**< 融资融券合同约定的负债归还模式的最大值 */

    __OES_CRD_DEBT_REPAY_MODE_INTEREST_ONLY = 10,       /**< 仅归还息费 (仅适用于API接口 @see OES_CRD_ASSIGNABLE_REPAY_MODE_INTEREST_ONLY) */
    __OES_CRD_DEBT_REPAY_MODE_MAX
} eOesCrdDebtRepayModeT;


/**
 * 可以由API接口指定的融资融券负债归还模式
 */
typedef enum _eOesCrdAssignableRepayMode {
    OES_CRD_ASSIGNABLE_REPAY_MODE_DEFAULT   = 0,        /**< 默认的负债归还模式 (使用融资融券合同约定的负债归还模式) */
    OES_CRD_ASSIGNABLE_REPAY_MODE_INTEREST_ONLY
                                            = 10,       /**< 仅归还息费 */
    __OES_CRD_ASSIGNABLE_REPAY_MODE_MAX
} eOesCrdAssignableRepayModeT;


/**
 * 融资融券负债流水类型
 */
typedef enum _eOesCrdDebtJournalType {
    OES_CRD_DEBT_JOURNAL_TYPE_OPEN_POSITION = 0,        /**< 合约开仓 */
    OES_CRD_DEBT_JOURNAL_TYPE_REPAY_MARGIN_BY_SELL
                                            = 1,        /**< 卖券还款 */
    OES_CRD_DEBT_JOURNAL_TYPE_REPAY_MARGIN_DIRECT
                                            = 2,        /**< 直接还款 */
    OES_CRD_DEBT_JOURNAL_TYPE_REPAY_STOCK_BY_BUY
                                            = 3,        /**< 买券还券 */
    OES_CRD_DEBT_JOURNAL_TYPE_REPAY_STOCK_DIRECT
                                            = 4,        /**< 直接还券 */
    OES_CRD_DEBT_JOURNAL_TYPE_REPAY_STOCK_BY_CASH
                                            = 5,        /**< 现金了结融券负债 */
    OES_CRD_DEBT_JOURNAL_TYPE_REPAY_STOCK_BY_OUTSIDE
                                            = 6,        /**< 场外了结融券负债 */
    OES_CRD_DEBT_JOURNAL_TYPE_REPAY_MARGIN_BY_OUTSIDE
                                            = 7,        /**< 场外了结融资负债 */
    OES_CRD_DEBT_JOURNAL_TYPE_CONTRACT_POST_PONE
                                            = 8,        /**< 合约展期(审批) */
    OES_CRD_DEBT_JOURNAL_TYPE_OTHER         = 9,        /**< 其它类型 */

    __OES_CRD_DEBT_JOURNAL_TYPE_MAX
} eOesCrdDebtJournalTypeT;


/**
 * 信用客户警戒状态
 */
typedef enum _eOesCrdCustGuardStatus {
    OES_CRD_CUST_GUARD_STATUS_NORMAL        = 0,        /**< 正常 */
    OES_CRD_CUST_GUARD_STATUS_ALERT         = 1,        /**< 警惕 */
    OES_CRD_CUST_GUARD_STATUS_BLOCKLIST     = 2,        /**< 黑名单 */
    __OES_CRD_CUST_GUARD_STATUS_MAX
} eOesCrdCustGuardStatusT;
/* -------------------------           */


#ifdef __cplusplus
}
#endif

#endif  /* _OES_BASE_CONSTANTS_H */
