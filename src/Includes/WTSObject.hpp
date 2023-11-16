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

#include <boost/pool/object_pool.hpp>
#include <boost/thread.hpp>

#include "WTSMarcos.h"


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
	static boost::thread_specific_ptr<boost::object_pool<T>> pool_;

public:
	WTSPoolObject(){}
	virtual ~WTSPoolObject() {}

public:
	static T*	allocate()
	{
		if (!pool_.get())
			pool_.reset(new boost::object_pool<T>());
		return pool_->construct();
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
				pool_->destroy((T*)this);
		}
		catch (...)
		{

		}
	}
};

template<typename T>
boost::thread_specific_ptr<boost::object_pool<T> > WTSPoolObject<T>::pool_;
NS_WTP_END
