#pragma once
#include <stdint.h>
#include <string>

#include "../Share/StdUtils.hpp"
#include "../Includes/FasterDefs.h"
#include "../Share/DLLHelper.hpp"

NS_WTP_BEGIN
class WtUftEngine;
NS_WTP_END
USING_NS_WTP;

typedef bool (*func_init_master)(const char*, const char*);
typedef uint64_t(*func_get_section_updatetime)(const char*, const char*);
typedef bool(*func_commit_section)(const char*, const char*);
typedef const char*(*func_allocate_string)(const char*, const char*, const char*, const char*, bool);
typedef int32_t* (*func_allocate_int32)(const char*, const char*, const char*, int32_t, bool);
typedef int64_t* (*func_allocate_int64)(const char*, const char*, const char*, int64_t, bool);
typedef uint32_t* (*func_allocate_uint32)(const char*, const char*, const char*, uint32_t, bool);
typedef uint64_t* (*func_allocate_uint64)(const char*, const char*, const char*, uint64_t, bool);
typedef double*	(*func_allocate_double)(const char*, const char*, const char*, double, bool);
typedef bool (*func_set_string)(const char*, const char*, const char*, const char*);
typedef bool (*func_set_int32)(const char*, const char*, const char*, int32_t);
typedef bool (*func_set_int64)(const char*, const char*, const char*, int64_t);
typedef bool (*func_set_uint32)(const char*, const char*, const char*, uint32_t);
typedef bool(*func_set_uint64)(const char*, const char*, const char*, uint64_t);
typedef bool(*func_set_double)(const char*, const char*, const char*, double);
typedef const char* (*func_get_string)(const char*, const char*, const char*, const char*);
typedef int32_t (*func_get_int32)(const char*, const char*, const char*, int32_t);
typedef int64_t (*func_get_int64)(const char*, const char*, const char*, int64_t);
typedef uint32_t (*func_get_uint32)(const char*, const char*, const char*, uint32_t);
typedef uint64_t (*func_get_uint64)(const char*, const char*, const char*, uint64_t);
typedef double (*func_get_double)(const char*, const char*, const char*, double);

class ShareManager
{
private:
	ShareManager():_inited(false), _stopped(false), _engine(nullptr), _sync("sync"){}
	~ShareManager()
	{
		_stopped = true;
		if (_worker)
			_worker->join();
	}

public:
	static ShareManager& self()
	{
		static ShareManager inst;
		return inst;
	}

	void	set_engine(WtUftEngine* engine) { _engine = engine; }

	bool	initialize(const char* module);

	bool	start_watching(uint32_t microsecs);

	bool	init_domain(const char* id);

	bool	commit_param_watcher(const char* section);

	bool	set_value(const char* section, const char* key, const char* val);

	bool	set_value(const char* section, const char* key, int32_t val);

	bool	set_value(const char* section, const char* key, int64_t val);

	bool	set_value(const char* section, const char* key, uint32_t val);

	bool	set_value(const char* section, const char* key, uint64_t val);

	bool	set_value(const char* section, const char* key, double val);

	const char*	get_value(const char* section, const char* key, const char* defVal = "");

	int32_t		get_value(const char* section, const char* key, int32_t defVal = 0);

	int64_t		get_value(const char* section, const char* key, int64_t defVal = 0);

	uint32_t	get_value(const char* section, const char* key, uint32_t defVal = 0);

	uint64_t	get_value(const char* section, const char* key, uint64_t defVal = 0);

	double		get_value(const char* section, const char* key, double defVal = 0);

	/*
	 *	在单向同步区分配字段
	 */
	const char*	allocate_value(const char* section, const char* key, const char* initVal = "", bool bForceWrite = false, bool isExchg = false);
	int32_t*	allocate_value(const char* section, const char* key, int32_t initVal = 0, bool bForceWrite = false, bool isExchg = false);
	int64_t*	allocate_value(const char* section, const char* key, int64_t initVal = 0, bool bForceWrite = false, bool isExchg = false);
	uint32_t*	allocate_value(const char* section, const char* key, uint32_t initVal = 0, bool bForceWrite = false, bool isExchg = false);
	uint64_t*	allocate_value(const char* section, const char* key, uint64_t initVal = 0, bool bForceWrite = false, bool isExchg = false);
	double*		allocate_value(const char* section, const char* key, double initVal = 0, bool bForceWrite = false, bool isExchg = false);

private:
	bool			_inited;
	std::string		_exchg;	//交换区
	std::string		_sync;

	wt_hashmap<std::string, uint64_t>	_secnames;

	bool			_stopped;
	StdThreadPtr	_worker;
	WtUftEngine*	_engine;

	DllHandle		_inst;
	std::string		_module;

	func_init_master _init_master;
	func_get_section_updatetime _get_section_updatetime;
	func_commit_section _commit_section;

	func_set_double _set_double;
	func_set_int32 _set_int32;
	func_set_int64 _set_int64;
	func_set_uint32 _set_uint32;
	func_set_uint64 _set_uint64;
	func_set_string _set_string;

	func_get_double _get_double;
	func_get_int32 _get_int32;
	func_get_int64 _get_int64;
	func_get_uint32 _get_uint32;
	func_get_uint64 _get_uint64;
	func_get_string _get_string;

	func_allocate_double _allocate_double;
	func_allocate_int32 _allocate_int32;
	func_allocate_int64 _allocate_int64;
	func_allocate_uint32 _allocate_uint32;
	func_allocate_uint64 _allocate_uint64;
	func_allocate_string _allocate_string;
};

