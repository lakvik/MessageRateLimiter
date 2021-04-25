#pragma once
#include "lakvik\ITokenProvider.h"
#include "lakvik\SenderID.h"
namespace lakvik
{
	using ISenderBasedTokenProvider = ITokenProvider<const SenderID&,Timestamp>;
}