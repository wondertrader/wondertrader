// code_generator_tag
// requests:8ffc1a9bd0dd3ec725af4dde9b4a63b5 template:3329d30835d0fc5b63f16c7fd86f60cb code:2f77a2b08488dcffe4d5cab8f23bcee3
#ifndef ATP_TRADE_CLIENT_H
#define ATP_TRADE_CLIENT_H

#include "atp_trade_export.h"
#include <memory>
#include "atp_trade_handler.h"
#include "atp_trade_msg.h"
#include <unordered_map>
/*
 * @brief AgwClient前置声明
 */
namespace agw
{
    class AgwClient;
}
/*
 * @brief 华锐交易API
 */

class ATPTradeHandlerExt;

class TRADE_API ATPTradeAPI
{
    friend class ATPTradeHandlerExt;
    public:
        /**
         * @brief 		ATPTradeAPI构造函数
         * @param[in] 	ver ATP协议版本号
         */
        ATPTradeAPI(ATPVersionType ver = ATPVersionTypeConst::kV220);  
          
        /**
         * @brief ATPTradeAPI 析构函数
         */
        ~ATPTradeAPI();

        /**
         * @brief  	    初始化函数
         * @param[in] 	station_name 站点信息,该字段已经不使用
         * @param[in]   cfg_path 配置文件路径
         * @param[in]	log_dir_path 日志路径
         * @param[in]	record_all_flag 是否记录所有委托消息
         * @param[in]	encrypt_cfg  
         *              ENCRYPT_SCHEMA “0”：表示API进行转发，API不在进行加密。“1”：表示API对消息中的密码字段进行密码
         *              ATP_LOGIN_ENCRYPT_PASSWORD  登入及密码修改消息中密码字段的加密算法so路径
         *              ATP_ENCRYPT_PASSWORD  除登入及密码修改外其他消息的密码字段加密算法
         *              GM_SM2_PUBLIC_KEY_PATH 采用国密算法时指定的公钥key路径
         *              RSA_PUBLIC_KEY_PATH  采用RSA算法时指定的公钥key路径
         *              注：如果ENCRYPT_SCHEMA = 1时，未配置ATP_LOGIN_ENCRYPT_PASSWORD时将采用默认内置SM2算法，未配置ATP_ENCRYPT_PASSWOR时将采用默认内置SM3算法
         * @return   	初始化结果
         * @note		只需要调用一次即可
         */
	    static ATPRetCodeType Init(const std::string& station_name = "",
								    const std::string& cfg_path=".", 
								    const std::string& log_dir_path = "", 
								    bool record_all_flag = true,
                                    std::unordered_map<std::string,std::string> encrypt_cfg=std::unordered_map<std::string,std::string>(),
									bool connection_retention_flag=false);

        /**
         *   @brief      设置终端信息
         *	@param[in]	property 终端信息
         *   @note       已弃用
         */
        static void SetFeatureCode(const TerminalFeatureCode &property);

        /**
         *	@brief 停止函数
         */
        static void Stop();

        /**
         *	@brief 		浮点数转整数，倍数放大函数
         *	@param[in]	real 浮点数
         *	@param[in]	dec 放大倍数可取值为0,1,2，3,4
         *	@return		放回放大的整数
         *	@note 	 	0表示不放大，1表示放大10倍，2表示放大100倍，3表示放大100倍，4表示放大10000倍
         *	
         */
        static int64_t DoubleExpandToInt(double real,int32_t dec) ;

        /**
         *   @brief      设置绑定的ip地址，对接接入控制网关需设置
         *	@param[in]	ip_adderss 地址
         *   @return        结果
         */
        static ATPRetCodeType BindIpAddress(const std::string& ip_adderss);

        /**
         *	@brief 		异步连接触发函数
         *	@param[in]	property 连接相关参数
         *	@param[in]	handler 处理句柄指针
         *	@param[in]	multi_channel_flag 回报消息主动订阅参数 0 取消 1 订阅 默认0xFF
         *	@return		返回连接函数错误码
         *	@note 	 	内部保留指针在API生命周期内不能被释放
         *	
         */
        ATPRetCodeType Connect(const ATPConnectProperty& property, ATPTradeHandler* handler, uint8_t multi_channel_flag=0xFF);
	
        /**
         *	@brief 		异步连接触发函数
         *	@param[in]	property 连接相关参数
         *	@param[in]	handler 处理句柄指针
         *      @param[in]	multi_channel_flag 回报消息主动订阅参数 0 取消 1 订阅 默认0xFF
         *	@return		返回连接函数错误码
         *	@note 	 	内部保留指针在API生命周期内不能被释放
         *
         */
        ATPRetCodeType ConnectCust(const ATPConnectCustProperty& property, ATPTradeHandler* handler, uint8_t multi_channel_flag=0xFF);

        /**
         *   @brief  	连接关闭函数
         *   @return   	连接关闭错误时返回错误码
         *	@note		回调函数中将会返回EndOfConnect事件
         */
        ATPRetCodeType Close();

        /**
         * @brief       错误码转译查询函数
         * @param[in]   error_code 错误码 error_code_type 错误码类型
         * @return      错误码描述
         */ 
        static std::string GetErrorInfo(ErrorCodeUnion error_code, ATPErrorCodeTypeType error_code_type);

        /**
         * @brief       源IP和源端口信息获取函数
         * @return      pair对象第一个值为源IP，第二个值为端口
         */ 
        static std::pair<std::string, uint32_t> GetClientInfo();



        /**
         * @brief      融资融券交易委托函数
         * @param[in]  ATPReqCreditOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqCreditOrder(const ATPReqCreditOrderMsg* msg);


        /**
         * @brief      融资融券非交易委托函数
         * @param[in]  ATPReqMarginNoTradeOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqMarginNoTradeOrder(const ATPReqMarginNoTradeOrderMsg* msg);


        /**
         * @brief      融资融券交易偿还委托函数
         * @param[in]  ATPReqMarginRepayOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqMarginRepayOrder(const ATPReqMarginRepayOrderMsg* msg);


        /**
         * @brief      融资融券直接还款函数
         * @param[in]  ATPReqExtRepayOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqExtRepayOther(const ATPReqExtRepayOtherMsg* msg);


        /**
         * @brief      现货（股票、基金、债券等）集中竞价交易委托函数
         * @param[in]  ATPReqCashAuctionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       order
         * @note       支持沪深市场主板、科创板、创业板的股票、债券、基金的竞价买卖业务
         */
        ATPRetCodeType ReqCashAuctionOrder(const ATPReqCashAuctionOrderMsg* msg);


        /**
         * @brief      盘后定价(科创板、创业板)业务委托函数
         * @param[in]  ATPReqTibAfterHourAuctionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqTibAfterHourAuctionOrder(const ATPReqTibAfterHourAuctionOrderMsg* msg);


        /**
         * @brief      网上发行业务委托函数
         * @param[in]  ATPReqIssueOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       order
         * @note       支持股票的新股申购（IPO）和增发发行；支持新债的申购；支持ETF基金、LOF基金的认购
         */
        ATPRetCodeType ReqIssueOrder(const ATPReqIssueOrderMsg* msg);


        /**
         * @brief      配售业务委托函数
         * @param[in]  ATPReqRightsIssueOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       order
         * @note       支持沪深市场主板A股、上海科创板股票、深圳创业板股票的配股配债业务
         */
        ATPRetCodeType ReqRightsIssueOrder(const ATPReqRightsIssueOrderMsg* msg);


        /**
         * @brief      质押式回购集中竞价业务委托函数
         * @param[in]  ATPReqRepoAuctionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqRepoAuctionOrder(const ATPReqRepoAuctionOrderMsg* msg);


        /**
         * @brief      债券分销业务委托函数
         * @param[in]  ATPReqBondDistributionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqBondDistributionOrder(const ATPReqBondDistributionOrderMsg* msg);


        /**
         * @brief      期权集中竞价业务委托函数
         * @param[in]  ATPReqOptionAuctionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqOptionAuctionOrder(const ATPReqOptionAuctionOrderMsg* msg);


        /**
         * @brief      期权报价业务委托函数
         * @param[in]  ATPReqOptionQuoteOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqOptionQuoteOrder(const ATPReqOptionQuoteOrderMsg* msg);


        /**
         * @brief      ETF申赎业务委托函数
         * @param[in]  ATPReqETFRedemptionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqETFRedemptionOrder(const ATPReqETFRedemptionOrderMsg* msg);


        /**
         * @brief      债券转股回售业务委托函数
         * @param[in]  ATPReqSwapPutbackOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqSwapPutbackOrder(const ATPReqSwapPutbackOrderMsg* msg);


        /**
         * @brief      期权行权业务委托函数
         * @param[in]  ATPReqOptionExerciseOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqOptionExerciseOrder(const ATPReqOptionExerciseOrderMsg* msg);


        /**
         * @brief      期权合并行权业务委托函数
         * @param[in]  ATPReqOptionCombinationExerciseOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqOptionCombinationExerciseOrder(const ATPReqOptionCombinationExerciseOrderMsg* msg);


        /**
         * @brief      期权组合策略保证金业务委托函数
         * @param[in]  ATPReqOptionCombinationOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqOptionCombinationOrder(const ATPReqOptionCombinationOrderMsg* msg);


        /**
         * @brief      转托管业务委托函数
         * @param[in]  ATPReqDesignationOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       转托管业务只支持深圳市场
         */
        ATPRetCodeType ReqDesignationOrder(const ATPReqDesignationOrderMsg* msg);


        /**
         * @brief      指定交易业务委托函数
         * @param[in]  ATPReqDesignatedTransactionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqDesignatedTransactionOrder(const ATPReqDesignatedTransactionOrderMsg* msg);


        /**
         * @brief      开放式基金申赎业务委托函数
         * @param[in]  ATPReqLOFOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       买卖方向为D-申购时，申购金额（cash_order_qty）字段必填；买卖方向为E-赎回时，赎回基金份额（order_qty）字段必填
         */
        ATPRetCodeType ReqLOFOrder(const ATPReqLOFOrderMsg* msg);


        /**
         * @brief      要约收购业务委托函数
         * @param[in]  ATPReqTenderOfferOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqTenderOfferOrder(const ATPReqTenderOfferOrderMsg* msg);


        /**
         * @brief      网络投票业务委托函数
         * @param[in]  ATPReqNetVotingOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqNetVotingOrder(const ATPReqNetVotingOrderMsg* msg);


        /**
         * @brief      密码服务业务委托函数
         * @param[in]  ATPReqPasswordServiceOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqPasswordServiceOrder(const ATPReqPasswordServiceOrderMsg* msg);


        /**
         * @brief      权证行权业务委托函数
         * @param[in]  ATPReqWarrantExerciseOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqWarrantExerciseOrder(const ATPReqWarrantExerciseOrderMsg* msg);


        /**
         * @brief      分级基金实时分拆合并业务委托函数
         * @param[in]  ATPReqStructuredFundOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqStructuredFundOrder(const ATPReqStructuredFundOrderMsg* msg);


        /**
         * @brief      质押入库出库业务委托函数
         * @param[in]  ATPReqPledgeInOrOutOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqPledgeInOrOutOrder(const ATPReqPledgeInOrOutOrderMsg* msg);


        /**
         * @brief      通用撤单函数
         * @param[in]  ATPReqCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       cancel
         * @note       支持以下业务类型的撤单：盘后定价交易、大宗交易、债券分销、现货交易、ETF、盘后定价（科创板、创业板）交易业务、港股通买卖、港股通公司行为、网上发行认购、网络投票、质押式回购、配售业务、分级基金分拆合并、债券转股回售、要约收购、权证行权
         */
        ATPRetCodeType ReqCancelOrder(const ATPReqCancelOrderMsg* msg);


        /**
         * @brief      高管额度划转业务委托函数
         * @param[in]  ATPReqReduceHoldingTransferOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqReduceHoldingTransferOrder(const ATPReqReduceHoldingTransferOrderMsg* msg);


        /**
         * @brief      高管额度划转撤单函数
         * @param[in]  ATPReqReduceHoldingTransferCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       cancel
         * @note       
         */
        ATPRetCodeType ReqReduceHoldingTransferCancelOrder(const ATPReqReduceHoldingTransferCancelOrderMsg* msg);


        /**
         * @brief      一键撤单函数
         * @param[in]  ATPReqBatchCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,brokerageOption,margin
         * @type       cancel
         * @note       批量撤单，可一次性发起条件选择范围内所有未成交委托的撤单委托
         */
        ATPRetCodeType ReqBatchCancelOrder(const ATPReqBatchCancelOrderMsg* msg);


        /**
         * @brief      FDEP公共撤单函数
         * @param[in]  ATPReqFDEPCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       cancel
         * @note       适用于深圳市场分级基金实时分拆合并业务的撤单
         */
        ATPRetCodeType ReqFDEPCancelOrder(const ATPReqFDEPCancelOrderMsg* msg);


        /**
         * @brief      期权撤单函数
         * @param[in]  ATPReqOptionCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       cancel
         * @note       
         */
        ATPRetCodeType ReqOptionCancelOrder(const ATPReqOptionCancelOrderMsg* msg);


        /**
         * @brief      同步客户回报函数
         * @param[in]  ATPReqSyncBizOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   common
         * @type       other
         * @note       
         */
        ATPRetCodeType ReqSyncBiz(const ATPReqSyncBizOtherMsg* msg);


        /**
         * @brief      订单查询函数
         * @param[in]  ATPReqOrderQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqOrderQuery(const ATPReqOrderQueryMsg* msg);


        /**
         * @brief      成交查询函数
         * @param[in]  ATPReqTradeOrderQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqTradeOrderQuery(const ATPReqTradeOrderQueryMsg* msg);


        /**
         * @brief      融资融券合约明细查询函数
         * @param[in]  ATPReqExtQueryContractSpecificationsMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryContractSpecifications(const ATPReqExtQueryContractSpecificationsMsg* msg);


        /**
         * @brief      融资融券信用额度查询函数
         * @param[in]  ATPReqExtQueryCreditAmountMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryCreditAmount(const ATPReqExtQueryCreditAmountMsg* msg);


        /**
         * @brief      融资融券保证金可用查询函数
         * @param[in]  ATPReqExtQueryMarginUseableMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryMarginUseable(const ATPReqExtQueryMarginUseableMsg* msg);


        /**
         * @brief      融资融券资金负债查询函数
         * @param[in]  ATPReqExtQueryFundLiabilityMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryFundLiability(const ATPReqExtQueryFundLiabilityMsg* msg);


        /**
         * @brief      融资融券股份负债查询函数
         * @param[in]  ATPReqExtQueryShareLiabilityMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryShareLiability(const ATPReqExtQueryShareLiabilityMsg* msg);


        /**
         * @brief      融资融券合约管理查询函数
         * @param[in]  ATPReqExtQueryContractSpecificationsManagerMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryContractSpecificationsManager(const ATPReqExtQueryContractSpecificationsManagerMsg* msg);


        /**
         * @brief      融资融券标的清单查询函数
         * @param[in]  ATPReqExtQueryMarginUnderlyingMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryMarginUnderlying(const ATPReqExtQueryMarginUnderlyingMsg* msg);


        /**
         * @brief      融资融券客户信用资料查询函数
         * @param[in]  ATPReqExtQueryCreditInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryCreditInfo(const ATPReqExtQueryCreditInfoMsg* msg);


        /**
         * @brief      融资融券资产信息查询函数
         * @param[in]  ATPReqExtQueryFundAssetsInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryFundAssetsInfo(const ATPReqExtQueryFundAssetsInfoMsg* msg);


        /**
         * @brief      ETF申赎成交查询函数
         * @param[in]  ATPReqETFQueryOrderExMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqETFQueryOrderEx(const ATPReqETFQueryOrderExMsg* msg);


        /**
         * @brief      增强成交查询函数
         * @param[in]  ATPReqTradeOrderExQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       相比于成交查询函数，增强成交查询函数在3.1.5版本之前支持按客户订单编号查询单条记录，并且支持撤单和废单的响应查询；在3.1.5版本及之后两者无差别
         */
        ATPRetCodeType ReqTradeOrderExQuery(const ATPReqTradeOrderExQueryMsg* msg);


        /**
         * @brief      资金查询函数
         * @param[in]  ATPReqFundQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqFundQuery(const ATPReqFundQueryMsg* msg);


        /**
         * @brief      股份查询函数
         * @param[in]  ATPReqShareQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       ATP3.1.5及之前版本，按照低版本查询方式，具体如下：<br/>--单市场查询场景<br/>传入account_id_array或者market_id + account_id_array.account_id进行查询<br/>--多市场查询场景<br/>传入多个account_id_array进行查询
         */
        ATPRetCodeType ReqShareQuery(const ATPReqShareQueryMsg* msg);


        /**
         * @brief      合约账户持仓查询函数
         * @param[in]  ATPReqAccountContractShareQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqAccountContractShareQuery(const ATPReqAccountContractShareQueryMsg* msg);


        /**
         * @brief      指定合约账户资金查询函数
         * @param[in]  ATPReqAccountContractFundQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqAccountContractFundQuery(const ATPReqAccountContractFundQueryMsg* msg);


        /**
         * @brief      报价订单查询函数
         * @param[in]  ATPReqQuoteQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqOptionQuoteOrderQuery(const ATPReqQuoteQueryMsg* msg);


        /**
         * @brief      期权组合策略保证金持仓查询函数
         * @param[in]  ATPReqCombinationQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqOptionCombinationPositionQuery(const ATPReqCombinationQueryMsg* msg);


        /**
         * @brief      期权组合策略保证金订单查询函数
         * @param[in]  ATPReqCombinationQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqOptionCombinationOrderQuery(const ATPReqCombinationQueryMsg* msg);


        /**
         * @brief      成交汇总查询函数
         * @param[in]  ATPReqTradeCollectQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,brokerageOption,margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqTradeCollectQuery(const ATPReqTradeCollectQueryMsg* msg);


        /**
         * @brief      科创板新股发行申购额度查询函数
         * @param[in]  ATPReqTIBIPOQtyQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       支持上海市场（科创板）新股发行申购额度查询
         */
        ATPRetCodeType ReqTIBIPOQtyQuery(const ATPReqTIBIPOQtyQueryMsg* msg);


        /**
         * @brief      最大可委托数查询函数
         * @param[in]  ATPReqMaxOrderQtyQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqMaxOrderQtyQuery(const ATPReqMaxOrderQtyQueryMsg* msg);


        /**
         * @brief      增强型最大可委托数查询函数
         * @param[in]  ATPReqNewMaxOrderQtyQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqNewMaxOrderQtyQuery(const ATPReqNewMaxOrderQtyQueryMsg* msg);


        /**
         * @brief      账户登入函数
         * @param[in]  ATPReqCustLoginOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   common
         * @type       session
         * @note       账户登录模式分为三种，分别为客户号登录模式、资金账号登录模式、证券账号登录模式；客户号、资金账号（营业部字段是否必填请咨询券商）、证券账号可以根据账户登录模式三个选填一个，其他账户字段不做要求
         */
        ATPRetCodeType ReqCustLoginOther(const ATPReqCustLoginOtherMsg* msg);


        /**
         * @brief      账户登出函数
         * @param[in]  ATPReqCustLogoutOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   common
         * @type       session
         * @note       登出时 资金账户必填，营业部是否必填请咨询券商
         */
        ATPRetCodeType ReqCustLogoutOther(const ATPReqCustLogoutOtherMsg* msg);


        /**
         * @brief      外部资金划拨函数
         * @param[in]  ATPReqExtFundTransferOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       other
         * @note       集中交易系统与ATP极速交易系统资金互转，具体支持情况请咨询券商
         */
        ATPRetCodeType ReqExtFundTransferOther(const ATPReqExtFundTransferOtherMsg* msg);


        /**
         * @brief      大宗盘后定价委托函数
         * @param[in]  ATPReqAfterHoursPricingOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqAfterHoursPricingOrder(const ATPReqAfterHoursPricingOrderMsg* msg);


        /**
         * @brief      大宗意向申报委托函数
         * @param[in]  ATPReqBlockTradeIntentionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqBlockTradeIntentionOrder(const ATPReqBlockTradeIntentionOrderMsg* msg);


        /**
         * @brief      大宗成交申报委托函数
         * @param[in]  ATPReqBlockTradeTransactionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqBlockTradeTransactionOrder(const ATPReqBlockTradeTransactionOrderMsg* msg);


        /**
         * @brief      大宗定价申报委托函数
         * @param[in]  ATPReqBlockTradePricingOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqBlockTradePricingOrder(const ATPReqBlockTradePricingOrderMsg* msg);


        /**
         * @brief      大宗交易撤单函数
         * @param[in]  ATPReqBlockTradeCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       cancel
         * @note       
         */
        ATPRetCodeType ReqBlockTradeCancelOrder(const ATPReqBlockTradeCancelOrderMsg* msg);


        /**
         * @brief      外部期权资金账户资金划拨函数
         * @param[in]  ATPReqExtOptionFundTransfeOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       other
         * @note       
         */
        ATPRetCodeType ReqExtOptionFundTransferOther(const ATPReqExtOptionFundTransfeOtherMsg* msg);


        /**
         * @brief      行权预估盈亏查询函数
         * @param[in]  ATPReqExtQueryExerciseProfitLossMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryExerciseProfitLoss(const ATPReqExtQueryExerciseProfitLossMsg* msg);


        /**
         * @brief      备兑锁定解锁业务委托函数
         * @param[in]  ATPReqCoveredLockOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqCoveredLockOrder(const ATPReqCoveredLockOrderMsg* msg);


        /**
         * @brief      期权最大可委托数查询函数
         * @param[in]  ATPReqCoverMaxOrderQtyQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqCoverMaxOrderQtyQuery(const ATPReqCoverMaxOrderQtyQueryMsg* msg);


        /**
         * @brief      转处置业务委托函数
         * @param[in]  ATPReqTurnToDisposalOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqTurnToDisposalOrder(const ATPReqTurnToDisposalOrderMsg* msg);


        /**
         * @brief      港股通业务委托函数
         * @param[in]  ATPReqHKStocksThroughAuctionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqHKStocksThroughAuctionOrder(const ATPReqHKStocksThroughAuctionOrderMsg* msg);


        /**
         * @brief      港股通投票业务委托函数
         * @param[in]  ATPReqHKVotingOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqHKVotingOrder(const ATPReqHKVotingOrderMsg* msg);


        /**
         * @brief      港股通公司行为业务委托函数
         * @param[in]  ATPReqHKCorporateActionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqHKCorporateActionOrder(const ATPReqHKCorporateActionOrderMsg* msg);


        /**
         * @brief      港股通转托管业务委托函数
         * @param[in]  ATPReqHKDesignationTransferOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqHKDesignationTransferOrder(const ATPReqHKDesignationTransferOrderMsg* msg);


        /**
         * @brief      公司行为撤单业务委托函数
         * @param[in]  ATPReqHKCorporateActionCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       cancel
         * @note       
         */
        ATPRetCodeType ReqHKCorporateActionCancelOrder(const ATPReqHKCorporateActionCancelOrderMsg* msg);


        /**
         * @brief      期权普通与备兑互转委托函数
         * @param[in]  ATPReqOptionCoveredTransUncoveredOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqOptionCoveredTransUncoveredOrder(const ATPReqOptionCoveredTransUncoveredOrderMsg* msg);


        /**
         * @brief      合并行权最大可委托数查询函数
         * @param[in]  ATPReqExtQueryCombinExerciseMaxOrderQtyMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryCombinExerciseMaxOrderQty(const ATPReqExtQueryCombinExerciseMaxOrderQtyMsg* msg);


        /**
         * @brief      合并行权预估盈亏查询函数
         * @param[in]  ATPReqExtQueryCombinExerciseProfitLossMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryCombinExerciseProfitLoss(const ATPReqExtQueryCombinExerciseProfitLossMsg* msg);


        /**
         * @brief      期权组合策略最大可委托数查询函数
         * @param[in]  ATPReqExternalQueryOptionCombinationMaxOrderQtyMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExternalQueryOptionCombinationMaxOrderQty(const ATPReqExternalQueryOptionCombinationMaxOrderQtyMsg* msg);


        /**
         * @brief      按证券账户划转资金函数
         * @param[in]  ATPReqExternalInsTETransFundReqMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       other
         * @note       
         */
        ATPRetCodeType ReqExternalInsTETransFundReq(const ATPReqExternalInsTETransFundReqMsg* msg);


        /**
         * @brief      期权按证券账户划转资金函数
         * @param[in]  ATPReqOptionExternalInsTETransFundReqMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       other
         * @note       
         */
        ATPRetCodeType ReqOptionExternalInsTETransFundReq(const ATPReqOptionExternalInsTETransFundReqMsg* msg);


        /**
         * @brief      客户密码修改函数
         * @param[in]  ATPReqCustPasswdModifyMsg
         * @return     kSuccess成功；其他失败 
         * @business   common
         * @type       other
         * @note       客户号、资金账号+营业部两者必选填其一
         */
        ATPRetCodeType ReqCustPasswdModify(const ATPReqCustPasswdModifyMsg* msg);


        /**
         * @brief      增强资金查询函数
         * @param[in]  ATPReqExtFundQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       相比于资金查询函数，增强资金查询函数支持币种字段填空查询所有币种的资金（ATP3.1.7开始支持）
         */
        ATPRetCodeType ReqExtFundQuery(const ATPReqExtFundQueryMsg* msg);


        /**
         * @brief      现货竞价批量委托函数
         * @param[in]  ATPReqBatchCashAuctionOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       批量委托类型为单代码等量拆单时：单笔上限字段（max_order_qty）为必填项，其值为等量拆单的每笔订单数量；批量委托类型为单代码递减拆单时：递减数量字段（decrease_qty）为必填项，其值为每笔的减数。单笔上限字段（max_order_qty）为必填项，其值为每笔数量的上限；批量委托类型为多代码批量报单时：多证券委托组合为必填项
         */
        ATPRetCodeType ReqBatchCashAuctionOrder(const ATPReqBatchCashAuctionOrderMsg* msg);


        /**
         * @brief      现货竞价批量委托查询函数
         * @param[in]  ATPReqBatchOrderQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqQueryBatchOrder(const ATPReqBatchOrderQueryMsg* msg);


        /**
         * @brief      批量委托撤销函数
         * @param[in]  ATPReqCancelBatchOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       cancel
         * @note       适用于现货竞价批量委托的撤单
         */
        ATPRetCodeType ReqCancelBatchOrder(const ATPReqCancelBatchOrderMsg* msg);


        /**
         * @brief      垫券还券业务委托函数
         * @param[in]  ATPReqCouponSwapOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqCouponSwapOrder(const ATPReqCouponSwapOrderMsg* msg);


        /**
         * @brief      待清偿委托业务委托函数
         * @param[in]  ATPReqDeductionToBePaidOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqDeductionToBePaidOrder(const ATPReqDeductionToBePaidOrderMsg* msg);


        /**
         * @brief      标的证券备兑占用查询函数
         * @param[in]  ATPReqExternalQueryLockedAccountContractQtyMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryLockedAccountQty(const ATPReqExternalQueryLockedAccountContractQtyMsg* msg);


        /**
         * @brief      期权强平业务委托函数
         * @param[in]  ATPReqOptionAuctionForceCloseOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       order
         * @note       
         */
        ATPRetCodeType ReqOptionAuctionForceCloseOrder(const ATPReqOptionAuctionForceCloseOrderMsg* msg);


        /**
         * @brief      期权客户保证金风险监控信息查询函数
         * @param[in]  ATPReqExternalQueryOptionMarginRiskInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExternalQueryOptionMarginRiskInfoMsg(const ATPReqExternalQueryOptionMarginRiskInfoMsg* msg);


        /**
         * @brief      期权基础信息查询函数
         * @param[in]  ATPReqExternalQueryOptionSecurityInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExternalQueryOptionSecurityInfoMsg(const ATPReqExternalQueryOptionSecurityInfoMsg* msg);


        /**
         * @brief      期权标的证券基础信息查询函数
         * @param[in]  ATPReqExternalQueryOptionUnderlyingSecurityInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExternalQueryOptionUnderlyingSecurityInfo(const ATPReqExternalQueryOptionUnderlyingSecurityInfoMsg* msg);


        /**
         * @brief      期权五档行情查询函数
         * @param[in]  ATPReqExternalQueryOptionMdSnapShotArrayInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExternalQueryOptionMdSnapShotArrayInfoMsg(const ATPReqExternalQueryOptionMdSnapShotArrayInfoMsg* msg);


        /**
         * @brief      港股通转托管最大可委托数查询函数
         * @param[in]  ATPReqHKExtQueryMaxQueryOrderQtyMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqHKEMaxQueryOrder(const ATPReqHKExtQueryMaxQueryOrderQtyMsg* msg);


        /**
         * @brief      信用账户融资保底额度查询函数
         * @param[in]  ATPReqExtQueryMarginTradingPrivateAmoutMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtMarginTradingPrivateAmountQuery(const ATPReqExtQueryMarginTradingPrivateAmoutMsg* msg);


        /**
         * @brief      信用账户融券保底额度查询函数
         * @param[in]  ATPReqExtQuerySecuritiesLendingPrivateAmountMsg
         * @return     kSuccess成功；其他失败 
         * @business   margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtSecuritiesLendingPrivateAmountQuery(const ATPReqExtQuerySecuritiesLendingPrivateAmountMsg* msg);


        /**
         * @brief      透传第三方OES通用函数
         * @param[in]  ATPReqThirdPartPassThroughMsg
         * @return     kSuccess成功；其他失败 
         * @business   common
         * @type       other
         * @note       
         */
        ATPRetCodeType ReqThirdPartPassThrough(const ATPReqThirdPartPassThroughMsg* msg);


        /**
         * @brief      期权成交查询函数
         * @param[in]  ATPReqOptionTradeOrderQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqOptionTradeOrderQuery(const ATPReqOptionTradeOrderQueryMsg* msg);


        /**
         * @brief      期权订单查询函数
         * @param[in]  ATPReqOptionOrderQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqOptionOrderQuery(const ATPReqOptionOrderQueryMsg* msg);


        /**
         * @brief      银证转账函数
         * @param[in]  ATPReqBankSecuritiesFundTransferOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   brokerageOption
         * @type       other
         * @note       将资金从银行账户转入资金账户中，具体支持情况请咨询券商
         */
        ATPRetCodeType ReqBankSecuritiesFundTransferOther(const ATPReqBankSecuritiesFundTransferOtherMsg* msg);


        /**
         * @brief      一步式银证转账函数
         * @param[in]  ATPReqCashBankSecuritiesFundTransferOtherMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       other
         * @note       将资金从银行账户转入资金账户中，具体支持情况请咨询券商
         */
        ATPRetCodeType ReqCashBankSecuritiesFundTransferOther(const ATPReqCashBankSecuritiesFundTransferOtherMsg* msg);


        /**
         * @brief      外部实时资金查询函数
         * @param[in]  ATPReqExtRealtimeFundQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       发往集中交易系统进行资金查询，具体支持情况请咨询券商
         */
        ATPRetCodeType ReqExtRealtimeFundQuery(const ATPReqExtRealtimeFundQueryMsg* msg);


        /**
         * @brief      新股清单查询函数
         * @param[in]  ATPReqExtQueryNewShareInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryNewShareInfo(const ATPReqExtQueryNewShareInfoMsg* msg);


        /**
         * @brief      新股发行申购额度查询函数
         * @param[in]  ATPReqIPOQtyQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       支持沪深市场新股发行申购额度查询<br/>ATP3.1.7 开始支持科创板新股发行申购额度查询
         */
        ATPRetCodeType ReqIPOQtyQuery(const ATPReqIPOQtyQueryMsg* msg);


        /**
         * @brief      证券信息查询函数
         * @param[in]  ATPReqExtQuerySecurityInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,margin
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQuerySecurityInfo(const ATPReqExtQuerySecurityInfoMsg* msg);


        /**
         * @brief      增强股份查询函数
         * @param[in]  ATPReqExtQueryShareExMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       相比于股份查询函数，查询结果减少“当前融券买入量”等无意义字段
         */
        ATPRetCodeType ReqExtQueryShareEx(const ATPReqExtQueryShareExMsg* msg);


        /**
         * @brief      增强资产查询函数
         * @param[in]  ATPReqExtQueryAssetExMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash,
         * @type       query
         * @note       
         */
        ATPRetCodeType ReqExtQueryAssetEx(const ATPReqExtQueryAssetExMsg* msg);


        /**
         * @brief      算法母单下达委托函数
         * @param[in]  ATPReqAlgoParentOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   algo
         * @type       order
         * @note       现阶段支持沪深市场主板、科创板、创业板的股票、债券、基金的竞价买卖业务；详细可见开发指南<br/>ATP最低版本要求V3.1.9.1
         */
        ATPRetCodeType ReqAlgoParentOrder(const ATPReqAlgoParentOrderMsg* msg);


        /**
         * @brief      算法母单撤销委托函数
         * @param[in]  ATPReqCancelAlgoParentOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   algo
         * @type       cancel
         * @note       ATP最低版本要求V3.1.9.1
         */
        ATPRetCodeType ReqCancelAlgoParentOrder(const ATPReqCancelAlgoParentOrderMsg* msg);


        /**
         * @brief      算法母单查询函数
         * @param[in]  ATPReqExternalQueryAlgoParentMsg
         * @return     kSuccess成功；其他失败 
         * @business   algo
         * @type       query
         * @note       ATP最低版本要求V3.1.9.1
         */
        ATPRetCodeType ReqExternalQueryAlgoParent(const ATPReqExternalQueryAlgoParentMsg* msg);


        /**
         * @brief      算法子单查询函数
         * @param[in]  ATPReqExternalQueryAlgoOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   algo
         * @type       query
         * @note       ATP最低版本要求V3.1.9.1
         */
        ATPRetCodeType ReqExternalQueryAlgoOrder(const ATPReqExternalQueryAlgoOrderMsg* msg);


        /**
         * @brief      债券协商成交委托函数
         * @param[in]  ATPReqBondConsultTradeOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       order
         * @note       ATP3.1.10开始支持该接口
         */
        ATPRetCodeType ReqBondConsultTradeOrder(const ATPReqBondConsultTradeOrderMsg* msg);


        /**
         * @brief      债券协商成交委托撤单委托函数
         * @param[in]  ATPReqBondConsultTradeCancelOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       cancel
         * @note       适用于债券协商成交委托的撤单<br/>ATP3.1.10开始支持该接口
         */
        ATPRetCodeType ReqBondConsultTradeCancelOrder(const ATPReqBondConsultTradeCancelOrderMsg* msg);


        /**
         * @brief      债券协商成交转发成交申报查询函数
         * @param[in]  ATPReqExtQueryOrderForwardInfoMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       ATP3.1.10开始支持该接口
         */
        ATPRetCodeType ReqExtQueryOrderForwardInfo(const ATPReqExtQueryOrderForwardInfoMsg* msg);


        /**
         * @brief      算法母单暂停恢复函数
         * @param[in]  ATPReqOperateAlgoParentOrderMsg
         * @return     kSuccess成功；其他失败 
         * @business   algo
         * @type       cancel
         * @note       ATP3.1.10开始支持该接口
         */
        ATPRetCodeType ReqOperateAlgoParentOrder(const ATPReqOperateAlgoParentOrderMsg* msg);


        /**
         * @brief      外部持仓查询函数
         * @param[in]  ATPReqExtStockQueryMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       query
         * @note       发往集中交易系统进行持仓查询，具体支持情况请咨询券商，ATP3.1.11开始支持该接口
         */
        ATPRetCodeType ReqExtStockQuery(const ATPReqExtStockQueryMsg* msg);


        /**
         * @brief      外部持仓划拨函数
         * @param[in]  ATPReqExtShareTransferMsg
         * @return     kSuccess成功；其他失败 
         * @business   cash
         * @type       other
         * @note       集中交易系统与ATP极速交易系统持仓互转，具体支持情况请咨询券商，ATP3.1.11开始支持该接口
         */
        ATPRetCodeType ReqExtShareTransfer(const ATPReqExtShareTransferMsg* msg);


        
    private:
    	std::shared_ptr<agw::AgwClient> m_agw_client_ptr; ///< API通信层对象
	    ATPVersionType m_version_ ; ///< 协议版本号
	    bool is_negotiate_version_; ///< 是否协商了版本号  
		//私有化拷贝构造函数、拷贝赋值运算符及移动构造函数
		ATPTradeAPI(const ATPTradeAPI&);
		ATPTradeAPI &operator=(const ATPTradeAPI&);
		ATPTradeAPI(ATPTradeAPI &&);
		ATPTradeHandlerExt* m_handlerext_ptr{nullptr};
};

#endif // ATP_TRADE_CLIENT_H




