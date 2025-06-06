#pragma once
#include <Engine/Utility/Logging/LogMessage.h>
#include <Engine/Utility/Logging/ILogListener.h>
#include <unordered_set>

namespace ost
{
	namespace log
	{
		class LoggingContext
		{
		public:
			static LoggingContext& GetInstance();
			static void Initialize();
			static void Uninitialize();
		private:
			static LoggingContext* _instancePtr;

		public:
			void PushLogMessage(LogMessage&& msg);

			void AddLogListener(ILogListener* listenerAddress);
			void RemoveLogListener(ILogListener* listenerAddress);

		private:
			std::unordered_set<ILogListener*> _logListeners;
		};
	}
}