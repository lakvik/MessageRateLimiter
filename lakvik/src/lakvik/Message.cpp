#include "lakvik\Message.h"
namespace lakvik
{
	Message::Message(const SenderID& senderID, const FunctionID& functionID, const MessageID& messageID)
		:m_senderID(senderID)
		, m_functionID(functionID)
		, m_messageID(messageID)
	{

	}

	const SenderID& Message::getSenderID() const
	{
		return m_senderID;
	}

	const FunctionID& Message::getFunctionID() const
	{
		return m_functionID;
	}

	const MessageID& Message::getMessageID() const
	{
		return m_messageID;
	}

	std::ostream & operator<<(std::ostream & stream, const Message & value)
	{
		stream << "{\"sender ID\":\"" << value.getSenderID();
		stream << "\",\"function ID\":\"" << value.getFunctionID();
		stream << "\",\"message ID\":" << value.getMessageID();
		return stream << "\"}";
	}

}
