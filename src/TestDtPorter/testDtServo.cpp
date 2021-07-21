#include "../WtDtServo/WtDtServo.h"
#include "../Includes/WTSDataDef.hpp"

#include <thread>

void on_bar(WTSBarStruct* curBar, bool isLast)
{
	printf("%u\r\n", curBar->time);
}

void testDtServo()
{
	initialize("dtservo.json", "logcfg.json");
	printf(get_version());
	printf("\r\n");

	get_bars("CFFEX.IC.HOT", "m1", 202001010930, 202107211500, on_bar);

	get_bars("CFFEX.IC.HOT", "m3", 202001010930, 202107211500, on_bar);

	std::this_thread::sleep_for(std::chrono::seconds(125));
	get_bars("CFFEX.IC.HOT", "m3", 202001010930, 202107211500, on_bar);
}