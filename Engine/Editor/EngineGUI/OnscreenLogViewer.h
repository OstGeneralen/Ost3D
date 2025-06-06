#pragma once
#include <Engine/OstEngineTypedefs.h>
#include <Engine/Utility/Logging/ILogListener.h>
#include <vector>

namespace ost
{
	namespace gui
	{
		class OnscreenLogViewer : public log::ILogListener
		{
		public:
			OnscreenLogViewer(uint32 visibleMessages);
			~OnscreenLogViewer();

			void Initialize();
			void Uninitialize();

			void Display();

			void NotifyMessage(const log::LogMessage& msg) override;
		private:
			std::vector<log::LogMessage> _displayMessages;
			int32 _messageCount;
		};
	}
}