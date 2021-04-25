#include "lakvik\SenderBasedThrottle.h"
#include "lakvik\ReceivedMessage.h"
#include <cassert>
#include <mutex>
namespace lakvik
{
	SenderBasedThrottle::SenderBasedThrottle(OnMessagePass && onMessagePass, RateLimiterCreator&& rateLimiterCreator)
		:m_onMessagePass(std::move(onMessagePass))
		,m_rateLimiterCreator(std::move(rateLimiterCreator))
	{
	}
	void SenderBasedThrottle::push(std::shared_ptr<ReceivedMessage>&& message)
	{
		assert(message);
		if (getRateLimiter(message->getSenderID()).isDropped(message->getReceivedTimestamp()))
		{
			return;
		}
		OnMessagePassData data{ std::move(message) };
		m_onMessagePass(data);
	}
	void SenderBasedThrottle::push(std::vector<std::shared_ptr<ReceivedMessage>>& messages)
	{
		for (auto&& message : messages)
		{
			push(std::move(message));
		}
	}
	
	IRateLimiter& SenderBasedThrottle::getRateLimiter(const SenderID & senderID)
	{
		std::lock_guard<std::mutex> lock(m_senderIDToRateLimiterLock);
		auto& rateLimiter = m_senderIDToRateLimiter[senderID];
		if (!rateLimiter)
		{
			rateLimiter = m_rateLimiterCreator(RateLimiterCreatorData{senderID});
			assert(rateLimiter);
		}
		return *rateLimiter;
	}
}