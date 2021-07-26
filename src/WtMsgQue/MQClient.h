/*!
 * \file EventCaster.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief UDP广播对象定义
 */
#pragma once
#include "PorterDefs.h"
#include <queue>

#include "../Includes/WTSMarcos.h"
#include "../Includes/FasterDefs.h"
#include "../Share/StdUtils.hpp"

NS_OTP_BEGIN
class MQManager;

class MQClient
{
public:
	MQClient(MQManager* mgr);
	~MQClient();

private:
	void	extract_buffer();

	inline bool	is_allowed(const char* topic)
	{
		if (_topics.empty())
			return true;

		auto it = _topics.find(topic);
		if (it != _topics.end())
			return true;

		return false;
	}

public:
	inline uint32_t id() const { return _id; }

	bool	init(const char* url, FuncMQCallback cb);

	void	start();

	inline void	sub_topic(const char* topic)
	{
		_topics.insert(topic);
	}

private:
	std::string		m_strURL;
	bool			m_bReady;
	int				_sock;
	MQManager*		_mgr;
	uint32_t		_id;

	StdThreadPtr	m_thrdRecv;
	bool			m_bTerminated;

	std::string		_buffer;
	FuncMQCallback	_cb_message;

	faster_hashset<std::string> _topics;
};

NS_OTP_END