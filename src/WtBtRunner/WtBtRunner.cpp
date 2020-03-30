// WtBtRunner.cpp : 定义控制台应用程序的入口点。
//
#include "../WtBtCore/HisDataReplayer.h"
#include "../WtBtCore/CtaMocker.h"
#include "../WtBtCore/ExecMocker.h"

#include "../WTSTools/WTSLogger.h"

#include "../Share/StdUtils.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../Common/mdump.h"

int main()
{
	CMiniDumper::Enable("WtBtRunner.exe", true);

	WTSLogger::init("logcfg.json");

	std::string filename = "config.json";

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);

	rj::Document root;
	if (root.Parse(content.c_str()).HasParseError())
	{
		WTSLogger::info("配置文件解析失败");
		return -1;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	jsonToVariant(root, cfg);

	HisDataReplayer replayer;
	replayer.init(cfg->get("replayer"));

	WTSVariant* cfgEnv = cfg->get("env");
	const char* mode = cfgEnv->getCString("mocker");
	if (strcmp(mode, "cta") == 0)
	{
		CtaMocker* mocker = new CtaMocker(&replayer, "cta");
		mocker->initCtaFactory(cfg->get("cta"));
		replayer.register_sink(mocker);
	}
	else if (strcmp(mode, "exec") == 0)
	{
		ExecMocker* mocker = new ExecMocker(&replayer);
		mocker->init(cfg->get("exec"));
		replayer.register_sink(mocker);
	}

	replayer.run();

	printf("press enter key to exit\r\n");
	getchar();

	WTSLogger::stop();
}
