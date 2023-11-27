#include "TraderDumper.h"
#include "Dumper.h"

#include "../Share/ModuleHelper.hpp"

Dumper& getDumper()
{
	static Dumper dumper;
	return dumper;
}

void register_callbacks(FuncOnAccount cbAccount, FuncOnOrder  cbOrder, FuncOnTrade cbTrade, FuncOnPosition cbPosition)
{
	getDumper().register_callbacks(cbAccount, cbOrder, cbTrade, cbPosition);
}

void init(const char* logProfile)
{
	getDumper().init(logProfile);
}

bool config(const char* cfgfile, bool isFile)
{
	return getDumper().config(cfgfile, isFile, getBinDir());
}

void run(bool bOnce)
{
	getDumper().run(bOnce);
}

void release()
{
	getDumper().release();
}