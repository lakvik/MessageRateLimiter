#pragma once
#include "lakvik\SenderID.h"
namespace lakvik
{
	class ISenderBasedRateControl
	{
	public:
		virtual ~ISenderBasedRateControl() = default;
		virtual void block(const SenderID& senderID)= 0;
		virtual void throttle(const SenderID& senderID, unsigned maxMessagesPerSecond) = 0;
		virtual void unblockAndRemoveThrottleLimit(const SenderID& senderID) = 0;
	};
}
