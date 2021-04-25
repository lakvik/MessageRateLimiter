#include "CppUnitTest.h"
#include "lakvik\FixedWindowRateLimiter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LakvikTests
{
	TEST_CLASS(FixedWindowRateLimiterTest)
	{
	public:

		TEST_METHOD(block)
		{
			lakvik::FixedWindowRateLimiter rateLimiter;
			Assert::AreEqual(false,rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(10))));
			rateLimiter.block();
			Assert::AreEqual(true, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(20))));
		}
		TEST_METHOD(unblockAndRemoveThrottleLimit)
		{
			lakvik::FixedWindowRateLimiter rateLimiter;
			Assert::AreEqual(false, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(10))));
			rateLimiter.unblockAndRemoveThrottleLimit();
			Assert::AreEqual(false, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(20))));
			Assert::AreEqual(false, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(30))));
		}
		TEST_METHOD(throttle)
		{
			lakvik::FixedWindowRateLimiter rateLimiter;
			rateLimiter.throttle(2);
			Assert::AreEqual(false, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(20))));
			Assert::AreEqual(false, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(30))));
			Assert::AreEqual(true, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(40))));
			Assert::AreEqual(true, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(1010))));
			Assert::AreEqual(false, rateLimiter.isDropped(lakvik::Timestamp(std::chrono::milliseconds(1021))));
		}
	};
}