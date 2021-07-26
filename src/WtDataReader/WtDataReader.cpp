#include "WtDataReader.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/BoostFile.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSDataDef.hpp"

#include "../WTSTools/WTSCmpHelper.hpp"

#ifdef _WIN32
#pragma comment(lib, "libmysql.lib")
#endif

#include <rapidjson/document.h>
namespace rj = rapidjson;

extern "C"
{
	EXPORT_FLAG IDataReader* createDataReader()
	{
		IDataReader* ret = new WtDataReader();
		return ret;
	}

	EXPORT_FLAG void deleteDataReader(IDataReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};

void inst_hlp() {}

#ifdef _WIN32
#include "../Common/mdump.h"
#ifdef _WIN64
char PLATFORM_NAME[] = "X64";
#else
char PLATFORM_NAME[] = "WIN32";
#endif

HMODULE	g_dllModule = NULL;

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dllModule = (HMODULE)hModule;
		break;
	}
	return TRUE;
}

#else
#include <dlfcn.h>

char PLATFORM_NAME[] = "UNIX";

const std::string& getInstPath()
{
	static std::string moduleName;
	if (moduleName.empty())
	{
		Dl_info dl_info;
		dladdr((void *)inst_hlp, &dl_info);
		moduleName = dl_info.dli_fname;
		//printf("1:%s\n", moduleName.c_str());
	}

	return moduleName;
}
#endif

std::string getBinDir()
{
	static std::string _bin_dir;
	if (_bin_dir.empty())
	{


#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		_bin_dir = getInstPath();
#endif

		uint32_t nPos = _bin_dir.find_last_of('/');
		_bin_dir = _bin_dir.substr(0, nPos + 1);
	}

	return _bin_dir;
}



WtDataReader::WtDataReader()
	: _last_time(0)
	, _base_data_mgr(NULL)
	, _hot_mgr(NULL)
{
}


WtDataReader::~WtDataReader()
{
}

void WtDataReader::init(WTSVariant* cfg, IDataReaderSink* sink)
{
	_sink = sink;
	_base_data_mgr = sink->get_basedata_mgr();
	_hot_mgr = sink->get_hot_mgr();

	if (cfg == NULL)
		return ;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);

	WTSVariant* dbConf = cfg->get("db");
	if (dbConf)
	{
		strcpy(_db_conf._host, dbConf->getCString("host"));
		strcpy(_db_conf._dbname, dbConf->getCString("dbname"));
		strcpy(_db_conf._user, dbConf->getCString("user"));
		strcpy(_db_conf._pass, dbConf->getCString("pass"));
		_db_conf._port = dbConf->getInt32("port");

		_db_conf._active = (strlen(_db_conf._host) > 0) && (strlen(_db_conf._dbname) > 0) && (_db_conf._port != 0);
		if (_db_conf._active)
			init_db();
	}

	bool bAdjLoaded = false;
	if (_db_conn)
		bAdjLoaded = loadStkAdjFactorsFromDB();
	
	if (!bAdjLoaded && cfg->has("adjfactor"))
		loadStkAdjFactorsFromFile(cfg->getCString("adjfactor"));
}

void WtDataReader::init_db()
{
	if (!_db_conf._active)
		return;

#ifdef _WIN32
	std::string module = getBinDir() + "libmysql.dll";
	DLLHelper::load_library(module.c_str());
#endif

	_db_conn.reset(new MysqlDb);
	my_bool autoreconnect = true;
	_db_conn->options(MYSQL_OPT_RECONNECT, &autoreconnect);
	_db_conn->options(MYSQL_SET_CHARSET_NAME, "utf8");

	if (_db_conn->connect(_db_conf._dbname, _db_conf._host, _db_conf._user, _db_conf._pass, _db_conf._port, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS))
	{
		if (_sink)
			_sink->reader_log(LL_INFO, "数据库连接成功:%s:%d", _db_conf._host, _db_conf._port);
	}
	else
	{
		if (_sink)
			_sink->reader_log(LL_ERROR, "数据库连接失败[%s:%d]:%s", _db_conf._host, _db_conf._port, _db_conn->errstr());
		_db_conn.reset();
	}
}

bool WtDataReader::loadStkAdjFactorsFromDB()
{
	MysqlQuery query(*_db_conn);
	if(!query.exec("SELECT exchange,code,date,factor FROM tb_adj_factors ORDER BY exchange,code,date DESC;"))
	{
		if (_sink)
			_sink->reader_log(LL_ERROR, "查询除权因子表出错:%s", query.errormsg());
		return false;
	}

	uint32_t stk_cnt = 0;
	uint32_t fct_cnt = 0;
	while(query.fetch_row())
	{
		const char* exchg = query.getstr(0);
		const char* code = query.getstr(1);
		uint32_t uDate = query.getuint(2);
		double factor = query.getdouble(3);

		std::string key = StrUtil::printf("%s.%s", exchg, code);
		if (_adj_factors.find(key) == _adj_factors.end())
			stk_cnt++;

		AdjFactorList& fctrLst = _adj_factors[key];
		AdjFactor adjFact;
		adjFact._date = uDate;
		adjFact._factor = factor;

		fctrLst.emplace_back(adjFact);
		fct_cnt++;
	}

	if (_sink) 
		_sink->reader_log(LL_INFO, "共加载%u只股票的%u条除权因子数据", stk_cnt, fct_cnt);
	return true;
}

bool WtDataReader::loadStkAdjFactorsFromFile(const char* adjfile)
{
	if(!BoostFile::exists(adjfile))
	{
		if (_sink) _sink->reader_log(LL_ERROR, "除权因子文件%s不存在", adjfile);
		return false;
	}

	std::string content;
	BoostFile::read_file_contents(adjfile, content);

	rj::Document doc;
	doc.Parse(content.c_str());

	if(doc.HasParseError())
	{
		if (_sink) _sink->reader_log(LL_ERROR, "除权因子文件%s解析失败", adjfile);
		return false;
	}

	uint32_t stk_cnt = 0;
	uint32_t fct_cnt = 0;
	for (auto& mExchg : doc.GetObject())
	{
		const char* exchg = mExchg.name.GetString();
		const rj::Value& itemExchg = mExchg.value;
		for(auto& mCode : itemExchg.GetObject())
		{
			const char* code = mCode.name.GetString();
			const rj::Value& ayFacts = mCode.value;
			if(!ayFacts.IsArray() )
				continue;

			std::string key = StrUtil::printf("%s.%s", exchg, code);
			stk_cnt++;

			AdjFactorList& fctrLst = _adj_factors[key];
			for (auto& fItem : ayFacts.GetArray())
			{
				AdjFactor adjFact;
				adjFact._date = fItem["date"].GetUint();
				adjFact._factor = fItem["factor"].GetDouble();

				fctrLst.emplace_back(adjFact);
				fct_cnt++;
			}
		}
	}

	if (_sink) _sink->reader_log(LL_INFO, "共加载%u只股票的%u条除权因子数据", stk_cnt, fct_cnt);
	return true;
}

WTSTickSlice* WtDataReader::readTickSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		curSecs = _sink->get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime * 100000 + curSecs;
	}
	else
	{
		//20190807124533900
		curDate = (uint32_t)(etime / 1000000000);
		curTime = (uint32_t)(etime % 1000000000) / 100000;
		curSecs = (uint32_t)(etime % 100000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (cInfo.isHot() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
	else if (cInfo.isSecond() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);

	std::vector<WTSTickStruct>	ayTicks;

	//比较时间的对象
	WTSTickStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		TickBlockPair* tPair = getRTTickBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTTickBlock* tBlock = tPair->_block;

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tBlock->_size - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_ticks;

		//如果光标定位的tick时间比目标时间打, 则全部回退一个
		if (pTick->action_date > eTick.action_date || pTick->action_time>eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, cnt);
		return slice;
	}
	else
	{
		std::string key = StrUtil::printf("%s-%d", stdCode, endTDate);

		auto it = _his_tick_map.find(key);
		if(it == _his_tick_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisTBlockPair& tBlkPair = _his_tick_map[key];
			StdFile::read_file_content(filename.c_str(), tBlkPair._buffer);
			if (tBlkPair._buffer.size() < sizeof(HisTickBlock))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
				tBlkPair._buffer.clear();
				return NULL;
			}

			HisTickBlock* tBlock = (HisTickBlock*)tBlkPair._buffer.c_str();
			if (tBlock->_version == BLOCK_VERSION_CMP)
			{
				//压缩版本,要重新检查文件大小
				HisTickBlockV2* tBlockV2 = (HisTickBlockV2*)tBlkPair._buffer.c_str();

				if (tBlkPair._buffer.size() != (sizeof(HisTickBlockV2) + tBlockV2->_size))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史Tick数据文件%s大小校验失败", filename.c_str());
					return NULL;
				}

				//需要解压
				std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

				//将原来的buffer只保留一个头部,并将所有tick数据追加到尾部
				tBlkPair._buffer.resize(sizeof(HisTickBlock));
				tBlkPair._buffer.append(buf);
				tBlockV2->_version = BLOCK_VERSION_RAW;
			}
			
			tBlkPair._block = (HisTickBlock*)tBlkPair._buffer.c_str();
		}
		
		HisTBlockPair& tBlkPair = _his_tick_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisTickBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
		if (tcnt <= 0)
			return NULL;

		WTSTickStruct* pTick = std::lower_bound(tBlock->_ticks, tBlock->_ticks + (tcnt - 1), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_ticks;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTickSlice* slice = WTSTickSlice::create(stdCode, tBlock->_ticks + sIdx, cnt);
		return slice;
	}
}

WTSOrdQueSlice* WtDataReader::readOrdQueSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		curSecs = _sink->get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime * 100000 + curSecs;
	}
	else
	{
		//20190807124533900
		curDate = (uint32_t)(etime / 1000000000);
		curTime = (uint32_t)(etime % 1000000000) / 100000;
		curSecs = (uint32_t)(etime % 100000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (cInfo.isHot() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
	else if (cInfo.isSecond() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);

	//比较时间的对象
	WTSOrdQueStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		OrdQueBlockPair* tPair = getRTOrdQueBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTOrdQueBlock* rtBlock = tPair->_block;

		WTSOrdQueStruct* pItem = std::lower_bound(rtBlock->_queues, rtBlock->_queues + (rtBlock->_size - 1), eTick, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pItem - rtBlock->_queues;

		//如果光标定位的tick时间比目标时间打, 则全部回退一个
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, rtBlock->_queues + sIdx, cnt);
		return slice;
	}
	else
	{
		std::string key = StrUtil::printf("%s-%d", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/queue/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisOrdQueBlockPair& hisBlkPair = _his_ordque_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisOrdQueBlockV2))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史委托队列数据文件%s大小校验失败", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisOrdQueBlockV2* tBlockV2 = (HisOrdQueBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisOrdQueBlockV2) + tBlockV2->_size))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史委托队列数据文件%s大小校验失败", filename.c_str());
				return NULL;
			}

			//需要解压
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//将原来的buffer只保留一个头部,并将所有tick数据追加到尾部
			hisBlkPair._buffer.resize(sizeof(HisOrdQueBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW;

			hisBlkPair._block = (HisOrdQueBlock*)hisBlkPair._buffer.c_str();
		}

		HisOrdQueBlockPair& tBlkPair = _his_ordque_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisOrdQueBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisOrdQueBlock)) / sizeof(WTSOrdQueStruct);
		if (tcnt <= 0)
			return NULL;

		WTSOrdQueStruct* pTick = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSOrdQueStruct& a, const WTSOrdQueStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_items;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdQueSlice* slice = WTSOrdQueSlice::create(stdCode, tBlock->_items + sIdx, cnt);
		return slice;
	}
}

WTSOrdDtlSlice* WtDataReader::readOrdDtlSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		curSecs = _sink->get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime * 100000 + curSecs;
	}
	else
	{
		//20190807124533900
		curDate = (uint32_t)(etime / 1000000000);
		curTime = (uint32_t)(etime % 1000000000) / 100000;
		curSecs = (uint32_t)(etime % 100000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (cInfo.isHot() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
	else if (cInfo.isSecond() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);

	//比较时间的对象
	WTSOrdDtlStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		OrdDtlBlockPair* tPair = getRTOrdDtlBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTOrdDtlBlock* rtBlock = tPair->_block;

		WTSOrdDtlStruct* pItem = std::lower_bound(rtBlock->_details, rtBlock->_details + (rtBlock->_size - 1), eTick, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pItem - rtBlock->_details;

		//如果光标定位的tick时间比目标时间打, 则全部回退一个
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, rtBlock->_details + sIdx, cnt);
		return slice;
	}
	else
	{
		std::string key = StrUtil::printf("%s-%d", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/orders/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisOrdDtlBlockPair& hisBlkPair = _his_orddtl_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisOrdDtlBlockV2))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史逐笔委托数据文件%s大小校验失败", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisOrdDtlBlockV2* tBlockV2 = (HisOrdDtlBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisOrdDtlBlockV2) + tBlockV2->_size))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史逐笔委托数据文件%s大小校验失败", filename.c_str());
				return NULL;
			}

			//需要解压
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//将原来的buffer只保留一个头部,并将所有tick数据追加到尾部
			hisBlkPair._buffer.resize(sizeof(HisOrdDtlBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW;

			hisBlkPair._block = (HisOrdDtlBlock*)hisBlkPair._buffer.c_str();
		}

		HisOrdDtlBlockPair& tBlkPair = _his_orddtl_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisOrdDtlBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisOrdDtlBlock)) / sizeof(WTSOrdDtlStruct);
		if (tcnt <= 0)
			return NULL;

		WTSOrdDtlStruct* pTick = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSOrdDtlStruct& a, const WTSOrdDtlStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_items;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSOrdDtlSlice* slice = WTSOrdDtlSlice::create(stdCode, tBlock->_items + sIdx, cnt);
		return slice;
	}
}

WTSTransSlice* WtDataReader::readTransSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		curSecs = _sink->get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime * 100000 + curSecs;
	}
	else
	{
		//20190807124533900
		curDate = (uint32_t)(etime / 1000000000);
		curTime = (uint32_t)(etime % 1000000000) / 100000;
		curSecs = (uint32_t)(etime % 100000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);

	bool isToday = (endTDate == curTDate);

	std::string curCode = cInfo._code;
	if (cInfo.isHot() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
	else if (cInfo.isSecond() && cInfo._category == CC_Future)
		curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);

	//比较时间的对象
	WTSTransStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	if (isToday)
	{
		TransBlockPair* tPair = getRTTransBlock(cInfo._exchg, curCode.c_str());
		if (tPair == NULL)
			return NULL;

		RTTransBlock* rtBlock = tPair->_block;

		WTSTransStruct* pItem = std::lower_bound(rtBlock->_trans, rtBlock->_trans + (rtBlock->_size - 1), eTick, [](const WTSTransStruct& a, const WTSTransStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pItem - rtBlock->_trans;

		//如果光标定位的tick时间比目标时间打, 则全部回退一个
		if (pItem->action_date > eTick.action_date || pItem->action_time > eTick.action_time)
		{
			pItem--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTransSlice* slice = WTSTransSlice::create(stdCode, rtBlock->_trans + sIdx, cnt);
		return slice;
	}
	else
	{
		std::string key = StrUtil::printf("%s-%d", stdCode, endTDate);

		auto it = _his_ordque_map.find(key);
		if (it == _his_ordque_map.end())
		{
			std::stringstream ss;
			ss << _base_dir << "his/trans/" << cInfo._exchg << "/" << endTDate << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				return NULL;

			HisTransBlockPair& hisBlkPair = _his_trans_map[key];
			StdFile::read_file_content(filename.c_str(), hisBlkPair._buffer);
			if (hisBlkPair._buffer.size() < sizeof(HisTransBlockV2))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史逐笔成交数据文件%s大小校验失败", filename.c_str());
				hisBlkPair._buffer.clear();
				return NULL;
			}

			HisTransBlockV2* tBlockV2 = (HisTransBlockV2*)hisBlkPair._buffer.c_str();

			if (hisBlkPair._buffer.size() != (sizeof(HisTransBlockV2) + tBlockV2->_size))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史逐笔成交数据文件%s大小校验失败", filename.c_str());
				return NULL;
			}

			//需要解压
			std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);

			//将原来的buffer只保留一个头部,并将所有tick数据追加到尾部
			hisBlkPair._buffer.resize(sizeof(HisTransBlock));
			hisBlkPair._buffer.append(buf);
			tBlockV2->_version = BLOCK_VERSION_RAW;

			hisBlkPair._block = (HisTransBlock*)hisBlkPair._buffer.c_str();
		}

		HisTransBlockPair& tBlkPair = _his_trans_map[key];
		if (tBlkPair._block == NULL)
			return NULL;

		HisTransBlock* tBlock = tBlkPair._block;

		uint32_t tcnt = (tBlkPair._buffer.size() - sizeof(HisTransBlock)) / sizeof(WTSTransStruct);
		if (tcnt <= 0)
			return NULL;

		WTSTransStruct* pTick = std::lower_bound(tBlock->_items, tBlock->_items + (tcnt - 1), eTick, [](const WTSTransStruct& a, const WTSTransStruct& b) {
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t eIdx = pTick - tBlock->_items;
		if (pTick->action_date > eTick.action_date || pTick->action_time >= eTick.action_time)
		{
			pTick--;
			eIdx--;
		}

		uint32_t cnt = min(eIdx + 1, count);
		uint32_t sIdx = eIdx + 1 - cnt;
		WTSTransSlice* slice = WTSTransSlice::create(stdCode, tBlock->_items + sIdx, cnt);
		return slice;
	}
}

bool WtDataReader::cacheHisBarsFromDB(const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	std::string tbname, pname;
	switch (period)
	{
	case KP_Minute1: 
		tbname = "tb_kline_min1"; 
		pname = "min1";
		break;
	case KP_Minute5: 
		tbname = "tb_kline_min5";
		pname = "min5"; 
		break;
	default: 
		tbname = "tb_kline_day"; 
		pname = "day"; 
		break;
	}

	BarsList& barList = _bars_cache[key];
	barList._code = stdCode;
	barList._period = period;
	barList._exchg = cInfo._exchg;

	std::vector<std::vector<WTSBarStruct>*> barsSections;
	bool isDay = (period == KP_DAY);

	uint32_t realCnt = 0;
	if (!cInfo.isFlat() && cInfo._category == CC_Future)//如果是读取期货主力连续数据
	{
		const char* hot_flag = cInfo.isHot() ? "HOT" : "2ND";

		//先按照HOT代码进行读取, 如rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastHotTime = 0;
		for (;;)
		{
			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s.%s' ORDER BY `date`;", 
					tbname.c_str(), cInfo._exchg, cInfo._product, hot_flag);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s.%s' ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._product, hot_flag);

			MysqlQuery query(*_db_conn);
			if(!query.exec(sql))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线读取失败: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);

				uint32_t idx = 0;
				while(query.fetch_row())
				{
					WTSBarStruct& bs = hotAy->at(idx);
					bs.date = query.getuint(0);
					bs.time = query.getuint(1);
					bs.open = query.getdouble(2);
					bs.high = query.getdouble(3);
					bs.low = query.getdouble(4);
					bs.close = query.getdouble(5);
					bs.settle = query.getdouble(6);
					bs.vol = query.getuint(7);
					bs.money = query.getdouble(8);
					bs.hold = query.getuint(9);
					bs.add = query.getdouble(10);
					idx++;
				}

				if (period != KP_DAY)
					lastHotTime = hotAy->at(barcnt - 1).time;
				else
					lastHotTime = hotAy->at(barcnt - 1).date;

				if (_sink) _sink->reader_log(LL_INFO, "主力合约%s历史%s数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
			}

			break;
		}

		HotSections secs;
		if (cInfo.isHot())
		{
			if (!_hot_mgr->splitHotSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
				return false;
		}
		else if (cInfo.isSecond())
		{
			if (!_hot_mgr->splitSecondSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
				return false;
		}

		if (secs.empty())
			return false;

		bool bAllCovered = false;
		for (auto it = secs.rbegin(); it != secs.rend() && left > 0; it++)
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
			const HotSection& hotSec = *it;
			const char* curCode = hotSec._code.c_str();
			uint32_t rightDt = hotSec._e_date;
			uint32_t leftDt = hotSec._s_date;

			//要先将日期转换为边界时间
			uint32_t stime, etime;
			if (!isDay)
			{
				uint64_t sTime = _base_data_mgr->getBoundaryTime(stdPID.c_str(), leftDt, false, true);
				uint64_t eTime = _base_data_mgr->getBoundaryTime(stdPID.c_str(), rightDt, false, false);

				stime = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

				if (stime < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					stime = lastHotTime + 1;
				}

				etime = ((uint32_t)(eTime / 10000) - 19900000) * 10000 + (uint32_t)(eTime % 10000);

				if (etime <= lastHotTime)	//右边界时间小于最后一条Hot时间, 说明全部交叉了, 没有再找的必要了
					break;
			}
			else
			{
				stime = leftDt;
				if (stime < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					stime = lastHotTime + 1;
				}

				etime = rightDt;

				if (etime <= lastHotTime)
					break;
			}

			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s "
					"WHERE exchange='%s' AND code='%s' AND `date`>=%u AND `date`<=%u ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, curCode, stime, etime);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s "
					"WHERE exchange='%s' AND code='%s' AND `time`>=%u AND `time`<=%u ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, curCode, stime, etime);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线读取失败: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				auto tempAy = new std::vector<WTSBarStruct>();
				tempAy->resize(barcnt);

				uint32_t idx = 0;
				while (query.fetch_row())
				{
					WTSBarStruct& bs = tempAy->at(idx);
					bs.date = query.getuint(0);
					bs.time = query.getuint(1);
					bs.open = query.getdouble(2);
					bs.high = query.getdouble(3);
					bs.low = query.getdouble(4);
					bs.close = query.getdouble(5);
					bs.settle = query.getdouble(6);
					bs.vol = query.getuint(7);
					bs.money = query.getdouble(8);
					bs.hold = query.getuint(9);
					bs.add = query.getdouble(10);
					idx++;
				}

				realCnt += barcnt;

				barsSections.emplace_back(tempAy);

				if (bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if (cInfo.isExright() && cInfo._category == CC_Stock)//如果是读取股票复权数据
	{
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastQTime = 0;

		do
		{
			char flag = cInfo._exright == 1 ? 'Q' : 'H';
			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s%c' ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, flag);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s%c' ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, flag);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线读取失败: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);

				uint32_t idx = 0;
				while (query.fetch_row())
				{
					WTSBarStruct& bs = hotAy->at(idx);
					bs.date = query.getuint(0);
					bs.time = query.getuint(1);
					bs.open = query.getdouble(2);
					bs.high = query.getdouble(3);
					bs.low = query.getdouble(4);
					bs.close = query.getdouble(5);
					bs.settle = query.getdouble(6);
					bs.vol = query.getuint(7);
					bs.money = query.getdouble(8);
					bs.hold = query.getuint(9);
					bs.add = query.getdouble(10);
					idx++;
				}

				if (period != KP_DAY)
					lastQTime = hotAy->at(barcnt - 1).time;
				else
					lastQTime = hotAy->at(barcnt - 1).date;

				if (_sink) _sink->reader_log(LL_INFO, "股票%s历史%s复权数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
			}
			break;
		} while (false);

		bool bAllCovered = false;
		do
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
			const char* curCode = cInfo._code;

			//要先将日期转换为边界时间
			WTSBarStruct sBar;
			if (period != KP_DAY)
			{
				sBar.date = TimeUtils::minBarToDate(lastQTime);

				sBar.time = lastQTime + 1;
			}
			else
			{
				sBar.date = lastQTime + 1;
			}

			char sql[256] = { 0 };
			if (isDay)
				sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' AND date>=%u ORDER BY `date`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, lastQTime + 1);
			else
				sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' AND time>=%u ORDER BY `time`;",
					tbname.c_str(), cInfo._exchg, cInfo._code, lastQTime + 1);

			MysqlQuery query(*_db_conn);
			if (!query.exec(sql))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线读取失败: %s", query.errormsg());
			}
			else
			{
				uint32_t barcnt = (uint32_t)query.num_rows();
				if (barcnt > 0)
				{

					auto tempAy = new std::vector<WTSBarStruct>();
					tempAy->resize(barcnt);

					uint32_t idx = 0;
					while (query.fetch_row())
					{
						WTSBarStruct& bs = tempAy->at(idx);
						bs.date = query.getuint(0);
						bs.time = query.getuint(1);
						bs.open = query.getdouble(2);
						bs.high = query.getdouble(3);
						bs.low = query.getdouble(4);
						bs.close = query.getdouble(5);
						bs.settle = query.getdouble(6);
						bs.vol = query.getuint(7);
						bs.money = query.getdouble(8);
						bs.hold = query.getuint(9);
						bs.add = query.getdouble(10);
						idx++;
					}

					realCnt += barcnt;

					auto& ayFactors = getAdjFactors(cInfo._code, cInfo._exchg);
					if (!ayFactors.empty())
					{
						//做前复权处理
						int32_t lastIdx = barcnt;
						WTSBarStruct bar;
						WTSBarStruct* firstBar = tempAy->data();
						for (auto& adjFact : ayFactors)
						{
							bar.date = adjFact._date;
							double factor = adjFact._factor;

							WTSBarStruct* pBar = NULL;
							pBar = std::lower_bound(firstBar, firstBar + lastIdx - 1, bar, [period](const WTSBarStruct& a, const WTSBarStruct& b) {
								return a.date < b.date;
							});

							if (pBar->date < bar.date)
								continue;

							WTSBarStruct* endBar = pBar;
							if (pBar != NULL)
							{
								int32_t curIdx = pBar - firstBar;
								while (pBar && curIdx < lastIdx)
								{
									pBar->open /= factor;
									pBar->high /= factor;
									pBar->low /= factor;
									pBar->close /= factor;

									pBar++;
									curIdx++;
								}
								lastIdx = endBar - firstBar;
							}

							if (lastIdx == 0)
								break;
						}
					}

					barsSections.emplace_back(tempAy);
				}
			}
		} while (false);

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else
	{
		//读取历史的
		char sql[256] = { 0 };
		if (isDay)
			sprintf(sql, "SELECT `date`,0,open,high,low,close,settle,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' ORDER BY `date`;",
				tbname.c_str(), cInfo._exchg, cInfo._code);
		else
			sprintf(sql, "SELECT `date`,`time`,open,high,low,close,0,volume,turnover,interest,diff_interest FROM %s WHERE exchange='%s' AND code='%s' ORDER BY `time`;",
				tbname.c_str(), cInfo._exchg, cInfo._code);

		MysqlQuery query(*_db_conn);
		if (!query.exec(sql))
		{
			if (_sink) _sink->reader_log(LL_ERROR, "历史K线读取失败: %s", query.errormsg());
		}
		else
		{
			uint32_t barcnt = (uint32_t)query.num_rows();
			if (barcnt > 0)
			{
				auto tempAy = new std::vector<WTSBarStruct>();
				tempAy->resize(barcnt);

				uint32_t idx = 0;
				while (query.fetch_row())
				{
					WTSBarStruct& bs = tempAy->at(idx);
					bs.date = query.getuint(0);
					bs.time = query.getuint(1);
					bs.open = query.getdouble(2);
					bs.high = query.getdouble(3);
					bs.low = query.getdouble(4);
					bs.close = query.getdouble(5);
					bs.settle = query.getdouble(6);
					bs.vol = query.getuint(7);
					bs.money = query.getdouble(8);
					bs.hold = query.getuint(9);
					bs.add = query.getdouble(10);
					idx++;
				}

				realCnt += barcnt;

				barsSections.emplace_back(tempAy);
			}
		}
	}

	if (realCnt > 0)
	{
		barList._bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barList._bars.data() + curIdx, tempAy->data(), tempAy->size() * sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barsSections.clear();
	}

	if (_sink) _sink->reader_log(LL_INFO, "合约%s的历史%s数据已缓存%u条", stdCode, pname.c_str(), realCnt);
	return true;
}

bool WtDataReader::cacheHisBarsFromFile(const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "min1"; break;
	case KP_Minute5: pname = "min5"; break;
	default: pname = "day"; break;
	}

	BarsList& barList = _bars_cache[key];
	barList._code = stdCode;
	barList._period = period;
	barList._exchg = cInfo._exchg;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;
	if (!cInfo.isFlat() && cInfo._category == CC_Future)//如果是读取期货主力连续数据
	{
		const char* hot_flag = cInfo.isHot() ? "HOT" : "2ND";

		//先按照HOT代码进行读取, 如rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastHotTime = 0;
		for (;;)
		{
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._exchg << "." << cInfo._product << "_" << hot_flag << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if(kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
					break;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					break;

				std::string rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), rawData.data(), rawData.size());	
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), kBlock->_bars, sizeof(WTSBarStruct)*barcnt);
			}		

			if (period != KP_DAY)
				lastHotTime = hotAy->at(barcnt - 1).time;
			else
				lastHotTime = hotAy->at(barcnt - 1).date;

			if (_sink) _sink->reader_log(LL_INFO, "%u items of back %s data of hot contract %s directly loaded", barcnt, pname.c_str(), stdCode);
			break;
		}

		HotSections secs;
		if (cInfo.isHot())
		{
			if (!_hot_mgr->splitHotSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
				return false;
		}
		else if (cInfo.isSecond())
		{
			if (!_hot_mgr->splitSecondSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
				return false;
		}

		if (secs.empty())
			return false;

		bool bAllCovered = false;
		for (auto it = secs.rbegin(); it != secs.rend() && left > 0; it++)
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
			const HotSection& hotSec = *it;
			const char* curCode = hotSec._code.c_str();
			uint32_t rightDt = hotSec._e_date;
			uint32_t leftDt = hotSec._s_date;

			//要先将日期转换为边界时间
			WTSBarStruct sBar, eBar;
			if (period != KP_DAY)
			{
				uint64_t sTime = _base_data_mgr->getBoundaryTime(stdPID.c_str(), leftDt, false, true);
				uint64_t eTime = _base_data_mgr->getBoundaryTime(stdPID.c_str(), rightDt, false, false);

				sBar.date = leftDt;
				sBar.time = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

				if(sBar.time < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					sBar.time = lastHotTime + 1;
				}

				eBar.date = rightDt;
				eBar.time = ((uint32_t)(eTime / 10000) - 19900000) * 10000 + (uint32_t)(eTime % 10000);

				if (eBar.time <= lastHotTime)	//右边界时间小于最后一条Hot时间, 说明全部交叉了, 没有再找的必要了
					break;
			}
			else
			{
				sBar.date = leftDt;
				if (sBar.date < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					sBar.date = lastHotTime + 1;
				}

				eBar.date = rightDt;

				if (eBar.date <= lastHotTime)
					break;
			}

			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			{
				std::string content;
				StdFile::read_file_content(filename.c_str(), content);
				if (content.size() < sizeof(HisKlineBlock))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
					return false;
				}

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				WTSBarStruct* firstBar = NULL;
				uint32_t barcnt = 0;
				std::string rawData;
				if(kBlock->_version == BLOCK_VERSION_CMP)
				{
					if (content.size() < sizeof(HisKlineBlockV2))
					{
						if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
						break;
					}

					HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
					if (kBlockV2->_size == 0)
						break;

					rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
					barcnt = rawData.size() / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						break;

					firstBar = (WTSBarStruct*)rawData.data();
				}
				else
				{
					barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						continue;

					firstBar = kBlock->_bars;
				}

				WTSBarStruct* pBar = std::lower_bound(firstBar, firstBar + (barcnt - 1), sBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
					if (period == KP_DAY)
					{
						return a.date < b.date;
					}
					else
					{
						return a.time < b.time;
					}
				});

				uint32_t sIdx = pBar - firstBar;
				if ((period == KP_DAY && pBar->date < sBar.date) || (period != KP_DAY && pBar->time < sBar.time))	//早于边界时间
				{
					//早于边界时间, 说明没有数据了, 因为lower_bound会返回大于等于目标位置的数据
					continue;
				}

				pBar = std::lower_bound(firstBar + sIdx, firstBar + (barcnt - 1), eBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
					if (period == KP_DAY)
					{
						return a.date < b.date;
					}
					else
					{
						return a.time < b.time;
					}
				});
				uint32_t eIdx = pBar - firstBar;
				if ((period == KP_DAY && pBar->date > eBar.date) || (period != KP_DAY && pBar->time > eBar.time))
				{
					pBar--;
					eIdx--;
				}

				if (eIdx < sIdx)
					continue;

				uint32_t curCnt = eIdx - sIdx + 1;
				std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
				tempAy->resize(curCnt);
				memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
				realCnt += curCnt;

				barsSections.emplace_back(tempAy);

				if(bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else if(cInfo.isExright() && cInfo._category == CC_Stock)//如果是读取股票复权数据
	{
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastQTime = 0;
		
		do
		{
			//先直接读取复权过的历史数据,路径如/his/day/sse/SH600000Q.dsb
			char flag = cInfo._exright == 1 ? 'Q' : 'H';
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._code << flag << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
					break;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					break;

				std::string rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), rawData.data(), rawData.size());
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), kBlock->_bars, sizeof(WTSBarStruct)*barcnt);
			}

			if (period != KP_DAY)
				lastQTime = hotAy->at(barcnt - 1).time;
			else
				lastQTime = hotAy->at(barcnt - 1).date;

			if (_sink) _sink->reader_log(LL_INFO, "股票%s历史%s复权数据直接缓存%u条", stdCode, pname.c_str(), barcnt);
			break;
		} while (false);

		bool bAllCovered = false;
		do
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
			const char* curCode = cInfo._code;

			//要先将日期转换为边界时间
			WTSBarStruct sBar;
			if (period != KP_DAY)
			{
				sBar.date = TimeUtils::minBarToDate(lastQTime);

				sBar.time = lastQTime + 1;
			}
			else
			{
				sBar.date = lastQTime + 1;
			}

			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			{
				std::string content;
				StdFile::read_file_content(filename.c_str(), content);
				if (content.size() < sizeof(HisKlineBlock))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
					return false;
				}

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				WTSBarStruct* firstBar = NULL;
				uint32_t barcnt = 0;
				std::string rawData;
				if (kBlock->_version == BLOCK_VERSION_CMP)
				{
					if (content.size() < sizeof(HisKlineBlockV2))
					{
						if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
						break;
					}

					HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
					if (kBlockV2->_size == 0)
						break;

					rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
					barcnt = rawData.size() / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						break;

					firstBar = (WTSBarStruct*)rawData.data();
				}
				else
				{
					barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						continue;

					firstBar = kBlock->_bars;
				}

				WTSBarStruct* pBar = std::lower_bound(firstBar, firstBar + (barcnt - 1), sBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
					if (period == KP_DAY)
					{
						return a.date < b.date;
					}
					else
					{
						return a.time < b.time;
					}
				});

				if(pBar != NULL)
				{
					uint32_t sIdx = pBar - firstBar;
					uint32_t curCnt = barcnt - sIdx;
					std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
					tempAy->resize(curCnt);
					memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
					realCnt += curCnt;

					auto& ayFactors = getAdjFactors(cInfo._code, cInfo._exchg);
					if(!ayFactors.empty())
					{
						//做前复权处理
						int32_t lastIdx = curCnt;
						WTSBarStruct bar;
						firstBar = tempAy->data();
						for (auto& adjFact : ayFactors)
						{
							bar.date = adjFact._date;
							double factor = adjFact._factor;

							WTSBarStruct* pBar = NULL;
							pBar = std::lower_bound(firstBar, firstBar + lastIdx - 1, bar, [period](const WTSBarStruct& a, const WTSBarStruct& b) {
								return a.date < b.date;
							});

							if (pBar->date < bar.date)
								continue;

							WTSBarStruct* endBar = pBar;
							if (pBar != NULL)
							{
								int32_t curIdx = pBar - firstBar;
								while (pBar && curIdx < lastIdx)
								{
									pBar->open /= factor;
									pBar->high /= factor;
									pBar->low /= factor;
									pBar->close /= factor;

									pBar++;
									curIdx++;
								}
								lastIdx = endBar - firstBar;
							}

							if (lastIdx == 0)
								break;
						}
					}

					barsSections.emplace_back(tempAy);
				}
			}
		} while (false);

		if (hotAy)
		{
			barsSections.emplace_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else
	{
		//读取历史的
		std::stringstream ss;
		ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._code << ".dsb";
		std::string filename = ss.str();
		if (StdFile::exists(filename.c_str()))
		{
			//如果有格式化的历史数据文件, 则直接读取
			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
				return false;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			WTSBarStruct* firstBar = NULL;
			uint32_t barcnt = 0;
			std::string rawData;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					if (_sink) _sink->reader_log(LL_ERROR, "历史K线数据文件%s大小校验失败", filename.c_str());
					return false;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					return false;

				rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					return false;

				firstBar = (WTSBarStruct*)rawData.data();
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					return false;

				firstBar = kBlock->_bars;
			}

			if (barcnt > 0)
			{
				
				uint32_t sIdx = 0;
				uint32_t idx = barcnt - 1;
				uint32_t curCnt = (idx - sIdx + 1);

				std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
				tempAy->resize(curCnt);
				memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
				realCnt += curCnt;

				barsSections.emplace_back(tempAy);
			}
		}
	}

	if (realCnt > 0)
	{
		barList._bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barList._bars.data() + curIdx, tempAy->data(), tempAy->size()*sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barsSections.clear();
	}

	if (_sink) _sink->reader_log(LL_INFO, "合约%s的历史%s数据已缓存%u条", stdCode, pname.c_str(), realCnt);
	return true;
}

WTSBarStruct* WtDataReader::indexBarFromCache(const std::string& key, uint64_t etime, uint32_t& count, bool isDay /* = false */)
{
	uint32_t curDate, curTime;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
	}
	else
	{
		curDate = (uint32_t)(etime / 10000);
		curTime = (uint32_t)(etime % 10000);
	}

	BarsList& barsList = _bars_cache[key];
	if (barsList._his_cursor == UINT_MAX)
	{
		//光标尚未初始化, 需要重新定位
		uint64_t nowTime = (uint64_t)curDate * 10000 + curTime;
		if (_last_time < nowTime)
		{
			_last_time = nowTime;
		}

		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;
		auto it = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), bar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});


		if (it == barsList._bars.end())
			barsList._his_cursor = barsList._bars.size() - 1;
		else
		{
			if ((isDay && it->date > bar.date) || (!isDay && it->time > bar.time))
			{
				it--;
			}

			barsList._his_cursor = it - barsList._bars.begin();
		}
	}

	uint32_t sIdx = 0;
	if (count <= barsList._his_cursor + 1)
	{
		sIdx = barsList._his_cursor - count + 1;
	}

	uint32_t curCnt = barsList._his_cursor - sIdx + 1;
	//if (curCnt > 0)
	//{
	//	ayBars.resize(curCnt);
	//	memcpy(ayBars.data(), &barsList._bars[sIdx], sizeof(WTSBarStruct)*curCnt);
	//}
	count = curCnt;
	return &barsList._bars[sIdx];
}

uint32_t WtDataReader::readBarsFromCache(const std::string& key, uint64_t etime, uint32_t count, std::vector<WTSBarStruct>& ayBars, bool isDay /* = false */)
{
	uint32_t curDate, curTime;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
	}
	else
	{
		curDate = (uint32_t)(etime / 10000);
		curTime = (uint32_t)(etime % 10000);
	}

	BarsList& barsList = _bars_cache[key];
	if (barsList._his_cursor == UINT_MAX)
	{
		//光标尚未初始化, 需要重新定位
		uint64_t nowTime = (uint64_t)curDate * 10000 + curTime;
		if (_last_time < nowTime)
		{
			_last_time = nowTime;
		}

		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;
		auto it = std::lower_bound(barsList._bars.begin(), barsList._bars.end(), bar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});
		

		if(it == barsList._bars.end())
			barsList._his_cursor = barsList._bars.size() - 1;
		else
		{
			if ((isDay && it->date > bar.date) || (!isDay && it->time > bar.time))
			{
				if (it == barsList._bars.begin())
					return 0;
				
				it--;
			}

			barsList._his_cursor = it - barsList._bars.begin();
		}
	}

	uint32_t sIdx = 0;
	if (count <= barsList._his_cursor + 1)
	{
		sIdx = barsList._his_cursor - count + 1;
	}

	uint32_t curCnt = barsList._his_cursor - sIdx + 1;
	if(curCnt > 0)
	{
		ayBars.resize(curCnt);
		memcpy(ayBars.data(), &barsList._bars[sIdx], sizeof(WTSBarStruct)*curCnt);
	}
	return curCnt;
}

WTSKlineSlice* WtDataReader::readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	std::string key = StrUtil::printf("%s#%u", stdCode, period);
	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		if(_db_conn)
			bHasHisData = cacheHisBarsFromDB(key, stdCode, period);
		else
			bHasHisData = cacheHisBarsFromFile(key, stdCode, period);
	}
	else
	{
		bHasHisData = true;
	}

	uint32_t curDate, curTime;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		etime = (uint64_t)curDate * 10000 + curTime;
	}
	else
	{
		curDate = (uint32_t)(etime / 10000);
		curTime = (uint32_t)(etime % 10000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);
	uint32_t curTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), 0, 0, false);


	//WTSKlineSlice* kData = WTSKlineSlice::create(stdCode, period, times, NULL, 0, NULL, 0);
	
	WTSBarStruct* hisHead = NULL;
	WTSBarStruct* rtHead = NULL;
	uint32_t hisCnt = 0;
	uint32_t rtCnt = 0;

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "min1"; break;
	case KP_Minute5: pname = "min5"; break;
	default: pname = "day"; break;
	}

	uint32_t left = count;

	//是否包含当天的
	bool bHasToday = (endTDate == curTDate);

	if (cInfo.isHot() && cInfo._category == CC_Future)
	{
		_bars_cache[key]._raw_code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, curTDate);
		if (_sink) _sink->reader_log(LL_INFO, "主力合约映射确认: %s -> %s", stdCode, _bars_cache[key]._raw_code.c_str());
	}
	else if (cInfo.isSecond() && cInfo._category == CC_Future)
	{
		_bars_cache[key]._raw_code = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, curTDate);
		if (_sink) _sink->reader_log(LL_INFO, "次主力合约映射确认: %s -> %s", stdCode, _bars_cache[key]._raw_code.c_str());
	}
	else
	{
		_bars_cache[key]._raw_code = cInfo._code;
	}

	if (bHasToday)
	{
		WTSBarStruct bar;
		bar.date = curDate;
		bar.time = (curDate - 19900000) * 10000 + curTime;

		const char* curCode = _bars_cache[key]._raw_code.c_str();

		//读取实时的
		RTKlineBlockPair* kPair = getRTKilneBlock(cInfo._exchg, curCode, period);
		if (kPair != NULL)
		{
			//读取当日的数据
			WTSBarStruct* pBar = std::lower_bound(kPair->_block->_bars, kPair->_block->_bars + (kPair->_block->_size - 1), bar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
				if (period == KP_DAY)
					return a.date < b.date;
				else
					return a.time < b.time;
			});
			uint32_t idx = pBar - kPair->_block->_bars;
			if ((period == KP_DAY && pBar->date > bar.date) || (period != KP_DAY && pBar->time > bar.time))
			{
				pBar--;
				idx--;
			}

			_bars_cache[key]._rt_cursor = idx;

			uint32_t sIdx = 0;
			if (left <= idx + 1)
			{
				sIdx = idx - left + 1;
			}

			uint32_t curCnt = (idx - sIdx + 1);
			left -= (idx - sIdx + 1);
			rtHead = &kPair->_block->_bars[sIdx];
			rtCnt = curCnt;
		}
	}

	if (left > 0 && bHasHisData)
	{
		hisCnt = left;
		hisHead = indexBarFromCache(key, etime, hisCnt, period == KP_DAY);
	}

	if (hisCnt + rtCnt > 0)
	{
		WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, 1, hisHead, hisCnt, rtHead, rtCnt);
		return slice;
	}

	return NULL;
}


WtDataReader::TickBlockPair* WtDataReader::getRTTickBlock(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);

	std::string path = StrUtil::printf("%srt/ticks/%s/%s.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	TickBlockPair& block = _rt_tick_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTickBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//说明文件大小已变, 需要重新映射
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTickBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::OrdDtlBlockPair* WtDataReader::getRTOrdDtlBlock(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);

	std::string path = StrUtil::printf("%srt/orders/%s/%s.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	OrdDtlBlockPair& block = _rt_orddtl_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdDtlBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//说明文件大小已变, 需要重新映射
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdDtlBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::OrdQueBlockPair* WtDataReader::getRTOrdQueBlock(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);

	std::string path = StrUtil::printf("%srt/queue/%s/%s.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	OrdQueBlockPair& block = _rt_ordque_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdQueBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//说明文件大小已变, 需要重新映射
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTOrdQueBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::TransBlockPair* WtDataReader::getRTTransBlock(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);

	std::string path = StrUtil::printf("%srt/trans/%s/%s.dmb", _base_dir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	TransBlockPair& block = _rt_trans_map[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTransBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//说明文件大小已变, 需要重新映射
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTTransBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

WtDataReader::RTKlineBlockPair* WtDataReader::getRTKilneBlock(const char* exchg, const char* code, WTSKlinePeriod period)
{
	if (period != KP_Minute1 && period != KP_Minute5)
		return NULL;

	std::string key = StrUtil::printf("%s.%s", exchg, code);

	RTKBlockFilesMap* cache_map = NULL;
	std::string subdir = "";
	BlockType bType;
	switch (period)
	{
	case KP_Minute1:
		cache_map = &_rt_min1_map;
		subdir = "min1";
		bType = BT_RT_Minute1;
		break;
	case KP_Minute5:
		cache_map = &_rt_min5_map;
		subdir = "min5";
		bType = BT_RT_Minute5;
		break;
	default: break;
	}

	std::string path = StrUtil::printf("%srt/%s/%s/%s.dmb", _base_dir.c_str(), subdir.c_str(), exchg, code);
	if (!StdFile::exists(path.c_str()))
		return NULL;

	RTKlineBlockPair& block = (*cache_map)[key];
	if (block._file == NULL || block._block == NULL)
	{
		if (block._file == NULL)
		{
			block._file.reset(new BoostMappingFile());
		}

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTKlineBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}
	else if (block._last_cap != block._block->_capacity)
	{
		//说明文件大小已变, 需要重新映射
		block._file.reset(new BoostMappingFile());
		block._last_cap = 0;
		block._block = NULL;

		if (!block._file->map(path.c_str(), boost::interprocess::read_only, boost::interprocess::read_only))
			return NULL;

		block._block = (RTKlineBlock*)block._file->addr();
		block._last_cap = block._block->_capacity;
	}

	return &block;
}

void WtDataReader::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */)
{
	//这里应该触发检查
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	if (nowTime <= _last_time)
		return;

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = (BarsList&)it->second;
		if (barsList._period != KP_DAY)
		{
			//如果历史数据指标不在尾部, 说明是回测模式, 要继续回放历史数据
			if (barsList._bars.size() - 1 > barsList._his_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._his_cursor + 1];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
					}
					else
					{
						break;
					}

					barsList._his_cursor++;

					if (barsList._his_cursor == barsList._bars.size() - 1)
						break;
				}
			}
			else if (!barsList._raw_code.empty())
			{
				RTKlineBlockPair* kBlk = getRTKilneBlock(barsList._exchg.c_str(), barsList._raw_code.c_str(), barsList._period);
				if (kBlk == NULL)
					continue;

				uint32_t preCnt = 0;
				if (barsList._rt_cursor == UINT_MAX)
					preCnt = 0;
				else
					preCnt = barsList._rt_cursor + 1;

				for (;;)
				{
					if (kBlk->_block->_size <= preCnt)
						break;

					WTSBarStruct& nextBar = kBlk->_block->_bars[preCnt];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
					}
					else
					{
						break;
					}

					preCnt++;
				}

				if (preCnt > 0)
					barsList._rt_cursor = preCnt - 1;
			}
		}
		else
		{
			if (barsList._his_cursor != UINT_MAX && barsList._bars.size() - 1 > barsList._his_cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._his_cursor + 1];

					if (nextBar.date <= endTDate)
					{
						_sink->on_bar(barsList._code.c_str(), barsList._period, &nextBar);
					}
					else
					{
						break;
					}

					barsList._his_cursor++;

					if (barsList._his_cursor == barsList._bars.size() - 1)
						break;
				}
			}
		}
	}

	if (_sink)
		_sink->on_all_bar_updated(uTime);

	_last_time = nowTime;
}