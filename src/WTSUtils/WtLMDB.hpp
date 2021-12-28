#pragma once
#include <stdint.h>
#include <string>
#include <io.h>
#include <direct.h>
#include <functional>
#include <vector>

#include "../Includes/WTSMarcos.h"
#include "lmdb/lmdb.h"

NS_WTP_BEGIN

typedef std::vector<std::string> ValueArray;
typedef std::function<void(const ValueArray&, const ValueArray&)> LMDBQueryCallback;

class WtLMDB
{
public:
	WtLMDB(bool bReadOnly = false)
		: _env(NULL)
		, _dbi(0)
		, _errno(0)
		, _readonly(bReadOnly)
	{}

	~WtLMDB()
	{
		if (_dbi != 0)
			mdb_dbi_close(_env, _dbi);

		if (_env != NULL)
			mdb_env_close(_env);
	}

public:
	inline MDB_env* env() const{ return _env; }
	inline MDB_dbi	dbi() const { return _dbi; }

	inline MDB_dbi update_dbi(MDB_txn* txn)
	{
		if (_dbi != 0)
			return _dbi;

		_errno = mdb_dbi_open(txn, NULL, 0, &_dbi);
		return _dbi;
	}

	bool open(const char* path, uint32_t mapsize = 8*1024*1024)
	{
		if(_access(path, 0) != 0)
		{
			_mkdir(path);
		}

		int _errno = mdb_env_create(&_env);
		if (_errno != MDB_SUCCESS)
			return false;

		_errno = mdb_env_open(_env, path, 0, 0664);
		if (_errno != MDB_SUCCESS)
			return false;

		return true;
	}

	inline void update_errno(int error) { _errno = error; }

	inline bool has_error() const { return _errno != MDB_SUCCESS; }

	inline bool is_readonly() const { return _readonly; }

	inline const char* errmsg()
	{
		return mdb_strerror(_errno);
	}

private:
	MDB_env*	_env;
	MDB_dbi		_dbi;
	int			_errno;
	bool		_readonly;
};

class WtLMDBQuery
{
public:
	WtLMDBQuery(WtLMDB& db)
		: _txn(NULL)
		, _commited(false)
		, _db(db)
	{
		_readonly = db.is_readonly();
		_db.update_errno(mdb_txn_begin(_db.env(), NULL, (_readonly ? MDB_RDONLY : 0), &_txn));
		_dbi = _db.update_dbi(_txn);
	}
	
	~WtLMDBQuery()
	{
		if (_readonly)
			mdb_txn_abort(_txn);
		else if(!_commited)
			_db.update_errno(mdb_txn_commit(_txn));
	}

public:
	inline void	rollback()
	{
		if (_commited)
			return;

		mdb_txn_abort(_txn);
		_commited = true;
	}

	inline void	commit()
	{
		if (_commited || _readonly)
			return;

		_db.update_errno(mdb_txn_commit(_txn));
		_commited = true;
	}

	bool put(const std::string& key, const std::string& val)
	{
		MDB_val mKey, mData;
		mKey.mv_data = (void*)key.data();
		mKey.mv_size = key.size();

		mData.mv_data = (void*)val.data();
		mData.mv_size = val.size();
		int _errno = mdb_put(_txn, _dbi, &mKey, &mData, 0);
		_db.update_errno(_errno);
		return (_errno == MDB_SUCCESS);
	}

	bool put(void* key, uint32_t klen, void* val, uint32_t vlen)
	{
		MDB_val mKey, mData;
		mKey.mv_data = key;
		mKey.mv_size = klen;

		mData.mv_data = val;
		mData.mv_size = vlen;
		int _errno = mdb_put(_txn, _dbi, &mKey, &mData, 0);
		_db.update_errno(_errno);
		return (_errno == MDB_SUCCESS);
	}

	std::string get(const std::string& key)
	{
		MDB_cursor* cursor;
		int _errno = mdb_cursor_open(_txn, _dbi, &cursor);
		_db.update_errno(_errno);
		if (_errno != MDB_SUCCESS)
			return std::move(std::string());

		MDB_val mKey, mData;
		mKey.mv_data = (void*)key.data();
		mKey.mv_size = key.size();
		_errno = mdb_cursor_get(cursor, &mKey, &mData, MDB_NEXT);
		_db.update_errno(_errno);
		if (_errno != MDB_SUCCESS)
			return std::move(std::string());
		
		auto ret = std::string((char*)mData.mv_data, mData.mv_size);
		mdb_cursor_close(cursor);
		return std::move(ret);
	}

	std::string get(void* key, uint32_t klen)
	{
		MDB_cursor* cursor;
		int _errno = mdb_cursor_open(_txn, _dbi, &cursor);
		_db.update_errno(_errno);
		if (_errno != MDB_SUCCESS)
			return std::move(std::string());

		MDB_val mKey, mData;
		mKey.mv_data = key;
		mKey.mv_size = klen;

		_errno = mdb_cursor_get(cursor, &mKey, &mData, MDB_NEXT);
		_db.update_errno(_errno);
		if (_errno != MDB_SUCCESS)
			return std::move(std::string());

		auto ret = std::string((char*)mData.mv_data, mData.mv_size);
		mdb_cursor_close(cursor);
		return std::move(ret);
	}

	int get_range(const std::string& lower_key, const std::string& upper_key, LMDBQueryCallback cb)
	{
		MDB_cursor* cursor;
		int _errno = mdb_cursor_open(_txn, _dbi, &cursor);
		_db.update_errno(_errno);
		if (_errno != MDB_SUCCESS)
			return 0;

		MDB_val lKey, rKey, mData;
		lKey.mv_data = (void*)lower_key.data();
		lKey.mv_size = lower_key.size();

		rKey.mv_data = (void*)upper_key.data();
		rKey.mv_size = upper_key.size();
		
		if (_errno != MDB_SUCCESS)
			return 0;

		int cnt = 0;
		MDB_cursor_op op = MDB_SET_RANGE;
		std::vector<std::string> ayKeys, ayVals;
		for(; (_errno = mdb_cursor_get(cursor, &lKey, &mData, op))==MDB_SUCCESS;)
		{
			_db.update_errno(_errno);
			if(_errno == MDB_NOTFOUND)
				break;

			if(memcmp(lKey.mv_data, rKey.mv_data, lKey.mv_size) > 0)
				break;

			//回调
			//cb(std::string((char*)lKey.mv_data, lKey.mv_size), std::string((char*)mData.mv_data, mData.mv_size), false);
			ayKeys.emplace_back(std::string((char*)lKey.mv_data, lKey.mv_size));
			ayVals.emplace_back(std::string((char*)mData.mv_data, mData.mv_size));
			cnt++;
			op = MDB_NEXT;
		} 

		cb(ayKeys, ayVals);
		mdb_cursor_close(cursor);
		return cnt;
	}

	int get_lowers(const std::string& upper_key, int count, LMDBQueryCallback cb)
	{
		MDB_cursor* cursor;
		int _errno = mdb_cursor_open(_txn, _dbi, &cursor);
		_db.update_errno(_errno);
		if (_errno != MDB_SUCCESS)
			return 0;

		MDB_val rKey, mData;
		rKey.mv_data = (void*)upper_key.data();
		rKey.mv_size = upper_key.size();

		int cnt = 0;
		std::vector<std::string> ayKeys, ayVals;
		_errno = mdb_cursor_get(cursor, &rKey, &mData, MDB_SET_RANGE);
		_db.update_errno(_errno);
		for (; _errno != MDB_NOTFOUND;)
		{
			//往前查找，所以如果拿到的key，比右边界大，则直接往前退回一条
			if (memcmp(rKey.mv_data, upper_key.data(), upper_key.size()) > 0)
			{
				_errno = mdb_cursor_get(cursor, &rKey, &mData, MDB_PREV);
				continue;
			}

			//回调
			ayKeys.emplace_back(std::string((char*)rKey.mv_data, rKey.mv_size));
			ayVals.emplace_back(std::string((char*)mData.mv_data, mData.mv_size));
			cnt++;

			//如果找到目标数量，则退出
			if(cnt == count)
				break;
			
			_errno = mdb_cursor_get(cursor, &rKey, &mData, MDB_PREV);
			_db.update_errno(_errno);
		}

		//向前查找，是逆序的，需要做一个reverse
		std::reverse(ayKeys.begin(), ayKeys.end());
		std::reverse(ayVals.begin(), ayVals.end());
		cb(ayKeys, ayVals);
		mdb_cursor_close(cursor);
		return cnt;
	}

	inline int get_uppers(const std::string& lower_key, int count, LMDBQueryCallback cb)
	{
		MDB_cursor* cursor;
		int _errno = mdb_cursor_open(_txn, _dbi, &cursor);
		if (_errno != MDB_SUCCESS)
			return 0;

		MDB_val bKey, mData;
		bKey.mv_data = (void*)lower_key.data();
		bKey.mv_size = lower_key.size();

		int cnt = 0;
		std::vector<std::string> ayKeys, ayVals;
		_errno = mdb_cursor_get(cursor, &bKey, &mData, MDB_SET_RANGE);
		_db.update_errno(_errno);
		for (; _errno != MDB_NOTFOUND;)
		{
			//回调
			ayKeys.emplace_back(std::string((char*)bKey.mv_data, bKey.mv_size));
			ayVals.emplace_back(std::string((char*)mData.mv_data, mData.mv_size));
			cnt++;

			//如果找到目标数量，则退出
			if (cnt == count)
				break;

			_errno = mdb_cursor_get(cursor, &bKey, &mData, MDB_NEXT);
			_db.update_errno(_errno);
		}

		cb(ayKeys, ayVals);
		mdb_cursor_close(cursor);
		return cnt;
	}

private:
	WtLMDB&		_db;
	MDB_txn*	_txn;
	MDB_dbi		_dbi;
	bool		_readonly;
	bool		_commited;
};

NS_WTP_END
