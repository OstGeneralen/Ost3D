#include "LogInstance.h"
#include "LogMessage.h"
#include "LoggingContext.h"
#include <chrono>
#include <Engine/Utility/Ops.h>

ost::log::LogInstance::LogInstance(const std::string& instanceName)
	: _instanceName{instanceName}
{
}

void ost::log::LogInstance::Log(ELevel level, const std::string& msg)
{
	// Construct message with current time point
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	time_t t  = std::chrono::system_clock::to_time_t(tp);
	LogMessage lm{ level, this, static_cast<long long>(t), msg };

	// Forward to the logging context
	LoggingContext::GetInstance().PushLogMessage(Move(lm));
}

const std::string& ost::log::LogInstance::GetInstanceName() const
{
	return _instanceName;
}
