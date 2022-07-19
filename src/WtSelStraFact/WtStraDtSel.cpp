#include "WtStraDtSel.h"

#include "../Includes/ISelStraCtx.h"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share/fmtlib.h"

extern const char* FACT_NAME;

WtStraDtSel::WtStraDtSel(const char* id)
	:SelStrategy(id)
{
}


WtStraDtSel::~WtStraDtSel()
{
}

const char* WtStraDtSel::getName()
{
	return "DualThrustSelection";
}

const char* WtStraDtSel::getFactName()
{
	return FACT_NAME;
}

bool WtStraDtSel::init(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	_days = cfg->getUInt32("days");
	_k1 = cfg->getDouble("k1");
	_k2 = cfg->getDouble("k2");

	_period = cfg->getCString("period");
	_count = cfg->getUInt32("count");

	_isstk = cfg->getBoolean("stock");

	//ͨ������ȷ����ʼ�����״���
	std::string codes = cfg->getCString("codes");
	auto ayCodes = StrUtil::split(codes, ",");
	for (auto& code : ayCodes)
		_codes.insert(code);

	return true;
}

void WtStraDtSel::on_init(ISelStraCtx* ctx)
{
	for(auto& code : _codes)
	{
		ctx->stra_sub_ticks(code.c_str());
	}
}

void WtStraDtSel::on_schedule(ISelStraCtx* ctx, uint32_t uDate, uint32_t uTime)
{
	for (auto& curCode : _codes)
	{
		WTSSessionInfo* sInfo = ctx->stra_get_sessinfo(curCode.c_str());
		if(!sInfo->isInTradingTime(uTime))
			continue;

		std::string code = curCode;
		if (_isstk)
			code += "-";
		WTSKlineSlice *kline = ctx->stra_get_bars(code.c_str(), _period.c_str(), _count);
		if (kline == NULL)
		{
			//����������һЩ��־
			return;
		}

		if (kline->size() == 0)
		{
			kline->release();
			return;
		}

		int32_t trdUnit = 1;
		if (_isstk)
			trdUnit = 100;


		int32_t days = (int32_t)_days;

		double hh = kline->maxprice(-days, -2);
		double ll = kline->minprice(-days, -2);

		WTSValueArray* closes = kline->extractData(KFT_CLOSE);
		double hc = closes->maxvalue(-days, -2);
		double lc = closes->minvalue(-days, -2);
		double curPx = closes->at(-1);
		closes->release();///!!!����ͷ�һ��Ҫ��

		double openPx = kline->at(-1)->open;
		double highPx = kline->at(-1)->high;
		double lowPx = kline->at(-1)->low;

		double upper_bound = openPx + _k1 * (std::max(hh - lc, hc - ll));
		double lower_bound = openPx - _k2 * std::max(hh - lc, hc - ll);

		WTSCommodityInfo* commInfo = ctx->stra_get_comminfo(curCode.c_str());

		double curPos = ctx->stra_get_position(curCode.c_str()) / trdUnit;
		if (decimal::eq(curPos, 0))
		{
			if (highPx >= upper_bound)
			{
				ctx->stra_set_position(curCode.c_str(), 1 * trdUnit, "DT_EnterLong");
				//����ͻ��
				ctx->stra_log_info(fmt::format("{} ����ͻ��{}>={},��ֽ���", curCode.c_str(), highPx, upper_bound).c_str());
			}
			else if (lowPx <= lower_bound && !_isstk)
			{
				ctx->stra_set_position(curCode.c_str(), -1 * trdUnit, "DT_EnterShort");
				//����ͻ��
				ctx->stra_log_info(fmt::format("{} ����ͻ��{}<={},�ղֽ���", curCode.c_str(), lowPx, lower_bound).c_str());
			}
		}
		//else if(curPos > 0)
		else if (decimal::gt(curPos, 0))
		{
			if (lowPx <= lower_bound)
			{
				//��ֳ���
				ctx->stra_set_position(curCode.c_str(), 0, "DT_ExitLong");
				ctx->stra_log_info(fmt::format("{} ����ͻ��{}<={},��ֳ���", curCode.c_str(), lowPx, lower_bound).c_str());
			}
		}
		//else if(curPos < 0)
		else if (decimal::lt(curPos, 0))
		{
			if (highPx >= upper_bound && !_isstk)
			{
				//�ղֳ���
				ctx->stra_set_position(curCode.c_str(), 0, "DT_ExitShort");
				ctx->stra_log_info(fmt::format("{} ����ͻ��{}>={},�ղֳ���", curCode.c_str(), highPx, upper_bound).c_str());
			}
		}

		//����ͷ�һ��Ҫ��
		kline->release();
	}
}

void WtStraDtSel::on_tick(ISelStraCtx* ctx, const char* stdCode, WTSTickData* newTick)
{
}

void WtStraDtSel::on_bar(ISelStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar)
{

}
