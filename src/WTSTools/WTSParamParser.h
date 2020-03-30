#pragma once
#include "../WTSUtils/pugixml/pugixml.hpp"

#include "../Share/WTSMarcos.h"


NS_OTP_BEGIN
	class WTSParams;
NS_OTP_END

USING_NS_OTP;

class WTSParamParser
{
public:
	static WTSParams* parseFromXML(const pugi::xml_node& root);

};

