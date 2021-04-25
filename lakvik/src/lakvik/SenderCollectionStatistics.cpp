#include "lakvik\SenderCollectionStatistics.h"
#include "lakvik\ReceivedMessage.h"
#include <ostream>
namespace lakvik
{
	void SenderCollectionStatistics::add(const ReceivedMessage & message)
	{
		std::lock_guard<std::mutex> lock(m_senderIDToStatisticsLock);
		auto& statistics = m_senderIDToStatistics[message.getSenderID()];
		if (!statistics)
		{
			statistics = std::make_unique<SenderStatistics>(message.getSenderID());
		}
		statistics->add(message);
	}
	SenderCollectionStatistics::Snapshot SenderCollectionStatistics::snapshot()
	{
		Snapshot result;
		std::lock_guard<std::mutex> lock(m_senderIDToStatisticsLock);
		for (auto& statistics : m_senderIDToStatistics)
		{
			statistics.second->snapshot();
			result[statistics.first].m_count = statistics.second->count();
		}
		return result;
	}
	std::ostream& operator<<(std::ostream& stream, const SenderCollectionStatistics::SenderStatisticsSnapshot& value)
	{
		return stream << "{\"count\":" << value.m_count << '}';
	}
}