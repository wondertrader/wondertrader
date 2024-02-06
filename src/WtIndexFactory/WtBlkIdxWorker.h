#pragma once
#include "../Includes/IIndexDefs.h"

#include "../Includes/WTSStruct.h"
#include "../Includes/FasterDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/SpinMutex.hpp"

NS_WTP_BEGIN
class WTSVariant;
class WTSTickData;
class IHotMgr;
class IBaseDataMgr;
class WTSContractInfo;

class WtBlkIdxWorker : public IIndexWorker
{
public:
	WtBlkIdxWorker(const char* id):IIndexWorker(id), _stopped(false), _process(false) {}

public:
	virtual bool		init(WTSVariant* config) override;
	virtual void		handle_quote(WTSTickData* newTick) override;
	virtual const char* get_fact_name() override;

private:
	void	generate_tick();

protected:
	std::string		_exchg;
	std::string		_code;
	std::string		_trigger;
	uint32_t		_timeout;
	uint64_t		_recalc_time;
	double			_stand_scale;
	WTSTickStruct	_cache;
	WTSContractInfo*	_cInfo;

	typedef struct _WeightFactor
	{
		double			_weight;
		WTSTickStruct	_tick;
		_WeightFactor()
		{
			memset(this, 0, sizeof(_WeightFactor));
		}
	}WeightFactor;
	SpinMutex	_mtx_data;
	wt_hashmap<std::string, WeightFactor>	_weight_scales;
	uint32_t	_weight_alg;

	StdThreadPtr	_thrd_trigger;
	StdUniqueMutex	_mtx_trigger;
	StdCondVariable	_cond_trigger;
	bool			_stopped;
	bool			_process;
};

NS_WTP_END