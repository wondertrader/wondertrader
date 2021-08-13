#pragma once
#include "PorterDefs.h"
#include "MQServer.h"
#include "MQClient.h"

#include "../Includes/FasterDefs.h"
#include "../Share/StdUtils.hpp"

NS_OTP_BEGIN

#pragma warning(disable:4200)

#pragma pack(push,1)
//UDPÇëÇó°ü
typedef struct _MQPacket
{
	char			_topic[32];
	uint32_t		_length;
	char			_data[0];
} MQPacket;
#pragma pack(pop)

typedef std::shared_ptr<MQServer> MQServerPtr;
typedef std::shared_ptr<MQClient> MQClientPtr;

class MQManager
{
public:
	MQManager() : _cb_log(NULL) {}

public:
	inline void		regiter_callbacks(FuncLogCallback cbLog) { _cb_log = cbLog; }

	WtUInt32	create_server(const char* url, bool confirm);
	void		destroy_server(WtUInt32 id);
	void		publish_message(WtUInt32 id, const char* topic, const void* data, WtUInt32 dataLen);

	WtUInt32	create_client(const char* url, FuncMQCallback cb);
	void		destroy_client(WtUInt32 id);
	void		sub_topic(WtUInt32 id, const char* topic);
	void		start_client(WtUInt32 id);

	void		log_server(WtUInt32 id, const char* message);
	void		log_client(WtUInt32 id, const char* message);

private:
	typedef faster_hashmap<uint32_t, MQServerPtr> ServerMap;
	ServerMap	_servers;

	typedef faster_hashmap<uint32_t, MQClientPtr> ClientMap;
	ClientMap	_clients;

	FuncLogCallback	_cb_log;
};

NS_OTP_END
