#pragma once
#if _WIN32
#include <Windows.h>
#endif
#include <Engine/OstEngineApp.h>

#include <Engine/Rendering/Window/Window.h>
#include <Engine/Rendering/DX/RenderingBackend.h>
#include <Engine/Editor/GUIHandler.h>

namespace ost
{
	class OstEngine
	{
	public:
#if _WIN32
		OstEngine(HINSTANCE appHinstance, LPSTR cmd, int cmdShow);
#endif
		~OstEngine() = default;

		void InitializeAndRun(OstEngineApp& app);

	private:
		void Initialize();
		void Run();
		void Shutdown();

	private:
		OstEngineApp* _app;
		Window _appWindow;
		dx::RenderingBackend _renderingBackend;
		gui::GUIHandler _guiHandler;
	};
}