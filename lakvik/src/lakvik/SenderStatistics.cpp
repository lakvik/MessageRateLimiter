#include "lakvik\SenderStatistics.h"
#include "lakvik\ReceivedMessage.h"
#include <cassert>
namespace lakvik
{
	SenderStatistics::SenderStatistics(const SenderID& senderID)
		:m_senderID(senderID)
	{

	}
	void SenderStatistics::add(const ReceivedMessage& message)
	{
		assert(m_senderID == message.getSenderID());
		++m_total;
	}
	void SenderStatistics::snapshot()
	{
		unsigned currentTotal = m_total.load();
		assert(m_lastTotal <= currentTotal);
		m_count = currentTotal - m_lastTotal;
		m_lastTotal = currentTotal;
	}
	unsigned SenderStatistics::count() const
	{
		return m_count;
	}
}