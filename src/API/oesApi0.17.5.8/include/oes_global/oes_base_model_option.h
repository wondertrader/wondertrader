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
 * @file    oes_base_model_option.h
 *
 * 订单执行系统的期权业务基础领域模型(数据结构)定义
 *
 * @version 0.16        2019/11/20
 *          - 新增 期权持仓回报结构体定义(OesOptHoldingReportT)
 *          - 新增 期权标的持仓回报信息的结构体定义(OesOptUnderlyingHoldingReportT)
 *          - 新增 期权结算单确认信息的结构体定义(OesOptSettlementConfirmBaseInfoT)
 *          - 新增 期权结算单确认回报信息的结构体定义(OesOptSettlementConfirmReportT)
 *          - 资金回报 (OesCashAssetReportT) 中增加限购额度相关字段
 *              - 上海市场可用限购额度 (sseAvailablePurchaseLimit)
 *              - 深圳市场可用限购额度 (szseAvailablePurchaseLimit)
 *          - 期权持仓变动 (OesOptHoldingReportT) 中增加限仓额度相关字段
 *              - 可用的权利仓限额 (availableLongPositionLimit)
 *              - 可用的总持仓限额 (availableTotalPositionLimit)
 *              - 可用的单日买入开仓限额 (availableDailyBuyOpenLimit)
 *          - 调整期权产品基础信息(OesOptionBaseInfoT)的结构体定义
 *              - 增加 '临近到期天数(expireDays)' 字段
 * @version 0.16.0.2    2019/12/27
 *          - '期权行权指派基础信息 (OesOptionExerciseAssignBaseT)' 中
 *              - 新增 期权合约名称 (securityName) 字段
 * @version 0.16.0.4    2020/02/28
 *          - '期权标的持仓基础信息 (OesOptUnderlyingHoldingBaseInfoT)' 中
 *              - 新增 当日最大可减持额度 (maxReduceQuota) 字段
 *          - '资金变动回报信息 (OesCashAssetReportT)' 中
 *              - 新增 未对冲实时价格保证金 (totalMarketMargin) 字段
 *              - 新增 已对冲实时价格保证金 (totalNetMargin) 字段
 * @version 0.17.1.1    2021/06/22
 *          - 提取单独的期权业务领域模型定义文件
 *
 * @since   0.17.1.1    2021/06/22
 */


#ifndef _OES_BASE_MODEL_OPTION_H
#define _OES_BASE_MODEL_OPTION_H


#include    <oes_global/oes_base_constants.h>
#include    <sutil/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ===================================================================
 * 期权产品基础信息的结构体定义
 * =================================================================== */

/**
 * 期权产品基础信息的内容定义
 */
#define __OES_OPTION_BASE_INFO_PKT                                      \
        /** 期权合约代码 */ \
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
        /** 合约类型 (认购/认沽) @see eOesOptContractTypeT */ \
        uint8               contractType; \
        /** 行权方式 @see eOesOptExerciseTypeT */ \
        uint8               exerciseType; \
        /** 交割方式 @see eOesOptDeliveryTypeT */ \
        uint8               deliveryType; \
        /** 是否支持当日回转交易 (0: 不支持; 其他: 支持) */ \
        uint8               isDayTrading; \
        \
        /** 限制开仓标识 @see eOesOptLimitOpenFlagT */ \
        uint8               limitOpenFlag; \
        /** 禁止交易标识 (0:正常交易, 非0:禁止交易) @see eOesSecuritySuspFlagT */ \
        uint8               suspFlag; \
        /** 临时停牌标识 (0 未停牌, 1 已停牌) */ \
        uint8               temporarySuspFlag; \
        /** 按64位对齐的填充域 */ \
        uint8               __OPTION_BASE_filler1[5]; \
        \
        /** 合约单位 (经过除权除息调整后的单位) */ \
        int32               contractUnit; \
        /** 期权行权价 (经过除权除息调整后的价格, 单位精确到元后四位, 即1元 = 10000) */ \
        int32               exercisePrice; \
        /** 交割日期 (格式为YYYYMMDD) */ \
        int32               deliveryDate; \
        /** 交割月份 (格式为YYYYMM) */ \
        int32               deliveryMonth; \
        \
        /** 上市日期 (格式为YYYYMMDD) */ \
        int32               listDate; \
        /** 最后交易日 (格式为YYYYMMDD) */ \
        int32               lastTradeDay; \
        /** 行权起始日期 (格式为YYYYMMDD) */ \
        int32               exerciseBeginDate; \
        /** 行权结束日期 (格式为YYYYMMDD) */ \
        int32               exerciseEndDate; \
        \
        /** 合约持仓量 (当前合约未平仓数) */ \
        int64               contractPosition; \
        /** 合约前收盘价 (单位精确到元后四位, 即1元 = 10000) */ \
        int32               prevClosePrice; \
        /** 合约前结算价 (单位精确到元后四位, 即1元 = 10000) */ \
        int32               prevSettlPrice; \
        /** 标的证券前收盘价 (单位精确到元后四位, 即1元 = 10000) */ \
        int32               underlyingClosePrice; \
        \
        /** 最小报价单位 (单位精确到元后四位, 即1元 = 10000) */ \
        int32               priceTick; \
        /** 涨停价 (单位精确到元后四位, 即1元 = 10000) */ \
        int32               upperLimitPrice; \
        /** 跌停价 (单位精确到元后四位, 即1元 = 10000) */ \
        int32               lowerLimitPrice; \
        \
        /** 买入单位 */ \
        int32               buyQtyUnit; \
        /** 限价买数量上限 (单笔申报的最大张数) */ \
        int32               lmtBuyMaxQty; \
        /** 限价买数量下限 (单笔申报的最小张数) */ \
        int32               lmtBuyMinQty; \
        /** 市价买数量上限 (单笔申报的最大张数) */ \
        int32               mktBuyMaxQty; \
        /** 市价买数量下限 (单笔申报的最小张数) */ \
        int32               mktBuyMinQty; \
        \
        /** 卖出单位 */ \
        int32               sellQtyUnit; \
        /** 限价卖数量上限 (单笔申报的最大张数) */ \
        int32               lmtSellMaxQty; \
        /** 限价卖数量下限 (单笔申报的最小张数) */ \
        int32               lmtSellMinQty; \
        /** 市价卖数量上限 (单笔申报的最大张数) */ \
        int32               mktSellMaxQty; \
        /** 市价卖数量下限 (单笔申报的最小张数) */ \
        int32               mktSellMinQty; \
        \
        /** 单位保证金 (上调后的今卖开每张保证金, 单位精确到元后四位, 即1元 = 10000) */ \
        int64               sellMargin; \
        /** 原始的单位保证金 (未上调的今卖开每张保证金, 单位精确到元后四位, 即1元 = 10000) */ \
        int64               originalSellMargin; \
        /** 交易所保证金比例计算参数一 (单位:万分比) */ \
        int32               marginRatioParam1; \
        /** 交易所保证金比例计算参数二 (单位:万分比) */ \
        int32               marginRatioParam2; \
        /** 券商保证金上浮比例 (单位:万分比) */ \
        int32               increasedMarginRatio; \
        /** 临近到期天数 */ \
        int32               expireDays; \
        \
        /** 期权合约交易所代码 */ \
        char                contractId[OES_CONTRACT_EXCH_ID_MAX_LEN]; \
        /** 期权合约名称 (UTF-8 编码) */ \
        char                securityName[OES_SECURITY_NAME_MAX_LEN]; \
        /**
         * 期权合约状态信息
         * 该字段为 8 位字符串，左起每位表示特定的含义，无定义则填空格。
         * 第 1 位: ‘0’表示可开仓，‘1’表示限制卖出开仓（不包括备兑开仓）和买入开仓。
         * 第 2 位: 预留，暂填 ‘0’
         * 第 3 位: ‘0’表示未临近到期日，‘1’表示距离到期日不足 5 个交易日。
         * 第 4 位: ‘0’表示近期未做调整，‘1’表示最近 5 个交易日内合约发生过调整。
         * 第 5 位: ‘A’表示当日新挂牌的合约，‘E’表示存续的合约。
         */ \
        char                securityStatusFlag[OES_SECURITY_STATUS_FLAG_MAX_LEN]; \
        \
        /** 标的证券代码 */ \
        char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 标的市场代码 @see eOesMarketIdT */ \
        uint8               underlyingMktId; \
        /** 标的证券类型 @see eOesSecurityTypeT */ \
        uint8               underlyingSecurityType; \
        /** 按64位对齐的填充域 */ \
        uint8               __OPTION_BASE_filler3[6]; \
        /** 预留的备用字段 */ \
        char                __OPTION_BASE_reserve[32]


#define __NULLOBJ_OES_OPTION_BASE_INFO_PKT                              \
        {0}, 0, \
        0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, {0}, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, \
        {0}, {0}, {0}, \
        {0}, 0, 0, {0}, {0}
/* -------------------------           */


/**
 * 期权产品基础信息的结构体定义
 */
typedef struct _OesOptionBaseInfo {
    __OES_OPTION_BASE_INFO_PKT;
} OesOptionBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPTION_BASE_INFO                                    \
        __NULLOBJ_OES_OPTION_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 期权持仓信息的结构体定义
 * =================================================================== */

/**
 * 期权持仓基础信息的内容定义
 */
#define __OES_OPT_HOLDING_BASE_INFO_PKT                                 \
        /** 账户代码 */ \
        char                invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 期权合约代码 */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        /** 持仓类型 @see eOesOptPositionTypeT */ \
        uint8               positionType; \
        \
        /** 产品类型 @see eOesProductTypeT */ \
        uint8               productType; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 合约类型 (认购/认沽) @see eOesOptContractTypeT */ \
        uint8               contractType; \
        /** 套保标志 (0 非套保, 1 套保) */ \
        uint8               hedgeFlag; \
        /** 按64位对齐的填充域 */ \
        uint8               __HOLD_BASE_filler; \
        \
        /** 日初总持仓张数 */ \
        int64               originalQty; \
        /** 日初可用持仓 */ \
        int64               originalAvlQty; \
        /** 按摊薄持仓成本价计的日初总持仓成本 (日初摊薄持仓成本价 * 日初总持仓) */ \
        int64               originalCostAmt; \
        /** 权利仓的日初持有成本 (日初持仓均价 * 日初总持仓, 不含费用) */ \
        int64               originalCarryingAmt; \
        \
        /** 日中累计开仓张数 */ \
        int64               totalOpenQty; \
        /** 开仓委托未成交张数 */ \
        int64               uncomeQty; \
        /** 日中累计平仓张数 */ \
        int64               totalCloseQty; \
        /** 平仓在途冻结张数 */ \
        int64               closeFrzQty; \
        \
        /** 手动冻结张数 */ \
        int64               manualFrzQty; \
        \
        /** 日中累计获得权利金 */ \
        int64               totalInPremium; \
        /** 日中累计付出权利金 */ \
        int64               totalOutPremium; \
        /** 日中累计开仓费用 */ \
        int64               totalOpenFee; \
        /** 日中累计平仓费用 */ \
        int64               totalCloseFee; \
        \
        /** 权利仓行权冻结张数 */ \
        int64               exerciseFrzQty; \
        /** 义务仓占用保证金 */ \
        int64               positionMargin; \
        \
        /** 预留的备用字段 */ \
        char                __OPT_HOLDING_BASE_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_OPT_HOLDING_BASE_INFO_PKT                         \
        {0}, {0}, 0, 0, \
        0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, \
        0, 0, 0, 0, \
        0, 0, \
        {0}
/* -------------------------           */


/**
 * 期权持仓回报信息的内容定义
 */
#define __OES_OPT_HOLDING_RPT_INFO_PKT                                  \
        /** 可平仓张数 (单位: 张) */ \
        int64               closeAvlQty; \
        /** 可行权张数 (单位: 张) */ \
        int64               exerciseAvlQty; \
        /** 总持仓张数 (单位: 张) */ \
        int64               sumQty; \
        /** 摊薄持仓成本价 */ \
        int64               costPrice; \
        /** 权利仓的持仓均价 */ \
        int64               carryingAvgPrice; \
        /** 可用的备兑持仓数量 (已锁定的标的持仓数量, 单位: 股) */ \
        int64               coveredAvlUnderlyingQty; \
        \
        /*
         * 限仓额度信息
         */ \
        /** 可用的权利仓限额 */ \
        int32               availableLongPositionLimit; \
        /** 可用的总持仓限额 */ \
        int32               availableTotalPositionLimit; \
        /** 可用的单日买入开仓限额 */ \
        int32               availableDailyBuyOpenLimit; \
        /** 按64位对齐的填充域 */ \
        int32               __OPT_HOLDING_EXT_filler2; \
        /** 预留的备用字段 */ \
        char                __OPT_HOLDING_EXT_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_OPT_HOLDING_RPT_INFO_PKT                          \
        0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, {0}
/* -------------------------           */


/**
 * 期权持仓基础信息的结构体定义
 */
typedef struct _OesOptHoldingBaseInfo {
    __OES_OPT_HOLDING_BASE_INFO_PKT;
} OesOptHoldingBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_HOLDING_BASE_INFO                               \
        __NULLOBJ_OES_OPT_HOLDING_BASE_INFO_PKT
/* -------------------------           */


/**
 * 期权持仓回报结构体定义
 */
typedef struct _OesOptHoldingReport {
    __OES_OPT_HOLDING_BASE_INFO_PKT;
    __OES_OPT_HOLDING_RPT_INFO_PKT;
} OesOptHoldingReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_HOLDING_REPORT                                  \
        __NULLOBJ_OES_OPT_HOLDING_BASE_INFO_PKT, \
        __NULLOBJ_OES_OPT_HOLDING_RPT_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 期权标的持仓信息的结构体定义
 * =================================================================== */

/**
 * 期权标的持仓基础信息的内容定义
 */
#define __OES_OPT_UNDERLYING_HOLDING_BASE_PKT                           \
        /** 股东账户代码 (不带'888'编码的原始股东账户代码) */ \
        char                invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 标的证券代码 */ \
        char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN]; \
        \
        /** 期权市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        /** 标的市场代码 @see eOesMarketIdT */ \
        uint8               underlyingMktId; \
        /** 标的证券类型 @see eOesSecurityTypeT */ \
        uint8               underlyingSecurityType; \
        /** 标的证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               underlyingSubSecurityType; \
        /** 按64位对齐的填充域 */ \
        uint8               __OPT_UNDERLYING_HOLD_BASE_filler[4]; \
        \
        /** 日初标的证券的总持仓数量 (单位: 股) */ \
        int64               originalHld; \
        /** 日初标的证券的可用持仓数量 (单位: 股) */ \
        int64               originalAvlHld; \
        /** 日初备兑仓主柜实际占用的标的证券数量 (单位: 股) */ \
        int64               originalCoveredQty; \
        /** 日初备兑仓应占用的标的证券数量 (单位: 股) */ \
        int64               initialCoveredQty; \
        \
        /** 当前备兑仓实际占用的标的证券数量 (单位: 股) */ \
        int64               coveredQty; \
        /** 当前备兑仓占用标的证券的缺口数量 (单位: 股) */ \
        int64               coveredGapQty; \
        /** 当前可用于备兑开仓的标的持仓数量 (单位: 股) */ \
        int64               coveredAvlQty; \
        /** 当前可锁定的标的持仓数量 (单位: 股) */ \
        int64               lockAvlQty; \
        /** \
         * 标的证券总持仓, 包括当前可用持仓、不可交易持仓和在途冻结持仓在內的汇总值 (单位: 股) \
         */ \
        int64               sumHld; \
        \
        /**
         * 当日最大可减持额度
         * - 小于0, 不进行减持额度控制
         * - 大于或等于0, 最大可减持额度
         */ \
        int64               maxReduceQuota


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_OPT_UNDERLYING_HOLDING_BASE_PKT                   \
        {0}, {0}, \
        0, 0, 0, 0, {0}, \
        0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0
/* -------------------------           */


/**
 * 期权标的持仓基础信息的结构体定义
 */
typedef struct _OesOptUnderlyingHoldingBaseInfo {
    __OES_OPT_UNDERLYING_HOLDING_BASE_PKT;
} OesOptUnderlyingHoldingBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_UNDERLYING_HOLDING_BASE_INFO                    \
        __NULLOBJ_OES_OPT_UNDERLYING_HOLDING_BASE_PKT
/* -------------------------           */


/**
 * 期权标的持仓回报信息的结构体定义
 */
typedef struct _OesOptUnderlyingHoldingReport {
    __OES_OPT_UNDERLYING_HOLDING_BASE_PKT;
} OesOptUnderlyingHoldingReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_UNDERLYING_HOLDING_REPORT                       \
        __NULLOBJ_OES_OPT_UNDERLYING_HOLDING_REPORT_PKT
/* -------------------------           */


/* ===================================================================
 * 期权客户限仓额度信息的结构体定义
 * =================================================================== */

/**
 * 期权客户限仓额度基础信息的内容定义
 */
#define __OES_OPTION_POSITION_LIMIT_BASE_PKT                            \
        /** 股东账户代码 (不带'888'编码的原始股东账户代码) */ \
        char                invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 标的证券代码 */ \
        char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 (衍生品市场) @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** 标的市场代码 @see eOesMarketIdT */ \
        uint8               underlyingMktId; \
        /** 标的证券类型 @see eOesSecurityTypeT */ \
        uint8               underlyingSecurityType; \
        /** 标的证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               underlyingSubSecurityType; \
        /** 按64位对齐的填充域 */ \
        uint8               __POSITION_LIMIT_BASE_filler1[4]; \
        \
        /*
         * 限仓额度信息
         */ \
        /** 权利仓限额 */ \
        int32               longPositionLimit; \
        /** 总持仓限额 */ \
        int32               totalPositionLimit; \
        /** 单日买入开仓限额 */ \
        int32               dailyBuyOpenLimit; \
        /** 按64位对齐的填充域 */ \
        int32               __POSITION_LIMIT_BASE_filler2; \
        \
        /*
         * 合约品种维度的统计信息 (限仓相关)
         */ \
        /** 日初权利仓持仓数量 (单位: 张) */ \
        int32               originalLongQty; \
        /** 累计买入开仓数量 (单位: 张) */ \
        int32               totalBuyOpenQty; \
        /** 当前尚未成交的买入开仓数量 */ \
        int32               uncomeBuyOpenQty; \
        /** 累计卖出平仓数量 (单位: 张) */ \
        int32               totalSellCloseQty; \
        \
        /** 日初义务仓持仓数量 (单位: 张) */ \
        int32               originalShortQty; \
        /** 累计卖出开仓数量 (单位: 张) */ \
        int32               totalSellOpenQty; \
        /** 当前尚未成交的卖出开仓数量 */ \
        int32               uncomeSellOpenQty; \
        /** 累计买入平仓数量 (单位: 张) */ \
        int32               totalBuyCloseQty; \
        \
        /** 日初备兑义务仓持仓数量 (单位: 张) */ \
        int32               originalCoveredQty; \
        /** 累计备兑开仓数量 (单位: 张) */ \
        int32               totalCoveredOpenQty; \
        /** 当前尚未成交的备兑开仓数量 (单位: 张) */ \
        int32               uncomeCoveredOpenQty; \
        /** 累计备兑平仓数量 (单位: 张) */ \
        int32               totalCoveredCloseQty


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_OPTION_POSITION_LIMIT_BASE_PKT                    \
        {0}, {0}, 0, \
        0, 0, 0, {0}, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0
/* -------------------------           */


/**
 * 期权客户限仓额度基础信息的结构体定义
 */
typedef struct _OesOptionPositionLimitBaseInfo {
    __OES_OPTION_POSITION_LIMIT_BASE_PKT;
} OesOptionPositionLimitBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPTION_POSITION_LIMIT_BASE_INFO                     \
        __NULLOBJ_OES_OPTION_POSITION_LIMIT_BASE_PKT
/* -------------------------           */


/* ===================================================================
 * 行权指派信息的结构体定义
 * =================================================================== */

/**
 * 期权行权指派信息的内容定义
 */
#define __OES_OPTION_EXERCISE_ASSIGN_BASE_PKT                           \
        /** 股东账户代码 (不带'888'编码的原始股东账户代码) */ \
        char                invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 期权合约代码 */ \
        char                securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        /** 持仓方向 (权利: 行权方, 义务/备兑: 被行权方) @see eOesOptPositionTypeT */ \
        uint8               positionType; \
        \
        /** 产品类型 @see eOesProductTypeT */ \
        uint8               productType; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 合约类型 (认购/认沽) @see eOesOptContractTypeT */ \
        uint8               contractType; \
        /** 交割方式 @see eOesOptDeliveryTypeT */ \
        uint8               deliveryType; \
        /** 按64位对齐的填充域 */ \
        uint8               __OPTION_EXERCISE_ASSIGN_filler1; \
        \
        /** 行权价格 (单位精确到元后四位, 即1元 = 10000) */ \
        int32               exercisePrice; \
        /** 行权张数 */ \
        int32               exerciseQty; \
        /** 标的证券收付数量 (正数表示应收, 负数表示应付) */ \
        int64               deliveryQty; \
        \
        /** 行权开始日期 (格式为YYYYMMDD) */ \
        int32               exerciseBeginDate; \
        /** 行权结束日期 (格式为YYYYMMDD) */ \
        int32               exerciseEndDate; \
        /** 清算日期 (格式为YYYYMMDD) */ \
        int32               clearingDate; \
        /** 交收日期 (格式为YYYYMMDD) */ \
        int32               deliveryDate; \
        \
        /** 清算金额 */ \
        int64               clearingAmt; \
        /** 清算费用 (费用合计, 佣金+过户费+结算费+其它费用) */ \
        int64               clearingFee; \
        /** 实际收付金额 (正数表示应收, 负数表示应付) */ \
        int64               settlementAmt; \
        \
        /** 标的证券代码 */ \
        char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 标的市场代码 @see eOesMarketIdT */ \
        uint8               underlyingMktId; \
        /** 标的证券类型 @see eOesSecurityTypeT */ \
        uint8               underlyingSecurityType; \
        /** 按64位对齐的填充域 */ \
        uint8               __OPTION_EXERCISE_ASSIGN_filler3[6]; \
        \
        /** 期权合约名称 (UTF-8 编码) */ \
        char                securityName[OES_SECURITY_NAME_MAX_LEN]; \
        \
        /** 预留的备用字段 */ \
        char                __OPTION_EXERCISE_ASSIGN_reserve[16]


#define __NULLOBJ_OES_OPTION_EXERCISE_ASSIGN_BASE_PKT                   \
        {0}, {0}, 0, 0, \
        0, 0, 0, 0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, \
        {0}, 0, 0, {0}, \
        {0}, {0}
/* -------------------------           */


/**
 * 期权行权指派基础信息的结构体定义
 */
typedef struct _OesOptionExerciseAssignBase {
    __OES_OPTION_EXERCISE_ASSIGN_BASE_PKT;
} OesOptionExerciseAssignBaseT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPTION_EXERCISE_ASSIGN_BASE                         \
        __NULLOBJ_OES_OPTION_EXERCISE_ASSIGN_BASE_PKT
/* -------------------------           */


/* ===================================================================
 * 期权结算单确认信息的结构体定义
 * =================================================================== */

#define __OES_OPT_SETTLEMENT_CONFIRM_BASE_PKT                           \
        /** 客户代码 */ \
        char                custId[OES_CUST_ID_MAX_LEN]; \
        \
        /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */ \
        union { \
            uint64          u64;                    /**< uint64 类型的用户私有信息 */ \
            int64           i64;                    /**< int64 类型的用户私有信息 */ \
            uint32          u32[2];                 /**< uint32[2] 类型的用户私有信息 */ \
            int32           i32[2];                 /**< int32[2] 类型的用户私有信息 */ \
            char            c8[8];                  /**< char[8] 类型的用户私有信息 */ \
        } userInfo; \
        \
        /** 登录客户端编号 */ \
        int16               clientId; \
        /** 登录客户端环境号 */ \
        int8                clEnvId; \
        /** 按64位对齐的填充域 */ \
        int8                __filler2; \
        \
        /** 发生日期 (格式为 YYYYMMDD, 形如 20160830) */ \
        int32               transDate; \
        /** 发生时间 (格式为 HHMMSSsss, 形如 141205000) */ \
        int32               transTime; \
        /** 拒绝原因 */ \
        int32               rejReason; \
        /** 预留的备用字段 */ \
        char                __OPT_SETTLEMENT_CONFIRM_BASE_reserve[24]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_OPT_SETTLEMENT_CONFIRM_BASE_PKT                   \
        {0}, {0}, \
        0, 0, 0, \
        0, 0, 0, {0}
/* -------------------------           */


/**
 * 期权结算单确认信息的结构体定义
 */
typedef struct _OesOptSettlementConfirmBaseInfo {
    __OES_OPT_SETTLEMENT_CONFIRM_BASE_PKT;
} OesOptSettlementConfirmBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OPT_SETTLEMENT_CONFIRM_BASE_INFO                        \
        __NULLOBJ_OES_OPT_SETTLEMENT_CONFIRM_BASE_PKT
/* -------------------------           */


/**
 * 期权结算单确认回报信息的结构体定义
 */
typedef struct _OesOptSettlementConfirmReport {
    __OES_OPT_SETTLEMENT_CONFIRM_BASE_PKT;
} OesOptSettlementConfirmReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OPT_SETTLEMENT_CONFIRM_REPORT                           \
        __NULLOBJ_OES_OPT_SETTLEMENT_CONFIRM_BASE_PKT
/* -------------------------           */


#ifdef __cplusplus
}
#endif

#endif  /* _OES_BASE_MODEL_OPTION_H */
