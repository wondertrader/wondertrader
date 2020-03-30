/*!
 * \file WTSParamParser.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WTSParamParser.h"
#include "../Share/WTSParams.hpp"


WTSParams* WTSParamParser::parseFromXML(const pugi::xml_node& root)
{
	const pugi::xml_node &nodeParams = root;
	if(!nodeParams)
		return NULL;

	WTSParams* ret =WTSParams::createObject();

	pugi::xml_node param = nodeParams.first_child();
	while(param)
	{
		if(strcmp(param.name(), "Param") == 0)
		{
			ret->append(param.attribute("name").value(), param.attribute("value").value(), param.attribute("desc").value());
		}
		else if(strcmp(param.name(), "Params") == 0)
		{
			ret->append(param.attribute("name").value(), parseFromXML(param), false);
		}
		param = param.next_sibling();
	}


	return ret;
}