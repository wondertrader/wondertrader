#include "gtest/gtest/gtest.h"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	int ret =  RUN_ALL_TESTS();
	printf("press enter key to exit\n");
	getchar();
	return ret;
}
