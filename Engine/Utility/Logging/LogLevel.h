#pragma once
#include <Engine/Utility/EnumFlag.h>

namespace ost
{
	namespace log
	{
		enum class ELevel
		{
			Verbose			= 1 << 0,
			Message			= 1 << 1,
			Info			= 1 << 2,
			Warning			= 1 << 3,
			Error			= 1 << 4,
			Confirmation	= 1 << 5,

			All				= Verbose | Message | Info | Warning | Error | Confirmation,
			None			= 0
		};
	}
}

IS_FLAGS(ost::log::ELevel);