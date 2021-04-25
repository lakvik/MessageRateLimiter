#pragma once
#include "lakvik\Message.h"
#include "lakvik\Timestamp.h"
#include <iosfwd>
namespace lakvik
{

	class ReceivedMessage :public Message
	{
	public:
		using Super = Message;
		ReceivedMessage(Message&& message, Timestamp recievedTimestamp);
		~ReceivedMessage() override = default;
		ReceivedMessage(const ReceivedMessage&) = default;
		ReceivedMessage(ReceivedMessage&&) = default;
		ReceivedMessage& operator=(const ReceivedMessage&) = default;
		ReceivedMessage& operator=(ReceivedMessage&&) = default;
		Timestamp getReceivedTimestamp()const;
	private:
		Timestamp m_receivedTimestamp;
	};
	std::ostream& operator<<(std::ostream& stream, const ReceivedMessage& value);
}
