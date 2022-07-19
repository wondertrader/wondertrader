#include "IndexWorker.h"
#include "IndexFactory.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"

#include "../Share/CodeHelper.hpp"
#include "../Share/decimal.h"
#include "../Share/TimeUtils.hpp"

#include "../WTSTools/WTSLogger.h"

const char* WEIGHT_ALGS[] = 
{
	"Fixed",
	"DynamicInterest",
	"DynamicVolume"
};

bool IndexWorker::init(WTSVariant* config)
{
	if (config == NULL)
		return false;

	_exchg = config->getCString("exchg");
	_code = config->getCString("code");

	_cInfo = _factor->get_bd_mgr()->getContract(_code.c_str(), _exchg.c_str());

	_trigger = config->getCString("trigger");
	_timeout = config->getUInt32("timeout");

	_stand_scale = config->getDouble("stand_scale");	//��׼��ϵ���������ֵΪ3000����׼���Ժ�Ϊ1000�����׼��ϵ��Ϊ0.333333
	if (decimal::eq(_stand_scale, 0.0))
		_stand_scale = 1.0;

	//�����������Լ��Ҫת�ɷ��º�Լ����Ϊ��ʵʱ�����
	IHotMgr* hotMgr = _factor->get_hot_mgr();

	if (_trigger != "time")
	{
		CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(_trigger.c_str(), hotMgr);
		if (strlen(cInfo._ruletag) > 0)
			_trigger = fmt::format("{}.{}", cInfo._exchg, hotMgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID()));
	}

	//Ȩ���㷨
	_weight_alg = config->getUInt32("weight_alg");

	WTSVariant* cfgComms = config->get("commodities");
	WTSVariant* cfgCodes = config->get("codes");
	if (cfgComms != NULL && cfgComms->size() > 0)
	{
		IBaseDataMgr* bdMgr = _factor->get_bd_mgr();

		std::size_t cnt = cfgComms->size();
		for (std::size_t i = 0; i < cnt; i++)
		{
			WTSVariant* cfgItem = cfgComms->get(i);
			std::string fullPid;
			double weight = 1.0;

			//����Ƕ��������scale����ΪȨ������ϵ����������Ҫ���ڶ�Ʒ�ְ�飬��ͬƷ��Ȩ�ز�ͬ
			if (cfgItem->isObject())
			{
				fullPid = cfgItem->getCString("code");
				weight = cfgItem->getDouble("weight");
				if (decimal::eq(weight, 0.0))
					weight = 1.0;
			}
			else
			{
				fullPid = cfgItem->asCString();
			}

			//ͨ��Ʒ�ִ����ҵ���Ӧ�ĺ�Լ�б�������
			WTSCommodityInfo* commInfo = bdMgr->getCommodity(fullPid.c_str());
			if (commInfo == NULL)
				continue;

			const auto& codes = commInfo->getCodes();
			for (const auto& c : codes)
			{
				std::string fullCode = fmt::format("{}.{}", commInfo->getExchg(), c.c_str());
				WeightFactor& wFactor = _weight_scales[fullCode];
				wFactor._weight = weight;

				//���ĵ�ʱ���ȡ���Ŀ��գ���Ϊ��������
				WTSTickData* lastTick = _factor->sub_ticks(fullCode.c_str());
				if (lastTick)
				{
					memcpy(&wFactor._tick, &lastTick->getTickStruct(), sizeof(WTSTickStruct));
					lastTick->release();
				}

				WTSLogger::info("Consist {} of block index {}.{} subscribed", fullCode, _exchg, _code);
			}
		}
	}
	else if (cfgCodes != NULL && cfgCodes->size() > 0)
	{
		IBaseDataMgr* bdMgr = _factor->get_bd_mgr();
		IHotMgr* hotMgr = _factor->get_hot_mgr();

		std::size_t cnt = cfgCodes->size();
		for (std::size_t i = 0; i < cnt; i++)
		{
			WTSVariant* cfgItem = cfgCodes->get(i);
			std::string fullCode;
			double weight = 1.0;

			//����Ƕ��������scale����ΪȨ������ϵ����������Ҫ���ڶ�Ʒ�ְ�飬��ͬƷ��Ȩ�ز�ͬ
			if (cfgItem->isObject())
			{
				std::string fullPid = cfgItem->getCString("code");
				weight = cfgItem->getDouble("weight");
				if (decimal::eq(weight, 0.0))
					weight = 1.0;
			}
			else
			{
				fullCode = cfgItem->asCString();
			}

			auto ay = StrUtil::split(fullCode, ".");
			std::string exchg = ay[0];
			std::string code;
			if(ay.size() == 2)
			{
				//����fullcode��ʽ����CFFEX.IF2202
				code = ay[1];
			}
			else
			{
				//����2������stdCode��ʽ����Ҫ����CFFEX.IF.HOT
				CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(fullCode.c_str(), hotMgr);
				if (strlen(cInfo._ruletag) > 0)
				{
					code = hotMgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID());
					WTSLogger::info("{} contract confirmed: {} -> {}.{}", cInfo._ruletag, fullCode, cInfo._exchg, code);
				}
				else
				{
					code = cInfo._code;
				}
				fullCode = fmt::format("{}.{}", cInfo._exchg, code);
				
			}

			WTSContractInfo* cInfo = _factor->get_bd_mgr()->getContract(code.c_str(), exchg.c_str());
			if(cInfo == NULL)
			{
				WTSLogger::error("Consist {} of block index {}.{} not exists", fullCode, _exchg, _code);
				continue;
			}

			WeightFactor& wFactor = _weight_scales[fullCode];
			wFactor._weight = weight;

			//���ĵ�ʱ���ȡ���Ŀ��գ���Ϊ��������
			WTSTickData* lastTick = _factor->sub_ticks(fullCode.c_str());
			if (lastTick)
			{
				memcpy(&wFactor._tick, &lastTick->getTickStruct(), sizeof(WTSTickStruct));
				lastTick->release();
			}

			WTSLogger::info("Consist {} of block index {}.{} subscribed", fullCode, _exchg, _code);
		}
	}

	WTSLogger::info("Block index {}.{} initialized��weight algorithm: {}, trigger: {}, timeout: {}", _exchg, _code, WEIGHT_ALGS[_weight_alg], _trigger, _timeout);

	return true;
}

void IndexWorker::handle_quote(WTSTickData* newTick)
{
	const char* fullCode = newTick->getContractInfo()->getFullCode();

	{
		SpinLock lock(_mtx_data);
		auto it = _weight_scales.find(fullCode);
		if (it == _weight_scales.end())
			return;

		WeightFactor& wFactor = (WeightFactor&)it->second;
		memcpy(&wFactor._tick, &newTick->getTickStruct(), sizeof(WTSTickStruct));
	}

	//���ʹ��time����ô����һ���ɷֺ�Լ����������Ժ󣬻�ȥ����ָ������ʱ��
	if(_trigger != "time" && _trigger.compare(fullCode) != 0)
		return;

	//�����_trigger����ʼ׼��������
	if(_timeout == 0)
	{
		//���û�����ó�ʱʱ�䣬��ֱ������ָ��
		generate_tick();
	}
	else
	{
		if(_thrd_trigger == NULL)
		{
			_thrd_trigger.reset(new StdThread([this]() {
				while(!_stopped)
				{
					while(!_process)
					{
						StdUniqueLock lck(_mtx_trigger);
						_cond_trigger.wait(_mtx_trigger);
					}

					//�ȴ�һ��ʱ��
					do
					{
						uint64_t now = TimeUtils::getLocalTimeNow();
						if(now >= _recalc_time)
							break;

						std::this_thread::sleep_for(std::chrono::milliseconds(5));
					} while (true);

					//��ʼ����ָ��
					generate_tick();
					_process = false;
				}
			}));
		}

		if(!_process)
		{
			_process = true;
			_recalc_time = TimeUtils::getLocalTimeNow() + _timeout;
			_cond_trigger.notify_all();
		}
	}
}

void IndexWorker::generate_tick()
{
	//Ȼ��ʼ����ָ��
	double total_base = 0.0;	//Ȩ�ػ���
	double total_value = 0.0;	//��ֵ�ۼ�
	double total_vol = 0.0;		//ָ���ܳɽ���
	double total_amt = 0.0;		//ָ���ܳɽ���
	double total_hold = 0.0;	//ָ���ܳ�
	uint64_t maxTime = 0;		//���һ��tick��ʱ��
	uint32_t tDate = 0;			//������

	double total_weight = 0;

	{
		//�Ȱ�������ס
		SpinLock lock(_mtx_data);
		for (const auto& v : _weight_scales)
		{
			//������ݲ�ȫ��ֱ���˳�
			const WeightFactor& wFactor = v.second;
			if (wFactor._tick.action_date == 0)
				return;

			uint64_t curTime = TimeUtils::makeTime(wFactor._tick.action_date, wFactor._tick.action_time);
			maxTime = std::max(maxTime, curTime);
			tDate = std::max(tDate, wFactor._tick.trading_date);

			total_vol += wFactor._tick.total_volume;
			total_amt += wFactor._tick.total_turnover;
			total_hold += wFactor._tick.open_interest;

			total_weight += wFactor._weight;

			switch (_weight_alg)
			{
			case 0://�̶�Ȩ�أ�ֻ�������weight
				total_base = 1;	//��Ϊֻ������weight������Ȩ�ػ���Ϊ1
				total_value += wFactor._tick.price * wFactor._weight;
				break;
			case 1:	//��̬�ܳ�
				total_base += wFactor._tick.open_interest;	//��̬�ܳ�Ϊ��ǰ�ܳ�
				total_value += wFactor._tick.open_interest * wFactor._tick.price * wFactor._weight;
				break;
			case 2:	//��̬�ɽ���
				total_base += wFactor._tick.total_volume;	//��̬�ɽ���
				total_value += wFactor._tick.total_volume * wFactor._tick.price * wFactor._weight;
				break;
			default:
				break;
			}
		}
	}

	//��������׼��
	double index = total_value / total_base / total_weight * _stand_scale;

	//ʱ����һ������
	maxTime += _timeout;
	TimeUtils::Time32 tm32(maxTime);

	if(_cache.action_time == 0)
	{
		strcpy(_cache.exchg, _exchg.c_str());
		strcpy(_cache.code, _code.c_str());
		_cache.trading_date = tDate;

		_cache.price = index;
		_cache.open = index;
		_cache.high = index;
		_cache.low = index;		

		_cache.action_date = tm32.date();
		_cache.action_time = tm32.time_ms();
		
		_cache.total_volume = total_vol;
		_cache.open_interest = total_hold;
		_cache.total_turnover = total_amt;
	}
	else
	{
		_cache.price = index;
		_cache.high = std::max(_cache.high, index);
		_cache.low = std::min(_cache.low, index);

		_cache.action_date = tm32.date();
		_cache.action_time = tm32.time_ms();

		_cache.total_volume = total_vol;
		_cache.open_interest = total_hold;
		_cache.total_turnover = total_amt;
	}

	WTSTickData *newTick = WTSTickData::create(_cache);
	newTick->setContractInfo(_cInfo);
	_factor->push_tick(newTick);
	WTSLogger::debug("{}.{} - {}.{} - {}", _cache.exchg, _cache.code, _cache.action_date, _cache.action_time, _cache.price);
	newTick->release();
}