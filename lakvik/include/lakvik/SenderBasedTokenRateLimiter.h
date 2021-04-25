#pragma once
#include "lakvik\SenderID.h"
#include "lakvik\IRateLimiter.h"
#include "lakvik\ISenderBasedTokenProvider.h"
namespace lakvik
{
	class SenderBasedTokenRateLimiter :public IRateLimiter
	{
	public:
		SenderBasedTokenRateLimiter(ISenderBasedTokenProvider& tokenProvider,const SenderID& senderID);
		~SenderBasedTokenRateLimiter() override = default;
		SenderBasedTokenRateLimiter(const SenderBasedTokenRateLimiter&) = delete;
		SenderBasedTokenRateLimiter& operator=(const SenderBasedTokenRateLimiter&) = delete;
		bool isDropped(Timestamp timestamp) override;
	private:
		ISenderBasedTokenProvider& m_tokenProvider;
		SenderID m_senderID;
		ISenderBasedTokenProvider::Tokens m_tokens;
	};
}
