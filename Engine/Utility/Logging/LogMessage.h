#pragma once
#include <Engine/Utility/Logging/LogLevel.h>
#include <Engine/Utility/Logging/LogInstance.h>
#include <string>

namespace ost
{
	namespace log
	{
		struct LogMessage
		{
			LogMessage(ELevel msgLvl, const LogInstance* lInstPtr, long long ts, const std::string& msg);
			LogMessage() = default;
			LogMessage(const LogMessage&) = default;
			LogMessage(LogMessage&&) noexcept = default;

			LogMessage& operator=(const LogMessage&) = default;
			LogMessage& operator=(LogMessage&&) noexcept = default;

			std::string GetTimeStampString() const;

			ELevel MsgLevel;
			const LogInstance* LoggingInstancePtr;
			long long TimeStamp;
			std::string Message;
		};
	}
}