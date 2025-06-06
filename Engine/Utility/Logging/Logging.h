#pragma once
#include <Engine/Utility/Logging/LogInstance.h>
#include <Engine/Utility/Logging/LogFormatting.h>
#include <Engine/Utility/Logging/LoggingContext.h>

#define STATIC_LOG( LogName ) ost::StaticLogType LogName{ #LogName }

namespace ost
{
	struct StaticLogType
	{
	public:
		StaticLogType(const char* logInstanceName) : _instance{ logInstanceName } {}

		template<typename ... TArgs> void LOG(const char* fmtMsg, TArgs&&... fmtArgs)
		{
			_instance.Log(ost::log::ELevel::Message, ost::log::FormatString(fmtMsg, fmtArgs...));
		}

		template<typename ... TArgs> void LOG_VERBOSE(const char* fmtMsg, TArgs&&... fmtArgs)
		{
			_instance.Log(ost::log::ELevel::Verbose, ost::log::FormatString(fmtMsg, fmtArgs...));
		}

		template<typename ... TArgs> void LOG_INFO(const char* fmtMsg, TArgs&&... fmtArgs)
		{
			_instance.Log(ost::log::ELevel::Info, ost::log::FormatString(fmtMsg, fmtArgs...));
		}

		template<typename ... TArgs> void LOG_WARNING(const char* fmtMsg, TArgs&&... fmtArgs)
		{
			_instance.Log(ost::log::ELevel::Warning, ost::log::FormatString(fmtMsg, fmtArgs...));
		}

		template<typename ... TArgs> void LOG_ERROR(const char* fmtMsg, TArgs&&... fmtArgs)
		{
			_instance.Log(ost::log::ELevel::Error, ost::log::FormatString(fmtMsg, fmtArgs...));
		}

		template<typename ... TArgs> void LOG_CONFIRM(const char* fmtMsg, TArgs&&... fmtArgs)
		{
			_instance.Log(ost::log::ELevel::Confirmation, ost::log::FormatString(fmtMsg, fmtArgs...));
		}
	private:
		log::LogInstance _instance;
	};
}