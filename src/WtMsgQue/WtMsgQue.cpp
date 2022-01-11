/*!
 * \file WtMsgQue.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtMsgQue.h"
#include "MQManager.h"

#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "nanomsg.lib")
#endif

USING_NS_WTP;

MQManager& getMgr()
{
	static MQManager runner;
	return runner;
}

void regiter_callbacks(FuncLogCallback cbLog)
{
	getMgr().regiter_callbacks(cbLog);
}

WtUInt32 create_server(const char* url, bool confirm)
{
	printf("create server\r\n");
	return getMgr().create_server(url, confirm);
}

void destroy_server(WtUInt32 id)
{
	getMgr().destroy_server(id);
}

void publish_message(WtUInt32 id, const char* topic, const char* data, WtUInt32 dataLen)
{
	getMgr().publish_message(id, topic, data, dataLen);
}

WtUInt32 create_client(const char* url, FuncMQCallback cb)
{
	return getMgr().create_client(url, cb);
}

void destroy_client(WtUInt32 id)
{
	getMgr().destroy_client(id);
}

void subscribe_topic(WtUInt32 id, const char* topic)
{
	return getMgr().sub_topic(id, topic);
}

void start_client(WtUInt32 id)
{
	getMgr().start_client(id);
}
