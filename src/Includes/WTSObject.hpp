/*!
 * \file WTSObject.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt»ù´¡Object¶¨Òå
 */
#pragma once
#include <stdint.h>
#include <atomic>
#include "WTSMarcos.h"

NS_OTP_BEGIN
class WTSObject
{
public:
	WTSObject() :m_uRefs(1){}
	virtual ~WTSObject(){}

public:
	uint32_t		retain(){ return m_uRefs.fetch_add(1) + 1; }

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

	bool			isSingleRefs() { return m_uRefs == 1; }

	uint32_t		retainCount() { return m_uRefs; }

protected:
	volatile std::atomic<uint32_t>	m_uRefs;
};

NS_OTP_END
