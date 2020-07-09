/*!
 * \file JsonToVariant.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief rapidjson×ªWTSVariantµÄ¸¨Öúº¯Êý
 */
#pragma once

#include "../Includes/WTSVariant.hpp"
#include <rapidjson/document.h>

namespace rj = rapidjson;

USING_NS_OTP;

bool static jsonToVariant(const rj::Value& root, WTSVariant* params)
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
				if (jsonToVariant(item, subObj))
					params->append(key, subObj, false);
			}
			break;
			case rj::kArrayType:
			{
				WTSVariant* subAy = WTSVariant::createArray();
				if (jsonToVariant(item, subAy))
					params->append(key, subAy, false);
			}
			break;
			case rj::kNumberType:
				if(item.IsInt())
					params->append(key, item.GetInt());
				else if (item.IsUint())
					params->append(key, item.GetUint());
				else if (item.IsInt64())
					params->append(key, item.GetInt64());
				else if (item.IsUint64())
					params->append(key, item.GetUint64());
				else if(item.IsDouble())
					params->append(key, item.GetDouble());
				break;
			case rj::kStringType:
				params->append(key, item.GetString());
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
				if (jsonToVariant(item, subObj))
					params->append(subObj, false);
			}
			break;
			case rj::kArrayType:
			{
				WTSVariant* subAy = WTSVariant::createArray();
				if (jsonToVariant(item, subAy))
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
				params->append(item.GetString());
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


