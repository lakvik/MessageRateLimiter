#pragma once
#include "lakvik\IControllableRateLimiter.h"
#include <mutex>
#include <limits>
namespace lakvik
{
	class FixedWindowRateLimiter : public IControllableRateLimiter
	{
	public:
		explicit FixedWindowRateLimiter(std::chrono::milliseconds window = std::chrono::seconds(1));
		~FixedWindowRateLimiter() = default;
		FixedWindowRateLimiter(const FixedWindowRateLimiter&) = delete;
		FixedWindowRateLimiter& operator=(const FixedWindowRateLimiter&) = delete;
		bool isDropped(Timestamp timestamp) override;
		void block()override;
		void throttle(unsigned maxMessagesPerSecond)override;
		void unblockAndRemoveThrottleLimit()override;
		bool isBlocked()const;
		bool isThrottled()const;
		unsigned getMaxMessagesPerWindow()const;
		std::chrono::milliseconds getWindow()const;
	private:
		std::chrono::milliseconds m_window;
		std::mutex m_lock;
		static const auto Unlimited = std::numeric_limits<unsigned>::max();
		unsigned m_maxMessagesPerWindow = Unlimited;
		bool m_windowEndValid = false;
		Timestamp m_windowEnd;
		unsigned m_passedCount = 0;
	};
}