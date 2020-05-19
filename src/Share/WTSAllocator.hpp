#pragma once

#include "WTSMarcos.h"
#include <stdlib.h>
#include <vector>
#include <atomic>

NS_OTP_BEGIN
template<std::size_t ObjSize, std::size_t BlkCap>
class WTSMemCache
{
public:
	inline bool	enlarge()
	{
		bool bEnlarge = false;
		while (!std::atomic_compare_exchange_weak(&_enlarging, &bEnlarge, true));

		//如果有别人在扩大缓存了，当前线程就不要动了，等别人扩大晚了返回即可
		if(bEnlarge)
		{
			printf("some other thread is enlarging the cache, wait until its done...\r\n");
			while (_enlarging.load());
			return true;
		}

		_obj_size = ObjSize;
		_blk_size = ObjSize;
		_blk_size *= BlkCap;
		char* addr = (char*)::malloc(_blk_size);
		if (addr == NULL)
			throw std::exception("malloc memory failed");

		_cache_blocks.resize(_cache_blocks.size() + 1);	//缓存块增加一个
		CacheBlock& items = _cache_blocks[_cache_blocks.size()-1];
		items.resize(BlkCap);	//初始化新缓存块的容量

		//构建链表
		for (std::size_t i = 0; i < BlkCap; i++)
		{
			items[i]._data = (addr + i*ObjSize);
			if (i < BlkCap - 1)
				items[i]._next = &items[i + 1];
		}

		//内存地址保存
		_addrs.push_back(addr);
		
		//如果头部指针为空，有未初始化和已用完两种情况
		//都将新分配的给头指针
		if (_head.load() == NULL)
		{
			CacheItem* head = NULL;
			while (!std::atomic_compare_exchange_weak(&_head, &head, &items[0]));
		}
		
		//替换尾部指针，如果原尾部指针不为空，则指向新分配的首地址
		{
			CacheItem* tail = NULL;
			while (!std::atomic_compare_exchange_weak(&_tail, &tail, &items[BlkCap - 1]));

			if (tail)
				tail->_next = &items[0];
		}

		while (!std::atomic_compare_exchange_weak(&_enlarging, &bEnlarge, false));
		return true;
	}

	inline bool is_empty() const
	{
		return !_cache_blocks.empty();
	}

	inline bool is_valid() const
	{
		while (_enlarging.load());
		return _head.load() != NULL;
	}

	inline void* gain_one()
	{
		if (_head.load() == NULL)
			throw std::exception("run out of cache");

		CacheItem* head = NULL;
		while (!std::atomic_compare_exchange_weak(&_head, &head, (*_head)._next));

		head->_next = NULL;
		head->_used = true;

		CacheItem* tail = _tail.load();
		if (tail == head)
		{
			while (!std::atomic_compare_exchange_weak(&_tail, &tail, (CacheItem*)NULL));
		}

		return head->_data;
	}

	inline bool	free_one(void* ptr)
	{
		//先定位内存位置
		bool bHit = false;
		std::size_t offset = 0;
		std::size_t blk_idx = 0;
		
		auto cnt = _addrs.size();
		for (auto i = 0; i < cnt; i++)
		{
			char* addr = _addrs[i];
			offset = (char*)ptr - addr;
			if (offset >= 0 && offset < _blk_size)
			{
				bHit = true;
				break;
			}

			//如果不能被对象大小整除，说明释放有问题，返回false
			if (bHit && offset % _obj_size != 0)
				return false;

			blk_idx++;
		}	

		if (!bHit)
			return false;

		//根据地址偏移确定对象索引
		std::size_t idx = offset / _obj_size;
		//如果该项缓存是未使用的，则不用修改了
		CacheItem* item = &_cache_blocks[blk_idx][idx];
		if (!item->_used)
			return false;

		//释放的缓存要放到链表尾部，所以next为NULL
		//释放以后是未使用状态，所以标记也要改
		item->_next = NULL;
		item->_used = false;

		//先把链表尾部修改掉
		CacheItem* tail = NULL;
		while (!std::atomic_compare_exchange_weak(&_tail, &tail, item));
		if (tail)
			tail->_next = item;

		//如果头部为空，再给头部赋值
		if (_head.load() == NULL)
		{
			//_head = item;
			CacheItem* head = NULL;
			while (!std::atomic_compare_exchange_weak(&_head, &head, item));
		}

		_usage.fetch_sub(1);

		return true;
	}

	WTSMemCache() :_head(NULL), _tail(NULL), _usage(0), _max_usage(0), _enlarging(false){}
	~WTSMemCache()
	{
		for(char*& addr : _addrs)
			::free(addr);
	}

private:
	typedef struct _CacheItem
	{
		void*		_data;
		_CacheItem*	_next;
		bool		_used;

		_CacheItem() :_data(NULL), _next(NULL), _used(false){}
	} CacheItem;

	typedef std::vector<CacheItem> CacheBlock;
	std::vector<CacheBlock>	_cache_blocks;	//缓存块列表
	std::atomic<CacheItem*>	_head;		//头部指针
	std::atomic<CacheItem*>	_tail;		//尾部指针
	std::vector<char*>		_addrs;		//内存块列表
	std::size_t				_blk_size;	//块大小
	std::size_t				_obj_size;	//对象大小
	std::atomic<uint32_t>	_usage;		//已使用
	uint32_t				_max_usage;	//最大用量
	std::atomic<bool>		_enlarging;
};


namespace allocator
{
	template<std::size_t ObjSize, std::size_t ObjCap>
	static WTSMemCache<ObjSize, ObjCap>& getMemCache(bool bNeedInit = true)
	{
		static WTSMemCache<ObjSize, ObjCap> inst;
		if ((!inst.is_empty() || !inst.is_valid()) && bNeedInit)
		{
			inst.enlarge();
		}

		return inst;
	}

	template<class T>
	static T* allocate()
	{
		auto& cache = getMemCache<sizeof(T), 128>(true);
		T* ret = (T*)cache.gain_one();
		new(ret)T();
		return ret;
	}

	template<class T>
	static void deallocate(T* obj)
	{
		if (obj == NULL)
			return;

		obj->~T();
		auto& cache = getMemCache<sizeof(T), 128>(false);
		cache.free_one(obj);
	}

};
NS_OTP_END