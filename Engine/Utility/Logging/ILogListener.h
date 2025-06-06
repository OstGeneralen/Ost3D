#pragma once
#include <Engine/Utility/Logging/LogMessage.h>

namespace ost
{
	namespace log
	{
		class ILogListener
		{
		public:
			virtual void NotifyMessage(const LogMessage& msg) = 0;
			ELevel GetEnabledLevels() const { return _levelFlags; }
		
		protected:
			void EnableLogLevels(ELevel level) 
			{
				_levelFlags = _levelFlags | level;
			}
			void DisableLogLevels(ELevel level) 
			{
				_levelFlags = _levelFlags & ~level;
			}

			ELevel _levelFlags = ELevel::All;
		};
	}
}