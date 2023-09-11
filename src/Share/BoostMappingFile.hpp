/*!
 * \file BoostMappingFile.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief boost的内存映射文件组件的封装,方便使用
 */
#pragma once
#include <boost/filesystem.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

class BoostMappingFile
{
public:
	BoostMappingFile()
	{
		_file_map=NULL;
		_map_region=NULL;
	}

	~BoostMappingFile()
	{
		close();
	}

	void close()
	{
		if(_map_region!=NULL)
			delete _map_region;

		if(_file_map!=NULL)
			delete _file_map;

		_file_map=NULL;
		_map_region=NULL;
	}

	void sync()
	{
		if(_map_region)
			_map_region->flush();
	}

	void *addr()
	{
		if(_map_region)
			return _map_region->get_address();
		return NULL;
	}

	size_t size()
	{
		if(_map_region)
			return _map_region->get_size();
		return 0;
	}

	bool map(const char *filename,
		int mode=boost::interprocess::read_write,
		int mapmode=boost::interprocess::read_write,bool zeroother=true)
	{
		if (!boost::filesystem::exists(filename))
		{
			return false;
		}
		_file_name = filename;

		_file_map = new boost::interprocess::file_mapping(filename,(boost::interprocess::mode_t)mode);
		if(_file_map==NULL)
			return false;

		_map_region = new boost::interprocess::mapped_region(*_file_map,(boost::interprocess::mode_t)mapmode);
		if(_map_region==NULL)
		{
			delete _file_map;
			return false;
		}

		return true;
	}

	const char* filename()
	{
		return _file_name.c_str();
	}

	bool valid() const
	{
		return _file_map != NULL;
	}

private:
	std::string _file_name;
	boost::interprocess::file_mapping *_file_map;
	boost::interprocess::mapped_region *_map_region;
};

