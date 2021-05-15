#include "ExpExecuter.h"
#include "WtRtRunner.h"

extern WtRtRunner& getRunner();

void ExpExecuter::init()
{
	getRunner().executer_init(_id.c_str());
}

void ExpExecuter::set_position(const faster_hashmap<std::string, double>& targets)
{
	for(auto& v : targets)
	{
		getRunner().executer_set_position(_id.c_str(), v.first.c_str(), v.second);
	}
}

void ExpExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	getRunner().executer_set_position(_id.c_str(), stdCode, targetPos);
}
