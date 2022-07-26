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
 * @file    oes_base_model_credit.h
 *
 * 订单执行系统的融资融券业务基础领域模型(数据结构)定义
 *
 * @version 0.17        2020/09/16
 * @version 0.17.0.10   2021/05/21
 *          - 调整 '融资融券合约负债流水的基础信息 (OesCrdDebtJournalBaseInfoT)' 结构体定义
 *              - 新增 融券合约流水的理论发生金额 (shortSellTheoryOccurAmt) 字段
 * @version 0.17.0.11   2021/06/05
 *          - 调整 '信用资产基础信息 (OesCrdCreditAssetBaseInfoT)' 结构体定义
 *              - 调整字段定义顺序
 *              - 新增 买担保品可用余额 (buyCollateralAvailableBal) 字段
 *              - 新增 买券还券可用余额 (repayStockAvailableBal) 字段
 *              - 新增 融资买入已用授信额度 (marginBuyUsedQuota) 字段
 *              - 新增 融券卖出已用授信额度 (shortSellUsedQuota) 字段
 *              - 新增 专项资金头寸金额 (specialCashPositionAmt) 字段
 *              - 新增 专项资金头寸可用余额 (specialCashPositionAvailableBal) 字段
 *              - 新增 公共资金头寸金额 (publicCashPositionAmt) 字段
 *              - 新增 公共资金头寸可用余额 (publicCashPositionAvailableBal) 字段
 * @version 0.17.1      2021/06/21
 *          - 调整 '信用资产基础信息 (OesCrdCreditAssetBaseInfoT)' 结构体定义
 *              - 重命名 证券持仓市值 (holdingMarketCap) => collateralHoldingMarketCap,
 *                @note 字段取值含义也调整为包含担保品转出冻结和直接还券冻结在内的持仓总市值
 *              - 新增 融资融券其他费用 (otherCreditFee) 字段
 *          - 调整 '客户单证券融资融券负债统计基础信息 (OesCrdSecurityDebtStatsBaseInfoT)' 结构体定义
 *              - 调整字段定义顺序
 *              - 删除 '证券持仓市值 (holdingMarketCap)' 等市值字段
 *              - 删除 '证券名称 (securityName)' 字段
 *              - 新增市值计算使用的证券价格、证券头寸可用数量等字段
 *              - 重命名 合计待归还负债数量 (securityRepayableQty) => shortSellRepayableDebtQty
 *              - 重命名 转出冻结的持仓数量 (collateralTotalTrsfOutQty) => collateralTrsfOutQty
 *              - 重命名 直接还券冻结的持仓数量 (collateralTotalRepayDirectQty) => collateralRepayDirectQty
 *          - 调整 '融资融券合约回报信息 (OesCrdDebtContractReportT)' 结构体定义
 *              - 新增 资金账户代码(cashAcctId) 字段
 *              - 重命名 合计待归还负债数量 (securityRepayableQty) => shortSellRepayableDebtQty
 *              - 重命名 合约当前待归还负债数量 (contractRepayableQty) => contractRepayableDebtQty
 * @version 0.17.1.1    2021/06/23
 *          - 重命名头文件 oes_base_credit_model.h => oes_base_model_credit.h
 *          - 调整 '客户单证券融资融券负债统计基础信息 (OesCrdSecurityDebtStatsBaseInfoT)' 结构体定义
 *              - 删除 '客户代码 (custId)' 字段
 * @version 0.17.1.3    2021/07/15
 *          - 调整 '客户单证券融资融券负债统计基础信息 (OesCrdSecurityDebtStatsBaseInfoT)' 结构体定义
 *              - 增加 '日初融资负债金额 (marginBuyOriginDebtAmt)' 字段
 *              - 增加 '日初融资负债数量 (marginBuyOriginDebtQty)' 字段
 *              - 增加 '当日已归还融资金额 (marginBuyRepaidAmt)' 字段
 *              - 增加 '当日已归还融资数量 (marginBuyRepaidQty)' 字段
 * @version 0.17.4.6    2022/01/14
 *          - 调整 '信用资产基础信息 (OesCrdCreditAssetBaseInfoT)' 结构体定义 (保持兼容)
 *              - 增加 '可转出资产价值 (trsfOutAbleAssetValue)' 字段
 *              - 增加 '标的证券市值 (underlyingMarketCap)' 字段
 * @version 0.17.4.7    2022/04/02
 *          - 调整 '信用资产基础信息 (OesCrdCreditAssetBaseInfoT)' 结构体定义 (保持兼容)
 *              - 增加 '修正资产价值 (correctAssetValue)' 字段
 *
 * @since   0.17        2020/09/16
 */


#ifndef _OES_BASE_MODEL_CREDIT_H
#define _OES_BASE_MODEL_CREDIT_H


#include    <oes_global/oes_base_constants.h>
#include    <sutil/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ===================================================================
 * 信用资产基础信息定义
 * =================================================================== */

/**
 * 客户信用资产基础信息的结构体定义
 */
typedef struct _OesCrdCreditAssetBaseInfo {
    /** 资金账户代码 */
    char                cashAcctId[OES_CASH_ACCT_ID_MAX_LEN];
    /** 客户代码 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 币种 @see eOesCurrTypeT */
    uint8               currType;
    /** 资金帐户类别(冗余自资金账户) @see eOesAcctTypeT */
    uint8               cashType;
    /** 资金帐户状态(冗余自资金账户) @see eOesAcctStatusT */
    uint8               cashAcctStatus;
    /** 按64位对齐的填充域 */
    uint8               __filler1[5];

    /**
     * 总资产 (包含其它担保资产价值; 单位精确到元后四位, 即1元=10000)
     * - 公式: 总资产 = 现金余额(包含冻结资金在内的资金余额) + 担保证券市值(不包含买入在途,包含卖出在途和转出在途) + 其它担保资产价值
     */
    int64               totalAssetValue;
    /**
     * 总负债 (不包括在途负债; 单位精确到元后四位, 即1元=10000)
     * - 公式: 总负债 = 融资已买入金额 + 融券已卖出证券市值 + 利息及费用(不包含未成交部分的利息及费用) + 其它负债金额
     */
    int64               totalDebtValue;
    /** 维持担保比例 (千分比) */
    int32               maintenaceRatio;
    /** 按64位对齐的填充域 */
    int32               __filler2;
    /** 保证金可用余额 (单位精确到元后四位, 即1元=10000) */
    int64               marginAvailableBal;

    /** 现金余额 (包含融券卖出所得资金和冻结资金在内的总现金资产; 单位精确到元后四位, 即1元=10000) */
    int64               cashBalance;
    /** 可用资金 (现金还款/买融资标的可用资金; 单位精确到元后四位, 即1元=10000) */
    int64               availableBal;
    /** 可取资金 (单位精确到元后四位, 即1元=10000) */
    int64               drawableBal;
    /** 买担保品可用资金 (买非融资标的可用资金; 单位精确到元后四位, 即1元=10000) */
    int64               buyCollateralAvailableBal;
    /** 买券还券可用资金 (买券还券/买高流通性证券可用资金, 即:包含融券卖出所得资金在内的可用余额; 单位精确到元后四位, 即1元=10000) */
    int64               repayStockAvailableBal;
    /** 融券卖出所得总额 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellGainedAmt;
    /** 融券卖出所得可用金额 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellGainedAvailableAmt;
    /** 日中累计已用于归还负债的资金总额 (卖券还款或现金归还金额; 单位精确到元后四位, 即1元=10000) */
    int64               totalRepaidAmt;
    /** 日中为归还负债而在途冻结的资金总额 (卖券还款或现金归还冻结金额; 单位精确到元后四位, 即1元=10000) */
    int64               repayFrzAmt;

    /** 融资买入授信额度 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyMaxQuota;
    /** 融券卖出授信额度 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellMaxQuota;
    /** 融资融券总授信额度 (单位精确到元后四位, 即1元=10000) */
    int64               creditTotalMaxQuota;
    /**
     * 融资买入已用授信额度 (单位精确到元后四位, 即1元=10000)
     * - 公式: 融资买入已用授信额度 = 融资负债金额 + 融资负债交易费用 + 在途融资金额 + 在途融资交易费用 + 其他负债金额
     */
    int64               marginBuyUsedQuota;
    /** 融资买入可用授信额度 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyAvailableQuota;
    /**
     * 融券卖出已用授信额度 (单位精确到元后四位, 即1元=10000)
     * - 公式: 融券卖出已用授信额度 = 融券卖出金额 + 在途融券卖出金额
     */
    int64               shortSellUsedQuota;
    /** 融券卖出可用授信额度 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellAvailableQuota;

    /** 专项资金头寸金额 (含已用; 单位精确到元后四位, 即1元=10000) */
    int64               specialCashPositionAmt;
    /** 专项资金头寸可用余额 (单位精确到元后四位, 即1元=10000) */
    int64               specialCashPositionAvailableBal;
    /** 公共资金头寸金额 (含已用; 单位精确到元后四位, 即1元=10000) */
    int64               publicCashPositionAmt;
    /** 公共资金头寸可用余额 (单位精确到元后四位, 即1元=10000) */
    int64               publicCashPositionAvailableBal;

    /**
     * 证券持仓总市值 (日初持仓市值+累计买入持仓-累计卖出持仓; 单位精确到元后四位, 即1元=10000)
     * - 包括自有持仓和融资买入持仓
     * - 包含在途卖出冻结的持仓市值
     * - 包含转出冻结的证券持仓市值
     * - 包含直接还券冻结的持仓市值
     * - 不包含在途买入持仓市值
     * - 不包含在途担保品转入持仓市值
     */
    int64               collateralHoldingMarketCap;
    /** 在途卖出证券持仓市值 (单位精确到元后四位, 即1元=10000) */
    int64               collateralUncomeSellMarketCap;
    /** 转出冻结的证券持仓市值 (单位精确到元后四位, 即1元=10000)  */
    int64               collateralTrsfOutMarketCap;
    /** 直接还券冻结的证券持仓市值 (单位精确到元后四位, 即1元=10000) */
    int64               collateralRepayDirectMarketCap;

    /** 融资负债金额 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtAmt;
    /** 融资负债交易费用 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtFee;
    /** 融资负债利息 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtInterest;
    /** 在途融资金额 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyUncomeAmt;
    /** 在途融资交易费用 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyUncomeFee;
    /** 在途融资利息 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyUncomeInterest;
    /** 融资买入证券市值 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtMarketCap;
    /** 融资买入负债占用的保证金金额 (单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtUsedMargin;

    /** 融券卖出金额 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtAmt;
    /** 融券负债交易费用 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtFee;
    /** 融券负债利息 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtInterest;
    /** 在途融券卖出金额 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellUncomeAmt;
    /** 在途融券交易费用 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellUncomeFee;
    /** 在途融券利息 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellUncomeInterest;
    /** 融券卖出证券市值 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtMarketCap;
    /** 融券卖出负债占用的保证金金额 (单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtUsedMargin;

    /** 其他负债金额 (单位精确到元后四位, 即1元=10000) */
    int64               otherDebtAmt;
    /** 其他负债利息 (单位精确到元后四位, 即1元=10000) */
    int64               otherDebtInterest;
    /** 融资融券其他费用 (单位精确到元后四位, 即1元=10000) */
    int64               otherCreditFee;
    /** 融资融券专项头寸总费用 (包含融资专项头寸成本费、融券专项头寸成本费和转融通成本费; 单位精确到元后四位, 即1元=10000) */
    int64               creditTotalSpecialFee;
    /** 融资专项头寸成本费 (已包含在 '融资融券专项头寸总费用' 中; 单位精确到元后四位, 即1元=10000) */
    int64               marginBuySpecialFee;
    /** 融券专项头寸成本费 (已包含在 '融资融券专项头寸总费用' 中; 单位精确到元后四位, 即1元=10000) */
    int64               shortSellSpecialFee;
    /** 其它担保资产价值 (已包含在 '总资产' 中; 单位精确到元后四位, 即1元=10000) */
    int64               otherBackedAssetValue;

    /** 可转出资产价值 (单位精确到元后四位, 即1元=10000) */
    int64               trsfOutAbleAssetValue;
    /**
     * 标的证券市值 (融资标的证券的持仓市值; 单位精确到元后四位, 即1元=10000)
     *  - 不包含融资买入未归还部分的持仓
     *  - 不包含在途买入和在途转出持仓
     *  - 包含在途卖出持仓
     */
    int64               underlyingMarketCap;
    /** 修正资产价值 (已包含在 '总资产' 中; 单位精确到元后四位, 即1元=10000) */
    int64               correctAssetValue;
    /** 保留字段 */
    char                __reserve[8];
} OesCrdCreditAssetBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_CREDIT_ASSET_BASE_INFO                          \
        {0}, {0}, 0, 0, 0, {0}, \
        0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, {0}
/* -------------------------           */


/* ===================================================================
 * 融资融券可充抵保证金证券及融资融券标的基础信息定义
 * =================================================================== */

/**
 * 融资融券可充抵保证金证券及融资融券标的基础信息的结构体定义
 */
typedef struct _OesCrdUnderlyingBaseInfo {
    /** 客户代码 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 证券代码 */
    char                securityId[OES_SECURITY_ID_MAX_LEN];
    /** 市场代码 @see eOesMarketIdT */
    uint8               mktId;
    /** 产品类型 @see eOesProductTypeT */
    uint8               productType;
    /** 证券类型 @see eOesSecurityTypeT */
    uint8               securityType;
    /** 证券子类型 @see eOesSubSecurityTypeT */
    uint8               subSecurityType;

    /** 是否为融资融券可充抵保证金证券 (0:不可充抵保证金, 1:可充抵保证金) */
    uint8               isCrdCollateral;
    /** 是否为融资标的 (0:不是融资标的, 1:是融资标的) */
    uint8               isCrdMarginTradeUnderlying;
    /** 是否为融券标的 (0:不是融券标的, 1:是融券标的) */
    uint8               isCrdShortSellUnderlying;
    /** 融资融券可充抵保证金证券的交易状态 (0:不可交易, 1:可交易) */
    uint8               isCrdCollateralTradable;
    /** 是否已为个人设置融资融券担保品参数 */
    uint8               isIndividualCollateral;
    /** 是否已为个人设置融资融券标的参数 */
    uint8               isIndividualUnderlying;
    /** 按64位对齐的填充域 */
    uint8               __filler1[6];

    /** 可充抵保证金折算率 (单位:万分比) */
    int32               collateralRatio;
    /** 融资买入保证金比例 (单位:万分比) */
    int32               marginBuyRatio;
    /** 融券卖出保证金比例 (单位:万分比) */
    int32               shortSellRatio;
    /** 按64位对齐的填充域 */
    int32               __filler2[3];
} OesCrdUnderlyingBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_UNDERLYING_BASE_INFO                            \
        {0}, {0}, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, {0}, \
        0, 0, 0, {0}
/* -------------------------           */


/* ===================================================================
 * 融资融券资金头寸(可融资头寸)基础信息定义
 * =================================================================== */

/**
 * 融资融券资金头寸(可融资头寸)的基础信息内容定义
 */
#define __OES_CRD_CASH_POSITION_BASE_INFO_PKT                           \
        /** 资金账户代码 */ \
        char            cashAcctId[OES_CASH_ACCT_ID_MAX_LEN]; \
        /** 头寸编号 */ \
        int32           cashGroupNo; \
        /** 头寸性质 @see eOesCrdCashGroupPropertyT */ \
        uint8           cashGroupProperty; \
        /** 币种 @see eOesCurrTypeT */ \
        uint8           currType; \
        /** 按64位对齐的填充域 */ \
        uint8           __CRD_CASH_POSITION_BASE_filler[2]; \
        \
        /** 资金头寸金额 (含已用) */ \
        int64           positionAmt; \
        /** 日间已归还金额 */ \
        int64           repaidPositionAmt; \
        /** 累计已用金额 (含日初已用) */ \
        int64           usedPositionAmt; \
        /** 当前尚未成交的在途冻结金额 */ \
        int64           frzPositionAmt; \
        \
        /** 期初余额 (单位精确到元后四位, 即1元=10000) */ \
        int64           originalBalance; \
        /** 期初可用余额 (单位精确到元后四位, 即1元=10000) */ \
        int64           originalAvailable; \
        /** 期初已用金额 (期初待归还负债金额; 单位精确到元后四位, 即1元=10000) */ \
        int64           originalUsed; \
        \
        /** 预留的备用字段 */ \
        char            __CRD_CASH_POSITION_BASE_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_CRD_CASH_POSITION_BASE_INFO_PKT                   \
        {0}, 0, 0, 0, {0}, \
        0, 0, 0, 0, \
        0, 0, 0, \
        {0}
/* -------------------------           */


/**
 * 融资融券资金头寸的基础信息结构体定义
 */
typedef struct _OesCrdCashPositionBaseInfo {
    __OES_CRD_CASH_POSITION_BASE_INFO_PKT;
} OesCrdCashPositionBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_CASH_POSITION_BASE_INFO                         \
        __NULLOBJ_OES_CRD_CASH_POSITION_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 融资融券证券头寸(可融券头寸)基础信息定义
 * =================================================================== */

/**
 * 融资融券证券头寸(可融券头寸)的基础信息内容定义
 */
#define __OES_CRD_SECURITY_POSITION_BASE_INFO_PKT                       \
        /** 证券账户 */ \
        char            invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 证券代码 */ \
        char            securityId[OES_SECURITY_ID_MAX_LEN]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8           mktId; \
        /** 头寸性质 @see eOesCrdCashGroupPropertyT */ \
        uint8           cashGroupProperty; \
        /** 按64位对齐的填充域 */ \
        uint8           __SECURITY_POSITION_BASE_filler[2]; \
        /** 头寸编号 */ \
        int32           cashGroupNo; \
        \
        /** 证券头寸数量 (含已用) */ \
        int64           positionQty; \
        /** 日间已归还数量 (当日归还不可用) */ \
        int64           repaidPositionQty; \
        /** 累计已用数量 (含日初已用) */ \
        int64           usedPositionQty; \
        /** 当前尚未成交的在途冻结数量 */ \
        int64           frzPositionQty; \
        \
        /** 期初数量 */ \
        int64           originalBalanceQty; \
        /** 期初可用数量 */ \
        int64           originalAvailableQty; \
        /** 期初已用数量 (期初待归还负债数量) */ \
        int64           originalUsedQty; \
        \
        /** 预留的备用字段 */ \
        char            __SECURITY_POSITION_BASE_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_CRD_SECURITY_POSITION_BASE_INFO_PKT               \
        {0}, {0}, 0, 0, {0}, 0, \
        0, 0, 0, 0, \
        0, 0, 0, \
        {0}
/* -------------------------           */


/**
 * 融资融券证券头寸的基础信息结构体定义
 */
typedef struct _OesCrdSecurityPositionBaseInfo {
    __OES_CRD_SECURITY_POSITION_BASE_INFO_PKT;
} OesCrdSecurityPositionBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_SECURITY_POSITION_BASE_INFO                     \
        __NULLOBJ_OES_CRD_SECURITY_POSITION_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 融资融券合约(负债)基础信息定义
 * =================================================================== */

/**
 * 融资融券合约的基础信息内容定义
 */
#define __OES_CRD_DEBT_CONTRACT_BASE_INFO_PKT                           \
        /** 合约编号 */ \
        char            debtId[OES_CREDIT_DEBT_ID_MAX_LEN]; \
        /** 资金账户代码 */ \
        char            cashAcctId[OES_CASH_ACCT_ID_MAX_LEN]; \
        /** 股东账户代码 */ \
        char            invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 证券代码 */ \
        char            securityId[OES_SECURITY_ID_MAX_LEN]; \
        \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8           mktId; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8           securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8           subSecurityType; \
        /** 证券的产品类型 @see eOesProductTypeT */ \
        uint8           securityProductType; \
        \
        /** 负债类型 @see eOesCrdDebtTypeT */ \
        uint8           debtType; \
        /** 负债状态 @see eOesCrdDebtStatusT */ \
        uint8           debtStatus; \
        /** 期初负债状态 @see eOesCrdDebtStatusT */ \
        uint8           originalDebtStatus; \
        /** 负债归还模式 @see eOesCrdDebtRepayModeT */ \
        uint8           debtRepayMode; \
        \
        /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */ \
        int32           ordDate; \
        /** 委托价格 (单位精确到元后四位, 即1元=10000) */ \
        int32           ordPrice; \
        /** 委托数量 */ \
        int32           ordQty; \
        /** 成交数量 */ \
        int32           trdQty; \
        \
        /** 委托金额 (单位精确到元后四位, 即1元=10000) */ \
        int64           ordAmt; \
        /** 成交金额 (单位精确到元后四位, 即1元=10000) */ \
        int64           trdAmt; \
        /** 成交费用 (仅用于展示, 负债部分参见合约手续费(currentDebtFee)字段. 单位精确到元后四位, 即1元=10000) */ \
        int64           trdFee; \
        \
        /** 实时合约金额 (单位精确到元后四位, 即1元=10000) */ \
        int64           currentDebtAmt; \
        /** 实时合约手续费 (单位精确到元后四位, 即1元=10000) */ \
        int64           currentDebtFee; \
        /** 实时合约利息 (含罚息. 单位精确到元后四位, 即1元=10000) */ \
        int64           currentDebtInterest; \
        /** 实时合约数量 */ \
        int32           currentDebtQty; \
        \
        /** 在途冻结数量 */ \
        int32           uncomeDebtQty; \
        /** 在途冻结金额 (单位精确到元后四位, 即1元=10000) */ \
        int64           uncomeDebtAmt; \
        /** 在途冻结手续费 (单位精确到元后四位, 即1元=10000) */ \
        int64           uncomeDebtFee; \
        /** 在途冻结利息 (单位精确到元后四位, 即1元=10000) */ \
        int64           uncomeDebtInterest; \
        \
        /**
         * 累计已归还金额 (单位精确到元后四位, 即1元=10000)
         * - 对于融资，是归还的融资负债金额
         * - 对于融券，是归还的融券数量*归还时的成交价格 (即实际归还金额)
         */ \
        int64           totalRepaidAmt; \
        /** 累计已归还手续费 (仅包含当日归还. 单位精确到元后四位, 即1元=10000) */ \
        int64           totalRepaidFee; \
        /** 累计已归还利息 (单位精确到元后四位, 即1元=10000) */ \
        int64           totalRepaidInterest; \
        /**
         * 累计已归还数量
         * - 对于融券，是归还的融券负债数量
         * - 对于融资，是归还的融资金额/归还时该证券最新价格
         */ \
        int32           totalRepaidQty; \
        /** 按64位对齐的填充域 */ \
        int32           __CRD_DEBT_CONTRACT_BASE_filler2; \
        \
        /** 期初待归还金额 (单位精确到元后四位, 即1元=10000) */ \
        int64           originalDebtAmt; \
        /** 期初待归还手续费 (单位精确到元后四位, 即1元=10000) */ \
        int64           originalDebtFee; \
        /** 期初待归还利息 (含罚息. 单位精确到元后四位, 即1元=10000) */ \
        int64           originalDebtInterest; \
        /** 期初待归还数量 */ \
        int32           originalDebtQty; \
        \
        /**
         * 期初已归还数量
         * - 对于融券，是归还的融券负债数量
         * - 对于融资，是归还的融资金额/归还时该证券最新价格
         */ \
        int32           originalRepaidQty; \
        /**
         * 期初已归还金额 (单位精确到元后四位, 即1元=10000)
         * - 对于融资，是归还的融资负债金额
         * - 对于融券，是归还的融券数量*归还时成交价格
         */ \
        int64           originalRepaidAmt; \
        /** 期初已归还利息 (含罚息. 单位精确到元后四位, 即1元=10000) */ \
        int64           originalRepaidInterest; \
        \
        /** 罚息 (仅供展示, 已在利息中体现. 单位精确到元后四位, 即1元=10000) */ \
        int64           punishInterest; \
        \
        /** 保证金比例 (单位:万分比) */ \
        int32           marginRatio; \
        /** 融资利率/融券费率 (单位精确到万分之一, 即费率8.36% = 836) */ \
        int32           interestRate; \
        /** 负债截止日期 (格式为 YYYYMMDD, 形如 20160830) */ \
        int32           repayEndDate; \
        /** 头寸编号 */ \
        int32           cashGroupNo; \
        /** 展期次数 */ \
        int32           postponeTimes; \
        /** 展期状态 @see eOesCrdDebtPostponeStatusT */ \
        uint8           postponeStatus; \
        /** 按64位对齐的填充域 */ \
        uint8           __CRD_DEBT_CONTRACT_BASE_filler3[3]; \
        \
        /** 预留的备用字段 */ \
        char            __CREDIT_DEBT_BASE_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_CRD_DEBT_CONTRACT_BASE_INFO_PKT                   \
        {0}, {0}, {0}, {0}, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, \
        0, \
        0, 0, 0, 0, 0, 0, {0}, \
        {0}
/* -------------------------           */


/**
 * 融资融券合约的基础信息结构体定义
 */
typedef struct _OesCrdDebtContractBaseInfo {
    __OES_CRD_DEBT_CONTRACT_BASE_INFO_PKT;
} OesCrdDebtContractBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_DEBT_CONTRACT_BASE_INFO                         \
        __NULLOBJ_OES_CRD_DEBT_CONTRACT_BASE_INFO_PKT
/* -------------------------           */


/**
 * 融资融券合约回报信息结构体定义
 */
typedef struct _OesCrdDebtContractReport {
    __OES_CRD_DEBT_CONTRACT_BASE_INFO_PKT;

    union {
        /**
         * 同一证券所有融券合约的合计可归还负债数量
         * - 公式: 同一证券合计可归还负债数量 = 日初融券负债数量 - 当日已归还融券数量 - 在途归还融券数量
         */
        int64           securityRepayableDebtQty;
        /** 同一证券所有融券合约的合计可归还负债数量 @deprecated 已废弃, 为了兼容旧版本而暂时保留 */
        int64           securityRepayableQty;
    };
    union {
        /**
         * 该融券合约的当前可归还负债数量
         * - 公式: 合约当前可归还负债数量 = 实时合约数量 - 归还指定合约的在途归还数量
         * - @note 实际允许归还的负债数量, 为该融券合约可归还负债数量与对应证券可归还负债数量的较小者
         */
        int32           contractRepayableDebtQty;
        /** 该融券合约的当前可归还负债数量 @deprecated 已废弃, 为了兼容旧版本而暂时保留 */
        int32           contractRepayableQty;
    };
    /** 按64位对齐的填充域 */
    int32               __filler;

    /** 保留字段 */
    char                __reserve[32];
} OesCrdDebtContractReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_DEBT_CONTRACT_REPORT                            \
        __NULLOBJ_OES_CRD_DEBT_CONTRACT_BASE_INFO_PKT, \
        {0}, {0}, 0, \
        {0}
/* -------------------------           */


/* ===================================================================
 * 融资融券合约流水信息定义
 * =================================================================== */

/**
 * 融资融券合约负债流水的基础信息内容定义
 * @note    记录合约负债变动流水, 包括: 开仓流水、归还流水、以及合约展期流水
 *          - 对于归还流水, 发生金额、发生费用、发生利息、发生证券数量为负数
 *          - 对于开仓流水, 对应数据为正数
 *          - 对于合约展期流水, 发生金额、发生费用等数据为0
 */
#define __OES_CRD_DEBT_JOURNAL_BASE_INFO_PKT                            \
        /** 合约编号 */ \
        char            debtId[OES_CREDIT_DEBT_ID_MAX_LEN]; \
        /** 资金账户代码 */ \
        char            cashAcctId[OES_CASH_ACCT_ID_MAX_LEN]; \
        /** 股东账户代码 */ \
        char            invAcctId[OES_INV_ACCT_ID_MAX_LEN]; \
        /** 证券代码 */ \
        char            securityId[OES_SECURITY_ID_MAX_LEN]; \
        \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8           mktId; \
        /** 负债类型 @see eOesCrdDebtTypeT */ \
        uint8           debtType; \
        /** 流水类型 @see eOesCrdDebtJournalTypeT */ \
        uint8           journalType; \
        /** 强制标志 @see eOesOrdMandatoryFlagT */ \
        uint8           mandatoryFlag; \
        /** 同一融资融券合约的负债流水的顺序号 */ \
        int32           seqNo; \
        \
        /** 发生金额 (不含息费; 单位精确到元后四位, 即1元=10000) */ \
        int64           occurAmt; \
        /** 发生费用 (单位精确到元后四位, 即1元=10000) */ \
        int64           occurFee; \
        /** 发生利息 (单位精确到元后四位, 即1元=10000) */ \
        int64           occurInterest; \
        /** 发生证券数量 */ \
        int32           occurQty; \
        /** 后余证券数量 */ \
        int32           postQty; \
        /** 后余金额 (不含息费; 单位精确到元后四位, 即1元=10000) */ \
        int64           postAmt; \
        /** 后余费用 (单位精确到元后四位, 即1元=10000) */ \
        int64           postFee; \
        /** 后余利息 (单位精确到元后四位, 即1元=10000) */ \
        int64           postInterest; \
        \
        /**
         * 融券合约流水的理论发生金额 (单位精确到元后四位, 即1元=10000)
         * - 开仓流水中等同于发生金额, 即成交金额
         * - 归还流水中为对应于合约开仓价格的理论上的发生金额
         */ \
        int64           shortSellTheoryOccurAmt; \
        /** 归还息费时使用融券卖出所得抵扣的金额 (单位精确到元后四位, 即1元=10000) */ \
        int64           useShortSellGainedAmt; \
        \
        /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */ \
        int32           ordDate; \
        /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */ \
        int32           ordTime; \
        /** 预留的备用字段 */ \
        char            __CRD_DEBT_JOURNAL_BASE_reserve[32]


/* 结构体的初始化值定义 */
#define __NULLOBJ_OES_CRD_DEBT_JOURNAL_BASE_INFO_PKT                    \
        {0}, {0}, {0}, {0}, \
        0, 0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, \
        0, 0, {0}
/* -------------------------           */


/**
 * 融资融券合约负债流水的基础信息结构体定义
 */
typedef struct _OesCrdDebtJournalBaseInfo {
    __OES_CRD_DEBT_JOURNAL_BASE_INFO_PKT;
} OesCrdDebtJournalBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_DEBT_JOURNAL_BASE_INFO                          \
        __NULLOBJ_OES_CRD_DEBT_JOURNAL_BASE_INFO_PKT
/* -------------------------           */


/**
 * 融资融券合约流水变动信息回报结构体定义
 */
typedef struct _OesCrdDebtJournalReport {
    __OES_CRD_DEBT_JOURNAL_BASE_INFO_PKT;
} OesCrdDebtJournalReportT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_DEBT_JOURNAL_REPORT                             \
        __NULLOBJ_OES_CRD_DEBT_JOURNAL_BASE_INFO_PKT
/* -------------------------           */


/* ===================================================================
 * 客户单证券融资融券负债统计基础信息定义
 * =================================================================== */

/**
 * 客户单证券融资融券负债统计基础信息的结构体定义
 */
typedef struct _OesCrdSecurityDebtStatsBaseInfo {
    /** 股东账户代码 */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 证券代码 */
    char                securityId[OES_SECURITY_ID_MAX_LEN];
    /** 市场代码 @see eOesMarketIdT */
    uint8               mktId;
    /** 产品类型 @see eOesProductTypeT */
    uint8               productType;
    /** 证券类型 @see eOesSecurityTypeT */
    uint8               securityType;
    /** 证券子类型 @see eOesSubSecurityTypeT */
    uint8               subSecurityType;

    /** 是否为融资融券可充抵保证金证券 (0:不可充抵保证金, 1:可充抵保证金) */
    uint8               isCrdCollateral;
    /** 是否为融资标的 (0:不是融资标的, 1:是融资标的) */
    uint8               isCrdMarginTradeUnderlying;
    /** 是否为融券标的 (0:不是融券标的, 1:是融券标的) */
    uint8               isCrdShortSellUnderlying;
    /** 融资融券可充抵保证金证券的交易状态 (0:不可交易, 1:可交易) */
    uint8               isCrdCollateralTradable;
    /** 可充抵保证金折算率 (单位:万分比) */
    int32               collateralRatio;
    /** 融资买入保证金比例 (单位:万分比) */
    int32               marginBuyRatio;
    /** 融券卖出保证金比例 (单位:万分比) */
    int32               shortSellRatio;
    /** 市值计算使用的证券价格 (等价于最新价或公允价, 取决于是否开启公允价格; 价格单位精确到元后四位, 即1元=10000) */
    int32               marketCapPrice;

    /** 可卖持仓数量 */
    int64               sellAvlHld;
    /** 可划出持仓数量 (可充抵保证金证券(担保品)可划出数量) */
    int64               trsfOutAvlHld;
    /** 直接还券可用持仓数量 */
    int64               repayStockDirectAvlHld;
    union {
        /**
         * 同一证券所有融券合约的合计待归还负债数量
         * - 公式: 同一证券合计待归还负债数量 = 日初融券负债数量 - 当日已归还融券数量 - 在途归还融券数量
         */
        int64           shortSellRepayableDebtQty;
        /** 同一证券所有融券合约的合计待归还负债数量 @deprecated 已废弃, 为了兼容旧版本而暂时保留 */
        int64           securityRepayableQty;
    };

    /** 专项证券头寸数量 (含已用) */
    int64               specialSecurityPositionQty;
    /** 专项证券头寸已用数量 (含尚未成交的在途冻结数量) */
    int64               specialSecurityPositionUsedQty;
    /**
     * 专项证券头寸可用数量
     * - 当可用头寸低于有效数量下限(卖委托数量下限)时该字段将返回0
     * - @note 实际剩余的未使用头寸数量可通过如下公式计算:
     *   实际剩余的可用头寸数量 = 专项证券头寸数量 - 专项证券头寸已用数量
     */
    int64               specialSecurityPositionAvailableQty;
    /** 公共证券头寸数量 (含已用) */
    int64               publicSecurityPositionQty;
    /** 公共证券头寸可用数量 */
    int64               publicSecurityPositionAvailableQty;

    /*
     * 担保证券持仓统计信息
     */
    /**
     * 总持仓数量 (日初持仓数量+累计买入数量-累计卖出数量)
     * - 包括自有持仓和融资买入持仓
     * - 包含在途卖出冻结的持仓数量
     * - 包含转出冻结的持仓数量
     * - 包含直接还券冻结的持仓数量
     * - 不包含在途买入数量
     * - 不包含在途担保品转入持仓数量
     */
    int64               collateralHoldingQty;
    /** 在途买入数量 (不包括融资买入在途数量) */
    int64               collateralUncomeBuyQty;
    /** 在途转入持仓数量 (包含已确认和未确认数量, 不包含已撤单数量) */
    int64               collateralUncomeTrsfInQty;

    /** 在途卖出冻结的持仓数量 */
    int64               collateralUncomeSellQty;
    union {
        /** 转出冻结的持仓数量 (包含已确认和未确认数量, 不包含已撤单数量) */
        int64           collateralTrsfOutQty;
        /** 转出冻结的持仓数量 @deprecated 已废弃, 为了兼容旧版本而暂时保留 */
        int64           collateralTotalTrsfOutQty;
    };
    union {
        /** 直接还券冻结的持仓数量 (包含已确认和未确认数量, 不包含已撤单/废单数量) */
        int64           collateralRepayDirectQty;
        /** 直接还券冻结的持仓数量 @deprecated 已废弃, 为了兼容旧版本而暂时保留 */
        int64           collateralTotalRepayDirectQty;
    };

    /*
     * 融资买入负债统计信息
     */
    /** 融资负债金额 (不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtAmt;
    /** 融资交易费用 (不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtFee;
    /** 融资负债利息 (包括罚息, 不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               marginBuyDebtInterest;
    /** 融资负债数量 (不包括已还) */
    int64               marginBuyDebtQty;

    /** 在途融资买入金额, 单位精确到元后四位, 即1元 = 10000 */
    int64               marginBuyUncomeAmt;
    /** 在途融资交易费用, 单位精确到元后四位, 即1元 = 10000 */
    int64               marginBuyUncomeFee;
    /** 在途融资利息, 单位精确到元后四位, 即1元 = 10000 */
    int64               marginBuyUncomeInterest;
    /** 在途融资数量 */
    int64               marginBuyUncomeQty;

    /** 日初融资负债金额 (日初融资余额, 不包括日初已还) */
    int64               marginBuyOriginDebtAmt;
    /** 日初融资负债数量 (不包括日初已还) */
    int64               marginBuyOriginDebtQty;
    /** 当日已归还融资金额 (日中发生的归还金额, 不包括日初已还) */
    int64               marginBuyRepaidAmt;
    /** 当日已归还融资数量 (对应于合约开仓价格的理论上的已归还融资数量) */
    int64               marginBuyRepaidQty;

    /*
     * 融券卖出负债统计信息
     */
    /** 融券负债金额 (不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtAmt;
    /** 融券交易费用 (不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtFee;
    /** 融券负债利息 (包括罚息, 不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               shortSellDebtInterest;
    /** 融券负债数量 (不包括已还) */
    int64               shortSellDebtQty;

    /** 在途融券卖出金额, 单位精确到元后四位, 即1元 = 10000 */
    int64               shortSellUncomeAmt;
    /** 在途融券交易费用, 单位精确到元后四位, 即1元 = 10000 */
    int64               shortSellUncomeFee;
    /** 在途融券利息, 单位精确到元后四位, 即1元 = 10000 */
    int64               shortSellUncomeInterest;
    /** 在途融券数量 */
    int64               shortSellUncomeQty;

    /** 日初融券负债数量 (日初融券余量, 不包括日初已还) */
    int64               shortSellOriginDebtQty;
    /** 当日已归还融券数量 (日中发生的归还数量, 不包括日初已还) */
    int64               shortSellRepaidQty;
    /** 在途归还融券数量 */
    int64               shortSellUncomeRepaidQty;
    /** 当日已归还融券金额 (按开仓价格计算的理论上已归还融券金额; 单位精确到元后四位, 即1元=10000) */
    int64               shortSellRepaidAmt;
    /** 当日实际归还融券金额 (按成交价计算的当日实际归还融券金额; 单位精确到元后四位, 即1元=10000) */
    int64               shortSellRealRepaidAmt;

    /*
     * '其它负债' 统计信息
     */
    /** '其它负债'的负债金额 (不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               otherDebtAmt;
    /** '其它负债'利息 (包括罚息, 不包括已还; 单位精确到元后四位, 即1元=10000) */
    int64               otherDebtInterest;

    /** 保留字段 */
    char                __reserve[32];
} OesCrdSecurityDebtStatsBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_SECURITY_DEBT_STATS_BASE_INFO                   \
        {0}, {0}, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, {0}, \
        0, 0, 0, 0, 0, \
        0, 0, 0, \
        0, {0}, {0}, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, \
        0, 0, 0, 0, 0, \
        0, 0, \
        {0}
/* -------------------------           */


/* ===================================================================
 * 余券基础信息定义
 * =================================================================== */

/**
 * 余券基础信息的结构体定义
 */
typedef struct _OesCrdExcessStockBaseInfo {
    /** 客户代码 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 证券账户代码 */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 证券代码 */
    char                securityId[OES_SECURITY_ID_MAX_LEN];
    /** 市场代码  @see eOesMarketIdT */
    uint8               mktId;
    /** 按64位对齐的填充域 */
    uint8               __filler[7];

    /** 日初余券数量 */
    int64               originExcessStockQty;
    /** 余券数量 (日初余券数量 + 日中余券数量) */
    int64               excessStockTotalQty;
    /** 余券已划转数量 (包含已确认和未确认数量, 不包含已撤单数量) */
    int64               excessStockUncomeTrsfQty;
    /** 余券可划转数量 */
    int64               excessStockTrsfAbleQty;

    /** 保留字段 */
    char                __reserve[32];
} OesCrdExcessStockBaseInfoT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_CRD_EXCESS_STOCK_BASE_INFO                          \
        {0}, {0}, {0}, 0, {0}, \
        0, 0, 0, 0, \
        {0}
/* -------------------------           */


#ifdef __cplusplus
}
#endif

#endif  /* _OES_BASE_MODEL_CREDIT_H */
