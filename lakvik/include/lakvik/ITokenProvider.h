#pragma once
#include "lakvik\Timestamp.h"
namespace lakvik
{
	template<typename... Args>
	class ITokenProvider
	{
	public:
		virtual ~ITokenProvider() = default;
		struct Tokens
		{
			Tokens() = default;
			Tokens(unsigned count, Timestamp expiry)
				:m_count(count)
				,m_expiry(expiry)
			{

			}
			~Tokens() = default;
			Tokens(const Tokens&) = default;
			Tokens& operator=(const Tokens&) = default;
			unsigned m_count=0;
			Timestamp m_expiry;
		};
		static constexpr auto Unlimited = std::numeric_limits<unsigned>::max();
		virtual Tokens request(Args... data) = 0;
	};
}
