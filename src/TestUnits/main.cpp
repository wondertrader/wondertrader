#include <gtest/gtest.h>
#ifdef _DEBUG
#pragma  comment(lib, "gtestd")
#else
#pragma  comment(lib, "gtest")
#endif


int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
