/*!
 * \file WtBtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtBtRunner.h"
#include "PyCtaMocker.h"
#include "PySelMocker.h"

#include <iomanip>

#include "../WtBtCore/ExecMocker.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSCmpHelper.hpp"

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif


WtBtRunner::WtBtRunner()
	: _cta_mocker(NULL)
	, _sel_mocker(NULL)
{
}


WtBtRunner::~WtBtRunner()
{
}

void WtBtRunner::registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	_cb_cta_init = cbInit;
	_cb_cta_tick = cbTick;
	_cb_cta_calc = cbCalc;
	_cb_cta_bar = cbBar;
}

void WtBtRunner::registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	_cb_sel_init = cbInit;
	_cb_sel_tick = cbTick;
	_cb_sel_calc = cbCalc;
	_cb_sel_bar = cbBar;
}

uint32_t WtBtRunner::initCtaMocker(const char* name)
{
	_cta_mocker = new PyCtaMocker(&_replayer, name);
	_replayer.register_sink(_cta_mocker);
	return _cta_mocker->id();
}

uint32_t WtBtRunner::initSelMocker(const char* name, uint32_t date, uint32_t time, const char* period)
{
	_sel_mocker = new PySelMocker(&_replayer, name);
	_replayer.register_sink(_sel_mocker);

	_replayer.register_task(_sel_mocker->id(), date, time, period);
	return _sel_mocker->id();
}

void WtBtRunner::ctx_on_bar(uint32_t id, const char* code, const char* period, WTSBarStruct* newBar, bool isCta /*= true*/)
{
	if (isCta && _cb_cta_bar)
		_cb_cta_bar(id, code, period, newBar);
	else if(!isCta && _cb_sel_bar)
		_cb_sel_bar(id, code, period, newBar);
}

void WtBtRunner::ctx_on_calc(uint32_t id, bool isCta /*= true*/)
{
	if (isCta && _cb_cta_calc)
		_cb_cta_calc(id);
	else if (!isCta && _cb_sel_calc)
		_cb_sel_calc(id);
}

void WtBtRunner::ctx_on_init(uint32_t id, bool isCta /*= true*/)
{
	if (isCta && _cb_cta_init)
		_cb_cta_init(id);
	else if (!isCta && _cb_sel_init)
		_cb_sel_init(id);
}

void WtBtRunner::ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, bool isCta /*= true*/)
{
	if (isCta && _cb_cta_tick)
		_cb_cta_tick(id, stdCode, &newTick->getTickStruct());
	else if (!isCta && _cb_sel_tick)
		_cb_sel_tick(id, stdCode, &newTick->getTickStruct());
}

void WtBtRunner::init(const char* logProfile /* = "" */)
{
	WTSLogger::init(logProfile);
}

void WtBtRunner::config(const char* cfgFile)
{
	std::string content;
	StdFile::read_file_content(cfgFile, content);

	rj::Document root;
	if (root.Parse(content.c_str()).HasParseError())
	{
		WTSLogger::info("配置文件解析失败");
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	jsonToVariant(root, cfg);

	_replayer.init(cfg->get("replayer"));

	WTSVariant* cfgEnv = cfg->get("env");
	const char* mode = cfgEnv->getCString("mocker");
	WTSVariant* cfgMode = cfg->get(mode);
	if (strcmp(mode, "cta") == 0 && cfgMode)
	{
		_cta_mocker = new PyCtaMocker(&_replayer, "cta");
		_cta_mocker->initCtaFactory(cfgMode);
		_replayer.register_sink(_cta_mocker);
	}
	else if (strcmp(mode, "mf") == 0 && cfgMode)
	{
		_sel_mocker = new PySelMocker(&_replayer, "mf");
		_sel_mocker->initMfFactory(cfgMode);
		_replayer.register_sink(_sel_mocker);
	}
	else if (strcmp(mode, "exec") == 0 && cfgMode)
	{
		_exec_mocker = new ExecMocker(&_replayer);
		_exec_mocker->init(cfgMode);
		_replayer.register_sink(_exec_mocker);
	}
}

void WtBtRunner::run()
{
	_replayer.run();
}

void WtBtRunner::release()
{
	WTSLogger::stop();
}

extern uint32_t strToTime(const char* strTime, bool bHasSec = false);
extern uint32_t strToDate(const char* strDate);

void WtBtRunner::trans_mc_bars(const char* csvFolder, const char* binFolder, const char* period)
{
	if (!BoostFile::exists(csvFolder))
		return;

	if (!BoostFile::exists(binFolder))
		BoostFile::create_directories(binFolder);

	WTSKlinePeriod kp = KP_DAY;
	if (my_stricmp(period, "m1") == 0)
		kp = KP_Minute1;
	else if (my_stricmp(period, "m5") == 0)
		kp = KP_Minute5;
	else
		kp = KP_DAY;

	boost::filesystem::path myPath(csvFolder);
	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (boost::filesystem::is_directory(iter->path()))
			continue;

		if (iter->path().extension() != ".csv")
			continue;

		const std::string& path = iter->path().string();

		std::ifstream ifs;
		ifs.open(path.c_str());

		WTSLogger::info("正在读取数据文件%s……", path.c_str());

		char buffer[512];
		bool headerskipped = false;
		std::vector<WTSBarStruct> bars;
		while (!ifs.eof())
		{
			ifs.getline(buffer, 512);
			if (strlen(buffer) == 0)
				continue;

			//跳过头部
			if (!headerskipped)
			{
				headerskipped = true;
				continue;
			}

			//逐行读取
			StringVector ay = StrUtil::split(buffer, ",");
			WTSBarStruct bs;
			bs.date = strToDate(ay[0].c_str());
			bs.time = TimeUtils::timeToMinBar(bs.date, strToTime(ay[1].c_str()));
			bs.open = strtod(ay[2].c_str(), NULL);
			bs.high = strtod(ay[3].c_str(), NULL);
			bs.low = strtod(ay[4].c_str(), NULL);
			bs.close = strtod(ay[5].c_str(), NULL);
			bs.vol = strtoul(ay[6].c_str(), NULL, 10);
			bars.push_back(bs);

			if (bars.size() % 1000 == 0)
			{
				WTSLogger::info("已读取数据%u条", bars.size());
			}
		}
		ifs.close();
		WTSLogger::info("数据文件%s全部读取完成，共%u条", path.c_str(), bars.size());

		BlockType btype;
		switch (kp)
		{
		case KP_Minute1: btype = BT_HIS_Minute1; break;
		case KP_Minute5: btype = BT_HIS_Minute5; break;
		default: btype = BT_HIS_Day; break;
		}

		HisKlineBlockV2 kBlock;
		strcpy(kBlock._blk_flag, BLK_FLAG);
		kBlock._type = btype;
		kBlock._version = BLOCK_VERSION_CMP;

		std::string cmprsData = WTSCmpHelper::compress_data(bars.data(), sizeof(WTSBarStruct)*bars.size());
		kBlock._size = cmprsData.size();

		std::string filename = StrUtil::standardisePath(binFolder);
		filename += iter->path().stem().string();
		filename += ".dsb";

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			bf.write_file(&kBlock, sizeof(HisKlineBlockV2));
		}
		bf.write_file(cmprsData);
		bf.close_file();
		WTSLogger::info("数据已转储至%s", filename.c_str());
	}
}

void WtBtRunner::dump_bars(const char* code, const char* period, const char* filename)
{
	WTSKlineSlice* kline = _replayer.get_kline_slice(code, period, UINT_MAX);
	if (kline)
	{
		std::ofstream ofs;
		ofs.open(filename);
		ofs << "date,time,open,high,low,close,vol,turnover" << std::endl;

		for (int32_t idx = 0; idx < kline->size(); idx++)
		{
			const WTSBarStruct& bs = *(kline->at(idx));
			ofs << (bs.time / 10000) + 19900000 << ","
				<< bs.time % 10000 << ","
				<< bs.open << ","
				<< bs.high << ","
				<< bs.low << ","
				<< bs.close << ","
				<< bs.vol << ","
				<< std::fixed << std::setprecision(0) << bs.money << std::endl;
		}
		ofs.close();
		kline->release();
	}
}
