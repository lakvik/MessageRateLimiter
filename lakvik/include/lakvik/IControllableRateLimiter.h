#pragma once
#include "lakvik\IRateLimiter.h"
namespace lakvik
{
	class IControllableRateLimiter : public IRateLimiter
	{
	public:
		~IControllableRateLimiter() override = default;
		virtual void block() = 0;
		virtual void throttle(unsigned maxMessagesPerSecond) = 0;
		virtual void unblockAndRemoveThrottleLimit() = 0;
	};
}