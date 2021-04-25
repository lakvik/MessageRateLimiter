#pragma once
#include "lakvik\ISenderBasedTokenProvider.h"
#include "lakvik\ISenderBasedRateControl.h"
#include <atomic>
#include <unordered_map>
#include <mutex>
namespace lakvik
{
	class SenderBasedTokenProvider : public ISenderBasedTokenProvider, public ISenderBasedRateControl
	{
	public:
		explicit SenderBasedTokenProvider(std::chrono::milliseconds window = std::chrono::seconds(1));
		~SenderBasedTokenProvider() override = default;
		SenderBasedTokenProvider(const SenderBasedTokenProvider&) = delete;
		SenderBasedTokenProvider& operator=(const SenderBasedTokenProvider&) = delete;
		void setNodeCount(unsigned nodeCount);
		Tokens request(const SenderID& senderID, Timestamp timestamp) override;
		void block(const SenderID& senderID) override;
		void throttle(const SenderID& senderID, unsigned maxMessagesPerSecond) override;
		void unblockAndRemoveThrottleLimit(const SenderID& senderID) override;
	private:
		std::chrono::milliseconds m_window;
		std::atomic<unsigned> m_nodeCount;
		std::mutex m_lock;
		struct SenderData
		{
			void block();
			void throttle(unsigned maxMessagesPerWindow);
			void unblockAndRemoveThrottleLimit();
			Tokens request(Timestamp timestamp,unsigned nodeCount, std::chrono::milliseconds window);
			unsigned m_maxMessagesPerWindow = Unlimited;
			Timestamp m_windowEnd;
			unsigned m_servedTokensCount = 0;
		};
		std::unordered_map<SenderID, SenderData> m_senderIDToData;
	};
}
