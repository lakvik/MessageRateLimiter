#include "lakvik\FixedWindowRateLimiter.h"
#include <cassert>
namespace lakvik
{
	FixedWindowRateLimiter::FixedWindowRateLimiter(std::chrono::milliseconds window)
		:m_window(window)
	{
		assert(0 < m_window.count());
	}
	bool FixedWindowRateLimiter::isDropped(Timestamp timestamp)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		switch (m_maxMessagesPerWindow)
		{
		case 0: return true;
		case Unlimited: return false;
		}
		if (!m_windowEndValid || m_windowEnd < timestamp)
		{
			m_windowEndValid = true;
			m_windowEnd = timestamp + m_window;
			m_passedCount = 1;
			return false;
		}
		if (m_maxMessagesPerWindow <= m_passedCount)
		{
			return true;
		}
		++m_passedCount;
		return false;
	}
	void FixedWindowRateLimiter::block()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_maxMessagesPerWindow = 0;
		m_windowEndValid = false;
	}
	void FixedWindowRateLimiter::throttle(unsigned maxMessagesPerSecond)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_maxMessagesPerWindow = static_cast<unsigned>((maxMessagesPerSecond * m_window)/std::chrono::seconds(1));
		m_windowEndValid = false;
	}
	void FixedWindowRateLimiter::unblockAndRemoveThrottleLimit()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_maxMessagesPerWindow = Unlimited;
		m_windowEndValid = false;
	}
	bool FixedWindowRateLimiter::isBlocked() const
	{
		return !m_maxMessagesPerWindow;
	}
	bool FixedWindowRateLimiter::isThrottled() const
	{
		return 0 < m_maxMessagesPerWindow && m_maxMessagesPerWindow != Unlimited;
	}
	unsigned FixedWindowRateLimiter::getMaxMessagesPerWindow() const
	{
		assert(m_maxMessagesPerWindow && m_maxMessagesPerWindow != Unlimited);
		return m_maxMessagesPerWindow;
	}
	std::chrono::milliseconds FixedWindowRateLimiter::getWindow() const
	{
		return m_window;
	}
}