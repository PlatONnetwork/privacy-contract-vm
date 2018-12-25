#pragma once

#include <map>
#include <mutex>


/// Concurrent queue.
/// You can push and pop elements to/from the queue. Pop will block until the queue is not empty.
/// The default backend (_QueueT) is std::queue. It can be changed to any type that has
/// proper push(), pop(), empty() and front() methods.
template<typename _T, typename _V, typename _MapT = std::map<_T, _V>>
class SafeMap
{
public:
	void insert(const _T& _key, const _V& _value)
	{
		std::lock_guard<decltype(x_mutex)> guard{ x_mutex };
		m_map[_key] = _value;

	}

	void insert(const _T& _key, _V& _value)
	{
		std::lock_guard<decltype(x_mutex)> guard{ x_mutex };
		m_map[_key] = _value;

	}

	int erase(const _T& _key, _V& _value)
	{
		std::unique_lock<std::mutex> lock{ x_mutex };

		auto iter = m_map.find(_key);
		if (iter != m_map.end())
		{
			_value = iter->second;
			m_map.erase(iter);
			return 0;
		}
		return -1;
	}
	int erase(const _T& _key)
	{
		std::unique_lock<std::mutex> lock{ x_mutex };

		auto iter = m_map.find(_key);
		if (iter != m_map.end())
		{
			m_map.erase(iter);
			return 0;
		}
		return -1;
	}

	int getElem(const _T& _key, _V& _value)
	{
		std::unique_lock<std::mutex> lock{ x_mutex };

		auto iter = m_map.find(_key);
		if (iter != m_map.end())
		{
			_value = iter->second;
			return 0;
		}

		return -1;
	}

	void clear()
	{
		std::unique_lock<std::mutex> lock{ x_mutex };
		m_map.clear();
	}

	void clear_delete()
	{
		std::unique_lock<std::mutex> lock{ x_mutex };
		auto iter = m_map.begin();
		for (; iter != m_map.end(); ++iter)
		{
			delete iter->second;
			iter->second = nullptr;
		}
		m_map.clear();
	}

	//给外部强制notify, 用于资源销毁， 防止线程推出等待
	void notify_all()
	{
	}

private:
	_MapT m_map;
	std::mutex x_mutex;
};


