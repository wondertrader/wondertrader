/*!
 * \file WTSObject.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt基础Object定义
 */
#pragma once
#include <stdint.h>
#include <atomic>
#include <boost/smart_ptr/detail/spinlock.hpp>

#include "WTSMarcos.h"
#include "../Share/ObjectPool.hpp"
#include "../Share/SpinMutex.hpp"

NS_WTP_BEGIN
class WTSObject
{
public:
	WTSObject() :m_uRefs(1){}
	virtual ~WTSObject(){}

public:
	inline uint32_t		retain(){ return m_uRefs.fetch_add(1) + 1; }

	virtual void	release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			uint32_t cnt = m_uRefs.fetch_sub(1);
			if (cnt == 1)
			{
				delete this;
			}
		}
		catch(...)
		{

		}
	}

	inline bool			isSingleRefs() { return m_uRefs == 1; }

	inline uint32_t		retainCount() { return m_uRefs; }

protected:
	volatile std::atomic<uint32_t>	m_uRefs;
};

template<typename T>
class WTSPoolObject : public WTSObject
{
private:
	typedef ObjectPool<T> MyPool;
	MyPool*			_pool;
	SpinMutex*	_mutex;

public:
	WTSPoolObject():_pool(NULL){}
	virtual ~WTSPoolObject() {}

public:
	static T*	allocate()
	{
		/*
		 *	By Wesley @ 2022.06.14
		 *	有用户反馈，这里使用了thread_local，线程销毁的话，内存池也销毁了
		 *	该用户在Trader里复现了这个bug，如果Trader底层销毁了一个API对象实例
		 *	那么这里内存池就已经析构了，如果有在系统中存储（retain）Trader创建的对象（WTSOrderInfo等），则会出现访问越界的问题
		 *	这里如果去掉thread_local，改成纯静态的，可能多线程并发的场景下也会有一些问题
		 *	总之如果要彻底安全，那么可能需要加一把锁才行，但是这样会带来性能开销
		 *	所以注释一下，如果有问题的可以参考一下
		 */
		thread_local static MyPool		pool;
		thread_local static SpinMutex	mtx;

		mtx.lock();
		T* ret = pool.construct();
		mtx.unlock();
		ret->_pool = &pool;
		ret->_mutex = &mtx;
		return ret;
	}

public:
	virtual void release() override
	{
		if (m_uRefs == 0)
			return;

		try
		{
			uint32_t cnt = m_uRefs.fetch_sub(1);
			if (cnt == 1)
			{
				_mutex->lock();
				_pool->destroy((T*)this);
				_mutex->unlock();
			}
		}
		catch (...)
		{

		}
	}
};
NS_WTP_END
