/*!
 * \file IniHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Ini文件辅助类,利用boost的property_tree来实现,可以跨平台使用
 */
#pragma once

#include <string>
#include <vector>
#include <map>

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>

typedef std::vector<std::string>			FieldArray;
typedef std::map<std::string, std::string>	FieldMap;

class IniHelper
{
private:
	boost::property_tree::ptree	_root;
	std::string					_fname;
	bool						_loaded;

	static const uint32_t MAX_KEY_LENGTH = 64;

public:
	IniHelper(): _loaded(false){}

	void	load(const char* szFile)
	{
		_fname = szFile;
		try
		{
			boost::property_tree::ini_parser::read_ini(szFile, _root);
		}
		catch(...)
		{

		}
		
		_loaded = true;
	}

	void	save(const char* filename = "")
	{
		if (strlen(filename) > 0)
			boost::property_tree::ini_parser::write_ini(filename, _root);
		else
			boost::property_tree::ini_parser::write_ini(_fname.c_str(), _root);
	}

	inline bool isLoaded() const{ return _loaded; }

public:
	void	removeValue(const char* szSec, const char* szKey)
	{
		try
		{
			boost::property_tree::ptree& sec = _root.get_child(szSec);
			sec.erase(szKey);
		}
		catch (...)
		{
			
		}
	}

	void	removeSection(const char* szSec)
	{
		try
		{
			_root.erase(szSec);
		}
		catch (...)
		{

		}
	}

	template<class T>
	T	readValue(const char* szPath, T defVal)
	{
		try
		{
			return _root.get<T>(szPath, defVal);
		}
		catch (...)
		{
			return defVal;
		}
	}

	std::string	readString(const char* szSec, const char* szKey, const char* defVal = "")
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		return readValue<std::string>(path, defVal);
	}

	int			readInt(const char* szSec, const char* szKey, int defVal = 0)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		return readValue<int>(path, defVal);
	}

	uint32_t	readUInt(const char* szSec, const char* szKey, uint32_t defVal = 0)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		return readValue<uint32_t>(path, defVal);
	}

	bool		readBool(const char* szSec, const char* szKey, bool defVal = false)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		return readValue<bool>(path, defVal);
	}

	double		readDouble(const char* szSec, const char* szKey, double defVal = 0.0)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		return readValue<double>(path, defVal);
	}

	int			readSections(FieldArray &aySection)
	{
		for (auto it = _root.begin(); it != _root.end(); it++)
		{
			aySection.emplace_back(it->first.data());
		}

		return (int)_root.size();
	}

	int			readSecKeyArray(const char* szSec, FieldArray &ayKey)
	{
		try
		{
			const boost::property_tree::ptree& _sec = _root.get_child(szSec);
			for (auto it = _sec.begin(); it != _sec.end(); it++)
			{
				ayKey.emplace_back(it->first.data());
			}

			return (int)_sec.size();
		}
		catch (...)
		{
			return 0;
		}
		
	}

	int			readSecKeyValArray(const char* szSec, FieldArray &ayKey, FieldArray &ayVal)
	{
		try
		{
			const boost::property_tree::ptree& _sec = _root.get_child(szSec);
			for (auto it = _sec.begin(); it != _sec.end(); it++)
			{
				ayKey.emplace_back(it->first.data());
				ayVal.emplace_back(it->second.data());
			}

			return (int)_sec.size();
		}
		catch (...)
		{
			return 0;
		}
	}

	template<class T>
	void		writeValue(const char* szPath, T val)
	{
		_root.put<T>(szPath, val);
	}

	void		writeString(const char* szSec, const char* szKey, const char* val)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		writeValue<std::string>(path, val);
	}

	void		writeInt(const char* szSec, const char* szKey, int val)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		writeValue<int>(path, val);
	}

	void		writeUInt(const char* szSec, const char* szKey, uint32_t val)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		writeValue<uint32_t>(path, val);
	}

	void		writeBool(const char* szSec, const char* szKey, bool val)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		writeValue<bool>(path, val);
	}

	void		writeDouble(const char* szSec, const char* szKey, double val)
	{
		static char path[MAX_KEY_LENGTH] = { 0 };
		sprintf(path, "%s.%s", szSec, szKey);
		writeValue<double>(path, val);
	}
};