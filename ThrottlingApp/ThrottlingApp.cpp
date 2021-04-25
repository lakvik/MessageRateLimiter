#include "lakvik\SenderBasedThrottle.h"
#include "lakvik\ReceivedMessage.h"
#include "lakvik\MockSenders.h"
#include "lakvik\SenderCollectionStatistics.h"
#include "lakvik\FixedWindowRateLimiter.h"
#include "lakvik\SenderBasedControllableRateLimiterControl.h"
#include "lakvik\print.h"
#include "lakvik\SenderBasedTokenRateLimiter.h"
#include "lakvik\SenderBasedTokenProvider.h"
#include <iostream>
#include <algorithm>
#include <iterator>

void windowBasedThrottling()
{
	lakvik::SenderCollectionStatistics stats;
	lakvik::SenderBasedControllableRateLimiterControl control([](const auto&) {
		return std::make_shared<lakvik::FixedWindowRateLimiter>(std::chrono::seconds(1));
	});
	lakvik::SenderBasedThrottle throttle([&](const auto& data)
	{
		stats.add(*data.m_message);
	}, [&](const auto& data) {
		return control.create(data.m_senderID);
	});
	lakvik::MockSenders mockSenders(5, 3, 4, [&](auto& data)
	{
		throttle.push(data.m_messages);
	});
	mockSenders.start();
	for (auto i = 0u; i < 10; ++i)
	{
		print(std::cout, stats.snapshot());
		std::cout << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void distributedTokenBasedThrottling()
{
	constexpr auto nodeCount = 10;
	lakvik::SenderCollectionStatistics stats;
	lakvik::SenderBasedTokenProvider tokenProvider;
	tokenProvider.setNodeCount(nodeCount);
	//tokenProvider.block("0");
	//tokenProvider.unblockAndRemoveThrottleLimit("0");
	//tokenProvider.throttle("0", 1);
	std::vector<std::unique_ptr<lakvik::SenderBasedThrottle>> throttles;
	std::generate_n(std::back_inserter(throttles),nodeCount, [&]()
	{
		return std::make_unique<lakvik::SenderBasedThrottle>([&](const auto& data)
		{
			stats.add(*data.m_message);
		}, [&](const auto& data) {
			return std::make_shared<lakvik::SenderBasedTokenRateLimiter>(tokenProvider, data.m_senderID);
		});
	});
	
	lakvik::MockSenders mockSenders(5, 3, 4, [&, i = 0u](auto& data)mutable
	{
		throttles[i]->push(data.m_messages);
		i = (i + 1) % nodeCount;
	});
	mockSenders.start();
	for (auto i = 0u; i < 10; ++i)
	{
		print(std::cout, stats.snapshot());
		std::cout << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main(int /*argc*/, char** /*argv*/)
{
	//windowBasedThrottling();
	distributedTokenBasedThrottling();
	return 0;
}