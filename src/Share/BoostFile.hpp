/*!
 * \file BoostFile.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief boost库文件操作的辅助对象
 */
#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <string>

#if _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif

namespace bip = boost::interprocess;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

//struct OVERLAPPED;
//extern "C" __declspec(dllimport) int __stdcall ReadFile(void *hnd, void *buffer, unsigned long bytes_to_write,unsigned long *bytes_written, OVERLAPPED* overlapped);
class BoostFile
{
public:
	BoostFile()
	{
		_handle=bip::ipcdetail::invalid_file(); 
	}
	~BoostFile()
	{
		close_file();
	}

	bool create_new_file(const char *name, bip::mode_t mode = bip::read_write, bool temporary = false)
	{
		_handle=bip::ipcdetail::create_or_open_file(name,mode,bip::permissions(),temporary);

		if (valid())
			return truncate_file(0);
		return false;
	}

	bool create_or_open_file(const char *name, bip::mode_t mode = bip::read_write, bool temporary = false)
	{
		_handle=bip::ipcdetail::create_or_open_file(name,mode,bip::permissions(),temporary);

		return valid();
	}

	bool open_existing_file(const char *name, bip::mode_t mode = bip::read_write, bool temporary = false)
	{
		_handle=bip::ipcdetail::open_existing_file(name,mode,temporary);
		return valid();
	}

	bool is_invalid_file()
	{  
		return _handle==bip::ipcdetail::invalid_file();  
	}

	bool valid()
	{
		return _handle!=bip::ipcdetail::invalid_file();
	}

	void close_file()
	{
		if(!is_invalid_file())
		{
			bip::ipcdetail::close_file(_handle);
			_handle=bip::ipcdetail::invalid_file();
		}
	}

	bool truncate_file (std::size_t size)
	{
		return bip::ipcdetail::truncate_file(_handle,size);
	}

	bool get_file_size(bip::offset_t &size)
	{
		return bip::ipcdetail::get_file_size(_handle,size);
	}

	unsigned long long get_file_size()
	{
		bip::offset_t size=0;
		if(!get_file_size(size))
			size=0;
		return size;
	}

	static unsigned long long get_file_size(const char *name)
	{
		BoostFile bf;
		if (!bf.open_existing_file(name))
			return 0;

		auto ret = bf.get_file_size();
		bf.close_file();
		return ret;
	}

	bool set_file_pointer(bip::offset_t off, bip::file_pos_t pos)
	{
		return bip::ipcdetail::set_file_pointer(_handle,off,pos);
	}

	bool seek_to_begin(int offsize=0)
	{
		return set_file_pointer(offsize,bip::file_begin);
	}

	bool seek_current(int offsize=0)
	{
		return set_file_pointer(offsize,bip::file_current);
	}

	bool seek_to_end(int offsize=0)
	{
		return set_file_pointer(offsize,bip::file_end);
	}

	bool get_file_pointer(bip::offset_t &off)
	{
		return bip::ipcdetail::get_file_pointer(_handle,off);
	}

	unsigned long long get_file_pointer()
	{
		bip::offset_t off=0;
		if(!get_file_pointer(off))
			return 0;
		return off;
	}

	bool write_file(const void *data, std::size_t numdata)
	{
		return bip::ipcdetail::write_file(_handle,data,numdata);
	}

	bool write_file(const std::string& data)
	{
		return bip::ipcdetail::write_file(_handle, data.data(), data.size());
	}

	bool read_file(void *data, std::size_t numdata)
	{
		unsigned long readbytes = 0;
#ifdef _WIN32
		int ret = ReadFile(_handle, data, (DWORD)numdata, &readbytes, NULL);
#else
		readbytes = read(_handle, data, (std::size_t)numdata);
#endif
		return numdata == readbytes;
	}

	int read_file_length(void *data, std::size_t numdata)
	{
		unsigned long readbytes = 0;
#ifdef _WIN32
		int ret = ReadFile(_handle, data, (DWORD)numdata, &readbytes, NULL);
#else
		readbytes = read(_handle, data, (std::size_t)numdata);
#endif
		return readbytes;
	}

private:
	bip::file_handle_t _handle;

public:
	static bool delete_file(const char *name)
	{
		return bip::ipcdetail::delete_file(name);
	}

	static bool read_file_contents(const char *filename,std::string &buffer)
	{
		BoostFile bf;
		if(!bf.open_existing_file(filename,bip::read_only))
			return false;
		unsigned int filesize=(unsigned int)bf.get_file_size();
		if(filesize==0)
			return false;
		buffer.resize(filesize);
		return bf.read_file((void *)buffer.c_str(),filesize);
	}

	static bool write_file_contents(const char *filename,const void *pdata,uint32_t datalen)
	{
		BoostFile bf;
		if(!bf.create_new_file(filename))
			return false;
		return bf.write_file(pdata,datalen);
	}
};

typedef boost::shared_ptr<BoostFile> BoostFilePtr;