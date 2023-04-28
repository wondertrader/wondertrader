// code_generator_tag
// requests:5f024b1066515f963731eec331bd9d61 responses:b0e958b18fde3671566c702f3c2a40de constants:3c699dd546cc3a70c1497ce1c443e2cc types:77cca557b8ed3391410ffcd7964e7093 trade_messages:3d837391385002e580b6d269eb6d2c26 template:cbd7ccf06b1731da577df564fc12dad1 code:6aebb897521436000d87bdda869bcd55
/**
 * Copyright (c) 2018 Archforce Financial Technology.  All rights reserved.
 * Redistribution and use in source and binary forms, with or without  modification, are not permitted.
 * For more information about Archforce, welcome to archforce.cn.
 *
 * @file atp_quant_msg.h
 * @author Archforce
 * @brief message
 *
 * @note ATPQuantAPI消息定义
 */
#ifndef ATP_QUANT_MSG_H_
#define ATP_QUANT_MSG_H_
#include <atp_quant_constant.h>

//前置声明
namespace atp
{
    namespace api
    {
        class PropertiesImpl;
    }
}

namespace atp
{
    namespace quant_api
    {
        /**
         * @brief 二进制数据流结构
         */
        class QUANT_API Buffer
        {
        public:
            Buffer() : data_size(0), data(nullptr) {}

            Buffer(const uint32_t data_size, const char* data) : data_size(data_size), data(data) {}

        public:
            uint32_t data_size;     // 二进制数据流长度
            const char* data;       // 二进制数据流首地址
        };

        /**
         * @brief 配置属性类
         */
        class QUANT_API ATPProperties
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPProperties();

            /**
             * @brief 析构函数
             */
            ~ATPProperties();

            ATPProperties(const ATPProperties&) = delete;

            ATPProperties& operator=(const ATPProperties&) = delete;

            void SetValue(const char* key, const char* value);

            void SetValue(const char* key, bool value);

            void SetValue(const char* key, char value);

            void SetValue(const char* key, uint8_t value);

            void SetValue(const char* key, int8_t value);

            void SetValue(const char* key, uint16_t value);

            void SetValue(const char* key, int16_t value);

            void SetValue(const char* key, uint32_t value);

            void SetValue(const char* key, int32_t value);

            void SetValue(const char* key, uint64_t value);

            void SetValue(const char* key, int64_t value);

            const char* GetValue(const char* key, const char* default_value) const;

            bool GetValue(const char* key, bool default_value) const;

            char GetValue(const char* key, char default_value) const;

            uint8_t GetValue(const char* key, uint8_t default_value) const;

            int8_t GetValue(const char* key, int8_t default_value) const;

            uint16_t GetValue(const char* key, uint16_t default_value) const;

            int16_t GetValue(const char* key, int16_t default_value) const;

            uint32_t GetValue(const char* key, uint32_t default_value) const;

            int32_t GetValue(const char* key, int32_t default_value) const;

            uint64_t GetValue(const char* key, uint64_t default_value) const;

            int64_t GetValue(const char* key, int64_t default_value) const;

            bool HasKey(const char* key) const;

        private:
            atp::api::PropertiesImpl* impl_;
        };

        /**
         * @brief 错误信息
         */
        class QUANT_API ATPRspErrorInfo
        {
        public:
            /**
             * @brief 构造方法
             */
            ATPRspErrorInfo() : error_id(0) {}

            /**
             * @brief 析构方法
             */
            virtual ~ATPRspErrorInfo() = default;

            /**
             * @brief 获取错误描述
             */
            virtual const char* GetErrorMsg() const = 0;

        public:
            int32_t error_id;                //错误码
        };

        /**
         * @brief 终端信息
         */
        class QUANT_API ClientFeatureCode
        {
        public:
            /**
             * @brief 构造方法
             */
            ClientFeatureCode() = default;

            /**
             * @brief 析构方法
             */
            virtual ~ClientFeatureCode() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ClientFeatureCode(const ClientFeatureCode&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ClientFeatureCode& operator=(const ClientFeatureCode&) = delete;

            /**
             * @brief 构建消息
             */
            static ClientFeatureCode* NewMessage();

            /**
             * @brief 销毁消息
             */
            static void DeleteMessage(ClientFeatureCode* ptr);

        public:
            //终端类型
            virtual void SetTerminalType(const char* terminal_type) = 0;
            virtual const char* GetTerminalType() const = 0;
            //公网IP地址
            virtual void SetIip(const char* iip) = 0;
            virtual const char* GetIip() const = 0;
            //公网端口
            virtual void SetIport(const char* iport) = 0;
            virtual const char* GetIport() const = 0;
            //内网IP地址
            virtual void SetLip(const char* lip) = 0;
            virtual const char* GetLip() const = 0;
            //MAC地址
            virtual void SetMac(const char* mac) = 0;
            virtual const char* GetMac() const = 0;
            //硬盘序列号
            virtual void SetHd(const char* hd) = 0;
            virtual const char* GetHd() const = 0;
            //PC终端设备名
            virtual void SetPcn(const char* pcn) = 0;
            virtual const char* GetPcn() const = 0;
            //CPU序列号
            virtual void SetCpu(const char* cpu) = 0;
            virtual const char* GetCpu() const = 0;
            //硬盘分区信息
            virtual void SetPi(const char* pi) = 0;
            virtual const char* GetPi() const = 0;
            //系统盘卷标号
            virtual void SetVol(const char* vol) = 0;
            virtual const char* GetVol() const = 0;
            //交易终端软件名称及版本
            virtual void SetTername(const char* tername) = 0;
            virtual const char* GetTername() const = 0;
        };

        /**
         * @brief 登录消息
         */
        class QUANT_API ATPLoginProperty
        {
        public:
            /**
             * @brief 构造方法
             */
            ATPLoginProperty() = default;

            /**
             * @brief 析构方法
             */
            virtual ~ATPLoginProperty() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPLoginProperty(const ATPLoginProperty&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPLoginProperty& operator=(const ATPLoginProperty&) = delete;

            /**
             * @brief 构建消息
             */
            static ATPLoginProperty* NewMessage();

            /**
             * @brief 销毁消息
             */
            static void DeleteMessage(ATPLoginProperty* ptr);

        public:
            //账户ID 客户号登录模式下填客户号，资金账号登录模式下填资金账号
            virtual void SetUserId(const char* user_id) = 0;
            virtual const char* GetUserId() const = 0;
            //营业部ID
            virtual void SetBranchId(const char* branch_id) = 0;
            virtual const char* GetBranchId() const = 0;
            //客户密码
            virtual void SetPassword(const char* password) = 0;
            virtual const char* GetPassword() const = 0;
            //登录模式
            virtual void SetLoginMode(uint8_t login_mode) = 0;
            virtual uint8_t GetLoginMode() const = 0;
            //扩展字段
            virtual void SetExtraData(const char* extra_data) = 0;
            virtual const char* GetExtraData() const = 0;
        };

        /**
         * @brief 交易用户信息
         */
        class QUANT_API ATPCustomerInfo
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPCustomerInfo() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPCustomerInfo() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPCustomerInfo(const ATPCustomerInfo&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPCustomerInfo& operator=(const ATPCustomerInfo&) = delete;

        public:
            //客户号
            virtual const char* GetCustId() const = 0;
            //是否为多分区账号
            virtual uint8_t GetIsMultiPartitions() const = 0;
            //资金账号重复组大小
            virtual uint32_t FundAccountArraySize() const = 0;
            //资金账号
            virtual const char* FundAccountArray_GetFundAccountId(uint32_t index) const = 0;
            //营业部ID
            virtual const char* FundAccountArray_GetBranchId(uint32_t index) const = 0;
            //证券账号重复组大小
            virtual uint32_t FundAccountArray_AccountArraySize(uint32_t index) const = 0;
            //证券账号
            virtual const char* FundAccountArray_AccountArray_GetAccountId(uint32_t index1, uint32_t index2) const = 0;
            //市场代码
            virtual uint16_t FundAccountArray_AccountArray_GetMarketId(uint32_t index1, uint32_t index2) const = 0;
            //账户角色
            virtual uint8_t FundAccountArray_AccountArray_GetAccountRole(uint32_t index1, uint32_t index2) const = 0;
            //所属分区号
            virtual uint8_t FundAccountArray_AccountArray_GetPartitionNo(uint32_t index1, uint32_t index2) const = 0;
        };

        /**
         * @brief 连接信息
         */
        class QUANT_API ATPConnectionInfo
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPConnectionInfo() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPConnectionInfo() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPConnectionInfo(const ATPConnectionInfo&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPConnectionInfo& operator=(const ATPConnectionInfo&) = delete;

        public:
            //实例ID
            virtual int32_t GetInstanceId() const = 0;
            //多通道主推消息订阅结果
            virtual uint8_t GetMultiChannelResult() const = 0;
        };



        /**
         * @brief 现货交易委托消息
         */
        class QUANT_API ATPReqCashAuctionOrderMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashAuctionOrderMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashAuctionOrderMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashAuctionOrderMsg(const ATPReqCashAuctionOrderMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashAuctionOrderMsg& operator=(const ATPReqCashAuctionOrderMsg&) = delete;
            /**
             * @brief 构建消息
             * @param business_type 业务类型
             */
            static ATPReqCashAuctionOrderMsg* NewMessage(ATPBusinessTypeType business_type);

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashAuctionOrderMsg* msg_ptr);

            /**
             * @brief 将消息结构编码为二进制数据流
             * @return Buffer 二进制数据流结构
             */
            virtual Buffer Encode() = 0;

            /**
             * @brief 将二进制数据流解码为消息结构
             * @param[in] const Buffer& 二进制数据流结构
             * @return bool 解码是否成功
             */
            virtual bool Decode(const Buffer&) = 0;

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账号ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID 
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //客户密码 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //证券代码 （当为配股业务时，该字段表示配售权证代码）
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //市场代码 
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //买卖方向 
            virtual void SetSide(char) = 0;
            virtual char GetSide() const = 0;
            //申报数量N15(2) 
            virtual void SetOrderQty(double) = 0;
            virtual double GetOrderQty() const = 0;
            //委托价格N13(4) 
            virtual void SetPrice(double) = 0;
            virtual double GetPrice() const = 0;
            //订单类型 
            virtual void SetOrderType(char) = 0;
            virtual char GetOrderType() const = 0;
            //客户自定义委托批号(0为系统保留值，不允许使用)(ATP3.2.3开始支持，且仅支持现货集中竞价)
            virtual void SetBatchClOrdNo(uint64_t) = 0;
            virtual uint64_t GetBatchClOrdNo() const = 0;
        };
        /**
         * @brief 现货订单委托响应
         */
        class QUANT_API ATPRspCashAuctionOrderMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashAuctionOrderMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashAuctionOrderMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashAuctionOrderMsg(const ATPRspCashAuctionOrderMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashAuctionOrderMsg& operator=(const ATPRspCashAuctionOrderMsg&) = delete;
        public:
            /**
             * @brief 构建消息
             */
            static ATPRspCashAuctionOrderMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPRspCashAuctionOrderMsg* msg_ptr);

            /**
             * @brief 将消息结构编码为二进制数据流
             * @return Buffer 二进制数据流结构
             */
            virtual Buffer Encode() const = 0;

            /**
             * @brief 将二进制数据流解码为消息结构
             * @param[in] const Buffer& 二进制数据流结构
             * @return bool 解码是否成功
             */
            virtual bool Decode(const Buffer&) = 0;

            //客户号ID
            virtual const char* GetCustId() const = 0;
            //资金账号ID
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //证券代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //市场代码
            virtual uint16_t GetMarketId() const = 0;
            //客户订单编号
            virtual int64_t GetClOrdNo() const = 0;
            //客户自定义委托批号（ATP3.2.3开始支持，且仅支持现货集中竞价）
            virtual uint64_t GetBatchClOrdNo() const = 0;
            //委托价格N13(4)
            virtual double GetPrice() const = 0;
            //委托数量N15(2)
            virtual double GetOrderQty() const = 0;
            //买卖方向
            virtual char GetSide() const = 0;
            //业务类型
            virtual uint8_t GetBusinessType() const = 0;
            //订单类型
            virtual char GetOrderType() const = 0;
            //响应时间
            virtual int64_t GetTransactTime() const = 0;
        };
        /**
         * @brief 现货撤单委托响应
         */
        class QUANT_API ATPRspCashCancelOrderMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashCancelOrderMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashCancelOrderMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashCancelOrderMsg(const ATPRspCashCancelOrderMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashCancelOrderMsg& operator=(const ATPRspCashCancelOrderMsg&) = delete;
        public:
            /**
             * @brief 构建消息
             */
            static ATPRspCashCancelOrderMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPRspCashCancelOrderMsg* msg_ptr);

            /**
             * @brief 将消息结构编码为二进制数据流
             * @return Buffer 二进制数据流结构
             */
            virtual Buffer Encode() const = 0;

            /**
             * @brief 将二进制数据流解码为消息结构
             * @param[in] const Buffer& 二进制数据流结构
             * @return bool 解码是否成功
             */
            virtual bool Decode(const Buffer&) = 0;

            //客户号ID
            virtual const char* GetCustId() const = 0;
            //资金账号ID
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //客户订单编号
            virtual int64_t GetClOrdNo() const = 0;
            //原委托的客户订单编号
            virtual int64_t GetOrigClOrdNo() const = 0;
            //响应时间
            virtual int64_t GetTransactTime() const = 0;
            //客户自定义委托批号，指示被撤消订单的batch_cl_ord_no。（ATP3.2.3开始支持，且仅支持现货集中竞价）
            virtual uint64_t GetBatchClOrdNo() const = 0;
        };
        /**
         * @brief 现货主推消息
         */
        class QUANT_API ATPRtnCashAuctionOrderMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRtnCashAuctionOrderMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRtnCashAuctionOrderMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRtnCashAuctionOrderMsg(const ATPRtnCashAuctionOrderMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRtnCashAuctionOrderMsg& operator=(const ATPRtnCashAuctionOrderMsg&) = delete;
        public:
            /**
             * @brief 构建消息
             */
            static ATPRtnCashAuctionOrderMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPRtnCashAuctionOrderMsg* msg_ptr);

            /**
             * @brief 将消息结构编码为二进制数据流
             * @return Buffer 二进制数据流结构
             */
            virtual Buffer Encode() const = 0;

            /**
             * @brief 将二进制数据流解码为消息结构
             * @param[in] const Buffer& 二进制数据流结构
             * @return bool 解码是否成功
             */
            virtual bool Decode(const Buffer&) = 0;

            //客户号ID 
            virtual const char* GetCustId() const = 0;
            //资金账号ID 
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID 
            virtual const char* GetAccountId() const = 0;
            //业务类型 
            virtual uint8_t GetBusinessType() const = 0;
            //客户订单编号 
            virtual int64_t GetClOrdNo() const = 0;
            //客户自定义委托批号（ATP3.2.3开始支持，且仅支持现货集中竞价）
            virtual uint64_t GetBatchClOrdNo() const = 0;
            //证券代码 
            virtual const char* GetSecurityId() const = 0;
            //市场代码 
            virtual uint16_t GetMarketId() const = 0;
            //订单状态 
            virtual uint8_t GetOrdStatus() const = 0;
            //订单标识 
            virtual uint8_t GetOrdSign() const = 0;
            //委托价格N13(4) 
            virtual double GetPrice() const = 0;
            //委托数量N15(2) 
            virtual double GetOrderQty() const = 0;
            //未成交部分的数量N15(2) 
            virtual double GetLeavesQty() const = 0;
            //累计成交数量N15(2) 
            virtual double GetCumQty() const = 0;
            //买卖方向 
            virtual char GetSide() const = 0;
            //委托时间 
            virtual int64_t GetTransactTime() const = 0;
            //交易所订单编号 
            virtual const char* GetOrderId() const = 0;
            //申报合同号 
            virtual const char* GetClOrdId() const = 0;
            //冻结交易金额N15(4)
            virtual double GetFrozenTradeValue() const = 0;
            //冻结费用N15(4)
            virtual double GetFrozenFee() const = 0;
            //成交编号
            virtual const char* GetExecId() const = 0;
            //成交价格N13(4)
            virtual double GetLastPx() const = 0;
            //成交数量N15(2)
            virtual double GetLastQty() const = 0;
            //成交金额N15(4)
            virtual double GetTotalValueTraded() const = 0;
            //ETF成交回报类型
            virtual uint8_t GetEtfTradeReportType() const = 0;
            //信用标识
            virtual char GetCashMargin() const = 0;
            //到期日
            virtual int64_t GetMaturityDate() const = 0;
            //原委托的客户订单编号
            virtual int64_t GetOrigClOrdNo() const = 0;
            //母单订单编号
            virtual int64_t GetParentClOrdNo() const = 0;
            //拒绝原因代码
            virtual int32_t GetRejectReasonCode() const = 0;
            //拒绝原因描述
            virtual const char* GetOrdRejReason() const = 0;
        };
        /**
         * @brief 现货通用撤单消息
         */
        class QUANT_API ATPReqCashCancelOrderMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashCancelOrderMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashCancelOrderMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashCancelOrderMsg(const ATPReqCashCancelOrderMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashCancelOrderMsg& operator=(const ATPReqCashCancelOrderMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashCancelOrderMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashCancelOrderMsg* msg_ptr);

            /**
             * @brief 将消息结构编码为二进制数据流
             * @return Buffer 二进制数据流结构
             */
            virtual Buffer Encode() = 0;

            /**
             * @brief 将二进制数据流解码为消息结构
             * @param[in] const Buffer& 二进制数据流结构
             * @return bool 解码是否成功
             */
            virtual bool Decode(const Buffer&) = 0;

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID 
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //原始交易客户方（券商）订单编号，指示被撤消订单的cl_ord_no（ATP3.2.3开始支持现货集中竞价非必填，其他业务必填）
            virtual void SetOrigClOrdNo(int64_t) = 0;
            virtual int64_t GetOrigClOrdNo() const = 0;
            //客户自定义委托批号，指示被撤消订单的batch_cl_ord_no。batch_cl_ord_no与orig_cl_ord_no至少填写一项。若orig_cl_ord_no不为0，则按orig_cl_ord_no撤单；否则，batch_cl_ord_no不为0，按batch_cl_ord_no撤单（ATP3.2.3开始支持，且仅支持现货集中竞价）
            virtual void SetBatchClOrdNo(uint64_t) = 0;
            virtual uint64_t GetBatchClOrdNo() const = 0;
            //市场代码，开通北交所业务后必填（ATP V3.4版本开始支持）
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
        };
        /**
         * @brief 现货订单查询消息
         */
        class QUANT_API ATPReqCashOrderQueryMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashOrderQueryMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashOrderQueryMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashOrderQueryMsg(const ATPReqCashOrderQueryMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashOrderQueryMsg& operator=(const ATPReqCashOrderQueryMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashOrderQueryMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashOrderQueryMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID 
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //客户订单编号，0表示查所有
            virtual void SetClOrdNo(int64_t) = 0;
            virtual int64_t GetClOrdNo() const = 0;
            //市场代码，0表示查所有。开通北交所业务后必填 
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //证券代码，空字符串表示查所有
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //业务类型，0表示查所有
            virtual void SetBusinessType(uint8_t) = 0;
            virtual uint8_t GetBusinessType() const = 0;
            //买卖方向，'0'表示查所有
            virtual void SetSide(char) = 0;
            virtual char GetSide() const = 0;
            //委托查询条件，0表示查所有
            virtual void SetOrderQueryCondition(uint8_t) = 0;
            virtual uint8_t GetOrderQueryCondition() const = 0;
            //查询返回数量，0表示按能返回的最大数量返回，具体数量请咨询券商
            virtual void SetReturnNum(int64_t) = 0;
            virtual int64_t GetReturnNum() const = 0;
            //返回顺序
            virtual void SetReturnSeq(uint8_t) = 0;
            virtual uint8_t GetReturnSeq() const = 0;
            //客户自定义委托批号(0为系统保留值，不允许使用)(ATP3.2.3开始支持，且仅支持现货集中竞价)
            virtual void SetBatchClOrdNo(uint64_t) = 0;
            virtual uint64_t GetBatchClOrdNo() const = 0;
        };
        /**
         * @brief 现货订单查询结果
         */
        class QUANT_API ATPRspCashOrderQueryResultMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashOrderQueryResultMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashOrderQueryResultMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashOrderQueryResultMsg(const ATPRspCashOrderQueryResultMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashOrderQueryResultMsg& operator=(const ATPRspCashOrderQueryResultMsg&) = delete;
        public:
            //终端识别码（ATP3.2.3 开始支持）
            virtual const char* GetClientFeatureCode() const = 0;
            //业务类型
            virtual uint8_t GetBusinessType() const = 0;
            //证券代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //市场代码
            virtual uint16_t GetMarketId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //买卖方向
            virtual char GetSide() const = 0;
            //订单类别
            virtual char GetOrdType() const = 0;
            //订单标识 
            virtual uint8_t GetOrdSign() const = 0;
            //当前申报的状态（对于撤单订单，填写被撤订单的当前状态，找不到原始订单时，固定填8 = Reject ）
            virtual uint8_t GetOrdStatus() const = 0;
            //申报状态详细信息
            virtual const char* GetOrderStatusInfo() const = 0;
            //客户委托时间
            virtual int64_t GetTransactTime() const = 0;
            //委托价格N13(4)
            virtual double GetOrderPrice() const = 0;
            //平均成交价格N13(4)
            virtual double GetExecPrice() const = 0;
            //委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
            virtual double GetOrderQty() const = 0;
            //未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
            virtual double GetLeavesQty() const = 0;
            //累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
            virtual double GetCumQty() const = 0;
            //客户订单编号
            virtual int64_t GetClOrdNo() const = 0;
            //交易所订单编号
            virtual const char* GetOrderId() const = 0;
            //申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓；
            virtual const char* GetClOrdId() const = 0;
            //对于撤单订单，为原始交易客户方（券商）订单编号，指示被撤消订单的cl_ord_no; 对于普通订单，取值为0
            virtual int64_t GetOrigClOrdNo() const = 0;
            //冻结交易金额N15(4)
            virtual double GetFrozenTradeValue() const = 0;
            //冻结费用N15(4)
            virtual double GetFrozenFee() const = 0;
            //货币种类
            virtual const char* GetCurrency() const = 0;
            //委托金额N15(4)
            virtual double GetOrderEntrustedAmt() const = 0;
            //成交金额N15(4)
            virtual double GetOrderCumTransactionAmt() const = 0;
            //执行类型（ATP3.1.8 开始支持）
            virtual char GetExecType() const = 0;
            //证券类别（ATP3.1.8 开始支持）
            virtual uint16_t GetSecurityType() const = 0;
            //已撤单数量N15(2)（ATP3.1.9 开始支持）
            virtual double GetCanceledQty() const = 0;
            //订单标识（ATP3.1.9 开始支持）（返回默认值表示当前版本不支持）
            virtual char GetOrderFlag() const = 0;
            //客户号ID 
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual const char* GetFundAccountId() const = 0;
            //客户自定义委托批号，对于撤单订单，为原订单的客户自定义委托批号。（ATP3.2.3开始支持，且仅支持现货集中竞价）
            virtual uint64_t GetBatchClOrdNo() const = 0;
        };
        /**
         * @brief 现货股份查询
         */
        class QUANT_API ATPReqCashShareQueryMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashShareQueryMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashShareQueryMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashShareQueryMsg(const ATPReqCashShareQueryMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashShareQueryMsg& operator=(const ATPReqCashShareQueryMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashShareQueryMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashShareQueryMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID 
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //业务类型，0表示查所有
            virtual void SetBusinessType(uint8_t) = 0;
            virtual uint8_t GetBusinessType() const = 0;
            //市场代码，0表示查所有。开通北交所业务后必填
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //证券代码，空字符串表示查所有
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //查询返回数量，0表示按能返回的最大数量返回，具体数量请咨询券商
            virtual void SetReturnNum(int64_t) = 0;
            virtual int64_t GetReturnNum() const = 0;
        };
        /**
         * @brief 现货股份查询结果
         */
        class QUANT_API ATPRspCashShareQueryResultMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashShareQueryResultMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashShareQueryResultMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashShareQueryResultMsg(const ATPRspCashShareQueryResultMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashShareQueryResultMsg& operator=(const ATPRspCashShareQueryResultMsg&) = delete;
        public:
            //证券代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //市场代码
            virtual uint16_t GetMarketId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //资金账户ID
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID
            virtual const char* GetBranchId() const = 0;
            //日初持仓量N15(2)
            virtual double GetInitQty() const = 0;
            //剩余股份数量N15(2)
            virtual double GetLeavesQty() const = 0;
            //可用股份数量N15(2)
            virtual double GetAvailableQty() const = 0;
            //浮动盈亏N15(4)
            virtual double GetProfitLoss() const = 0;
            //市值N15(4)
            virtual double GetMarketValue() const = 0;
            //成本价N15(4)
            virtual double GetCostPrice() const = 0;
            //最新价N13(4)
            virtual double GetLastPrice() const = 0;
            //股份买入解冻
            virtual double GetStockBuy() const = 0;
            //股份卖出冻结
            virtual double GetStockSale() const = 0;
            //证券类别
            virtual uint16_t GetSecurityType() const = 0;
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //对于ETF基金表示可赎回数量，对于其他证券表示可用于ETF申购的数量（ATP3.2.1开始支持）
            virtual double GetEtfRedemptionQty() const = 0;
        };
        /**
         * @brief 现货按证券账户划转资金请求消息
         */
        class QUANT_API ATPReqCashExternalInsTETransFundReqMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashExternalInsTETransFundReqMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashExternalInsTETransFundReqMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashExternalInsTETransFundReqMsg(const ATPReqCashExternalInsTETransFundReqMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashExternalInsTETransFundReqMsg& operator=(const ATPReqCashExternalInsTETransFundReqMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashExternalInsTETransFundReqMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashExternalInsTETransFundReqMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID 
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //资金转出账户ID 
            virtual void SetFundOutAccountId(const char*) = 0;
            virtual const char* GetFundOutAccountId() const = 0;
            //资金转出市场代码
            virtual void SetFundOutMarketId(uint16_t) = 0;
            virtual uint16_t GetFundOutMarketId() const = 0;
            //资金转入账户ID 
            virtual void SetFundInAccountId(const char*) = 0;
            virtual const char* GetFundInAccountId() const = 0;
            //资金转入市场代码
            virtual void SetFundInMarketId(uint16_t) = 0;
            virtual uint16_t GetFundInMarketId() const = 0;
            //划拨金额 N15(4)
            virtual void SetValue(double) = 0;
            virtual double GetValue() const = 0;
        };
        /**
         * @brief 按证券账户划转资金响应消息
         */
        class QUANT_API ATPRspCashExternalInsTETransFundRespMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashExternalInsTETransFundRespMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashExternalInsTETransFundRespMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashExternalInsTETransFundRespMsg(const ATPRspCashExternalInsTETransFundRespMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashExternalInsTETransFundRespMsg& operator=(const ATPRspCashExternalInsTETransFundRespMsg&) = delete;
        public:
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //资金账户ID
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID
            virtual const char* GetBranchId() const = 0;
        };
        /**
         * @brief 现货资金划拨请求消息
         */
        class QUANT_API ATPReqCashExtFundTransferOtherMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashExtFundTransferOtherMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashExtFundTransferOtherMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashExtFundTransferOtherMsg(const ATPReqCashExtFundTransferOtherMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashExtFundTransferOtherMsg& operator=(const ATPReqCashExtFundTransferOtherMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashExtFundTransferOtherMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashExtFundTransferOtherMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //营业部ID（ATP3.1.9开始支持非必填）
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //划拨方式 ：1=转入；2=转出
            virtual void SetTransferWay(uint8_t) = 0;
            virtual uint8_t GetTransferWay() const = 0;
            //划拨金额 N13(2) 
            virtual void SetTransferValue(double) = 0;
            virtual double GetTransferValue() const = 0;
            //市场代码，开通北交所业务后必填（ATP V3.4版本开始支持）
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
        };
        /**
         * @brief 现货资金划拨(转入、转出)应答消息
         */
        class QUANT_API ATPRspCashExtFundTransferResultOtherMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashExtFundTransferResultOtherMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashExtFundTransferResultOtherMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashExtFundTransferResultOtherMsg(const ATPRspCashExtFundTransferResultOtherMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashExtFundTransferResultOtherMsg& operator=(const ATPRspCashExtFundTransferResultOtherMsg&) = delete;
        public:
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //回报时间
            virtual int64_t GetTransactTime() const = 0;
            //资金账户ID
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID
            virtual const char* GetBranchId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //货币种类
            virtual const char* GetCurrency() const = 0;
            //日初可用余额N15(4)
            virtual double GetInitAmt() const = 0;
            //委托预冻结N15(4)
            virtual double GetOrderFrozen() const = 0;
            //买入成交N15(4)
            virtual double GetBuyTrade() const = 0;
            //卖出成交N15(4)
            virtual double GetSellTrade() const = 0;
            //异常冻结N15(4)
            virtual double GetUnusualFrozen() const = 0;
            //异常冻结取消N15(4)
            virtual double GetUnusualFrozenCancel() const = 0;
            //冻结费用N15(4)
            virtual double GetFeeFrozen() const = 0;
            //成交费用N15(4)
            virtual double GetFeeTrade() const = 0;
            //当日入金N15(4)
            virtual double GetTodayIn() const = 0;
            //当日出金N15(4)
            virtual double GetTodayOut() const = 0;
            //临时调增N15(4)
            virtual double GetTempAdd() const = 0;
            //临时调减N15(4)
            virtual double GetTempSub() const = 0;
            //临时冻结N15(4)
            virtual double GetTempFrozen() const = 0;
            //临时冻结取消N15(4)
            virtual double GetTempFrozenCancel() const = 0;
            //调整前资金余额N15(4)
            virtual double GetPreBalance() const = 0;
            //调整前T+0可用资金N15(4)
            virtual double GetPreAvailableT0() const = 0;
            //调整前T+1在途可用资金N15(4)
            virtual double GetPreOnTheWayT1() const = 0;
            //转移金额N13(2)
            virtual double GetTransferValue() const = 0;
        };
        /**
         * @brief 现货成交查询消息
         */
        class QUANT_API ATPReqCashTradeOrderQueryMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashTradeOrderQueryMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashTradeOrderQueryMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashTradeOrderQueryMsg(const ATPReqCashTradeOrderQueryMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashTradeOrderQueryMsg& operator=(const ATPReqCashTradeOrderQueryMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashTradeOrderQueryMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashTradeOrderQueryMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //市场代码，0表示查所有。开通北交所业务后必填
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //证券代码，空字符串表示查所有
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //业务类型， 0表示查所有
            virtual void SetBusinessType(uint8_t) = 0;
            virtual uint8_t GetBusinessType() const = 0;
            //查询返回数量，0表示按能返回的最大数量返回，具体数量请咨询券商
            virtual void SetReturnNum(int64_t) = 0;
            virtual int64_t GetReturnNum() const = 0;
            //返回顺序，1表示按时间正序排序
            virtual void SetReturnSeq(uint8_t) = 0;
            virtual uint8_t GetReturnSeq() const = 0;
            //客户订单编号，0表示查所有
            virtual void SetClOrdNo(int64_t) = 0;
            virtual int64_t GetClOrdNo() const = 0;
            //报盘合同号 
            virtual void SetClOrdId(const char*) = 0;
            virtual const char* GetClOrdId() const = 0;
            //执行编号 
            virtual void SetExecId(const char*) = 0;
            virtual const char* GetExecId() const = 0;
            //客户自定义委托批号(0为系统保留值，不允许使用)(ATP3.2.3开始支持，且仅支持现货集中竞价)
            virtual void SetBatchClOrdNo(uint64_t) = 0;
            virtual uint64_t GetBatchClOrdNo() const = 0;
        };
        /**
         * @brief 现货成交查询结果
         */
        class QUANT_API ATPRspCashTradeOrderQueryResultMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashTradeOrderQueryResultMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashTradeOrderQueryResultMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashTradeOrderQueryResultMsg(const ATPRspCashTradeOrderQueryResultMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashTradeOrderQueryResultMsg& operator=(const ATPRspCashTradeOrderQueryResultMsg&) = delete;
        public:
            //业务类型
            virtual uint8_t GetBusinessType() const = 0;
            //证券代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //市场代码
            virtual uint16_t GetMarketId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //买卖方向
            virtual char GetSide() const = 0;
            //订单类别
            virtual char GetOrdType() const = 0;
            //执行类型（ATP3.1.8 开始支持）
            virtual char GetExecType() const = 0;
            //执行编号
            virtual const char* GetExecId() const = 0;
            //客户订单编号
            virtual int64_t GetClOrdNo() const = 0;
            //交易所订单编号
            virtual const char* GetOrderId() const = 0;
            //申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓；
            virtual const char* GetClOrdId() const = 0;
            //客户委托时间
            virtual int64_t GetTransactTime() const = 0;
            //成交价格N13(4)
            virtual double GetLastPx() const = 0;
            //成交数量N15(2)
            virtual double GetLastQty() const = 0;
            //成交金额N15(4)
            virtual double GetTotalValueTraded() const = 0;
            //成交费用N15(4)
            virtual double GetFee() const = 0;
            //货币种类
            virtual const char* GetCurrency() const = 0;
            //证券类别（ATP3.1.8 开始支持）
            virtual uint16_t GetSecurityType() const = 0;
            //ETF成交回报类型
            virtual uint8_t GetEtfTradeReportType() const = 0;
            //资金账户ID 
            virtual const char* GetFundAccountId() const = 0;
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //客户自定义委托批号
            virtual uint64_t GetBatchClOrdNo() const = 0;
        };
        /**
         * @brief 现货证券信息查询消息
         */
        class QUANT_API ATPReqCashExtQuerySecurityInfoMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashExtQuerySecurityInfoMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashExtQuerySecurityInfoMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashExtQuerySecurityInfoMsg(const ATPReqCashExtQuerySecurityInfoMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashExtQuerySecurityInfoMsg& operator=(const ATPReqCashExtQuerySecurityInfoMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashExtQuerySecurityInfoMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashExtQuerySecurityInfoMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //市场代码，0表示查所有
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //证券代码，空字符串表示查所有
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //查询返回数量，0表示按能返回的最大数量返回，具体数量请咨询券商
            virtual void SetReturnNum(int64_t) = 0;
            virtual int64_t GetReturnNum() const = 0;
        };
        /**
         * @brief 现货证券信息查询结果消息
         */
        class QUANT_API ATPRspCashExtQueryResultSecurityInfoMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashExtQueryResultSecurityInfoMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashExtQueryResultSecurityInfoMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashExtQueryResultSecurityInfoMsg(const ATPRspCashExtQueryResultSecurityInfoMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashExtQueryResultSecurityInfoMsg& operator=(const ATPRspCashExtQueryResultSecurityInfoMsg&) = delete;
        public:
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //资金账户ID
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //市场代码
            virtual uint16_t GetMarketId() const = 0;
            //证券代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //证券类别
            virtual uint16_t GetSecurityType() const = 0;
            //昨日收盘价N13(4)
            virtual double GetPrevClosePrice() const = 0;
            //涨停价N13(4)
            virtual double GetPriceUpperLimit() const = 0;
            //跌停价N13(4)
            virtual double GetPriceLowerLimit() const = 0;
            //最小变动价位N13(4)
            virtual double GetPriceTick() const = 0;
            //限价买数量下限N15(2)
            virtual double GetBuyQtyLowerLimit() const = 0;
            //限价卖数量下限N15(2)
            virtual double GetSellQtyLowerLimit() const = 0;
            //是否支持回转交易
            virtual bool GetDayTrading() const = 0;
            //涨跌幅限制
            virtual bool GetHasPriceLimit() const = 0;
            //证券状态
            virtual uint64_t GetSecurityStatus() const = 0;
            //限价买数量单位N15(2)(ATP3.3.0开始支持)
            virtual double GetBuyQtyUnit() const = 0;
            //限价卖数量单位N15(2)(ATP3.3.0开始支持)
            virtual double GetSellQtyUnit() const = 0;
            //市价买数量单位N15(2)(ATP3.3.0开始支持)
            virtual double GetMarketBuyQtyUnit() const = 0;
            //市价卖数量单位N15(2)(ATP3.3.0开始支持)
            virtual double GetMarketSellQtyUnit() const = 0;
            //市价买数量上限N15(2)(ATP3.1.10开始支持)
            virtual double GetMarketBuyQtyUpperLimit() const = 0;
            //市价买数量下限N15(2)(ATP3.1.10开始支持)
            virtual double GetMarketBuyQtyLowerLimit() const = 0;
            //市价卖数量上限N15(2)(ATP3.1.10开始支持)
            virtual double GetMarketSellQtyUpperLimit() const = 0;
            //市价卖数量下限N15(2)(ATP3.1.10开始支持)
            virtual double GetMarketSellQtyLowerLimit() const = 0;
            //限价买数量上限N15(2)(ATP3.1.10开始支持)
            virtual double GetBuyQtyUpperLimit() const = 0;
            //限价卖数量上限N15(2)(ATP3.1.10开始支持)
            virtual double GetSellQtyUpperLimit() const = 0;
        };
        /**
         * @brief 现货资金查询消息
         */
        class QUANT_API ATPReqCashFundQueryMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashFundQueryMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashFundQueryMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashFundQueryMsg(const ATPReqCashFundQueryMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashFundQueryMsg& operator=(const ATPReqCashFundQueryMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashFundQueryMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashFundQueryMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID （ATP3.1.7版本开始支持此字段非必填）
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //证券账户ID（ATP3.1.7版本开始支持此字段非必填）
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //货币种类 （ATP3.1.7起开始支持非必填）
            virtual void SetCurrency(const char*) = 0;
            virtual const char* GetCurrency() const = 0;
            //市场代码，开通北交所业务后必填（ATP V3.4版本开始支持）
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
        };
        /**
         * @brief 现货资金查询结果
         */
        class QUANT_API ATPRspCashFundQueryResultMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashFundQueryResultMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashFundQueryResultMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashFundQueryResultMsg(const ATPRspCashFundQueryResultMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashFundQueryResultMsg& operator=(const ATPRspCashFundQueryResultMsg&) = delete;
        public:
            //资金余额N15(4)
            virtual double GetLeavesValue() const = 0;
            //日初资金金额N15(4)
            virtual double GetInitLeavesValue() const = 0;
            //当前所有冻结N15(4)
            virtual double GetFrozenAll() const = 0;
            //当日可取资金N15(4)
            virtual double GetAvailableT0() const = 0;
            //当日可用资金N15(4)
            virtual double GetAvailableT1() const = 0;
            //货币种类
            virtual const char* GetCurrency() const = 0;
            //资金账户ID 
            virtual const char* GetFundAccountId() const = 0;
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
        };
        /**
         * @brief 现货资产查询消息
         */
        class QUANT_API ATPReqCashAssetQueryMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashAssetQueryMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashAssetQueryMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashAssetQueryMsg(const ATPReqCashAssetQueryMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashAssetQueryMsg& operator=(const ATPReqCashAssetQueryMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashAssetQueryMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashAssetQueryMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID 
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //客户密码（该字段是否必填请咨询券商） 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //货币种类 
            virtual void SetCurrency(const char*) = 0;
            virtual const char* GetCurrency() const = 0;
            //市场代码，开通北交所业务后必填（ATP V3.4版本开始支持）
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
        };
        /**
         * @brief 现货资产查询结果
         */
        class QUANT_API ATPRspCashAssetQueryResultMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashAssetQueryResultMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashAssetQueryResultMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashAssetQueryResultMsg(const ATPRspCashAssetQueryResultMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashAssetQueryResultMsg& operator=(const ATPRspCashAssetQueryResultMsg&) = delete;
        public:
            //资金余额,N15(4)
            virtual double GetLeavesValue() const = 0;
            //日初资金金额,N15(4)
            virtual double GetInitLeavesValue() const = 0;
            //当前所有冻结,N15(4)
            virtual double GetFrozenAll() const = 0;
            //当日可取资金,N15(4)
            virtual double GetAvailableT0() const = 0;
            //当日可用资金,N15(4)
            virtual double GetAvailableT1() const = 0;
            //资产总值,N15(4)
            virtual double GetTotalAsset() const = 0;
            //总市值,N15(4)
            virtual double GetMarketValue() const = 0;
            //买入冻结,N15(4)
            virtual double GetFundBuy() const = 0;
            //卖出解冻,N15(4)
            virtual double GetFundSale() const = 0;
            //资金账户ID 
            virtual const char* GetFundAccountId() const = 0;
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
        };
        /**
         * @brief 现货成交汇总查询消息
         */
        class QUANT_API ATPReqCashTradeCollectQueryMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashTradeCollectQueryMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashTradeCollectQueryMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashTradeCollectQueryMsg(const ATPReqCashTradeCollectQueryMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashTradeCollectQueryMsg& operator=(const ATPReqCashTradeCollectQueryMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashTradeCollectQueryMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashTradeCollectQueryMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //密码 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //市场代码，0表示查所有 
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //证券代码 
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //业务类型，0表示查所有
            virtual void SetBusinessType(uint8_t) = 0;
            virtual uint8_t GetBusinessType() const = 0;
        };
        /**
         * @brief 现货成交汇总查询结果
         */
        class QUANT_API ATPRspCashCollectQueryResultMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashCollectQueryResultMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashCollectQueryResultMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashCollectQueryResultMsg(const ATPRspCashCollectQueryResultMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashCollectQueryResultMsg& operator=(const ATPRspCashCollectQueryResultMsg&) = delete;
        public:
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //资金账户ID
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //买入均价N13(4)
            virtual double GetBuyAvgPrice() const = 0;
            //买入累计总量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
            virtual double GetBuyCumQty() const = 0;
            //买入成交总金额，精确到3位小数N15(4)
            virtual double GetBuyTotalValueTraded() const = 0;
            //卖出均价N13(4)
            virtual double GetSellAvgPrice() const = 0;
            //卖出累计总量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
            virtual double GetSellCumQty() const = 0;
            //卖出成交金额，精确到3位小数N15(4)
            virtual double GetSellTotalValueTraded() const = 0;
            //买卖合计成交金额，精确到3位小数N15(4)
            virtual double GetTotalValueTraded() const = 0;
            //证券代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //市场代码 
            virtual uint16_t GetMarketId() const = 0;
            //货币种类
            virtual const char* GetCurrency() const = 0;
            //证券类别（ATP3.1.8 开始支持）
            virtual uint16_t GetSecurityType() const = 0;
        };
        /**
         * @brief 现货最大可委托数查询消息
         */
        class QUANT_API ATPReqCashMaxOrderQtyQueryMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqCashMaxOrderQtyQueryMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqCashMaxOrderQtyQueryMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqCashMaxOrderQtyQueryMsg(const ATPReqCashMaxOrderQtyQueryMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqCashMaxOrderQtyQueryMsg& operator=(const ATPReqCashMaxOrderQtyQueryMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqCashMaxOrderQtyQueryMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqCashMaxOrderQtyQueryMsg* msg_ptr);

        public:
            //客户号ID 
            virtual void SetCustId(const char*) = 0;
            virtual const char* GetCustId() const = 0;
            //资金账户ID 
            virtual void SetFundAccountId(const char*) = 0;
            virtual const char* GetFundAccountId() const = 0;
            //证券账户ID 
            virtual void SetAccountId(const char*) = 0;
            virtual const char* GetAccountId() const = 0;
            //市场代码 
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //营业部ID 
            virtual void SetBranchId(const char*) = 0;
            virtual const char* GetBranchId() const = 0;
            //证券代码 
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //密码 
            virtual void SetPassword(const char*) = 0;
            virtual const char* GetPassword() const = 0;
            //业务类型 
            virtual void SetBusinessType(uint8_t) = 0;
            virtual uint8_t GetBusinessType() const = 0;
            //买卖方向 
            virtual void SetSide(char) = 0;
            virtual char GetSide() const = 0;
            //委托价格N13(4)(市价填0，科创版填保护限价) 
            virtual void SetPrice(double) = 0;
            virtual double GetPrice() const = 0;
            //订单类型 
            virtual void SetOrderType(char) = 0;
            virtual char GetOrderType() const = 0;
        };
        /**
         * @brief 现货最大可委托数查询结果
         */
        class QUANT_API ATPRspCashMaxOrderQueryResultMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspCashMaxOrderQueryResultMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspCashMaxOrderQueryResultMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspCashMaxOrderQueryResultMsg(const ATPRspCashMaxOrderQueryResultMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspCashMaxOrderQueryResultMsg& operator=(const ATPRspCashMaxOrderQueryResultMsg&) = delete;
        public:
            //客户号ID
            virtual const char* GetCustId() const = 0;
            //资金账户ID
            virtual const char* GetFundAccountId() const = 0;
            //营业部ID
            virtual const char* GetBranchId() const = 0;
            //证券账户ID
            virtual const char* GetAccountId() const = 0;
            //最大可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
            virtual double GetMaxOrderQty() const = 0;
            //理论可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
            virtual double GetTheoreticalOrderQty() const = 0;
        };
        /**
         * @brief ETF信息查询消息
         */
        class QUANT_API ATPReqExtQueryETFInfoMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqExtQueryETFInfoMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqExtQueryETFInfoMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqExtQueryETFInfoMsg(const ATPReqExtQueryETFInfoMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqExtQueryETFInfoMsg& operator=(const ATPReqExtQueryETFInfoMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqExtQueryETFInfoMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqExtQueryETFInfoMsg* msg_ptr);

        public:
            //市场代码，0表示查所有
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //证券代码，空字符串表示查所有
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //查询返回数量，0表示按能返回的最大数量返回，具体数量请咨询券商
            virtual void SetReturnNum(int64_t) = 0;
            virtual int64_t GetReturnNum() const = 0;
            //申赎类型（默认值为0，表示查所有）
            virtual void SetPrType(uint8_t) = 0;
            virtual uint8_t GetPrType() const = 0;
        };
        /**
         * @brief ETF信息查询结果
         */
        class QUANT_API ATPRspExtQueryResultETFInfoMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspExtQueryResultETFInfoMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspExtQueryResultETFInfoMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspExtQueryResultETFInfoMsg(const ATPRspExtQueryResultETFInfoMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspExtQueryResultETFInfoMsg& operator=(const ATPRspExtQueryResultETFInfoMsg&) = delete;
        public:
            //市场代码
            virtual uint16_t GetMarketId() const = 0;
            //ETF代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //证券类别
            virtual uint16_t GetSecurityType() const = 0;
            //申赎类型（1-申赎，2-实物申赎）
            virtual uint8_t GetPrType() const = 0;
            //是否有PD券商资格
            virtual bool GetIsPdBroker() const = 0;
            //是否允许申购
            virtual bool GetIsPurchase() const = 0;
            //是否允许赎回
            virtual bool GetIsRedemption() const = 0;
            //ETF申购赎回单位N15(2)
            virtual double GetPurchaseRedemptionUnit() const = 0;
            //ETF股票篮现金差额N15(4)
            virtual double GetEstimateCashComponent() const = 0;
            //ETF最大现金替代比例N6(5)
            virtual double GetMaxCashRatio() const = 0;
            //ETF股票篮记录数
            virtual double GetTotalRecordNum() const = 0;
            //ETF单位净值
            virtual double GetNav() const = 0;
            //ETF基准单位净值
            virtual double GetNavPerCu() const = 0;
            //单账户累计申购上限N18(2)
            virtual double GetPurchaseLimitPerUser() const = 0;
            //单账户累计赎回上限N18(2)
            virtual double GetRedemptionLimitPerUser() const = 0;
            //单账户净申购上限N18(2)
            virtual double GetNetPurchaseLimitPerUser() const = 0;
            //单账户净赎回上限N18(2)
            virtual double GetNetRedemptionLimitPerUser() const = 0;
        };
        /**
         * @brief ETF成分股信息查询消息
         */
        class QUANT_API ATPReqExtQueryETFComponentInfoMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPReqExtQueryETFComponentInfoMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPReqExtQueryETFComponentInfoMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPReqExtQueryETFComponentInfoMsg(const ATPReqExtQueryETFComponentInfoMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPReqExtQueryETFComponentInfoMsg& operator=(const ATPReqExtQueryETFComponentInfoMsg&) = delete;
            /**
             * @brief 构建消息
             */
            static ATPReqExtQueryETFComponentInfoMsg* NewMessage();

            /**
             * @brief 销毁消息
             * @param msg_ptr 待销毁的消息指针
             */
            static void DeleteMessage(ATPReqExtQueryETFComponentInfoMsg* msg_ptr);

        public:
            //市场代码
            virtual void SetMarketId(uint16_t) = 0;
            virtual uint16_t GetMarketId() const = 0;
            //证券代码
            virtual void SetSecurityId(const char*) = 0;
            virtual const char* GetSecurityId() const = 0;
            //成分股代码（默认值为空，表示查所有）
            virtual void SetComponentSecurityId(const char*) = 0;
            virtual const char* GetComponentSecurityId() const = 0;
            //查询返回数量，0表示按能返回的最大数量返回，具体数量请咨询券商
            virtual void SetReturnNum(int64_t) = 0;
            virtual int64_t GetReturnNum() const = 0;
            //申赎类型（默认值为1，表示查普通申赎）
            virtual void SetPrType(uint8_t) = 0;
            virtual uint8_t GetPrType() const = 0;
        };
        /**
         * @brief ETF成分股信息查询结果
         */
        class QUANT_API ATPRspExtQueryResultETFComponentInfoMsg
        {
        public:
            /**
             * @brief 构造函数
             */
            ATPRspExtQueryResultETFComponentInfoMsg() = default;

            /**
             * @brief 析构函数
             */
            virtual ~ATPRspExtQueryResultETFComponentInfoMsg() = default;

            /**
             * @brief 禁用拷贝构造
             */
            ATPRspExtQueryResultETFComponentInfoMsg(const ATPRspExtQueryResultETFComponentInfoMsg&) = delete;

            /**
             * @brief 禁用赋值构造
             */
            ATPRspExtQueryResultETFComponentInfoMsg& operator=(const ATPRspExtQueryResultETFComponentInfoMsg&) = delete;
        public:
            //市场代码
            virtual uint16_t GetMarketId() const = 0;
            //ETF代码
            virtual const char* GetSecurityId() const = 0;
            //证券简称
            virtual const char* GetSecuritySymbol() const = 0;
            //成分股市场代码
            virtual uint16_t GetComponentMarketId() const = 0;
            //成分股代码
            virtual const char* GetComponentSecurityId() const = 0;
            //成分股简称
            virtual const char* GetComponentSecuritySymbol() const = 0;
            //现金替代标志
            virtual uint8_t GetSubstituteFlag() const = 0;
            //申购溢价比例N7(5)
            virtual double GetPremiumRatio() const = 0;
            //赎回折价比例N7(5)
            virtual double GetRedemptionPremiumRatio() const = 0;
            //成分股数量，最小单位度量（股，张）
            virtual double GetComponentShareQty() const = 0;
            //申购现金替代金额N18(4)
            virtual double GetPurchaseCashSubstitute() const = 0;
            //赎回现金替代金额N18(4)
            virtual double GetRedemptionCashSubstitute() const = 0;
            //申赎类型
            virtual uint8_t GetPrType() const = 0;
        };
   } //namespace quant_api
} //namespace atp



#endif //  ATP_QUANT_MSG_H_