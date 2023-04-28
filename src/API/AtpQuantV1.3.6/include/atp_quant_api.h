// code_generator_tag
// requests:5f024b1066515f963731eec331bd9d61 responses:b0e958b18fde3671566c702f3c2a40de template:3a7a64e1ed9f5358ed8966384ffd67b9 code:174b44dcb1ce47cf1820a42d41b40a5d
/**
 * Copyright (c) 2018 Archforce Financial Technology.  All rights reserved.
 * Redistribution and use in source and binary forms, with or without  modification, are not permitted.
 * For more information about Archforce, welcome to archforce.cn.
 *
 * @file atp_quant_api.h
 * @author Archforce
 * @brief ATPQuantAPI
 *
 * @note ATPQuantAPI接口及回调句柄定义
 */

#ifndef ATP_QUANT_API_H_
#define ATP_QUANT_API_H_
#include "atp_quant_msg.h"


namespace atp
{
    namespace quant_api
    {
        class Context; //前置声明
        class QUANT_API ATPQuantHandler
        {
        public:
            /**
             * @brief 析构函数
             */
            virtual ~ATPQuantHandler() = default;
        public:
            /**
             * @brief 登录成功
             */
            virtual void OnLogin(const ATPCustomerInfo& msg) {}

            /**
             * @brief 登出成功
             */
            virtual void OnLogout(const char* desc) {}

            /**
             * @brief 自动重连
             */
            virtual void OnRecovering(const char* desc) {}

        public:
            /**
             * @brief 现货委托响应
             */
            virtual void OnRspCashAuctionOrder(const ATPRspCashAuctionOrderMsg& msg, const ATPRspErrorInfo& error_info, const int64_t request_id) {}

            /**
             * @brief 现货委托回报
             */
            virtual void OnRtnCashAuctionOrder(const ATPRtnCashAuctionOrderMsg& msg) {}
            /**
             * @brief 现货撤单响应
             */
            virtual void OnRspCashCancelOrder(const ATPRspCashCancelOrderMsg& msg, const ATPRspErrorInfo& error_info, const int64_t request_id) {}

            /**
             * @brief 现货订单查询结果
             */
            virtual void OnRspCashOrderQueryResult(const ATPRspCashOrderQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief 现货股份查询结果
             */
            virtual void OnRspCashShareQueryResult(const ATPRspCashShareQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief 现货按证券账户划转资金响应
             */
            virtual void OnRspCashExternalInsTETransFundResp(const ATPRspCashExternalInsTETransFundRespMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info) {}
            /**
             * @brief 现货资金划拨(转入、转出)应答函数
             */
            virtual void OnRspCashExtFundTransferResult(const ATPRspCashExtFundTransferResultOtherMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info) {}
            /**
             * @brief 现货成交查询结果
             */
            virtual void OnRspCashTradeOrderQueryResult(const ATPRspCashTradeOrderQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief 现货证券信息查询结果消息
             */
            virtual void OnRspCashExtQueryResultSecurityInfo(const ATPRspCashExtQueryResultSecurityInfoMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief 现货资金查询结果
             */
            virtual void OnRspCashFundQueryResult(const ATPRspCashFundQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief 现货资产查询结果
             */
            virtual void OnRspCashAssetQueryResult(const ATPRspCashAssetQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief 现货成交汇总查询结果
             */
            virtual void OnRspCashTradeCollectQueryResult(const ATPRspCashCollectQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief 现货最大可委托数查询结果
             */
            virtual void OnRspCashMaxOrderQueryResult(const ATPRspCashMaxOrderQueryResultMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief ETF信息查询结果
             */
            virtual void OnRspExtQueryResultETFInfo(const ATPRspExtQueryResultETFInfoMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
            /**
             * @brief ETF成分股信息查询结果
             */
            virtual void OnRspExtQueryResultETFComponentInfo(const ATPRspExtQueryResultETFComponentInfoMsg& msg, const int64_t request_id, const ATPRspErrorInfo& error_info, const bool isLast) {}
        };

        class QUANT_API ATPQuantAPI
        {
        public:
            /**
             * @brief 构造函数
             *
             * @param[in]  handler    回调句柄
             * @param[in]  properties 配置参数
             *
             *              "locations"                             网关地址，const char*，必填，格式为"ip:port;ip:port"
             *              "order_way"                             委托方式，char，必填
             *              "client_feature_code"                   终端识别码，const char*，必填
             *              "callback_resource_mode"                回调线程资源配置模式，ATPCallbackResourceModeType，非必填，默认值 kLowLatencyMode-低时延模式
             *              "callback_thread_mode"                  回调线程模型，ATPThreadModelType，非必填，默认值 kShared-共享
             *              "group_id"                              回调线程组，uint8_t，非必填，默认值 0
             *              "min_resident_micro"                    回调线程休眠时间，uint32_t，单位μs，非必填，默认值 1
             *              "recevie_thread_cpu"                    回调线程绑定CPU，uint8_t，非必填，默认值0xFF 表示不绑定
             *              "is_tcp_direct"                         是否开启tcp_direct，bool，非必填，默认值false 表示不开启
             *              "bind_ip_address"                       绑定本地网卡地址，const char*，如果启用TcpDirect或RUDP协议，则该key值必填，默认值 空 表示不绑定
             *              "connection_protocol"                   协议类型，uint8_t，非必填，默认值kTCPProtocol-TCP协议
             *              "agw_user"                              网关用户名，const char*，非必填
             *                                                      网关用户名为空时表示匿名登录模式，Login只能登录一个客户号
             *                                                      网关用户名不为空时表示网关用户登录模式，可以多次调用Login登录多个客户号
             *              "agw_password"                          网关用户密码，const char*，非必填
             *              "retransmit_mode"                       重传模式，uint8_t，非必填，默认值kQuickMode-Quick模式 表示不重传历史RTN消息。1-Restart模式，重传所有历史RTN消息
             *              "multi_channel_flag"                    多通道自主订阅标志，uint8_t，默认值kDefault 表示以后台开关为准
             */
            ATPQuantAPI(const ATPQuantHandler* handler, const ATPProperties* properties);

            /**
             * @brief 析构函数
             */
            ~ATPQuantAPI();

            /**
             * @brief 禁用拷贝构造
             */
            ATPQuantAPI(const ATPQuantAPI&) = delete;
            /**
             * @brief 禁用赋值构造
             */
            ATPQuantAPI& operator=(const ATPQuantAPI&) = delete;

            /**
             * @brief      初始化
             *
             * @note       只需要调用一次，非线程安全
             * @param[in]  properties 配置参数
             *
             *             "log_level"                            业务日志级别，ATPLogLevelType，非必填，默认值kInfo
             *             "common_log_path"                      业务日志路径，const char*，非必填，默认值 空 表示可执行文件当前目录的 ./log/quant_api_common_log_yyyymmdd.log
             *             "indicator_log_path"                   指标日志路径，const char*，非必填，默认值 空 表示可执行文件当前目录的 ./log/quant_api_indicator_log_yyyymmdd.log
             *             "io_log_path"                          IO日志路径，const char*，非必填，默认值 空 表示可执行文件当前目录的 ./log/quant_api_io_log_yyyymmdd.log
             *             "is_enable_latency"                    是否启用时延统计功能，bool，非必填，默认值false 表示不启用
             *
             * @return     ErrorCode 错误码
             */
            static ATPErrorCodeType  Init(const ATPProperties* properties);

            /**
             * @brief 停止
             *
             * @note 初始化成功后才能调用，调用之后会停止初始化中启动的资源和服务，之后API对象都将不能再发送消息，只能调用一次
             */
            static ATPErrorCodeType Stop();

            /**
             * @brief  登录接口
             * @param[in]  property  登录属性
             *
             * @return ErrorCode 错误码
             */
            ATPErrorCodeType  Login(const ATPLoginProperty* property);

            /**
             * @brief  登出接口
             * @return ErrorCode 错误码
             */
            ATPErrorCodeType  Logout();

            /**
             * @brief  释放资源
             * @return ErrorCode 错误码
             */
            ATPErrorCodeType  Release();

            /**
             * @brief       获取当前交易用户账号信息
             * @param[in]   const char* 客户号ID
             *              匿名连接模式下，返回当前连接登录的客户信息
             *              网关用户模式下，传入时返回对应的客户信息，未传入时返回最后一次登录成功的客户信息
             * @note        如果api未处于登录成功，且会话保持状态，则该函数返回空指针
             * @return      账号信息
             */
            ATPCustomerInfo* GetCustomerInfo(const char* cust_id = "") const;

            /**
             * @brief       获取API版本号
             * @return      版本信息
             */
            const char* GetVersion() const;

            /**
             * @brief       获取当前对象实例ID
             */
            int32_t GetInstanceId() const;

            /**
             * @brief       获取终端识别码
             *
             * @return      处理完成的终端识别码
             */
            static const char* GetClientFeatureCode(const ClientFeatureCode* client_feature_code);

            /**
             * @brief       获取当前用户连接信息
             *
             * @return      用户连接信息
             */
            ATPConnectionInfo* GetConnectionInfo() const;

        public:
            /**
             * @brief 现货通用下单
             */
            ATPErrorCodeType ReqCashAuctionOrder(const ATPReqCashAuctionOrderMsg* msg, const int64_t request_id);
            /**
             * @brief 现货通用撤单
             */
            ATPErrorCodeType ReqCashCancelOrder(const ATPReqCashCancelOrderMsg* msg, const int64_t request_id);
            /**
             * @brief 现货订单查询
             */
            ATPErrorCodeType ReqCashOrderQuery(const ATPReqCashOrderQueryMsg* msg, const int64_t request_id);
            /**
             * @brief 现货股份查询
             */
            ATPErrorCodeType ReqCashShareQuery(const ATPReqCashShareQueryMsg* msg, const int64_t request_id);
            /**
             * @brief 现货按证券账户划转资金
             */
            ATPErrorCodeType ReqCashExternalInsTETransFundReq(const ATPReqCashExternalInsTETransFundReqMsg* msg, const int64_t request_id);
            /**
             * @brief 现货资金划拨
             */
            ATPErrorCodeType ReqCashExtFundTransferOther(const ATPReqCashExtFundTransferOtherMsg* msg, const int64_t request_id);
            /**
             * @brief 现货成交查询
             */
            ATPErrorCodeType ReqCashTradeOrderQuery(const ATPReqCashTradeOrderQueryMsg* msg, const int64_t request_id);
            /**
             * @brief 现货证券信息查询消息
             */
            ATPErrorCodeType ReqCashExtQuerySecurityInfo(const ATPReqCashExtQuerySecurityInfoMsg* msg, const int64_t request_id);
            /**
             * @brief 现货资金查询
             */
            ATPErrorCodeType ReqCashFundQuery(const ATPReqCashFundQueryMsg* msg, const int64_t request_id);
            /**
             * @brief 现货资产查询
             */
            ATPErrorCodeType ReqCashAssetQuery(const ATPReqCashAssetQueryMsg* msg, const int64_t request_id);
            /**
             * @brief 现货成交汇总查询
             */
            ATPErrorCodeType ReqCashTradeCollectQuery(const ATPReqCashTradeCollectQueryMsg* msg, const int64_t request_id);
            /**
             * @brief 现货最大可委托数查询
             */
            ATPErrorCodeType ReqCashMaxOrderQtyQuery(const ATPReqCashMaxOrderQtyQueryMsg* msg, const int64_t request_id);
            /**
             * @brief ETF信息查询
             */
            ATPErrorCodeType ReqExtQueryETFInfo(const ATPReqExtQueryETFInfoMsg* msg, const int64_t request_id);
            /**
             * @brief ETF成分股信息查询
             */
            ATPErrorCodeType ReqExtQueryETFComponentInfo(const ATPReqExtQueryETFComponentInfoMsg* msg, const int64_t request_id);

        private:
            atp::quant_api::Context* context_;
        };
    } //namespace quant_api
} //namespace atp




#endif    //ATP_QUANT_API_H_