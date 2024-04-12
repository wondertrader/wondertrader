#pragma once
#include <stdint.h>
#include <memory>

#include "../Share/BoostMappingFile.hpp"
#include "../Share/fmtlib.h"
#include "../Includes/FasterDefs.h"

USING_NS_WTP;

typedef std::shared_ptr<BoostMappingFile> MappedFilePtr;

namespace shareblock
{
	const char BLK_FLAG[] = "&^%$#@!\0";

	constexpr int FLAG_SIZE = 8;
	constexpr int MAX_SEC_CNT = 64;
	constexpr int MAX_KEY_CNT = 128;
	constexpr int MAX_CMD_SIZE = 64;

	typedef enum tagValueType : uint64_t
	{
		SMVT_INT32 = 1,
		SMVT_UINT32 = 2,
		SMVT_INT64 = 3,
		SMVT_UINT64 = 4,
		SMVT_DOUBLE = 5,
		SMVT_STRING = 6
	} ValueType;

	const std::size_t SMVT_SIZES[] = { 0,4,4,8,8,8,64 };

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
		KeyInfo		_keys[MAX_KEY_CNT];
		uint16_t	_count;			//数据条数，即key的个数
		uint16_t	_state;			//状态：0-无效，1-生效
		uint32_t	_offset;		//记录下一个可分配地址的偏移量
		uint64_t	_updatetime;
		char		_data[1024];	//极端情况下，128个double，刚好是1024，考虑到还有一些int和string，算下来刚好

		template<typename T>
		T* get(uint32_t offset)
		{
			return (T*)(_data + offset);
		}

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

	typedef struct _CmdInfo
	{
		uint32_t	_state;
		char		_command[MAX_CMD_SIZE];

		_CmdInfo() { memset(this, 0, sizeof(_CmdInfo)); }
	} CmdInfo;

	template <int N = 128>
	struct _CmdBlock
	{
		uint32_t	_capacity = N;
	 	volatile uint32_t	_readable;
		volatile uint32_t	_writable;
		uint32_t	_cmdpid;
		CmdInfo		_commands[N];

		_CmdBlock():_readable(UINT32_MAX),_writable(0),_cmdpid(0){}
	};

	typedef _CmdBlock<128>	CmdBlock;

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

		typedef void(*FuncLogger)(uint32_t, const char*);
		void	register_logger(FuncLogger logger)
		{
			_logger = logger;
		}

		template<typename ...Args>
		void	write_log(uint32_t lvl, const char* format, const Args&... args) noexcept
		{
			if (!_logger)
				return;

			const char* buffer = fmtutil::format(format, args...);
			_logger(lvl, buffer);
		}

		bool	init_master(const char* name, const char* path = "");
		bool	init_slave(const char* name, const char* path = "");

		bool	update_slave(const char* name, bool bForce);
		bool	release_slave(const char* name);

		std::vector<std::string>	get_sections(const char* domain);
		std::vector<KeyInfo*>		get_keys(const char* domain, const char* section);

		uint64_t get_section_updatetime(const char* domain, const char* section);
		bool	commit_section(const char* domain, const char* section);

		bool	delete_section(const char* domain, const char*section);

		const char* allocate_string(const char* domain, const char* section, const char* key, const char* initVal = "", bool bForceWrite = false);
		int32_t*	allocate_int32(const char* domain, const char* section, const char* key, int32_t initVal = 0, bool bForceWrite = false);
		int64_t*	allocate_int64(const char* domain, const char* section, const char* key, int64_t initVal = 0, bool bForceWrite = false);
		uint32_t*	allocate_uint32(const char* domain, const char* section, const char* key, uint32_t initVal = 0, bool bForceWrite = false);
		uint64_t*	allocate_uint64(const char* domain, const char* section, const char* key, uint64_t initVal = 0, bool bForceWrite = false);
		double*		allocate_double(const char* domain, const char* section, const char* key, double initVal = 0, bool bForceWrite = false);

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

	public:
		bool	init_cmder(const char* name, bool isCmder = false, const char* path = "");
		bool	add_cmd(const char* name, const char* cmd);
		const char*	get_cmd(const char* name, uint32_t& lastIdx);

	private:
		void*	make_valid(const char* domain, const char* section, const char* key, ValueType vType, SecInfo* &secInfo);
		void*	check_valid(const char* domain, const char* section, const char* key, ValueType vType, SecInfo* &secInfo);

	private:
		typedef struct _ShmPair
		{
			MappedFilePtr	_domain;
			ShmBlock*		_block;
			bool			_master;
			uint64_t		_blocktime;

			typedef wt_hashmap<std::string, KeyInfo*>	KVMap;
			typedef struct _KVPair
			{
				uint32_t	_index;
				KVMap		_keys;
			} KVPair;
			typedef wt_hashmap<std::string, KVPair>	SectionMap;
			SectionMap	_sections;

			_ShmPair() :_block(nullptr),_master(false)
			{
			}
		}ShmPair;
		typedef wt_hashmap<std::string, ShmPair>	ShmBlockMap;
		ShmBlockMap		_shm_blocks;

		typedef struct _CmdPair
		{
			MappedFilePtr	_domain;
			CmdBlock*		_block;
			bool			_cmder;
		} CmdPair;
		typedef wt_hashmap<std::string, CmdPair>	CmdBlockMap;
		CmdBlockMap		_cmd_blocks;

		FuncLogger	_logger = nullptr;
	};
}