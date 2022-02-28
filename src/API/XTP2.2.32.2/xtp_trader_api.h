/////////////////////////////////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xtp_trader_api.h
///@brief 定义客户端交易接口
/////////////////////////////////////////////////////////////////////////

#ifndef _XTP_TRADER_API_H_
#define _XTP_TRADER_API_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xtp_api_struct.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __declspec(dllexport)
#else
#define TRADER_API_EXPORT __declspec(dllimport)
#endif
#else
#define TRADER_API_EXPORT 
#endif

/*!
* \class XTP::API::TraderSpi
*
* \brief 交易接口响应类
*
* \author 中泰证券股份有限公司
* \date 十月 2015
*/
namespace XTP {
	namespace API {

		class TraderSpi
		{
		public:

			///当客户端的某个连接与交易后台通信连接断开时，该方法被调用。
			///@param reason 错误原因，请与错误代码表对应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 用户主动调用logout导致的断线，不会触发此函数。api不会自动重连，当断线发生时，请用户自行选择后续操作，可以在此函数中调用Login重新登录，并更新session_id，此时用户收到的数据跟断线之前是连续的
			virtual void OnDisconnected(uint64_t session_id, int reason) {};

			///错误应答
			///@param error_info 当服务器响应发生错误时的具体的错误代码和错误信息,当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 此函数只有在服务器发生错误时才会调用，一般无需用户处理
			virtual void OnError(XTPRI *error_info) {};

			///报单通知
			///@param order_info 订单响应具体信息，用户可以通过order_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单，order_info.qty_left字段在订单为未成交、部成、全成、废单状态时，表示此订单还没有成交的数量，在部撤、全撤状态时，表示此订单被撤的数量。order_info.order_cancel_xtp_id为其所对应的撤单ID，不为0时表示此单被撤成功
			///@param error_info 订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 每次订单状态更新时，都会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，在订单未成交、全部成交、全部撤单、部分撤单、已拒绝这些状态时会有响应，对于部分成交的情况，请由订单的成交回报来自行确认。所有登录了此用户的客户端都将收到此用户的订单响应
			virtual void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) {};

			///成交通知
			///@param trade_info 成交回报的具体信息，用户可以通过trade_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。对于上交所，exec_id可以唯一标识一笔成交。当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交了。对于深交所，exec_id是唯一的，暂时无此判断机制。report_index+market字段可以组成唯一标识表示成交回报。
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 订单有成交发生的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的成交回报。相关订单为部成状态，需要用户通过成交回报的成交数量来确定，OnOrderEvent()不会推送部成状态。
			virtual void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) {};

			///撤单出错响应
			///@param cancel_info 撤单具体信息，包括撤单的order_cancel_xtp_id和待撤单的order_xtp_id
			///@param error_info 撤单被拒绝或者发生错误时错误代码和错误信息，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 此响应只会在撤单发生错误时被回调
			virtual void OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) {};

			///请求查询报单响应-旧版本接口
			///@param order_info 查询到的一个报单
			///@param error_info 查询报单时发生错误时，返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询报单响应-新版本接口
			///@param order_info 查询到的一个报单信息
			///@param error_info 查询报单时发生错误时，返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryOrderEx(XTPOrderInfoEx *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///分页请求查询报单响应-旧版本接口
			///@param order_info 查询到的一个报单
			///@param req_count 分页请求的最大数量
			///@param order_sequence 分页请求的当前回报数量
			///@param query_reference 当前报单信息所对应的查询索引，需要记录下来，在进行下一次分页查询的时候需要用到
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当order_sequence为0，表明当次查询没有查到任何记录，当is_last为true时，如果order_sequence等于req_count，那么表示还有报单，可以进行下一次分页查询，如果不等，表示所有报单已经查询完毕。一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOrderByPage(XTPQueryOrderRsp *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

			///分页请求查询报单响应-新版本接口
			///@param order_info 查询到的一个报单
			///@param req_count 分页请求的最大数量
			///@param order_sequence 分页请求的当前回报数量
			///@param query_reference 当前报单信息所对应的查询索引，需要记录下来，在进行下一次分页查询的时候需要用到
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当order_sequence为0，表明当次查询没有查到任何记录，当is_last为true时，如果order_sequence等于req_count，那么表示还有报单，可以进行下一次分页查询，如果不等，表示所有报单已经查询完毕。一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOrderByPageEx(XTPOrderInfoEx *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询成交响应
			///@param trade_info 查询到的一个成交回报
			///@param error_info 查询成交回报发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///分页请求查询成交响应
			///@param trade_info 查询到的一个成交信息
			///@param req_count 分页请求的最大数量
			///@param trade_sequence 分页请求的当前回报数量
			///@param query_reference 当前报单信息所对应的查询索引，需要记录下来，在进行下一次分页查询的时候需要用到
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当trade_sequence为0，表明当次查询没有查到任何记录，当is_last为true时，如果trade_sequence等于req_count，那么表示还有回报，可以进行下一次分页查询，如果不等，表示所有回报已经查询完毕。一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryTradeByPage(XTPQueryTradeRsp *trade_info, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询投资者持仓响应
			///@param position 查询到的一只股票的持仓情况
			///@param error_info 查询账户持仓发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 由于用户可能持有多个股票，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询资金账户响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param asset 查询到的资金账户情况
			///@param error_info 查询资金账户发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询分级基金信息响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param fund_info 查询到的分级基金情况
			///@param error_info 查询分级基金发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryStructuredFund(XTPStructuredFundInfo *fund_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询资金划拨订单响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param fund_transfer_info 查询到的资金账户情况
			///@param error_info 查询资金账户发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///资金划拨通知
			///@param fund_transfer_info 资金划拨通知的具体信息，用户可以通过fund_transfer_info.serial_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。
			///@param error_info 资金划拨订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误。当资金划拨方向为一号两中心节点之间划拨，且error_info.error_id=11000384时，error_info.error_msg为结点中可用于划拨的资金（以整数为准），用户需进行stringToInt的转化，可据此填写合适的资金，再次发起划拨请求
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当资金划拨订单有状态变化的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的资金划拨通知。
			virtual void OnFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, uint64_t session_id) {};

			///请求查询其他节点可用资金的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param fund_info 查询到的其他节点可用资金情况
			///@param error_info 查询其他节点可用资金发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryOtherServerFund(XTPFundQueryRsp *fund_info, XTPRI *error_info, int request_id, uint64_t session_id) {};

			///请求查询ETF清单文件的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param etf_info 查询到的ETF清单文件情况
			///@param error_info 查询ETF清单文件发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询ETF股票篮的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param etf_component_info 查询到的ETF合约的相关成分股信息
			///@param error_info 查询ETF股票篮发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询今日新股申购信息列表的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param ipo_info 查询到的今日新股申购的一只股票信息
			///@param error_info 查询今日新股申购信息列表发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询用户新股申购额度信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param quota_info 查询到的用户某个市场的今日新股申购额度信息
			///@param error_info 查查询用户新股申购额度信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询期权合约的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param option_info 查询到的期权合约情况
			///@param error_info 查询期权合约发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryOptionAuctionInfo(XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///融资融券业务中现金直接还款的响应
			///@param cash_repay_info 现金直接还款通知的具体信息，用户可以通过cash_repay_info.xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。
			///@param error_info 现金还款发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnCreditCashRepay(XTPCrdCashRepayRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id) {};

			///融资融券业务中现金还息的响应
			///@param cash_repay_info 现金还息通知的具体信息，用户可以通过cash_repay_info.xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。
			///@param error_info 现金还息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnCreditCashRepayDebtInterestFee(XTPCrdCashRepayDebtInterestFeeRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id) {};

			///请求查询融资融券业务中的现金直接还款报单的响应
			///@param cash_repay_info 查询到的某一笔现金直接还款通知的具体信息
			///@param error_info 查询现金直接报单发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditCashRepayInfo(XTPCrdCashRepayInfo *cash_repay_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询信用账户额外信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param fund_info 查询到的信用账户额外信息情况
			///@param error_info 查询信用账户额外信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditFundInfo(XTPCrdFundInfo *fund_info, XTPRI *error_info, int request_id, uint64_t session_id) {};

			///请求查询信用账户负债信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param debt_info 查询到的信用账户合约负债情况
			///@param error_info 查询信用账户负债信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditDebtInfo(XTPCrdDebtInfo *debt_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询信用账户指定证券负债未还信息响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param debt_info 查询到的信用账户指定证券负债未还信息情况
			///@param error_info 查询信用账户指定证券负债未还信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditTickerDebtInfo(XTPCrdDebtStockInfo *debt_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询信用账户待还资金的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param remain_amount 查询到的信用账户待还资金
			///@param error_info 查询信用账户待还资金发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditAssetDebtInfo(double remain_amount, XTPRI *error_info, int request_id, uint64_t session_id) {};

			///请求查询信用账户可融券头寸信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param assign_info 查询到的信用账户可融券头寸信息
			///@param error_info 查询信用账户可融券头寸信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditTickerAssignInfo(XTPClientQueryCrdPositionStkInfo *assign_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///融资融券业务中请求查询指定余券信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param stock_info 查询到的余券信息
			///@param error_info 查询信用账户余券信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo* stock_info, XTPRI *error_info, int request_id, uint64_t session_id) {};

			///融资融券业务中请求查询余券信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param stock_info 查询到的余券信息
			///@param error_info 查询信用账户余券信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryMulCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo* stock_info, XTPRI *error_info, int request_id, uint64_t session_id, bool is_last) {};

			///融资融券业务中负债合约展期的通知
			///@param debt_extend_info 负债合约展期通知的具体信息，用户可以通过debt_extend_info.xtpid来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。
			///@param error_info 负债合约展期订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误。
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当负债合约展期订单有状态变化的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的负债合约展期通知。
			virtual void OnCreditExtendDebtDate(XTPCreditDebtExtendNotice *debt_extend_info, XTPRI *error_info, uint64_t session_id) {};

			///查询融资融券业务中负债合约展期订单响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param debt_extend_info 查询到的负债合约展期情况
			///@param error_info 查询负债合约展期发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误。当error_info.error_id=11000350时，表明没有记录，当为其他非0值时，表明合约发生拒单时的错误原因
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditExtendDebtDateOrders(XTPCreditDebtExtendNotice *debt_extend_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///查询融资融券业务中信用账户附加信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param fund_info 信用账户附加信息
			///@param error_info 查询信用账户附加信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditFundExtraInfo(XTPCrdFundExtraInfo *fund_info, XTPRI *error_info, int request_id, uint64_t session_id) {};

			///查询融资融券业务中信用账户指定证券的附加信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			///@param fund_info 信用账户指定证券的附加信息
			///@param error_info 查询信用账户附加信息发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnQueryCreditPositionExtraInfo(XTPCrdPositionExtraInfo *fund_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///期权组合策略报单通知
			///@param order_info 订单响应具体信息，用户可以通过order_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单，order_info.qty_left字段在订单为未成交、部成、全成、废单状态时，表示此订单还没有成交的数量，在部撤、全撤状态时，表示此订单被撤的数量。order_info.order_cancel_xtp_id为其所对应的撤单ID，不为0时表示此单被撤成功
			///@param error_info 订单被拒绝或者发生错误时错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 每次订单状态更新时，都会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，在订单未成交、全部成交、全部撤单、部分撤单、已拒绝这些状态时会有响应，对于部分成交的情况，请由订单的成交回报来自行确认。所有登录了此用户的客户端都将收到此用户的订单响应
			virtual void OnOptionCombinedOrderEvent(XTPOptCombOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) {};

			///期权组合策略成交通知
			///@param trade_info 成交回报的具体信息，用户可以通过trade_info.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。对于上交所，exec_id可以唯一标识一笔成交。当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交了。对于深交所，exec_id是唯一的，暂时无此判断机制。report_index+market字段可以组成唯一标识表示成交回报。
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 订单有成交发生的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的成交回报。相关订单为部成状态，需要用户通过成交回报的成交数量来确定，OnOrderEvent()不会推送部成状态。
			virtual void OnOptionCombinedTradeEvent(XTPOptCombTradeReport *trade_info, uint64_t session_id) {};

			///期权组合策略撤单出错响应
			///@param cancel_info 撤单具体信息，包括撤单的order_cancel_xtp_id和待撤单的order_xtp_id
			///@param error_info 撤单被拒绝或者发生错误时错误代码和错误信息，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 此响应只会在撤单发生错误时被回调
			virtual void OnCancelOptionCombinedOrderError(XTPOptCombOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) {};

			///请求查询期权组合策略报单响应-旧版本接口
			///@param order_info 查询到的一个报单
			///@param error_info 查询报单时发生错误时，返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。此对应的请求函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual void OnQueryOptionCombinedOrders(XTPQueryOptCombOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询期权组合策略报单响应-新版本接口
			///@param order_info 查询到的一个报单
			///@param error_info 查询报单时发生错误时，返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。此对应的请求函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual void OnQueryOptionCombinedOrdersEx(XTPOptCombOrderInfoEx *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///分页请求查询期权组合策略报单响应-旧版本接口
			///@param order_info 查询到的一个报单
			///@param req_count 分页请求的最大数量
			///@param order_sequence 分页请求的当前回报数量
			///@param query_reference 当前报单信息所对应的查询索引，需要记录下来，在进行下一次分页查询的时候需要用到
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当order_sequence为0，表明当次查询没有查到任何记录，当is_last为true时，如果order_sequence等于req_count，那么表示还有报单，可以进行下一次分页查询，如果不等，表示所有报单已经查询完毕。一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOptionCombinedOrdersByPage(XTPQueryOptCombOrderRsp *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

			///分页请求查询期权组合策略报单响应-新版本接口
			///@param order_info 查询到的一个报单
			///@param req_count 分页请求的最大数量
			///@param order_sequence 分页请求的当前回报数量
			///@param query_reference 当前报单信息所对应的查询索引，需要记录下来，在进行下一次分页查询的时候需要用到
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当order_sequence为0，表明当次查询没有查到任何记录，当is_last为true时，如果order_sequence等于req_count，那么表示还有报单，可以进行下一次分页查询，如果不等，表示所有报单已经查询完毕。一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOptionCombinedOrdersByPageEx(XTPOptCombOrderInfoEx *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询期权组合策略成交响应
			///@param trade_info 查询到的一个成交回报
			///@param error_info 查询成交回报发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。此对应的请求函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual void OnQueryOptionCombinedTrades(XTPQueryOptCombTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///分页请求查询期权组合策略成交响应
			///@param trade_info 查询到的一个成交信息
			///@param req_count 分页请求的最大数量
			///@param trade_sequence 分页请求的当前回报数量
			///@param query_reference 当前报单信息所对应的查询索引，需要记录下来，在进行下一次分页查询的时候需要用到
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 当trade_sequence为0，表明当次查询没有查到任何记录，当is_last为true时，如果trade_sequence等于req_count，那么表示还有回报，可以进行下一次分页查询，如果不等，表示所有回报已经查询完毕。一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOptionCombinedTradesByPage(XTPQueryOptCombTradeRsp *trade_info, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询期权组合策略持仓响应
			///@param position_info 查询到的一个持仓信息
			///@param error_info 查询持仓发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOptionCombinedPosition(XTPQueryOptCombPositionRsp *position_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///请求查询期权组合策略信息响应
			///@param strategy_info 查询到的一个组合策略信息
			///@param error_info 查询成交回报发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOptionCombinedStrategyInfo(XTPQueryCombineStrategyInfoRsp *strategy_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

			///查询期权行权合并头寸的响应
			///@param position_info 查询到的一个行权合并头寸信息
			///@param error_info 查询持仓发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param request_id 此消息响应函数对应的请求ID
			///@param is_last 此消息响应函数是否为request_id这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@param session_id 资金账户对应的session_id，登录时得到
			///@remark 一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。
			virtual void OnQueryOptionCombinedExecPosition(XTPQueryOptCombExecPosRsp *position_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {};

		};
	}
}

#ifndef WINDOWS
#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif
#endif

/*!
* \class XTP::API::TraderApi
*
* \brief 交易接口类
*
* \author 中泰证券股份有限公司
* \date 十月 2015
*/
namespace XTP {
	namespace API {

		class TRADER_API_EXPORT TraderApi
		{
		public:
			///创建TraderApi
			///@param client_id （必须输入）客户端id，用于区分同一用户的不同客户端，由用户自定义，普通用户必须使用1-99之间的数值，否则可能无法登录
			///@param save_file_path （必须输入）存贮订阅信息文件的目录，请设定一个真实存在的有可写权限的路径
			///@param log_level 日志输出级别
			///@return 创建出的UserApi
			///@remark 只能创建一次，如果一个账户需要在多个客户端登录，请使用不同的client_id，系统允许一个账户同时登录多个客户端，但是对于同一账户，相同的client_id只能保持一个session连接，后面的登录在前一个session存续期间，无法连接。系统不支持过夜，请确保每天开盘前重新启动
			static TraderApi *CreateTraderApi(uint8_t client_id, const char *save_file_path, XTP_LOG_LEVEL log_level = XTP_LOG_LEVEL_DEBUG);

			///删除接口对象本身
			///@remark 不再使用本接口对象时,调用该函数删除接口对象
			virtual void Release() = 0;

			///获取当前交易日
			///@return 获取到的交易日
			///@remark 只有登录成功后,才能得到正确的交易日
			virtual const char *GetTradingDay() = 0;

			///注册回调接口
			///@param spi 派生自回调接口类的实例，请在登录之前设定
			virtual void RegisterSpi(TraderSpi *spi) = 0;

			///获取API的系统错误
			///@return 返回的错误信息，可以在Login、InsertOrder、CancelOrder返回值为0时调用，获取失败的原因
			///@remark 可以在调用api接口失败时调用，例如login失败时
			virtual XTPRI *GetApiLastError() = 0;

			///获取API的发行版本号
			///@return 返回api发行版本号
			virtual const char* GetApiVersion() = 0;

			///通过报单在xtp系统中的ID获取下单的客户端id
			///@return 返回客户端id，可以用此方法过滤自己下的订单
			///@param order_xtp_id 报单在xtp系统中的ID
			///@remark 由于系统允许同一用户在不同客户端上登录操作，每个客户端通过不同的client_id进行区分
			virtual uint8_t GetClientIDByXTPID(uint64_t order_xtp_id) = 0;

			///通过报单在xtp系统中的ID获取相关资金账户名
			///@return 返回资金账户名
			///@param order_xtp_id 报单在xtp系统中的ID
			///@remark 只有资金账户登录成功后,才能得到正确的信息
			virtual const char* GetAccountByXTPID(uint64_t order_xtp_id) = 0;

			///订阅公共流。
			///@param resume_type 公共流（订单响应、成交回报）重传方式  
			///        XTP_TERT_RESTART:从本交易日开始重传
			///        XTP_TERT_RESUME:(保留字段，此方式暂未支持)从上次收到的续传
			///        XTP_TERT_QUICK:只传送登录后公共流的内容
			///@remark 该方法要在Login方法前调用。若不调用则不会收到公共流的数据。注意在用户断线后，如果不登出就login()，公共流订阅方式不会起作用。用户只会收到断线后的所有消息。如果先logout()再login()，那么公共流订阅方式会起作用，用户收到的数据会根据用户的选择方式而定。
			virtual void SubscribePublicTopic(XTP_TE_RESUME_TYPE resume_type) = 0;

			///设置软件开发版本号
			///@param version 用户开发软件版本号，非api发行版本号，长度不超过15位，以'\0'结尾
			///@remark 此函数必须在Login之前调用，标识的是客户端版本号，而不是API的版本号，由用户自定义
			virtual void SetSoftwareVersion(const char* version) = 0;

			///设置软件开发Key
			///@param key 用户开发软件Key，用户申请开户时给予，以'\0'结尾
			///@remark 此函数必须在Login之前调用
			virtual void SetSoftwareKey(const char* key) = 0;

			///设置心跳检测时间间隔，单位为秒
			///@param interval 心跳检测时间间隔，单位为秒
			///@remark 此函数必须在Login之前调用
			virtual void SetHeartBeatInterval(uint32_t interval) = 0;

			///用户登录请求
			///@return session_id表明此资金账号登录是否成功，“0”表示登录失败，可以调用GetApiLastError()来获取错误代码，非“0”表示登录成功，此时需要记录下这个返回值session_id，与登录的资金账户对应
			///@param ip 服务器地址，类似“127.0.0.1”
			///@param port 服务器端口号
			///@param user 登录用户名
			///@param password 登录密码
			///@param sock_type “1”代表TCP，“2”代表UDP，目前暂时只支持TCP
			///@param local_ip 本地网卡地址，类似“127.0.0.1”
			///@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api可支持多个账户连接，但是同一个账户同一个client_id只能有一个session连接，后面的登录在前一个session存续期间，无法连接
			virtual uint64_t Login(const char* ip, int port, const char* user, const char* password, XTP_PROTOCOL_TYPE sock_type, const char* local_ip = NULL) = 0;


			///登出请求
			///@return 登出是否成功，“0”表示登出成功，“-1”表示登出失败
			///@param session_id 资金账户对应的session_id,登录时得到
			virtual int Logout(uint64_t session_id) = 0;

			///服务器是否重启过
			///@return “true”表示重启过，“false”表示没有重启过
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 此函数必须在Login之后调用
			virtual bool IsServerRestart(uint64_t session_id) = 0;

			///修改已登录用户的硬件信息，仅限授权系统使用
			///@return 发送消息是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param info 需要修改成的用户硬件信息
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 此函数必须在Login之后调用，且仅限授权系统使用，一般客户无需使用
			virtual int ModifyUserTerminalInfo(const XTPUserTerminalInfoReq* info,uint64_t session_id) = 0;

			///报单录入请求
			///@return 报单在XTP系统中的ID,如果为‘0’表示报单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示报单发送成功，用户需要记录下返回的order_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param order 报单录入信息，其中order.order_client_id字段是用户自定义字段，用户输入什么值，订单响应OnOrderEvent()返回时就会带回什么值，类似于备注，方便用户自己定位订单。当然，如果你什么都不填，也是可以的。order.order_xtp_id字段无需用户填写，order.ticker必须不带空格，以'\0'结尾
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 交易所接收订单后，会在报单响应函数OnOrderEvent()中返回报单未成交的状态，之后所有的订单状态改变（除了部成状态）都会通过报单响应函数返回
			virtual uint64_t InsertOrder(XTPOrderInsertInfo *order, uint64_t session_id) = 0;

			///报单操作请求
			///@return 撤单在XTP系统中的ID,如果为‘0’表示撤单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示撤单发送成功，用户需要记录下返回的order_cancel_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param order_xtp_id 需要撤销的委托单在XTP系统中的ID
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 如果撤单成功，会在报单响应函数OnOrderEvent()里返回原单部撤或者全撤的消息，如果不成功，会在OnCancelOrderError()响应函数中返回错误原因
			virtual uint64_t CancelOrder(const uint64_t order_xtp_id, uint64_t session_id) = 0;

			///根据报单ID请求查询报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOrderByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id) = 0;

			///请求查询报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual int QueryOrders(const XTPQueryOrderReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询未完结报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryUnfinishedOrders(uint64_t session_id, int request_id) = 0;

			///分页请求查询报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要分页查询订单的条件，如果第一次查询，那么query_param.reference填0
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分页查询，注意用户需要记录下最后一笔查询结果的reference以便用户下次查询使用
			virtual int QueryOrdersByPage(const XTPQueryOrderByPageReq *query_param, uint64_t session_id, int request_id) = 0;

			///根据报单ID请求查询报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOrderByXTPIDEx(const uint64_t order_xtp_id, uint64_t session_id, int request_id) = 0;

			///请求查询报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual int QueryOrdersEx(const XTPQueryOrderReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询未完结报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryUnfinishedOrdersEx(uint64_t session_id, int request_id) = 0;

			///分页请求查询报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要分页查询订单的条件，如果第一次查询，那么query_param.reference填0
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分页查询，注意用户需要记录下最后一笔查询结果的reference以便用户下次查询使用
			virtual int QueryOrdersByPageEx(const XTPQueryOrderByPageReq *query_param, uint64_t session_id, int request_id) = 0;

			///根据委托编号请求查询相关成交
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的委托编号，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 此函数查询出的结果可能对应多个查询结果响应
			virtual int QueryTradesByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id) = 0;

			///请求查询已成交
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的成交回报筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有成交回报，否则查询时间段内所有跟股票代码相关的成交回报，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual int QueryTrades(XTPQueryTraderReq *query_param, uint64_t session_id, int request_id) = 0;

			///分页请求查询成交回报
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要分页查询成交回报的条件，如果第一次查询，那么reference填0
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分页查询，注意用户需要记录下最后一笔查询结果的reference以便用户下次查询使用
			virtual int QueryTradesByPage(const XTPQueryTraderByPageReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询投资者持仓
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param ticker 需要查询持仓的合约代码，可以为NULL，表示查询全市场，如果不为NULL，请不带空格，并以'\0'结尾，注意需与market匹配，不匹配的话，可能由于证券代码沪深2个市场有重复，而导致查询不到所需的持仓
			///@param market 需要查询持仓的合约所在市场，默认为0，仅在合约代码不为NULL的时候，才会使用。market不指定或者为非0的其他非有效值情况下，可能由于证券代码沪深2个市场有重复，而导致查询不到所需的持仓。如果想正确查询指定持仓，请指定market
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法如果用户提供了合约代码，则会查询此合约的持仓信息（注意请指定market，如果market为0，可能会查询到2个市场的持仓，如果market为其他非有效值，则查询结果会返回找不到持仓），如果合约代码为空，则默认查询所有持仓信息。
			virtual int QueryPosition(const char *ticker, uint64_t session_id, int request_id, XTP_MARKET_TYPE market = XTP_MKT_INIT) = 0;

			///请求查询资产
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryAsset(uint64_t session_id, int request_id) = 0;

			///请求查询分级基金
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的分级基金筛选条件，其中母基金代码可以为空，则默认所有存在的母基金，如果不为空，请不带空格，并以'\0'结尾，其中交易市场不能为空
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 此函数查询出的结果可能对应多个查询结果响应
			virtual int QueryStructuredFund(XTPQueryStructuredFundInfoReq *query_param, uint64_t session_id, int request_id) = 0;

			///资金划拨请求
			///@return 资金划拨订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的serial_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param fund_transfer 资金划拨的请求信息
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 此函数支持一号两中心节点之间的资金划拨，注意资金划拨的方向。
			virtual uint64_t FundTransfer(XTPFundTransferReq *fund_transfer, uint64_t session_id) = 0;

			///请求查询资金划拨
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的资金划拨订单筛选条件，其中serial_id可以为0，则默认所有资金划拨订单，如果不为0，则请求特定的资金划拨订单
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryFundTransfer(XTPQueryFundTransferLogReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询其他节点可用资金
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 查询时需要提供的信息
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOtherServerFund(XTPFundQueryReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询ETF清单文件
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的ETF清单文件的筛选条件，其中合约代码可以为空，则默认所有存在的ETF合约代码，market字段也可以为初始值，则默认所有市场的ETF合约
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryETF(XTPQueryETFBaseReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询ETF股票篮
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询股票篮的的ETF合约，其中合约代码不可以为空，market字段也必须指定
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryETFTickerBasket(XTPQueryETFComponentReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询今日新股申购信息列表
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryIPOInfoList(uint64_t session_id, int request_id) = 0;

			///请求查询用户新股申购额度信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryIPOQuotaInfo(uint64_t session_id, int request_id) = 0;

			///请求查询期权合约
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的期权合约的筛选条件，可以为NULL（为NULL表示查询所有的期权合约）
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOptionAuctionInfo(XTPQueryOptionAuctionInfoReq *query_param, uint64_t session_id, int request_id) = 0;

			///融资融券业务中现金直接还款请求
			///@return 现金直接还款订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param amount 现金还款的金额
			///@param session_id 资金账户对应的session_id,登录时得到
			virtual uint64_t CreditCashRepay(double amount, uint64_t session_id) = 0;

			///融资融券业务中现金还指定负债合约息费请求
			///@return 现金还息订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param debt_id 指定的负债合约编号
			///@param amount 现金还息的金额
			///@param session_id 资金账户对应的session_id,登录时得到
			virtual uint64_t CreditCashRepayDebtInterestFee(const char* debt_id, double amount, uint64_t session_id) = 0;

			///融资融券业务中卖券还指定负债合约息费请求
			///@return 卖券还息订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param order 卖券的报单录入信息，其中order.order_client_id字段是用户自定义字段，用户输入什么值，订单响应OnOrderEvent()返回时就会带回什么值，类似于备注，方便用户自己定位订单。当然，如果你什么都不填，也是可以的。order.order_xtp_id字段无需用户填写，order.ticker必须不带空格，以'\0'结尾
			///@param debt_id 指定的负债合约编号
			///@param session_id 资金账户对应的session_id,登录时得到
			virtual uint64_t CreditSellStockRepayDebtInterestFee(XTPOrderInsertInfo* order, const char* debt_id, uint64_t session_id) = 0;

			///请求查询融资融券业务中的现金直接还款报单
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditCashRepayInfo(uint64_t session_id, int request_id) = 0;


			///请求查询信用账户特有信息，除资金账户以外的信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditFundInfo(uint64_t session_id, int request_id) = 0;

			///请求查询信用账户负债合约信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditDebtInfo(uint64_t session_id, int request_id) = 0;

			///请求查询指定证券负债未还信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的指定证券，筛选条件中ticker可以全填0，如果不为0，请不带空格，并以'\0'结尾
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditTickerDebtInfo(XTPClientQueryCrdDebtStockReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询信用账户待还资金信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditAssetDebtInfo(uint64_t session_id, int request_id) = 0;

			///请求查询信用账户可融券头寸信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的证券，筛选条件中ticker可以全填0，如果不为0，请不带空格，并以'\0'结尾
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditTickerAssignInfo(XTPClientQueryCrdPositionStockReq *query_param, uint64_t session_id, int request_id) = 0;

			///融资融券业务中请求查询指定证券的余券
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的余券信息，不可以为空，需要明确指定
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法中用户必须提供了证券代码和所在市场
			virtual int QueryCreditExcessStock(XTPClientQueryCrdSurplusStkReqInfo *query_param, uint64_t session_id, int request_id) = 0;

			///融资融券业务中请求查询余券
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的余券信息。若填入市场和股票代码，返回单支股票信息；若市场代码为空，股票代码非空，是无效查询，会在SPI中返回错误；若市场和股票代码均为空，返回全市场信息；若市场代码非空，股票代码为空，返回单市场信息。
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryMulCreditExcessStock(XTPClientQueryCrdSurplusStkReqInfo *query_param, uint64_t session_id, int request_id) = 0;   

			///融资融券业务中请求负债合约展期
			///@return 负债合约展期订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param debt_extend 负债合约展期的请求信息
			///@param session_id 资金账户对应的session_id,登录时得到
			virtual uint64_t CreditExtendDebtDate(XTPCreditDebtExtendReq *debt_extend, uint64_t session_id) = 0;

			///融资融券业务中请求查询负债合约展期
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param xtp_id 需要查询的负债合约展期订单筛选条件，xtp_id可以为0，则默认所有负债合约展期订单，如果不为0，则请求特定的负债合约展期订单
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditExtendDebtDateOrders(uint64_t xtp_id, uint64_t session_id, int request_id) = 0;

			///请求查询融资融券业务中账戶的附加信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditFundExtraInfo(uint64_t session_id, int request_id) = 0;

			///请求查询融资融券业务中账戶指定证券的附加信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要指定的证券，筛选条件中ticker可以全填0，如果不为0，请不带空格，并以'\0'结尾
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryCreditPositionExtraInfo(XTPClientQueryCrdPositionStockReq *query_param, uint64_t session_id, int request_id) = 0;

			///期权组合策略报单录入请求
			///@return 报单在XTP系统中的ID,如果为‘0’表示报单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示报单发送成功，用户需要记录下返回的order_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param order 报单录入信息，其中order.order_client_id字段是用户自定义字段，用户输入什么值，订单响应OnOptionCombinedOrderEvent()返回时就会带回什么值，类似于备注，方便用户自己定位订单。当然，如果你什么都不填，也是可以的。order.order_xtp_id字段无需用户填写，order.ticker必须不带空格，以'\0'结尾
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 交易所接收订单后，会在报单响应函数OnOptionCombinedOrderEvent()中返回报单未成交的状态，之后所有的订单状态改变（除了部成状态）都会通过报单响应函数返回
			virtual uint64_t InsertOptionCombinedOrder(XTPOptCombOrderInsertInfo *order, uint64_t session_id) = 0;

			///期权组合策略报单撤单请求
			///@return 撤单在XTP系统中的ID,如果为‘0’表示撤单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示撤单发送成功，用户需要记录下返回的order_cancel_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
			///@param order_xtp_id 需要撤销的期权组合策略委托单在XTP系统中的ID
			///@param session_id 资金账户对应的session_id,登录时得到
			///@remark 如果撤单成功，会在报单响应函数OnOptionCombinedOrderEvent()里返回原单部撤或者全撤的消息，如果不成功，会在OnCancelOrderError()响应函数中返回错误原因
			virtual uint64_t CancelOptionCombinedOrder(const uint64_t order_xtp_id, uint64_t session_id) = 0;

			///请求查询期权组合策略未完结报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOptionCombinedUnfinishedOrders(uint64_t session_id, int request_id) = 0;

			///根据报单ID请求查询期权组合策略报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOptionCombinedOrderByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id) = 0;

			///请求查询期权组合策略报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual int QueryOptionCombinedOrders(const XTPQueryOptCombOrderReq *query_param, uint64_t session_id, int request_id) = 0;

			///分页请求查询期权组合策略报单-旧版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要分页查询订单的条件，如果第一次查询，那么query_param.reference填0
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分页查询，注意用户需要记录下最后一笔查询结果的reference以便用户下次查询使用
			virtual int QueryOptionCombinedOrdersByPage(const XTPQueryOptCombOrderByPageReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询期权组合策略未完结报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOptionCombinedUnfinishedOrdersEx(uint64_t session_id, int request_id) = 0;

			///根据报单ID请求查询期权组合策略报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			virtual int QueryOptionCombinedOrderByXTPIDEx(const uint64_t order_xtp_id, uint64_t session_id, int request_id) = 0;

			///请求查询期权组合策略报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual int QueryOptionCombinedOrdersEx(const XTPQueryOptCombOrderReq *query_param, uint64_t session_id, int request_id) = 0;

			///分页请求查询期权组合策略报单-新版本接口
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要分页查询订单的条件，如果第一次查询，那么query_param.reference填0
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分页查询，注意用户需要记录下最后一笔查询结果的reference以便用户下次查询使用
			virtual int QueryOptionCombinedOrdersByPageEx(const XTPQueryOptCombOrderByPageReq *query_param, uint64_t session_id, int request_id) = 0;

			///根据期权组合策略委托编号请求查询相关成交
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param order_xtp_id 需要查询的委托编号，即InsertOrder()成功时返回的order_xtp_id
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 此函数查询出的结果可能对应多个查询结果响应
			virtual int QueryOptionCombinedTradesByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id) = 0;

			///请求查询期权组合策略的成交回报
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的成交回报筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有成交回报，否则查询时间段内所有跟股票代码相关的成交回报，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
			virtual int QueryOptionCombinedTrades(const XTPQueryOptCombTraderReq *query_param, uint64_t session_id, int request_id) = 0;

			///分页请求查询期权组合策略成交回报
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要分页查询成交回报的条件，如果第一次查询，那么reference填0
			///@param session_id 资金账户对应的session_id，登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法支持分页查询，注意用户需要记录下最后一笔查询结果的reference以便用户下次查询使用
			virtual int QueryOptionCombinedTradesByPage(const XTPQueryOptCombTraderByPageReq *query_param, uint64_t session_id, int request_id) = 0;

			///请求查询投资者期权组合策略持仓
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询持仓的筛选条件，其中组合策略代码可以初始化为空，表示查询所有，如果不为空，请不带空格，并以'\0'结尾，注意需与market匹配，不匹配的话，可能导致查询不到所需的持仓
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法如果用户提供了合约代码，则会查询此合约的持仓信息（注意请指定market，如果market为0，可能会查询到2个市场的持仓，如果market为其他非有效值，则查询结果会返回找不到持仓），如果合约代码为空，则默认查询所有持仓信息。
			virtual int QueryOptionCombinedPosition(const XTPQueryOptCombPositionReq* query_param, uint64_t session_id, int request_id) = 0;

			///请求查询期权组合策略信息
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法仅支持精确查询，不支持模糊查询
			virtual int QueryOptionCombinedStrategyInfo(uint64_t session_id, int request_id) = 0;

			///请求查询期权行权合并头寸
			///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@param query_param 需要查询的行权合并的筛选条件，其中market为0会默认查询全市场，成分合约代码可以初始化为空，如果不为空，请不带空格，并以'\0'结尾，注意所有填写的条件都会进行匹配
			///@param session_id 资金账户对应的session_id,登录时得到
			///@param request_id 用于用户定位查询响应的ID，由用户自定义
			///@remark 该方法可能对应多条响应消息
			virtual int QueryOptionCombinedExecPosition(const XTPQueryOptCombExecPosReq* query_param, uint64_t session_id, int request_id) = 0;

		protected:
			~TraderApi() {};
		};

			}
}

#ifndef WINDOWS
#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif
#endif


#endif
