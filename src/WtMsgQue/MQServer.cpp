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
#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"

#include <atomic>


#ifndef NN_STATIC_LIB
#define NN_STATIC_LIB
#endif
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>


USING_NS_WTP;

constexpr static uint32_t PACKET_BUFFER_SIZE = 1 * 1024 * 1024;


inline uint32_t makeMQSvrId()
{
	static std::atomic<uint32_t> _auto_server_id{ 1001 };
	return _auto_server_id.fetch_add(1);
}


MQServer::MQServer(MQManager* mgr)
	: _sock(-1)
	, _ready(false)
	, _mgr(mgr)
	, _confirm(false)
	, m_bTerminated(false)
	, m_bTimeout(false)
{
	_id = makeMQSvrId();
}

MQServer::~MQServer()
{
	if (!_ready)
		return;

	m_bTerminated = true;
	if (m_thrdCast)
		m_thrdCast->join();

	if (m_sendBuf)
		delete[] m_sendBuf;

	if (_sock >= 0)
		nn_close(_sock);
}

bool MQServer::init(const char* url, bool confirm /* = false */)
{
	if (_sock >= 0)
		return true;

	_confirm = confirm;

	_sock = nn_socket(AF_SP, NN_PUB);
	if(_sock < 0)
	{
		_mgr->log_server(_id, fmtutil::format("MQServer {} initializing failed: {}", _id, nn_strerror(nn_errno())));
		_sock = -1;
		return false;
	}

	int bufsize = 8 * 1024 * 1024;
	if(nn_setsockopt(_sock, NN_SOL_SOCKET, NN_SNDBUF, &bufsize, sizeof(bufsize)) < 0)
	{
		_mgr->log_server(_id, fmtutil::format("MQServer {} setsockopt failed: {}", _id, nn_strerror(nn_errno())));
		nn_close(_sock);
		_sock = -1;
		return false;
	}

	_url = url;
	int ec = nn_bind(_sock, url);
	if(ec < 0)
	{
		_mgr->log_server(_id, fmtutil::format("MQServer {} binding url {} failed: {}", _id, url, nn_strerror(nn_errno())));
		nn_close(_sock);
		_sock = -1;
		return false;
	}
	else
	{
		_mgr->log_server(_id, fmtutil::format("MQServer {} has binded to {} ", _id, url));
	}

	_ready = true;

	return true;
}

void MQServer::publish(const char* topic, const void* data, uint32_t dataLen)
{
	if(_sock < 0)
	{
		_mgr->log_server(_id, fmtutil::format("MQServer {} has not been initialized yet", _id));
		return;
	}

	if(data == NULL || dataLen == 0 || m_bTerminated)
		return;

	{
		SpinLock lock(m_mtxCast);
		m_dataQue.emplace_back(PubData(topic, data, dataLen));
		m_uLastHBTime = TimeUtils::getLocalTimeNow();
		m_uTotalPacks.fetch_add(1);
	}

	if(m_thrdCast == NULL)
	{
		m_thrdCast.reset(new StdThread([this](){

			if(!m_sendBuf)
				m_sendBuf = new char[PACKET_BUFFER_SIZE];

			m_uLastHBTime = TimeUtils::getLocalTimeNow();
			while (!m_bTerminated)
			{
				int cnt = (int)nn_get_statistic(_sock, NN_STAT_CURRENT_CONNECTIONS);
				if(m_dataQue.empty() || (cnt == 0 && _confirm))
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(2));

					m_bTimeout = true;
					uint64_t now = TimeUtils::getLocalTimeNow();
					//如果有连接，并且超过60s没有新的数据推送，就推送一条心跳包
					if (now - m_uLastHBTime > 60*1000 && cnt>0)
					{
						//等待超时以后，广播心跳包
						SpinLock lock(m_mtxCast);
						m_dataQue.emplace_back(PubData("HEARTBEAT", "", 0));
						m_uTotalPacks.fetch_add(1);
						m_uLastHBTime = now;
						_mgr->log_server(_id, fmtutil::format("HeartBeat timestamp updated to {}", m_uLastHBTime));
					}
					else
					{
						continue;
					}
				}	

				PubDataQue tmpQue;
				{
					SpinLock lock(m_mtxCast);
					tmpQue.swap(m_dataQue);
				}
				
				std::size_t total_len = 0;
				for (const PubData& pubData : tmpQue)
				{
					std::size_t len = sizeof(MQPacket) + pubData._data.size();

					//如果数据包缓存满了，则先发送一次
					if (total_len + len > PACKET_BUFFER_SIZE)
					{
						_mgr->log_server(_id, fmtutil::format("Packet buffer is about to be full ({} - > {}), force to send", total_len, total_len + len));
						int bytes_snd = 0;
						for (;;)
						{
							int bytes = nn_send(_sock, m_sendBuf + bytes_snd, total_len - bytes_snd, 0);
							if (bytes >= 0)
							{
								bytes_snd += bytes;
							}
							else
							{
								_mgr->log_server(_id, fmtutil::format("Publishing error: {}", nn_strerror(nn_errno())));
							}

							if (bytes_snd == total_len)
								break;
						}

						//发完以后把长度置为0
						total_len = 0;
					}

					MQPacket* pack = (MQPacket*)(m_sendBuf + total_len);
					memset(pack, 0, len);
					strncpy(pack->_topic, pubData._topic.c_str(), 32);
					pack->_length = (uint32_t)pubData._data.size();
					if(!pubData._data.empty())
						memcpy(&pack->_data, pubData._data.data(), pubData._data.size());

					total_len += len;
				}

				if(total_len > 0)
				{
					int bytes_snd = 0;
					for (;;)
					{
						int bytes = nn_send(_sock, m_sendBuf + bytes_snd, total_len - bytes_snd, 0);
						if (bytes >= 0)
						{
							bytes_snd += bytes;
						}
						else
						{
							_mgr->log_server(_id, fmtutil::format("Publishing error: {}", nn_strerror(nn_errno())));
						}

						if (bytes_snd == total_len)
							break;
					}
				}

				m_uTotalSents.fetch_add(tmpQue.size());			

				if(m_dataQue.empty())
				{
					if(m_uTotalSents != m_uTotalPacks)
					{
						_mgr->log_server(_id, fmtutil::format("Total sent packs {} != total packs {}, force to sync", m_uTotalSents, m_uTotalPacks));
						m_uTotalSents = m_uTotalPacks.fetch_add(0);
					}
					else if(m_uTotalSents % 100 == 0)
					{
						_mgr->log_server(_id, fmtutil::format("{} packets published", m_uTotalSents));
					}
				}

				if (tmpQue.size() > m_maxMultiPacks)
				{
					m_maxMultiPacks = tmpQue.size();
					_mgr->log_server(_id, fmtutil::format("Max Multi packs updated to {}", m_maxMultiPacks));
				}
			}
		}));
	}
}