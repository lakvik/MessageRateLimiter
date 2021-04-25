#pragma once
#include "lakvik\SenderID.h"
#include "lakvik\IRateLimiter.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <mutex>
namespace lakvik
{
	class ReceivedMessage;
	class SenderBasedThrottle
	{
	public:
		struct OnMessagePassData
		{
			std::shared_ptr<ReceivedMessage> m_message;
		};
		using OnMessagePass = std::function<void(OnMessagePassData&)>;
		struct RateLimiterCreatorData
		{
			RateLimiterCreatorData() = delete;
			RateLimiterCreatorData(const RateLimiterCreatorData&) = default;
			RateLimiterCreatorData& operator=(const RateLimiterCreatorData&) = delete;
			RateLimiterCreatorData& operator=(RateLimiterCreatorData&&) = delete;
			const SenderID& m_senderID;
		};
		using RateLimiterCreator = std::function<std::shared_ptr<IRateLimiter>(const RateLimiterCreatorData&)>;
		explicit SenderBasedThrottle(OnMessagePass&& onMessagePass, RateLimiterCreator&& rateLimiterCreator);
		~SenderBasedThrottle() = default;
		SenderBasedThrottle(const SenderBasedThrottle&) = delete;
		SenderBasedThrottle& operator=(const SenderBasedThrottle&) = delete;
		void push(std::shared_ptr<ReceivedMessage>&& message);
		void push(std::vector<std::shared_ptr<ReceivedMessage>>& messages);
	private:
		IRateLimiter& getRateLimiter(const SenderID& senderID);
		OnMessagePass m_onMessagePass;
		RateLimiterCreator m_rateLimiterCreator;
		std::mutex m_senderIDToRateLimiterLock;
		std::unordered_map<SenderID, std::shared_ptr<IRateLimiter>> m_senderIDToRateLimiter;
	};
}
