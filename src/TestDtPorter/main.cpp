#define _CRT_SECURE_NO_WARNINGS
#include "../WtDtHelper/WtDtHelper.h"
#include "../WtDtPorter/WtDtPorter.h"
#include "../Includes/WTSDataDef.hpp"
#include "../Share/fmtlib.h"

void on_get_bar(WTSBarStruct* bar, WtUInt32 count, bool isLast)
{
	fmt::print("{}.{}\n", bar->date, bar->time);
}

void on_bar_cnt(WtUInt32 dataCnt)
{
	fmt::print("共{}条K线\n", dataCnt);
}

void on_log(const char* message)
{
	fmt::print(message);
	fmt::print("\n");
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
	initialize("dtcfg.json", "logcfgdt.json", true, true);
	start();
}

int main()
{
	test_porter();
    return 0;
}