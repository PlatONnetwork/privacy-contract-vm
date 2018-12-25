#pragma once

#include <utility>
#include <queue>
#include <condition_variable>
#include <mutex>


/// Concurrent queue.
/// You can push and pop elements to/from the queue. Pop will block until the queue is not empty.
/// The default backend (_QueueT) is std::queue. It can be changed to any type that has
/// proper push(), pop(), empty() and front() methods.
template<typename _T, typename _QueueT = std::queue<_T>>
class SafeQueue
{
public:
	void push(_T&& _elem)
	{
		{
			std::lock_guard<decltype(x_mutex)> guard{ x_mutex };
			//m_queue.push(std::forward<_U>(_elem));
			m_queue.push(std::move(_elem));
		}
		m_cv.notify_one();
	}

	void push(const _T& _elem)
	{
		{
			std::lock_guard<decltype(x_mutex)> guard{ x_mutex };
			m_queue.push(_elem);
		}
		m_cv.notify_one();
	}


	int pop(_T& _elem, long timeout = -1)
	{
		std::unique_lock<std::mutex> lock{ x_mutex };
		if (timeout <= 0) {
			m_cv.wait(lock, [this] { return !m_queue.empty() || m_exited; });
		}
		else {
			m_cv.wait_for(lock, std::chrono::milliseconds(timeout), [this] { return !m_queue.empty() || m_exited; });
		}

		if (m_exited)
			return -1;

		if (m_queue.empty()) {
			return -2;
		}

		_elem = m_queue.front();
		m_queue.pop();
		return 0;
	}

	void clear()
	{
		std::unique_lock<std::mutex> lock{ x_mutex };
		//m_cv.wait(lock, [this]{ return !m_queue.empty(); });
		while (!m_queue.empty())
			m_queue.pop();
	}

	//给外部强制notify, 用于资源销毁， 防止线程推出等待
	void notify_exit()
	{
		m_cv.notify_all();
		m_exited = true;
	}

	size_t size() {
		return m_queue.size();
	}


private:
	volatile bool m_exited = false;
	_QueueT m_queue;
	std::mutex x_mutex;
	std::condition_variable m_cv;
};

