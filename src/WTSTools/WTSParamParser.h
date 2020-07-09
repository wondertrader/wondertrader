/*!
 * \file WTSParamParser.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 参数解析器定义
 */
#pragma once
#include "../WTSUtils/pugixml/pugixml.hpp"

#include "../Includes/WTSMarcos.h"


NS_OTP_BEGIN
	class WTSParams;
NS_OTP_END

USING_NS_OTP;

class WTSParamParser
{
public:
	static WTSParams* parseFromXML(const pugi::xml_node& root);

};

