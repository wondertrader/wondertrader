/*!
 * \file EventCaster.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#include "MQClient.h"
#include "MQManager.h"

#include "../Share/StrUtil.hpp"

#include <spdlog/fmt/fmt.h>
#include <atomic>


#ifndef NN_STATIC_LIB
#define NN_STATIC_LIB
#endif
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>


USING_NS_OTP;

#pragma warning(disable:4200)

inline uint32_t makeMQCientId()
{
	static std::atomic<uint32_t> _auto_client_id{ 5001 };
	return _auto_client_id.fetch_add(1);
}


MQClient::MQClient(MQManager* mgr)
	: _sock(-1)
	, m_bReady(false)
	, _mgr(mgr)
	, m_bTerminated(false)
	, _cb_message(NULL)
{
	_id = makeMQCientId();
}

MQClient::~MQClient()
{
	if (!m_bReady)
		return;

	m_bTerminated = true;
	if (m_thrdRecv)
		m_thrdRecv->join();

	if (_sock != 0)
		nn_close(_sock);
}

bool MQClient::init(const char* url, FuncMQCallback cb)
{
	if (_sock >= 0)
		return true;

	_cb_message = cb;
	_sock = nn_socket(AF_SP, NN_SUB);
	if (_sock < 0)
	{
		_mgr->log_client(_id, fmt::format("MQClient {} has an error {} while initializing", _id, _sock).c_str());
		return false;
	}

	nn_setsockopt(_sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);

	m_strURL = url;
	if (nn_connect(_sock, url) < 0)
	{
		_mgr->log_client(_id, fmt::format("MQClient {} has an error while connecting url {}", _id, url).c_str());
		return false;
	}
	else
	{
		_mgr->log_client(_id, fmt::format("MQClient {} has connected to {} ", _id, url).c_str());
	}

	m_bReady = true;

	_mgr->log_client(_id, fmt::format("MQClient {} inited", _id).c_str());
	return true;
}

void MQClient::start()
{
	if (m_bTerminated)
		return;

	if(_sock < 0)
	{
		_mgr->log_client(_id, fmt::format("MQClient {} has not been initialized yet", _id).c_str());
		return;
	}

	if (m_thrdRecv == NULL)
	{
		m_thrdRecv.reset(new StdThread([this]() {

			while (!m_bTerminated)
			{
				static char buf[1024];
				bool hasData = false;
				for(;;)
				{
					int nBytes = nn_recv(_sock, buf, 1024, NN_DONTWAIT);
					if (nBytes > 0)
					{
						hasData = true;
						_buffer.append(buf, nBytes);
					}
					else
					{
						break;
					}
				}

				if (hasData)
					extract_buffer();
				else
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				
			}
		}));

		_mgr->log_client(_id, fmt::format("MQClient {} has started successfully", _id).c_str());
	}
	else
	{
		_mgr->log_client(_id, fmt::format("MQClient {} has already started", _id).c_str());
	}
	
}

void MQClient::extract_buffer()
{
	MQPacket* packet = (MQPacket*)_buffer.data();

	//先做长度检查
	if (_buffer.length() < sizeof(MQPacket))
		return;

	if (_buffer.length() < sizeof(MQPacket)+packet->_length)
		return;

	char* data = packet->_data;


	if(is_allowed(packet->_topic))
		_cb_message(_id, packet->_topic, packet->_data, packet->_length);
	_buffer.erase(0, sizeof(MQPacket) + packet->_length);
}