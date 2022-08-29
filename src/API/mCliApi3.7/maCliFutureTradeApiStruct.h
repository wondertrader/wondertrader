#if !defined(__MA_CLI_FUTURE_TRADE_API_STRUCT_H__)
#define __MA_CLI_FUTURE_TRADE_API_STRUCT_H__

#include "maCliTradeApi.h"
BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10330001:期货登陆--------------------------
struct MATRADEAPI CReqFutureUserLoginField
{
  char            szCustCode[16 + 1];         // 客户代码
  char            szCuacctCode[16 + 1];       // 资产账户 CTP  客户ID
  char            szCustPwd[256 + 1];         // 客户密码
  char            szEncryptKey[32 + 1];       // 加密因子
  char            szMacAddress[21 + 1];       // Mac地址
  char            szOnetimePassword[41 + 1];  // 动态密码
  char            szClientIpAddress[16 + 1];  // 终端IP地址
};

struct MATRADEAPI CRspFutureUserLoginField
{
  int             iErrorId;                   // 错误代码
  char            szErrorMsg[81 + 1];         // 错误信息
  char            szTradingDay[9 + 1];        // 交易日
  char            szLoginTime[9 + 1];         // 登录成功时间
  char            szCuacctCode[16 + 1];       // 资产账户 CTP  客户ID
  char            szCustCode[16 + 1];         // 客户代码
  char            szSystemName[41 + 1];       // 交易系统名称
  int             iFrontId;                   // 前置编号
  LONGLONG        llSessionId;                // 会话编号
  char            szMaxOrderRef[13 + 1];      // 最大报单引用
  char            szShfeTime[9 + 1];          // 上期所时间
  char            szDceTime[9 + 1];           // 大商所时间
  char            szCzceTime[9 + 1];          // 郑商所时间
  char            szFfexTime[9 + 1];          // 中金所时间
};

//-------------------------------10330002:期货登出--------------------------
struct MATRADEAPI CReqFutureUserLogoutField
{
  char            szCustCode[16 + 1];         // 客户代码
  char            szCuacctCode[16 + 1];       // 资产账户 CTP 客户ID
};

struct MATRADEAPI CRspFutureUserLogoutField
{
  int             iErrorId;                   // 错误代码
  char            szErrorMsg[81 + 1];         // 错误信息
  char            szCustCode[16 + 1];         // 客户代码
  char            szCuacctCode[16 + 1];       // 资产账户 CTP 客户ID
};

//报单录入
struct MATRADEAPI CReqFutureOrderField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];//合约代码    INSTRUMENT_ID        VARCHAR(31)Y 48
  char     chOrderPriceType;     //报单价格条件  ORDER_PRICE_TYPE     VARCHAR(1) Y 8739
  char     chDirection;          //买卖方向    DIRECTION            VARCHAR(1) Y 8740
  char     szCombOffsetFlag[5+1];//组合开平标志  COMB_OFFSET_FLAG     VARCHAR(5) Y 8741
  char     szCombHedgeFlag[5+1];//组合投机套保标志COMB_HEDGE_FLAG      VARCHAR(5) X 8742
  char     szLimitPrice[21+1];   //价格      LIMIT_PRICE          CMONEY     Y 8958
  LONGLONG llVolumeTotalOriginal;//数量      VOLUME_TOTAL_ORIGINALBIGINT     Y 8986
  char     chTimeCondition;      //有效期类型   TIME_CONDITION       VARCHAR(1) X 8710字典：TIME_CONDITION
  char     szGtdData[9+1];      //GTD日期   GTD_DATA             VARCHAR(9) X 8723
  char     chVolumeCondition;    //成交量类型   VOLUME_CONDITION     VARCHAR(1) X 8711
  LONGLONG llMinVolume;          //最小成交量   MIN_VOLUME           BIGINT     X 8712
  char     chContingentCondition;//触发条件    CONTINGENT_CONDITION VARCHAR(1) X 8713
  char     szStopPrice[21+1];    //止损价     STOP_PRICE           CMONEY     X 8714
  char     chForceCloseReason;   //强平原因    FORCE_CLOSE_REASON   VARCHAR(1) X 8715
  int      iIsAutoSuspend;       //自动挂起标志  IS_AUTO_SUSPEND      SMALLINT   X 8716
  char     szBusinessUnit[21+1];//业务单元    BUSINESS_UNIT        VARCHAR(21)X 8717
  int      iUserForceClose;      //用户强评标志  USER_FORCE_CLOSE     SMALLINT   X 8719
  int      iIsSwapOrder;         //互换单标志   IS_SWAP_ORDER        SMALLINT   X 8720
};

struct MATRADEAPI CRspFutureOrderField
{
  int      iErrorId;                 //错误代码    ERROR_ID             SMALLINT   8900
  char     szErrorMsg[81+1];        //错误信息    ERROR_MSG            VARCHAR(81)8901
  LONGLONG llOrderNo;                //委托编号    ORDER_NO             INTEGER    9106
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];    //合约代码    INSTRUMENT_ID        VARCHAR(31)48
  char     szOrderRef[13+1];        //报单引用    ORDER_REF            VARCHAR(13)8738
  char     szUserId[16+1];          //用户代码    USER_ID              VARCHAR(16)9080
  char     chOrderPriceType;         //报单价格条件  ORDER_PRICE_TYPE     VARCHAR(1) 8739
  char     chDirection;              //买卖方向    DIRECTION            VARCHAR(1) 8740
  char     szCombOffsetFlag[5+1];   //组合开平标志  COMB_OFFSET_FLAG     VARCHAR(5) 8741
  char     szCombHedgeFlag[5+1];    //组合投机套保标志COMB_HEDGE_FLAG      VARCHAR(5) 8742
  char     szLimitPrice[21+1];       //价格      LIMIT_PRICE          CMONEY     8958
  LONGLONG llVolumeTotalOriginal;    //数量      VOLUME_TOTAL_ORIGINALBIGINT     8986
  char     chTimeCondition;          //有效期类型   TIME_CONDITION       VARCHAR(1) 8710
  char     szGtdData[9+1];          //GTD日期   GTD_DATA             VARCHAR(9) 8723
  char     chVolumeCondition;        //成交量类型   VOLUME_CONDITION     VARCHAR(1) 8711
  LONGLONG llMinVolume;              //最小成交量   MIN_VOLUME           BIGINT     8712
  char     chContingentCondition;    //触发条件    CONTINGENT_CONDITION VARCHAR(1) 8713
  char     szStopPrice[21+1];        //止损价     STOP_PRICE           CMONEY     8714
  char     chForceCloseReason;       //强平原因    FORCE_CLOSE_REASON   VARCHAR(1) 8715
  int      iIsAutoSuspend;           //自动挂起标志  IS_AUTO_SUSPEND      SMALLINT   8716
  char     szBusinessUnit[21+1];    //业务单元    BUSINESS_UNIT        VARCHAR(21)8717
  int      iRequestId;               //请求编号    REQUEST_ID           SMALLINT   8718
  int      iUserForceClose;          //用户强评标志  USER_FORCE_CLOSE     SMALLINT   8719
  int      iIsSwapOrder;             //互换单标志   IS_SWAP_ORDER        SMALLINT   8720
};

//报单操作
struct MATRADEAPI CReqFutureChgOrderField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szOrderRef[13+1];//报单引用 ORDER_REF    VARCHAR(13)X 8738CTP必填
  int      iRequestId;    //请求编号 REQUEST_ID   INTEGER    X 8718CTP必填
  int      iFrontId;      //前置编号 FRONT_ID     SMALLINT   Y 8732CTP必填
  char     szExchangeId[9+1];//交易所代码EXCHANGE_ID  VARCHAR(9) X 207
  char     szOrderSysId[21+1];//报单编号 ORDER_SYS_ID VARCHAR(21)Y 8745
  char     chActionFlag;  //操作标志 ACTION_FLAG  VARCHAR(1) X 8747CTP必填（改单、撤单）
  char     szLimitPrice[21+1];//价格   LIMIT_PRICE  CMONEY     X 8958CTP改单必填
  LONGLONG llVolumeChange;//数量变化 VOLUME_CHANGEBIGINT     X 8721CTP改单必填
  char     szUserId[16+1];//用户代码 USER_ID      VARCHAR(16)X 9080
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_IDVARCHAR(31)X 48
};

struct MATRADEAPI CRspFutureChgOrderField
{
  int      iErrorId;          //错误代码  ERROR_ID     SMALLINT   8900
  char     szErrorMsg[81+1]; //错误信息  ERROR_MSG    VARCHAR(81)8901
  char     szBrokerId[11+1]; //经纪公司代码BROKER_ID    VARCHAR(11)8890
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szOrderRef[13+1]; //报单引用  ORDER_REF    VARCHAR(13)8738
  int      iRequestId;        //请求编号  REQUEST_ID   INTEGER    8718
  int      iFrontId;          //前置编号  FRONT_ID     INTEGER    8732
  char     szExchangeId[9+1];//交易所代码 EXCHANGE_ID  VARCHAR(9) 207
  char     szOrderSysId[21+1];//报单编号  ORDER_SYS_ID VARCHAR(21)8745
  char     chActionFlag;      //操作标志  ACTION_FLAG  VARCHAR(1) 8747
  char     szLimitPrice[21+1];//价格    LIMIT_PRICE  CMONEY     8958
  LONGLONG llVolumeChange;    //数量变化  VOLUME_CHANGEBIGINT     8721
  char     szUserId[16+1];   //用户代码  USER_ID      VARCHAR(16)9080
  char     szInstrumentId[31+1];//合约代码  INSTRUMENT_IDVARCHAR(31)48
};
//报单查询
struct MATRADEAPI CReqFutureOrderInfoField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_ID    VARCHAR(31)X 48
  char     szExchangeId[9+1];//交易所代码EXCHANGE_ID      VARCHAR(9) X 207
  char     szOrderSysId[21+1];//报单编号 ORDER_SYS_ID     VARCHAR(21)X 8745
  char     szInsertTimeStart[9+1];//开始时间 INSERT_TIME_STARTVARCHAR(9) X 9040
  char     szInsertTimeEnd[9+1];//结束时间 INSERT_TIME_END  VARCHAR(9) X 9041
};

struct MATRADEAPI CRspFutureOrderInfoField
{
  int      iErrorId;                  //错误代码        ERROR_ID               SMALLINT   8900
  char     szErrorMsg[81+1];         //错误信息        ERROR_MSG              VARCHAR(81)8901
  char     szBrokerId[11+1];         //经纪公司代码      BROKER_ID              VARCHAR(11)8890
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];     //合约代码        INSTRUMENT_ID          VARCHAR(31)48
  char     szOrderRef[13+1];         //报单引用        ORDER_REF              VARCHAR(13)8738
  char     szUserId[16+1];           //用户代码        USER_ID                VARCHAR(16)9080
  char     chOrderPriceType;          //报单价格条件      ORDER_PRICE_TYPE       VARCHAR(1) 8739
  char     chDirection;               //买卖方向        DIRECTION              VARCHAR(1) 8740
  char     szCombOffsetFlag[5+1];    //组合开平标志      COMB_OFFSET_FLAG       VARCHAR(5) 8741
  char     szCombHedgeFlag[5+1];     //组合投机套保标志    COMB_HEDGE_FLAG        VARCHAR(5) 8742
  char     szLimitPrice[21+1];        //价格          LIMIT_PRICE            CMONEY     8958
  LONGLONG llVolumeTotalOriginal;     //数量          VOLUME_TOTAL_ORIGINAL  BIGINT     8986
  char     chTimeCondition;           //有效期类型       TIME_CONDITION         VARCHAR(1) 8710
  char     szGtdDate[9+1];           //GTD日期       GTD_DATE               VARCHAR(9) 8723
  char     chVolumeCondition;         //成交量类型       VOLUME_CONDITION       VARCHAR(1) 8711
  LONGLONG llMinVolume;               //最小成交量       MIN_VOLUME             BIGINT     8712
  char     chContingentCondition;     //触发条件        CONTINGENT_CONDITION   VARCHAR(1) 8713
  char     szStopPrice[21+1];         //止损价         STOP_PRICE             CMONEY     8714
  char     chForceCloseReason;        //强平原因        FORCE_CLOSE_REASON     VARCHAR(1) 8715
  int      iIsAutoSuspend;            //自动挂起标志      IS_AUTO_SUSPEND        SMALLINT   8716
  char     szBusinessUnit[21+1];     //业务单元        BUSINESS_UNIT          VARCHAR(21)8717
  int      iRequestId;                //请求编号        REQUEST_ID             INTEGER    8718
  char     szOrderLocalId[13+1];     //本地报单编号      ORDER_LOCAL_ID         VARCHAR(13)8722
  char     szExchangeId[9+1];        //交易所代码       EXCHANGE_ID            VARCHAR(9) 207
  char     szParticipantId[11+1];    //会员代码        PARTICIPANT_ID         VARCHAR(11)8724
  char     szClientId[11+1];         //客户代码        CLIENT_ID              VARCHAR(11)8902
  char     szExchangeInstId[31+1];   //合约在交易所的代码   EXCHANGE_INST_ID       VARCHAR(31)8725
  char     szTraderId[21+1];         //交易所交易员代码    TRADER_ID              VARCHAR(21)8726
  int      iInstallId;                //安装编号        INSTALL_ID             SMALLINT   8728
  char     chOrderSubmitStatus;       //报单提交状态      ORDER_SUBMIT_STATUS    VARCHAR(1) 8729
  int      iNotifySequence;           //报单提示序号      NOTIFY_SEQUENCE        SMALLINT   8743
  char     szTradingDay[9+1];        //交易日         TRADING_DAY            VARCHAR(9) 8844
  int      iSettlementId;             //结算编号        SETTLEMENT_ID          SMALLINT   8744
  char     szOrderSysId[21+1];       //报单编号        ORDER_SYS_ID           VARCHAR(21)8745
  char     chOrderSource;             //报单来源        ORDER_SOURCE           VARCHAR(1) 8750
  char     chOrderStatus;             //报单状态        ORDER_STATUS           VARCHAR(1) 8751
  char     chOrderType;               //报单类型        ORDER_TYPE             VARCHAR(1) 8752
  LONGLONG llVolumeTraded;            //今成交数量       VOLUME_TRADED          BIGINT     8753
  LONGLONG llVolumeTotal;             //剩余数量        VOLUME_TOTAL           BIGINT     8754
  char     szInsertDate[9+1];        //报单日期        INSERT_DATE            VARCHAR(9) 8834
  char     szInsertTime[9+1];        //委托时间        INSERT_TIME            VARCHAR(9) 8845
  char     szActiveTime[9+1];        //激活时间        ACTIVE_TIME            VARCHAR(9) 8755
  char     szSuspendTime[9+1];       //挂起时间        SUSPEND_TIME           VARCHAR(9) 8756
  char     szUpdateTime[9+1];        //最后修改时间      UPDATE_TIME            VARCHAR(9) 8757
  char     szCancelTime[9+1];        //撤销时间        CANCEL_TIME            VARCHAR(9) 8758
  char     szActiveTraderId[21+1];   //最后修改交易所交易员代码ACTIVE_TRADER_ID       VARCHAR(21)8727
  char     szClearingPartId[11+1];   //结算会员编号      CLEARING_PART_ID       VARCHAR(11)8759
  int      iSequenceNo;               //序号          SEQUENCE_NO            SMALLINT   8832
  char     szUserProductInfo[11+1];  //用户端产品信息     USER_PRODUCT_INFO      VARCHAR(11)8730
  char     szStatusMsg[81+1];        //状态信息        STATUS_MSG             VARCHAR(81)8760
  int      iUserForceClose;           //用户强评标志      USER_FORCE_CLOSE       SMALLINT   8719
  char     szActiveUserId[16+1];     //操作用户代码      ACTIVE_USER_ID         VARCHAR(16)8810
  int      iBrokerOrderSeq;           //经纪公司报单编号    BROKER_ORDER_SEQ       SMALLINT   8746
  char     szRealtiveOrderSysId[21+1];//相关报单        REALTIVE_ORDER_SYS_ID  VARCHAR(21)8761
  LONGLONG llZceTotalTradedVolume;    //郑商所成交数量     ZCE_TOTAL_TRADED_VOLUMEBIGINT     8762
  int      iIsSwapOrder;              //互换单标志       IS_SWAP_ORDER          SMALLINT   8720
};
//成交查询
struct MATRADEAPI CReqFutureMatchInfoField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_ID    VARCHAR(31)X 48
  char     szExchangeId[9+1];//交易所代码EXCHANGE_ID      VARCHAR(9) X 207
  char     szOrderSysId[32+1];//成交编号 ORDER_SYS_ID     VARCHAR(21)X 17
  char     szInsertTimeStart[9+1];//开始时间 INSERT_TIME_STARTVARCHAR(9) X 9040
  char     szInsertTimeEnd[9+1];//结束时间 INSERT_TIME_END  VARCHAR(9) X 9041
};

struct MATRADEAPI CRspFutureMatchInfoField
{
  int      iErrorId;                 //错误代码     ERROR_ID            SMALLINT   8900
  char     szErrorMsg[81+1];        //错误信息     ERROR_MSG           VARCHAR(81)8901
  char     szBrokerId[11+1];        //经纪公司代码   BROKER_ID           VARCHAR(11)8890
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];    //合约代码     INSTRUMENT_ID       VARCHAR(31)48
  char     szOrderRef[13+1];        //报单引用     ORDER_REF           VARCHAR(13)8738
  char     szUserId[16+1];          //用户代码     USER_ID             VARCHAR(16)9080
  char     szExchangeId[9+1];       //交易所代码    EXCHANGE_ID         VARCHAR(9) 207
  char     szTradeId[32+1];         //成交编号     TRADE_ID            VARCHAR(21)17
  char     chDirection;              //买卖方向     DIRECTION           VARCHAR(1) 8740
  char     szOrderSysId[21+1];      //报单编号     ORDER_SYS_ID        VARCHAR(21)8745
  char     szParticipantId[11+1];   //会员代码     PARTICIPANT_ID      VARCHAR(11)8724
  char     szClientId[11+1];        //客户代码     CLIENT_ID           VARCHAR(11)8902
  char     chTradingRole;            //交易角色     TRADING_ROLE        VARCHAR(1) 8763
  char     szExchangeInstId[31+1];  //合约在交易所的代码EXCHANGE_INST_ID    VARCHAR(31)8725
  char     szOffsetFlag[5+1];       //组合开平标志   OFFSET_FLAG         VARCHAR(5) 8741
  char     szHedgeFlag[5+1];        //组合投机套保标志 HEDGE_FLAG          VARCHAR(5) 8742
  char     szPrice[21+1];            //价格       PRICE               CMONEY     44
  LONGLONG llVolume;                 //数量       VOLUME              BIGINT     8986
  char     szTradeDate[9+1];        //成交时期     TRADE_DATE          VARCHAR(9) 8839
  char     szTradeTime[9+1];        //成交时间     TRADE_TIME          VARCHAR(9) 8840
  char     chTradeType;              //成交类型     TRADE_TYPE          VARCHAR(1) 8764
  char     chPriceSource;            //成交价来源    PRICE_SOURCE        VARCHAR(1) 8765
  char     szTraderId[21+1];        //交易所交易员代码 TRADER_ID           VARCHAR(21)8726
  char     szOrderLocalId[13+1];    //本地报单编号   ORDER_LOCAL_ID      VARCHAR(13)8722
  char     szClearingPartId[11+1];  //结算会员编号   CLEARING_PART_ID    VARCHAR(11)8759
  char     chTimeCondition;          //有效期类型    TIME_CONDITION      VARCHAR(1) 8710
  char     szGtdDate[9+1];          //GTD日期    GTD_DATE            VARCHAR(9) 8723
  char     chVolumeCondition;        //成交量类型    VOLUME_CONDITION    VARCHAR(1) 8711
  LONGLONG llMinVolume;              //最小成交量    MIN_VOLUME          BIGINT     8712
  char     chContingentCondition;    //触发条件     CONTINGENT_CONDITIONVARCHAR(1) 8713
  char     szStopPrice[21+1];        //止损价      STOP_PRICE          CMONEY     8714
  char     chForceCloseReason;       //强平原因     FORCE_CLOSE_REASON  VARCHAR(1) 8715
  int      iIsAutoSuspend;           //自动挂起标志   IS_AUTO_SUSPEND     SMALLINT   8716
  char     szBusinessUnit[21+1];    //业务单元     BUSINESS_UNIT       VARCHAR(21)8717
  int      iRequestId;               //请求编号     REQUEST_ID          INTEGER    8718
  int      iSequenceNo;              //序号       SEQUENCE_NO         SMALLINT   8832
  char     szTradingDay[9+1];       //交易日      TRADING_DAY         VARCHAR(9) 8844
  int      iSettlementId;            //结算编号     SETTLEMENT_ID       SMALLINT   8744
  int      iBrokerOrderSeq;          //经纪公司报单编号 BROKER_ORDER_SEQ    SMALLINT   8746
  char     chTradeSource;            //成交来源     TRADE_SOURCE        VARCHAR(1) 8766
};
//投资者查询
struct MATRADEAPI CReqFutureCustInfoField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
};

struct MATRADEAPI CRspFutureCustInfoField
{
  int      iErrorId;                //错误代码    ERROR_ID            SMALLINT    8900
  char     szErrorMsg[81+1];       //错误信息    ERROR_MSG           VARCHAR(81) 8901
  char     szInvestorId[13+1];     //投资者代码   INVESTOR_ID         VARCHAR(13) 8903
  char     szBrokerId[11+1];       //经纪公司代码  BROKER_ID           VARCHAR(11) 8890
  char     szInvestorGroupId[13+1];//投资者分组代码 INVESTOR_GROUP_ID   VARCHAR(13) 8749
  char     szInvestorName[81+1];   //投资者名称   INVESTOR_NAME       VARCHAR(81) 8938
  char     chIdentifiedCardType;    //证件类型    IDENTIFIED_CARD_TYPEVARCHAR(1)  8893
  char     szIdentifiedCardNo[51+1];//证件号码    IDENTIFIED_CARD_NO  VARCHAR(51) 8894
  int      iIsActive;               //是否活跃    IS_ACTIVE           SMALLINT    8767
  char     szTelephone[41+1];      //联系电话    TELEPHONE           VARCHAR(41) 8768
  char     szAddress[101+1];       //通讯地址    ADDRESS             VARCHAR(101)8769
  char     szOpendate[9+1];        //开户日期    OPENDATE            VARCHAR(9)  8895
  char     szMobile[41+1];         //手机      MOBILE              VARCHAR(41) 8770
  char     szCommmodelId[13+1];    //手续费率模板代码COMMMODEL_ID        VARCHAR(13) 8771
  char     szMarginmodelId[13+1];  //保证金率模板代码MARGINMODEL_ID      VARCHAR(13) 8772
};
//投资者持仓查询
struct MATRADEAPI CReqFuturePosInfoField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szCustCode[16+1];     //客户代码CUST_CODE      VARCHAR(16)
  char     szInstrumentId[31+1];//合约代码INSTRUMENT_IDVARCHAR(31)X 48
};

struct MATRADEAPI CRspFuturePosInfoField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szExchangeId[9+1];  //交易所代码 EXCHANGE_ID     VARCHAR(9) 207
  char     szExchangeName[31+1];//交易所名称 EXCHANGE_NAME   VARCHAR(31)9441
  char     szInstrumentId[31+1];//合约代码  INSTRUMENT_ID   VARCHAR(31)48
  char     szInstrumentName[21+1];//合约名称  INSTRUMENT_NAME VARCHAR(21)55
  char     szHedgeFlag[5+1];   //投保标记  HEDGE_FLAG      VARCHAR(5) 8939
  char     chPosiDirection;     //持仓多空方向POSI_DIRECTION  VARCHAR(1) 8740
  LONGLONG llPositionVolume;    //持仓量   POSITION_VOLUME BIGINT     9351
  LONGLONG llExeAvl;            //可执行量  EXE_AVL         BIGINT     8869
  int      iCloseFrzVolume;     //平仓冻结手数CLOSE_FRZ_VOLUMESMALLINT   8870
  char     szPositionCost[21+1];//持仓均价  POSITION_COST   CMONEY     8785
  char     szOpenCost[21+1];    //开仓均价  OPEN_COST       CMONEY     8700
  char     szLatestPrice[21+1]; //最新价   LATEST_PRICE    CMONEY     9094
  char     szHedgeName[31+1];  //投保名称  HEDGE_NAME      VARCHAR(31)9414
  char     szFloatProfit[21+1]; //浮动盈亏  FLOAT_PROFIT    CMONEY     8796
  char     szExchangeCode[31+1];//交易编码  EXCHANGE_CODE   VARCHAR(31)9080
  char     szProductCode[31+1];//品种代码  PRODUCT_CODE    VARCHAR(31)8705
  char     szProductName[31+1];//品种名称  PRODUCT_NAME    VARCHAR(31)8706
  char     szUseMargin[21+1];   //持仓保证金 USE_MARGIN      CMONEY     8787
  char     szDelivDate[31+1];  //交割期   DELIV_DATE      VARCHAR(31)9151
  LONGLONG llTodayPosition;  //当日持仓 TODAY_POSITION  BIGINT  9445
};
//合约查询
struct MATRADEAPI CReqFutureContractInfoField
{
  char     szInstrumentId[31+1];//合约代码    INSTRUMENT_ID   VARCHAR(31)X 48
  char     szExchangeId[9+1];//交易所代码   EXCHANGE_ID     VARCHAR(9) X 207
  char     szExchangeInstId[31+1];//合约在交易所代码EXCHANGE_INST_IDVARCHAR(31)X 8725
  char     szProductId[31+1];//产品代码    PRODUCT_ID      VARCHAR(31)X 8705
};

struct MATRADEAPI CRspFutureContractInfoField
{
  int      iErrorId;                    //错误代码         ERROR_ID                 SMALLINT   8900
  char     szErrorMsg[81+1];           //错误信息         ERROR_MSG                VARCHAR(81)8901
  char     szInstrumentId[31+1];       //合约代码         INSTRUMENT_ID            VARCHAR(31)48
  char     szExchangeId[9+1];          //交易所代码        EXCHANGE_ID              VARCHAR(9) 207
  char     szInstrumentName[21+1];     //合约名称         INSTRUMENT_NAME          VARCHAR(21)55
  char     szExchangeInstId[31+1];     //合约在交易所的代码    EXCHANGE_INST_ID         VARCHAR(31)8725
  char     szProductId[31+1];          //产品代码         PRODUCT_ID               VARCHAR(31)8705
  char     chProductClass;              //产品类型         PRODUCT_CLASS            VARCHAR(1) 8706
  int      iDeliveryYear;               //交割年份         DELIVERY_YEAR            SMALLINT   8707
  int      iDeliveryMonth;              //交割月          DELIVERY_MONTH           SMALLINT   8708
  LONGLONG llMaxMarketOrderVolume;      //市价单最大下单量     MAX_MARKET_ORDER_VOLUME  BIGINT     8670
  LONGLONG llMinMarketOrderVolume;      //市价单最小下单量     MIN_MARKET_ORDER_VOLUME  BIGINT     8671
  LONGLONG llMaxLimitOrderVolume;       //限价单最大下单量     MAX_LIMIT_ORDER_VOLUME   BIGINT     8672
  LONGLONG llMinLimitOrderVolume;       //限价单最小下单量     MIN_LIMIT_ORDER_VOLUME   BIGINT     8673
  LONGLONG llVolumeMultiple;            //合约数量乘数       VOLUME_MULTIPLE          BIGINT     8674
  char     szPriceTick[21+1];           //最小变动价位       PRICE_TICK               CMONEY     8675
  char     szCreateDate[9+1];          //创建日          CREATE_DATE              VARCHAR(9) 8676
  char     szOpenDate[9+1];            //上市日          OPEN_DATE                VARCHAR(9) 8677
  char     szExpireDate[9+1];          //到期日          EXPIRE_DATE              VARCHAR(9) 8678
  char     szStartDelivDate[9+1];      //开始交割日        START_DELIV_DATE         VARCHAR(9) 8679
  char     szEndDelivDate[9+1];        //结束交割日        END_DELIV_DATE           VARCHAR(9) 8680
  char     chInstLifePhase;             //合约生命周期状态     INST_LIFE_PHASE          VARCHAR(1) 8681
  int      iIsTrading;                  //当前是否交易       IS_TRADING               SMALLINT   8682
  char     chPositionType;              //持仓类型         POSITION_TYPE            VARCHAR(1) 8683
  char     chPositionDateType;          //持仓日期类型       POSITION_DATE_TYPE       VARCHAR(1) 8773
  char     szLongMarginRatio[8+1];      //多头保证金率       LONG_MARGIN_RATIO        CRATE      8684
  char     szShortMarginRatio[8+1];     //空头保证金率       SHORT_MARGIN_RATIO       CRATE      8685
  char     chMaxMarginSideAlgorithm;    //是否使用大额单边保证金算法MAX_MARGIN_SIDE_ALGORITHMVARCHAR(1) 8686
};
//投资者资金查询
struct MATRADEAPI CReqFutureFundInfoField
{
  char     szCuacctCode[16+1]; //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     chCuacctType; //账户类型 CUACCT_TYPE CHAR(1) √  8826
  char     szCustCode[16+1]; //客户代码 CUST_CODE VARCHAR(16) X 8902
};

struct MATRADEAPI CRspFutureFundInfoField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szCurrencyType[5+1];    //币种     CURRENCY_TYPE      VARCHAR(5) 15
  char     szClentName[31+1];      //客户简称   CLENT_NAME         VARCHAR(31)8910
  int      iTrdDate;                //交易日期   TRD_DATE           INTEGER    8844
  char     szPrebalanceAmout[21+1]; //上日结存   PREBALANCE_AMOUT   CMONEY     8604
  char     szAvailableAmout[21+1];  //可用资金   AVAILABLE_AMOUT    CMONEY     8611
  char     szLongFrozen[21+1];      //买冻结    LONG_FROZEN        CMONEY     8776
  char     szShortFrozen[21+1];     //卖冻结    SHORT_FROZEN       CMONEY     8778
  char     szFrozenCommission[21+1];//冻结手续费  FROZEN_COMMISSION  CMONEY     8790
  char     szCashIn[21+1];          //资金差额  CASH_IN CMONEY  8791
  char     szCommission[21+1];      //手续费    COMMISSION         CMONEY     8792
  char     szBuyMargin[21+1];       //买保证金   BUY_MARGIN         CMONEY     9403
  char     szSellMargin[21+1];      //卖保证金   SELL_MARGIN        CMONEY     9405
  char     szCloseProfit[21+1];     //平仓盈亏   CLOSE_PROFIT       CMONEY     8793
  char     szFloatProfit[21+1];     //浮动盈亏   FLOAT_PROFIT       CMONEY     9089
  char     szPositionProfit[21+1];  //持仓盈亏  POSITION_PROFIT CMONEY  8796
  char     szFundTrdOtd[21+1];      //在途资金   FUND_TRD_OTD       CMONEY     8866
  char     szNoDeliveryProfit[21+1];//未交割盈亏  NO_DELIVERY_PROFIT CMONEY     9406
  char     szDepositAmout[21+1];    //今日入金   DEPOSIT_AMOUT      CMONEY     8607
  char     szWithdrawAmout[21+1];   //今日出金   WITHDRAW_AMOUT     CMONEY     8608
  char     szDynamicRight[21+1];    //动态权益   DYNAMIC_RIGHT      CMONEY     9350
  char     szFrozenCash[21+1];      //冻结资金   FROZEN_CASH        CMONEY     8789
  char     szCurrmargin[21+1];      //总保证金   CURRMARGIN         CMONEY     8609
  char     szExchangeMargin[21+1];  //交易所保证金 EXCHANGE_MARGIN    CMONEY     8701
  char     szMortgageAmout[21+1];   //质押金额   MORTGAGE_AMOUT     CMONEY     8615
  char     szCreditAmout[21+1];     //信用金额   CREDIT_AMOUT       CMONEY     8614
  char     szRiskLevel[21+1];       //风险级别   RISK_LEVEL         CMONEY     9407
  char     szRiskLevel1[21+1];      //风险度1   RISK_LEVEL_1       CMONEY     9408
  char     szRiskLevel2[21+1];      //风险度2   RISK_LEVEL_2       CMONEY     9409
  char     szDynamicmarketRight[21+1];//市值动态权益 DYNAMICMARKET_RIGHTCMONEY     9083
  char     szExeDynamicMargin[21+1];//执行冻结保证金EXE_DYNAMIC_MARGIN CMONEY     9410
  char     szCurrentMargin[21+1];   //实时保证金  CURRENT_MARGIN     CMONEY     9411
  char     szPreferentialMargin[21+1];//优惠保证金  PREFERENTIAL_MARGINCMONEY     9412
  char     szAddMargin[21+1];       //追加保证金  ADD_MARGIN         CMONEY     9413
  char     szDeliveryMargin[21+1];  //投资者交割保证金  DELIVERY_MARGIN CMONEY  8616
  char     szInterest[21+1];        //利息收入  INTEREST  CMONEY  8606
  char     szInterestBase[21+1];    //利息积数  INTEREST_BASE CMONEY  8605
  int      iSettlementId;           //结算编号  SETTLEMENT_ID INTEGER 8744
};
//最大可开仓数量
struct MATRADEAPI CReqFutureAvlQtyField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_IDVARCHAR(31)Y 48
  char     szExchangeId[9+1];//交易所代码EXCHANGE_ID  VARCHAR(9) Y 207
  int      iStkBiz;       //证券业务 STK_BIZ      SMALLINT   Y 8842
  char     szOrderPrice[21+1];//价格   ORDER_PRICE  CMONEY     X 44
};

struct MATRADEAPI CRspFutureAvlQtyField
{
  LONGLONG llOrderQty;          //可开仓手数ORDER_QTY      BIGINT 38  可委托最大数量
  char     szAvailableAmout[21+1];//可用资金 AVAILABLE_AMOUTCMONEY 8611
  LONGLONG llBuyOrderQty;       //可买开手数BUY_ORDER_QTY  BIGINT 9080
  int      iSellOrderQty;       //可卖开手数SELL_ORDER_QTY INTEGER9081
  LONGLONG llMarketValue;       //动态权益 MARKET_VALUE   BIGINT 9082
};
//最大可平仓数量
struct MATRADEAPI CReqFutureAvlPosField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_IDVARCHAR(31)Y 48
  char     szExchangeId[9+1];//交易所代码EXCHANGE_ID  VARCHAR(9) Y 207
  int      iStkBiz;       //证券业务 STK_BIZ      SMALLINT   Y 8842
  char     szStkpbu[8+1];//席位号  STKPBU       VARCHAR(8) Y 8843
};

struct MATRADEAPI CRspFutureAvlPosField
{
  char     szMktTrdCode[31+1]; //交易编码 MKT_TRD_CODE   VARCHAR(31)9080
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_ID  VARCHAR(31)48
  char     szAvailableAmout[21+1];//可用资金 AVAILABLE_AMOUTCMONEY     8611
  LONGLONG llOrderQty;          //可平仓手数ORDER_QTY      BIGINT     38  可委托最大数量
  LONGLONG llTodayOrderQty;     //可平今手数TODAY_ORDER_QTYBIGINT     9081可委托最大数量
  char     szStkpbu[8+1];      //席位号  STKPBU         VARCHAR(8) 8843
  LONGLONG llMarketValue;       //动态权益 MARKET_VALUE   BIGINT     9082
};
//客户量查询
struct MATRADEAPI CReqFutureUsersOnlineField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
};

struct MATRADEAPI CRspFutureUsersOnlineField
{
  int      iUerNum;    //客户量UER_NUMSMALLINT8600
};

//手续费率查询
struct MATRADEAPI CReqFutureFeeField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  int      iTrdDate;      //交易日期 TRD_DATE     SMALLINT   X 8844
  char     szExchangeId[9+1];//交易所代码EXCHANGE_ID  VARCHAR(9) X 207 交易所代码
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_IDVARCHAR(31)X 48
};

struct MATRADEAPI CRspFutureFeeField
{
  int      iTrdDate;                           //交易日期      TRD_DATE                        SMALLINT   8844
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szExchangeId[9+1];                 //交易所代码     EXCHANGE_ID                     VARCHAR(9) 207
  char     szSeatId[12+1];                    //席位号       SEAT_ID                         VARCHAR(12)8724
  char     szProductId[31+1];                 //品种代码      PRODUCT_ID                      VARCHAR(31)8705
  char     szDeliveryDate[31+1];              //交割期       DELIVERY_DATE                   VARCHAR(31)9151
  char     szBuketOpenCommissionByqty[21+1];   //投机开平手续费按手数BUKET_OPEN_COMMISSION_BYQTY     CMONEY     9415
  char     szBuketOpenCommissionByamt[21+1];   //投机开平手续费按金额BUKET_OPEN_COMMISSION_BYAMT     CMONEY     9416
  char     szBuketCloseCommissionByqty[21+1];  //投机平今手续费按手数BUKET_CLOSE_COMMISSION_BYQTY    CMONEY     9417
  char     szBuketCloseCommissionByamt[21+1];  //投机平今手续费按金额BUKET_CLOSE_COMMISSION_BYAMT    CMONEY     9418
  char     szHedgeOpenCommissionByqty[21+1];   //保值开平手续费按手数HEDGE_OPEN_COMMISSION_BYQTY     CMONEY     9419
  char     szHedgeOpenCommissionByamt[21+1];   //保值开平手续费按金额HEDGE_OPEN_COMMISSION_BYAMT     CMONEY     9420
  char     szHedgeCloseCommissionByqty[21+1];  //保值平今手续费按手数HEDGE_CLOSE_COMMISSION_BYQTY    CMONEY     9421
  char     szHedgeCloseCommissionByamt[21+1];  //保值平今手续费按金额HEDGE_CLOSE_COMMISSION_BYAMT    CMONEY     9422
  char     szArbitrageOpenCommissionByqty[21+1];//套利开平手续费按手数ARBITRAGE_OPEN_COMMISSION_BYQTY CMONEY     9423
  char     szArbitrageOpenCommissionByamt[21+1];//套利开平手续费按金额ARBITRAGE_OPEN_COMMISSION_BYAMT CMONEY     9424
  char     szArbitrageCloseCommissionByqty[21+1];//套利平今手续费按手数ARBITRAGE_CLOSE_COMMISSION_BYQTYCMONEY     9425
  char     szArbitrageCloseCommissionByamt[21+1];//套利平今手续费按金额ARBITRAGE_CLOSE_COMMISSION_BYAMTCMONEY     9426
  char     szExeCommissionByqty[21+1];         //执行手续费率（手数）EXE_COMMISSION_BYQTY            CMONEY     9427
  char     szExeCommissionByamt[21+1];         //执行手续费率（金额）EXE_COMMISSION_BYAMT            CMONEY     9428
  char     szInstrumentId[31+1];              //合约编码      INSTRUMENT_ID                   VARCHAR(31)48
};
//保证金率查询
struct MATRADEAPI CReqFutureMarginField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  int      iTrdDate;      //交易日期 TRD_DATE     SMALLINT   X 8844
  char     szExchangeId[9+1];//交易所代码EXCHANGE_ID  VARCHAR(9) X 207 交易所代码
  char     szInstrumentId[31+1];//合约代码 INSTRUMENT_IDVARCHAR(31)X 48
};

struct MATRADEAPI CRspFutureMarginField
{
  int      iTrdDate;                      //交易日期     TRD_DATE                   SMALLINT   8844
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szExchangeId[9+1];            //交易所代码    EXCHANGE_ID                VARCHAR(9) 207
  char     szSeatId[12+1];               //席位号      SEAT_ID                    VARCHAR(12)8724
  char     szProductId[31+1];            //品种代码     PRODUCT_ID                 VARCHAR(31)8705
  char     szDeliveryDate[31+1];         //交割期      DELIVERY_DATE              VARCHAR(31)9151
  char     szBuketBuyMarginByqty[21+1];   //投机买保证金按手数BUKET_BUY_MARGIN_BYQTY     CMONEY     9429
  char     szBuketBuyMarginByamt[21+1];   //投机买保证金按金额BUKET_BUY_MARGIN_BYAMT     CMONEY     9430
  char     szBuketSellMarginByqty[21+1];  //投机卖保证金按手数BUKET_SELL_MARGIN_BYQTY    CMONEY     9431
  char     szBuketSellMarginByamt[21+1];  //投机卖保证金按金额BUKET_SELL_MARGIN_BYAMT    CMONEY     9432
  char     szHedgeBuyMarginByqty[21+1];   //保值买保证金按手数HEDGE_BUY_MARGIN_BYQTY     CMONEY     9433
  char     szHedgeBuyMarginByamt[21+1];   //保值买保证金按金额HEDGE_BUY_MARGIN_BYAMT     CMONEY     9434
  char     szHedgeSellMarginByqty[21+1];  //保值卖保证金按手数HEDGE_SELL_MARGIN_BYQTY    CMONEY     9435
  char     szHedgeSellMarginByamt[21+1];  //保值卖保证金按金额HEDGE_SELL_MARGIN_BYAMT    CMONEY     9436
  char     szArbitrageBuyMarginByqty[21+1];//套利买保证金按手数ARBITRAGE_BUY_MARGIN_BYQTY CMONEY     9437
  char     szArbitrageBuyMarginByamt[21+1];//套利买保证金按金额ARBITRAGE_BUY_MARGIN_BYAMT CMONEY     9438
  char     szArbitrageSellMarginByqty[21+1];//套利卖保证金按手数ARBITRAGE_SELL_MARGIN_BYQTYCMONEY     9439
  char     szArbitrageSellMarginByamt[21+1];//套利卖保证金按金额ARBITRAGE_SELL_MARGIN_BYAMTCMONEY     9440
  char     szInstrumentId[31+1];         //合约编码     INSTRUMENT_ID              VARCHAR(31)48
};
//交易日期查询
struct MATRADEAPI CReqFutureDateInfoField
{
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
};

struct MATRADEAPI CRspFutureDateInfoField
{
  int      iTrdDate;        //交易日期TRD_DATE   SMALLINT   8844
  char     szSystemDate[32+1];//系统日期SYSTEM_DATEVARCHAR(32)9093
  char     szSystemTime[32+1];//系统时间SYSTEM_TIMEVARCHAR(32)9092
};
//委托推送
struct MATRADEAPI CRtnFutureOrderField
{
  char     szBrokerId[11+1];         //经纪公司代码      BROKER_ID              VARCHAR(11)8890
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];     //合约代码        INSTRUMENT_ID          VARCHAR(31)48
  char     szOrderRef[13+1];         //报单引用        ORDER_REF              VARCHAR(13)8738
  char     szUserId[16+1];           //用户代码        USER_ID                VARCHAR(16)9080
  char     chOrderPriceType;          //报单价格条件      ORDER_PRICE_TYPE       VARCHAR(1) 8739
  char     chDirection;               //买卖方向        DIRECTION              VARCHAR(1) 8740
  char     szCombOffsetFlag[5+1];    //组合开平标志      COMB_OFFSET_FLAG       VARCHAR(5) 8741
  char     szCombHedgeFlag[5+1];     //组合投机套保标志    COMB_HEDGE_FLAG        VARCHAR(5) 8742
  char     szLimitPrice[21+1];        //价格          LIMIT_PRICE            CMONEY     8958
  LONGLONG llVolumeTotalOriginal;     //数量          VOLUME_TOTAL_ORIGINAL  BIGINT     8986
  char     chTimeCondition;           //有效期类型       TIME_CONDITION         VARCHAR(1) 8710
  char     szGtdDate[9+1];           //GTD日期       GTD_DATE               VARCHAR(9) 8723
  char     chVolumeCondition;         //成交量类型       VOLUME_CONDITION       VARCHAR(1) 8711
  LONGLONG llMinVolume;               //最小成交量       MIN_VOLUME             BIGINT     8712
  char     chContingentCondition;     //触发条件        CONTINGENT_CONDITION   VARCHAR(1) 8713
  char     szStopPrice[21+1];         //止损价         STOP_PRICE             CMONEY     8714
  char     chForceCloseReason;        //强平原因        FORCE_CLOSE_REASON     VARCHAR(1) 8715
  int      iIsAutoSuspend;            //自动挂起标志      IS_AUTO_SUSPEND        SMALLINT   8716
  char     szBusinessUnit[21+1];     //业务单元        BUSINESS_UNIT          VARCHAR(21)8717
  int      iRequestId;                //请求编号        REQUEST_ID             INTEGER    8718
  char     szOrderLocalId[13+1];     //本地报单编号      ORDER_LOCAL_ID         VARCHAR(13)8722
  char     szExchangeId[9+1];        //交易所代码       EXCHANGE_ID            VARCHAR(9) 207
  char     szParticipantId[11+1];    //会员代码        PARTICIPANT_ID         VARCHAR(11)8724
  char     szClientId[11+1];         //客户代码        CLIENT_ID              VARCHAR(11)8902
  char     szExchangeInstId[31+1];   //合约在交易所的代码   EXCHANGE_INST_ID       VARCHAR(31)8725
  char     szTraderId[21+1];         //交易所交易员代码    TRADER_ID              VARCHAR(21)8726
  int      iInstallId;                //安装编号        INSTALL_ID             SMALLINT   8728
  char     chOrderSubmitStatus;       //报单提交状态      ORDER_SUBMIT_STATUS    VARCHAR(1) 8729
  int      iNotifySequence;           //报单提示序号      NOTIFY_SEQUENCE        SMALLINT   8743
  char     szTradingDay[9+1];        //交易日         TRADING_DAY            VARCHAR(9) 8844
  int      iSettlementId;             //结算编号        SETTLEMENT_ID          SMALLINT   8744
  char     szOrderSysId[21+1];       //报单编号        ORDER_SYS_ID           VARCHAR(21)8745
  char     chOrderSource;             //报单来源        ORDER_SOURCE           VARCHAR(1) 8750
  char     chOrderStatus;             //报单状态        ORDER_STATUS           VARCHAR(1) 8751
  char     chOrderType;               //报单类型        ORDER_TYPE             VARCHAR(1) 8752
  LONGLONG llVolumeTraded;            //今成交数量       VOLUME_TRADED          BIGINT     8753
  LONGLONG llVolumeTotal;             //剩余数量        VOLUME_TOTAL           BIGINT     8754
  char     szInsertDate[9+1];        //报单日期        INSERT_DATE            VARCHAR(9) 8834
  char     szInsertTime[9+1];        //委托时间        INSERT_TIME            VARCHAR(9) 8845
  char     szActiveTime[9+1];        //激活时间        ACTIVE_TIME            VARCHAR(9) 8755
  char     szSuspendTime[9+1];       //挂起时间        SUSPEND_TIME           VARCHAR(9) 8756
  char     szUpdateTime[9+1];        //最后修改时间      UPDATE_TIME            VARCHAR(9) 8757
  char     szCancelTime[9+1];        //撤销时间        CANCEL_TIME            VARCHAR(9) 8758
  char     szActiveTraderId[21+1];   //最后修改交易所交易员代码ACTIVE_TRADER_ID       VARCHAR(21)8727
  char     szClearingPartId[11+1];   //结算会员编号      CLEARING_PART_ID       VARCHAR(11)8759
  int      iSequenceNo;               //序号          SEQUENCE_NO            SMALLINT   8832
  char     szUserProductInfo[11+1];  //用户端产品信息     USER_PRODUCT_INFO      VARCHAR(11)8730
  char     szStatusMsg[81+1];        //状态信息        STATUS_MSG             VARCHAR(81)8760
  int      iUserForceClose;           //用户强评标志      USER_FORCE_CLOSE       SMALLINT   8719
  char     szActiveUserId[16+1];     //操作用户代码      ACTIVE_USER_ID         VARCHAR(16)8810
  int      iBrokerOrderSeq;           //经纪公司报单编号    BROKER_ORDER_SEQ       SMALLINT   8746
  char     szRealtiveOrderSysId[21+1];//相关报单        REALTIVE_ORDER_SYS_ID  VARCHAR(21)8761
  LONGLONG llZceTotalTradedVolume;    //郑商所成交数量     ZCE_TOTAL_TRADED_VOLUMEBIGINT     8762
  int      iIsSwapOrder;              //互换单标志       IS_SWAP_ORDER          SMALLINT   8720
  int      iFrontId;                  //前置编号        FRONT_ID               INTEGER    8732
  LONGLONG llSessionId;               //会话编号        SESSION_ID             BIGINT     8814
};
//成交推送
struct MATRADEAPI CRtnFutureMatchField
{
  int      iErrorId;                //错误代码     ERROR_ID             SMALLINT   8900
  char     szErrorMsg[81+1];       //错误信息     ERROR_MSG            VARCHAR(81)8901
  char     szBrokerId[11+1];       //经纪公司代码   BROKER_ID            VARCHAR(11)8890
  char     szCuacctCode[16+1];     //资产账户 CUACCT_CODE      VARCHAR(16)     Y 8920CTP  客户ID
  char     szInstrumentId[31+1];   //合约代码     INSTRUMENT_ID        VARCHAR(31)48
  char     szOrderRef[13+1];       //报单引用     ORDER_REF            VARCHAR(13)8738
  char     szUserId[16+1];         //用户代码     USER_ID              VARCHAR(16)9080
  char     szExchangeId[9+1];      //交易所代码    EXCHANGE_ID          VARCHAR(9) 207
  char     szMatchedSn[32+1];      //成交编号     MATCHED_SN           VARCHAR(16)17
  char     chDirection;             //买卖方向     DIRECTION            VARCHAR(1) 8740
  char     szOrderSysId[21+1];     //报单编号     ORDER_SYS_ID         VARCHAR(21)8745
  char     szParticipantId[11+1];  //会员代码     PARTICIPANT_ID       VARCHAR(11)8724
  char     szClientId[11+1];       //客户代码     CLIENT_ID            VARCHAR(11)8902
  char     chTradingRole;           //交易角色     TRADING_ROLE         VARCHAR(1) 8763
  char     szExchangeInstId[31+1]; //合约在交易所的代码EXCHANGE_INST_ID     VARCHAR(31)8725
  char     szCombOffsetFlag[5+1];  //组合开平标志   COMB_OFFSET_FLAG     VARCHAR(5) 8741
  char     szCombHedgeFlag[5+1];   //组合投机套保标志 COMB_HEDGE_FLAG      VARCHAR(5) 8742
  LONGLONG llLimitPrice;            //价格       LIMIT_PRICE          BIGINT     8958
  int      iVolumeTotalOriginal;    //数量       VOLUME_TOTAL_ORIGINALSMALLINT   8986
  char     szTradeDate[9+1];       //成交时期     TRADE_DATE           VARCHAR(9) 8839
  char     szTradeTime[9+1];       //成交时间     TRADE_TIME           VARCHAR(9) 8840
  char     chTradeType;             //成交类型     TRADE_TYPE           VARCHAR(1) 8764
  char     chPriceSource;           //成交价来源    PRICE_SOURCE         VARCHAR(1) 8765
  char     szTraderId[21+1];       //交易所交易员代码 TRADER_ID            VARCHAR(21)8726
  char     szOrderLocalId[13+1];   //本地报单编号   ORDER_LOCAL_ID       VARCHAR(13)8722
  char     szClearingPartId[11+1]; //结算会员编号   CLEARING_PART_ID     VARCHAR(11)8759
  char     szBusinessUnit[21+1];   //业务单元     BUSINESS_UNIT        VARCHAR(21)8717
  int      iSequenceNo;             //序号       SEQUENCE_NO          SMALLINT   8832
  char     szTradingDay[9+1];      //交易日      TRADING_DAY          VARCHAR(9) 8844
  int      iSettlementId;           //结算编号     SETTLEMENT_ID        SMALLINT   8744
  int      iBrokerOrderSeq;         //经纪公司报单编号 BROKER_ORDER_SEQ     SMALLINT   8746
  char     chTradeSource;           //成交来源     TRADE_SOURCE         VARCHAR(1) 8766
};

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

END_NAMESPACE_MACLI
#endif//__MA_CLI_FUTURE_TRADE_API_STRUCT_H__