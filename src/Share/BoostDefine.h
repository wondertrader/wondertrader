/*!
 * \file BoostDefine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief boost相关组件的typedef，方便使用
 */
#pragma once

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

//读写锁
typedef boost::shared_mutex							BoostRWMutex;
typedef boost::shared_lock<boost::shared_mutex>		BoostReadLock;
typedef boost::unique_lock<boost::shared_mutex>		BoostWriteLock;

//简单的互斥锁
typedef boost::recursive_mutex						BoostRecursiveMutex;
typedef boost::recursive_mutex::scoped_lock			BoostScopedLock;

typedef boost::mutex								BoostUniqueMutex;

typedef boost::unique_lock<boost::mutex>			BoostUniqueLock;				

//线程
typedef boost::thread								BoostThread;
typedef boost::shared_ptr<boost::thread>			BoostThreadPtr;

typedef boost::condition_variable_any				BoostCondition;
