#pragma once
#include "lakvik\SenderStatistics.h"
#include "lakvik\SenderID.h"
#include <unordered_map>
#include <mutex>
#include <iosfwd>
namespace lakvik
{
	class ReceivedMessage;
	class SenderCollectionStatistics
	{
	public:
		SenderCollectionStatistics() = default;
		~SenderCollectionStatistics() = default;
		SenderCollectionStatistics(const SenderCollectionStatistics&) = delete;
		SenderCollectionStatistics& operator=(const SenderCollectionStatistics&) = delete;
		void add(const ReceivedMessage& message);
		struct SenderStatisticsSnapshot
		{
			unsigned m_count;
			friend std::ostream& operator<<(std::ostream& stream, const SenderStatisticsSnapshot& value);
		};
		using Snapshot = std::unordered_map<SenderID, SenderStatisticsSnapshot>;
		Snapshot snapshot();
	private:
		std::mutex m_senderIDToStatisticsLock;
		std::unordered_map<SenderID, std::unique_ptr<SenderStatistics>> m_senderIDToStatistics;
	};
}
