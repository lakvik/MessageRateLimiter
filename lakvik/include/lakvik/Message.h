#pragma once
#include "lakvik\SenderID.h"
#include "lakvik\FunctionID.h"
#include "lakvik\MessageID.h"
#include <iosfwd>
namespace lakvik
{
	class Message
	{
	public:
		Message(const SenderID& senderID, const FunctionID& functionID, const MessageID& messageID);
		virtual ~Message() = default;
		Message(const Message&) = default;
		Message(Message&&) = default;
		Message& operator=(const Message&) = default;
		Message& operator=(Message&&) = default;
		const SenderID& getSenderID()const;
		const FunctionID& getFunctionID()const;
		const MessageID& getMessageID()const;
	private:
		SenderID m_senderID;
		FunctionID m_functionID;
		MessageID m_messageID;
	};
	std::ostream& operator<<(std::ostream& stream, const Message& value);
}
