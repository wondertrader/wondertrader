/*!
 * \file main.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */

#include "stdafx.h"
#include "WtRunner.h"
#include "..\WTSTools\WTSLogger.h"
#include "..\Share\IniHelper.hpp"

//#include <vld.h>

void main()
{
	WtRunner runner;
	runner.init();

	runner.config("config.json");

	runner.run(false);
}

