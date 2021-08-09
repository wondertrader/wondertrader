/*!
 * \file WTSParams.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief WT参数对象定义
 */
#pragma once

#include "WTSTypes.h"
#include "WTSObject.hpp"
#include "WTSCollection.hpp"

#include <string>
#include <vector>
#include <map>


NS_OTP_BEGIN

class WTSParams : public WTSObject
{
public:
	typedef WTSArray					ChildrenArray;
	typedef WTSHashMap<std::string>		ChildrenMap;
	typedef std::vector<std::string>	MemberNames;

	typedef enum
	{
		VT_Value,
		VT_Array,
		VT_Object
	}ValueType;


protected:
	WTSParams():_type(VT_Value){}

private:
	static WTSParams* create(int32_t i32, const char* desc="")
	{
		WTSParams* ret = new WTSParams();
		char s[32] = { 0 };
		sprintf(s, "%d", i32);
		ret->_value._string = new std::string(s);
		ret->_desc = desc;
		return ret;
	}

	static WTSParams* create(uint32_t u32, const char* desc="")
	{
		WTSParams* ret = new WTSParams();
		char s[32] = { 0 };
		sprintf(s, "%u", u32);
		ret->_value._string = new std::string(s);
		ret->_desc = desc;
		return ret;
	}

	static WTSParams* create(double _real, const char* desc="")
	{
		WTSParams* ret = new WTSParams();
		char s[32] = { 0 };
		sprintf(s, "%f", _real);
		ret->_value._string = new std::string(s);
		ret->_desc = desc;
		return ret;
	}

	static WTSParams* create(const char* _string, const char* desc="")
	{
		WTSParams* ret = new WTSParams();
		ret->_value._string = new std::string(_string);
		ret->_desc = desc;
		return ret;
	}

	static WTSParams* create(bool _bool, const char* desc="")
	{
		WTSParams* ret = new WTSParams();
		ret->_value._string = new std::string(_bool?"true":"false");
		ret->_desc = desc;
		return ret;
	}

public:
	static WTSParams* createObject(const char* desc="")
	{
		WTSParams* ret = new WTSParams();
		ret->_type = VT_Object;
		ret->_value._map = ChildrenMap::create();
		ret->_desc = desc;
		return ret;
	}

	static WTSParams* createArray(const char* desc="")
	{
		WTSParams* ret = new WTSParams();
		ret->_type = VT_Array;
		ret->_value._array = ChildrenArray::create();
		ret->_desc = desc;
		return ret;
	}

	int32_t		asInt32() const
	{
		switch(_type)
		{
		case VT_Value:
			return _value._string?atoi(_value._string->c_str()):0;
		default:
			return 0;
		}
	}

	uint32_t	asUInt32() const
	{
		switch(_type)
		{
		case VT_Value:
			return _value._string?strtoul(_value._string->c_str(), NULL, 10):0;
		default:
			return 0;
		}
	}

	double		asDouble() const
	{
		switch(_type)
		{
		case VT_Value:
			return _value._string?strtod(_value._string->c_str(), NULL):0.0;
		default:
			return 0.0;
		}
	}

	std::string	asString() const
	{
		switch(_type)
		{
		case VT_Value:
			return _value._string?*_value._string:"";
		default:
			return "";
		}
	}

	const char* asCString() const
	{
		if(_type == VT_Value && _value._string != NULL)
			return _value._string->c_str();

		return "";
	}

	bool asBoolean() const
	{
		if(_type == VT_Value && _value._string)
		{
			return _value._string->compare("true") == 0;
		}

		return false;
	}

	int32_t		getInt32(const char* name) const
	{
		WTSParams* p = get(name);
		if(p)
			return p->asInt32();

		return 0;		
	}

	uint32_t	getUInt32(const char* name) const
	{
		WTSParams* p = get(name);
		if(p)
			return p->asUInt32();

		return 0;	
	}

	double		getDouble(const char* name) const
	{
		WTSParams* p = get(name);
		if(p)
			return p->asDouble();

		return 0.0;	
	}

	std::string	getString(const char* name) const
	{
		WTSParams* p = get(name);
		if(p)
			return p->asString();

		return "";	
	}

	const char* getCString(const char* name) const
	{
		WTSParams* p = get(name);
		if(p)
			return p->asCString();

		return "";	
	}

	bool getBoolean(const char* name) const
	{
		WTSParams* p = get(name);
		if(p)
			return p->asBoolean();

		return false;	
	}

	WTSParams* get(const char* name) const
	{
		if(_type != VT_Object)
			return NULL;

		if(_value._map == NULL)
			return NULL;

		WTSParams* ret = static_cast<WTSParams*>(_value._map->get(name));
		return ret;
	}

	WTSParams* get(const std::string& name) const
	{
		if(_type != VT_Object)
			return NULL;

		if(_value._map == NULL)
			return NULL;

		WTSParams* ret = static_cast<WTSParams*>(_value._map->get(name));
		return ret;
	}

	WTSParams* get(uint32_t idx) const
	{
		if(_type != VT_Array)
			return NULL;

		if(_value._array == NULL)
			return NULL;

		WTSParams* ret = static_cast<WTSParams*>(_value._array->at(idx));
		return ret;
	}

	bool append(const char* _name, const char* _string, const char* desc="")
	{
		if(_type != VT_Object)
			return false;

		if(_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		WTSParams* item = WTSParams::create(_string, desc);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	bool append(const char* _name, int32_t _i32, const char* desc="")
	{
		if(_type != VT_Object)
			return false;

		if(_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		WTSParams* item = WTSParams::create(_i32, desc);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	bool append(const char* _name, uint32_t _u32, const char* desc="")
	{
		if(_type != VT_Object)
			return false;

		if(_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		WTSParams* item = WTSParams::create(_u32, desc);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	bool append(const char* _name, double _real, const char* desc="")
	{
		if(_type != VT_Object)
			return false;

		if(_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		WTSParams* item = WTSParams::create(_real, desc);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	bool append(const char* _name, bool _bool, const char* desc="")
	{
		if(_type != VT_Object)
			return false;

		if(_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		WTSParams* item = WTSParams::create(_bool, desc);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	bool append(const char* _name, WTSParams *item, bool bAutoRetain = true)
	{
		if(_type != VT_Object || NULL == item)
			return false;

		if(_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		_value._map->add(_name, item, bAutoRetain);

		return true;
	}

	bool append(int32_t _i32, const char* desc="")
	{
		if(_type != VT_Array )
			return false;

		if(_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		WTSParams* item = WTSParams::create(_i32, desc);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	bool append(uint32_t _u32, const char* desc="")
	{
		if(_type != VT_Array )
			return false;

		if(_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		WTSParams* item = WTSParams::create(_u32, desc);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	bool append(double _real, const char* desc="")
	{
		if(_type != VT_Array )
			return false;

		if(_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		WTSParams* item = WTSParams::create(_real, desc);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	bool append(bool _bool, const char* desc="")
	{
		if(_type != VT_Array )
			return false;

		if(_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		WTSParams* item = WTSParams::create(_bool, desc);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	bool append( WTSParams *item, bool bAutoRetain = true)
	{
		if(_type != VT_Array || NULL == item)
			return false;

		if(_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		_value._array->append(item, bAutoRetain);

		return true;
	}

	uint32_t size() const
	{
		if(_type == VT_Value)
			return 0;
		else if(_type == VT_Array)
		{
			return (_value._array == NULL)?0:_value._array->size();
		}
		else
		{
			return (_value._map == NULL)?0:_value._map->size();
		}
	}

	MemberNames memberNames() const
	{
		MemberNames names;
		if(_type == VT_Object && _value._map != NULL)
		{
			auto it = _value._map->begin();
			for( ; it != _value._map->end(); it++)
			{
				names.emplace_back(it->first);
			}
		}

		return names;
	}

	virtual void release()
	{
		if(isSingleRefs())
		{
			switch(_type)
			{
			case VT_Array:
				if(NULL != _value._array)
				{
					_value._array->release();
				}
				break;
			case VT_Object:
				if(NULL != _value._map)
				{
					_value._map->release();
				}
				break;
			case VT_Value:
				if(NULL != _value._string)
				{
					delete _value._string;
				}
				break;
			}
		}
		WTSObject::release();
	}

	inline void setDescription(const char* desc){_desc = desc;}
	inline const char* description() const{return _desc.c_str();}

private:	
	union ValueHolder
	{
		std::string*	_string;

		ChildrenMap*	_map;
		ChildrenArray*	_array;
	};
	std::string	_desc;
	ValueHolder	_value;
	ValueType	_type;
};

NS_OTP_END