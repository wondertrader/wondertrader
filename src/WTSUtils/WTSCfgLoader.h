#pragma once
#include "../Includes/WTSMarcos.h"

#include <string>

NS_WTP_BEGIN
class WTSVariant;
NS_WTP_END

USING_NS_WTP;

class WTSCfgLoader
{
	static WTSVariant*	load_from_json(const char* content);
	static WTSVariant*	load_from_yaml(const char* content);

public:
	static WTSVariant*	load_from_file(const char* filename);
	static WTSVariant*	load_from_content(const std::string& content, bool isYaml = false);

	static WTSVariant*	load_from_file(const std::string& filename)
	{
		return load_from_file(filename.c_str());
	}
};

