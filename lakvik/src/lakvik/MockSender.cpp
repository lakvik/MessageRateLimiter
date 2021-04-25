#include "lakvik\MockSender.h"
#include "lakvik\ReceivedMessage.h"
namespace lakvik
{
	MockSender::MockSender(const SenderID & senderID)
		:m_senderID(senderID)
	{
	}
	std::shared_ptr<ReceivedMessage> MockSender::get(Timestamp timestamp, const FunctionID & functionID, const MessageID & messageID)
	{
		return std::make_shared<ReceivedMessage>(Message(m_senderID, functionID, messageID), timestamp);
	}
}