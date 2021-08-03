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

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Includes/WTSVariant.hpp"
#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;


USING_NS_OTP;

void on_mq_log(unsigned long id, const char* message, bool bServer)
{

}

EventNotifier::EventNotifier()
	: _mq_sid(0)
{
	
}


EventNotifier::~EventNotifier()
{
	printf("removing notifier\r\n");
	if (_remover && _mq_sid != 0)
		_remover(_mq_sid);
}

bool EventNotifier::init(WTSVariant* cfg)
{
	if (!cfg->getBoolean("active"))
		return false;

	m_strURL = cfg->getCString("url");
	std::string module = DLLHelper::wrap_module("WtMsgQue", "lib");
	//先看工作目录下是否有对应模块
	std::string dllpath = WtHelper::getCWD() + module;
	//如果没有,则再看模块目录,即dll同目录下
	if (!StdFile::exists(dllpath.c_str()))
		dllpath = WtHelper::getInstDir() + module;

	DllHandle dllInst = DLLHelper::load_library(dllpath.c_str());
	if (dllInst == NULL)
	{
		WTSLogger::error("MQ module %s loading failed", dllpath.c_str());
		return false;
	}

	_creator = (FuncCreateMQServer)DLLHelper::get_symbol(dllInst, "create_server");
	if (_creator == NULL)
	{
		DLLHelper::free_library(dllInst);
		WTSLogger::error("MQ module %s is not compatible", dllpath.c_str());
		return false;
	}

	_remover = (FuncDestroyMQServer)DLLHelper::get_symbol(dllInst, "destroy_server");
	_publisher = (FundPublishMessage)DLLHelper::get_symbol(dllInst, "publish_message");
	_register = (FuncRegCallbacks)DLLHelper::get_symbol(dllInst, "regiter_callbacks");

	//注册回调函数
	_register(on_mq_log);
	
	//创建一个MQServer
	_mq_sid = _creator(m_strURL.c_str(), true);

	WTSLogger::info("EventNotifier initialized with channel %s", m_strURL.c_str());

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
		_publisher(_mq_sid, "BT_LOG", data.c_str(), data.size());
}

void EventNotifier::notifyEvent(const char* evtType)
{
	if (_publisher)
		_publisher(_mq_sid, "BT_EVENT", evtType, strlen(evtType));
}

void EventNotifier::notifyData(const char* topic, void* data , uint32_t dataLen)
{
	if (_publisher)
		_publisher(_mq_sid, topic, (const char*)data, dataLen);
}

void EventNotifier::notifySignal(const char* topic, const char* stdCode, double price, double volume, const char* usertag, uint64_t barTime)
{
	std::string output;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("code", rj::Value(stdCode, allocator), allocator);
		root.AddMember("tag", rj::Value(usertag, allocator), allocator);
		root.AddMember("volume", volume, allocator);
		root.AddMember("price", price, allocator);
		root.AddMember("bartime", barTime, allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		output = sb.GetString();
	}

	if (_publisher)
		_publisher(_mq_sid, topic, (const char*)output.c_str(), output.size());
}

void EventNotifier::notifyFund(const char* topic, uint32_t uDate, double total_profit, double dynprofit, double dynbalance, double total_fee)
{
	std::string output;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("date", uDate, allocator);
		root.AddMember("total_profit", total_profit, allocator);
		root.AddMember("dynprofit", dynprofit, allocator);
		root.AddMember("dynbalance", dynbalance, allocator);
		root.AddMember("total_fee", total_fee, allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		output = sb.GetString();
	}

	if (_publisher)
		_publisher(_mq_sid, topic, (const char*)output.c_str(), output.size());
}

void EventNotifier::notifyClose(const char* topic, const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx,
	double qty, double profit, double maxprofit, double maxloss, double totalprofit /* = 0 */, const char* enterTag /* = "" */, const char* exitTag /* = "" */)
{
	std::string output;
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		root.AddMember("code", rj::Value(stdCode, allocator), allocator);
		root.AddMember("long", isLong, allocator);
		root.AddMember("open_time", openTime, allocator);
		root.AddMember("open_price", openpx, allocator);
		root.AddMember("close_time", closeTime, allocator);
		root.AddMember("close_price", closepx, allocator);
		root.AddMember("volume", qty, allocator);
		root.AddMember("profit", profit, allocator);
		root.AddMember("max_profit", maxprofit, allocator);
		root.AddMember("max_loss", maxloss, allocator);
		root.AddMember("total_profit", totalprofit, allocator);
		root.AddMember("enter_tag", rj::Value(enterTag, allocator), allocator);
		root.AddMember("exit_tag", rj::Value(exitTag, allocator), allocator);

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);

		output = sb.GetString();
	}

	if (_publisher)
		_publisher(_mq_sid, topic, (const char*)output.c_str(), output.size());
}