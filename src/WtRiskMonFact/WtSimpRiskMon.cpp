/*!
 * \file WtSimpRiskMon.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtSimpRiskMon.h"

#include "../Includes/WTSRiskDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../Share/fmtlib.h"

extern const char* FACT_NAME;

const char* WtSimpleRiskMon::getName()
{
	return "WtSimpleRiskMon";
}

const char* WtSimpleRiskMon::getFactName()
{
	return FACT_NAME;
}

void WtSimpleRiskMon::init(WtPortContext* ctx, WTSVariant* cfg)
{
	WtRiskMonitor::init(ctx, cfg);

	_calc_span = cfg->getUInt32("calc_span");
	_risk_span = cfg->getUInt32("risk_span");
	_basic_ratio = cfg->getUInt32("basic_ratio");
	_inner_day_fd = cfg->getDouble("inner_day_fd");
	_inner_day_active = cfg->getBoolean("inner_day_active");
	_multi_day_fd = cfg->getDouble("multi_day_fd");
	_multi_day_active = cfg->getBoolean("multi_day_active");
	_base_amount = cfg->getDouble("base_amount");
	_risk_scale = cfg->getDouble("risk_scale");

	ctx->writeRiskLog(fmt::format("Params inited, Checking frequency: {} s, MaxIDD: {}({:.2f}%), MaxMDD: {}({:.2f}%), Capital: {:.1f}, Profit Boudary: {:.2f}%, Calc Span: {} mins, Risk Scale: {:.2f}",
		_calc_span, _inner_day_active ? "ON" : "OFF", _inner_day_fd, _multi_day_active ? "ON" : "OFF", _multi_day_fd, _base_amount, _basic_ratio, _risk_span, _risk_scale).c_str());
}

void WtSimpleRiskMon::run()
{
	if (_thrd)
		return;

	_thrd.reset(new std::thread([this](){
		while (!_stopped)
		{
			if (_ctx && _ctx->isInTrading())
			{
				WTSPortFundInfo* fundInfo = _ctx->getFundInfo();
				const WTSFundStruct& fs = fundInfo->fundInfo();
				/*
				* ����1: �������ӵĸ����������һ�����ս���ʱ�����̼ۼƣ�, ���� 1% ����
				*		����̵Ķ�̬Ȩ�� �� ��������ʱ�Ķ�̬Ȩ��� 101%
				* ����2: 30min����, �ӽ��ոߵ�ص��� 80%����
				*		30min����, ��������Ӹߵ�ص��� 80%����
				*
				* ����: 
				* ��ʽA:  ����Ʒ�ּ��֣����ٵ� 30% ��λ��, ��һ���������°������²�λ����
				* ��ʽB:  ����ӯ��Ʒ�ֶ� ƽ��, ��һ���������°������²�λ����
				*/

				if (_inner_day_active && fs._max_dyn_bal != DBL_MAX)
				{
					double predynbal = fundInfo->predynbalance() + _base_amount;	//���ն�̬Ȩ��
					double maxBal = fs._max_dyn_bal + _base_amount;					//�������̬Ȩ��
					double curBal = fs._balance + fs._dynprofit + _base_amount;		//��ǰ��̬Ȩ��

					double rate = 0.0;
					if(!decimal::eq(maxBal, predynbal))
						rate = (maxBal - curBal) * 100 / (maxBal - predynbal);	//����ӯ���س�����

					//����������Ȩ�泬��ֹӯ�߽�����
					if (maxBal > (_basic_ratio*predynbal / 100.0))
					{
						
						/*
						 *	����Ҫת�����ڷ���������
						 *	��Ȼ�������������������������, 
						 *	���ܻ���Ϊ��;��Ϣʱ�����, �����������
						 *	���¸�����յķ���
						 */
						uint32_t maxTime = _ctx->transTimeToMin(fundInfo->max_dynbal_time());	
						uint32_t curTime = _ctx->transTimeToMin(_ctx->getCurTime());			//ת�����ڷ�����

						if (rate >= _inner_day_fd && curTime - maxTime <= _risk_span && !_limited)
						{
							_ctx->writeRiskLog(fmt::format("Current IDD {:.2f}%, ��MaxIDD {:.2f}%, Position down to {:.1f}%", rate, _inner_day_fd, _risk_scale).c_str());
							_ctx->setVolScale(_risk_scale);
							_limited = true;
						}
						else
						{
							_ctx->writeRiskLog(fmt::format("Current Balance Ratio: {:.2f}%, Current IDD: {:.2f}%", curBal*100.0 / predynbal, rate).c_str());
							//_limited = false;
						}
					}
					else
					{
						//����������Ȩ��û�г���ӯ���߽�����
						_ctx->writeRiskLog(fmt::format("Current Balance Ratio: {:.2f}%", curBal*100.0 / predynbal).c_str());
						//_limited = false;
					}
				}

				if (_multi_day_active && fs._max_md_dyn_bal._date != 0)
				{
					double maxBal = fs._max_md_dyn_bal._dyn_balance + _base_amount;
					double curBal = fs._balance + fs._dynprofit + _base_amount;

					if (curBal < maxBal)
					{
						double rate = (maxBal - curBal) * 100 / maxBal;
						if (rate >= _multi_day_fd)
						{
							_ctx->writeRiskLog(fmt::format("Current MDD {:.2f}%, >= MaxMDD {:.2f}%, Position down to 0.0%", rate, _multi_day_fd).c_str());
							_ctx->setVolScale(0.0);
						}
					}
				}
			}

			_last_time = TimeUtils::getLocalTimeNow();

			while (!_stopped)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				uint64_t now = TimeUtils::getLocalTimeNow();;
				if(now - _last_time >= _calc_span*1000)
					break;
			}
		}
	}));
}

void WtSimpleRiskMon::stop()
{
	_stopped = true;
	if (_thrd)
		_thrd->join();
}