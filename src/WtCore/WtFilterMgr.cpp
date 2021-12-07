#include "WtFilterMgr.h"
#include "EventNotifier.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../WTSTools/WTSLogger.h"

#include <boost/filesystem.hpp>

#include <rapidjson/document.h>
namespace rj = rapidjson;

USING_NS_OTP;

void WtFilterMgr::load_filters(const char* fileName)
{
	if (_filter_file.empty() && (strlen(fileName) == 0))
		return;

	if(strlen(fileName) > 0)
		_filter_file = fileName;

	if (!StdFile::exists(_filter_file.c_str()))
	{
		WTSLogger::error("Filters configuration file %s not exists", _filter_file.c_str());
		return;
	}

	uint64_t lastModTime = boost::filesystem::last_write_time(boost::filesystem::path(_filter_file));
	if (lastModTime <= _filter_timestamp)
		return;

	if (_filter_timestamp != 0)
	{
		WTSLogger::info("Filters configuration file %s modified, will be reloaded", _filter_file.c_str());
		if (_notifier)
			_notifier->notifyEvent("Filter file has been reloaded");
	}

	std::string content;
	StdFile::read_file_content(_filter_file.c_str(), content);
	if (content.empty())
	{
		WTSLogger::error("Filters configuration file %s is empty", _filter_file.c_str());
		return;
	}

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
	{
		WTSLogger::error("Filters configuration file %s parsing failed", _filter_file.c_str());
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	if (!jsonToVariant(root, cfg))
	{
		WTSLogger::error("Filters configuration file %s converting failed", _filter_file.c_str());
		return;
	}

	_filter_timestamp = lastModTime;

	_stra_filters.clear();
	_code_filters.clear();
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
			if (wt_stricmp(action, "ignore") == 0)
				fAct = FA_Ignore;
			else if (wt_stricmp(action, "redirect") == 0)
				fAct = FA_Redirect;

			if (fAct == FA_None)
			{
				WTSLogger::error("Action %s of strategy filter %s not recognized", action, key.c_str());
				continue;
			}

			FilterItem& fItem = _stra_filters[key];
			fItem._key = key;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info("Strategy filter %s loaded", key.c_str());
		}
	}

	//读代码过滤器
	WTSVariant* filterCodes = cfg->get("code_filters");
	if (filterCodes)
	{
		auto codes = filterCodes->memberNames();
		for (const std::string& stdCode : codes)
		{

			WTSVariant* cfgItem = filterCodes->get(stdCode.c_str());
			const char* action = cfgItem->getCString("action");
			FilterAction fAct = FA_None;
			if (wt_stricmp(action, "ignore") == 0)
				fAct = FA_Ignore;
			else if (wt_stricmp(action, "redirect") == 0)
				fAct = FA_Redirect;

			if (fAct == FA_None)
			{
				WTSLogger::error("Action %s of code filter %s not recognized", action, stdCode.c_str());
				continue;
			}

			FilterItem& fItem = _code_filters[stdCode];
			fItem._key = stdCode;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info("Code filter %s loaded", stdCode.c_str());
		}
	}

	//读通道过滤器
	WTSVariant* filterExecuters = cfg->get("executer_filters");
	if (filterExecuters)
	{
		auto executer_ids = filterExecuters->memberNames();
		for (const std::string& execid : executer_ids)
		{
			bool bDisabled = filterExecuters->getBoolean(execid.c_str());
			WTSLogger::info("Executer %s is %s", execid.c_str(), bDisabled?"disabled":"enabled");
			_exec_filters[execid] = bDisabled;
		}
	}

	cfg->release();
}

bool WtFilterMgr::is_filtered_by_executer(const char* execid)
{
	auto it = _exec_filters.find(execid);
	if (it == _exec_filters.end())
		return false;

	return it->second;
}

const char* FLTACT_NAMEs[] =
{
	"Ignore",
	"Redirect"
};

bool WtFilterMgr::is_filtered_by_strategy(const char* straName, double& targetPos, bool isDiff /* = false */)
{
	auto it = _stra_filters.find(straName);
	if (it != _stra_filters.end())
	{
		const FilterItem& fItem = it->second;
		if(isDiff)
		{
			//如果过滤器触发，并且是增量头寸，则直接过滤掉
			WTSLogger::info("[Filters] Strategy filter %s triggered, the change of position ignored directly", straName);
			return true;
		}

		WTSLogger::info("[Filters] Strategy filter %s triggered, action: %s", straName, fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "Unknown");
		if (fItem._action == FA_Ignore)
		{
			return true;
		}
		else if (fItem._action == FA_Redirect)
		{
			//只有不是增量的时候,才有效
			targetPos = fItem._target;
		}

		return false;
	}

	return false;
}

bool WtFilterMgr::is_filtered_by_code(const char* stdCode, double& targetPos)
{
	auto cit = _code_filters.find(stdCode);
	if (cit != _code_filters.end())
	{
		const FilterItem& fItem = cit->second;
		WTSLogger::info("[Filters] Code filter %s triggered, action: %s", stdCode, fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "Unknown");
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
	cit = _code_filters.find(stdPID);
	if (cit != _code_filters.end())
	{
		const FilterItem& fItem = cit->second;
		WTSLogger::info("[Filters] CommID filter %s triggered, action: %s", stdPID.c_str(), fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "Unknown");
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



