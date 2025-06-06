#include "LoggingContext.h"

using namespace ost::log;

// ------------------------------------------------------------
// Statics
// ------------------------------------------------------------

LoggingContext* LoggingContext::_instancePtr;

LoggingContext& LoggingContext::GetInstance()
{
	return *_instancePtr;
}

void LoggingContext::Initialize()
{
	_instancePtr = new LoggingContext();
}

void LoggingContext::Uninitialize()
{
	delete _instancePtr;
}

// ------------------------------------------------------------
// Instance
// ------------------------------------------------------------

void LoggingContext::PushLogMessage(LogMessage&& msg)
{
	for (ILogListener* listener : _logListeners)
	{
		Flags<ELevel> listenerFlags{ listener->GetEnabledLevels() };

		if (listenerFlags.Has(msg.MsgLevel))
		{
			listener->NotifyMessage(msg);
		}
	}
}

void ost::log::LoggingContext::AddLogListener(ILogListener* listenerAddress)
{
	_logListeners.insert(listenerAddress);
}

void ost::log::LoggingContext::RemoveLogListener(ILogListener* listenerAddress)
{
	if (_logListeners.contains(listenerAddress))
	{
		_logListeners.erase(listenerAddress);
	}
}
