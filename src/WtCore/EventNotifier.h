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

#include <boost/asio/io_service.hpp>

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSObject.hpp"
#include "../Share/StdUtils.hpp"

typedef unsigned long(*FuncCreateMQServer)(const char*);
typedef void(*FuncDestroyMQServer)(unsigned long);
typedef void(*FundPublishMessage)(unsigned long, const char*, const char*, unsigned long);

typedef void(*FuncLogCallback)(unsigned long, const char*, bool);
typedef void(*FuncRegCallbacks)(FuncLogCallback);


NS_WTP_BEGIN
class WTSTradeInfo;
class WTSOrderInfo;
class WTSVariant;

class EventNotifier
{
public:
	EventNotifier();
	~EventNotifier();


private:
	void	tradeToJson(const char* trader, uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo, std::string& output);
	void	orderToJson(const char* trader, uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo, std::string& output);

public:
	bool	init(WTSVariant* cfg);

	void	notify(const char* trader, uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo);
	void	notify(const char* trader, uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo);
	void	notify(const char* trader, const char* message);

	void	notify_log(const char* tag, const char* message);

	void	notify_event(const char* message);

	void	notify_chart_marker(uint64_t time, const char* straId, double price, const char* icon, const char* tag);
	void	notify_chart_index(uint64_t time, const char* straId, const char* idxName, const char* lineName, double val);
	void	notify_trade(const char* straId, const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, const char* userTag);

private:
	std::string		_url;
	uint32_t		_mq_sid;
	FuncCreateMQServer	_creator;
	FuncDestroyMQServer	_remover;
	FundPublishMessage	_publisher;
	FuncRegCallbacks	_register;

	bool			_stopped;
	boost::asio::io_service		_asyncio;
	StdThreadPtr				_worker;
};

NS_WTP_END