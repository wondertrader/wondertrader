/*!
 * \file EventCaster.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "EventNotifier.h"

#include "../Share/StrUtil.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSCollection.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

USING_NS_OTP;

#pragma warning(disable:4200)


#define UDP_MSG_PUSHTRADE	0x300
#define UDP_MSG_PUSHORDER	0x301
#define UDP_MSG_PUSHEVENT	0x302

#pragma pack(push,1)
//UDP请求包
typedef struct _UDPPacket
{
	char			_group[16];
	char			_trader[16];
	uint32_t		_type;
	uint32_t		_length;
	char			_data[0];
} UDPPacket;
#pragma pack(pop)

EventNotifier::EventNotifier()
	: m_bTerminated(false)
	, m_bReady(false)
{
	
}


EventNotifier::~EventNotifier()
{
}

bool EventNotifier::init(WTSVariant* cfg)
{
	if (!cfg->getBoolean("active"))
		return false;

	m_strGroupTag = cfg->getCString("tag");

	WTSVariant* cfgBC = cfg->get("broadcast");
	if (cfgBC)
	{
		for (uint32_t idx = 0; idx < cfgBC->size(); idx++)
		{
			WTSVariant* cfgItem = cfgBC->get(idx);
			addBRecver(cfgItem->getCString("host"), cfgItem->getInt32("port"));
		}
	}

	WTSVariant* cfgMC = cfg->get("multicast");
	if (cfgMC)
	{
		for (uint32_t idx = 0; idx < cfgMC->size(); idx++)
		{
			WTSVariant* cfgItem = cfgMC->get(idx);
			addMRecver(cfgItem->getCString("host"), cfgItem->getInt32("port"), cfgItem->getInt32("sendport"));
		}
	}

	start();

	m_bReady = true;

	return true;
}

void EventNotifier::start()
{
	if (!m_listRawRecver.empty())
	{
		m_sktBroadcast.reset(new UDPSocket(m_ioservice, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)));
	}

	m_thrdIO.reset(new StdThread([this](){
		try
		{
			m_ioservice.run();
		}
		catch(...)
		{
			m_ioservice.stop();
		}
	}));

	WTSLogger::info("事件通知器已启动");
}

void EventNotifier::stop()
{
	if (!m_bReady)
		return;

	m_bTerminated = true;
	m_ioservice.stop();
	if (m_thrdIO)
		m_thrdIO->join();

	m_condCast.notify_all();
	if (m_thrdCast)
		m_thrdCast->join();
}

bool EventNotifier::addBRecver(const char* remote, int port)
{
	try
	{
		boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(remote);
		m_listRawRecver.emplace_back(EndPoint(addr, port));

		WTSLogger::info("接收端%s:%d已加入", remote, port);
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool EventNotifier::addMRecver(const char* remote, int port, int sendport)
{
	try
	{
		boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(remote);
		auto ep = EndPoint(addr, port);
		UDPSocketPtr sock(new UDPSocket(m_ioservice, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), sendport)));
		boost::asio::ip::multicast::join_group option(ep.address());
		sock->set_option(option);
		m_listRawGroup.emplace_back(std::make_pair(sock, ep));
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void EventNotifier::notify(const char* trader, uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo)
{
	if (trdInfo == NULL || !m_bReady)
		return;

	std::string data;
	tradeToJson(localid, stdCode, trdInfo, data);
	notify(trader, data, UDP_MSG_PUSHTRADE);
}

void EventNotifier::notify(const char* trader, uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo)
{
	if (ordInfo == NULL || !m_bReady)
		return;

	std::string data;
	orderToJson(localid, stdCode, ordInfo, data);
	notify(trader, data, UDP_MSG_PUSHORDER);
}

void EventNotifier::notify(const char* trader, const std::string& message)
{
	if (message.empty() || !m_bReady)
		return;

	notify(trader, message, UDP_MSG_PUSHEVENT);
}

void EventNotifier::tradeToJson(uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo, std::string& output)
{
	if(trdInfo == NULL)
	{
		output = "{}";
		return;
	}

	bool isLong = (trdInfo->getDirection() == WDT_LONG);
	bool isOpen = (trdInfo->getOffsetType() == WOT_OPEN);
	bool isToday = (trdInfo->getOffsetType() == WOT_CLOSETODAY);

	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("localid", localid, allocator);
		root.AddMember("code", rj::Value(stdCode, allocator), allocator);
		root.AddMember("islong", isLong, allocator);
		root.AddMember("isopen", isOpen, allocator);
		root.AddMember("istoday", isToday, allocator);

		root.AddMember("volume", trdInfo->getVolume(), allocator);
		root.AddMember("price", trdInfo->getPrice(), allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		output = sb.GetString();
	}
}

void EventNotifier::orderToJson(uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo, std::string& output)
{
	if (ordInfo == NULL)
	{
		output = "{}";
		return;
	}

	bool isLong = (ordInfo->getDirection() == WDT_LONG);
	bool isOpen = (ordInfo->getOffsetType() == WOT_OPEN);
	bool isToday = (ordInfo->getOffsetType() == WOT_CLOSETODAY);
	bool isCanceled = (ordInfo->getOrderState() == WOS_Canceled);

	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("localid", localid, allocator);
		root.AddMember("code", rj::Value(stdCode, allocator), allocator);
		root.AddMember("islong", isLong, allocator);
		root.AddMember("isopen", isOpen, allocator);
		root.AddMember("istoday", isToday, allocator);
		root.AddMember("canceled", isCanceled, allocator);

		root.AddMember("total", ordInfo->getVolume(), allocator);
		root.AddMember("left", ordInfo->getVolLeft(), allocator);
		root.AddMember("traded", ordInfo->getVolTraded(), allocator);
		root.AddMember("price", ordInfo->getPrice(), allocator);
		root.AddMember("state", rj::Value(ordInfo->getStateMsg(), allocator), allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		output = sb.GetString();
	}
}

void EventNotifier::notify(const char* trader, const std::string& data, uint32_t dataType)
{
	if(m_sktBroadcast == NULL || data.empty() || m_bTerminated)
		return;

	{
		StdUniqueLock lock(m_mtxCast);
		m_dataQue.push(NotifyData(trader, data, dataType));
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

				std::queue<NotifyData> tmpQue;
				{
					StdUniqueLock lock(m_mtxCast);
					tmpQue.swap(m_dataQue);
				}
				
				while(!tmpQue.empty())
				{
					const NotifyData& castData = tmpQue.front();

					if (castData._data.empty())
						break;

					//直接广播
					if (!m_listRawGroup.empty() || !m_listRawRecver.empty())
					{
						std::string buf_raw;
						buf_raw.resize(sizeof(UDPPacket) + castData._data.size());
						UDPPacket* pack = (UDPPacket*)buf_raw.data();
						pack->_length = castData._data.size();
						pack->_type = castData._datatype;
						strcpy(pack->_group, m_strGroupTag.c_str());
						strcpy(pack->_trader, castData._trader.c_str());
						memcpy(&pack->_data, castData._data.data(), castData._data.size());

						//广播
						for (auto it = m_listRawRecver.begin(); it != m_listRawRecver.end(); it++)
						{
							const EndPoint& receiver = (*it);
							m_sktBroadcast->send_to(boost::asio::buffer(buf_raw), receiver);
						}

						//组播
						for (auto it = m_listRawGroup.begin(); it != m_listRawGroup.end(); it++)
						{
							const MulticastPair& item = *it;
							it->first->send_to(boost::asio::buffer(buf_raw), item.second);
						}
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

void EventNotifier::handle_send_broad(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(error)
	{
		WTSLogger::error("事件广播失败，目标地址：%s，错误信息：%s", ep.address().to_string().c_str(), error.message().c_str());
	}
}

void EventNotifier::handle_send_multi(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(error)
	{
		WTSLogger::error("事件多播失败，目标地址：%s，错误信息：%s", ep.address().to_string().c_str(), error.message().c_str());
	}
}

;