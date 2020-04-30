#ifndef ITAP_API_ERROR_H
#define ITAP_API_ERROR_H

//=============================================================================
/**
 *	\addtogroup G_ERR_INNER_API		ITapAPI内部返回的错误码定义。
 *	@{
 */
//=============================================================================
//! 成功
namespace ITapTrade
{
	//=============================================================================
	/**
	*	\addtogroup G_ERR_LOGIN		登录过程错误
	*	@{
	*/
	//=============================================================================
    //! 登录过程执行错误
	const int			TAPIERROR_LOGIN										= 10001;
    //! 登录用户不存在
	const int			TAPIERROR_LOGIN_USER								= 10002;
    //! 需要进行动态认证
	const int			TAPIERROR_LOGIN_DDA									= 10003;
    //! 登录用户未授权
	const int			TAPIERROR_LOGIN_LICENSE 							= 10004;
    //! 登录模块不正确
	const int			TAPIERROR_LOGIN_MODULE								= 10005;
    //! 需要强制修改密码
	const int			TAPIERROR_LOGIN_FORCE 								= 10006;
    //! 登录状态禁止登录
	const int			TAPIERROR_LOGIN_STATE 								= 10007;
    //! 登录密码不正确
	const int			TAPIERROR_LOGIN_PASS 								= 10008;
    //! 没有该模块登录权限
	const int			TAPIERROR_LOGIN_RIGHT 								= 10009;
    //! 登录数量超限
	const int			TAPIERROR_LOGIN_COUNT 								= 10010;
    //! 登录用户不在服务器标识下可登录用户列表中
	const int			TAPIERROR_LOGIN_NOTIN_SERVERFLAGUSRES 				= 10011;
	//! 登录用户已被冻结
	const int			TAPIERROR_LOGIN_FREEZE 								= 10012;
	//! 密码错误，用户冻结
	const int			TAPIERROR_LOGIN_TOFREEZE 							= 10013;
	//! 客户状态不允许登录
	const int			TAPIERROR_LOGIN_ACCOUNTSTATE 						= 10014;
	//! 需要进行二次认证
	const int			TAPIERROR_LOGIN_SECCERTIFI 							= 10015;

	//未绑定二次认证信息
	const int			TAPIERROR_LOGIN_NOSECONDSET 						= 10016;
	//不受信任的计算机登录
	const int			TAPIERROR_LOGIN_NOTURSTHOST 						= 10017;
	//非本交易中心客户
	const int			TAPIERROR_LOGIN_NOTINTRADECENTER 					= 10019;
	//版本和后台版本不不一致
	const int			TAPIERROR_LOGIN_INCONSISTENT 						= 10020;
	//客户所属交易中心前置地址未配置
	const int			TAPIERROR_LOGIN_NOCENTERFRONTADDRESS 				= 10021;
	//不允许此账号类型登录
	const int			TAPIERROR_LOGIN_PROHIBITACCOUNTTYPE 				= 10022;

	//!需要信息采集-直连
	const int			TAPIERROR_LOGIN_GATHERINFO_DIRECT 					= 10023;
	//!需要信息采集-中继
	const int			TAPIERROR_LOGIN_GATHERINFO_RELAY 					= 10024;

	//!	二次验证失败
	const int          TAPIERROR_SECONDCERTIFICATION_FAIL 					= 14001;
	//!	二次验证超时
	const int          TAPIERROR_SECONDCERTIFICATION_TIMEOVER 				= 14002;
	//!二次认证错误次数超限，重新登录
	const int		 TAPIERROR_SECONDCERTIFICATION_RELOGIN 					= 14003;
	//!二次认证错误次数超限，用户冻结
	const int		 TAPIERROR_SECONDCERTIFICATION_FREEZE 					= 14004;
	//=============================================================================

    //! 数据库连接失败
	const int			TAPIERROR_CONN_DATABASE 							= 11000;
    //! 数据库操作失败
	const int			TAPIERROR_OPER_DATABASE 							= 11001;
    //! 不允许一对多
	const int			TAPIERROR_NEED_ONETOONE 							= 11002;
    //! 删除失败-存在关联信息，
	const int			TAPIERROR_EXIST_RELATEINFO 							= 11003;
	//! 删除分组失败-分组有下属或在操作员下属中
	const int			TAPIERROR_EXIST_RELATEINFOOFGROUP 					= 11004;

	//审核状态不予许修改
	const int		 TAPIERROR_CHECK_FAILED 								= 11006;
	//不允许存在重复3.0外部品种编号
	const int		 TAPIERROR_EXIST_OUTSIDECOMMODITYNO 					= 11007;
	//客户结算单不存在
	const int		 TAPIERROR_NOTEXIST_BILL 								= 11008;
	//不允许增加此类型账号
	const int		 TAPIERROR_LOGIN_PROHIBITADDACCOUNTTYPE 				= 11009;
	//账号类型不允许为空
	const int		 TAPIERROR_ACCOUNTINFO_NOTEXPTY 						= 11010;
	//主账号不允许为空
	const int		 TAPIERROR_ACCOUNTINFO_SuperiorNOTEMPTY 				= 11011;

    //! 登录用户密码修改失败-原始密码错误
	const int			TAPIERROR_USERPASSWORD_MOD_SOURCE 					= 12001;
    //! 登录用户密码修改失败-不能与前n次密码相同
	const int			TAPIERROR_USERPASSWORD_MOD_SAME 					= 12002;
    //! 登录用户密码修改失败-新密码不符合密码复杂度要求
	const int          TAPIERROR_USERPASSWORD_MOD_COMPLEXITY 				= 12003;

    //! 一个币种组只能设置一个基币
	const int			TAPIERROR_CURRENCY_ONLY_ONEBASE 					= 13001;
    //! 基币只能是美元或港币
	const int			TAPIERROR_CURRENCY_ONLY_USDHKD 						= 13002;

	//! 用户采集信息密钥未找到
	const int			TAPIERROR_GATHERINFO_NO_AUTHKEY						= 17001;
	//! APPID认证失败
	const int			TAPIERROR_GATHERINFO_AUTH_FAILED					= 17002;
	//=============================================================================
	/**
	*	\addtogroup G_ERR_TRADE_SERVICE		交易服务错误代码定义
	*	@{
	*/
	//=============================================================================

    //! 资金账号不存在
	const int			TAPIERROR_ORDERINSERT_ACCOUNT 						= 60001;
    //! 资金账号状态不正确
	const int			TAPIERROR_ORDERINSERT_ACCOUNT_STATE 				= 60002;
	//! 资金账号交易中心不一致
	const int			TAPIERROR_ORDERINSERT_TRADECENT_ERROR 				= 60003;
	//主账号不允许下单
	const int		 TAPIERROR_ORDERINT_MAINACCOUNT_ERROR 					= 60004;
	//主账号信息错误
	const int		 TAPIERROR_ORDERINT_MAINACCINFO_ERROR 					= 60005;
	//账号禁止期权市价下单
	const int		 TAPIERROR_ORDERINT_NO_OPTMARKET_ERROR 					= 60006;
	//目前不支持的指令
	const int		 TAPIERROR_ORDERINT_UN_SUPPORT_ERROR 					= 60007;
    //! 下单无效的合约
	const int			TAPIERROR_ORDERINSERT_CONTRACT 						= 60011;
    //! LME未准备就绪
	const int			TAPIERROR_ORDERINSERT_LME_NOTREADY 					= 60012;
	//!不支持的下单类型
	const int			TAPIERROR_ORDERINSERT_ERROR_ORDER_TYPE 				= 60013;
	//!错误的埋单类型
	const int			TAPIERROR_ORDERINSERT_READY_TYPE_ERROR 				= 60014;
	//!不合法的委托类型
	const int			TAPIERROR_ORDERINSERT_ORDER_TYPE_ERROR 				= 60015;
	//下单合约标的合约不存在
	const int		 TAPIERROR_ORDERINSERT_SUBCONTRACT 						= 60016;
	//现货下单买入数量错误
	const int		 TAPIERROR_ORDERINSERT_SPOTBUYQTY 						= 60017;
	//非认购期权不允许备兑
	const int		 TAPIERROR_ORDERINSERT_RESERVE_PUT 						= 60018;
	//买入开仓不允许备兑
	const int		 TAPIERROR_ORDERINSERT_RESERVE_B_OPEN 					= 60019;
	//卖出平仓不允许备兑
	const int		 TAPIERROR_ORDERINSERT_RESERVE_S_COVER 					= 60020;
    //! 客户权限禁止交易
	const int			TAPIERROR_ORDER_NOTRADE_ACCOUNT 					= 60021;
    //! 客户品种分组禁止交易
	const int			TAPIERROR_ORDER_NOTRADE_COM_GROUP 					= 60022;
    //! 客户合约特设禁止交易
	const int			TAPIERROR_ORDER_NOTRADE_ACC_CONTRACT 				= 60023;
    //! 系统权限禁止交易
	const int			TAPIERROR_ORDER_NOTRADE_SYSTEM 						= 60024;
    //! 客户权限只可平仓
	const int			TAPIERROR_ORDER_CLOSE_ACCOUNT 						= 60025;
    //! 客户合约特设只可平仓
	const int			TAPIERROR_ORDER_CLOSE_ACC_CONTRACT 					= 60026;
    //! 系统权限只可平仓
	const int			TAPIERROR_ORDER_CLOSE_SYSTEM 						= 60027;
	//! 只可平仓提前天数限制只可平仓
	const int			TAPIERROR_ORDER_CLOSE_DAYS 							= 60028;
	//! 客户品种风控权限禁止交易
	const int			TAPIERROR_ORDER_NOTRADE_RISK 						= 60029;
	//! 客户品种风控权限只可平仓
	const int			TAPIERROR_ORDER_CLOSE_RISK 							= 60030;

    //! 持仓量超过最大限制
	const int			TAPIERROR_ORDERINSERT_POSITIONMAX 					= 60031;
    //! 下单超过单笔最大量
	const int			TAPIERROR_ORDERINSERT_ONCEMAX 						= 60032;
    //! 下单合约无交易路由
	const int			TAPIERROR_ORDERINSERT_TRADEROUTE 					= 60033;
	//! 委托价格超出偏离范围
	const int			TAPIERROR_ORDER_IN_MOD_PRICE_ERROR 					= 60034;
	//! 超过GiveUp最大持仓量
	const int			TAPIERROR_ORDER_IN_GIVEUP_POS_MAX 					= 60035;
	//持仓量超过ETF总持仓限制
	const int			TAIERROR_ORDER_ETF_POSITIONMAX 						= 60036;
	//持仓量超过ETF权利仓限制
	const int			TAIERROR_ORDER_ETF_BUYPOSITIONMAX 					= 60037;
	//持仓量超过ETF单日买入开仓限制
	const int			TAIERROR_ORDER_ETF_BUYONEDAYPOSITIONMAX 			= 60038;
	//可用锁定现货不足
	const int			TAIERROR_ORDERINSERT_NOTENOUGHSPOT 					= 60039;
	//现货不支持备兑
	const int			TAIERROR_ORDERINSERT_RESERVE_SPOT 					= 60040;


    //! 未登录网关
	const int          TAPIERROR_UPPERCHANNEL_NOT_LOGIN 					= 60041;
	//! 未找到网关信息
	const int          TAPIERROR_UPPERCHANNEL_NOT_FOUND 					= 60042;
	//此品种不支持解锁或锁定
	const int          TAPIERROR_COMMODITY_LOCK 							= 60043;
	//现货未设置对应期权品种
	const int          TAPIERROR_SPOT_ROOT_COMMODITY 						= 60044;
	//现货对应期权无交易路由
	const int			TAIERROR_SPOT_ROOTCOM_TRADEROUTE 					= 60045;

    //! 下单资金不足
	const int			TAPIERROR_ORDERINSERT_NOTENOUGHFUND 				= 60051;
    //! 手续费参数错误
	const int			TAPIERROR_ORDERINSERT_FEE 							= 60052;
    //! 保证金参数错误
	const int			TAPIERROR_ORDERINSERT_MARGIN 						= 60053;
    //! 总基币资金不足
	const int			TAPIERROR_ORDERINSERT_BASENOFUND 					= 60054;

	//! 超过保证金额度
	const int			TAPIERROR_ORDERINSERT_MARGINAMOUNT 					= 60055;
	//! 总基币超过开仓比例限制
	const int			TAPIERROR_ORDERINSERT_OPENRATIO 					= 60056;
	//! 独立币种组超过开仓比例限制
	const int			TAPIERROR_ORDERINSERT_GROUP_OPENRATIO 				= 60057;
	//! 风险阵列参数错误
	const int			TAPIERROR_ORDERINSERT_RISKARRAY 					= 60058;
	//总基币超过限购额度限制
	const int			TAIERROR_ORDERINSERT_BUYLIMITE 						= 60059;
	//独立币种组超过限购额度限制
	const int			TAIERROR_ORDERINSERT_GROUP_BUYLIMITE 				= 60060;
    //! 撤单无此系统号
	const int          TAPIERROR_ORDERDELETE_NOT_SYSNO 						= 60061;
    //! 此状态不允许撤单
	const int          TAPIERROR_ORDERDELETE_NOT_STATE 						= 60062;
	//! 录单不允许撤单
	const int          TAPIERROR_ORDERDELETE_NO_INPUT 						= 60063;
	//不允许撤销锁定/解锁指令
	const int          TAPIERROR_ORDERDELETE_NO_TRADE 						= 60064;

    //! 此状态不允许改单
	const int			TAPIERROR_ORDERMODIFY_NOT_STATE 					= 60071;
    //! 人工单不允许改单
	const int			TAPIERROR_ORDERMODIFY_BACK_INPUT 					= 60072;
	//! 风险报单不允许改单
	const int			TAPIERROR_ORDERMODIFY_RISK_ORDER 					= 60073;
	//! 成交量大于改单量
	const int			TAPIERROR_ORDERMODIFY_ERROR_QTY 					= 60074;
	//! 预埋单不允许改单
	const int			TAPIERROR_ORDERMODIFY_ERROR_READY 					= 60075;

    //! 已删除报单不能转移
	const int			TAPIERROR_ORDERINPUT_CANNOTMOVE 					= 60081;

    //! 录单重复
	const int			TAPIERROR_ORDERINPUT_REPEAT 						= 60091;

	//! 合约行情价格修改失败
	const int			TAPIERROR_CONTRACT_QUOTE 							= 60101;

	//! 下单超过上手单笔最大量
	const int			TAPIERROR_UPPER_ONCEMAX 							= 60111;
	//! 下单超过上手最大持仓量
	const int			TAPIERROR_UPPER_POSITIONMAX 						= 60112;

	//! 开平方式错误
	const int			TAPIERROR_ORDERINSERT_CLOSEMODE 					= 60121;
	//! 委托平仓持仓不足
	const int			TAPIERROR_CLOSE_ORDER 								= 60122;
	//! 成交平仓失败
	const int			TAPIERROR_CLOSE_MATCH 								= 60123;

	//! 未找到本地委托
	const int			TAPIERROR_MOD_DEL_NO_ORDER 							= 60131;
	//! 与网关断开连接
	const int			TAPIERROR_MOD_DEL_GATEWAY_DISCON 					= 60132;

	//! 录单成交重复
	const int			TAPIERROR_MATCHINPUT_REPEAT 						= 60141;
	//! 录单成交未找到对应委托
	const int			TAPIERROR_MATCHINPUT_NO_ORDER 						= 60142;
	//! 录单成交合约不存在
	const int			TAPIERROR_MATCHINPUT_NO_CONTRACT 					= 60143;
	//! 录单成交参数错误
	const int			TAPIERROR_MATCHINPUT_PARM_ERROR 					= 60144;
	//! 录单成交委托状态错误
	const int			TAPIERROR_MATCHINPUT_OSTATE_ERROR 					= 60145;

	//! 成交删除未找到成交
	const int			TAPIERROR_MATCHREMOVE_NO_MATCH 						= 60151;
	//! 此状态成交不可删
	const int			TAPIERROR_MATCHREMOVE_STATE_ERROR 					= 60152;

	//! 不允许录入此状态订单
	const int			TAPIERROR_ORDERINPUT_STATE_ERROR 					= 60161;
	//! 错误的修改订单请求
	const int			TAPIERROR_ORDERINPUT_MOD_ERROR 						= 60162;
	//! 订单不可删，存在对应成交
	const int			TAPIERROR_ORDERREMOVE_ERROR 						= 60163;
	//! 不合法的委托状态
	const int			TAPIERROR_ORDERINPUT_MOD_STATE_ERROR 				= 60164;
	//! 此状态不允许订单转移
	const int			TAPIERROR_ORDEREXCHANGE_STATE_ERROR 				= 60165;
	//! 订单不允许删除
	const int			TAPIERROR_ORDERREMOVE_NOT_ERROR 					= 60166;

	//! 做市商双边撤单未找到委托
	const int			TAPIERROR_ORDERMARKET_DELETE_NOTFOUND 				= 60171;
	//! 做市商双边撤单客户不一致
	const int			TAPIERROR_ORDERMARKET_DEL_ACCOUNT_NE 				= 60172;
	//! 做市商双边撤单品种不一致
	const int			TAPIERROR_ORDERMARKET_DEL_COMMODITY_NE 				= 60173;
	//! 做市商双边撤单合约不一致
	const int			TAPIERROR_ORDERMARKET_DEL_CONTRACT_NE 				= 60174;
	//! 做市商双边撤单买卖方向相同
	const int			TAPIERROR_ORDERMARKET_DEL_SIDE_EQ 					= 60175;
	//! 做市商双边撤单买卖方向错误
	const int			TAPIERROR_ORDERMARKET_DEL_SIDE_ERROR 				= 60176;
	//! 做市商单边检查未通过
	const int			TAPIERROR_ORDERMARKET_OTHER_SIDE_ERROR 				= 60177;

	//! 埋单激活失败，订单未找到
	const int			TAPIERROR_ORDERACTIVATE_NOTFOUND_ERROR 				= 60181;
	//! 埋单激活失败，非有效状态
	const int			TAPIERROR_ORDERACTIVATE_STATE_ERROR 				= 60182;
	
	//操作员无跨中心下单权限
	const int			TAPIERROR_TRANSIT_ORDERINSERT_RIGHT 				= 60191;
	//未连接中转服务
	const int			TAPIERROR_TRANSIT_ORDERINSERT_DISCON 				= 60192;
	//下单未连接目标交易中心
	const int			TAPIERROR_TRANSIT_ORDERINSERT_DISCON_DEST 			= 60193;
	//撤单未连接目标交易中心
	const int			TAPIERROR_TRANSIT_ORDERDELETE_DISCON_DEST 			= 60194;
	//改单未连接目标交易中心
	const int			TAPIERROR_TRANSIT_ORDERMODIFY_DISCON_DEST 			= 60195;
	//错误的中转订单操作
	const int			TAPIERROR_TRANSIT_ORDER_OPERATOR 					= 60196;

	//客户权限禁止买入
	const int			TAPIERROR_ORDER_DISALLOWBUY_ACCOUNT 				= 60201;
	//客户权限禁止卖出
	const int			TAPIERROR_ORDER_DISALLOWSELL_ACCOUNT 				= 60202;
	//系统权限禁止买入
	const int			TAPIERROR_ORDER_DISALLOWBUY_SYSTEM 					= 60203;
	//系统权限禁止卖出
	const int			TAPIERROR_ORDER_DISALLOWSELL_SYSTEM 				= 60204;
	//客户权限禁止卖开仓期权 -仅外盘系统
	const int			TAPIERROR_ORDER_DIS_SELLOPTION_ACCOUNT				= 60205;
	//系统权限禁止卖开仓期权 -仅外盘系统
	const int			TAPIERROR_ORDER_DIS_SELLOPTION_SYSTEM				= 60206;

	//超出贷款限额
	const int			TAPIERROR_ORDERINSERT_LOANAMOUNT					= 60211;
	//此品种不支持组合策略
	const int			TAPIERROR_COMBINE_COMMODITY							= 60220;
	//组合申报合约期权类型不符合要求
	const int			TAPIERROR_COMBINE_CALLORPUT							= 60221;
	//拆分的组合持仓不存在
	const int			TAPIERROR_COMBINE_COMPOSITION						= 60222;
	//拆分的组合持仓数量不足
	const int			TAPIERROR_COMBINE_COMPOSITION_QTY					= 60223;
	//组合申报合约备兑标识不符合要求
	const int			TAPIERROR_COMBINE_HEDGEFLAG							= 60224;
	//组合申报合约买卖方向不符合要求
	const int			TAPIERROR_COMBINE_ORDERSIDE							= 60225;
	//组合申报合约大小不符合要求
	const int			TAPIERROR_COMBINE_CONTRACTSIZE						= 60226;
	//组合申报合约到期日不符合要求
	const int			TAPIERROR_COMBINE_CONTRACTDAYS						= 60227;
	//组合申报合约行权价不符合要求
	const int			TAPIERROR_COMBINE_STRIKEPRICE						= 60228;
	//相同合约不允许组合
	const int			TAPIERROR_COMBINE_CONTRACT_SAME						= 60229;

	//=============================================================================
	/**
	*	\addtogroup G_ERR_GATE_WAY		网关错误代码定义
	*	@{
	*/
	//=============================================================================

    //! 网关未就绪，未连接上手
	const int			TAPIERROR_GW_NOT_READY 								= 80001;
    //! 品种错误
	const int			TAPIERROR_GW_INVALID_COMMODITY 						= 80002;
    //! 合约错误
	const int			TAPIERROR_GW_INVALID_CONTRACT 						= 80003;
    //! 报单字段有误
	const int			TAPIERROR_GW_INVALID_FIELD 							= 80004;
    //! 价格不合法
	const int		    TAPIERROR_GW_INVALID_PRICE 							= 80005;
    //! 数量不合法
	const int			TAPIERROR_GW_INVALID_VOLUME 						= 80006;
    //! 报单类型不合法
	const int			TAPIERROR_GW_INVALID_TYPE 							= 80007;
    //! 委托模式不合法
	const int			TAPIERROR_GW_INVALID_MODE 							= 80008;
    //! 委托不存在（改单、撤单）
	const int			TAPIERROR_GW_ORDER_NOT_EXIST 						= 80009;
    //! 发送报单失败
	const int			TAPIERROR_GW_SEND_FAIL 								= 80010;
    //! 被上手拒绝
	const int			TAPIERROR_GW_REJ_BYUPPER 							= 80011;

	//=============================================================================
	/**
	*	\addtogroup G_ERR_FRONT_SERVICE		前置返回错误
	*	@{
	*/
	//=============================================================================

    //! 前置不允许该模块登录
	const int			TAPIERROR_TRADEFRONT_MODULETYPEERR 					= 90001;
    //! 一次请求太多数据
	const int			TAPIERROR_TRADEFRONT_TOOMANYDATA 					= 90002;
    //! 前置没有所要数据
	const int			TAPIERROR_TRADEFRONT_NODATA 						= 90003;
	//! 所查询的操作员信息不存在
	const int			TAPIERROT_TRADEFRONT_NOUSER 						= 90004;

    //! 前置与交易断开
	const int			TAPIERROR_TRADEFRONT_DISCONNECT_TRADE 				= 90011;
    //! 前置与管理断开
	const int			TAPIERROR_TRADEFRONT_DISCONNECT_MANAGE 				= 90012;

    //! 下属资金账号不存在
	const int			TAPIERROR_TRADEFRONT_ACCOUNT 						= 90021;
	//! 该操作员不允许交易
	const int			TAPIERROR_TRADEFRONT_ORDER 							= 90022;
	//! 查询频率过快
	const int			TAPIERROR_TRADEFRONT_FREQUENCY 						= 90023;
	//! 该授权不予许登录
	const int			TAPIERROR_TRADEFRONT_RUFUSE 						= 90024;
	//! 自成交验证不通过
	const int			TAPIERROR_TRADEFRONT_SELFMATCH 						= 90025;
    
    const int TAPIERROR_SUCCEED                                            = 0;
    //! 连接服务失败
    const int TAPIERROR_ConnectFail                                        = -1;
    //! 链路认证失败
    const int TAPIERROR_LinkAuthFail                                       = -2;
    //! 主机地址不可用
    const int TAPIERROR_HostUnavailable                                    = -3;
    //! 发送数据错误
    const int TAPIERROR_SendDataError                                      = -4;
    //! 测试编号不合法
    const int TAPIERROR_TestIDError                                        = -5;
    //! 没准备好测试网络
    const int TAPIERROR_NotReadyTestNetwork                                = -6;
    //! 当前网络测试还没结束
    const int TAPIERROR_CurTestNotOver                                     = -7;
    //! 没用可用的接入前置
    const int TAPIERROR_NOFrontAvailable                                   = -8;
    //! 数据路径不可用
    const int TAPIERROR_DataPathAvaiable                                   = -9;
    //! 重复登录
    const int TAPIERROR_RepeatLogin                                        = -10;
    //! 内部错误	
    const int TAPIERROR_InnerError                                         = -11;
    //! 上一次请求还没有结束	
    const int TAPIERROR_LastReqNotFinish                                   = -12;
    //! 输入参数非法	
    const int TAPIERROR_InputValueError                                    = -13;
    //! 授权码不合法	
    const int TAPIERROR_AuthCode_Invalid                                   = -14;
    //! 授权码超期	
    const int TAPIERROR_AuthCode_Expired                                   = -15;
    //! 授权码类型不匹配	
    const int TAPIERROR_AuthCode_TypeNotMatch                              = -16;
    //! API还没有准备好
    const int TAPIERROR_API_NotReady                                       = -17;
    //! UDP端口监听失败
    const int TAPIERROR_UDP_LISTEN_FAILED                                  = -18;
    //! UDP正在监听
    const int TAPIERROR_UDP_LISTENING                                      = -19;
    //! 接口未实现
    const int TAPIERROR_NotImplemented                                     = -20;
    //! 每次登录只允许调用一次
	const int TAPIERROR_CallOneTimeOnly										= -21;
	//! 超过下单频率。
	const int TAPIERROR_ORDER_FREQUENCY										= -22;
	//! 查询频率太快。
	const int TAPIERROR_RENTQRY_TOOFAST										= -23;
	//! 不符合调用条件。
	const int TAPIERROR_CALL_NOCONDITION									= -24;
	//! 改单撤单时没有找到对应订单。
	const int TAPIERROR_ORDER_NOTFOUND										= -25;

	//! 日志路径为空。
	const int TAPIERROR_LOGPATH_EMPTY										= -26;
	//! 打开日志文件失败
	const int TAPIERROR_LOGPATH_FAILOPEN									= -27;
	//! 没有交易员登录权限
	const int TAPIERROR_RIGHT_TRADER 										= -28;
	//! 没有订单录入或者成交录入权限
	const int TAPIERROR_RIGHT_ORDERINPUT									= -29;
	//! 没有订单修改和订单删除权限，成交删除权限
	const int TAPIERROR_RIGHT_LOCALOPERATION 								= -30;
	//! 没有订单转移权限
	const int TAPIERROR_RIGHT_ORDERTRANSFER 								= -31;
	//! 成交录入时系统号为空
	const int TAPIERROR_FILLINPUT_SYSTEMNO 									= -32;
	//! 成交删除时成交号为空。
	const int TAPIERROR_FILLREMOVE_MATCHNO 									= -33;

	//! 成交删除时没有找到对应的成交
	const int TAPIERROR_FILLREQMOVE_NOFUND 									= -34;
	//! 订单修改时客户账号变动。
	const int TAPIERROR_LOCALMODIFY_ACCOUNT 								= -35;
	//! 订单转移时客户账号没有变动
	const int TAPIERROR_LOCALTRANSFER_ACCOUNT 								= -36;
	//! 修改的电话密码位数不对或者包含特殊字符。
	const int TAPIERROR_INPUTERROR_PHONE 									= -37;

	//!	未绑定的二次认证信息
	const int TAPIERROR_ERROR_CONTACT										= -38;
	//! 二次认证有效期内不能再申请二次认证码
	const int TAPIERROR_ERROR_REJESTVERTIFICATE 							= -39;
	//! 发送二次认证码的验证请求之前，需要先发送请求二次认证码
	const int TAPIERROR_ERROR_NOTREQUESTSECONDCODE 							= -44;

	//! 没有设置客户密码的权限。
	const int TAPIERROR_RIGHT_SETPASSWORD 									= -40;
	//! 风险保单单客户无法撤销或更改
	const int TAPIERROR_RISK_OPERERROR										= -41;
	//! 改单是客户账号填写与订单客户账号不一致
	const int TAPIERROR_ORDER_MODACCOUNT									= -42;
	//! 内存申请失败
	const int TAPIERROR_MEMORY_ALLOCFAILED									= -43;

	//! 用户授权信息没有该品种下单权限
	const int TAPIERROR_ERROR_LICENSECOMMODITY								= -45;
	//! 非中继模式禁止调用该接口
	const int TAPIERROR_GATHERINFO_NORELAY									= -46;
	//! 用户采集信息不全（不影响登录）
	const int TAPIERROR_GATHERINFO_PARTY									= -47;
	//! 北斗星系统不可调用
	const int TAPIERROR_ERROR_SYSTEMTYPE									= -48;
	//! 看穿式信息采集库加载失败
	const int TAPIERROR_GATHERINFO_DATALOAD									= -49;

    /** @}*/


    //=============================================================================
    /**
     *	\addtogroup G_ERR_INPUT_CHECK		输入参数检查错误
     *	@{
     */
    //=============================================================================
    //! 输入数据为NULL
    const int TAPIERROR_INPUTERROR_NULL                                    = -10000;
    //! 输入错误的:TAPIYNFLAG
    const int TAPIERROR_INPUTERROR_TAPIYNFLAG                              = -10001;
    //! 输入错误的:TAPILOGLEVEL
    const int TAPIERROR_INPUTERROR_TAPILOGLEVEL                            = -10002;
    //! 输入错误的:TAPICommodityType
    const int TAPIERROR_INPUTERROR_TAPICommodityType                       = -10003;
    //! 输入错误的:TAPICallOrPutFlagType
    const int TAPIERROR_INPUTERROR_TAPICallOrPutFlagType                   = -10004;
    //! 输入错误的:TAPIBucketDateFlag
    const int TAPIERROR_INPUTERROR_TAPIBucketDateFlag                      = -11001;
    //! 输入错误的:TAPIHisQuoteType
    const int TAPIERROR_INPUTERROR_TAPIHisQuoteType                        = -11002;
    //! 输入错误的:TAPIAccountType
    const int TAPIERROR_INPUTERROR_TAPIAccountType                         = -12001;
    //! 输入错误的:TAPIUserTypeType
    const int TAPIERROR_INPUTERROR_TAPIUserTypeType                        = -12002;
    //! 输入错误的:TAPIAccountState
    const int TAPIERROR_INPUTERROR_TAPIAccountState                        = -12003;
    //! 输入错误的:TAPIAccountFamilyType
    const int TAPIERROR_INPUTERROR_TAPIAccountFamilyType                   = -12004;
    //! 输入错误的:TAPIOrderTypeType
    const int TAPIERROR_INPUTERROR_TAPIOrderTypeType                       = -12005;
    //! 输入错误的:TAPIOrderSourceType
    const int TAPIERROR_INPUTERROR_TAPIOrderSourceType                     = -12006;
    //! 输入错误的:TAPITimeInForceType
    const int TAPIERROR_INPUTERROR_TAPITimeInForceType                     = -12007;
    //! 输入错误的:TAPISideType
    const int TAPIERROR_INPUTERROR_TAPISideType                            = -12008;
    //! 输入错误的:TAPIPositionEffectType
    const int TAPIERROR_INPUTERROR_TAPIPositionEffectType                  = -12009;
    //! 输入错误的:TAPIHedgeFlagType
    const int TAPIERROR_INPUTERROR_TAPIHedgeFlagType                       = -12010;
    //! 输入错误的:TAPIOrderStateType
    const int TAPIERROR_INPUTERROR_TAPIOrderStateType                      = -12011;
    //! 输入错误的:TAPICalculateModeType
    const int TAPIERROR_INPUTERROR_TAPICalculateModeType                   = -12012;
    //! 输入错误的:TAPIMatchSourceType
    const int TAPIERROR_INPUTERROR_TAPIMatchSourceType                     = -12013;
    //! 输入错误的:TAPIOpenCloseModeType
    const int TAPIERROR_INPUTERROR_TAPIOpenCloseModeType                   = -12014;
    //! 输入错误的:TAPIFutureAlgType
    const int TAPIERROR_INPUTERROR_TAPIFutureAlgType                       = -12015;
    //! 输入错误的:TAPIOptionAlgType
    const int TAPIERROR_INPUTERROR_TAPIOptionAlgType                       = -12016;
    //! 输入错误的:TAPIBankAccountLWFlagType
    const int TAPIERROR_INPUTERROR_TAPIBankAccountLWFlagType               = -12017;
    //! 输入错误的:TAPIBankAccountStateType
    const int TAPIERROR_INPUTERROR_TAPIBankAccountStateType                = -12018;
    //! 输入错误的:TAPIBankAccountSwapStateType
    const int TAPIERROR_INPUTERROR_TAPIBankAccountSwapStateType            = -12019;
    //! 输入错误的:TAPIBankAccountTransferStateType
    const int TAPIERROR_INPUTERROR_TAPIBankAccountTransferStateType        = -12020;
    //! 输入错误的:TAPIMarginCalculateModeType
    const int TAPIERROR_INPUTERROR_TAPIMarginCalculateModeType             = -12021;
    //! 输入错误的:TAPIOptionMarginCalculateModeType
    const int TAPIERROR_INPUTERROR_TAPIOptionMarginCalculateModeType       = -12022;
    //! 输入错误的:TAPICmbDirectType
    const int TAPIERROR_INPUTERROR_TAPICmbDirectType                       = -12023;
    //! 输入错误的:TAPIDeliveryModeType
    const int TAPIERROR_INPUTERROR_TAPIDeliveryModeType                    = -12024;
    //! 输入错误的:TAPIContractTypeType
    const int TAPIERROR_INPUTERROR_TAPIContractTypeType                    = -12025;
    //! 输入错误的:TAPIPartyTypeType
    const int TAPIERROR_INPUTERROR_TAPIPartyTypeType                       = -12026;
    //! 输入错误的:TAPIPartyCertificateTypeType
    const int TAPIERROR_INPUTERROR_TAPIPartyCertificateTypeType            = -12027;
    //! 输入错误的:TAPIMsgReceiverType
    const int TAPIERROR_INPUTERROR_TAPIMsgReceiverType                     = -12028;
    //! 输入错误的:TAPIMsgTypeType
    const int TAPIERROR_INPUTERROR_TAPIMsgTypeType                         = -12029;
    //! 输入错误的:TAPIMsgLevelType
    const int TAPIERROR_INPUTERROR_TAPIMsgLevelType                        = -12030;
    //! 输入错误的:TAPITransferDirectType
    const int TAPIERROR_INPUTERROR_TAPITransferDirectType                  = -12031;
    //! 输入错误的:TAPITransferStateType
    const int TAPIERROR_INPUTERROR_TAPITransferStateType                   = -12032;
    //! 输入错误的:TAPITransferTypeType
    const int TAPIERROR_INPUTERROR_TAPITransferTypeType                    = -12033;
    //! 输入错误的:TAPITransferDeviceIDType
    const int TAPIERROR_INPUTERROR_TAPITransferDeviceIDType                = -12034;
    //! 输入错误的:TAPITacticsTypeType
    const int TAPIERROR_INPUTERROR_TAPITacticsTypeType                     = -12035;
    //! 输入错误的:TAPIORDERACT
    const int TAPIERROR_INPUTERROR_TAPIORDERACT                            = -12036;
    //! 输入错误的:TAPIBillTypeType
    const int TAPIERROR_INPUTERROR_TAPIBillTypeType                        = -12037;
    //! 输入错误的:TAPIBillFileTypeType
    const int TAPIERROR_INPUTERROR_TAPIBillFileTypeType                    = -12038;
    //! 输入错误的:TAPIOFFFlagType
    const int TAPIERROR_INPUTERROR_TAPIOFFFlagType                         = -12039;
    //! 输入错误的:TAPICashAdjustTypeType
    const int TAPIERROR_INPUTERROR_TAPICashAdjustTypeType                  = -12040;
    //! 输入错误的:TAPITriggerConditionType
    const int TAPIERROR_INPUTERROR_TAPITriggerConditionType                = -12041;
    //! 输入错误的:TAPITriggerPriceTypeType
    const int TAPIERROR_INPUTERROR_TAPITriggerPriceTypeType                = -12042;
    //! 输入错误的:TAPITradingStateType 
    const int TAPIERROR_INPUTERROR_TAPITradingStateType                    = -12043;
    //! 输入错误的:TAPIMarketLevelType 
    const int TAPIERROR_INPUTERROR_TAPIMarketLevelType                     = -12044;
    //! 输入错误的:TAPIOrderQryTypeType 
    const int TAPIERROR_INPUTERROR_TAPIOrderQryTypeType                    = -12045;
	//! 输入错误的: ClientID，ClientID包含特殊字符。
	const int TAPIERROR_INPUTERROR_TAPIClientID                            = -12046;
    //! 历史行情查询参数不合法
    const int TAPIERROR_INPUTERROR_QryHisQuoteParam                        = -13001;
	//! 价格和数量中包含NAN或者INF不合法的数值
	const int TAPIERROR_INPUTERROR_TAPIIncludeNAN							= -13002;
	//! 输入错误的到期日
	const  int TAPIERROR_INPUTERROR_TAPIExpireTime							= -12047;
	//! 错误的密码类型
	const int TAPIERROR_INPUTERROR_TAPIPasswordType							= -12048;
    //! 错误的结算数据类型
    const int TAPIERROR_INPUTERROR_TAPISettleFlagType                       = -12049;
	//! 输入错误的:TAPILoginTypeType
	const int TAPIERROR_INPUTERROR_TAPILoginTypeType						= -12050;
	//! 输入错误的:TapAPISpecialOrderTypeType			
	const int TAPIERROR_INPUTERROR_TapAPISpecialOrderTypeType				= -12051;
	//! 输入错误的:TapAPISpecialOrderTypeType
	const int TAPIERROR_INPUTERROR_TapAPICombineStrategyType				= -12052;

    /** @}*/

    //=============================================================================
    /**
     *	\addtogroup G_ERR_DISCONNECT_REASON	网络断开错误代码定义
     *	@{
     */
    //=============================================================================
    //! 主动断开
    const int TAPIERROR_DISCONNECT_CLOSE_INIT                              = 1;
    //! 被动断开
    const int TAPIERROR_DISCONNECT_CLOSE_PASS                              = 2;
    //! 读错误
    const int TAPIERROR_DISCONNECT_READ_ERROR                              = 3;
    //! 写错误
    const int TAPIERROR_DISCONNECT_WRITE_ERROR                             = 4;
    //! 缓冲区满
    const int TAPIERROR_DISCONNECT_BUF_FULL                                = 5;
    //! 异步操作错误
    const int TAPIERROR_DISCONNECT_IOCP_ERROR                              = 6;
    //! 解析数据错误
    const int TAPIERROR_DISCONNECT_PARSE_ERROR                             = 7;
    //! 连接超时
    const int TAPIERROR_DISCONNECT_CONNECT_TIMEOUT                         = 8;
    //! 初始化失败
    const int TAPIERROR_DISCONNECT_INIT_ERROR                              = 9;
    //! 已经连接
    const int TAPIERROR_DISCONNECT_HAS_CONNECTED                           = 10;
    //! 工作线程已结束
    const int TAPIERROR_DISCONNECT_HAS_EXIT                                = 11;
    //! 操作正在进行，请稍后重试
    const int TAPIERROR_DISCONNECT_TRY_LATER                               = 12;

    /** @}*/
}
#endif //! TAP_API_ERROR_H
