#include "lakvik\ReceivedMessage.h"
#include <ostream>
namespace lakvik
{
	ReceivedMessage::ReceivedMessage(Message && message, Timestamp recievedTimestamp)
		:Super(message)
		, m_receivedTimestamp(recievedTimestamp)
	{

	}
	Timestamp ReceivedMessage::getReceivedTimestamp() const
	{
		return m_receivedTimestamp;
	}
	std::ostream& operator<<(std::ostream& stream, const ReceivedMessage& value)
	{
		stream << "{\"super\":" << static_cast<const ReceivedMessage::Super&>(value);
		stream << ",\"received timestamp\":" << value.getReceivedTimestamp().time_since_epoch().count();
		return stream << '}';
	}
}