#include "../WtMsgQue/WtMsgQue.h"
#include <string.h>
#include <stdio.h>
#include <chrono>
#include <thread>

void on_log(WtUInt32 id, const char* message, bool bServer)
{
	if(bServer)
		printf("Server %u: ", id);
	else
		printf("Client %u: ", id);
	printf(message);
	printf("\r\n");
}

void on_message(WtUInt32 id, const char* topic, const char* data, WtUInt32 dataLen)
{
	printf("Client %u: ", id);
	printf(data);
	printf("\r\n");
}

void main()
{
	regiter_callbacks(on_log);

	auto cid = create_client("ipc:///wtpy/test.ipc", on_message);
	subscribe_topic(cid, "test");
	start_client(cid);

	auto sid = create_server("ipc:///wtpy/test.ipc");
	
	char buf[64] = { 0 };
	for(auto i = 0; i < 100; i++)
	{
		int count = sprintf(buf, "this is message no %d", i + 1);
		publish_message(sid, "topic_110", buf, count);
		std::this_thread::sleep_for(std::chrono::seconds(3));

		if(i % 5 == 0)
		{
			auto cid = create_client("ipc:///wtpy/test.ipc", on_message);
			if (i % 10 == 0)
				subscribe_topic(cid, "topic_110");
			else
				subscribe_topic(cid, "test");
			start_client(cid);
		}
	}
}