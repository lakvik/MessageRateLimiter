#include "lakvik\MockSenders.h"
#include <algorithm>
#include <iterator>
namespace
{
	lakvik::MessageID toMessageID(unsigned senderID, unsigned messageID)
	{
		return std::to_string(senderID) + ':' + std::to_string(messageID);
	}
	lakvik::FunctionID toFunctionID(unsigned functionID)
	{
		return std::to_string(functionID);
	}
}
namespace lakvik
{
	MockSenders::MockSenders(unsigned senderCount, unsigned functionCount, unsigned messagesPerSecond, OnSend && onSend)
		:m_functionCount(functionCount)
		, m_messagesPerSecondPerSender(messagesPerSecond)
		, m_onSend(std::move(onSend))
	{
		m_mockSenders.reserve(senderCount);
		std::generate_n(std::back_inserter(m_mockSenders), senderCount, [i = 0u]() mutable
		{
			return std::make_unique<MockSender>(std::to_string(i++));
		});
	}
	MockSenders::~MockSenders()
	{
		m_stop = true;
		if (!m_sender.joinable())
		{
			return;
		}
		m_sender.join();
	}
	void MockSenders::start()
	{
		if (m_started)
		{
			return;
		}
		m_sender = std::thread([this]()
		{
			send();
		});
		m_started = true;
	}
	void MockSenders::stop()
	{
		m_stop = true;
	}
	void MockSenders::send()
	{
		Timestamp current;
		auto step = Timestamp::duration(std::chrono::seconds(1)) / m_messagesPerSecondPerSender;
		auto nextFunctionID = 0u;
		auto nextMessageID = 0u;
		OnSendData data;
		auto senderCount = m_mockSenders.size();
		data.m_messages.resize(senderCount);
		while (!m_stop)
		{
			for (auto i = 0u; i < senderCount; ++i)
			{
				if (nextFunctionID == m_functionCount)
				{
					nextFunctionID = 0;
				}
				data.m_messages[i] = m_mockSenders[i]->get(current, toFunctionID(nextFunctionID++), toMessageID(i,nextMessageID));
			}
			++nextMessageID;
			m_onSend(data);
			std::this_thread::sleep_for(step);
			current += step;
		}
	}
}