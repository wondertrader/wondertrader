#include "../WtDtHelper/WtDtHelper.h"
#include "../WtDtPorter/WtDtPorter.h"
#include "../Includes/WTSDataDef.hpp"

void on_get_bar(WTSBarStruct* bar, WtUInt32 count, bool isLast)
{
	printf("%u\r\n", bar->time);
}

void on_bar_cnt(WtUInt32 dataCnt)
{
	printf("共%u条K线\r\n", dataCnt);
}

void on_log(const char* message)
{
	printf(message);
	printf("\r\n");
}

void testDtHelper()
{
	const char* session_str = "{\
		\"name\":\"股票白盘0930\",\
		\"offset\" : 0,\
		\"auction\" : {\
			\"from\": 929,\
			\"to\" : 930\
		},\
		\"sections\" : [{\
			\"from\": 930,\
			\"to\" : 1130\
			},{\
			\"from\": 1300,\
			\"to\" : 1500\
			}]\
		}";

	resample_bars("IC2009.dsb", on_get_bar, on_bar_cnt, 202001010931, 202009181500, "m1", 5, session_str, on_log);
}

void test_porter()
{
	initialize("dtcfg.json", "logcfgdt.json");
	start();
}

int main()
{
	test_porter();
    return 0;
}