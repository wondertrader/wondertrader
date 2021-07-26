/*!
 * \file EventCaster.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "MQServer.h"
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


inline uint32_t makeMQSvrId()
{
	static std::atomic<uint32_t> _auto_server_id{ 1001 };
	return _auto_server_id.fetch_add(1);
}


MQServer::MQServer(MQManager* mgr)
	: _sock(-1)
	, m_bReady(false)
	, _mgr(mgr)
	, m_bTerminated(false)
{
	_id = makeMQSvrId();
}

MQServer::~MQServer()
{
	if (!m_bReady)
		return;

	m_bTerminated = true;
	m_condCast.notify_all();
	if (m_thrdCast)
		m_thrdCast->join();

	if (_sock >= 0)
		nn_close(_sock);
}

bool MQServer::init(const char* url)
{
	if (_sock >= 0)
		return true;

	_sock = nn_socket(AF_SP, NN_PUB);
	if(_sock < 0)
	{
		_mgr->log_server(_id, fmt::format("MQServer {} has an error {} while initializing", _id, _sock).c_str());
		return false;
	}

	m_strURL = url;
	if(nn_bind(_sock, url) < 0)
	{
		_mgr->log_server(_id, fmt::format("MQServer {} has an error while binding url {}", _id, url).c_str());
		return false;
	}
	else
	{
		_mgr->log_server(_id, fmt::format("MQServer {} has binded to {} ", _id, url).c_str());
	}

	m_bReady = true;

	_mgr->log_server(_id, fmt::format("MQServer {} ready", _id).c_str());
	return true;
}

void MQServer::publish(const char* topic, const void* data, uint32_t dataLen)
{
	if(_sock < 0)
	{
		_mgr->log_server(_id, fmt::format("MQServer {} has not been initialized yet", _id).c_str());
		return;
	}

	if(data == NULL || dataLen == 0 || m_bTerminated)
		return;

	{
		StdUniqueLock lock(m_mtxCast);
		m_dataQue.push(PubData(topic, data, dataLen));
	}

	if(m_thrdCast == NULL)
	{
		m_thrdCast.reset(new StdThread([this](){

			while (!m_bTerminated)
			{
				if(m_dataQue.empty())
				{
					StdUniqueLock lock(m_mtxCast);
					m_condCast.wait(lock);
					continue;
				}	

				PubDataQue tmpQue;
				{
					StdUniqueLock lock(m_mtxCast);
					tmpQue.swap(m_dataQue);
				}
				
				while(!tmpQue.empty())
				{
					const PubData& pubData = tmpQue.front();

					if (!pubData._data.empty())
					{
						std::string buf_raw;
						buf_raw.resize(sizeof(MQPacket) + pubData._data.size());
						MQPacket* pack = (MQPacket*)buf_raw.data();
						strncpy(pack->_topic, pubData._topic.c_str(), 32);
						pack->_length = pubData._data.size();
						memcpy(&pack->_data, pubData._data.data(), pubData._data.size());
						nn_send(_sock, buf_raw.data(), buf_raw.size(), 0);
					}
					tmpQue.pop();
				} 
			}
		}));
	}
	else
	{
		m_condCast.notify_all();
	}
}