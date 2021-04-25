#include "lakvik\SenderBasedTokenRateLimiter.h"


lakvik::SenderBasedTokenRateLimiter::SenderBasedTokenRateLimiter(ISenderBasedTokenProvider& tokenProvider, const SenderID& senderID)
	:m_tokenProvider(tokenProvider)
	,m_senderID(senderID)
{
}

bool lakvik::SenderBasedTokenRateLimiter::isDropped(Timestamp timestamp)
{
	if (m_tokens.m_expiry <= timestamp)
	{
		m_tokens = m_tokenProvider.request(m_senderID,timestamp);
	}
	switch (m_tokens.m_count)
	{
	case 0: return true;
	case ISenderBasedTokenProvider::Unlimited: return false;
	}
	--m_tokens.m_count;
	return false;
}
