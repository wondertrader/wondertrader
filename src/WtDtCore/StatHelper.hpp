/*!
 * \file StatHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据统计辅助类
 */
#pragma once
#include <boost/atomic.hpp>
#include <boost/interprocess/detail/atomic.hpp>

class StatHelper
{
public:
	static StatHelper& one()
	{
		static StatHelper only;
		return only;
	}

public:
	typedef struct _StatInfo
	{
		uint32_t	_recv_packs;
		uint32_t	_send_packs;
		uint64_t	_send_bytes;

		_StatInfo()
		{
			_recv_packs = 0;
			_send_bytes = 0;
			_send_packs = 0;
		}
	} StatInfo;

	typedef enum
	{
		ST_BROADCAST
	} StatType;

	typedef enum 
	{
		UF_Recv		= 0x0001,
		UF_Send		= 0x0002
	} UpdateFlag;

public:
	void updateStatInfo(StatType sType, uint32_t recvPacks, uint32_t sendPacks, uint64_t sendBytes)
	{
		BoostWriteLock lock(_mutexes[sType]);
		StatInfo& sInfo = _stats[sType];
		sInfo._recv_packs += recvPacks;
		sInfo._send_packs += sendPacks;
		if(UINT64_MAX - sInfo._send_bytes < sendBytes)
			sInfo._send_bytes = sendBytes;
		else
			sInfo._send_bytes += sendBytes;
		
		uint32_t flag = 0;
		if (recvPacks > 0)
			flag |= UF_Recv;
		if (sendPacks > 0)
			flag |= UF_Send;
	}
	
	StatInfo getStatInfo(StatType sType)
	{
		BoostReadLock lock(_mutexes[sType]);
		return _stats[sType];
	}

private:
	StatInfo		_stats[5];
	BoostRWMutex	_mutexes[5];
};

