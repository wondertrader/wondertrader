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

#include <queue>

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSObject.hpp"
#include "../Share/StdUtils.hpp"


NS_OTP_BEGIN
class WTSTradeInfo;
class WTSOrderInfo;
class WTSVariant;

class EventNotifier
{
public:
	EventNotifier();
	~EventNotifier();


private:
	void	notify(const char* trader, const std::string& data, uint32_t dataType);

	void	tradeToJson(uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo, std::string& output);
	void	orderToJson(uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo, std::string& output);

public:
	bool	init(WTSVariant* cfg);
	void	start();
	void	stop();


	void	notify(const char* trader, uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo);
	void	notify(const char* trader, uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo);

	void	notify(const char* trader, const std::string& message);

private:
	std::string		m_strGroupTag;
	std::string		m_strURL;
	bool			m_bReady;

	int		_sock;

	StdThreadPtr	m_thrdCast;
	StdCondVariable	m_condCast;
	StdUniqueMutex	m_mtxCast;
	bool			m_bTerminated;

	typedef struct _NotifyData
	{
		std::string	_trader;
		uint32_t	_datatype;
		std::string	_data;

		_NotifyData(const char* trader, const std::string& data, uint32_t dataType = 0)
			: _data(data), _datatype(dataType), _trader(trader)
		{
		}
	} NotifyData;

	std::queue<NotifyData>		m_dataQue;
};

NS_OTP_END