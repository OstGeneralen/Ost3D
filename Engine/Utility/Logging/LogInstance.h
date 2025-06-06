#pragma once
#include <Engine/Utility/Logging/LogLevel.h>
#include <string>

namespace ost
{
	namespace log
	{
		struct LogInstance
		{
			LogInstance(const std::string& instanceName);
			void Log(ELevel level, const std::string& msg);

			const std::string& GetInstanceName() const;

		private:
			const std::string _instanceName;
		};
	}
}