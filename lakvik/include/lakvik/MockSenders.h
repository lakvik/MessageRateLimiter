#pragma once
#include "lakvik\MockSender.h"
#include <memory>
#include <vector>
#include <functional>
#include <thread>
namespace lakvik
{
	class ReceivedMessage;
	class MockSenders
	{
	public:
		struct OnSendData
		{
			std::vector<std::shared_ptr<ReceivedMessage>> m_messages;
		};
		using OnSend = std::function<void(OnSendData&)>;
		MockSenders(unsigned senderCount, unsigned functionCount, unsigned messagesPerSecondPerSender, OnSend&& onSend);
		~MockSenders();
		MockSenders(const MockSenders&) = delete;
		MockSenders& operator=(const MockSenders&) = delete;
		void start();
		void stop();
	private:
		void send();
		unsigned m_functionCount;
		unsigned m_messagesPerSecondPerSender;
		OnSend m_onSend;
		bool m_started = false;
		bool m_stop = false;
		std::vector<std::unique_ptr<MockSender>> m_mockSenders;
		std::thread m_sender;
	};
}
