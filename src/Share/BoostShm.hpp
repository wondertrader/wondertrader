#pragma once

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

class BoostShm
{
private:
	std::string	_name;
	boost::interprocess::shared_memory_object*	_obj;
	boost::interprocess::mapped_region *		_region;

public:
	BoostShm(): _obj(nullptr), _region(nullptr){}

	~BoostShm()
	{
		close();
	}

	void close()
	{
		if (_region)
			delete _region;

		if (_obj)
			delete _obj;

		_obj = nullptr;
		_region = nullptr;
	}

	bool open(const char* name)
	{
		try
		{
			_obj = new boost::interprocess::shared_memory_object(boost::interprocess::open_only, name, boost::interprocess::read_write);
			_region = new boost::interprocess::mapped_region(*_obj, boost::interprocess::read_write);

			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	bool create(const char* name, std::size_t size)
	{
		try
		{
			boost::interprocess::shared_memory_object::remove(name);
			_obj = new boost::interprocess::shared_memory_object(boost::interprocess::create_only, name, boost::interprocess::read_write);
			_obj->truncate(size);
			_region = new boost::interprocess::mapped_region(*_obj, boost::interprocess::read_write);

			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	inline void *addr()
	{
		if (_region)
			return _region->get_address();
		return nullptr;
	}

	inline size_t size()
	{
		if (_region)
			return _region->get_size();
		return 0;
	}

	inline bool valid() const
	{
		return _obj != nullptr;
	}
};