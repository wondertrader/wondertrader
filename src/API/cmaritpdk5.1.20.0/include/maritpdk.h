#pragma once



#include <map>
#include <vector>
#include <string>
using std::string;
using std::map;
using std::vector;

#include "itpdk/itpdk_typedef.h"
#include "itpdk/itpdk_dict.h"
#include "maritpdk/maritpdk_struct.h"
using namespace MarItpdk;


//////////////////////////////////////////////////////////////////////////
//获取时间戳毫秒级（仅linux，win同GetTickCount64）
ITPDK_API int64 MARITPDK_GetTickCount();
//获取时间戳微秒级（仅linux，win同GetTickCount64）
ITPDK_API int64 MARITPDK_GetDoubleTickCount();
//查询系统交易日期
ITPDK_API int64 MARITPDK_GetTradeDate();
////获取服务器系统日期
ITPDK_API int64 MARITPDK_GetSystemDate();
//获取修正过的时间(按交易服务器时间修正)
ITPDK_API int64 MARITPDK_GetReviseTimeAsLong();
//获取修正过的时间(按交易服务器时间修正)
ITPDK_API void MARITPDK_GetReviseTime(char *buffer);
//////////////////////////////////////////////////////////////////////////
//设置配置文件所在路径，ITPDK_Init 函数前调用生效
ITPDK_API void MARITPDK_SetProfilePath(const char *path);
// 设置是否启用ITPDK自动重连、切备功能，ITPDK_Init 函数前调用生效
ITPDK_API void MARITPDK_SetAutoReconnect(bool breconnect);
//设置是否记录日志
ITPDK_API void MARITPDK_SetWriteLog(bool bLog);
//设置是否记录Fix日志
ITPDK_API void MARITPDK_SetFixWriteLog(bool bLog);

//设置是日志路径，ITPDK_Init 函数前调用生效
ITPDK_API void MARITPDK_SetLogPath(const char *path);
//初始化
ITPDK_API bool MARITPDK_Init(int commsize);
//退出
ITPDK_API void MARITPDK_Exit();
//获取最近错误信息和错误码
ITPDK_API int64 MARITPDK_GetLastError(char *result_msg);
ITPDK_API int64 MARITPDK_GetLastError2(string& result_msg);
//设置最新错误信息
ITPDK_API void MARITPDK_SetLastError(int64 result_code, const char *result_msg, const char* result_text = nullptr, ITPDK_CusReqInfo* cusreqinfo = nullptr);

//获取当前库版本信息
ITPDK_API string MARITPDK_GetVersion();

/*
	设置信息回调函数
		消息数据pMsg为json串

		撤单消息 -- NOTIFY_PUSH_WITHDRAW
		废单消息 -- NOTIFY_PUSH_INVALID
		确认消息 -- NOTIFY_PUSH_ORDER
            WTH		int64		委托号
			KHH		char[]		客户号
            YYB     char[]      营业部
            GDH     char[]      股东号
			JYS		char[]		交易所
			ZQDM	char[]		证券代码
            XWH     char[]      席位号
            BZ      char[]      币种
            SBJG    int64        申报结果
            JYLB    int64        交易类别
            CXBZ    char        撤销标志
            CXWTH   int64        撤销委托号
            CJBH    char[]      成交编号
            CJSJ    char[]      成交时间
            CJSL    int64        成交数量
            CJJG    double      成交价格
            CJJE    doube       成交金额
		成交消息 -- NOTIFY_PUSH_MATCH
			KHH		char[]		客户号
			JYS		char[]		交易所
			ZQDM	char[]		证券代码
			WTH		int64		委托号
			CJSL	int64		成交数量
			CJJG	double		成交价格
		资产变动消息 -- NOTIFY_ASSETVARY
			KHH		char[]		客户号
			ZJZH	char[]		资金账号
			ZZC		double		总资产
			KYZJ	double		可用资金
			ZXSZ	double		最新市值

	func		[in]		回调函数
*/
ITPDK_API void MARITPDK_SetMsgCallback(pMessageCallbackMethod func);

// 异步回调函数
ITPDK_API void MARITPDK_SetFuncCallback(pAsyncCallbackFunc func);

// 连接掉线、恢复事件回调函数
/*
pConnEventCallback

pKhh		[in]		客户号
pConnKey	[in]		连接配置名
nEvent		[in]		事件:；0：恢复，1：断开
pData       [in]        保留字段
*/
ITPDK_API void MARITPDK_SetConnEventCallback(pConnEventCallback func);


// 设置站点信息
ITPDK_API bool MARITPDK_SetNode(const char *val);

// 设置委托方式
ITPDK_API bool MARITPDK_SetWTFS(const char *val);

/*
	交易登录
	lpKeyName	[in]		使用的配置Key值
	khh			[in]		客户号
	pwd			[in]		交易密码
	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_TradeLogin(const char *lpKeyName, const char *khh, const char *pwd);
/*
	交易登录
	lpKeyName	[in]		使用的配置Key值
	cusreqinfo	[in/out]	客户信息
	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_TradeLoginEx(const char* lpKeyName, ITPDK_CusReqInfo& cusreqinfo);

/*
	资金账号登录
	lpKeyName	[in]		使用的配置Key值
	zjzh		[in]		资金账号
	yyb			[in]		营业部
	pwd			[in]		交易密码
	sKHH		[out]		客户号，登录成功后客户号通过此参数返回，之后交易通过客户号进行交易
	return				成功返回>0，失败返回<=0
*/

ITPDK_API int64 MARITPDK_TradeLoginByFundAcc(const char* lpKeyName, const char* zjzh, const char* yyb, const char* pwd, string& sKHH);
/*
	资金账号登录
	lpKeyName	[in]		使用的配置Key值
	cusreqinfo	[in/out]	客户信息
	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_TradeLoginByFundAccEx(const char* lpKeyName, ITPDK_CusReqInfo& cusreqinfo);

/*
    登出 
    KhhOrZjzh			[in]		客户号
*/
ITPDK_API int64 MARITPDK_TradeLogout(const char* KhhOrZjzh);

/*
	密码修改

	lpKhh        [in]        客户号
	oldpwd       [in]        旧密码
	newpwd       [in]        新密码
	nType        [in]        修改类型（0 修改交易密码）
	return                成功返回true，失败返回false
*/
ITPDK_API bool MARITPDK_ModifyPwd(const char* lpKhh, const char* oldpwd, const char* newpwd, int nType);

//////////////////////////////////////////////////////////
//交易类接口
//////////////////////////////////////////////////////////

/*
	信用交易

	lpKhh		[in]		客户号
	lpJys		[in]		交易所
	lpZqdm		[in]		证券代码
	nJylb		[in]		交易类别(JYLB_BUY、JYLB_SALE、JYLB_RZMR、JYLB_MQHK、JYLB_MQHQ、JYLB_RQMC、JYLB_DBWTJ、JYLB_DBWFH、JYLB_HQHZ、JYLB_ZJHK、JYLB_PHDJMR、JYLB_PHDJMC、JYLB_ZZZG、JYLB_PGJK、JYLB_PSSG、JYLB_ZZHS、JYLB_KJHZHG、JYLB_HSCX)
	lWtsl		[in]		委托数量
	lfWtjg		[in]		委托价格(订单类型为市价时，该参数不生效。交易类别为 直接还款时，还款金额填到此字段)
	lDdlx		[in]		订单类型(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		股东号
	nTCLX		[in]		头寸类型  1普通头寸 2专项头寸(开仓: 送0 默认专项 ,偿还: 送0 偿还所有类型)
	lpHYBH		[in]		指定偿还负债流水号
	nChlx		[in]		偿还类型: 0-按了结方式) 1-只还利息
	nFlag		[in]	    撤单允许标志（还券划转业务专用） 0代表实时影响负债 1代表不实时影响负 -1代表没有送入撤单允许标志的委托
	return				成功返回委托号，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginEntrust(const char *lpKhh,const char *lpJys,const char *lpZqdm,int nJylb,int64 lWtsl,double lfWtjg, int64 lDdlx,const char *lpGdh,int nTCLX,const char * lpHYBH,int nChlx,int nFlag);
ITPDK_API int64 MARITPDK_MarginEntrustEx(ITPDK_CusReqInfo& cusreqinfo, const char* lpJys, const char* lpZqdm, int nJylb, int64 lWtsl, double lfWtjg, int64 lDdlx, int nTCLX, const char* lpHYBH, int nChlx, int nFlag);

/*
	信用交易接口(异步)

	lpKhh		[in]		客户号
	lpJys		[in]		交易所
	lpZqdm		[in]		证券代码
	nJylb		[in]		交易类别(JYLB_BUY、JYLB_SALE、JYLB_RZMR、JYLB_MQHK、JYLB_MQHQ、JYLB_RQMC、JYLB_DBWTJ、JYLB_DBWFH、JYLB_HQHZ、JYLB_ZJHK、JYLB_PHDJMR、JYLB_PHDJMC、JYLB_ZZZG、JYLB_PGJK、JYLB_PSSG、JYLB_ZZHS、JYLB_KJHZHG、JYLB_HSCX)
	lWtsl		[in]		委托数量
	lfWtjg		[in]		委托价格(订单类型为市价时，该参数不生效)
	lDdlx		[in]		订单类型(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		股东号
	nTCLX		[in]		头寸类型  1普通头寸 2专项头寸(开仓: 送0 默认专项 ,偿还: 送0 偿还所有类型)
	lpHYBH		[in]		指定偿还负债流水号
	nChlx		[in]		偿还类型: 0-按了结方式) 1-只还利息
	nFlag		[in]	    撤单允许标志（还券划转业务专用） 0代表实时影响负债 1代表不实时影响负
	nKFSBDBH    [in]        开发商本地编号，若未送，则由itpdk自动生成(自定义委托号，在确认、成交等推送消息中会包含该值)
	return				成功返回开发商本地编号，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginEntrust_ASync(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, int64 lWtsl, double lfWtjg, int64 lDdlx, const char* lpGdh, int nTCLX, const char* lpHYBH, int nChlx,  int nFlag, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginEntrustEx_ASync(ITPDK_CusReqInfo& cusreqinfo, const char* lpJys, const char* lpZqdm, int nJylb, int64 lWtsl, double lfWtjg, int64 lDdlx, int nTCLX, const char* lpHYBH,  int nChlx ,  int nFlag, int64 nKFSBDBH = 0);


/*
	信用委托普通撤单

	lpKhh		[in]		客户号
	lpGdh		[in]		股东号
	lWth		[in]		委托号
    nJylb       [in]       交易类别
	return				成功返回>0，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginOrderWithdraw(const char *lpKhh,const char *lpGdh, int64 lWth,int nJylb);
ITPDK_API int64 MARITPDK_MarginOrderWithdrawEx(ITPDK_CusReqInfo& cusreqinfo, int64 lWth, int nJylb);
/*
	信用委托普通撤单(异步)

	lpKhh		[in]		客户号
	lpGdh		[in]		股东号
	lWth		[in]		委托号
	nJylb       [in]       交易类别
	nKFSBDBH	[in]        流水号：透传字段，在回调函数中通过该值匹配调用；
								若未送该字段，由接口内部生成。
	return				成功返回>0，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginOrderWithdraw_ASync(const char* lpKhh, const char* lpGdh, int64 lWth, int nJylb, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginOrderWithdrawEx_ASync(ITPDK_CusReqInfo& cusreqinfo, int64 lWth, int nJylb, int64 nKFSBDBH = 0);
/*
	信用委托开发商代码撤单

	lpKhh		[in]		客户号
	lpGdh		[in]		股东号
	nCxkfsbdh	[in]		撤销委托号
	nJylb       [in]       交易类别
	return				成功返回>0，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdraw(const char* lpKhh, const char* lpGdh, int64 nCxkfsbdh, int nJylb);
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdrawEx(ITPDK_CusReqInfo& cusreqinfo, int64 nCxkfsbdh, int nJylb);

/*
	信用委托开发商代码撤单(异步)

	lpKhh		[in]		客户号
	lpGdh		[in]		股东号
	nCxkfsbdh	[in]		撤销委托号
	nJylb       [in]       交易类别
	nKFSBDBH	[in]        流水号：透传字段，在回调函数中通过该值匹配调用；
								若未送该字段，由接口内部生成。
	return				成功返回>0，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdraw_ASync(const char* lpKhh, const char* lpGdh, int64 nCxkfsbdh, int nJylb, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginOrderKFSDMWithdrawEx_ASync(ITPDK_CusReqInfo& cusreqinfo, int64 nCxkfsbdh, int nJylb, int64 nKFSBDBH = 0);

/*
    担保品划转（3rd接口）

    lpKhh		[in]		客户号
    lpJymm      [in]        交易密码
    lpJys       [in]        交易所
    lpZqdm      [in]        证券代码
    nJylb       [in]        交易类别
    lWtsl       [in]        委托数量
    lDdlx       [in]        订单类型
    lpDFGDH     [in]        对方股东号
    lpDFXW      [in]        对方席位
    lpGdh		[in]		股东号
    return				成功返回>0，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginGuarantyTran(const char* lpKhh, const char* lpJymm, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx, const char* lpDFGDH, const char* lpDFXW, const char *lpGdh);
ITPDK_API int64 MARITPDK_MarginGuarantyTranEx(ITPDK_CusReqInfo& cusreqinfo, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx); 

/*
    担保品划转(异步)（3rd接口）

    lpKhh		[in]		客户号
    lpJymm      [in]        交易密码
    lpJys       [in]        交易所
    lpZqdm      [in]        证券代码
    nJylb       [in]        交易类别
    lWtsl       [in]        委托数量
    lDdlx       [in]        订单类型
    lpDFGDH     [in]        对方股东号
    lpDFXW      [in]        对方席位
    lpGdh		[in]		股东号
    nKFSBDBH	[in]        流水号：透传字段，在回调函数中通过该值匹配调用；
                                若未送该字段，由接口内部生成。
    return				成功返回>0，失败返回<0
*/
ITPDK_API int64 MARITPDK_MarginGuarantyTran_ASync(const char* lpKhh, const char* lpJymm, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx, const char* lpDFGDH, const char* lpDFXW, const char *lpGdh, int64 nKFSBDBH = 0);
ITPDK_API int64 MARITPDK_MarginGuarantyTranEx_ASync(ITPDK_CusReqInfo& cusreqinfo, const char *lpJys, const char *lpZqdm, int nJylb, int64 lWtsl, int64 lDdlx, int64 nKFSBDBH = 0);


/*
	信用可委托数量计算
	lpKhh		[in]		客户号
	lpJys		[in]		交易所
	lpZqdm		[in]		证券代码
	nJylb		[in]		交易类别(JYLB_BUY、JYLB_SALE)
	lfWtjg		[in]		委托价格(订单类型为市价时，该参数不生效)
	lDdlx		[in]		订单类型(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		股东号
	lpDfGdh		[in]		对方股东号
	nTCLX		[in]		头寸类型  1普通头寸 2专项头寸
	nCHBZ       [in]        偿还标志：0-所有 1-普通 2-专项
	lpHYBH      [in]        合约编号（还券划转可指定合约编号）

*/
ITPDK_API int64 MARITPDK_MarginTradableQty(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, int64 lDdlx, const char* lpGdh, const char* lpDfGdh, int nTCLX, int nCHBZ, const char* lpHYBH);
ITPDK_API int64 MARITPDK_MarginTradableQtyEx(ITPDK_CusReqInfo& cusreqinfo,const char *lpJys,const char *lpZqdm,int nJylb,double lfWtjg, int64 lDdlx ,int nTCLX, int nCHBZ, const char* lpHYBH);


//////////////////////////////////////////////////////////
//查询类接口
//////////////////////////////////////////////////////////


/*
	查询资金信息

	khh			[in]		客户号
	arInfo		[out]		结果数据
	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryFundInfo(const char* khh, vector<ITPDK_XYZJZH>& arInfo);
ITPDK_API int64 MARITPDK_QueryFundInfoEx(ITPDK_CusReqInfo& cusreqinfo, vector<ITPDK_XYZJZH>& arInfo);

/*
	查询股东信息

	khh			[in]		客户号
	arInfo		[out]		结果数据
	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryAccInfo(const char* khh, vector<ITPDK_XYGDH>& arInfo);
ITPDK_API int64 MARITPDK_QueryAccInfoEx(ITPDK_CusReqInfo& cusreqinfo, vector<ITPDK_XYGDH>& arInfo);

/*
	查询信用负债

	lpKhh		[in]		客户号
	lpJys		[in]		交易所
	lpGdh		[in]		股东号
	lpZqdm		[in]		证券代码
	lpHybh		[in]		合约编号
	nJylb		[in]		交易类别（A5柜台为合约类别：0-全部，1-融资，2-融券）
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	nKsrq		[in]		开始日期
	nJsrq		[in]		结束日期
	lWth		[in]		委托号
	nType		[in]		是否了结(0-所有合约,1-已了结 2-未了结；A5柜台为：0-所有，1-欠款，2-欠券，3-未了结，4-已了结)
	nTclx		[in]		头寸类型(0 所有类型 1 普通 2 专项)
	arInfo		[out]		结果数据
	return  返回条数

*/
ITPDK_API int64 MARITPDK_QueryDebts(const char* lpKhh, const char* lpJys, const char* lpGdh, const char* lpZqdm, const char* lpHybh, int64 nJylb, int64 nRowcount, const char* nBrowindex, int64 nKsrq, int64 nJsrq, int64 lWth, int64 nType, int64 nTclx, vector<ITPDK_XYFZ>& arInfo);
ITPDK_API int64 MARITPDK_QueryDebtsEx(ITPDK_CusReqInfo& cusreqinfo, const char * lpJys,const char * lpZqdm ,const char * lpHybh, int64 nJylb, int64 nRowcount, const char * nBrowindex, int64 nKsrq, int64 nJsrq, int64 lWth, int64 nType, int64 nTclx, vector<ITPDK_XYFZ>& arInfo);


/*
	两融查询当日委托

	khh			[in]		客户号
	nType		[in]		是否仅查询可撤委托（0查询全部，1查询可撤）
	nSortType	[in]		排序方式，当前仅支持逆序
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	jys			[in]		交易所（为空时查询所有交易所）
	zqdm		[in]		证券代码（为空时查询所有代码）
	nJylb		[in]		交易类别
	sCxbz		[in]		撤销标志
	lWth		[in]		委托号（为空时查询所有委托号）
	arInfo		[out]		结果数据
	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryOrders_MR_DRWT(const char* khh, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char* jys, const char* zqdm, int nJylb, const char* sCxbz, int64 lWth, vector<ITPDK_XYDRWT>& arInfo);
ITPDK_API int64 MARITPDK_QueryOrders_MR_DRWTEx(ITPDK_CusReqInfo& cusreqinfo, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char *jys, const char *zqdm, int nJylb, const char *sCxbz, int64 lWth, vector<ITPDK_XYDRWT>& arInfo);

/*
	两融查询实时成交


	khh			[in]		客户号
	nType		[in]		是否查询可撤单委托部分（0查询全部，1 过滤 撤单数量CDSL>0部分）  
	nSortType	[in]		排序方式，0正序，1逆序
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	jys			[in]		交易所（为空时查询所有交易所）
	zqdm		[in]		证券代码（为空时查询所有代码）
	nJylb		[in]		交易类别	
	lWth		[in]		委托号（为空时查询所有委托号）
	arInfo		[out]		结果数据
	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryMatchs_MR_SSCJ(const char *khh, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char *jys, const char *zqdm,int nJylb, int64 lWth, vector<ITPDK_XYSSCJ>& arInfo);
ITPDK_API int64 MARITPDK_QueryMatchs_MR_SSCJEx(ITPDK_CusReqInfo& cusreqinfo, int nType, int nSortType, int nRowcount, int64 nBrowindex, const char* jys, const char* zqdm, int nJylb, int64 lWth, vector<ITPDK_XYSSCJ>& arInfo);

/*
	两融查询持仓


	khh			[in]		客户号
	nFlag		[in]		扩展标志（0不扩展，1扩展）
	nSortType	[in]		排序方式，当前仅支持逆序
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	gdh			[in]		股东号
	jys			[in]		交易所（为空时查询所有交易所）
	zqdm		[in]		证券代码（为空时查询所有代码）
	arInfo		[out]		结果数据
	
	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryPosition_MR_ZQGL(const char *khh, int nFlag, int nSortType, int nRowcount, int64 nBrowindex,const char * gdh, const char *jys, const char *zqdm, vector<ITPDK_XYZQGL>& arInfo);
ITPDK_API int64 MARITPDK_QueryPosition_MR_ZQGLEx(ITPDK_CusReqInfo& cusreqinfo, int nFlag, int nSortType, int nRowcount, int64 nBrowindex, const char* jys, const char* zqdm, vector<ITPDK_XYZQGL>& arInfo);

/*
	两融查询信用资产


	khh			[in]		客户号
	jys			[in]		交易所
	zqdm		[in]		证券代码
	nFlag		[in]		扩展标志（0不扩展，1扩展）
	
	arInfo		[out]		结果数据
	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_XYZC(const char *khh, const char* jys, const char* zqdm, int nFlag , vector<ITPDK_XYZC>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_XYZCEx(ITPDK_CusReqInfo& cusreqinfo, const char* jys, const char* zqdm, int nFlag, vector<ITPDK_XYZC>& arInfo);


/*
	两融查询客户可融券信息


	khh			[in]		客户号
	jys			[in]		交易所
	zqdm		[in]		证券代码
	tcxz		[in]		头寸来源(1 普通头寸 2 专项头寸)
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRQXX(const char* khh, const char* jys, const char* zqdm, int32 tcxz, int nRowcount, int64 nBrowindex, vector<ITPDK_RQZQ>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRQXXEx(ITPDK_CusReqInfo& cusreqinfo, const char* jys, const char* zqdm, int32 tcxz, int nRowcount, int64 nBrowindex, vector<ITPDK_RQZQ>& arInfo);

/*
	两融查询客户可融资金信息


	khh			[in]		客户号
	tcxz		[in]		头寸来源(1 普通头寸 2 专项头寸)
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRZJXX(const char* khh, int32 tcxz, vector<ITPDK_KRZJXX>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRZJXXEx(ITPDK_CusReqInfo& cusreqinfo, int32 tcxz, vector<ITPDK_KRZJXX>& arInfo);


/*
	两融查询客户配售权益


	khh			[in]		客户号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_PSQY(const char* khh, vector<ITPDK_XYPSQY>& arInfo);
ITPDK_API int64 MARITPDK_QueryTradeData_MR_PSQYEx(ITPDK_CusReqInfo& cusreqinfo, vector<ITPDK_XYPSQY>& arInfo);

/*
	发行业务可认购数量

	lpKhh		[in]		客户号
	lpJys		[in]		交易所
	lpZqdm		[in]		ETF代码
	nJylb		[in]		交易类别(JYLB_PGJK配股缴款,JYLB_PSSG配售申购,JYLB_BUY增发买入)
	lfWtjg		[in]		委托价格(订单类型为市价时，该参数不生效)
	lDdlx		[in]		订单类型(DDLX_XJWT,DDLX_SJWT)
	lpGdh		[in]		股东号
	return				成功返回可申购数量，失败返回<0
*/
ITPDK_API int64 MARITPDK_FXYWTradableQty(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, int32 lDdlx, const char* lpGdh);
ITPDK_API int64 MARITPDK_FXYWTradableQtyEx(ITPDK_CusReqInfo& cusreqinfo, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, int32 lDdlx);

/*
	两融查询证券代码


	jys			[in]		交易所
	zqdm		[in]		证券代码
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_ZQDM(const char* jys, const char* zqdm, int nRowcount, int64 nBrowindex, vector<ITPDK_XYZQDM>& arInfo);

/*
	两融查询资格证券


	jys			[in]		交易所
	zqdm		[in]		证券代码
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_ZGZQ(const char* jys, const char* zqdm, int nRowcount, int64 nBrowindex, vector<ITPDK_ZGZQ>& arInfo);


/*
	查询集中交易可用资金

	lpKhh		[in]		客户号
	sJymm		[in]		交易密码
	sZjzh		[in]		资金账号
	dKyzj       [out]       查询成功,返回可用资金
	return				成功返回>=0，失败返回<0
*/
ITPDK_API int64 MARITPDK_QueryCentreFundAvl(const char* sKhh, const char* sJymm, const char* sZjzh, double& dKyzj, const char* sGyh = nullptr, const char* sGymm = nullptr);

//资金调入
/*

	lpKhh		[in]		客户号
	sJymm		[in]		交易密码
	dFsje		[in]		发生金额
	sZjzh		[in]		资金账号
	sGyh        [in]		柜员号
	sGymm        [in]		柜员密码
	sZy		    [in]		摘要
	return				成功返回>=0，失败返回<0
*/
ITPDK_API int64 MARITPDK_FundTransIn(const char* sKhh, const char* sJymm, double dFsje, const char* sZjzh = nullptr, const char* sGyh = nullptr, const char* sGymm = nullptr, const char* sZy = nullptr);
//资金调出
ITPDK_API int64 MARITPDK_FundTransOut(const char* sKhh, const char* sJymm, double dFsje, const char* sZjzh = nullptr, const char* sGyh = nullptr, const char* sGymm = nullptr, const char* sZy = nullptr);

/*
查询A股可担保划入持仓（东北）


khh			[in]		客户号
arInfo		[out]		结果数据

return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryPosition_DBHR_All(const char *khh, vector<ITPDK_XYDBHRSL>& arInfo);

/*
	专项头寸证券调拨（中信）
	sKhh		[in]		客户号
	sJys		[in]		交易所
	sZqdm		[in]		证券代码
	nFssl		[in]		调拨数量
	nYysl		[in]		预约数量到期增量
	nFlag		[in]		调拨方向,0为MDB调出到柜台，1位柜台调入MDB
	arInfo		[out]		结果数据

	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_SpecialPosition_ZQDB(const char* sKhh, const char* sJys, const char* sZqdm, int64 nFssl, int64 nYysl, int64 nFlag, vector<ITPDK_ZXTCZQDB>& arInfo);

/*
	专项头寸资金调拨（中信）
	sKhh		[in]		客户号
	dFsje		[in]		发生金额
	nFlag		[in]		调拨方向,0为MDB调出到柜台，1位柜台调入MDB
	arInfo		[out]		结果数据

	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_SpecialPosition_ZJDB(const char* sKhh, double dFsje, int64 nFlag);

/*
	查询证券黑名单
	sJys		[in]		交易所
	sXysx		[in]		信用属性 信用属性 "01" - 集中度  "02" - 黑名单
	nType		[in]		类型(1-集中度证券代码 2-集中度证券类别 3-证券黑名单证券代码)
	arInfo		[out]		结果数据

	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_ZQHMD(const char* sJys, const char* sXysx, long nType, vector<ITPDK_ZQHMD>& arInfo);

/*
	查询新股申购信息
	sJys		[in]		交易所
	sZqdm		[in]		证券代码
	nRgrq		[in]		认购日期
	sFxfs		[in]		发行方式
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryNewStockInfo(const char* sJys, const char* sZqdm, int64 nRgrq, const char* sFxfs, vector<ITPDK_XGSG>& arInfo);

/*
	查询负债变动明细
	sKhh		[in]		客户号
	sJys		[in]		交易所
	sZqdm		[in]		证券代码
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_DRBD(const char* sKhh, const char* sJys, const char* sZqdm, int nRowcount, int nBrowindex, vector<ITPDK_FZBDMX>& arInfo);

/*
	融资仓单明细查询（光大证券）
	sKhh		[in]		客户号
	sJymm		[in]		交易密码
	sYyb		[in]		营业部
	sJys		[in]		交易所
	sBz			[in]		币种
	sKsrq		[in]		开始日期
	sJsrq		[in]		结束日期
	sWtfs		[in]		委托方式
	sRqlx		[in]		日期输入类型（0:仓单发生日期 1:仓单到期日期，默认为 0）
	nLjbs		[in]		了结标识
	sFlag		[in]		是否包含实时委托（0或空:否 1:是）
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_RZCDMX(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sBz, const char* sKsrq, const char* sJsrq, const char* sWtfs, const char* sRqlx, int nLjbs, const char* sFlag, vector<ITPDK_RZCDMX>& arInfo);

/*
	融券仓单明细查询（光大证券）
	sKhh		[in]		客户号
	sJymm		[in]		交易密码
	sYyb		[in]		营业部
	sJys		[in]		交易所
	sBz			[in]		币种
	sKsrq		[in]		开始日期
	sJsrq		[in]		结束日期
	sWtfs		[in]		委托方式
	sRqlx		[in]		日期输入类型（0:仓单发生日期 1:仓单到期日期，默认为 0）
	nLjbs		[in]		了结标识
	sFlag		[in]		是否包含实时委托（0或空:否 1:是）
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_RQCDMX(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sBz, const char* sKsrq, const char* sJsrq, const char* sWtfs, const char* sRqlx, int nLjbs, const char* sFlag, vector<ITPDK_RQCDMX>& arInfo);

/*
	融资融券展期（光大证券）
	sKhh		[in]		客户号
	sJymm		[in]		交易密码
	sYyb		[in]		营业部
	sJys		[in]		交易所
	sCdh		[in]		仓单号
	sHylx		[in]		合约类型
	sKcrq		[in]		开仓日期
	sKsrq		[in]		开始日期
	sJsrq		[in]		结束日期
	sWtfs		[in]		委托方式

	return				成功返回>=0，失败返回<0
*/
ITPDK_API int64 MARITPDK_ContractExtension(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sCdh, const char* sHylx, const char* sKcrq, const char* sKsrq, const char* sJsrq, const char* sWtfs);

/*
	仓单变更申请拒绝原因查询（光大证券）
	sKhh		[in]		客户号
	sJymm		[in]		交易密码
	sYyb		[in]		营业部
	sJys		[in]		交易所
	sKsrq		[in]		开始日期
	sJsrq		[in]		结束日期
	sWtfs		[in]		委托方式
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_CDBGSQ_REFUSE(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sKsrq, const char* sJsrq, const char* sWtfs, vector<ITPDK_CDBGSQJJYYCX>& arInfo);

/*
	仓单变更申请查询（光大证券）
	sKhh		[in]		客户号
	sJymm		[in]		交易密码
	sYyb		[in]		营业部(源)
	sJys		[in]		交易所
	sJgdm		[in]		营业部
	sZqdm		[in]		证券代码
	sKsrq		[in]		开始日期
	sJsrq		[in]		结束日期
	sHylx		[in]		合约类型
	sTzlx		[in]		调整类型
	sCpbh		[in]		产品编号
	sSqzt		[in]		申请状态
	sWtfs		[in]		委托方式
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_CDBGSQ(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sJgdm, const char* sZqdm, const char* sKsrq, const char* sJsrq, const char* sHylx, const char* sTzlx, const char* sCpbh, const char* sSqzt, const char* sWtfs, vector<ITPDK_CDBGSQCX>& arInfo);


/*
	查询新股中签（光大证券）

	sKhh		[in]        客户号
	sJymm		[in]        交易密码
	sYyb		[in]		营业部
	sJys		[in]        市场代码
	sZqdm		[in]        证券代码
	sKsrq		[in]        起始日期
	sJsrq		[in]        结束日期
	nRowCount   [in]        查询行数
	sSyrq		[in]        索引日期
	sSyYybdm	[in]        索引营业部代码
	sBrowindex	[in]        索引流水号
	sWtfs		[in]        委托方式
	arInfo      [out]       结果数据

	return                  返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_XGZQ(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sZqdm, const char* sKsrq, const char* sJsrq, int nRowCount, const char* sSyrq, const char* sSyYybdm, const char* sBrowindex, const char* sWtfs, vector<ITPDK_XGZQ>& arInfo);

/*
	配号查询（光大证券）

	sKhh		[in]        客户号
	sJymm		[in]        交易密码
	sYyb		[in]        营业部
	sJys		[in]        市场代码
	sGdh		[in]        股东代码
	sPhdm		[in]        配号代码
	sKsrq		[in]        起始日期
	sJsrq		[in]        结束日期
	nRowCount   [in]        查询行数
	sBrowindex	[in]        索引流水号
	sWtfs		[in]        委托方式

	arInfo      [out]       结果数据
	return                  返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_PHCX(const char* sKhh, const char* sJymm, const char* sYyb, const char* sJys, const char* sGdh, const char* sPhdm, const char* sKsrq, const char* sJsrq, int nRowCount, const char* sBrowindex, const char* sWtfs, vector<ITPDK_PHCX>& arInfo);

/*
	查询银证业务流水（光大证券）

	sKhh		[in]        客户号
	sJymm		[in]        交易密码
	sYyb		[in]		营业部
	sScdm		[in]        市场代码
	sYhdm		[in]        银行代码
	sYhzh		[in]        银行账号
	nRowCount   [in]        查询行数
	sLsh		[in]        流水号
	sWtfs		[in]        委托方式
	sBrowindex	[in]		索引流水号
	nFlag		[in]        是否取辅账户流水（0:否 1:是，默认为 0）

	arInfo      [out]       结果数据
	return                  返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_YZYWLS(const char* sKhh, const char* sJymm, const char* sYyb, const char* sScdm, const char* sYhdm, const char* sYhzh, int nRowCount, const char* sLsh, const char* sWtfs, const char* sBrowindex, int nFlag, vector<ITPDK_YZYWLS>& arInfo);

/*
	银证转账业务（光大证券）

	sKhh		[in]        客户号
	sJymm		[in]        交易密码
	sZjzh		[in]        资金账号
	sYyb		[in]		营业部
	sScdm		[in]        市场代码
	sYhdm		[in]        银行代码
	sYhzh		[in]        银行账号
	nYwlb       [in]        业务类别（1:银行转证券、2:证券转银行）
	dZzje		[in]        转账金额
	sYhmm		[in]        银行密码
	sQkmm		[in]        取款密码
	sWtfs		[in]        委托方式

	return				成功返回>=0，失败返回<0
*/
ITPDK_API int64 MARITPDK_BankBusiness(const char* sKhh, const char* sJymm, const char* sZjzh, const char* sYyb, const char* sScdm, const char* sYhdm, const char* sYhzh, int nYwlb, double dZzje, const char* sYhmm, const char* sQkmm, const char* sWtfs);

/*
	查询客户银行代码（光大证券）

	sKhh		[in]        客户号
	sJymm		[in]        交易密码
	sYyb		[in]		营业部
	sScdm		[in]        市场代码
	sWtfs		[in]        委托方式
	nYhlb		[in]		银行类别
	nFlag		[in]		是否取辅账户流水（0:否，1:是，默认为0）

	arInfo      [out]       结果数据
	return                  返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_YHDM(const char* sKhh, const char* sJymm, const char* sYyb, const char* sScdm, const char* sWtfs, int nYhlb, int nFlag, vector<ITPDK_YHDM>& arInfo);

/*
	查询银行余额（光大证券）

	sKhh		[in]        客户号
	sJymm		[in]        交易密码
	sZjzh		[in]        资金账号
	sYhdm		[in]        银行代码
	sYhzh		[in]        银行账号
	sYhmm		[in]        银行密码
	sQkmm		[in]        取款密码

	arInfo      [out]       结果数据
	return                  返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_YHYE(const char* sKhh, const char* sJymm, const char* sZjzh, const char* sYhdm, const char* sYhzh, const char* sYhmm, const char* sQkmm, vector<ITPDK_YHYE>& arInfo);

/*
	查询客户信息

	sKhh        [in]        客户号

	arInfo      [out]       结果数据
	return                  返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_KHXX(const char* sKhh, vector<ITPDK_KHXX>& arInfo);

/*
	查询客户资金冻结明细

	sKhh		[in]        客户号
	sJymm		[in]        交易密码
	sZjzh		[in]        资金账号
	nDjlb		[in]        冻结类别
	sLsh		[in]        流水号
	nRowCount   [in]        查询行数
	nBrowindex	[in]		分页索引值

	arInfo      [out]       结果数据
	return                  返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_ZJDJMX(const char* sKhh, const char* sJymm, const char* sZjzh, int64 nDjlb, const char* sLSH, int64 nRowCount, int64 nBrowIndex, vector<ITPDK_ZJDJMX>& arInfo);

/*
	客户其他负债调整(中信证券)

	lpKeyName	[in]		使用的配置Key值
	sKhh		[in]		客户号
	sLogin		[in]		柜员账号
	sLoginPwd	[in]		柜员密码
	dQtfy		[in]		其他负债变动金额

	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_ModifyQTFY(const char* lpKeyName, const char* sKhh, const char* sLogin, const char* sLoginPwd, double dQtfy);

/*
	两融查询客户可融券信息(中信证券)

	lpKeyName	[in]		使用的配置Key值
	khh			[in]		客户号
	sLogin		[in]		柜员账号
	sLoginPwd	[in]		柜员密码
	jys			[in]		交易所
	zqdm		[in]		证券代码
	tcxz		[in]		头寸来源(1 普通头寸 2 专项头寸)
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRQXX_ZX(const char* lpKeyName, const char* khh, const char* sLogin, const char* sLoginPwd, const char* jys, const char* zqdm, int32 tcxz, int nRowcount, int64 nBrowindex, vector<ITPDK_RQZQ>& arInfo);

/*
	两融查询客户可融资金信息(中信证券)

	lpKeyName	[in]		使用的配置Key值
	khh			[in]		客户号
	sLogin		[in]		柜员账号
	sLoginPwd	[in]		柜员密码
	tcxz		[in]		头寸来源(1 普通头寸 2 专项头寸)
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_KRZJXX_ZX(const char* lpKeyName, const char* khh, const char* sLogin, const char* sLoginPwd, int32 tcxz, vector<ITPDK_KRZJXX>& arInfo);

/*
	专项头寸证券调拨（中信证券）

	lpKeyName	[in]		使用的配置Key值
	sKhh		[in]		客户号
	sLogin		[in]		柜员账号
	sLoginPwd	[in]		柜员密码
	sJys		[in]		交易所
	sZqdm		[in]		证券代码
	nFssl		[in]		调拨数量
	nYysl		[in]		预约数量到期增量
	nFlag		[in]		调拨方向,0为MDB调出到柜台，1位柜台调入MDB
	arInfo		[out]		结果数据

	return				成功返回>0，失败返回<=0
*/
ITPDK_API int64 MARITPDK_SpecialPosition_ZQDB_ZX(const char* lpKeyName, const char* sKhh, const char* sLogin, const char* sLoginPwd, const char* sJys, const char* sZqdm, int64 nFssl, int64 nYysl, int64 nFlag, vector<ITPDK_ZXTCZQDB>& arInfo);

/*
	两融查询融券负债汇总
	sKhh		[in]		客户号
	sJys		[in]		交易所（与证券代码同时送生效,否则查询全部）
	sZqdm		[in]		证券代码（与交易所同时送生效,否则查询全部）
	nRowcount	[in]		返回条数
	nBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_MR_RQFZHZ(const char* sKhh, const char* sJys, const char* sZqdm, int64 nRowCount, int64 nBrowIndex, vector<ITPDK_XYRQFZHZ>& arInfo);

/*
	A5两融查询资格证券


	sJys		[in]		交易所
	sZqdm		[in]		证券代码
	nRzzt		[in]		融资状态
	nRqzt		[in]		融券状态
	nRowcount	[in]		返回条数
	sBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_A5MR_ZGZQ(const char* sJys, const char* sZqdm, int64 nRzzt, int64 nRqzt, int64 nRowcount, const char *sBrowindex, vector<ITPDK_A5ZGZQ>& arInfo);

/*
	A5两融查询担保证券


	sJys		[in]		交易所
	sZqdm		[in]		证券代码
	nJyzt		[in]		交易状态
	nRowcount	[in]		返回条数
	sBrowindex	[in]		分页索引号
	arInfo		[out]		结果数据

	return				返回查询到的记录数
*/
ITPDK_API int64 MARITPDK_QueryTradeData_A5MR_DBZQ(const char* sJys, const char* sZqdm, int64 nJyzt, int64 nRowcount, const char* sBrowindex, vector<ITPDK_A5DBZQ>& arInfo);

/*
	大宗交易委托

	lpKhh		[in]		客户号
	lpJys		[in]		交易所
	lpZqdm		[in]		证券代码
	nJylb		[in]		交易类别
	lWtsl		[in]		委托数量
	lfWtjg		[in]		委托价格
	lDdlx		[in]		订单类型
	lpGdh		[in]		股东号

	lpDfgdh		[in]		对方股东号
	lpDfxw		[in]		对方席位
	lpLxr		[in]		联系人
	lpLxdh		[in]		联系电话
	lpPdcj		[in]		是否配对成交：1为配对成交
	lpYdh		[in]		约定号
	lTdbz		[in]		特定股份大宗交易标志
	return				成功返回委托号，失败返回<0
*/
ITPDK_API int64 MARITPDK_BlockTrading(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, long lWtsl, double lfWtjg, long lDdlx, const char* lpGdh, const char* lpDfgdh, const char* lpDfxw, const char* lpLxr, const char* lpLxdh, const char* lpPdcj, const char* lpYdh, long lTdbz);

/*
	大宗交易可买卖数量计算

	lpKhh		[in]		客户号
	lpJys		[in]		交易所
	lpZqdm		[in]		证券代码
	nJylb		[in]		交易类别
	lfWtjg		[in]		委托价格
	lDdlx		[in]		订单类型
	lpGdh		[in]		股东号

	lTdbz		[in]		特定股份大宗交易标志
	return				成功返回可委托数量，失败返回<0
*/
ITPDK_API int64 MARITPDK_BlockTrading_TradableQty(const char* lpKhh, const char* lpJys, const char* lpZqdm, int nJylb, double lfWtjg, long lDdlx, const char* lpGdh, long lTdbz);
