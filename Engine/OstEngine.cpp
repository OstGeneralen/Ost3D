#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")

#include <Engine/EngineConfig.h>
#include "OstEngine.h"
#if _WIN32
#include "Rendering/WindowsInfo.h"
#endif


#include "Rendering/DX/RenderingBackend.h"
#include "Utility/Logging/Logging.h"
#include "Editor/GUIHandler.h"
#include "Utility/Time/Timer.h"

STATIC_LOG(EngineLog);

using namespace ost;

#if _WIN32
ost::OstEngine::OstEngine(HINSTANCE appHinstance, LPSTR cmd, int cmdShow)
	: _app{nullptr}
	, _appWindow{}
	, _renderingBackend{}
	, _guiHandler{}
{
	internal::WindowsInfo::appInstance = appHinstance;
	internal::WindowsInfo::cmdLine = cmd;
	internal::WindowsInfo::cmdShow = cmdShow;
}
#endif

void ost::OstEngine::InitializeAndRun(OstEngineApp& app)
{
	_app = &app;
	Initialize();
	Run();
	Shutdown();
}

void ost::OstEngine::Initialize()
{
	log::LoggingContext::Initialize();

	_appWindow.Create(_app->GetAppName().c_str(), {1920,1080});
	_renderingBackend.InitializeForWindow(_appWindow);
	_guiHandler.Init(_appWindow, _renderingBackend);

	_app->Start();
}

void ost::OstEngine::Run()
{
	Timer deltaTimer;
	deltaTimer.Start();

	while (_appWindow.GetIsOpen())
	{
		deltaTimer.Stop();
		const float dt = deltaTimer.GetDurationSeconds();
		deltaTimer.Start();

		_appWindow.ProcessEvents();

		_renderingBackend.BeginFrame({ 0x1b1b1bFF });
		_guiHandler.BeginGuiFrame();

		_app->Tick(dt);

		_guiHandler.EndGuiFrame(_renderingBackend);
		_renderingBackend.EndAndPresentFrame();
	}
}

void ost::OstEngine::Shutdown()
{
	_app->Shutdown();
	_guiHandler.Uninit();
	_renderingBackend.Release();
}