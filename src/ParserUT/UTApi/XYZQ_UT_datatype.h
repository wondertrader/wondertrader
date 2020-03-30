//#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000
#ifdef linux 
#define __stdcall
#endif

#define max_code_length         32
#define short_string_length     64
#define long_string_length      512
typedef char ut_code_string[max_code_length];
typedef char ut_short_string[short_string_length];
typedef char ut_long_string[long_string_length];

// 数据类型
#define ut_null_type       0
#define ut_int_type        1
#define ut_uint_type       2
#define ut_int64_type      3
#define ut_uint64_type     4
#define ut_double_type     5
#define ut_string_type     6

// 订阅类型
#define ut_all_sub            0
#define ut_snapshot_sub       1
#define ut_transaction_sub    2
#define ut_order_sub          3
#define ut_order_queue_sub    4

// 品种类型
#define ut_stocktype_unknown        0
#define ut_stocktype_index          1
#define ut_stocktype_stock          2
#define ut_stocktype_fund           3
#define ut_stocktype_bond           4
#define ut_stocktype_option         5
#define ut_stocktype_future         6
#define ut_stocktype_others         100

// 行情数据结构
#pragma pack(push)
#pragma pack(4)
typedef struct _ut_data_item
{
    unsigned short tag;
    unsigned short dtype;
    union 
    {
        int             int_value;
        unsigned int    uint_value;
        long long       int64_value;
        double          double_value;
        char*           string_value;
    };
} ut_data_item;
#pragma pack(pop)

#ifdef __cplusplus
#include <string>
#include <map>
#include <vector>

using namespace std;

//修改密码
#pragma pack(push)
#pragma pack(4)
typedef struct change_pwd
{
	string account_no;
	string username;
	string oldpassword;
	string newpassword;
} change_pwd;

//错误信息包
typedef struct error_msg_struct
{
	int err_code;
	int related_packet_id;
	string msg;
} error_msg;

//登录包
typedef struct authorize_struct
{
	string account_no;
	string username;
	string password;
} authorize_struct;

//订阅、取快照包
// 字段表
typedef vector<int> sub_fields;

// 单个code订阅表
typedef struct sub_item
{
    string code;
    sub_fields fields;
} sub_item;

// 多个code订阅表
typedef vector<sub_item> sub_list;
typedef vector<string> code_list;

//退订包
typedef struct unsub
{
	int sub_type;
	code_list codes;
} unsub_struct;

//行情推送包
typedef struct _ut_data
{
    int object_type;
    string xyzq_code;
    vector<ut_data_item> quotations;
}  ut_data;

typedef struct _ut_data_list
{
    int related_packet_id;
    vector<ut_data> data_list;
}  ut_data_list;

// 委托队列包
typedef struct _ut_order_queue
{
    int direction;
    double price;
    int orders_count;
    vector<int> orders;
} ut_order_queue;

typedef struct _ut_order_queue_list
{
    int object_type;
    string xyzq_code;
    int time;
    int millisecond;
    vector<ut_order_queue> order_queues;
} ut_order_queue_list;

//基础信息请求包
typedef struct base_info_req
{
	string xyzq_code;
	vector<int> tags;
} base_info_req;

typedef struct ttv
{
	int tag;
	int tagtype;
	int tagvalue;
} ttv;

//基础信息返回包
typedef struct rsp_base_info_s
{
	char code_type;
	string xyzq_code;
	vector<ttv> ttvs;
} rsp_base_info_s;
#pragma pack(pop)
#endif

// C style definitions
typedef void (__stdcall *xyzq_ut_c_spi) (int msgtype, void* msg_body);
///////////////////////////
// c style callback msgtype
///////////////////////////
#define ut_null_msg               0     //未知信息
#define ut_error_msg              1 	//错误信息响应
#define ut_status_changed_msg     2	    //状态响应
#define ut_market_data_msg        3	    //单个品种行情推送
#define ut_transactions_msg       4	    //单个品种逐笔成交推送
#define ut_orders_msg             5     //单个品种逐笔委托推送
#define ut_order_list_msg         6	    //单个品种委托队列推送
#define ut_base_info_msg          7	    //查询基础信息响应
#define ut_multi_market_data_msg  8     //多个品种行情推送

///////////////////////////
// c style object type
///////////////////////////
#define ut_data_list_c_type       1 	

#pragma pack(push)
#pragma pack(4)
typedef struct _ut_his_data_c
{
    ut_code_string xyzq_code;
    int const_data_count;
    ut_data_item const_data[8];
    ut_short_string const_data_brief[8];
    int data_length;
    ut_data_item quotations[1];
}  ut_his_data_c;

typedef struct _ut_data_c
{
    int object_type;
    ut_code_string xyzq_code;
    int data_length;
    ut_data_item quotations[1];
}  ut_data_c;

typedef struct _ut_data_list_c
{
    int related_packet_id;
    int code_count;
    ut_data_c* data_pointers[1];
}  ut_data_list_c;

typedef struct _ut_order_queue_c
{
    int direction;
    double price;
    int orders_count;
    int shown_orders_count;
    int orders[1];
} ut_order_queue_c;

typedef struct _ut_order_queue_list_c
{
    int object_type;
    ut_code_string xyzq_code;
    int time;
    int millisecond;
    int queue_count;
    ut_order_queue_c* queue_pointers[1];
} ut_order_queue_list_c;

typedef struct error_msg_struct_c
{
	int err_code;
	int related_packet_id;
	ut_long_string msg;
} error_msg_c;

//订阅、取快照包
// 字段表
typedef struct sub_fields_c
{
    int count;
    int fields[1];
} sub_fields_c;

// 单个code订阅表
typedef struct sub_item_c
{
    ut_code_string code;
    sub_fields_c fields;
} sub_item_c;
#pragma pack(pop)
