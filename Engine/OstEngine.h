#pragma once
#if _WIN32
#include <Windows.h>
#include <Engine/Rendering/Window/Window.h>
#include <Engine/Math/Vector/VectorsGeneric.h>
#endif
#include <Engine/Utility/Time/Timer.h>
#include <Engine/Utility/Time/FPSTracker.h>


namespace ost
{
	class OstEngine
	{
	public:
#if _WIN32
		OstEngine(HINSTANCE appHinstance, LPSTR cmd, int cmdShow);
#endif
		~OstEngine();
		void CreateAppWindow(const wchar_t* title, Dimensions windowDimensions);
		bool IsAppWindowOpen() const;

		void BeginFrame();
		void EndFrame();
	private:
		Window _appWindow;
		Timer _frameTimer;
		FPSTracker _fpsTracker{10, 0.5f};
	};
}