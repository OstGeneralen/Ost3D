#pragma once
#include <format>

namespace ost
{
	namespace log
	{
		template<typename ... TArgs>
		std::string FormatString(std::string_view fmtStr, TArgs&&... fmtArgs)
		{
			return std::vformat(fmtStr, std::make_format_args(fmtArgs...));
		}

		extern std::string FormatLogTimeStamp(long long timeStamp);
	}
}