#pragma once
#include "../Includes/WTSStruct.h"

USING_NS_WTP;

#pragma pack(push, 1)

const char BLK_FLAG[] = "&^%$#@!\0";

const int FLAG_SIZE = 8;

typedef enum tagBlockType
{
	BT_RT_Cache			= 4		//实时缓存
} BlockType;

#define BLOCK_VERSION_RAW	1	//普通版本

typedef struct _BlockHeader
{
	char		_blk_flag[FLAG_SIZE];
	uint16_t	_type;
	uint16_t	_version;
} BlockHeader;

#define BLOCK_HEADER_SIZE	sizeof(BlockHeader)

typedef struct _RTBlockHeader : BlockHeader
{
	uint32_t _size;
	uint32_t _capacity;
} RTBlockHeader;

typedef struct _TickCacheItem
{
	uint32_t		_date;
	WTSTickStruct	_tick;
} TickCacheItem;

//实时tick缓存
typedef struct _RTTickCache : RTBlockHeader
{
	TickCacheItem	_items[0];
} RTTickCache;

typedef struct _BarCacheItem
{
	char			_exchg[16];
	char			_code[32];
	WTSBarStruct	_bar;
} BarCacheItem;

//实时K线缓存
typedef struct _RTBarCache : RTBlockHeader
{
	BarCacheItem	_items[0];
} RTBarCache;

#pragma pack(pop)
