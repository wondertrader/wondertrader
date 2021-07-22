#include "../WtDtServo/WtDtServo.h"
#include "../Includes/WTSDataDef.hpp"

#include <thread>

void on_bar(WTSBarStruct* curBar, bool isLast)
{
	printf("%u\r\n", curBar->time);
}

void on_tick(WTSTickStruct* curTick, bool isLast)
{
	printf("%u.%u\r\n", curTick->action_date, curTick->action_time);
}

void testDtServo()
{
	initialize("dtservo.json", true);
	printf(get_version());
	printf("\r\n");

	//get_bars("CFFEX.IC.HOT", "m1", 202001010930, 202107211500, on_bar);

	get_bars_by_count("CFFEX.IC.HOT", "m3", 500, 202107221500, on_bar);

	std::this_thread::sleep_for(std::chrono::seconds(125));
	get_bars_by_count("CFFEX.IC.HOT", "m3", 500, 202107221500, on_bar);
	//get_ticks_by_count("CFFEX.IC.HOT", 80000, 202107221000, on_tick);
}