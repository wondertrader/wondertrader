/*!
 * \file WTSCollection.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt集合组件定义文件
 */
#pragma once
#include "WTSObject.hpp"
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include "../FasterLibs/tsl/robin_map.h"

#include <deque>

NS_WTP_BEGIN

//////////////////////////////////////////////////////////////////////////
//WTSArray

/*
 *	平台数组容器
 *	内部使用vector实现
 *	数据使用WTSObject指针对象
 *	所有WTSObject的派生类都可以使用
 *	用于平台内使用
 */
class WTSArray : public WTSObject
{
public:
	/*
	 *	数组迭代器
	 */
	typedef std::vector<WTSObject*>::iterator Iterator;
	typedef std::vector<WTSObject*>::const_iterator ConstIterator;

	typedef std::vector<WTSObject*>::reverse_iterator ReverseIterator;
	typedef std::vector<WTSObject*>::const_reverse_iterator ConstReverseIterator;

	typedef std::function<bool(WTSObject*, WTSObject*)>	SortFunc;

	/*
	 *	创建数组对象
	 */
	static WTSArray* create()
	{
		WTSArray* pRet = new WTSArray();
		return pRet;
	}

	/*
	 *	读取数组长度
	 */
	uint32_t size() const{ return (uint32_t)_vec.size(); }

	/*
	 *	清空数组,并重新分配空间
	 *	调用该函数会预先分配长度
	 *	预先分配好的数据都是NULL
	 */
	void resize(uint32_t _size)
	{
		if(!_vec.empty())
			clear();

		_vec.resize(_size, NULL);
	}

	/*
	 *	读取数组指定位置的数据
	 *	对比grab接口,at接口只取得数据
	 *	不增加数据的引用计数
	 *	grab接口读取数据以后,增加引用计数
	 */
	WTSObject* at(uint32_t idx)
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		WTSObject* pRet = _vec.at(idx);
		return pRet;
	}

	uint32_t idxOf(WTSObject* obj)
	{
		if (obj == NULL)
			return -1;

		uint32_t idx = 0;
		auto it = _vec.begin();
		for (; it != _vec.end(); it++, idx++)
		{
			if (obj == (*it))
				return idx;
		}

		return -1;
	}

	template<typename T> 
	T* at(uint32_t idx)
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		WTSObject* pRet = _vec.at(idx);
		return static_cast<T*>(pRet);
	}

	/*
	 *	[]操作符重载
	 *	用法同at函数
	 */
	WTSObject* operator [](uint32_t idx)
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		WTSObject* pRet = _vec.at(idx);
		return pRet;
	}

	/*
	 *	读取数组指定位置的数据
	 *	增加引用计数
	 */
	WTSObject*	grab(uint32_t idx)
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		WTSObject* pRet = _vec.at(idx);
		if (pRet)
			pRet->retain();

		return pRet;
	}

	/*
	 *	数组末尾追加数据
	 *	数据自动增加引用计数
	 */
	void append(WTSObject* obj, bool bAutoRetain = true)
	{
		if (bAutoRetain && obj)
			obj->retain();

		_vec.emplace_back(obj);
	}

	/*
	 *	设置指定位置的数据
	 *	如果该位置已有数据,则释放掉
	 *	新数据引用计数增加
	 */
	void set(uint32_t idx, WTSObject* obj, bool bAutoRetain = true)
	{
		if(idx >= _vec.size() || obj == NULL)
			return;

		if(bAutoRetain)
			obj->retain();

		WTSObject* oldObj = _vec.at(idx);
		if(oldObj)
			oldObj->release();

		_vec[idx] = obj;
	}

	void append(WTSArray* ay)
	{
		if(ay == NULL)
			return;

		_vec.insert(_vec.end(), ay->_vec.begin(), ay->_vec.end());
		ay->_vec.clear();
	}

	/*
	 *	数组清空
	 *	数组内所有数据释放引用
	 */
	void clear()
	{
		{
			std::vector<WTSObject*>::iterator it = _vec.begin();
			for (; it != _vec.end(); it++)
			{
				WTSObject* obj = (*it);
				if (obj)
					obj->release();
			}
		}
		
		_vec.clear();
	}

	/*
	 *	释放数组对象,用法如WTSObject
	 *	不同的是,如果引用计数为1时
	 *	释放所有数据
	 */
	virtual void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch(...)
		{

		}
	}

	/*
	 *	取得数组对象起始位置的迭代器
	 */
	Iterator begin()
	{
		return _vec.begin();
	}

	ConstIterator begin() const
	{
		return _vec.begin();
	}

	ReverseIterator rbegin()
	{
		return _vec.rbegin();
	}

	ConstReverseIterator rbegin() const
	{
		return _vec.rbegin();
	}

	/*
	 *	取得数组对象末尾位置的迭代器
	 */
	Iterator end()
	{
		return _vec.end();
	}

	ConstIterator end() const
	{
		return _vec.end();
	}

	ReverseIterator rend()
	{
		return _vec.rend();
	}

	ConstReverseIterator rend() const
	{
		return _vec.rend();
	}

	void	sort(SortFunc func)
	{
		std::sort(_vec.begin(), _vec.end(), func);
	}

protected:
	WTSArray():_holding(false){}
	virtual ~WTSArray(){}

	std::vector<WTSObject*>	_vec;
	std::atomic<bool>		_holding;
};


/*
 *	map容器
 *	内部采用std:map实现
 *	模版类型为key类型
 *	数据使用WTSObject指针对象
 *	所有WTSObject的派生类都适用
 */
template <class T>
class WTSMap : public WTSObject
{
public:
	/*
	 *	容器迭代器的定义
	 */
	typedef typename std::map<T, WTSObject*>	_MyType;
	typedef typename _MyType::iterator			Iterator;
	typedef typename _MyType::const_iterator	ConstIterator;

	/*
	 *	创建map容器
	 */
	static WTSMap<T>*	create()
	{
		WTSMap<T>* pRet = new WTSMap<T>();
		return pRet;
	}

	/*
	 *	返回map容器的大小
	 */
	uint32_t size() const{ return (uint32_t)_map.size(); }

	/*
	 *	读取指定key对应的数据
	 *	不增加数据的引用计数
	 *	没有则返回NULL
	 */
	WTSObject* get(const T &_key)
	{
		Iterator it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		WTSObject* pRet = it->second;
		return pRet;
	}

	/*
	 *	[]操作符重载
	 *	用法同get函数
	 */
	WTSObject* operator[](const T &_key)
	{
		Iterator it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		WTSObject* pRet = it->second;
		return pRet;
	}

	/*
	 *	读取指定key对应的数据
	 *	增加数据的引用计数
	 *	没有则返回NULL
	 */
	WTSObject* grab(const T &_key)
	{
		Iterator it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		WTSObject* pRet = it->second;
		if (pRet)
			pRet->retain();

		return pRet;
	}

	/*
	 *	新增一个数据,并增加数据引用计数
	 *	如果key存在,则将原有数据释放
	 */
	void add(T _key, WTSObject* obj, bool bAutoRetain = true)
	{
		if(bAutoRetain && obj)
			obj->retain();

		WTSObject* pOldObj = NULL;
		Iterator it = _map.find(_key);
		if(it != _map.end())
		{
			pOldObj = it->second;
		}

		_map[_key] = obj;

		if (pOldObj) pOldObj->release();
	}

	/*
	 *	根据key删除一个数据
	 *	如果key存在,则对应数据引用计数-1
	 */
	void remove(T _key)
	{
		Iterator it = _map.find(_key);
		if(it != _map.end())
		{
			WTSObject* obj = it->second;
			_map.erase(it);
			if (obj) obj->release();
		}
	}

	/*
	 *	获取容器起始位置的迭代器
	 */
	Iterator begin()
	{
		return _map.begin();
	}

	ConstIterator begin() const
	{
		return _map.begin();
	}

	/*
	 *	获取容易末尾位置的迭代器
	 */
	Iterator end()
	{
		return _map.end();
	}

	ConstIterator end() const
	{
		return _map.end();
	}

	Iterator find(const T& key)
	{
		return _map.find(key);
	}

	ConstIterator find(const T& key) const
	{
		return _map.find(key);
	}

	void erase(ConstIterator it)
	{
		_map.erase(it);
	}

	Iterator lower_bound(const T& key)
	{
		 return _map.lower_bound(key);
	}

	ConstIterator lower_bound(const T& key) const
	{
		return _map.lower_bound(key);
	}

	Iterator upper_bound(const T& key)
	{
	 	 return _map.upper_bound(key);
	}
	 
	ConstIterator upper_bound(const T& key) const
	{
		return _map.upper_bound(key);
	}

	WTSObject* last() 
	{
		if(_map.empty())
			return NULL;
		
		return _map.rbegin()->second;
	}
	

	/*
	 *	清空容器
	 *	容器内所有数据引用计数-1
	 */
	void clear()
	{
		Iterator it = _map.begin();
		for(; it != _map.end(); it++)
		{
			it->second->release();
		}
		_map.clear();
	}

	/*
	 *	释放容器对象
	 *	如果容器引用计数为1,则清空所有数据
	 */
	virtual void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch(...)
		{

		}
	}

protected:
	WTSMap(){}
	~WTSMap(){}

	std::map<T, WTSObject*>	_map;
};

/*
 *	map容器
 *	内部采用std:map实现
 *	模版类型为key类型
 *	数据使用WTSObject指针对象
 *	所有WTSObject的派生类都适用
 */
template <typename T>
class WTSHashMap : public WTSObject
{
public:
	/*
	 *	容器迭代器的定义
	 */
	typedef tsl::robin_map<T, WTSObject*>		_MyType;
	typedef typename _MyType::const_iterator	ConstIterator;

	/*
	 *	创建map容器
	 */
	static WTSHashMap<T>*	create()
	{
		WTSHashMap<T>* pRet = new WTSHashMap<T>();
		return pRet;
	}

	/*
	 *	返回map容器的大小
	 */
	uint32_t size() const{return (uint32_t)_map.size();}

	/*
	 *	读取指定key对应的数据
	 *	不增加数据的引用计数
	 *	没有则返回NULL
	 */
	WTSObject* get(const T &_key)
	{
		auto it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		WTSObject* pRet = it->second;
		return pRet;
	}

	/*
	 *	读取指定key对应的数据
	 *	增加数据的引用计数
	 *	没有则返回NULL
	 */
	WTSObject* grab(const T &_key)
	{
		auto it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		WTSObject* pRet = it->second;
		pRet->retain();
		return pRet;
	}

	/*
	 *	新增一个数据,并增加数据引用计数
	 *	如果key存在,则将原有数据释放
	 */
	void add(const T &_key, WTSObject* obj, bool bAutoRetain = true)
	{
		if (bAutoRetain && obj)
			obj->retain();

		WTSObject* pOldObj = NULL;
		auto it = _map.find(_key);
		if (it != _map.end())
		{
			pOldObj = it->second;
		}

		_map[_key] = obj;

		if (pOldObj) pOldObj->release();
	}

	/*
	 *	根据key删除一个数据
	 *	如果key存在,则对应数据引用计数-1
	 */
	void remove(const T &_key)
	{
		auto it = _map.find(_key);
		if(it != _map.end())
		{
			it->second->release();
			_map.erase(it);
		}
	}

	/*
	 *	获取容器起始位置的迭代器
	 */
	ConstIterator begin() const
	{
		return _map.begin();
	}

	/*
	 *	获取容易末尾位置的迭代器
	 */
	ConstIterator end() const
	{
		return _map.end();
	}

	ConstIterator find(const T& key) const
	{
		return _map.find(key);
	}

	/*
	 *	清空容器
	 *	容器内所有数据引用计数-1
	 */
	void clear()
	{
		ConstIterator it = _map.begin();
		for(; it != _map.end(); it++)
		{
			it->second->release();
		}
		_map.clear();
	}

	/*
	 *	释放容器对象
	 *	如果容器引用计数为1,则清空所有数据
	 */
	virtual void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch (...)
		{

		}
	}

protected:
	WTSHashMap(){}
	virtual ~WTSHashMap(){}

	//std::unordered_map<T, WTSObject*>	_map;
	tsl::robin_map<T, WTSObject*>	_map;
};

//////////////////////////////////////////////////////////////////////////
//WTSQueue
class WTSQueue : public WTSObject
{
public:
	typedef std::deque<WTSObject*>::iterator Iterator;
	typedef std::deque<WTSObject*>::const_iterator ConstIterator;

	static WTSQueue* create()
	{
		WTSQueue* pRet = new WTSQueue();
		return pRet;
	}

	void pop()
	{
		_queue.pop_front();
	}

	void push(WTSObject* obj, bool bAutoRetain = true)
	{
		if (obj && bAutoRetain)
			obj->retain();

		_queue.emplace_back(obj);
	}

	WTSObject* front(bool bRetain = true)
	{
		if(_queue.empty())
			return NULL;

		WTSObject* obj = _queue.front();
		if(bRetain)
			obj->retain();

		return obj;
	}

	WTSObject* back(bool bRetain = true)
	{
		if(_queue.empty())
			return NULL;

		WTSObject* obj = _queue.back();
		if(bRetain)
			obj->retain();

		return obj;
	}

	uint32_t size() const{ return (uint32_t)_queue.size(); }

	bool	empty() const{return _queue.empty();}

	void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch (...)
		{

		}
	}

	void clear()
	{
		Iterator it = begin();
		for(; it != end(); it++)
		{
			(*it)->release();
		}
		_queue.clear();
	}

	/*
	 *	取得数组对象起始位置的迭代器
	 */
	Iterator begin()
	{
		return _queue.begin();
	}

	ConstIterator begin() const
	{
		return _queue.begin();
	}

	void swap(WTSQueue* right)
	{
		_queue.swap(right->_queue);
	}

	/*
	 *	取得数组对象末尾位置的迭代器
	 */
	Iterator end()
	{
		return _queue.end();
	}

	ConstIterator end() const
	{
		return _queue.end();
	}

protected:
	WTSQueue(){}
	virtual ~WTSQueue(){}

	std::deque<WTSObject*>	_queue;
};

NS_WTP_END