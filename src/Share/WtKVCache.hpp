#pragma once
#include "SpinMutex.hpp"
#include "BoostFile.hpp"
#include "BoostMappingFile.hpp"
#include "../Includes/FasterDefs.h"

#define SIZE_STEP 200
#define CACHE_FLAG "&^%$#@!\0"
#define FLAG_SIZE 8

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

#pragma warning(disable:4200)

NS_WTP_BEGIN

typedef std::function<void(const char*)> CacheLogger;

class WtKVCache
{
public:
	WtKVCache() {}
	WtKVCache(const WtKVCache&) = delete;
	WtKVCache& operator=(const WtKVCache&) = delete;

private:
	

	typedef struct _CacheItem
	{
		char	_key[64] = { 0 };
		char	_val[64] = { 0 };
	} CacheItem;

	typedef struct CacheBlock
	{
		char		_blk_flag[FLAG_SIZE];
		uint32_t	_size;
		uint32_t	_capacity;
		uint32_t	_date;
		CacheItem	_items[0];
	} CacheBlock;

	typedef struct _CacheBlockPair
	{
		CacheBlock*		_block;
		BoostMFPtr		_file;

		_CacheBlockPair()
		{
			_block = NULL;
			_file = NULL;
		}
	} CacheBlockPair;

	CacheBlockPair	_cache;
	SpinMutex		_lock;
	wt_hashmap<std::string, uint32_t> _indice;

private:
	bool	resize(uint32_t newCap, CacheLogger logger = nullptr)
	{
		if (_cache._file == NULL)
			return false;

		//调用该函数之前,应该保证线程安全了
		CacheBlock* cBlock = _cache._block;
		if (cBlock->_capacity >= newCap)
			return _cache._file->addr();

		std::string filename = _cache._file->filename();
		uint64_t uOldSize = sizeof(CacheBlock) + sizeof(CacheItem)*cBlock->_capacity;
		uint64_t uNewSize = sizeof(CacheBlock) + sizeof(CacheItem)*newCap;
		std::string data;
		data.resize((std::size_t)(uNewSize - uOldSize), 0);
		try
		{
			BoostFile f;
			f.open_existing_file(filename.c_str());
			f.seek_to_end();
			f.write_file(data.c_str(), data.size());
			f.close_file();
		}
		catch (std::exception&)
		{
			if (logger) logger("Got an exception while resizing cache file");
			return false;
		}


		_cache._file.reset();
		BoostMappingFile* pNewMf = new BoostMappingFile();
		try
		{
			if (!pNewMf->map(filename.c_str()))
			{
				delete pNewMf;
				if (logger) logger("Mapping cache file failed");
				return false;
			}
		}
		catch (std::exception&)
		{
			if (logger) logger("Got an exception while mapping cache file");
			return false;
		}

		_cache._file.reset(pNewMf);

		_cache._block = (CacheBlock*)_cache._file->addr();
		_cache._block->_capacity = newCap;
		return true;
	}

public:
	bool	init(const char* filename, uint32_t uDate, CacheLogger logger = nullptr)
	{
		bool isNew = false;
		if (!BoostFile::exists(filename))
		{
			uint64_t uSize = sizeof(CacheBlock) + sizeof(CacheItem) * SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename);
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		_cache._file.reset(new BoostMappingFile);
		if (!_cache._file->map(filename))
		{
			_cache._file.reset();
			if (logger) logger("Mapping cache file failed");
			return false;
		}
		_cache._block = (CacheBlock*)_cache._file->addr();

		if (!isNew &&  _cache._block->_date != uDate)
		{
			 _cache._block->_size = 0;
			 _cache._block->_date = uDate;

			memset(& _cache._block->_items, 0, sizeof(CacheItem)* _cache._block->_capacity);

			if (logger) logger("Cache file reset due to a different date");
		}

		if (isNew)
		{
			 _cache._block->_capacity = SIZE_STEP;
			 _cache._block->_size = 0;
			 _cache._block->_date = uDate;
			strcpy( _cache._block->_blk_flag, CACHE_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题,要自动恢复
			do
			{
				uint64_t uSize = sizeof(CacheBlock) + sizeof(CacheItem) *  _cache._block->_capacity;
				uint64_t realSz =  _cache._file->size();
				if (realSz != uSize)
				{
					uint32_t realCap = (uint32_t)((realSz - sizeof(CacheBlock)) / sizeof(CacheItem));
					uint32_t markedCap =  _cache._block->_capacity;
					//文件大小不匹配,一般是因为capacity改了,但是实际没扩容
					//这是做一次扩容即可
					 _cache._block->_capacity = realCap;
					 _cache._block->_size = (realCap < markedCap) ? realCap : markedCap;
				}

			} while (false);
		}

		//这里把索引加到hashmap中
		for (uint32_t i = 0; i < _cache._block->_size; i++)
			_indice[_cache._block->_items[i]._key] = i;

		return true;
	}

	inline void clear()
	{
		_lock.lock();

		if (_cache._block == NULL)
			return;

		_indice.clear();

		memset(_cache._block->_items, 0, sizeof(CacheItem)*_cache._block->_capacity);
		_cache._block->_size = 0;

		_lock.unlock();
	}

	inline const char*	get(const char* key) const
	{
		auto it = _indice.find(key);
		if (it == _indice.end())
			return "";

		return _cache._block->_items[it->second]._val;
	}

	void	put(const char* key, const char*val, std::size_t len = 0, CacheLogger logger = nullptr)
	{
		auto it = _indice.find(key);
		if (it != _indice.end())
		{
			wt_strcpy(_cache._block->_items[it->second]._val, val, len);
		}
		else
		{
			_lock.lock();
			if(_cache._block->_size == _cache._block->_capacity)
				resize(_cache._block->_capacity*2, logger);

			_indice[key] = _cache._block->_size;
			wt_strcpy(_cache._block->_items[_cache._block->_size]._key, key);
			wt_strcpy(_cache._block->_items[_cache._block->_size]._val, val, len);
			_cache._block->_size += 1;
			_lock.unlock();
		}
	}

	inline bool	has(const char* key) const 
	{
		return (_indice.find(key) != _indice.end());
	}

	inline uint32_t size() const
	{
		if (_cache._block == 0)
			return 0;

		return _cache._block->_size;
	}

	inline uint32_t capacity() const
	{
		if (_cache._block == 0)
			return 0;

		return _cache._block->_capacity;
	}
};

NS_WTP_END