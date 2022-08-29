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
 * @file    oes_qry_packets_option.h
 *
 * 期权业务查询消息的报文定义
 *
 * @version 0.16        2019/01/18
 *          - 新增 '期权限仓额度查询' 相关报文定义
 *              - 新增结构体 查询期权限仓额度消息过滤条件(OesQryOptPositionLimitFilterT)
 *              - 新增结构体 期权限仓额度信息内容(OesOptPositionLimitItemT)
 *              - 新增结构体 查询期权限仓额度消息请求(OesQryOptPositionLimitReqT)
 *              - 新增结构体 查询期权限仓额度消息应答(OesQryOptPositionLimitRspT)
 *          - 新增 '期权限购额度信息查询' 相关报文定义
 *              - 新增结构体 查询期权限购额度信息过滤条件(OesQryOptPurchaseLimitFilterT)
 *              - 新增结构体 期权限购额度信息内容(OesOptPurchaseLimitItemT)
 *              - 新增结构体 查询期权限购额度信息请求(OesQryOptPurchaseLimitReqT)
 *              - 新增结构体 查询期权限购额度信息应答(OesQryOptPurchaseLimitRspT)
 *          - 新增 '期权行权指派查询' 相关报文定义
 *              - 新增结构体 查询期权行权指派消息过滤条件(OesQryOptExerciseAssignFilterT)
 *              - 新增结构体 期权行权指派信息内容(OesOptxerciseAssignItemT)
 *              - 新增结构体 查询期权行权指派消息请求(OesQryOptExerciseAssignReqT)
 *              - 新增结构体 查询期权行权指派消息应答(OesQryOptExerciseAssignRspT)
 * @version 0.16.0.2    2020/01/03
 *          - '期权行权指派信息 (OesOptExerciseAssignItemT)'中
 *              - 新增 期权合约名称 (securityName) 字段
 * @version 0.17.1.1    2021/06/28
 *          - 提取单独的期权业务查询消息定义文件
 *
 * @since   0.17.1.1    2021/06/28
 */


#ifndef _OES_QRY_PACKETS_OPTION_H
#define _OES_QRY_PACKETS_OPTION_H


#include    <oes_global/oes_base_model.h>
#include    <oes_global/oes_base_model_option.h>
#include    <oes_global/oes_qry_packets.h>
#include    <sutil/net/spk_global_packet.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ===================================================================
 * 常量定义
 * =================================================================== */

/** 每条查询应答报文中期权持仓信息的最大数量 */
#define OES_MAX_OPT_HOLDING_ITEM_CNT_PER_PACK               (30)

/** 每条查询应答报文中期权产品的最大数量 */
#define OES_MAX_OPT_OPTION_ITEM_CNT_PER_PACK                (30)

/** 每条查询应答报文中期权标的信息的最大数量 */
#define OES_MAX_OPT_UNDERLYING_ITEM_CNT_PER_PACK            (30)

/** 每条查询应答报文中期权限购额度信息的最大数量 */
#define OES_MAX_OPT_PURCHASE_LIMIT_ITEM_CNT_PER_PACK        (30)

/** 每条查询应答报文中期权行权指派信息的最大数量 */
#define OES_MAX_OPT_EXERCISE_ASSIGN_ITEM_CNT_PER_PACK       (30)

/** 每条查询应答报文中期权产品的最大数量 @deprecated 已过时, 请使用 OES_MAX_OPT_OPTION_ITEM_CNT_PER_PACK */
#define OES_MAX_OPTION_ITEM_CNT_PER_PACK                                \
        OES_MAX_OPT_OPTION_ITEM_CNT_PER_PACK

/** 每条查询应答报文中期权行权指派信息的最大数量 @deprecated 已过时, 请使用 OES_MAX_OPT_EXERCISE_ASSIGN_ITEM_CNT_PER_PACK */
#define OES_MAX_EXERCISE_ASSIGN_ITEM_CNT_PER_PACK                       \
        OES_MAX_OPT_EXERCISE_ASSIGN_ITEM_CNT_PER_PACK
/* -------------------------           */


/* ===================================================================
 * 查询期权产品信息相关结构体定义
 * =================================================================== */

/**
 * 查询期权产品信息过滤条件
 */
typedef struct _OesQryOptionFilter {
    /** 证券代码, 可选项 */
    char                securityId[OES_SECURITY_ID_MAX_LEN];

    /**
     * 市场代码, 可选项。如无需此过滤条件请使用 OES_MKT_ID_UNDEFINE
     * @see eOesMarketIdT
     */
    uint8               mktId;
    /** 按64位对齐的填充域 */
    uint8               __filler[7];

    /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */
    int64               userInfo;
} OesQryOptionFilterT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPTION_FILTER                                   \
        {0}, 0, {0}, 0
/* -------------------------           */


/**
 * 期权产品信息内容
 */
typedef OesOptionBaseInfoT          OesOptionItemT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPTION_ITEM                                         \
        NULLOBJ_OES_OPTION_BASE_INFO
/* -------------------------           */


/**
 * 查询期权产品信息请求
 */
typedef struct _OesQryOptionReq {
    /** 查询请求消息头 */
    OesQryReqHeadT      reqHead;
    /** 查询过滤条件 */
    OesQryOptionFilterT qryFilter;
} OesQryOptionReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPTION_REQ                                      \
        {NULLOBJ_OES_QRY_REQ_HEAD}, \
        {NULLOBJ_OES_QRY_OPTION_FILTER}
/* -------------------------           */


/**
 * 查询期权产品信息应答
 */
typedef struct _OesQryOptionRsp {
    /** 查询应答消息头 */
    OesQryRspHeadT      rspHead;
    /** 期权产品信息数组 */
    OesOptionItemT      qryItems[OES_MAX_OPT_OPTION_ITEM_CNT_PER_PACK];
} OesQryOptionRspT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPTION_RSP                                      \
        {NULLOBJ_OES_QRY_RSP_HEAD}, \
        {{NULLOBJ_OES_OPTION_ITEM}}
/* -------------------------           */


/* ===================================================================
 * 查询期权持仓信息相关结构体定义
 * =================================================================== */

/**
 * 查询期权持仓信息过滤条件
 */
typedef struct _OesQryOptHoldingFilter {
    /** 客户代码, 可选项 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 证券账户代码, 可选项 */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 证券代码, 可选项 */
    char                securityId[OES_SECURITY_ID_MAX_LEN];

    /** 市场代码 @see eOesMarketIdT */
    uint8               mktId;
    /** 持仓类型 @see eOesOptPositionTypeT */
    uint8               positionType;
    /** 按64位对齐的填充域 */
    uint8               __filler[6];

    /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */
    int64               userInfo;
} OesQryOptHoldingFilterT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_HOLDING_FILTER                              \
        {0}, {0}, {0}, \
        0, 0, {0}, 0
/* -------------------------           */


/**
 * 查询到的期权持仓信息内容
 */
typedef struct _OesOptHoldingItem {
    __OES_OPT_HOLDING_BASE_INFO_PKT;
    __OES_OPT_HOLDING_RPT_INFO_PKT;

    /** 交易所合约代码 */
    char                contractId[OES_CONTRACT_EXCH_ID_MAX_LEN];
    /** 期权合约简称 */
    char                contractSymbol[OES_SECURITY_NAME_MAX_LEN];
    /** 昨结算价 */
    int64               prevSettlPrice;
} OesOptHoldingItemT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_HOLDING_ITEM                                    \
        __NULLOBJ_OES_OPT_HOLDING_BASE_INFO_PKT, \
        __NULLOBJ_OES_OPT_HOLDING_RPT_INFO_PKT, \
        {0}, {0}, 0
/* -------------------------           */


/**
 * 查询期权持仓信息请求
 */
typedef struct _OesQryOptHoldingReq {
    /** 查询请求消息头 */
    OesQryReqHeadT      reqHead;
    /** 查询过滤条件 */
    OesQryOptHoldingFilterT
                        qryFilter;
} OesQryOptHoldingReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_HOLDING_REQ                                 \
        {NULLOBJ_OES_QRY_REQ_HEAD}, \
        {NULLOBJ_OES_QRY_OPT_HOLDING_FILTER}
/* -------------------------           */


/**
 * 查询期权持仓信息应答
 */
typedef struct _OesQryOptHoldingRsp {
    /** 查询应答消息头 */
    OesQryRspHeadT      rspHead;
    /** 持仓信息数组 */
    OesOptHoldingItemT  qryItems[OES_MAX_OPT_HOLDING_ITEM_CNT_PER_PACK];
} OesQryOptHoldingRspT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_HOLDING_RSP                                 \
        {NULLOBJ_OES_QRY_RSP_HEAD}, \
        {{NULLOBJ_OES_OPT_HOLDING_ITEM}}
/* -------------------------           */


/* ===================================================================
 * 查询期权标的持仓信息相关结构体定义
 * =================================================================== */

/**
 * 查询期权标的持仓信息过滤条件
 */
typedef struct _OesQryOptUnderlyingHoldingFilter {
    /** 客户代码, 可选项 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 证券账户代码, 可选项 */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 标的证券代码, 可选项 */
    char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN];

    /** 市场代码  @see eOesMarketIdT */
    uint8               mktId;
    /** 证券类别  @see eOesSecurityTypeT */
    uint8               underlyingSecurityType;
    /** 按64位对齐的填充域 */
    uint8               __filler[6];

    /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */
    int64               userInfo;
} OesQryOptUnderlyingHoldingFilterT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_UNDERLYING_HOLDING_FILTER                   \
        {0}, {0}, {0}, \
        0, 0, {0}, 0
/* -------------------------           */


/**
 * 查询到的期权标的持仓信息内容
 */
typedef OesOptUnderlyingHoldingBaseInfoT                                \
        OesOptUnderlyingHoldingItemT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_UNDERLYING_HOLDING_ITEM                         \
        NULLOBJ_OES_OPT_UNDERLYING_HOLDING_BASE_INFO
/* -------------------------           */


/**
 * 查询期权标的持仓信息请求
 */
typedef struct _OesQryOptUnderlyingHoldingReq {
    /** 查询请求消息头 */
    OesQryReqHeadT      reqHead;
    /** 查询过滤条件 */
    OesQryOptUnderlyingHoldingFilterT
                        qryFilter;
} OesQryOptUnderlyingHoldingReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_UNDERLYING_HOLDING_REQ                      \
        {NULLOBJ_OES_QRY_REQ_HEAD}, \
        {NULLOBJ_OES_QRY_OPT_UNDERLYING_HOLDING_FILTER}
/* -------------------------           */


/**
 * 查询期权标的持仓信息应答
 */
typedef struct _OesQryOptUnderlyingHoldingRsp {
    /** 查询应答消息头 */
    OesQryRspHeadT      rspHead;
    /** 期权标的持仓信息数组 */
    OesOptUnderlyingHoldingItemT
                        qryItems[OES_MAX_OPT_HOLDING_ITEM_CNT_PER_PACK];
} OesQryOptUnderlyingHoldingRspT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_UNDERLYING_HOLDING_RSP                      \
        {NULLOBJ_OES_QRY_RSP_HEAD}, \
        {{NULLOBJ_OES_OPT_UNDERLYING_HOLDING_ITEM}}
/* -------------------------           */


/* ===================================================================
 * 查询期权限仓额度信息相关结构体定义
 * =================================================================== */

/**
 * 查询期权限仓额度信息过滤条件
 */
typedef struct _OesQryOptPositionLimitFilter {
    /** 客户代码, 可选项 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 证券账户代码, 可选项 */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 标的证券代码, 可选项 */
    char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN];

    /** 市场代码  @see eOesMarketIdT */
    uint8               mktId;
    /** 证券类别  @see eOesSecurityTypeT */
    uint8               underlyingSecurityType;
    /** 按64位对齐的填充域 */
    uint8               __filler[6];

    /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */
    int64               userInfo;
} OesQryOptPositionLimitFilterT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_POSITION_LIMIT_FILTER                       \
        {0}, {0}, {0}, \
        0, 0, {0}, 0
/* -------------------------           */


/**
 * 查询到的期权限仓额度信息内容
 */
typedef struct _OesOptPositionLimitItem {
    /** 股东账户代码 (不带'888'编码的原始股东账户代码) */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 标的证券代码 */
    char                underlyingSecurityId[OES_SECURITY_ID_MAX_LEN];
    /** 市场代码 (衍生品市场) @see eOesMarketIdT */
    uint8               mktId;

    /** 标的市场代码 @see eOesMarketIdT */
    uint8               underlyingMktId;
    /** 标的证券类型 @see eOesSecurityTypeT */
    uint8               underlyingSecurityType;
    /** 标的证券子类型 @see eOesSubSecurityTypeT */
    uint8               underlyingSubSecurityType;
    /** 按64位对齐的填充域 */
    uint8               __filler1[4];

    /** 权利仓限额 */
    int32               longPositionLimit;
    /** 总持仓限额 */
    int32               totalPositionLimit;
    /** 单日买入开仓限额 */
    int32               dailyBuyOpenLimit;
    /** 按64位对齐的填充域 */
    int32               __filler2;

    /** 日初权利仓持仓数量 (单位: 张) */
    int32               originalLongQty;
    /** 日初义务仓持仓数量 (单位: 张) */
    int32               originalShortQty;
    /** 日初备兑义务仓持仓数量 (单位: 张) */
    int32               originalCoveredQty;

    /** 未占用的权利仓限额 */
    int32               availableLongPositionLimit;
    /** 未占用的总持仓限额 */
    int32               availableTotalPositionLimit;
    /** 未占用的单日买入开仓限额 */
    int32               availableDailyBuyOpenLimit;

    /** 预留的备用字段 */
    char                __reserve[8];
} OesOptPositionLimitItemT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_POSITION_LIMIT_ITEM                             \
        {0}, {0}, \
        0, 0, 0, 0, {0}, \
        0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, \
        {0}
/* -------------------------           */


/**
 * 查询期权限仓额度信息请求
 */
typedef struct _OesQryOptPositionLimitReq {
    /** 查询请求消息头 */
    OesQryReqHeadT      reqHead;
    /** 查询过滤条件 */
    OesQryOptPositionLimitFilterT
                        qryFilter;
} OesQryOptPositionLimitReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_POSITION_LIMIT_REQ                          \
        {NULLOBJ_OES_QRY_REQ_HEAD}, \
        {NULLOBJ_OES_QRY_OPT_POSITION_LIMIT_FILTER}
/* -------------------------           */


/**
 * 查询期权限仓额度信息应答
 */
typedef struct _OesQryOptPositionLimitRsp {
    /** 查询应答消息头 */
    OesQryRspHeadT      rspHead;
    /** 期权标的持仓信息数组 */
    OesOptPositionLimitItemT
                        qryItems[OES_MAX_OPT_UNDERLYING_ITEM_CNT_PER_PACK];
} OesQryOptPositionLimitRspT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_POSITION_LIMIT_RSP                          \
        {NULLOBJ_OES_QRY_RSP_HEAD}, \
        {{NULLOBJ_OES_OPT_POSITION_LIMIT_ITEM}}
/* -------------------------           */


/* ===================================================================
 * 查询限购消息相关结构体定义
 * =================================================================== */

/**
 * 查询期权限购额度信息过滤条件
 */
typedef struct _OesQryOptPurchaseLimitFilter {
    /** 客户代码, 可选项 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 证券账户代码 (不带'888'编码的原始股东账户代码), 可选项 */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];

    /**
     * 市场代码 (衍生品市场), 可选项。如无需此过滤条件请使用 OES_MKT_ID_UNDEFINE
     * @see eOesMarketIdT
     */
    uint8               mktId;
    /** 按64位对齐的填充域 */
    uint8               __filler[7];

    /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */
    int64               userInfo;
} OesQryOptPurchaseLimitFilterT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_PURCHASE_LIMIT_FILTER                       \
        {0}, {0}, 0, {0}, \
        0
/* -------------------------           */


/**
 * 期权限购额度信息内容
 */
typedef struct _OesOptPurchaseLimitItem {
    /** 客户代码 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 资金账户代码 */
    char                cashAcctId[OES_CASH_ACCT_ID_MAX_LEN];
    /** 股东账户代码 (不带'888'编码的原始股东账户代码) */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 市场代码 (衍生品市场) @see eOesMarketIdT */
    uint8               mktId;
    /** 客户类别 @see eOesCustTypeT */
    uint8               custType;
    /** 按64位对齐的填充域 */
    uint8               __filler[6];

    /** 限购额度/套保额度 */
    int64               purchaseLimit;
    /** 日初占用的期权限购额度 */
    int64               originalUsedPurchaseAmt;
    /** 日中累计开仓占用的期权限购额度 */
    int64               totalOpenPurchaseAmt;
    /** 当前挂单冻结的期权限购额度 */
    int64               frzPurchaseAmt;
    /** 日中累计平仓释放的期权限购额度 */
    int64               totalClosePurchaseAmt;
    /** 可用限购额度/套保额度 */
    int64               availablePurchaseLimit;
} OesOptPurchaseLimitItemT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_PURCHASE_LIMIT_ITEM                             \
        {0}, {0}, {0}, 0, 0, {0}, \
        0, 0, 0, 0, 0, 0
/* -------------------------           */


/**
 * 查询期权限购额度信息请求
 */
typedef struct _OesQryOptPurchaseLimitReq {
    /** 查询请求消息头 */
    OesQryReqHeadT      reqHead;
    /** 查询过滤条件 */
    OesQryOptPurchaseLimitFilterT
                        qryFilter;
} OesQryOptPurchaseLimitReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_PURCHASE_LIMIT_REQ                          \
        {NULLOBJ_OES_QRY_REQ_HEAD}, \
        {NULLOBJ_OES_QRY_OPT_PURCHASE_LIMIT_FILTER}
/* -------------------------           */


/**
 * 查询期权限购额度信息应答
 */
typedef struct _OesQryOptPurchaseLimitRsp {
    /** 查询应答消息头 */
    OesQryRspHeadT      rspHead;
    /** 通知消息数组 */
    OesOptPurchaseLimitItemT
                        qryItems[OES_MAX_OPT_PURCHASE_LIMIT_ITEM_CNT_PER_PACK];
} OesQryOptPurchaseLimitRspT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_PURCHASE_LIMIT_RSP                          \
        {NULLOBJ_OES_QRY_RSP_HEAD}, \
        {{NULLOBJ_OES_OPT_PURCHASE_LIMIT_ITEM}}
/* -------------------------           */


/* ===================================================================
 * 查询期权行权指派信息相关结构体定义
 * =================================================================== */

/**
 * 查询期权行权指派信息过滤条件
 */
typedef struct _OesQryOptExerciseAssignFilter {
    /** 客户代码, 可选项 */
    char                custId[OES_CUST_ID_MAX_LEN];
    /** 证券账户代码, 可选项 */
    char                invAcctId[OES_INV_ACCT_ID_MAX_LEN];
    /** 期权合约代码, 可选项 */
    char                securityId[OES_SECURITY_ID_MAX_LEN];

    /** 市场代码 @see eOesMarketIdT */
    uint8               mktId;
    /** 持仓类型 @see eOesOptPositionTypeT */
    uint8               positionType;
    /** 按64位对齐的填充域 */
    uint8               __filler[6];

    /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */
    int64               userInfo;
} OesQryOptExerciseAssignFilterT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_EXERCISE_ASSIGN_FILTER                      \
        {0}, {0}, {0}, \
        0, 0, {0}, 0
/* -------------------------           */


/**
 * 查询到的期权行权指派信息内容
 */
typedef OesOptionExerciseAssignBaseT                                    \
        OesOptExerciseAssignItemT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_OPT_EXERCISE_ASSIGN_ITEM                            \
        NULLOBJ_OES_OPTION_EXERCISE_ASSIGN_BASE
/* -------------------------           */


/**
 * 查询期权行权指派信息请求
 */
typedef struct _OesQryOptExerciseAssignReq {
    /** 查询请求消息头 */
    OesQryReqHeadT      reqHead;
    /** 查询过滤条件 */
    OesQryOptExerciseAssignFilterT
                        qryFilter;
} OesQryOptExerciseAssignReqT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_EXERCISE_ASSIGN_REQ                         \
        {NULLOBJ_OES_QRY_REQ_HEAD}, \
        {NULLOBJ_OES_QRY_OPT_EXERCISE_ASSIGN_FILTER}
/* -------------------------           */


/**
 * 查询期权行权指派信息应答
 */
typedef struct _OesQryOptExerciseAssignRsp {
    /** 查询应答消息头 */
    OesQryRspHeadT      rspHead;
    /** 期权行权指派信息数组 */
    OesOptExerciseAssignItemT
                        qryItems[OES_MAX_OPT_EXERCISE_ASSIGN_ITEM_CNT_PER_PACK];
} OesQryOptExerciseAssignRspT;


/* 结构体的初始化值定义 */
#define NULLOBJ_OES_QRY_OPT_EXERCISE_ASSIGN_RSP                         \
        {NULLOBJ_OES_QRY_RSP_HEAD}, \
        {{NULLOBJ_OES_OPT_EXERCISE_ASSIGN_ITEM}}
/* -------------------------           */


#ifdef __cplusplus
}
#endif

#endif  /* _OES_QRY_PACKETS_OPTION_H */
