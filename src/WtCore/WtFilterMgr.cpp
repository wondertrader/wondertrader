#include "WtFilterMgr.h"
#include "EventNotifier.h"

#include "../Share/CodeHelper.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../WTSUtils/WTSCfgLoader.h"
#include "../WTSTools/WTSLogger.h"

#include <boost/filesystem.hpp>

USING_NS_WTP;

void WtFilterMgr::load_filters(const char* fileName)
{
	if (_filter_file.empty() && (strlen(fileName) == 0))
		return;

	if(strlen(fileName) > 0)
		_filter_file = fileName;

	if (!StdFile::exists(_filter_file.c_str()))
	{
		WTSLogger::debug_f("Filters configuration file {} not exists", _filter_file);
		return;
	}

	uint64_t lastModTime = boost::filesystem::last_write_time(boost::filesystem::path(_filter_file));
	if (lastModTime <= _filter_timestamp)
		return;

	if (_filter_timestamp != 0)
	{
		WTSLogger::info_f("Filters configuration file {} modified, will be reloaded", _filter_file);
		if (_notifier)
			_notifier->notifyEvent("Filter file has been reloaded");
	}

	WTSVariant* cfg = WTSCfgLoader::load_from_file(_filter_file.c_str(), true);

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
				WTSLogger::error_f("Action {} of strategy filter {} not recognized", action, key);
				continue;
			}

			FilterItem& fItem = _stra_filters[key];
			fItem._key = key;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info_f("Strategy filter {} loaded", key);
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
				WTSLogger::error_f("Action {} of code filter {} not recognized", action, stdCode);
				continue;
			}

			FilterItem& fItem = _code_filters[stdCode];
			fItem._key = stdCode;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info_f("Code filter {} loaded", stdCode);
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
			WTSLogger::info_f("Executer {} is %s", execid, bDisabled?"disabled":"enabled");
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



