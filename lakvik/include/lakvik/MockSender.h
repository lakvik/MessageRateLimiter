#pragma once
#include "lakvik\SenderID.h"
#include "lakvik\MessageID.h"
#include "lakvik\FunctionID.h"
#include "lakvik\Timestamp.h"
#include <memory>
#include <functional>
namespace lakvik
{
	class ReceivedMessage;
	class MockSender
	{
	public:
		explicit MockSender(const SenderID& senderID);
		~MockSender() = default;
		MockSender(const MockSender&) = delete;
		MockSender& operator=(const MockSender&) = delete;
		std::shared_ptr<ReceivedMessage> get(Timestamp timestamp, const FunctionID& functionID, const MessageID& messageID);
	private:
		SenderID m_senderID;
	};
}