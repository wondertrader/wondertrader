#pragma once
#include <stdint.h>
#include <memory>

#include "../Share/BoostMappingFile.hpp"
#include "../Includes/FasterDefs.h"

USING_NS_WTP;

typedef std::shared_ptr<BoostMappingFile> MappedFilePtr;

namespace shareblock
{
	const char BLK_FLAG[] = "&^%$#@!\0";

	const int FLAG_SIZE = 8;
	const int MAX_SEC_CNT = 64;

	typedef uint64_t ValueType;
	const ValueType	SMVT_INT32 = 1;
	const ValueType	SMVT_UINT32 = 2;
	const ValueType	SMVT_INT64 = 3;
	const ValueType	SMVT_UINT64 = 4;
	const ValueType	SMVT_DOUBLE = 5;
	const ValueType	SMVT_STRING = 6;

	const std::size_t VTL_INT32 = 4;
	const std::size_t VTL_UINT32 = 4;
	const std::size_t VTL_INT64 = 8;
	const std::size_t VTL_UINT64 = 8;
	const std::size_t VTL_DOUBLE = 8;
	const std::size_t VTL_STRING = 64;

	#pragma pack(push, 1)
	typedef struct _KeyInfo
	{
		char		_key[32];
		ValueType	_type;
		uint32_t	_offset;
		uint64_t	_updatetime;
	} KeyInfo;

	/*
	 *	小节信息
	 */
	typedef struct _SectionInfo
	{
		char		_name[32];
		KeyInfo		_keys[32];
		uint32_t	_count;			//数据条数，即key的个数
		uint32_t	_offset;		//记录下一个可分配地址的偏移量
		uint64_t	_updatetime;
		char		_data[1024];

		_SectionInfo()
		{
			memset(this, 0, sizeof(_SectionInfo));
		}
	} SecInfo;

	typedef struct _ShmBlock
	{
		char		_flag[8];
		char		_name[32];
		SecInfo		_sections[MAX_SEC_CNT];
		uint64_t	_updatetime;
		uint32_t	_count;

		_ShmBlock()
		{
			memset(this, 0, sizeof(_ShmBlock));
		}
	} ShmBlock;
	#pragma pack(pop)


	class ShareBlocks
	{
	private:
		ShareBlocks(){}

	public:
		static ShareBlocks& one()
		{
			static ShareBlocks inst;
			return inst;
		}

		bool	init_master(const char* name, const char* path = "");

		bool	init_slave(const char* name, const char* path = "");

		std::vector<std::string>	get_sections(const char* domain);
		std::vector<KeyInfo*>		get_keys(const char* domain, const char* section);

		uint64_t get_section_updatetime(const char* domain, const char* section);
		bool	commit_section(const char* domain, const char* section);

		bool	set_string(const char* domain, const char* section, const char* key, const char* val);
		bool	set_int32(const char* domain, const char* section, const char* key, int32_t val);
		bool	set_int64(const char* domain, const char* section, const char* key, int64_t val);
		bool	set_uint32(const char* domain, const char* section, const char* key, uint32_t val);
		bool	set_uint64(const char* domain, const char* section, const char* key, uint64_t val);
		bool	set_double(const char* domain, const char* section, const char* key, double val);

		const char*	get_string(const char* domain, const char* section, const char* key, const char* defVal = "");
		int32_t		get_int32(const char* domain, const char* section, const char* key, int32_t defVal = 0);
		int64_t		get_int64(const char* domain, const char* section, const char* key, int64_t defVal = 0);
		uint32_t	get_uint32(const char* domain, const char* section, const char* key, uint32_t defVal = 0);
		uint64_t	get_uint64(const char* domain, const char* section, const char* key, uint64_t defVal = 0);
		double		get_double(const char* domain, const char* section, const char* key, double defVal = 0);

	private:
		void*	make_valid(const char* domain, const char* section, const char* key, std::size_t len, SecInfo* &secInfo);

		void*	check_valid(const char* domain, const char* section, const char* key, ValueType vType, SecInfo* &secInfo);

	private:
		typedef struct _ShmPair
		{
			MappedFilePtr	_domain;
			ShmBlock*		_block;
			bool			_master;

			typedef faster_hashmap<ShortKey, KeyInfo*>	KVMap;
			typedef struct _KVPair
			{
				uint32_t	_index;
				KVMap		_keys;
			} KVPair;
			typedef faster_hashmap<ShortKey, KVPair>	SectionMap;
			SectionMap	_sections;

			_ShmPair() :_block(nullptr),_master(false)
			{
			}
		}ShmPair;
		typedef faster_hashmap<ShortKey, ShmPair>	ShmBlockMap;
		ShmBlockMap		_shm_blocks;
	};
}