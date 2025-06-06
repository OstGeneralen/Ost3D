#pragma once
#if _WIN32
#include <Windows.h>
#include <Engine/Rendering/Window/Window.h>
#include <Engine/Math/Vector/VectorsGeneric.h>
#endif


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
	};
}