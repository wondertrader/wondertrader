/////////////////////////////////////////////////////////////////////////
///产品 HTS
///公司 福建顶点软件股份有限公司
///文件 itpdk_dict.h
///用途 定义了参数字典
///版本
///20210910  5.1.0.0	郑东辉		新增版本说明
/////////////////////////////////////////////////////////////////////////

#pragma once


#define PRODUCT             "itpdk"
#define VERSION             "5230"


//错误码    200 - 600
#define     ERR_CONN_INVALID            -201            //连接无效
#define     ERR_ALLOCSESS_FAILED        -202            //发起会话失败
#define     ERR_NOT_LOGIN               -203            //客户未登录
#define     ERR_REQ_FAILED              -204            //发送请求失败
#define     ERR_WRONG_ANS               -205            //接收到错误应答数据
#define     ERR_READ_CONFIG_FILE        -206            //读取配置文件失败

#define     ERR_UNKNOW                  -220            //未知错误
#define     ERR_PARAM_INVALID           -221            //非法参数
#define     ERR_NOT_SUPPORT             -222            //接口不支持的操作
#define     ERR_SYSTEM_TYPE_INVALID     -223            //无效系统类型
#define     ERR_NOT_FOUND_CONN_INFO     -224            //无法获取到连接信息
#define     ERR_NO_TRADE                -225            //禁止交易
#define     ERR_NO_SPECIFY_NODEID       -226            //未指定交易节点
#define     ERR_NO_NODEID_INFO          -227            //无客户节点信息


//服务器类型
const char svrtype_quote[] = "QUOTE";       //行情服务器
const char svrtype_hts[] = "HTS";           //HTS交易服务器
//系统类型
#define SYSTEM_TYPE_SEC       4           //现货系统
#define SYSTEM_TYPE_MAR       7           //两融系统
#define SYSTEM_TYPE_SOP       9           //期权系统
//安全认证方式
#define FIX_AUTH_STYLE_DTKL   2           //动态口令
#define FIX_AUTH_STYLE_CERT   5           //本地证书
//站点地址内容
#define FIX_NODE_TYPE_MAC     1           //MAC地址
#define FIX_NODE_TYPE_HDSN    2           //硬盘序列号
#define SEPARATOR_SVRADDR     ';'         //多个中间件地址分隔符
#define MAX_ERRMSG_SIZE       1024        //处理结果信息字符串最大长度
#define MAX_RESULT_SIZE       128         //成功时返回的字符串数据最大长度
//银证业务类别
#define YWLB_YHTOZQ           1           //银行转证券
#define YWLB_ZQTOYH           2           //证券转银行
#define YWLB_CXYHYE           6           //查银行余额
//接口推送信息类型
#define NOTIFY_CONNECT        1           //通讯连接(ProfileKey,Message)
#define NOTIFY_DISCONNECT     2           //通讯连接(ProfileKey,Message)
#define NOTIFY_LOGIN          3           //客户登录(ProfileKey,AccountId,Message)
#define NOTIFY_FILEUPGRADE    4           //文件升级(ProfileKey,Message)
#define NOTIFY_FILEUSING      5           //文件升级-文件被占用
#define NOTIFY_SUBSCIBE       6           //订阅处理(ProfileKey,Message)
#define NOTIFY_QUOTESUBSCIBE  7           //行情订阅(Message)
#define NOTIFY_PUSH_ORDER     8           //委托推送(AccountId,OrderId,Message)
#define NOTIFY_PUSH_WITHDRAW  9           //撤单推送(AccountId,OrderId,Message)
#define NOTIFY_PUSH_MATCH     10          //成交推送(AccountId,OrderId,Message)
#define NOTIFY_PUSH_INVALID   11          //废单推送(AccountId,OrderId,Message)
#define NOTIFY_PUSH_MODIFYPWD 12          //客户在其他地方修改密码
#define NOTIFY_PUSH_BULLETIN  13          //公告--备用
#define NOTIFY_PUSH_MESSAGE   14          //消息--备用
#define NOTIFY_ASSETVARY      20          //资产变动(AccountId,FundAccount,Message)--比较频繁，不建议作为显示信息。
#define NOTIFY_QUOTEVARY      21          //行情变动({Market,StockCode,varys}) vars为行情变动标志的位组合
                                          //         例如：SH,600600,00000006，表示SH600600的最新价和成交数量有变动
                                          //行情变动推送非常频繁，不建议作为显示信息。
#define NOTIFY_BANKTRANSFER   22          //银行转账结果(ProfileKey,AccountId,Message)
#define NOTIFY_PUSH_DEBETS_CHANGE   23          //负债变动推送
#define NOTIFY_PUSH_QUOTE     24          //双边报价委托确认推送(AccountId,OrderId,Message)
#define NOTIFY_CONNEVENT_MGR  25          //管理连接
#define NOTIFY_CONNEVENT_TRADE  26        //交易连接
#define NOTIFY_CONNEVENT_QUERY  27        //查询连接
#define NOTIFY_CONNEVENT_SUBS   28        //订阅连接
//交易类别
#define JYLB_BUY              1           //买入
#define JYLB_SALE             2           //卖出
#define JYLB_PGJK             3           //配股缴款
#define JYLB_HGRZ             4           //回购融资
#define JYLB_HGRQ             5           //回购融券
#define JYLB_BONUS            6           //红利
#define JYLB_ZTG              7           //深圳转托管
#define JYLB_XGPH             8           //新股配号
#define JYLB_ZDJY             9           //上海指定交易
#define JYLB_CZJY             10          //上海撤指交易
#define JYLB_ZZZG             11          //转债转股
#define JYLB_ZZHS             12          //转债回售
#define JYLB_ZZSH             13          //转债赎回
#define JYLB_PSSG             14          //配售申购
#define JYLB_RGFX             15          //认购发行
#define JYLB_SG               16          //送股
#define JYLB_DF               17          //兑付
#define JYLB_PSFQ             24          //配售放弃
#define JYLB_ETFSG            29          //ETF申购
#define JYLB_ETFSH            30          //ETF赎回
#define JYLB_ZYQRK            37          //质押券入库
#define JYLB_ZYQCK            38          //质押券出库
#define JYLB_JJRG             41          //基金认购
#define JYLB_JJSG             42          //基金申购
#define JYLB_JJSH             43          //基金赎回
#define JYLB_JJFHSZ           44          //基金分红设置
#define JYLB_JJZH             46          //基金转换
#define JYLB_JJFC             47          //基金分拆
#define JYLB_JJHB             48          //基金合并
#define JYLB_YXMR             55          //意向买入
#define JYLB_YXMC             56          //意向卖出
#define JYLB_DJMR             57          //定价买入
#define JYLB_DJMC             58          //定价卖出
#define JYLB_MRQR             59          //买入确认
#define JYLB_MCQR             60          //卖出确认
#define JYLB_PHMR             78          //盘后买入
#define JYLB_PHMC             79          //盘后卖出
#define JYLB_RZMR             61          //融资买入
#define JYLB_MQHK             62          //卖券还款
#define JYLB_MQHQ             63          //买券还券
#define JYLB_RQMC             64          //融券卖出
#define JYLB_DBWTJ            65          //担保划入
#define JYLB_DBWFH            66          //担保划出
#define JYLB_QYHR             67          //券源划入
#define JYLB_QYHC             68          //券源划出
#define JYLB_YQHZ             69          //余券划转
#define JYLB_HQHZ             70          //还券划转
#define JYLB_RZQP             71          //融资强平
#define JYLB_RQQP             72          //融券强平
#define JYLB_ZJHK             73          //直接还款
#define JYLB_YSYY             76          //预受要约
#define JYLB_JCYS             77          //解除预受要约
#define JYLB_ZFMR			  101		  //增发买入
#define JYLB_PHDJMR           178         //盘后定价买入
#define JYLB_PHDJMC           179         //盘后定价卖出
#define JYLB_HSCX             212         //回售冲销
#define JYLB_SHJJRG           241         //基金认购
#define JYLB_SHJJSG           242         //基金申购
#define JYLB_SHJJSH           243         //基金赎回
#define JYLB_SHJJFHSZ         244         //基金分红设置
#define JYLB_SHJJZH           246         //基金转换
#define JYLB_SHJJFC           247         //基金分拆
#define JYLB_SHJJHB           248         //基金合并
//订单类型
#define DDLX_XJWT                  0           //限价
// 上海市价
#define DDLX_SHHB_ZYWDSYCX         1           //最优五档即时成交剩余撤销
#define DDLX_SHHB_ZYWDSYZXJ        2           //最优五档即时成交剩余转限价
#define DDLX_SHHB_DSFZYJ           4           //对手方最优价格
#define DDLX_SHHB_BFZYJ            5           //本方最优价格
#define DDLX_SHHB_PHDJDZJYSPJ      6           //盘后定价大宗交易收盘价
// 深圳市价
#define DDLX_SZSB_DSFZYJ           101         //对手方最优价格
#define DDLX_SZSB_BFZYJ            102         //本方最优价格
#define DDLX_SZSB_SYCX             103         //即时成交剩余撤销
#define DDLX_SZSB_ZYWDSYCX         104         //最优五档即时成交剩余撤销
#define DDLX_SZSB_QECJCX           105         //全额成交或撤销
#define DDLX_SZSB_PHDJDZJYSPJ      106         //盘后定价大宗交易收盘价
#define DDLX_SZSB_PHDJDZJYSJ       107         //盘后定价大宗交易平均价

//上海期权订单类型
#define DDLX_SHQQ_XJGFD         0           //限价GFD
#define DDLX_SHQQ_SJIOC         1           //市价IOC
#define DDLX_SHQQ_SJZXJGFD      2           //市价剩余转限价GFD
#define DDLX_SHQQ_XJFOK         4           //限价FOK
#define DDLX_SHQQ_SJFOK         5           //市价FOK
//深圳期权订单类型
#define DDLX_SZQQ_XJGFD         0           //限价委托
#define DDLX_SZQQ_XJFOK         4           //限价全额成交或撤销
#define DDLX_SZQQ_DSFZYJ        101         //对手方最优剩余转限价
#define DDLX_SZQQ_BFZYJ         102         //本方最优
#define DDLX_SZQQ_SYCX          103         //市价立即成交剩余撤销
#define DDLX_SZQQ_ZYWDSYCX      104         //市价最优五档全额成交剩余撤销
#define DDLX_SZQQ_QECJCX        105         //市价全额成交或撤销

//订单交易限制
#define OT_ALO                1           //竞价限价盘 At-auction Limit Order(Pre-opening Session and Closing Auction Session)
#define OT_ELO                2           //增强限价盘 Enhanced Limit Order(Continuous Trading Session)
//行情变化标志
#define QV_TPBZ               0x00000001  //停牌标志
#define QV_ZXJ                0x00000002  //最新价
#define QV_ZSP                0x00000004  //昨收盘
#define QV_CJSL               0x00000008  //成交数量(成交金额)
#define QV_CCL                0x00000010  //持仓量
#define QV_ZGZDJ              0x00000020  //最高最低价
#define QV_MRJG1              0x00000100  //买入价格1
#define QV_MCJG1              0x00000200  //卖出价格1
#define QV_MRSL1              0x00000400  //买入数量1
#define QV_MCSL1              0x00000800  //卖出数量1
#define QV_MRJG2              0x00001000
#define QV_MCJG2              0x00002000
#define QV_MRSL2              0x00004000
#define QV_MCSL2              0x00008000
#define QV_MRJG3              0x00010000
#define QV_MCJG3              0x00020000
#define QV_MRSL3              0x00040000
#define QV_MCSL3              0x00080000
#define QV_MRJG4              0x00100000
#define QV_MCJG4              0x00200000
#define QV_MRSL4              0x00400000
#define QV_MCSL4              0x00800000
#define QV_MRJG5              0x01000000
#define QV_MCJG5              0x02000000
#define QV_MRSL5              0x04000000
#define QV_MCSL5              0x08000000
//查询结构排序方式
#define SORT_TYPE_DESC        0        //逆序
#define SORT_TYPE_AES         1        //正序

//申报结果定义
#define  SBJG_WAITING                0       //待报
#define  SBJG_SENDING                1       //正报
#define  SBJG_CONFIRM                2       //已报
#define  SBJG_INVALID                3       //非法委托
#define  SBJG_FUNDREQ                4       //资金申请中
#define  SBJG_PARTTRADE              5       //部分成交
#define  SBJG_COMPLETE               6       //全部成交
#define  SBJG_PTADPWTD               7       //部成部撤
#define  SBJG_WITHDRAW               8       //全部撤单
#define  SBJG_WTDFAIL                9       //撤单未成
#define  SBJG_MANUAL                 10      //等待人工申报

// 异步回调函数交易类型
//      期权类型
#define FUNC_CALLBACK_KPJY                  0         //开平交易
#define FUNC_CALLBACK_KPJY_WITHDRAW         1         //开平交易撤单
#define FUNC_CALLBACK_ORDERPETRUST_BJ       2       //报价
#define FUNC_CALLBACK_KPJY_WITHDRAW_ALL     3         //开平交易全部撤单
#define FUNC_CALLBACK_BJ_WITHDRAW_ALL       4         //报价全部撤单
//      现货类型
#define FUNC_CALLBACK_PTMM          30       //普通买卖
#define FUNC_CALLBACK_ETFSG         31       //ETF申购
#define FUNC_CALLBACK_ETFSH         32       //ETF赎回
#define FUNC_CALLBACK_ZQHG          33       //债券回购
#define FUNC_CALLBACK_ZQCRK         34       //债券出入库
#define FUNC_CALLBACK_LOF           35       //LOF基金
#define FUNC_CALLBACK_PHDJ          36       //盘后定价买卖
#define FUNC_CALLBACK_FXYW          37       //发行业务
#define FUNC_CALLBACK_FJY           38       //非交易业务
#define FUNC_CALLBACK_WD_PTMM       40       //普通买卖撤单
#define FUNC_CALLBACK_WD_BATCH      41       //批量买卖撤单
#define FUNC_CALLBACK_WD_ETF        42       //ETF撤单
#define FUNC_CALLBACK_WD_ZQ         43       //债券回购撤单
#define FUNC_CALLBACK_WD_LOF        44       //LOF撤单
#define FUNC_CALLBACK_WD_PHDJ       45       //盘后定价撤单
#define FUNC_CALLBACK_WD_FXYW       46       //发行业务撤单

//      两融类型
#define MR_FUNC_CALLBACK_XYMM          100       //信用买卖
#define MR_FUNC_CALLBACK_RZRQ          101       //融资融券
#define MR_FUNC_CALLBACK_CHYW          102       //偿还业务(卖券还款、买券还券)
#define MR_FUNC_CALLBACK_FJYYW         103       //非交易业务(担保物提交、担保物返还、还券划转)
#define MR_FUNC_CALLBACK_FXYW          104       //发行业务(配售申购、配股缴款)
#define MR_FUNC_CALLBACK_PHDJ          105       //盘后定价
#define MR_FUNC_CALLBACK_WD_XYMM       120       //信用买卖撤单
#define MR_FUNC_CALLBACK_WD_RZRQ       121       //融资融券撤单
#define MR_FUNC_CALLBACK_WD_CHYW       122       //偿还业务撤单(卖券还款、买券还券)
#define MR_FUNC_CALLBACK_WD_FJYYW      123       //非交易业务撤单(担保物提交、担保物返还、还券划转)
#define MR_FUNC_CALLBACK_WD_FXYW       124       //发行业务撤单(配售申购、配股缴款)
#define MR_FUNC_CALLBACK_WD_PHDJ       125       //盘后定价撤单

//表名--缓存数据名字
const char table_khh[] = "tKHH";      //客户号
const char table_bz[] = "tBZ";       //币种
const char table_jys[] = "tJYS";      //交易所
const char table_xtdm[] = "tXTDM";     //系统代码
const char table_zqjysx[] = "tZQJYSX";   //证券交易属性
const char table_jylb[] = "tJYLB";     //交易类别
const char table_jgdm[] = "tJGDM";     //机构代码
const char table_yhcs[] = "tYHCS";     //银行参数
const char table_etfxx[] = "tETFXX";    //ETF信息
const char table_etfmx[] = "tETFMX";    //ETF成分股明细
const char table_jjhq[] = "tJJHQ";     //基金信息
const char table_fjjj[] = "tFJJJ";     //分级基金
const char table_xgsg[] = "tXGSG";     //本日新股申购
const char table_hlcs[] = "tHLCS";     //汇率参数
const char table_zqhq[] = "tZQHQ";     //证券行情
const char table_jyjw[] = "tJYJW";     //交易价位
const char table_zjzh[] = "tZJZH";     //资金账号
const char table_zcxx[] = "tZCXX";     //资产统计
const char table_gdh[] = "tGDH";      //股东号
const char table_yhzh[] = "tYHZH";     //银行账号
const char table_zqgl[] = "tZQGL";     //证券持仓
const char table_drwt[] = "tDRWT";     //当日委托
const char table_sscj[] = "tSSCJ";     //实时成交
const char table_zzsq[] = "tZZSQ";     //证券发起的转账申请
const char table_jgmx[] = "tJGMX";     //交割流水
const char table_wtls[] = "tWTLS";     //历史委托
const char table_zjls[] = "tZJLS";     //资金流水
const char table_zqjk[] = "tZQJK";     //中签缴款
const char table_sodrwt[] = "tSODRWT";   //期权当日委托
const char table_sohycc[] = "tSOHYCC";   //期权合约持仓
const char table_sobdzqcc[] = "tSOBDZQCC";   //期权备兑证券持仓
const char table_sozhclcc[] = "tSOZHCLCC";   //期权组合策略持仓
const char table_sobzjxx[] = "tSOBZJXX";  //期权保证金
const char table_sosscj[] = "tSOSSCJ";     //期权实时成交
const char table_sofbcj[] = "tSOFBCJ";     //期权分笔成交
const char table_sohydm[] = "tSOHYDM";     //期权合约代码
const char table_sohyzh[] = "tSOHYZH";     //期权合约账户
const char table_sokhjykzxx[] = "tSOKHJYKZXX";     //期权客户交易控制信息
const char table_sokhxqdjszjqk[] = "tSOKHXQDJSZJQK";     //期权客户行权待交收资金缺口
const char table_sokhxqdjszqqk[] = "tSOKHXQDJSZQQK";     //期权客户行权待交收证券缺口
const char table_sokhxqzpxx[] = "tSOKHXQZPXX";     //期权客户行权指派信息
const char table_sokhbdzqbz[] = "tSOKHBDZQBZ";     //期权客户备兑证券不足
const char table_sokhbcxx[] = "tSOKHBCXX";     //期权客户补充信息

//两融相关表
const char table_xyxgsg[] = "tXYXGSG";   //信用本日新股申购
const char table_xygdh[] = "tXYGDH";    //信用股东号
const char table_xyzjzh[] = "tXYZJZH";   //信用资金账号
const char table_xyzqgl[] = "tXYZQGL";   //信用证券持仓
const char table_xydrwt[] = "tXYDRWT";   //信用当日委托
const char table_xysscj[] = "tXYSSCJ";   //信用实时成交
const char table_xyzc[] = "tXYZC";     //信用资产
const char table_xyfz[] = "tXYFZ";     //信用负债
const char table_xypsqy[] = "tXYPSQY";   //信用配售权益
const char table_zgzq[] = "tZGZQ";     //资格证券
const char table_rqzq[] = "tRQZQ";     //标的证券，融券余额
const char table_xydrbd[] = "tXYDRBD";   //当日信用负债变动明细
const char table_xyrqfzhz[] = "tXYRQFZHZ";   //当日信用融券负债汇总
const char table_xyfzls[] = "tXYFZLS";   //信用负债变动明细
const char table_xyzqdm[] = "tXYZQDM";     //信用证券代码

//交易所
const char dict_jys_dl[] = "DL";        //大连
const char dict_jys_sq[] = "SQ";        //上期
const char dict_jys_zj[] = "ZJ";        //中金
const char dict_jys_zz[] = "ZZ";        //郑州
const char dict_jys_sn[] = "SN";        //原油
const char dict_jys_sh[] = "SH";        //上海
const char dict_jys_sz[] = "SZ";        //深圳
const char dict_jys_hk_sh[] = "HK";        //沪港
const char dict_jys_hk_sz[] = "SK";        //深港
//币种
const char dict_bz_rmb[] = "RMB";       //人民币
const char dict_bz_hkd[] = "HKD";       //港币
const char dict_bz_usd[] = "USD";       //美元

