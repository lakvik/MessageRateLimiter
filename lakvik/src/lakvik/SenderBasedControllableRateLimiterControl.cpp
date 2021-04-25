#include "lakvik\SenderBasedControllableRateLimiterControl.h"
#include "lakvik\IControllableRateLimiter.h"
#include <cassert>
namespace lakvik
{
	SenderBasedControllableRateLimiterControl::SenderBasedControllableRateLimiterControl(RateLimiterCreator && rateLimiterCreator)
		:m_rateLimiterCreator(rateLimiterCreator)
	{
	}
	void SenderBasedControllableRateLimiterControl::block(const SenderID & senderID)
	{
		getRateLimiter(senderID).block();
	}
	void SenderBasedControllableRateLimiterControl::throttle(const SenderID & senderID, unsigned maxMessagesPerSecond)
	{
		getRateLimiter(senderID).throttle(maxMessagesPerSecond);
	}
	void SenderBasedControllableRateLimiterControl::unblockAndRemoveThrottleLimit(const SenderID & senderID)
	{
		getRateLimiter(senderID).unblockAndRemoveThrottleLimit();
	}
	std::shared_ptr<IControllableRateLimiter> SenderBasedControllableRateLimiterControl::create(const SenderID & senderID)
	{
		return m_rateLimiterCreator(RateLimiterCreatorData{ senderID });
	}
	IControllableRateLimiter& SenderBasedControllableRateLimiterControl::getRateLimiter(const SenderID & senderID)
	{
		std::lock_guard<std::mutex> lock(m_senderIDToRateLimiterLock);
		auto& rateLimiter = m_senderIDToRateLimiter[senderID];
		if (!rateLimiter)
		{
			rateLimiter = m_rateLimiterCreator(RateLimiterCreatorData{ senderID });
			assert(rateLimiter);
		}
		return *rateLimiter;
	}
}