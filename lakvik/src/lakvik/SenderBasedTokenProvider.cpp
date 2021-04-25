#include "lakvik\SenderBasedTokenProvider.h"
namespace lakvik
{
	SenderBasedTokenProvider::SenderBasedTokenProvider(std::chrono::milliseconds window)
		:m_window(window)
	{
	}
	void SenderBasedTokenProvider::setNodeCount(unsigned nodeCount)
	{
		m_nodeCount = nodeCount;
	}
	SenderBasedTokenProvider::Tokens SenderBasedTokenProvider::request(const SenderID& senderID, Timestamp timestamp)
	{
		auto nodeCount = m_nodeCount.load();
		if (!nodeCount)
		{
			return Tokens(0, timestamp);
		}
		std::lock_guard<std::mutex> lock(m_lock);
		return m_senderIDToData[senderID].request(timestamp, nodeCount, m_window);
	}
	void SenderBasedTokenProvider::block(const SenderID & senderID)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_senderIDToData[senderID].block();
	}
	void SenderBasedTokenProvider::throttle(const SenderID & senderID, unsigned maxMessagesPerSecond)
	{
		auto maxMessagesPerWindow = static_cast<unsigned>((maxMessagesPerSecond * m_window) / std::chrono::seconds(1));
		std::lock_guard<std::mutex> lock(m_lock);
		m_senderIDToData[senderID].throttle(maxMessagesPerWindow);

	}
	void SenderBasedTokenProvider::unblockAndRemoveThrottleLimit(const SenderID & senderID)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_senderIDToData[senderID].unblockAndRemoveThrottleLimit();
	}
	void SenderBasedTokenProvider::SenderData::block()
	{
		m_maxMessagesPerWindow = 0;
	}
	void SenderBasedTokenProvider::SenderData::throttle(unsigned maxMessagesPerSecond)
	{
		m_maxMessagesPerWindow = maxMessagesPerSecond;
		m_servedTokensCount = 0;
		m_windowEnd = Timestamp();
	}
	void SenderBasedTokenProvider::SenderData::unblockAndRemoveThrottleLimit()
	{
		m_maxMessagesPerWindow = Unlimited;
	}
	SenderBasedTokenProvider::Tokens SenderBasedTokenProvider::SenderData::request(Timestamp timestamp, unsigned nodeCount, std::chrono::milliseconds window)
	{
		switch (m_maxMessagesPerWindow)
		{
		case 0: return Tokens(0, timestamp + window);
		case Unlimited: return Tokens(Unlimited, timestamp + window);
		}
		if (m_windowEnd < timestamp)
		{
			m_servedTokensCount = 0;
			m_windowEnd = timestamp + window;
		}
		auto messagesPerNodePerWindow = static_cast<unsigned>(ceil((1.0 * m_maxMessagesPerWindow) / nodeCount));
		auto tokenCount = messagesPerNodePerWindow;
		if (m_maxMessagesPerWindow < m_servedTokensCount + tokenCount)
		{
			if (m_maxMessagesPerWindow < m_servedTokensCount)
			{
				tokenCount = 0;
			}
			else
			{
				tokenCount = m_maxMessagesPerWindow - m_servedTokensCount;
			}
		}
		m_servedTokensCount += tokenCount;
		return Tokens(tokenCount, m_windowEnd);
	}
}