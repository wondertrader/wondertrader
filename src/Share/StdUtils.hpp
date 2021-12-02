/*!
 * \file StdUtils.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief C++标准库一些定义的简单封装,方便调用
 */
#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include <string>

#if _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif

//////////////////////////////////////////////////////////////////////////
//std线程类
typedef std::thread StdThread;
typedef std::shared_ptr<StdThread> StdThreadPtr;

//////////////////////////////////////////////////////////////////////////
//std互斥量和锁
typedef std::recursive_mutex	StdRecurMutex;
typedef std::mutex				StdUniqueMutex;
typedef std::condition_variable_any	StdCondVariable;

typedef std::unique_lock<StdUniqueMutex>	StdUniqueLock;

template<typename T>
class StdLocker
{
public:
	StdLocker(T& mtx)
	{
		mtx.lock();
		_mtx = &mtx;
	}

	~StdLocker(){
		_mtx->unlock();
	}

private:
	T* _mtx;
};

//////////////////////////////////////////////////////////////////////////
//文件辅助类
class StdFile
{
public:
	static inline uint64_t read_file_content(const char* filename, std::string& content)
	{
		FILE* f = fopen(filename, "rb");
		fseek(f, 0, SEEK_END);
		uint32_t length = ftell(f);
		content.resize(length);   // allocate memory for a buffer of appropriate dimension
		fseek(f, 0, 0);
		fread((void*)content.data(), sizeof(char), length, f);
		fclose(f);
		return length;
	}

	static inline void write_file_content(const char* filename, const std::string& content)
	{
		FILE* f = fopen(filename, "wb");
		fwrite((void*)content.data(), sizeof(char), content.size(), f);
		fclose(f);
	}

	static inline void write_file_content(const char* filename, const void* data, std::size_t length)
	{
		FILE* f = fopen(filename, "wb");
		fwrite(data, sizeof(char), length, f);
		fclose(f);
	}

	static inline bool exists(const char* filename)
	{
#if _WIN32
		int ret = _access(filename, 0);
#else
		int ret = access(filename, 0);
#endif
		return ret == 0;
	}
};
