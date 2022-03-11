#include "ExpExecuter.h"
#include "WtRtRunner.h"

extern WtRtRunner& getRunner();

void ExpExecuter::init()
{
	getRunner().executer_init(name());
}

void ExpExecuter::set_position(const faster_hashmap<LongKey, double>& targets)
{
	for(auto& v : targets)
	{
		getRunner().executer_set_position(name(), v.first.c_str(), v.second);
	}
}

void ExpExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	getRunner().executer_set_position(name(), stdCode, targetPos);
}
