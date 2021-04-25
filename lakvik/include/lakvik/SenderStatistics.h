#pragma once
#include "lakvik\SenderID.h"
#include <atomic>
namespace lakvik
{
	class ReceivedMessage;
	class SenderStatistics
	{
	public:
		explicit SenderStatistics(const SenderID& senderID);
		~SenderStatistics() = default;
		SenderStatistics(const SenderStatistics&) = delete;
		SenderStatistics& operator=(const SenderStatistics&) = delete;
		void add(const ReceivedMessage& message);
		void snapshot();
		unsigned count()const;
	private:
		SenderID m_senderID;
		std::atomic<unsigned> m_total = 0;
		unsigned m_lastTotal = 0;
		unsigned m_count = 0;
	};
}