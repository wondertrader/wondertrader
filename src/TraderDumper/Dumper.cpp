#include "Dumper.h"
#include "WtHelper.h"
#include "TraderAdapter.h"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSUtils/WTSCfgLoader.h"

#include "../Includes/WTSVariant.hpp"

USING_NS_WTP;

WTSBaseDataMgr		g_bdMgr;
TraderAdapterMgr	g_adapterMgr;

void Dumper::init(const char* logProfile)
{
	WTSLogger::init(logProfile);
}

bool Dumper::config(const char* cfgfile, bool isFile, const char* modDir)
{
	WtHelper::set_module_dir(modDir);

	WTSVariant* root = NULL;
	if (isFile)
		root = WTSCfgLoader::load_from_file(cfgfile);
	else
		root = WTSCfgLoader::load_from_content(cfgfile, false);

	WTSVariant* cfg = root->get("config");
	if(cfg)
	{
		if(cfg->has("refresh_span"))
		{
			_refresh_span = cfg->getUInt32("refresh_span");
		}
		
	}

	//基础数据文件
	WTSVariant* cfgBF = root->get("basefiles");
	if (cfgBF->get("session"))
	{
		g_bdMgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("Trading sessions loaded");
	}

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			g_bdMgr.loadCommodities(cfgItem->asCString());
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				g_bdMgr.loadCommodities(cfgItem->get(i)->asCString());
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			g_bdMgr.loadContracts(cfgItem->asCString());
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				g_bdMgr.loadContracts(cfgItem->get(i)->asCString());
			}
		}
	}

	cfg = root->get("traders");
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* channelid = cfgItem->getCString("channelid");

		TraderAdapterPtr adapter(new TraderAdapter(&g_adapterMgr));
		adapter->init(channelid, cfgItem, &g_bdMgr);
		g_adapterMgr.addAdapter(channelid, adapter);
	}

	root->release();

	WTSLogger::info("交易数据落地模块初始化完成，主动刷新间隔:{}s", _refresh_span);

	return true;
}

void Dumper::run(bool bOnce /* = true */)
{
	g_adapterMgr.run();

	if(bOnce)
	{
		for (;;)
		{
			if (g_adapterMgr.isAnyAlive())
				std::this_thread::sleep_for(std::chrono::seconds(1));
			else
				break;
		}
	}
	else
	{
		_worker.reset(new StdThread([this]() {
			
			while(!_stopped)
			{
				std::this_thread::sleep_for(std::chrono::seconds(_refresh_span));
				g_adapterMgr.refresh();
			}
			
		}));
	}
}

void Dumper::release()
{
	_stopped = true;
	if (_worker)
		_worker->join();
}

void Dumper::on_account(const char* channelid, uint32_t curTDate, const char* currency, double prebalance, 
		double balance, double dynbalance, double closeprofit, double dynprofit, double fee, 
		double margin, double deposit, double withdraw, bool isLast)
{
	if (_cb_account)
		_cb_account(channelid, curTDate, currency, prebalance, balance, dynbalance, closeprofit, dynprofit, fee, margin, deposit, withdraw, isLast);
}

void Dumper::on_position(const char* channelid, const char* exchg, const char* code, uint32_t curTDate, uint32_t direct,
		double volume, double cost, double margin, double avgpx, double dynprofit, uint32_t volscale, bool isLast)
{
	if (_cb_position)
		_cb_position(channelid, exchg, code, curTDate, direct, volume, cost, margin, avgpx, dynprofit, volscale, isLast);
}

void Dumper::on_trade(const char* channelid, const char* exchg, const char* code, uint32_t curTDate, const char* tradeid, const char* orderid, 
		uint32_t direct, uint32_t offset, double volume, double price, double amount, uint32_t ordertype, uint32_t tradetype, WtUInt64 tradetime, bool isLast)
{
	if (_cb_trade)
		_cb_trade(channelid, exchg, code, curTDate, tradeid, orderid, direct, offset, volume, price, amount, ordertype, tradetype, tradetime, isLast);
}

void Dumper::on_order(const char* channelid, const char* exchg, const char* code, uint32_t curTDate, const char* orderid, uint32_t direct, uint32_t offset, double volume, double leftover, double traded, double price, uint32_t ordertype, uint32_t pricetype, WtUInt64 ordertime, uint32_t state, const char* statemsg, bool isLast)
{
	if (_cb_order)
		_cb_order(channelid, exchg, code, curTDate, orderid, direct, offset, volume, leftover, traded, price, ordertype, pricetype, ordertime, state, statemsg, isLast);
}