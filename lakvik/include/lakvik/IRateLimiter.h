#pragma once
#include "lakvik\Timestamp.h"
namespace lakvik
{
	class IRateLimiter
	{
	public:
		virtual ~IRateLimiter() = default;
		virtual bool isDropped(Timestamp timestamp) = 0;
	};
}
