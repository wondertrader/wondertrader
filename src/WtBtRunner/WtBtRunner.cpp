/*!
 * \file WtBtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "../WtBtCore/HisDataReplayer.h"
#include "../WtBtCore/CtaMocker.h"
#include "../WtBtCore/ExecMocker.h"
#include "../WtBtCore/HftMocker.h"
#include "../WtBtCore/SelMocker.h"

#include "../WTSTools/WTSLogger.h"

#include "../Share/StdUtils.hpp"
#include "../Share/JsonToVariant.hpp"

int main()
{
	WTSLogger::init("logcfg.json");

	std::string filename = "config.json";

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);

	rj::Document root;
	if (root.Parse(content.c_str()).HasParseError())
	{
		WTSLogger::info("Parsing configuration file failed");
		return -1;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	jsonToVariant(root, cfg);

	HisDataReplayer replayer;
	replayer.init(cfg->get("replayer"));

	WTSVariant* cfgEnv = cfg->get("env");
	const char* mode = cfgEnv->getCString("mocker");
	int32_t slippage = cfgEnv->getInt32("slippage");
	if (strcmp(mode, "cta") == 0)
	{
		CtaMocker* mocker = new CtaMocker(&replayer, "cta", slippage);
		mocker->initCtaFactory(cfg->get("cta"));
		replayer.register_sink(mocker, "cta");
	}
	else if (strcmp(mode, "hft") == 0)
	{
		HftMocker* mocker = new HftMocker(&replayer, "hft");
		mocker->initHftFactory(cfg->get("hft"));
		replayer.register_sink(mocker, "hft");
	}
	else if (strcmp(mode, "sel") == 0)
	{
		SelMocker* mocker = new SelMocker(&replayer, "sel", slippage);
		mocker->initSelFactory(cfg->get("cta"));
		replayer.register_sink(mocker, "sel");
	}
	else if (strcmp(mode, "exec") == 0)
	{
		ExecMocker* mocker = new ExecMocker(&replayer);
		mocker->init(cfg->get("exec"));
		replayer.register_sink(mocker, "exec");
	}

	replayer.run();

	printf("press enter key to exit\r\n");
	getchar();

	WTSLogger::stop();
}
