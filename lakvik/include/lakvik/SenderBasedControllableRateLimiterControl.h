#pragma once
#include "lakvik\ISenderBasedRateControl.h"
#include <memory>
#include <mutex>
#include <unordered_map>
namespace lakvik
{
	class IControllableRateLimiter;
	class SenderBasedControllableRateLimiterControl : public ISenderBasedRateControl
	{
	public:
		struct RateLimiterCreatorData
		{
			RateLimiterCreatorData() = delete;
			RateLimiterCreatorData(const RateLimiterCreatorData&) = default;
			RateLimiterCreatorData& operator=(const RateLimiterCreatorData&) = delete;
			RateLimiterCreatorData& operator=(RateLimiterCreatorData&&) = delete;
			const SenderID& m_senderID;
		};
		using RateLimiterCreator = std::function<std::shared_ptr<IControllableRateLimiter>(const RateLimiterCreatorData&)>;
		explicit SenderBasedControllableRateLimiterControl(RateLimiterCreator&& rateLimiterCreator);
		~SenderBasedControllableRateLimiterControl() = default;
		SenderBasedControllableRateLimiterControl(const SenderBasedControllableRateLimiterControl&) = delete;
		SenderBasedControllableRateLimiterControl& operator=(const SenderBasedControllableRateLimiterControl&) = delete;
		void block(const SenderID& senderID) override;
		void throttle(const SenderID& senderID, unsigned maxMessagesPerSecond) override;
		void unblockAndRemoveThrottleLimit(const SenderID& senderID) override;
		std::shared_ptr<IControllableRateLimiter> create(const SenderID& senderID);
	private:
		IControllableRateLimiter& getRateLimiter(const SenderID & senderID);
		RateLimiterCreator m_rateLimiterCreator;
		std::mutex m_senderIDToRateLimiterLock;
		std::unordered_map<SenderID, std::shared_ptr<IControllableRateLimiter>> m_senderIDToRateLimiter;
	};
}
