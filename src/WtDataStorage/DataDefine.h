#pragma once
#include "../Includes/WTSStruct.h"

USING_NS_WTP;

#pragma pack(push, 1)

const char BLK_FLAG[] = "&^%$#@!\0";

const int FLAG_SIZE = 8;

typedef enum tagBlockType
{
	BT_RT_Minute1		= 1,	//实时1分钟线
	BT_RT_Minute5		= 2,	//实时5分钟线
	BT_RT_Ticks			= 3,	//实时tick数据
	BT_RT_Cache			= 4,	//实时缓存
	BT_RT_Trnsctn		= 5,	//实时逐笔成交
	BT_RT_OrdDetail		= 6,	//实时逐笔委托
	BT_RT_OrdQueue		= 7,	//实时委托队列

	BT_HIS_Minute1		= 21,	//历史1分钟线
	BT_HIS_Minute5		= 22,	//历史5分钟线
	BT_HIS_Day			= 23,	//历史日线
	BT_HIS_Ticks		= 24,	//历史tick
	BT_HIS_Trnsctn		= 25,	//历史逐笔成交
	BT_HIS_OrdDetail	= 26,	//历史逐笔委托
	BT_HIS_OrdQueue		= 27	//历史委托队列
} BlockType;

#define BLOCK_VERSION_RAW		0x01	//老结构体未压缩
#define BLOCK_VERSION_CMP		0x02	//老结构体压缩
#define BLOCK_VERSION_RAW_V2	0x03	//新结构体未压缩
#define BLOCK_VERSION_CMP_V2	0x04	//新结构体压缩

typedef struct _BlockHeader
{
	char		_blk_flag[FLAG_SIZE];
	uint16_t	_type;
	uint16_t	_version;

	inline bool is_old_version() const {
		return (_version == BLOCK_VERSION_CMP || _version == BLOCK_VERSION_RAW);
	}

	inline bool is_compressed() const {
		return (_version == BLOCK_VERSION_CMP || _version == BLOCK_VERSION_CMP_V2);
	}
} BlockHeader;

typedef struct _BlockHeaderV2
{
	char		_blk_flag[FLAG_SIZE];
	uint16_t	_type;
	uint16_t	_version;

	uint64_t	_size;		//压缩后的数据大小

	inline bool is_old_version() const {
		return (_version == BLOCK_VERSION_CMP || _version == BLOCK_VERSION_RAW);
	}

	inline bool is_compressed() const {
		return (_version == BLOCK_VERSION_CMP || _version == BLOCK_VERSION_CMP_V2);
	}
} BlockHeaderV2;

#define BLOCK_HEADER_SIZE	sizeof(BlockHeader)
#define BLOCK_HEADERV2_SIZE sizeof(BlockHeaderV2)

typedef struct _RTBlockHeader : BlockHeader
{
	uint32_t _size;
	uint32_t _capacity;
} RTBlockHeader;

//每日实时数据块头部
typedef struct _RTDayBlockHeader : RTBlockHeader
{
	uint32_t		_date;
} RTDayBlockHeader;

//实时K线数据块
typedef struct _RTKlineBlock : _RTDayBlockHeader
{
	WTSBarStruct	_bars[0];
} RTKlineBlock;

//tick数据数据块
//By Wesley @ 2021.12.30
//实时tick缓存，直接用新版本的tick结构
//切换程序一定要在盘后进行！！！
typedef struct _RTTickBlock : RTDayBlockHeader
{
	WTSTickStruct	_ticks[0];
} RTTickBlock;

//逐笔成交数据块
typedef struct _RTTransBlock : RTDayBlockHeader
{
	WTSTransStruct	_trans[0];
} RTTransBlock;

//逐笔委托数据块
typedef struct _RTOrdDtlBlock : RTDayBlockHeader
{
	WTSOrdDtlStruct	_details[0];
} RTOrdDtlBlock;

//委托队列数据块
typedef struct _RTOrdQueBlock : RTDayBlockHeader
{
	WTSOrdQueStruct	_queues[0];
} RTOrdQueBlock;

typedef struct _TickCacheItem
{
	uint32_t		_date;
	WTSTickStruct	_tick;
} TickCacheItem;

//实时tick缓存
typedef struct _RTTickCache : RTBlockHeader
{
	TickCacheItem	_ticks[0];
} RTTickCache;


//历史Tick数据
typedef struct _HisTickBlock : BlockHeader
{
	WTSTickStruct	_ticks[0];
} HisTickBlock;

//历史Tick数据V2
typedef struct _HisTickBlockV2 : BlockHeaderV2
{
	char			_data[0];
} HisTickBlockV2;

typedef struct _HisTransBlock : BlockHeader
{
	WTSTransStruct	_items[0];
} HisTransBlock;

typedef struct _HisTransBlockV2 : BlockHeaderV2
{
	char			_data[0];
} HisTransBlockV2;

typedef struct _HisOrdDtlBlock : BlockHeader
{
	WTSOrdDtlStruct	_items[0];
} HisOrdDtlBlock;

typedef struct _HisOrdDtlBlockV2 : BlockHeaderV2
{
	char			_data[0];
} HisOrdDtlBlockV2;

typedef struct _HisOrdQueBlock : BlockHeader
{
	WTSOrdQueStruct	_items[0];
} HisOrdQueBlock;

typedef struct _HisOrdQueBlockV2 : BlockHeaderV2
{
	char			_data[0];
} HisOrdQueBlockV2;

//历史K线数据
typedef struct _HisKlineBlock : BlockHeader
{
	WTSBarStruct	_bars[0];
} HisKlineBlock;

//历史K线数据V2
typedef struct _HisKlineBlockV2 : BlockHeaderV2
{
	char			_data[0];
} HisKlineBlockV2;

//历史K线数据
typedef struct _HisKlineBlockOld : BlockHeader
{
	WTSBarStructOld	_bars[0];
} HisKlineBlockOld;

#pragma pack(pop)
