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
#include "WtHelper.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSCollection.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

USING_NS_WTP;

void on_mq_log(unsigned long id, const char* message, bool bServer)
{

}

EventNotifier::EventNotifier()
	: _mq_sid(0)
	, _publisher(NULL)
{
	
}


EventNotifier::~EventNotifier()
{
	if (_remover && _mq_sid != 0)
		_remover(_mq_sid);
}

bool EventNotifier::init(WTSVariant* cfg)
{
	if (!cfg->getBoolean("active"))
		return false;

	m_strURL = cfg->getCString("url");
	std::string module = DLLHelper::wrap_module("WtMsgQue", "lib");
	//�ȿ�����Ŀ¼���Ƿ��ж�Ӧģ��
	std::string dllpath = WtHelper::getCWD() + module;
	//���û��,���ٿ�ģ��Ŀ¼,��dllͬĿ¼��
	if (!StdFile::exists(dllpath.c_str()))
		dllpath = WtHelper::getInstDir() + module;

	DllHandle dllInst = DLLHelper::load_library(dllpath.c_str());
	if (dllInst == NULL)
	{
		WTSLogger::error("MQ module {} loading failed", dllpath.c_str());
		return false;
	}

	_creator = (FuncCreateMQServer)DLLHelper::get_symbol(dllInst, "create_server");
	if (_creator == NULL)
	{
		DLLHelper::free_library(dllInst);
		WTSLogger::error("MQ module {} is not compatible", dllpath.c_str());
		return false;
	}

	_remover = (FuncDestroyMQServer)DLLHelper::get_symbol(dllInst, "destroy_server");
	_publisher = (FundPublishMessage)DLLHelper::get_symbol(dllInst, "publish_message");
	_register = (FuncRegCallbacks)DLLHelper::get_symbol(dllInst, "regiter_callbacks");

	//ע��ص�����
	_register(on_mq_log);
	
	//����һ��MQServer
	_mq_sid = _creator(m_strURL.c_str());

	WTSLogger::info("EventNotifier initialized with channel {}", m_strURL.c_str());

	return true;
}

void EventNotifier::notifyLog(const char* tag, const char* message)
{
	std::string data;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("tag", rj::Value(tag, allocator), allocator);
		root.AddMember("time", TimeUtils::getLocalTimeNow(), allocator);
		root.AddMember("message", rj::Value(message, allocator), allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		data = sb.GetString();
	}

	if (_publisher)
		_publisher(_mq_sid, "LOG", data.c_str(), (unsigned long)data.size());
}

void EventNotifier::notifyEvent(const char* message)
{
	std::string data;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("time", TimeUtils::getLocalTimeNow(), allocator);
		root.AddMember("message", rj::Value(message, allocator), allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		data = sb.GetString();
	}
	if (_publisher)
		_publisher(_mq_sid, "GRP_EVENT", data.c_str(), (unsigned long)data.size());
}

void EventNotifier::notify(const char* trader, const char* message)
{
	std::string data;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("trader", rj::Value(trader, allocator), allocator);
		root.AddMember("time", TimeUtils::getLocalTimeNow(), allocator);
		root.AddMember("message", rj::Value(message, allocator), allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		data = sb.GetString();
	}
	if (_publisher)
		_publisher(_mq_sid, "TRD_NOTIFY", data.c_str(), (unsigned long)data.size());
}

void EventNotifier::notify(const char* trader, uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo)
{
	if (trdInfo == NULL || _mq_sid == 0)
		return;

	std::string data;
	tradeToJson(trader, localid, stdCode, trdInfo, data);
	if (_publisher)
		_publisher(_mq_sid, "TRD_TRADE", data.c_str(), (unsigned long)data.size());
}

void EventNotifier::notify(const char* trader, uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo)
{
	if (ordInfo == NULL || _mq_sid == 0)
		return;

	std::string data;
	orderToJson(trader, localid, stdCode, ordInfo, data);
	if (_publisher)
		_publisher(_mq_sid, "TRD_ORDER", data.c_str(), (unsigned long)data.size());
}

void EventNotifier::tradeToJson(const char* trader, uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo, std::string& output)
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

		root.AddMember("trader", rj::Value(trader, allocator), allocator);
		root.AddMember("time", TimeUtils::getLocalTimeNow(), allocator);
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

void EventNotifier::orderToJson(const char* trader, uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo, std::string& output)
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

		root.AddMember("trader", rj::Value(trader, allocator), allocator);
		root.AddMember("time", TimeUtils::getLocalTimeNow(), allocator);
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