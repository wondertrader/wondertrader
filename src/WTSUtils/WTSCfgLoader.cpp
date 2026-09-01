#include "WTSCfgLoader.h"
#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include "../Share/charconv.hpp"

#include "../Includes/WTSVariant.hpp"
#include <rapidjson/document.h>
namespace rj = rapidjson;

//Win下窄字符文件接口(std::filesystem/ifstream等)按ANSI代码页解释char*,
//而Python端经ctypes传入的配置内容(json的\uXXXX转义、yaml原文)最终解析出的是UTF-8编码
//的字符串值, 含中文时必须转为本地ANSI编码, 否则下游所有以"文件路径"形态消费该值的
//调用方(loadSessions/loadCommodities等的exists检查与打开文件)均会因乱码路径失败。
//纯ASCII内容不受影响; 已是ANSI编码的字节序列不满足合法UTF-8特征, 同样跳过转换。
static std::string to_local_str(const std::string& src)
{
#ifdef _WIN32
	if (!src.empty())
	{
		UTF8toChar conv(src);
		return std::string(conv.c_str());
	}
#endif
	return src;
}


bool json_to_variant(const rj::Value& root, WTSVariant* params)
{
	if (root.IsObject() && params->type() != WTSVariant::VT_Object)
		return false;

	if (root.IsArray() && params->type() != WTSVariant::VT_Array)
		return false;

	if (root.IsObject())
	{
		for (auto& m : root.GetObject())
		{
			const char* key = m.name.GetString();
			const rj::Value& item = m.value;
			switch (item.GetType())
			{
			case rj::kObjectType:
			{
				WTSVariant* subObj = WTSVariant::createObject();
				if (json_to_variant(item, subObj))
					params->append(key, subObj, false);
			}
			break;
			case rj::kArrayType:
			{
				WTSVariant* subAy = WTSVariant::createArray();
				if (json_to_variant(item, subAy))
					params->append(key, subAy, false);
			}
			break;
			case rj::kNumberType:
				if (item.IsInt())
					params->append(key, item.GetInt());
				else if (item.IsUint())
					params->append(key, item.GetUint());
				else if (item.IsInt64())
					params->append(key, item.GetInt64());
				else if (item.IsUint64())
					params->append(key, item.GetUint64());
				else if (item.IsDouble())
					params->append(key, item.GetDouble());
				break;
			case rj::kStringType:
			{
				std::string val = to_local_str(item.GetString());
				params->append(key, val.c_str());
			}
			break;
			case rj::kTrueType:
			case rj::kFalseType:
				params->append(key, item.GetBool());
				break;

			}
		}
	}
	else
	{
		for (auto& item : root.GetArray())
		{
			switch (item.GetType())
			{
			case rj::kObjectType:
			{
				WTSVariant* subObj = WTSVariant::createObject();
				if (json_to_variant(item, subObj))
					params->append(subObj, false);
			}
			break;
			case rj::kArrayType:
			{
				WTSVariant* subAy = WTSVariant::createArray();
				if (json_to_variant(item, subAy))
					params->append(subAy, false);
			}
			break;
			case rj::kNumberType:
				if (item.IsInt())
					params->append(item.GetInt());
				else if (item.IsUint())
					params->append(item.GetUint());
				else if (item.IsInt64())
					params->append(item.GetInt64());
				else if (item.IsUint64())
					params->append(item.GetUint64());
				else if (item.IsDouble())
					params->append(item.GetDouble());
				break;
			case rj::kStringType:
			{
				std::string val = to_local_str(item.GetString());
				params->append(val.c_str());
			}
			break;
			case rj::kTrueType:
			case rj::kFalseType:
				params->append(item.GetBool());
				break;
			}
		}
	}
	return true;
}

WTSVariant* WTSCfgLoader::load_from_json(const char* content)
{
	rj::Document root;
	root.Parse(content);

	if (root.HasParseError())
		return NULL;

	WTSVariant* ret = WTSVariant::createObject();
	if (!json_to_variant(root, ret))
	{
		ret->release();
		return NULL;
	}

	return ret;
}

#include "../WTSUtils/yamlcpp/yaml.h"
bool yaml_to_variant(const YAML::Node& root, WTSVariant* params)
{
	if (root.IsNull() && params->type() != WTSVariant::VT_Object)
		return false;

	if (root.IsSequence() && params->type() != WTSVariant::VT_Array)
		return false;

	bool isMap = root.IsMap();
	for (auto& m : root)
	{
		std::string key = isMap ? m.first.as<std::string>() : "";
		const YAML::Node& item = isMap ? m.second : m;
		switch (item.Type())
		{
		case YAML::NodeType::Map:
		{
			WTSVariant* subObj = WTSVariant::createObject();
			if (yaml_to_variant(item, subObj))
			{
				if(isMap)
					params->append(key.c_str(), subObj, false);
				else
					params->append(subObj, false);
			}
		}
		break;
		case YAML::NodeType::Sequence:
		{
			WTSVariant* subAy = WTSVariant::createArray();
			if (yaml_to_variant(item, subAy))
			{
				if (isMap)
					params->append(key.c_str(), subAy, false);
				else
					params->append(subAy, false);
			}
		}
		break;
		case YAML::NodeType::Scalar:
			if (isMap)
			{
				std::string val = to_local_str(item.as<std::string>());
				params->append(key.c_str(), val.c_str());
			}
			else
			{
				std::string val = to_local_str(item.as<std::string>());
				params->append(val.c_str());
			}
			break;
		}
	}

	return true;
}

WTSVariant* WTSCfgLoader::load_from_yaml(const char* content)
{
	YAML::Node root = YAML::Load(content);

	if (root.IsNull())
		return NULL;

	WTSVariant* ret = WTSVariant::createObject();
	if (!yaml_to_variant(root, ret))
	{
		ret->release();
		return NULL;
	}

	return ret;
}

WTSVariant* WTSCfgLoader::load_from_content(const std::string& content, bool isYaml /* = false */)
{
	//加一个自动检测编码的逻辑
	bool isUTF8 = EncodingHelper::isUtf8((unsigned char*)content.data(), content.size());

	std::string buffer;
	//Linux下得是UTF8
	//Win下得是GBK
#ifdef _WIN32
	if (isUTF8)
		buffer = UTF8toChar(content);
#else
	if (!isUTF8)
		buffer = ChartoUTF8(content);
#endif

	if (buffer.empty())
		buffer = content;

	if (isYaml)
		return load_from_yaml(buffer.c_str());
	else
		return load_from_json(buffer.c_str());
}

WTSVariant* WTSCfgLoader::load_from_file(const char* filename)
{
	//Win下窄字符文件接口按ANSI编码解释路径, 外部传入的配置文件路径常为UTF-8编码,
	//含中文时需先转为本地ANSI; 纯ASCII或已是ANSI编码的路径不做转换
	std::string raw_name;
	UTF8toChar name_conv("");
	const char* real_path = filename;
#ifdef _WIN32
	raw_name = filename;
	if (EncodingHelper::isUtf8((unsigned char*)raw_name.data(), raw_name.size()))
	{
		name_conv.init(raw_name.c_str());
		real_path = name_conv.c_str();
	}
#endif

	if (!StdFile::exists(real_path))
		return NULL;

	std::string content;
	StdFile::read_file_content(real_path, content);
	if (content.empty())
		return NULL;

	//加一个自动检测编码的逻辑
	bool isUTF8 = EncodingHelper::isUtf8((unsigned char*)content.data(), content.size());

	//By Wesley @ 2022.01.07
	//Linux下得是UTF8
	//Win下得是GBK
#ifdef _WIN32
	if(isUTF8)
		content = UTF8toChar(content);
#else
	if (!isUTF8)
		content = ChartoUTF8(content);
#endif

	if (StrUtil::endsWith(filename, ".json"))
		return load_from_json(content.c_str());
	else if (StrUtil::endsWith(filename, ".yaml") || StrUtil::endsWith(filename, ".yml"))
		return load_from_yaml(content.c_str());

	return NULL;
}
