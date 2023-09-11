/*!
 * /file main.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */

#include "../WTSTools/WTSLogger.h"

extern void test_uft();

int main()
{
	test_uft();
	printf("press enter key to exit\r\n");
	getchar();
	return 0;
}

