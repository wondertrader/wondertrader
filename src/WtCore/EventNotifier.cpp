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

#define NN_STATIC_LIB
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#ifdef _WIN32
#pragma comment(lib, "nanomsg.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Mswsock.lib")
#endif

USING_NS_OTP;

#pragma warning(disable:4200)


#define UDP_MSG_PUSHTRADE	0x300
#define UDP_MSG_PUSHORDER	0x301
#define UDP_MSG_PUSHEVENT	0x302

#pragma pack(push,1)
//UDPÇëÇó°ü
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
	m_strURL = cfg->getCString("url");

	start();

	return true;
}

void EventNotifier::start()
{
	_sock = nn_socket(AF_SP, NN_PUB);
	if(_sock < 0)
	{
		WTSLogger::info("Event notifier has an error %d while initializing", _sock);
		return;
	}

	if(nn_bind(_sock, m_strURL.c_str()) < 0)
	{
		WTSLogger::info("Event notifier has an error while binding url");
		return;
	}

	m_bReady = true;

	WTSLogger::info("Event notifier started");
}

void EventNotifier::stop()
{
	if (!m_bReady)
		return;

	m_bTerminated = true;
	m_condCast.notify_all();
	if (m_thrdCast)
		m_thrdCast->join();
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
	if(data.empty() || m_bTerminated)
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

					std::string buf_raw;
					buf_raw.resize(sizeof(UDPPacket) + castData._data.size());
					UDPPacket* pack = (UDPPacket*)buf_raw.data();
					pack->_length = castData._data.size();
					pack->_type = castData._datatype;
					strcpy(pack->_group, m_strGroupTag.c_str());
					strcpy(pack->_trader, castData._trader.c_str());
					memcpy(&pack->_data, castData._data.data(), castData._data.size());
					nn_send(_sock, buf_raw.data(), buf_raw.size(), 0);
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