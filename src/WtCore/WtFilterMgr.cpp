#include "WtFilterMgr.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/BoostFile.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
namespace rj = rapidjson;

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif

USING_NS_OTP;

void WtFilterMgr::load_filters(const char* fileName)
{
	if (_filter_file.empty() && strlen(fileName) == 0)
		return;

	_filter_file = fileName;

	if (!StdFile::exists(_filter_file.c_str()))
	{
		WTSLogger::error("过滤器配置文件%s不存在", _filter_file.c_str());
		return;
	}

	uint64_t lastModTime = boost::filesystem::last_write_time(boost::filesystem::path(_filter_file));
	if (lastModTime <= _filter_timestamp)
		return;

	if(_filter_timestamp != 0)
		WTSLogger::info("过滤器配置文件%s已修改, 需要重新加载", _filter_file.c_str());

	std::string content;
	StdFile::read_file_content(_filter_file.c_str(), content);
	if (content.empty())
	{
		WTSLogger::error("过滤器配置文件%s为空", _filter_file.c_str());
		return;
	}

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
	{
		WTSLogger::error("过滤器配置文件%s解析失败", _filter_file.c_str());
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	if (!jsonToVariant(root, cfg))
	{
		WTSLogger::error("过滤器配置文件%s转换失败", _filter_file.c_str());
		return;
	}

	_filter_timestamp = lastModTime;

	_stra_filters.clear();
	_exec_filters.clear();

	//读策略过滤器
	WTSVariant* filterStra = cfg->get("strategy_filters");
	if (filterStra)
	{
		auto keys = filterStra->memberNames();
		for (const std::string& key : keys)
		{
			WTSVariant* cfgItem = filterStra->get(key.c_str());
			const char* action = cfgItem->getCString("action");
			FilterAction fAct = FA_None;
			if (my_stricmp(action, "ignore") == 0)
				fAct = FA_Ignore;
			else if (my_stricmp(action, "redirect") == 0)
				fAct = FA_Redirect;

			if (fAct == FA_None)
			{
				WTSLogger::error("策略过滤器%s操作%s不可识别", key.c_str(), action);
				continue;
			}

			FilterItem& fItem = _stra_filters[key];
			fItem._key = key;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info("策略过滤器%s已加载", key.c_str());
		}
	}

	//读执行器过滤器
	WTSVariant* filterCodes = cfg->get("code_filters");
	if (filterCodes)
	{
		auto codes = filterCodes->memberNames();
		for (const std::string& stdCode : codes)
		{

			WTSVariant* cfgItem = filterCodes->get(stdCode.c_str());
			const char* action = cfgItem->getCString("action");
			FilterAction fAct = FA_None;
			if (my_stricmp(action, "ignore") == 0)
				fAct = FA_Ignore;
			else if (my_stricmp(action, "redirect") == 0)
				fAct = FA_Redirect;

			if (fAct == FA_None)
			{
				WTSLogger::error("代码过滤器%s操作%s不可识别", stdCode.c_str(), action);
				continue;
			}

			FilterItem& fItem = _exec_filters[stdCode];
			fItem._key = stdCode;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info("代码过滤器%s已加载", stdCode.c_str());
		}
	}

	cfg->release();
}

const char* FLTACT_NAMEs[] =
{
	"忽略",
	"重定向"
};

bool WtFilterMgr::is_filtered_by_strategy(const char* straName, double& targetPos, bool isDiff /* = false */)
{
	auto it = _stra_filters.find(straName);
	if (it != _stra_filters.end())
	{
		const FilterItem& fItem = it->second;
		WTSLogger::info("[过滤器] 策略过滤器%s触发, 过滤操作: %s", straName, fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "未知");
		if (fItem._action == FA_Ignore)
		{
			return true;
		}
		else if (fItem._action == FA_Redirect && !isDiff)
		{
			//只有不是增量的时候，才有效
			targetPos = fItem._target;
		}

		return false;
	}

	return false;
}

bool WtFilterMgr::is_filtered_by_code(const char* stdCode, double& targetPos)
{
	auto cit = _exec_filters.find(stdCode);
	if (cit != _exec_filters.end())
	{
		const FilterItem& fItem = cit->second;
		WTSLogger::info("[过滤器] 代码过滤器%s触发, 过滤操作: %s", stdCode, fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "未知");
		if (fItem._action == FA_Ignore)
		{
			return true;
		}
		else if (fItem._action == FA_Redirect)
		{
			targetPos = fItem._target;
		}

		return false;
	}

	std::string stdPID = CodeHelper::stdCodeToStdCommID(stdCode);
	cit = _exec_filters.find(stdPID);
	if (cit != _exec_filters.end())
	{
		const FilterItem& fItem = cit->second;
		WTSLogger::info("[过滤器] 品种过滤器%s触发, 过滤操作: %s", stdPID.c_str(), fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "未知");
		if (fItem._action == FA_Ignore)
		{
			return true;
		}
		else if (fItem._action == FA_Redirect)
		{
			targetPos = fItem._target;
		}

		return false;
	}

	return false;
}

