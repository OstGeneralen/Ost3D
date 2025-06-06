#pragma once
#include <Engine/Utility/Time/FPSTracker.h>
#include <Engine/Utility/Time/SignalTimer.h>

namespace ost
{
	namespace gui
	{
		class FrameDetailsGUI
		{
		public:
			FrameDetailsGUI();
			void BindFPSTracker(const FPSTracker& tracker);
			void Display();
		private:
			const FPSTracker* _fpsTracker{nullptr};
			SignalTimer _regraphTrackerTimer{ 0.25f, false };

			void RecordFPS();

			constexpr static size_t s_fpsRecordCount{ 20 };
			float _fpsRecords[s_fpsRecordCount]{ 0 };
			size_t _fpsRecordsHead{ 0 };
		};
	}
}