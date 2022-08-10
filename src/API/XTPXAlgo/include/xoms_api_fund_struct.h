/////////////////////////////////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xoms_api_fund_struct.h
///@brief 定义资金划拨相关结构体类型
/////////////////////////////////////////////////////////////////////////
#ifndef XOMS_API_FUND_STRUCT_H_
#define XOMS_API_FUND_STRUCT_H_

#include "xtp_api_data_type.h"
#include "xoms_api_struct.h"
#include "xtp_api_struct_common.h"

#pragma pack(8)

/// 用户资金账户的密码字符串长度
#define XTP_ACCOUNT_PASSWORD_LEN 64

/////////////////////////////////////////////////////////////////////////
///用户资金请求
/////////////////////////////////////////////////////////////////////////
struct XTPFundTransferReq
{
    ///资金内转编号，无需用户填写，类似于xtp_id
    uint64_t	serial_id;
	///资金账户代码
	char        fund_account[XTP_ACCOUNT_NAME_LEN];
	///资金账户密码
	char	    password[XTP_ACCOUNT_PASSWORD_LEN];
	///金额
	double	    amount;
	///内转类型
	XTP_FUND_TRANSFER_TYPE	transfer_type;

};

/////////////////////////////////////////////////////////////////////////
///用户资金划转请求的响应-复用资金通知结构体
/////////////////////////////////////////////////////////////////////////
typedef struct XTPFundTransferNotice XTPFundTransferAck ;

/////////////////////////////////////////////////////////////////////////
///用户资金查询请求结构体
/////////////////////////////////////////////////////////////////////////
struct XTPFundQueryReq
{
	///资金账户代码
	char        fund_account[XTP_ACCOUNT_NAME_LEN];
	///资金账户密码
	char	    password[XTP_ACCOUNT_PASSWORD_LEN];
	///查询类型
	XTP_FUND_QUERY_TYPE	query_type;
	///预留字段，用户无需填写
	uint64_t	unknown[4];

};

/////////////////////////////////////////////////////////////////////////
///用户资金查询响应结构体
/////////////////////////////////////////////////////////////////////////
struct XTPFundQueryRsp
{
	///金额
	double        amount;
	///查询类型
	XTP_FUND_QUERY_TYPE	query_type;
	///预留字段，用户无需填写
	uint64_t	unknown[4];

};

#pragma pack()

#endif